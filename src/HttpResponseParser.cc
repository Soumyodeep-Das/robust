#include "HttpResponseParser.hpp"
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <string>
#include <trantor/utils/Logger.h>

using namespace trantor;
using namespace drogon;

HttpResponseParser::HttpResponseParser(const trantor::TcpConnectionPtr &connPtr)
    : status_(HttpResponseParseStatus::kExpectResponseLine),
      responsePtr_(new HttpResponseImpl),
      conn_(connPtr)
{
}

void HttpResponseParser::reset()
{
    status_ = HttpResponseParseStatus::kExpectResponseLine;
    responsePtr_.reset(new HttpResponseImpl);
    parseResponseForHeadMethod_ = false;
    leftBodyLength_ = 0;
    currentChunkLength_ = 0;
}

bool HttpResponseParser::processResponseLine(const char *begin, const char *end)
{
    const char *start = begin;
    const char *space = std::find(start, end, ' ');
    if (space != end)
    {
        LOG_TRACE << *(space - 1);
        if (*(space - 1) == '1')
        {
            responsePtr_->setVersion(Version::kHttp11);
        }
        else if (*(space - 1) == '0')
        {
            responsePtr_->setVersion(Version::kHttp10);
        }
        else
        {
            return false;
        }
    }

    start = space + 1;
    space = std::find(start, end, ' ');
    if (space != end)
    {
        std::string status_code(start, space - start);
        std::string status_message(space + 1, end - space - 1);
        LOG_TRACE << status_code << " " << status_message;
        auto code = atoi(status_code.c_str());
        responsePtr_->setStatusCode(HttpStatusCode(code));

        return true;
    }
    return false;
}

bool HttpResponseParser::parseResponseOnClose()
{
    if (status_ == HttpResponseParseStatus::kExpectClose)
    {
        status_ = HttpResponseParseStatus::kGotAll;
        return true;
    }
    return false;
}

