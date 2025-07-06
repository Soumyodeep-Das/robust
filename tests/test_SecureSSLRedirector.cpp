#include "SecureSSLRedirector.hpp"
#include <gtest/gtest.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <json/json.h>

using namespace drogon;
using namespace drogon::plugin;

class SecureSSLRedirectorTest : public ::testing::Test
{
protected:
    SecureSSLRedirector redirector;

    HttpRequestPtr makeRequest(const std::string &path,
                              const std::string &query = "",
                              bool secure = false,
                              const std::string &hostHeader = "") const
    {
        auto req = HttpRequest::newHttpRequest();
        req->setPath(path);
        req->setQuery(query);
        req->setSecureConnection(secure);
        if (!hostHeader.empty())
        {
            req->addHeader("host", hostHeader);
        }
        return req;
    }
};

TEST_F(SecureSSLRedirectorTest, InitAndStartWithExemptRegex)
{
    Json::Value config;
    Json::Value exempt(Json::arrayValue);
    exempt.append("/exempt1");
    exempt.append("/exempt2");
    config["ssl_redirect_exempt"] = exempt;
    config["secure_ssl_host"] = "secure.example.com";

    redirector.initAndStart(config);

    // Test that regexFlag_ is true by redirectingAdvice behavior
    auto reqExempt = makeRequest("/exempt1", "", false);
    auto respExempt = redirector.redirectingAdvice(reqExempt);
    EXPECT_EQ(respExempt, nullptr);

    auto reqNonExempt = makeRequest("/notexempt", "", false);
    auto respNonExempt = redirector.redirectingAdvice(reqNonExempt);
    ASSERT_NE(respNonExempt, nullptr);
}

TEST_F(SecureSSLRedirectorTest, RedirectingAdviceOnSecureConnection)
{
    auto req = makeRequest("/anypath", "", true);
    auto resp = redirector.redirectingAdvice(req);
    EXPECT_EQ(resp, nullptr);
}

TEST_F(SecureSSLRedirectorTest, RedirectToSSLWithSecureHost)
{
    Json::Value config;
    config["secure_ssl_host"] = "securehost.com";
    redirector.initAndStart(config);

    auto req = makeRequest("/path", "a=1&b=2", false);
    auto resp = redirector.redirectToSSL(req);
    ASSERT_NE(resp, nullptr);
    EXPECT_EQ(resp->statusCode(), k302Found);
    EXPECT_EQ(resp->getHeader("location"), "https://securehost.com/path?a=1&b=2");
}

TEST_F(SecureSSLRedirectorTest, RedirectToSSLWithHostHeader)
{
    Json::Value config;
    redirector.initAndStart(config);

    auto req = makeRequest("/path", "a=1", false, "hostheader.com");
    auto resp = redirector.redirectToSSL(req);
    ASSERT_NE(resp, nullptr);
    EXPECT_EQ(resp->statusCode(), k302Found);
    EXPECT_EQ(resp->getHeader("location"), "https://hostheader.com/path?a=1");
}

TEST_F(SecureSSLRedirectorTest, RedirectToSSLNoHostHeader)
{
    Json::Value config;
    redirector.initAndStart(config);

    auto req = makeRequest("/path", "a=1", false);
    auto resp = redirector.redirectToSSL(req);
    ASSERT_NE(resp, nullptr);
    EXPECT_EQ(resp->statusCode(), k404NotFound);
}

TEST_F(SecureSSLRedirectorTest, RedirectingAdviceWithRegexFlagFalse)
{
    Json::Value config;
    redirector.initAndStart(config);

    auto req = makeRequest("/anypath", "", false);
    auto resp = redirector.redirectingAdvice(req);
    ASSERT_NE(resp, nullptr);
}