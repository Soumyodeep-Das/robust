#include "WebSocketClient.hpp"
#include <iostream>

using namespace drogon;
using namespace std::chrono_literals;

void WebSocketClientExample::connect(const std::string &server,
                                     const std::optional<uint16_t> &port,
                                     const std::string &path)
{
    std::string serverString;
    if (port.value_or(0) != 0)
        serverString = server + ":" + std::to_string(port.value());
    else
        serverString = server;
    auto wsPtr = WebSocketClient::newWebSocketClient(serverString);
    auto req = HttpRequest::newHttpRequest();
    req->setPath(path);

    wsPtr->setMessageHandler([](const std::string &message,
                                const WebSocketClientPtr &,
                                const WebSocketMessageType &type) {
        std::string messageType = "Unknown";
        if (type == WebSocketMessageType::Text)
            messageType = "text";
        else if (type == WebSocketMessageType::Pong)
            messageType = "pong";
        else if (type == WebSocketMessageType::Ping)
            messageType = "ping";
        else if (type == WebSocketMessageType::Binary)
            messageType = "binary";
        else if (type == WebSocketMessageType::Close)
            messageType = "Close";

        LOG_INFO << "new message (" << messageType << "): " << message;
    });

    wsPtr->setConnectionClosedHandler([](const WebSocketClientPtr &) {
        LOG_INFO << "WebSocket connection closed!";
    });

    LOG_INFO << "Connecting to WebSocket at " << server;
    wsPtr->connectToServer(
        req,
        [](ReqResult r,
           const HttpResponsePtr &,
           const WebSocketClientPtr &wsPtr) {
            if (r != ReqResult::Ok)
            {
                LOG_ERROR << "Failed to establish WebSocket connection!";
                wsPtr->stop();
                return;
            }
            LOG_INFO << "WebSocket connected!";
            wsPtr->getConnection()->setPingMessage("", 2s);
            wsPtr->getConnection()->send("hello!");
        });

    // Quit the application after 15 seconds
    app().getLoop()->runAfter(15, []() { app().quit(); });

    app().setLogLevel(trantor::Logger::kDebug);
    app().run();
    LOG_INFO << "bye!";
}