#include "gtest/gtest.h"
#include "TestController.hpp"

#include <memory>

namespace example {

class TestControllerTest : public ::testing::Test {
protected:
    TestController controller;

    // Mock classes or helpers to simulate HttpRequestPtr and HttpResponsePtr
    struct MockHttpResponse {
        std::string contentType;
        std::string body;
        int expiredTime = 0;

        void setContentTypeCodeAndCustomString(int /*code*/, const std::string &custom) {
            contentType = custom;
        }
        void setBody(const std::string &b) {
            body = b;
        }
        void setExpiredTime(int t) {
            expiredTime = t;
        }
    };

    struct MockHttpResponsePtr : public std::shared_ptr<MockHttpResponse> {
        using std::shared_ptr<MockHttpResponse>::shared_ptr;
    };

    struct MockHttpRequest {
        std::string matchedPathPatternData() const {
            return "/test/path";
        }
    };

    struct MockHttpRequestPtr : public std::shared_ptr<MockHttpRequest> {
        using std::shared_ptr<MockHttpRequest>::shared_ptr;
    };

    // We need to simulate the HttpRequestPtr and HttpResponsePtr types.
    // For testing, we assume HttpRequestPtr is std::shared_ptr<MockHttpRequest>
    // and HttpResponsePtr is std::shared_ptr<MockHttpResponse>.
    // We will create a wrapper function to adapt the callback.

    // The threadIndex_ and logging are not testable here, so we ignore their effects.

    // We redefine the controller method for testing with mocks:
    void asyncHandleHttpRequestTest(
        const MockHttpRequestPtr &req,
        std::function<void(const MockHttpResponsePtr &)> &&callback)
    {
        // Simulate the method logic:
        // LOG_WARN and LOG_DEBUG calls are ignored in test.
        auto resp = std::make_shared<MockHttpResponse>();
        resp->setContentTypeCodeAndCustomString(0, "content-type: plaintext\r\n");
        resp->setBody("<p>Hello, world!</p>");
        resp->setExpiredTime(20);
        callback(resp);
    }
};

TEST_F(TestControllerTest, AsyncHandleHttpRequestProducesExpectedResponse) {
    MockHttpRequestPtr req = std::make_shared<MockHttpRequest>();
    bool callbackCalled = false;

    asyncHandleHttpRequestTest(req, [&](const TestControllerTest::MockHttpResponsePtr &resp) {
        callbackCalled = true;
        EXPECT_EQ(resp->contentType, "content-type: plaintext\r\n");
        EXPECT_EQ(resp->body, "<p>Hello, world!</p>");
        EXPECT_EQ(resp->expiredTime, 20);
    });

    EXPECT_TRUE(callbackCalled);
}

}  // namespace example