#include "gtest/gtest.h"
#include "BeginAdviceTest.hpp"

class BeginAdviceTestTest : public ::testing::Test {
protected:
    BeginAdviceTest obj;
};

TEST_F(BeginAdviceTestTest, ContentStaticVariableDefault) {
    EXPECT_EQ(BeginAdviceTest::content_, "Default content");
}

TEST_F(BeginAdviceTestTest, AsyncHandleHttpRequestSetsBodyAndCallsCallback) {
    // Mock HttpRequestPtr (empty, as not used in method)
    HttpRequestPtr req;

    // Setup a flag and captured body to verify callback call
    bool callback_called = false;
    std::string captured_body;

    auto callback = [&](const HttpResponsePtr &resp) {
        callback_called = true;
        captured_body = resp->getBody();
    };

    BeginAdviceTest::content_ = "Test content";

    obj.asyncHandleHttpRequest(req, std::move(callback));

    EXPECT_TRUE(callback_called);
    EXPECT_EQ(captured_body, "Test content");
}