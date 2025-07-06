#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP

#include <memory>
#include <deque>
#include <vector>
#include <string>
#include <utility>
#include <trantor/net/TcpConnection.h>
#include <trantor/net/EventLoop.h>
#include <trantor/utils/Date.h>
#include <drogon/HttpTypes.h>
#include <drogon/HttpRequestImpl.h>
#include <drogon/HttpResponse.h>
#include <trantor/utils/MsgBuffer.h>

namespace drogon
{
class HttpRequestParser : public std::enable_shared_from_this<HttpRequestParser>
{
  public:
    explicit HttpRequestParser(const trantor::TcpConnectionPtr &connPtr);

    void shutdownConnection(HttpStatusCode code);

    bool processRequestLine(const char *begin, const char *end);

    static HttpRequestImplPtr makeRequestForPool(HttpRequestImpl *ptr);

    void reset();

    // Return false if any error
    bool parseRequest(trantor::MsgBuffer *buf);

    void pushRequestToPipelining(const HttpRequestPtr &req);

    HttpRequestPtr getFirstRequest() const;

    std::pair<HttpResponsePtr, bool> getFirstResponse() const;

    void popFirstRequest();

    void pushResponseToPipelining(const HttpRequestPtr &req,
                                 const HttpResponsePtr &resp,
                                 bool isHeadMethod);

  private:
    HttpRequestParseStatus status_;
    trantor::EventLoop *loop_;
    std::weak_ptr<trantor::TcpConnection> conn_;
    HttpRequestImplPtr request_;
    std::vector<HttpRequestImplPtr> requestsPool_;
    size_t currentContentLength_{0};
    size_t currentChunkLength_{0};
    size_t requestsCounter_{0};
    std::deque<std::pair<HttpRequestPtr, std::pair<HttpResponsePtr, bool>>> requestPipelining_;
};

}  // namespace drogon

#endif  // HTTPREQUESTPARSER_HPP