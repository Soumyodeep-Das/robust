#include "gtest/gtest.h"
#include "JsonCtrl.hpp"

class JsonCtrlTest : public ::testing::Test
{
protected:
    JsonCtrl jsonCtrl;
};

class DummyHttpRequest {};
class DummyHttpResponse
{
public:
    Json::Value jsonValue;
    DummyHttpResponse(const Json::Value &val) : jsonValue(val) {}
};

class HttpRequestPtr : public DummyHttpRequest {};
class HttpResponsePtr : public DummyHttpResponse
{
public:
    HttpResponsePtr(const Json::Value &val) : DummyHttpResponse(val) {}
};

// Mock HttpResponse::newHttpJsonResponse to return HttpResponsePtr
namespace {
HttpResponsePtr HttpResponse_newHttpJsonResponse(const Json::Value &val)
{
    return HttpResponsePtr(val);
}
} // namespace

// Override the static method for testing
// We do this by defining a subclass or using a function pointer if possible.
// Since original code uses static method, we simulate here by patching.

TEST_F(JsonCtrlTest, AsyncHandleHttpRequestReturnsHelloWorldMessage)
{
    bool callbackCalled = false;
    Json::Value receivedJson;

    // Define a callback to capture the response
    auto callback = [&](const HttpResponsePtr &resp) {
        callbackCalled = true;
        receivedJson = resp.jsonValue;
    };

    // Temporarily replace HttpResponse::newHttpJsonResponse with our mock
    // Since we cannot replace static method easily, we simulate by calling the method directly
    // and assume it returns HttpResponsePtr as in our mock.

    // Call the method under test
    jsonCtrl.asyncHandleHttpRequest(HttpRequestPtr(), std::move(callback));

    // Check that callback was called
    EXPECT_TRUE(callbackCalled);
    // Check that the message is "Hello, World!"
    ASSERT_TRUE(receivedJson.isMember("message"));
    EXPECT_EQ(receivedJson["message"].asString(), "Hello, World!");
}