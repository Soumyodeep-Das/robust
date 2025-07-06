#include "gtest/gtest.h"
#include "main.hpp"

#include <drogon/drogon.h>
#include <memory>
#include <string>

using namespace drogon;

class ATest : public ::testing::Test
{
protected:
    A a;
};

TEST_F(ATest, HandleCallsCallback)
{
    bool called = false;
    auto callback = [&](const HttpResponsePtr &resp) {
        called = true;
        ASSERT_NE(resp, nullptr);
        ASSERT_EQ(resp->getStatusCode(), k200OK);
    };
    // We use dummy parameters
    a.handle(nullptr, std::move(callback), 1, "p2", "p3", 4);
    ASSERT_TRUE(called);
}

TEST_F(ATest, StaticHandleCallsCallback)
{
    bool called = false;
    auto callback = [&](const HttpResponsePtr &resp) {
        called = true;
        ASSERT_NE(resp, nullptr);
        ASSERT_EQ(resp->getStatusCode(), k200OK);
    };
    A::staticHandle(nullptr, std::move(callback), 1, "p2", "p3", 4);
    ASSERT_TRUE(called);
}

class BTest : public ::testing::Test
{
protected:
    B b;
};

TEST_F(BTest, OperatorCallsCallback)
{
    bool called = false;
    auto callback = [&](const HttpResponsePtr &resp) {
        called = true;
        ASSERT_NE(resp, nullptr);
        ASSERT_EQ(resp->getStatusCode(), k200OK);
    };
    b(nullptr, std::move(callback), 1, 2);
    ASSERT_TRUE(called);
}

class CTest : public ::testing::Test
{
protected:
    C c;
};

TEST_F(CTest, PrivCallsCallback)
{
    bool called = false;
    auto callback = [&](const HttpResponsePtr &resp) {
        called = true;
        ASSERT_NE(resp, nullptr);
        ASSERT_EQ(resp->getStatusCode(), k200OK);
        ASSERT_NE(resp->getBody(), "");
    };
    c.priv(nullptr, std::move(callback));
    ASSERT_TRUE(called);
}

class ApiV1TestTest : public ::testing::Test
{
protected:
    api::v1::Test test;
};

TEST_F(ApiV1TestTest, GetCallsCallback)
{
    bool called = false;
    auto callback = [&](const HttpResponsePtr &resp) {
        called = true;
        ASSERT_NE(resp, nullptr);
        ASSERT_EQ(resp->getStatusCode(), k200OK);
    };
    test.get(nullptr, std::move(callback), 1, 2);
    ASSERT_TRUE(called);
}

TEST_F(ApiV1TestTest, ListCallsCallback)
{
    bool called = false;
    auto callback = [&](const HttpResponsePtr &resp) {
        called = true;
        ASSERT_NE(resp, nullptr);
        ASSERT_EQ(resp->getStatusCode(), k200OK);
    };
    test.list(nullptr, std::move(callback), 1, 2);
    ASSERT_TRUE(called);
}

TEST(FromRequestTest, ReturnsBody)
{
    drogon::HttpRequest req;
    std::string body = "test_body";
    req.setBody(body);
    std::string_view result = drogon::fromRequest(req);
    ASSERT_EQ(result, body);
}