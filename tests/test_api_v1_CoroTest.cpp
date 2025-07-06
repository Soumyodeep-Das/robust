#include "api_v1_CoroTest.hpp"
#include <gtest/gtest.h>
#include <drogon/drogon.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <trantor/net/EventLoop.h>
#include <chrono>
#include <future>
#include <exception>

using namespace api::v1;

class CoroTestTest : public ::testing::Test
{
protected:
    CoroTest coroTest;

    // Helper to run coroutine with callback and wait for completion
    std::string runGetWithCallback()
    {
        std::promise<std::string> promise;
        auto future = promise.get_future();

        auto callback = [&promise](const drogon::HttpResponsePtr &resp) {
            if (resp)
                promise.set_value(resp->getBody());
            else
                promise.set_value("");
        };

        auto task = coroTest.get(nullptr, callback);
        // Run the coroutine task on the current event loop
        drogon::app().getLoop()->queueInLoop([task = std::move(task)]() mutable {
            task.resume();
        });

        // Wait for the result with timeout
        if (future.wait_for(std::chrono::seconds(1)) == std::future_status::timeout)
        {
            return "";
        }
        return future.get();
    }
};

TEST_F(CoroTestTest, Get_ReturnsDEADBEEF)
{
    std::string body = runGetWithCallback();
    EXPECT_EQ(body, "DEADBEEF");
}

TEST_F(CoroTestTest, Get2_ReturnsBADDBEEF)
{
    auto task = coroTest.get2(nullptr);
    drogon::HttpResponsePtr resp = nullptr;
    // Run coroutine and get result synchronously for test
    std::promise<drogon::HttpResponsePtr> promise;
    auto future = promise.get_future();

    drogon::app().getLoop()->queueInLoop([&coroTest, &promise]() {
        auto task = coroTest.get2(nullptr);
        task.then([&promise](drogon::HttpResponsePtr resp) {
            promise.set_value(resp);
        });
    });

    if (future.wait_for(std::chrono::seconds(1)) == std::future_status::timeout)
    {
        resp = nullptr;
    }
    else
    {
        resp = future.get();
    }

    ASSERT_NE(resp, nullptr);
    EXPECT_EQ(resp->getBody(), "BADDBEEF");
}

TEST_F(CoroTestTest, ThisWillFail_ThrowsException)
{
    auto callback = [](const drogon::HttpResponsePtr &) {};
    try
    {
        auto task = coroTest.this_will_fail(nullptr, callback);
        task.resume();
        FAIL() << "Expected std::runtime_error";
    }
    catch (const std::runtime_error &e)
    {
        EXPECT_STREQ(e.what(), "This is an excpected exception");
    }
    catch (...)
    {
        FAIL() << "Expected std::runtime_error";
    }
}

TEST_F(CoroTestTest, ThisWillFail2_ThrowsException)
{
    try
    {
        auto task = coroTest.this_will_fail2(nullptr);
        task.resume();
        FAIL() << "Expected std::runtime_error";
    }
    catch (const std::runtime_error &e)
    {
        EXPECT_STREQ(e.what(), "This is an excpected exception");
    }
    catch (...)
    {
        FAIL() << "Expected std::runtime_error";
    }
}