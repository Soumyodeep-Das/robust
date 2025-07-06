#include "WebSocketTest.hpp"
#include <drogon/drogon.h>

namespace example
{

struct WebSocketTest::Subscriber
{
    std::string chatRoomName_;
    drogon::SubscriberID id_;
};

void WebSocketTest::handleNewMessage(const drogon::WebSocketConnectionPtr &wsConnPtr,
                                     std::string &&message,
                                     const drogon::WebSocketMessageType &type)
{
    // write your application logic here
    LOG_DEBUG << "new websocket message:" << message;
    if (type == drogon::WebSocketMessageType::Ping)
    {
        LOG_DEBUG << "recv a ping";
    }
    else if (type == drogon::WebSocketMessageType::Text)
    {
        auto &s = wsConnPtr->getContextRef<Subscriber>();
        chatRooms_.publish(s.chatRoomName_, message);
    }
}

void WebSocketTest::handleConnectionClosed(const drogon::WebSocketConnectionPtr &conn)
{
    LOG_DEBUG << "websocket closed!";
    auto &s = conn->getContextRef<Subscriber>();
    chatRooms_.unsubscribe(s.chatRoomName_, s.id_);
}

void WebSocketTest::handleNewConnection(const drogon::HttpRequestPtr &req,
                                        const drogon::WebSocketConnectionPtr &conn)
{
    LOG_DEBUG << "new websocket connection!";
    conn->send("haha!!!");
    Subscriber s;
    s.chatRoomName_ = req->getParameter("room_name");
    s.id_ = chatRooms_.subscribe(s.chatRoomName_,
                                 [conn](const std::string &topic,
                                        const std::string &message) {
                                     conn->send(message);
                                 });
    conn->setContext(std::make_shared<Subscriber>(std::move(s)));
}

} // namespace example