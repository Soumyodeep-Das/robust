#ifndef HTTPRESPONSEPARSER_HPP
#define HTTPRESPONSEPARSER_HPP

#include <memory>
#include <string>
#include <functional>
#include <trantor/net/TcpConnection.h>
#include <trantor/utils/MsgBuffer.h>
#include "HttpResponseImpl.h"

namespace drogon
{
enum class HttpResponseParseStatus
{
    kExpectResponseLine,
    kExpectHeaders,
    kExpectBody,
    kExpectClose,
    kExpectChunkLen,
    kExpectChunkBody,
    kExpectLastEmptyChunk,
    kGotAll
};

class HttpResponseParser
{
  public:
    explicit HttpResponseParser(const trantor::TcpConnectionPtr &connPtr);
    void reset();
    bool processResponseLine(const char *begin, const char *end);
    bool parseResponseOnClose();
    bool parseResponse(trantor::MsgBuffer *buf);

  private:
    HttpResponseParseStatus status_;
    std::shared_ptr<HttpResponseImpl> responsePtr_;
    std::weak_ptr<trantor::TcpConnection> conn_;
    bool parseResponseForHeadMethod_{false};
    size_t leftBodyLength_{0};
    size_t currentChunkLength_{0};
};

}  // namespace drogon

#endif  // HTTPRESPONSEPARSER_HPP