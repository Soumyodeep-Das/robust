#include <gtest/gtest.h>
#include "WebSocketClient.hpp"

namespace
{
    // Since the connect method runs the event loop and connects to a real server,
    // we test that the method is callable without exceptions for a dummy server string.
    // Full integration tests would require a mock WebSocket server.

    TEST(WebSocketClientExampleTest, ConnectFunctionCallable)
    {
        // We test with a dummy server and path, expecting no exceptions thrown.
        // This test does not verify network behavior.
        EXPECT_NO_THROW(
            drogon::WebSocketClientExample::connect("ws://127.0.0.1", 8848, "/chat");
        );
    }

    TEST(WebSocketClientExampleTest, ConnectFunctionWithNoPort)
    {
        EXPECT_NO_THROW(
            drogon::WebSocketClientExample::connect("wss://echo.websocket.org", std::nullopt, "/");
        );
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}