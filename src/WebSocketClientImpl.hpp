#ifndef WEBSOCKETCLIENTIMPL_HPP
#define WEBSOCKETCLIENTIMPL_HPP

#include <memory>
#include <string>
#include <functional>
#include <chrono>
#include <trantor/net/InetAddress.h>
#include <trantor/net/TcpClient.h>
#include <drogon/HttpRequest.h>
#include <drogon/WebSocketConnection.h>
#include <drogon/WebSocketClient.h>

namespace drogon
{
class WebSocketClientImpl : public std::enable_shared_from_this<WebSocketClientImpl>
{
  public:
    using WebSocketRequestCallback = std::function<void(ReqResult, const HttpResponsePtr &, const std::shared_ptr<WebSocketClientImpl> &)>;
    ~WebSocketClientImpl();

    WebSocketConnectionPtr getConnection();

    void stop();

    void connectToServer(const HttpRequestPtr &request,
                         const WebSocketRequestCallback &callback);

  private:
    friend class WebSocketClient;

    WebSocketClientImpl(trantor::EventLoop *loop,
                        const trantor::InetAddress &addr,
                        bool useSSL,
                        bool useOldTLS,
                        bool validateCert);

    WebSocketClientImpl(trantor::EventLoop *loop,
                        const std::string &hostString,
                        bool useOldTLS,
                        bool validateCert);

    void createTcpClient();

    void connectToServerInLoop();

    void sendReq(const trantor::TcpConnectionPtr &connPtr);

    void onRecvMessage(const trantor::TcpConnectionPtr &connPtr,
                       trantor::MsgBuffer *msgBuffer);

    void onRecvWsMessage(const trantor::TcpConnectionPtr &connPtr,
                         trantor::MsgBuffer *msgBuffer);

    void reconnect();

    trantor::EventLoop *loop_;
    trantor::InetAddress serverAddr_;
    bool useSSL_{false};
    bool useOldTLS_{false};
    bool validateCert_{true};
    std::string domain_;
    bool stop_{false};
    bool upgraded_{false};

    std::shared_ptr<trantor::TcpClient> tcpClientPtr_;
    WebSocketConnectionPtr websockConnPtr_;
    HttpRequestPtr upgradeRequest_;
    WebSocketRequestCallback requestCallback_;
    std::function<void(const std::shared_ptr<WebSocketClientImpl> &)> connectionClosedCallback_;
    std::function<void(std::string &&, const std::shared_ptr<WebSocketClientImpl> &, const WebSocketMessageType &)> messageCallback_;

    std::string wsKey_;
    std::string wsAccept_;

    std::shared_ptr<trantor::Resolver> resolver_;
};

}  // namespace drogon

#endif  // WEBSOCKETCLIENTIMPL_HPP