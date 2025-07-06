#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <trantor/net/TcpServer.h>
#include <trantor/net/EventLoop.h>
#include <trantor/net/InetAddress.h>
#include <memory>
#include <vector>
#include <functional>

namespace drogon
{
class HttpRequestImpl;
class HttpRequestParser;
class WebSocketConnectionImpl;

class HttpServer
{
  public:
    using TcpConnectionPtr = trantor::TcpConnectionPtr;
    using HttpRequestPtr = HttpRequestPtr;
    using HttpResponsePtr = HttpResponsePtr;
    using HttpRequestImplPtr = std::shared_ptr<HttpRequestImpl>;
    using HttpRequestParserPtr = std::shared_ptr<HttpRequestParser>;
    using WebSocketConnectionImplPtr = std::shared_ptr<WebSocketConnectionImpl>;

    HttpServer(EventLoop *loop,
               const trantor::InetAddress &listenAddr,
               const std::string &name,
               const std::vector<std::function<HttpResponsePtr(const HttpRequestPtr &)>> &syncAdvices,
               const std::vector<std::function<void(const HttpRequestPtr &, const HttpResponsePtr &)>> &preSendingAdvices);

    ~HttpServer();

    void start();
    void stop();

  private:
    void onConnection(const TcpConnectionPtr &conn);
    void onMessage(const TcpConnectionPtr &conn, trantor::MsgBuffer *buf);
    void onRequests(const TcpConnectionPtr &conn,
                    const std::vector<HttpRequestImplPtr> &requests,
                    const HttpRequestParserPtr &requestParser);
    void sendResponse(const TcpConnectionPtr &conn,
                      const HttpResponsePtr &response,
                      bool isHeadMethod);
    void sendResponses(const TcpConnectionPtr &conn,
                       const std::vector<std::pair<HttpResponsePtr, bool>> &responses,
                       trantor::MsgBuffer &buffer);

    trantor::TcpServer server_;
    std::function<void(const HttpRequestPtr &, std::function<void(const HttpResponsePtr &)> &&)> httpAsyncCallback_;
    std::function<void(const HttpRequestPtr &, std::function<void(const HttpResponsePtr &)> &&, const WebSocketConnectionImplPtr &)> newWebsocketCallback_;
    std::function<void(const TcpConnectionPtr &)> connectionCallback_;
    std::vector<std::function<HttpResponsePtr(const HttpRequestPtr &)>> syncAdvices_;
    std::vector<std::function<void(const HttpRequestPtr &, const HttpResponsePtr &)>> preSendingAdvices_;
};

}  // namespace drogon

#endif  // HTTPSERVER_HPP