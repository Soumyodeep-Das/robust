#include "gtest/gtest.h"
#include "AOPAdvice.hpp"
#include <vector>
#include <deque>
#include <atomic>

namespace drogon
{
class AOPAdviceTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(AOPAdviceTest, VectorAdvicesChainCallsAllAdvicesAndCallback)
{
    std::atomic<int> adviceCallCount{0};
    std::atomic<int> finalCallbackCalled{0};
    std::vector<std::function<void(const HttpRequestPtr &,
                                   AdviceCallback &&,
                                   AdviceChainCallback &&)>> advices;

    // Create 3 advices that call chain callback to continue
    for (int i = 0; i < 3; ++i)
    {
        advices.emplace_back(
            [&adviceCallCount](const HttpRequestPtr &,
                               AdviceCallback &&callback,
                               AdviceChainCallback &&chainCallback) {
                ++adviceCallCount;
                // Simulate calling next advice in chain
                chainCallback();
            });
    }

    // Setup request and callbackPtr
    HttpRequestImplPtr req = nullptr;  // nullptr is acceptable for this test
    auto callbackCalled = std::make_shared<std::atomic<int>>(0);
    auto callbackPtr = std::make_shared<const std::function<void(const HttpResponsePtr &)>>(
        [callbackCalled](const HttpResponsePtr &) {
            ++(*callbackCalled);
        });

    bool missCallbackCalled = false;
    auto missCallback = [&missCallbackCalled]() {
        missCallbackCalled = true;
    };

    doAdvicesChain(advices, 0, req, callbackPtr, std::move(missCallback));

    // All advices should be called
    EXPECT_EQ(adviceCallCount.load(), 3);
    // Miss callback should be called after all advices
    EXPECT_TRUE(missCallbackCalled);
    // The callbackPtr should not be called in this test because advices call chainCallback, not callback
    EXPECT_EQ(callbackCalled->load(), 0);
}

TEST_F(AOPAdviceTest, VectorAdvicesChainCallsCallback)
{
    std::atomic<int> adviceCallCount{0};
    std::atomic<int> callbackCalled{0};
    std::vector<std::function<void(const HttpRequestPtr &,
                                   AdviceCallback &&,
                                   AdviceChainCallback &&)>> advices;

    advices.emplace_back(
        [&adviceCallCount, &callbackCalled](const HttpRequestPtr &,
                                           AdviceCallback &&callback,
                                           AdviceChainCallback &&) {
            ++adviceCallCount;
            callback(std::make_shared<HttpResponse>());
            // Do not call chainCallback
        });

    HttpRequestImplPtr req = nullptr;
    auto callbackPtr = std::make_shared<const std::function<void(const HttpResponsePtr &)>>(
        [&callbackCalled](const HttpResponsePtr &) {
            ++callbackCalled;
        });

    bool missCallbackCalled = false;
    auto missCallback = [&missCallbackCalled]() {
        missCallbackCalled = true;
    };

    doAdvicesChain(advices, 0, req, callbackPtr, std::move(missCallback));

    EXPECT_EQ(adviceCallCount.load(), 1);
    EXPECT_EQ(callbackCalled.load(), 1);
    EXPECT_FALSE(missCallbackCalled);
}

TEST_F(AOPAdviceTest, DequeAdvicesChainCallsAllAdvicesAndCallback)
{
    std::atomic<int> adviceCallCount{0};
    std::atomic<int> finalCallbackCalled{0};
    std::deque<std::function<void(const HttpRequestPtr &,
                                  AdviceCallback &&,
                                  AdviceChainCallback &&)>> advices;

    for (int i = 0; i < 3; ++i)
    {
        advices.emplace_back(
            [&adviceCallCount](const HttpRequestPtr &,
                               AdviceCallback &&,
                               AdviceChainCallback &&chainCallback) {
                ++adviceCallCount;
                chainCallback();
            });
    }

    HttpRequestImplPtr req = nullptr;
    auto callbackCalled = std::make_shared<std::atomic<int>>(0);
    auto callbackPtr = std::make_shared<const std::function<void(const HttpResponsePtr &)>>(
        [callbackCalled](const HttpResponsePtr &) {
            ++(*callbackCalled);
        });

    bool missCallbackCalled = false;
    auto missCallback = [&missCallbackCalled]() {
        missCallbackCalled = true;
    };

    doAdvicesChain(advices, 0, req, callbackPtr, std::move(missCallback));

    EXPECT_EQ(adviceCallCount.load(), 3);
    EXPECT_TRUE(missCallbackCalled);
    EXPECT_EQ(callbackCalled->load(), 0);
}

TEST_F(AOPAdviceTest, DequeAdvicesChainCallsCallback)
{
    std::atomic<int> adviceCallCount{0};
    std::atomic<int> callbackCalled{0};
    std::deque<std::function<void(const HttpRequestPtr &,
                                  AdviceCallback &&,
                                  AdviceChainCallback &&)>> advices;

    advices.emplace_back(
        [&adviceCallCount, &callbackCalled](const HttpRequestPtr &,
                                           AdviceCallback &&callback,
                                           AdviceChainCallback &&) {
            ++adviceCallCount;
            callback(std::make_shared<HttpResponse>());
        });

    HttpRequestImplPtr req = nullptr;
    auto callbackPtr = std::make_shared<const std::function<void(const HttpResponsePtr &)>>(
        [&callbackCalled](const HttpResponsePtr &) {
            ++callbackCalled;
        });

    bool missCallbackCalled = false;
    auto missCallback = [&missCallbackCalled]() {
        missCallbackCalled = true;
    };

    doAdvicesChain(advices, 0, req, callbackPtr, std::move(missCallback));

    EXPECT_EQ(adviceCallCount.load(), 1);
    EXPECT_EQ(callbackCalled.load(), 1);
    EXPECT_FALSE(missCallbackCalled);
}

}  // namespace drogon