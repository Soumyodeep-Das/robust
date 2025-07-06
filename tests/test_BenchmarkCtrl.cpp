#include "BenchmarkCtrl.hpp"
#include <gtest/gtest.h>

class BenchmarkCtrlTest : public ::testing::Test {
protected:
    BenchmarkCtrl ctrl;
};

class DummyHttpRequest {};
class DummyHttpResponse {
public:
    std::string body;
    int expiredTime = -1;
    void setBody(const std::string& b) { body = b; }
    void setExpiredTime(int t) { expiredTime = t; }
};

class HttpRequestPtr : public DummyHttpRequest {};
class HttpResponsePtr : public DummyHttpResponse {
public:
    static HttpResponsePtr newHttpResponse() {
        return HttpResponsePtr();
    }
};

TEST_F(BenchmarkCtrlTest, AsyncHandleHttpRequest_CallsCallbackWithResponse) {
    bool callbackCalled = false;
    ctrl.asyncHandleHttpRequest(HttpRequestPtr(),
        [&](const HttpResponsePtr &resp) {
            callbackCalled = true;
            EXPECT_EQ(resp.body, "<p>Hello, world!</p>");
            EXPECT_EQ(resp.expiredTime, 0);
        });
    EXPECT_TRUE(callbackCalled);
}