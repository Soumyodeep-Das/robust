#ifndef WEBSOCKETSERVER_HPP
#define WEBSOCKETSERVER_HPP

#include <drogon/WebSocketController.h>
#include <drogon/PubSubService.h>
#include <drogon/HttpAppFramework.h>
#include <string>

class WebSocketChat : public drogon::WebSocketController<WebSocketChat>
{
  public:
    virtual void handleNewMessage(const drogon::WebSocketConnectionPtr &,
                                  std::string &&,
                                  const drogon::WebSocketMessageType &) override;
    virtual void handleConnectionClosed(
        const drogon::WebSocketConnectionPtr &) override;
    virtual void handleNewConnection(const drogon::HttpRequestPtr &,
                                     const drogon::WebSocketConnectionPtr &) override;
    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/chat", Get);
    WS_PATH_LIST_END
  private:
    drogon::PubSubService<std::string> chatRooms_;
};

struct Subscriber
{
    std::string chatRoomName_;
    drogon::SubscriberID id_;
};

#endif // WEBSOCKETSERVER_HPP