#include "gtest/gtest.h"
#include "TestViewCtl.hpp"
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <memory>

class TestViewCtlTest : public ::testing::Test
{
protected:
    TestViewCtl ctl;

    // Helper to create a dummy HttpRequestPtr
    drogon::HttpRequestPtr createRequest()
    {
        return drogon::HttpRequest::newHttpRequest();
    }
};

TEST_F(TestViewCtlTest, AsyncHandleHttpRequest_CallsCallbackWithValidResponse)
{
    drogon::HttpRequestPtr req = createRequest();

    bool callbackCalled = false;
    drogon::HttpResponsePtr capturedResponse;

    ctl.asyncHandleHttpRequest(req,
        [&callbackCalled, &capturedResponse](const drogon::HttpResponsePtr &resp) {
            callbackCalled = true;
            capturedResponse = resp;
        });

    EXPECT_TRUE(callbackCalled);
    ASSERT_NE(capturedResponse, nullptr);
    EXPECT_EQ(capturedResponse->getStatusCode(), drogon::k200OK);
    // The view name is "TestView", but HttpResponse does not expose it directly.
    // We can check that the content type is set to HTML (typical for views).
    EXPECT_EQ(capturedResponse->getContentType(), "text/html");
}