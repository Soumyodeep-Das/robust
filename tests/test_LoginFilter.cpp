#include "gtest/gtest.h"
#include "LoginFilter.hpp"
#include <drogon/drogon.h>

TEST(LoginFilterTest, AuthorizationHeaderMissing) {
    LoginFilter filter;
    auto req = drogon::HttpRequest::newHttpRequest();
    bool fcbCalled = false;
    drogon::HttpResponsePtr resp;
    auto fcb = [&fcbCalled, &resp](const drogon::HttpResponsePtr& response) {
        fcbCalled = true;
        resp = response;
    };
    auto fccb = []() { ADD_FAILURE() << "Filter chain callback should not be called"; };
    filter.doFilter(req, std::move(fcb), std::move(fccb));
    ASSERT_TRUE(fcbCalled);
    ASSERT_EQ(resp->statusCode(), drogon::k400BadRequest);
}