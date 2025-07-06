#include "gtest/gtest.h"
#include "WebSocketTest.hpp"
#include <drogon/HttpRequest.h>
#include <drogon/WebSocketConnection.h>
#include <memory>

namespace example
{
class WebSocketTestTest : public ::testing::Test
{
protected:
    WebSocketTest wsTest;

    class MockWebSocketConnection : public drogon::WebSocketConnection
    {
    public:
        MockWebSocketConnection() : drogon::WebSocketConnection(nullptr) {}

        std::string lastSentMessage;
        std::shared_ptr<void> context;

        void send(const std::string &message) override
        {
            lastSentMessage = message;
        }

        template <typename T>
        T &getContextRef()
        {
            return *std::static_pointer_cast<T>(context);
        }

        void setContext(const std::shared_ptr<void> &ctx)
        {
            context = ctx;
        }
    };

    class MockHttpRequest : public drogon::HttpRequest
    {
    public:
        MockHttpRequest() : drogon::HttpRequest() {}

        std::string roomName;

        std::string getParameter(const std::string &key) const override
        {
            if (key == "room_name")
                return roomName;
            return "";
        }
    };
};

TEST_F(WebSocketTestTest, HandleNewConnection_SetsContextAndSendsMessage)
{
    MockHttpRequest req;
    req.roomName = "testRoom";
    MockWebSocketConnection conn;

    wsTest.handleNewConnection(std::make_shared<MockHttpRequest>(req),
                               std::make_shared<MockWebSocketConnection>(conn));

    // The context should be set with Subscriber having chatRoomName_ = "testRoom"
    auto &subscriber = conn.getContextRef<WebSocketTest::Subscriber>();
    EXPECT_EQ(subscriber.chatRoomName_, "testRoom");

    // The connection should have sent the welcome message "haha!!!"
    EXPECT_EQ(conn.lastSentMessage, "haha!!!");
}

TEST_F(WebSocketTestTest, HandleNewMessage_PingType_LogsPing)
{
    // Cannot easily test logs, but can test no crash and no publish call
    MockWebSocketConnection conn;
    WebSocketTest::Subscriber subscriber;
    subscriber.chatRoomName_ = "room1";
    subscriber.id_ = 1;
    conn.setContext(std::make_shared<WebSocketTest::Subscriber>(subscriber));

    // We call handleNewMessage with Ping type, expecting no exceptions
    EXPECT_NO_THROW(wsTest.handleNewMessage(std::make_shared<MockWebSocketConnection>(conn),
                                            std::string("ping message"),
                                            drogon::WebSocketMessageType::Ping));
}

TEST_F(WebSocketTestTest, HandleNewMessage_TextType_PublishesMessage)
{
    MockWebSocketConnection conn;
    WebSocketTest::Subscriber subscriber;
    subscriber.chatRoomName_ = "room1";
    subscriber.id_ = 1;
    conn.setContext(std::make_shared<WebSocketTest::Subscriber>(subscriber));

    // We call handleNewMessage with Text type, expecting no exceptions
    EXPECT_NO_THROW(wsTest.handleNewMessage(std::make_shared<MockWebSocketConnection>(conn),
                                            std::string("hello"),
                                            drogon::WebSocketMessageType::Text));
}

TEST_F(WebSocketTestTest, HandleConnectionClosed_Unsubscribes)
{
    MockWebSocketConnection conn;
    WebSocketTest::Subscriber subscriber;
    subscriber.chatRoomName_ = "room1";
    subscriber.id_ = 1;
    conn.setContext(std::make_shared<WebSocketTest::Subscriber>(subscriber));

    EXPECT_NO_THROW(wsTest.handleConnectionClosed(std::make_shared<MockWebSocketConnection>(conn)));
}