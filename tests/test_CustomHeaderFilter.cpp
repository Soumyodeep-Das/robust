#include "gtest/gtest.h"
#include "CustomHeaderFilter.hpp"
#include <memory>

using namespace drogon;

class CustomHeaderFilterTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}

    class MockHttpRequest : public HttpRequest
    {
    public:
        std::string getHeader(const std::string &key) const override
        {
            auto it = headers.find(key);
            if (it != headers.end())
                return it->second;
            return "";
        }
        std::unordered_map<std::string, std::string> headers;
    };

    CustomHeaderFilter filter;
};

TEST_F(CustomHeaderFilterTest, DoFilter_PassesWhenHeaderMatches)
{
    MockHttpRequest req;
    req.headers["X-Test-Header"] = "expected_value";

    // Set private members using friend or workaround (here we use pointer hack)
    // Since field_ and value_ are private, we use a lambda to set them via pointer offset
    auto ptr = reinterpret_cast<char *>(&filter);
    // Assuming field_ is first private member, value_ second
    std::string *field_ptr = reinterpret_cast<std::string *>(ptr + offsetof(CustomHeaderFilter, field_));
    std::string *value_ptr = reinterpret_cast<std::string *>(ptr + offsetof(CustomHeaderFilter, value_));
    *field_ptr = "X-Test-Header";
    *value_ptr = "expected_value";

    bool fccb_called = false;
    bool fcb_called = false;

    filter.doFilter(
        std::cref(req),
        [&](const HttpResponsePtr &) { fcb_called = true; },
        [&]() { fccb_called = true; });

    EXPECT_TRUE(fccb_called);
    EXPECT_FALSE(fcb_called);
}

TEST_F(CustomHeaderFilterTest, DoFilter_FailsWhenHeaderDoesNotMatch)
{
    MockHttpRequest req;
    req.headers["X-Test-Header"] = "wrong_value";

    auto ptr = reinterpret_cast<char *>(&filter);
    std::string *field_ptr = reinterpret_cast<std::string *>(ptr + offsetof(CustomHeaderFilter, field_));
    std::string *value_ptr = reinterpret_cast<std::string *>(ptr + offsetof(CustomHeaderFilter, value_));
    *field_ptr = "X-Test-Header";
    *value_ptr = "expected_value";

    bool fccb_called = false;
    bool fcb_called = false;
    HttpResponsePtr response;

    filter.doFilter(
        std::cref(req),
        [&](const HttpResponsePtr &res) {
            fcb_called = true;
            response = res;
        },
        [&]() { fccb_called = true; });

    EXPECT_FALSE(fccb_called);
    EXPECT_TRUE(fcb_called);
    ASSERT_NE(response, nullptr);
    EXPECT_EQ(response->getStatusCode(), k500InternalServerError);
}