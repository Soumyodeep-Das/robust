#include "gtest/gtest.h"
#include "HelloController.hpp"
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <memory>
#include <string>

using namespace drogon;

class SayHelloTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(SayHelloTest, GenericHelloResponseBody)
{
    SayHello controller;

    // Prepare dummy HttpRequestPtr (nullptr is acceptable for this test)
    HttpRequestPtr req;

    // Capture the response via the callback
    bool callbackCalled = false;
    std::string responseBody;

    controller.genericHello(req, [&](const HttpResponsePtr &resp) {
        ASSERT_NE(resp, nullptr);
        responseBody = resp->getBody();
        callbackCalled = true;
    });

    EXPECT_TRUE(callbackCalled);
    EXPECT_EQ(responseBody,
              "Hello, this is a generic hello message from the SayHello controller");
}

TEST_F(SayHelloTest, PersonalizedHelloResponseBody)
{
    SayHello controller;

    HttpRequestPtr req;

    bool callbackCalled = false;
    std::string responseBody;

    controller.personalizedHello(req, [&](const HttpResponsePtr &resp) {
        ASSERT_NE(resp, nullptr);
        responseBody = resp->getBody();
        callbackCalled = true;
    });

    EXPECT_TRUE(callbackCalled);
    EXPECT_EQ(responseBody,
              "Hi there, this is another hello from the SayHello Controller");
}