bool HttpResponseParser::parseResponse(MsgBuffer *buf)
{
    bool ok = true;
    bool hasMore = true;
    while (hasMore)
    {
        if (status_ == HttpResponseParseStatus::kExpectResponseLine)
        {
            const char *crlf = buf->findCRLF();
            if (crlf)
            {
                ok = processResponseLine(buf->peek(), crlf);
                if (ok)
                {
                    buf->retrieveUntil(crlf + 2);
                    status_ = HttpResponseParseStatus::kExpectHeaders;
                }
                else
                {
                    hasMore = false;
                }
            }
            else
            {
                hasMore = false;
            }
        }
        else if (status_ == HttpResponseParseStatus::kExpectHeaders)
        {
            const char *crlf = buf->findCRLF();
            if (crlf)
            {
                const char *colon = std::find(buf->peek(), crlf, ':');
                if (colon != crlf)
                {
                    responsePtr_->addHeader(buf->peek(), colon, crlf);
                }
                else
                {
                    const std::string &len =
                        responsePtr_->getHeaderBy("content-length");
                    if (!len.empty())
                    {
                        leftBodyLength_ = atoi(len.c_str());
                        status_ = HttpResponseParseStatus::kExpectBody;
                    }
                    else
                    {
                        const std::string &encode =
                            responsePtr_->getHeaderBy("transfer-encoding");
                        if (encode == "chunked")
                        {
                            status_ = HttpResponseParseStatus::kExpectChunkLen;
                            hasMore = true;
                        }
                        else
                        {
                            if (responsePtr_->statusCode() == k204NoContent ||
                                (responsePtr_->statusCode() ==
                                         k101SwitchingProtocols &&
                                     [this]() -> bool {
                                    std::string upgradeValue =
                                        responsePtr_->getHeaderBy("upgrade");
                                    std::transform(upgradeValue.begin(),
                                                   upgradeValue.end(),
                                                   upgradeValue.begin(),
                                                   ::tolower);
                                    return upgradeValue == "websocket";
                                }()))
                            {
                                status_ = HttpResponseParseStatus::kGotAll;
                                hasMore = false;
                            }
                            else
                            {
                                status_ = HttpResponseParseStatus::kExpectClose;
                                auto connPtr = conn_.lock();
                                if (connPtr)
                                {
                                    connPtr->shutdown();
                                }
                                hasMore = true;
                            }
                        }
                    }
                    if (parseResponseForHeadMethod_)
                    {
                        leftBodyLength_ = 0;
                        status_ = HttpResponseParseStatus::kGotAll;
                        hasMore = false;
                    }
                }
                buf->retrieveUntil(crlf + 2);
            }
            else
            {
                hasMore = false;
            }
        }
        else if (status_ == HttpResponseParseStatus::kExpectBody)
        {
            if (buf->readableBytes() == 0)
            {
                if (leftBodyLength_ == 0)
                {
                    status_ = HttpResponseParseStatus::kGotAll;
                }
                break;
            }
            if (!responsePtr_->bodyPtr_)
            {
                responsePtr_->bodyPtr_ =
                    std::make_shared<HttpMessageStringBody>();
            }
            if (leftBodyLength_ >= buf->readableBytes())
            {
                leftBodyLength_ -= buf->readableBytes();

                responsePtr_->bodyPtr_->append(buf->peek(),
                                               buf->readableBytes());
                buf->retrieveAll();
            }
            else
            {
                responsePtr_->bodyPtr_->append(buf->peek(), leftBodyLength_);
                buf->retrieve(leftBodyLength_);
                leftBodyLength_ = 0;
            }
            if (leftBodyLength_ == 0)
            {
                status_ = HttpResponseParseStatus::kGotAll;
                LOG_TRACE << "post got all:len=" << leftBodyLength_;
                LOG_TRACE << "content(END)";
                hasMore = false;
            }
        }
        else if (status_ == HttpResponseParseStatus::kExpectClose)
        {
            if (!responsePtr_->bodyPtr_)
            {
                responsePtr_->bodyPtr_ =
                    std::make_shared<HttpMessageStringBody>();
            }
            responsePtr_->bodyPtr_->append(buf->peek(), buf->readableBytes());
            buf->retrieveAll();
            break;
        }
        else if (status_ == HttpResponseParseStatus::kExpectChunkLen)
        {
            const char *crlf = buf->findCRLF();
            if (crlf)
            {
                std::string len(buf->peek(), crlf - buf->peek());
                char *end;
                currentChunkLength_ = strtol(len.c_str(), &end, 16);
                if (currentChunkLength_ != 0)
                {
                    status_ = HttpResponseParseStatus::kExpectChunkBody;
                }
                else
                {
                    status_ = HttpResponseParseStatus::kExpectLastEmptyChunk;
                }
                buf->retrieveUntil(crlf + 2);
            }
            else
            {
                hasMore = false;
            }
        }
        else if (status_ == HttpResponseParseStatus::kExpectChunkBody)
        {
            if (buf->readableBytes() >= (currentChunkLength_ + 2))
            {
                if (*(buf->peek() + currentChunkLength_) == '\r' &&
                    *(buf->peek() + currentChunkLength_ + 1) == '\n')
                {
                    if (!responsePtr_->bodyPtr_)
                    {
                        responsePtr_->bodyPtr_ =
                            std::make_shared<HttpMessageStringBody>();
                    }
                    responsePtr_->bodyPtr_->append(buf->peek(),
                                                   currentChunkLength_);
                    buf->retrieve(currentChunkLength_ + 2);
                    currentChunkLength_ = 0;
                    status_ = HttpResponseParseStatus::kExpectChunkLen;
                }
                else
                {
                    buf->retrieveAll();
                    return false;
                }
            }
            else
            {
                hasMore = false;
            }
        }
        else if (status_ == HttpResponseParseStatus::kExpectLastEmptyChunk)
        {
            const char *crlf = buf->findCRLF();
            if (crlf)
            {
                buf->retrieveUntil(crlf + 2);
                status_ = HttpResponseParseStatus::kGotAll;
                responsePtr_->addHeader("content-length",
                                        std::to_string(
                                            responsePtr_->getBody().length()));
                responsePtr_->removeHeaderBy("transfer-encoding");
                break;
            }
            else
            {
                hasMore = false;
            }
        }
    }
    return ok;
}