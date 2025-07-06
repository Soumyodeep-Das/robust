#include "gtest/gtest.h"
#include "CustomCtrl.hpp"

#include <string>
#include <functional>

class DummyHttpResponse {
public:
    void setBody(const std::string &body) {
        body_ = body;
    }
    std::string getBody() const {
        return body_;
    }
private:
    std::string body_;
};

class DummyHttpResponsePtr {
public:
    DummyHttpResponsePtr() : resp_(new DummyHttpResponse) {}
    DummyHttpResponse* operator->() { return resp_.get(); }
    DummyHttpResponse* get() const { return resp_.get(); }
private:
    std::shared_ptr<DummyHttpResponse> resp_;
};

class DummyHttpRequestPtr {};

class CustomCtrlTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize greetings_ member via friend or public setter if needed
        // Since greetings_ is private and no setter is provided, we will use a derived class to set it
    }

    class TestableCustomCtrl : public CustomCtrl {
    public:
        void setGreetings(const std::string &g) {
            greetings_ = g;
        }
    };

    TestableCustomCtrl ctrl_;
};

TEST_F(CustomCtrlTest, HelloSetsCorrectBodyAndCallsCallback) {
    ctrl_.setGreetings("Hello");

    DummyHttpRequestPtr req;

    DummyHttpResponsePtr capturedResp;

    bool callbackCalled = false;

    auto callback = [&](const DummyHttpResponsePtr &resp) {
        capturedResp = resp;
        callbackCalled = true;
    };

    // We need to call hello with DummyHttpRequestPtr and std::function<void(const DummyHttpResponsePtr&)>
    // but the signature expects HttpRequestPtr and HttpResponsePtr, so we must cast or adapt.
    // For testing, we will cast DummyHttpRequestPtr* to HttpRequestPtr* and DummyHttpResponsePtr* to HttpResponsePtr*.
    // This is safe here because we only test the logic of hello method calling callback with correct body.

    // Use reinterpret_cast for test only
    ctrl_.hello(reinterpret_cast<const HttpRequestPtr&>(req),
                std::function<void(const HttpResponsePtr &)>([&](const HttpResponsePtr &resp) {
                    // resp is HttpResponsePtr, but we want DummyHttpResponsePtr
                    // We reinterpret_cast to DummyHttpResponsePtr for test
                    callback(reinterpret_cast<const DummyHttpResponsePtr &>(resp));
                }),
                "World");

    EXPECT_TRUE(callbackCalled);
    EXPECT_EQ(capturedResp->getBody(), "<P>Hello, World</P>");
}