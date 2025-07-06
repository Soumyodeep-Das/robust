#include "gtest/gtest.h"
#include "MethodTest.hpp"
#include <drogon/HttpResponse.h>
#include <drogon/HttpRequest.h>
#include <memory>
#include <string>

class MethodTestTest : public ::testing::Test
{
protected:
    MethodTest methodTest;

    // Helper to create a dummy HttpRequestPtr (nullptr is acceptable for these tests)
    drogon::HttpRequestPtr createRequest()
    {
        return nullptr;
    }
};

TEST_F(MethodTestTest, GetCallsCallbackWithGET)
{
    bool called = false;
    methodTest.get(createRequest(),
                   [&](const drogon::HttpResponsePtr &resp) {
                       ASSERT_NE(resp, nullptr);
                       ASSERT_EQ(resp->getBody(), "GET");
                       called = true;
                   });
    EXPECT_TRUE(called);
}

TEST_F(MethodTestTest, PostCallsCallbackWithPOST)
{
    bool called = false;
    methodTest.post(createRequest(),
                    [&](const drogon::HttpResponsePtr &resp) {
                        ASSERT_NE(resp, nullptr);
                        ASSERT_EQ(resp->getBody(), "POST");
                        called = true;
                    },
                    "test");
    EXPECT_TRUE(called);
}

TEST_F(MethodTestTest, GetRegCallsCallbackWithGET)
{
    bool called = false;
    methodTest.getReg(createRequest(),
                      [&](const drogon::HttpResponsePtr &resp) {
                          ASSERT_NE(resp, nullptr);
                          ASSERT_EQ(resp->getBody(), "GET");
                          called = true;
                      },
                      "regex");
    EXPECT_TRUE(called);
}

TEST_F(MethodTestTest, PostRegCallsCallbackWithPOST)
{
    bool called = false;
    methodTest.postReg(createRequest(),
                       [&](const drogon::HttpResponsePtr &resp) {
                           ASSERT_NE(resp, nullptr);
                           ASSERT_EQ(resp->getBody(), "POST");
                           called = true;
                       },
                       "regex",
                       "string");
    EXPECT_TRUE(called);
}

TEST_F(MethodTestTest, PostRegexCallsCallbackWithPOST)
{
    bool called = false;
    methodTest.postRegex(createRequest(),
                         [&](const drogon::HttpResponsePtr &resp) {
                             ASSERT_NE(resp, nullptr);
                             ASSERT_EQ(resp->getBody(), "POST");
                             called = true;
                         },
                         "regex");
    EXPECT_TRUE(called);
}