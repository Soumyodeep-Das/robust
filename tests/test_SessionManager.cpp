#include "gtest/gtest.h"
#include "SessionManager.hpp"
#include <memory>

using namespace drogon;

class SessionManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        loop = nullptr;  // Using nullptr since EventLoop is complex to mock here
        timeout = 1;
        manager = std::make_unique<SessionManager>(loop, timeout);
    }

    trantor::EventLoop *loop;
    size_t timeout;
    std::unique_ptr<SessionManager> manager;
};

TEST_F(SessionManagerTest, ConstructorTimeoutZero)
{
    SessionManager managerZero(loop, 0);
    // No direct way to check internals, so just ensure no crash
    SUCCEED();
}

TEST_F(SessionManagerTest, GetSessionCreatesNewSession)
{
    auto session = manager->getSession("session1", true);
    ASSERT_NE(session, nullptr);
    EXPECT_EQ(session->sessionId(), "session1");
}

TEST_F(SessionManagerTest, GetSessionReturnsSameSession)
{
    auto session1 = manager->getSession("session2", true);
    auto session2 = manager->getSession("session2", false);
    ASSERT_EQ(session1, session2);
}

TEST_F(SessionManagerTest, ChangeSessionIdUpdatesSessionId)
{
    auto session = manager->getSession("session3", true);
    auto oldId = session->sessionId();
    manager->changeSessionId(session);
    auto newId = session->sessionId();
    EXPECT_NE(oldId, newId);
}

TEST_F(SessionManagerTest, GetSessionEmptySessionIdAssert)
{
    // Since getSession asserts on empty sessionID, test that it crashes
    EXPECT_DEATH(manager->getSession("", true), "");
}