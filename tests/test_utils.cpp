#include "gtest/gtest.h"
#include "utils.hpp"
#include <json/json.h>
#include <drogon/HttpResponse.h>

TEST(UtilsTest, MakeErrRespReturnsCorrectError) {
    std::string errorMsg = "Test error";
    Json::Value result = makeErrResp(errorMsg);
    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result.isMember("error"));
    ASSERT_EQ(result["error"].asString(), errorMsg);
}

TEST(UtilsTest, BadRequestCallsCallbackWithCorrectResponse) {
    std::string errorMsg = "Bad request error";
    drogon::HttpStatusCode code = drogon::k400BadRequest;
    drogon::HttpResponsePtr capturedResp;
    auto callback = [&capturedResp](const drogon::HttpResponsePtr &resp) {
        capturedResp = resp;
    };
    badRequest(std::move(callback), errorMsg, code);
    ASSERT_NE(capturedResp, nullptr);
    ASSERT_EQ(capturedResp->statusCode(), code);
    auto bodyStr = capturedResp->body();
    Json::Reader reader;
    Json::Value respJson;
    ASSERT_TRUE(reader.parse(bodyStr, respJson));
    ASSERT_TRUE(respJson.isObject());
    ASSERT_TRUE(respJson.isMember("error"));
    ASSERT_EQ(respJson["error"].asString(), errorMsg);
}