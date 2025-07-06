#include "gtest/gtest.h"
#include "TaskTimeoutFlag.hpp"

#include <chrono>
#include <atomic>
#include <thread>

namespace
{
class DummyEventLoop
{
  public:
    void runAfter(const std::chrono::duration<double> &timeout, std::function<void()> func)
    {
        // Run the function after timeout in a separate thread to simulate async behavior
        std::thread([timeout, func]() {
            std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>(timeout));
            func();
        }).detach();
    }
};

class TaskTimeoutFlagTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        called = false;
        loop = new DummyEventLoop();
        timeoutFlag = std::make_shared<drogon::TaskTimeoutFlag>(
            loop,
            std::chrono::milliseconds(50),
            [this]() { called = true; });
    }

    void TearDown() override
    {
        delete loop;
    }

    DummyEventLoop *loop;
    std::shared_ptr<drogon::TaskTimeoutFlag> timeoutFlag;
    std::atomic<bool> called;
};

}  // namespace

TEST_F(TaskTimeoutFlagTest, DoneInitiallyFalse)
{
    // Initially done() should return false and set isDone_ to true
    bool firstCall = timeoutFlag->done();
    EXPECT_FALSE(firstCall);

    // Subsequent done() calls should return true
    bool secondCall = timeoutFlag->done();
    EXPECT_TRUE(secondCall);
}

TEST_F(TaskTimeoutFlagTest, TimeoutCallbackCalled)
{
    timeoutFlag->runTimer();
    // Wait longer than timeout to ensure callback is called
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_TRUE(called);
}

TEST_F(TaskTimeoutFlagTest, TimeoutCallbackNotCalledIfDone)
{
    // Mark done before running timer
    timeoutFlag->done();
    timeoutFlag->runTimer();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_FALSE(called);
}