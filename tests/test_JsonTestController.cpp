#include "gtest/gtest.h"
#include "JsonTestController.hpp"
#include <json/json.h>
#include <memory>

class DummyHttpRequest {};
class DummyHttpResponse
{
public:
    DummyHttpResponse(const Json::Value &json) : json_(json) {}
    std::shared_ptr<const Json::Value> jsonObject() const
    {
        return std::make_shared<const Json::Value>(json_);
    }
private:
    Json::Value json_;
};

class HttpRequestPtr : public std::shared_ptr<DummyHttpRequest>
{
public:
    using std::shared_ptr<DummyHttpRequest>::shared_ptr;
};

class HttpResponsePtr : public std::shared_ptr<DummyHttpResponse>
{
public:
    using std::shared_ptr<DummyHttpResponse>::shared_ptr;
};

class HttpResponse
{
public:
    static HttpResponsePtr newHttpJsonResponse(const Json::Value &json)
    {
        return std::make_shared<DummyHttpResponse>(json);
    }
};

TEST(JsonTestControllerTest, AsyncHandleHttpRequestProducesValidJson)
{
    JsonTestController controller;
    HttpRequestPtr req = std::make_shared<DummyHttpRequest>();

    bool callback_called = false;
    controller.asyncHandleHttpRequest(req,
        [&](const HttpResponsePtr &resp)
        {
            ASSERT_TRUE(resp != nullptr);
            auto json_ptr = resp->jsonObject();
            ASSERT_TRUE(json_ptr != nullptr);
            const Json::Value &json = *json_ptr;
            EXPECT_EQ(json["path"].asString(), "json");
            EXPECT_EQ(json["name"].asString(), "json test");
            ASSERT_TRUE(json.isMember("rows"));
            const Json::Value &rows = json["rows"];
            ASSERT_TRUE(rows.isArray());
            EXPECT_EQ(rows.size(), 5);
            for (Json::ArrayIndex i = 0; i < rows.size(); ++i)
            {
                const Json::Value &user = rows[i];
                EXPECT_EQ(user["id"].asInt(), static_cast<int>(i));
                EXPECT_EQ(user["name"].asString(), "none");
                EXPECT_EQ(user["c_name"].asString(), "张三");
            }
            callback_called = true;
        });
    EXPECT_TRUE(callback_called);
}