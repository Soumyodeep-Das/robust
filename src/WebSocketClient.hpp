#ifndef WEBSOCKETCLIENT_HPP
#define WEBSOCKETCLIENT_HPP

#include <string>
#include <functional>
#include <memory>
#include <optional>
#include <drogon/WebSocketClient.h>
#include <drogon/HttpAppFramework.h>

namespace drogon
{
    using namespace std::chrono_literals;

    // Wrapper class to encapsulate the example functionality
    class WebSocketClientExample
    {
    public:
        // Connect to WebSocket server with given parameters
        static void connect(const std::string &server,
                            const std::optional<uint16_t> &port,
                            const std::string &path);
    };
}

#endif // WEBSOCKETCLIENT_HPP