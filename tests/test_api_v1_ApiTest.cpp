#include "gtest/gtest.h"
#include "api_v1_ApiTest.hpp"
#include <memory>
#include <string>
#include <functional>

using namespace api::v1;

// Mock classes and functions to enable compilation and testing
class MockHttpResponse {
public:
    std::string body;
    int contentTypeCode = 0;
    uint64_t expiredTime = 0;
    void setBody(const std::string &b) { body = b; }
    void setContentTypeCode(int c) { contentTypeCode = c; }
    void setExpiredTime(uint64_t t) { expiredTime = t; }
};

class HttpResponsePtr : public std::shared_ptr<MockHttpResponse> {
public:
    using std::shared_ptr<MockHttpResponse>::shared_ptr;
};

class MockHttpRequest {
public:
    std::unordered_map<std::string, std::string> parameters;
    std::unordered_map<std::string, std::string> getParameters() const { return parameters; }
    std::shared_ptr<void> get() const { return std::make_shared<int>(42); }
    std::shared_ptr<class MockAttributes> attributes = std::make_shared<class MockAttributes>();
    std::shared_ptr<class MockAttributes> getAttributes() const { return attributes; }
    std::string matchedPathPatternData() const { return "matched_path"; }
};

using HttpRequestPtr = std::shared_ptr<MockHttpRequest>;

class MockAttributes {
    std::unordered_map<std::string, uint64_t> store;
public:
    bool find(const std::string &key) const { return store.find(key) != store.end(); }
    void insert(const std::string &key, uint64_t value) { store[key] = value; }
    template<typename T>
    T get(const std::string &key) const { return static_cast<T>(store.at(key)); }
};

using AttributesPtr = std::shared_ptr<MockAttributes>;

class MockHttpResponseFactory {
public:
    static HttpResponsePtr newHttpResponse() {
        return std::make_shared<MockHttpResponse>();
    }
    static HttpResponsePtr newHttpViewResponse(const std::string &, const class HttpViewData &) {
        return std::make_shared<MockHttpResponse>();
    }
    static HttpResponsePtr newCustomHttpResponse(const Json::Value &) {
        return std::make_shared<MockHttpResponse>();
    }
    static HttpResponsePtr newHttpJsonResponse(const Json::Value &) {
        return std::make_shared<MockHttpResponse>();
    }
};

class HttpViewData {
    std::unordered_map<std::string, std::string> mapString;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> mapMap;
public:
    void insert(const std::string &key, const std::string &value) { mapString[key] = value; }
    void insert(const std::string &key, const std::unordered_map<std::string, std::string> &value) { mapMap[key] = value; }
    static std::string htmlTranslate(const std::string &input) { return input; }
};

namespace HttpResponse {
    inline HttpResponsePtr newHttpResponse() { return MockHttpResponseFactory::newHttpResponse(); }
    inline HttpResponsePtr newHttpViewResponse(const std::string &view, const HttpViewData &data) { return MockHttpResponseFactory::newHttpViewResponse(view, data); }
    inline HttpResponsePtr newCustomHttpResponse(const Json::Value &json) { return MockHttpResponseFactory::newCustomHttpResponse(json); }
    inline HttpResponsePtr newHttpJsonResponse(const Json::Value &json) { return MockHttpResponseFactory::newHttpJsonResponse(json); }
}

namespace Json {
    class Value {
        std::unordered_map<std::string, std::string> values;
    public:
        std::string &operator[](const std::string &key) { return values[key]; }
        const std::string &operator[](const std::string &key) const { return values.at(key); }
    };
}

// CT_TEXT_PLAIN mock
constexpr int CT_TEXT_PLAIN = 1;

class ApiTestTest : public ::testing::Test {
protected:
    ApiTest apiTest;
    HttpRequestPtr req;
    bool callbackCalled;
    HttpResponsePtr response;

    void SetUp() override {
        req = std::make_shared<MockHttpRequest>();
        callbackCalled = false;
        response = nullptr;
    }

    void callback(const HttpResponsePtr &resp) {
        callbackCalled = true;
        response = resp;
    }
};

TEST_F(ApiTestTest, rootGet) {
    apiTest.rootGet(*req, [this](const HttpResponsePtr &resp) { callback(resp); });
    EXPECT_TRUE(callbackCalled);
    EXPECT_EQ(response->body, "ROOT Get!!!");
}

TEST_F(ApiTestTest, rootPost) {
    // rootPost runs callback asynchronously, wait for it
    std::promise<void> promise;
    auto future = promise.get_future();
    apiTest.rootPost(*req, [this, &promise](const HttpResponsePtr &resp) {
        callback(resp);
        promise.set_value();
    });
    future.wait();
    EXPECT_TRUE(callbackCalled);
    EXPECT_EQ(response->body, "ROOT Post!!!");
}

