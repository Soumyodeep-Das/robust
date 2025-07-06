#ifndef WEBSOCKETCONNECTIONIMPL_HPP
#define WEBSOCKETCONNECTIONIMPL_HPP

#include <string>
#include <chrono>
#include <memory>
#include <atomic>
#include <vector>
#include "trantor/net/TcpConnection.h"
#include "trantor/net/InetAddress.h"
#include "trantor/net/EventLoop.h"
#include "trantor/net/TimerId.h"
#include "WebSocketConnectionImpl_fwd.h"

namespace drogon
{

enum class WebSocketMessageType
{
    Text,
    Binary,
    Close,
    Ping,
    Pong,
    Unknown
};

enum class CloseCode : uint16_t;

class WebSocketConnectionImpl : public std::enable_shared_from_this<WebSocketConnectionImpl>
{
  public:
    WebSocketConnectionImpl(const trantor::TcpConnectionPtr &conn, bool isServer);
    ~WebSocketConnectionImpl();

    void send(const char *msg, uint64_t len, const WebSocketMessageType type);
    void send(const std::string &msg, const WebSocketMessageType type);

    const trantor::InetAddress &localAddr() const;
    const trantor::InetAddress &peerAddr() const;

    bool connected() const;
    bool disconnected() const;

    void shutdown(const CloseCode code = CloseCode::NormalClosure, const std::string &reason = std::string());
    void forceClose();

    void setPingMessage(const std::string &message, const std::chrono::duration<double> &interval);
    void disablePing();

    void onNewMessage(const trantor::TcpConnectionPtr &connPtr, trantor::MsgBuffer *buffer);

  private:
    void sendWsData(const char *msg, uint64_t len, unsigned char opcode);
    void disablePingInLoop();
    void setPingMessageInLoop(std::string &&message, const std::chrono::duration<double> &interval);

    trantor::TcpConnectionPtr tcpConnectionPtr_;
    trantor::InetAddress localAddr_;
    trantor::InetAddress peerAddr_;
    bool isServer_;
    std::atomic<bool> usingMask_;
    std::vector<int> masks_;
    trantor::TimerId pingTimerId_{trantor::InvalidTimerId};

    class WebSocketMessageParser
    {
      public:
        bool parse(trantor::MsgBuffer *buffer);
        bool gotAll(std::string &message, WebSocketMessageType &type) const
        {
            if (gotAll_)
            {
                message = message_;
                type = type_;
                return true;
            }
            return false;
        }

      private:
        bool gotAll_{false};
        std::string message_;
        WebSocketMessageType type_{WebSocketMessageType::Unknown};
    } parser_;

    std::function<void(std::string &&, std::shared_ptr<WebSocketConnectionImpl>, WebSocketMessageType)> messageCallback_;
};

}  // namespace drogon

#endif  // WEBSOCKETCONNECTIONIMPL_HPP