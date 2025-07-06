#ifndef WEBSOCKETTEST_HPP
#define WEBSOCKETTEST_HPP

#include <string>
#include <memory>
#include "drogon/WebSocketController.h"
#include "drogon/HttpRequest.h"

namespace example
{
class WebSocketTest : public drogon::WebSocketController<WebSocketTest>
{
public:
    void handleNewMessage(const drogon::WebSocketConnectionPtr &wsConnPtr,
                          std::string &&message,
                          const drogon::WebSocketMessageType &type);

    void handleConnectionClosed(const drogon::WebSocketConnectionPtr &conn);

    void handleNewConnection(const drogon::HttpRequestPtr &req,
                             const drogon::WebSocketConnectionPtr &conn);

private:
    struct Subscriber;
    drogon::SubscriberManager chatRooms_;
};

} // namespace example

#endif // WEBSOCKETTEST_HPP