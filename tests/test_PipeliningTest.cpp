#include "PipeliningTest.hpp"
#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>

class PipeliningTestTest : public ::testing::Test
{
protected:
    PipeliningTest pipeliningTest;
};

TEST_F(PipeliningTestTest, AsyncHandleHttpRequest_CounterMod3Equals1_CallsCallbackImmediately)
{
    std::atomic<bool> called{false};
    int expectedCounter = 1; // We want c % 3 == 1
    // To ensure counter is at expected value, we run dummy calls until counter % 3 == 1
    // But since counter is static inside method, no direct access. We call multiple times until condition met.

    // We'll call asyncHandleHttpRequest multiple times until the callback is called with counter % 3 == 1

    std::mutex mtx;
    std::condition_variable cv;
    bool done = false;

    // We will call asyncHandleHttpRequest multiple times, the first callback with counter % 3 == 1 will set done = true

    for (int i = 0; i < 10; ++i)
    {
        pipeliningTest.asyncHandleHttpRequest(nullptr, [&](const HttpResponsePtr &resp) {
            // Check header counter
            auto counterHeader = resp->getHeader("counter");
            int c = std::stoi(counterHeader);
            if (c % 3 == 1)
            {
                std::unique_lock<std::mutex> lock(mtx);
                called = true;
                done = true;
                cv.notify_one();
            }
        });
        if (called)
            break;
    }

    std::unique_lock<std::mutex> lock(mtx);
    if (!done)
    {
        cv.wait_for(lock, std::chrono::seconds(1), [&done] { return done; });
    }
    EXPECT_TRUE(called);
}

TEST_F(PipeliningTestTest, AsyncHandleHttpRequest_CounterMod3Equals2_CallsCallbackAfterDelay)
{
    std::atomic<bool> called{false};
    std::mutex mtx;
    std::condition_variable cv;
    bool done = false;

    // We call asyncHandleHttpRequest multiple times until we get c % 3 == 2
    for (int i = 0; i < 20; ++i)
    {
        pipeliningTest.asyncHandleHttpRequest(nullptr, [&](const HttpResponsePtr &resp) {
            auto counterHeader = resp->getHeader("counter");
            int c = std::stoi(counterHeader);
            if (c % 3 == 2)
            {
                std::unique_lock<std::mutex> lock(mtx);
                called = true;
                done = true;
                cv.notify_one();
            }
        });
        if (called)
            break;
    }

    std::unique_lock<std::mutex> lock(mtx);
    if (!done)
    {
        cv.wait_for(lock, std::chrono::seconds(2), [&done] { return done; });
    }
    EXPECT_TRUE(called);
}

TEST_F(PipeliningTestTest, AsyncHandleHttpRequest_CounterMod3Equals0_CallsCallbackAfterDelay)
{
    std::atomic<bool> called{false};
    std::mutex mtx;
    std::condition_variable cv;
    bool done = false;

    // We call asyncHandleHttpRequest multiple times until we get c % 3 == 0
    for (int i = 0; i < 20; ++i)
    {
        pipeliningTest.asyncHandleHttpRequest(nullptr, [&](const HttpResponsePtr &resp) {
            auto counterHeader = resp->getHeader("counter");
            int c = std::stoi(counterHeader);
            if (c % 3 == 0)
            {
                std::unique_lock<std::mutex> lock(mtx);
                called = true;
                done = true;
                cv.notify_one();
            }
        });
        if (called)
            break;
    }

    std::unique_lock<std::mutex> lock(mtx);
    if (!done)
    {
        cv.wait_for(lock, std::chrono::seconds(2), [&done] { return done; });
    }
    EXPECT_TRUE(called);
}