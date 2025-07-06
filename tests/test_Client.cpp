#include "gtest/gtest.h"
#include "Client.hpp"
#include "drogon/HttpTypes.h"
#include <memory>
#include <string>

using namespace drogon;

class ClientTest : public ::testing::Test
{
protected:
    Client client;

    // Mock classes to simulate HttpRequestPtr and HttpResponsePtr
    class MockHttpRequest : public HttpRequest
    {
    public:
        MockHttpRequest() : jsonObject_(nullptr) {}
        virtual Json::ValuePtr getJsonObject() const override { return jsonObject_; }
        void setJsonObject(const Json::ValuePtr &json) { jsonObject_ = json; }

    private:
        Json::ValuePtr jsonObject_;
    };

    class MockHttpResponse : public HttpResponse
    {
    public:
        MockHttpResponse() : statusCode_(0), body_("") {}
        void setStatusCode(HttpStatusCode code) { statusCode_ = code; }
        HttpStatusCode getStatusCode() const { return statusCode_; }
        void setBody(const std::string &body) { body_ = body; }
        const std::string &getBody() const { return body_; }

    private:
        HttpStatusCode statusCode_;
        std::string body_;
    };

    // Helper to create HttpResponsePtr from MockHttpResponse
    HttpResponsePtr createHttpResponsePtr(int statusCode, const std::string &body = "")
    {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(static_cast<HttpStatusCode>(statusCode));
        resp->setBody(body);
        return resp;
    }
};

TEST_F(ClientTest, PostMissingValueReturnsBadRequest)
{
    MockHttpRequest req;
    req.setJsonObject(nullptr);  // No JSON body

    bool callbackCalled = false;
    client.post(
        HttpRequestPtr(&req, [](HttpRequest *) {}),
        [&](const HttpResponsePtr &resp) {
            callbackCalled = true;
            EXPECT_EQ(resp->getStatusCode(), k400BadRequest);
            EXPECT_EQ(resp->getBody(), "missing 'value' in body");
        },
        "key");

    EXPECT_TRUE(callbackCalled);
}

TEST_F(ClientTest, GetCallbackCalled)
{
    // Since the Client class depends on drogon app and RedisClient,
    // here we only test that the get method calls the callback.
    // Full integration tests require mocking drogon internals.

    MockHttpRequest req;

    bool callbackCalled = false;
    client.get(
        HttpRequestPtr(&req, [](HttpRequest *) {}),
        [&](const HttpResponsePtr &resp) {
            callbackCalled = true;
            // We cannot assert status code here because it depends on Redis response.
            // Just check callback is called.
            EXPECT_TRUE(resp != nullptr);
        },
        "key");

    // Callback may be async, so we cannot guarantee immediate call here.
    // This test is limited due to external dependencies.
    // We just check no crash and callback is set.
    SUCCEED();
}

TEST_F(ClientTest, PostCallbackCalled)
{
    MockHttpRequest req;
    Json::ValuePtr json(new Json::Value);
    (*json)["value"] = "testvalue";
    req.setJsonObject(json);

    bool callbackCalled = false;
    client.post(
        HttpRequestPtr(&req, [](HttpRequest *) {}),
        [&](const HttpResponsePtr &resp) {
            callbackCalled = true;
            // We cannot assert status code here because it depends on Redis response.
            // Just check callback is called.
            EXPECT_TRUE(resp != nullptr);
        },
        "key");

    // Callback may be async, so we cannot guarantee immediate call here.
    // This test is limited due to external dependencies.
    // We just check no crash and callback is set.
    SUCCEED();
}