TEST_F(ApiTestTest, get) {
    apiTest.get(*req, [this](const HttpResponsePtr &resp) { callback(resp); }, 42, "test");
    EXPECT_TRUE(callbackCalled);
    // Body is not set in get, so response body is empty
    EXPECT_TRUE(response != nullptr);
}

TEST_F(ApiTestTest, your_method_name) {
    apiTest.your_method_name(*req, [this](const HttpResponsePtr &resp) { callback(resp); }, 3.14, 7);
    EXPECT_TRUE(callbackCalled);
    EXPECT_TRUE(response != nullptr);
}

TEST_F(ApiTestTest, staticApi) {
    apiTest.staticApi(*req, [this](const HttpResponsePtr &resp) { callback(resp); });
    EXPECT_TRUE(callbackCalled);
    EXPECT_EQ(response->body, "staticApi,hello!!");
    EXPECT_EQ(response->expiredTime, 0);
}

TEST_F(ApiTestTest, get2) {
    apiTest.get2(*req, [this](const HttpResponsePtr &resp) { callback(resp); }, "param");
    EXPECT_TRUE(callbackCalled);
    EXPECT_TRUE(response != nullptr);
}

TEST_F(ApiTestTest, jsonTestWithJson) {
    auto json = std::make_shared<Json::Value>();
    apiTest.jsonTest(std::move(json), [this](const HttpResponsePtr &resp) { callback(resp); });
    EXPECT_TRUE(callbackCalled);
    EXPECT_TRUE(response != nullptr);
}

TEST_F(ApiTestTest, jsonTestWithoutJson) {
    std::shared_ptr<Json::Value> json;
    apiTest.jsonTest(std::move(json), [this](const HttpResponsePtr &resp) { callback(resp); });
    EXPECT_TRUE(callbackCalled);
    EXPECT_TRUE(response != nullptr);
}

TEST_F(ApiTestTest, formTestOk) {
    req->parameters["k1"] = "1";
    req->parameters["k2"] = "安";
    req->parameters["k3"] = "test@example.com";
    apiTest.formTest(*req, [this](const HttpResponsePtr &resp) { callback(resp); });
    EXPECT_TRUE(callbackCalled);
    EXPECT_TRUE(response != nullptr);
}

TEST_F(ApiTestTest, formTestBad) {
    req->parameters["k1"] = "0";
    req->parameters["k2"] = "安";
    req->parameters["k3"] = "test@example.com";
    apiTest.formTest(*req, [this](const HttpResponsePtr &resp) { callback(resp); });
    EXPECT_TRUE(callbackCalled);
    EXPECT_TRUE(response != nullptr);
}

TEST_F(ApiTestTest, attributesTestOk) {
    req->attributes->insert("OTHER_KEY", 1);
    apiTest.attributesTest(*req, [this](const HttpResponsePtr &resp) { callback(resp); });
    EXPECT_TRUE(callbackCalled);
    EXPECT_TRUE(response != nullptr);
}

TEST_F(ApiTestTest, attributesTestBadFind) {
    req->attributes->insert("ATTR_ADDR", 123);
    apiTest.attributesTest(*req, [this](const HttpResponsePtr &resp) { callback(resp); });
    EXPECT_TRUE(callbackCalled);
    EXPECT_TRUE(response != nullptr);
}

TEST_F(ApiTestTest, regexTest) {
    apiTest.regexTest(*req, [this](const HttpResponsePtr &resp) { callback(resp); }, 5, "test");
    EXPECT_TRUE(callbackCalled);
    EXPECT_TRUE(response != nullptr);
}

TEST_F(ApiTestTest, cacheTest) {
    apiTest.cacheTest(*req, [this](const HttpResponsePtr &resp) { callback(resp); });
    EXPECT_TRUE(callbackCalled);
    EXPECT_TRUE(response != nullptr);
    EXPECT_EQ(response->contentTypeCode, CT_TEXT_PLAIN);
}

TEST_F(ApiTestTest, cacheTest2) {
    apiTest.cacheTest2(*req, [this](const HttpResponsePtr &resp) { callback(resp); });
    EXPECT_TRUE(callbackCalled);
    EXPECT_TRUE(response != nullptr);
    EXPECT_EQ(response->contentTypeCode, CT_TEXT_PLAIN);
}

TEST_F(ApiTestTest, cacheTestRegex) {
    apiTest.cacheTestRegex(*req, [this](const HttpResponsePtr &resp) { callback(resp); });
    EXPECT_TRUE(callbackCalled);
    EXPECT_TRUE(response != nullptr);
    EXPECT_EQ(response->contentTypeCode, CT_TEXT_PLAIN);
}