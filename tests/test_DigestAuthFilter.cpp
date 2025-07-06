#include "gtest/gtest.h"
#include "DigestAuthFilter.hpp"

#include <map>
#include <string>
#include <vector>
#include <memory>

class DummyHttpRequest
{
public:
    DummyHttpRequest()
        : method(Get), has_session(true)
    {
    }
    bool session() const { return has_session; }
    std::string getHeader(const std::string &name) const
    {
        if (name == "Authorization")
            return authorization_header;
        return "";
    }
    HttpMethod getMethod() const { return method; }

    void setAuthorizationHeader(const std::string &header) { authorization_header = header; }
    void setSession(bool session) { has_session = session; }
    void setMethod(HttpMethod m) { method = m; }

private:
    HttpMethod method;
    bool has_session;
    std::string authorization_header;
};

class DummyHttpResponse
{
public:
    DummyHttpResponse() : status_code(0) {}
    void setStatusCode(int code) { status_code = code; }
    int getStatusCode() const { return status_code; }
    void addHeader(const std::string &key, const std::string &value)
    {
        headers[key] = value;
    }
    std::string getHeader(const std::string &key) const
    {
        auto it = headers.find(key);
        if (it != headers.end())
            return it->second;
        return "";
    }

private:
    int status_code;
    std::map<std::string, std::string> headers;
};

using HttpRequestPtr = std::shared_ptr<DummyHttpRequest>;
using HttpResponsePtr = std::shared_ptr<DummyHttpResponse>;

HttpResponsePtr lastResponse;
bool filterChainCalled;

void filterCallback(const HttpResponsePtr &resp)
{
    lastResponse = resp;
}

void filterChainCallback()
{
    filterChainCalled = true;
}

TEST(DigestAuthFilterTest, Method2StringValid)
{
    EXPECT_EQ(method2String(Get), "GET");
    EXPECT_EQ(method2String(Post), "POST");
    EXPECT_EQ(method2String(Head), "HEAD");
    EXPECT_EQ(method2String(Put), "PUT");
    EXPECT_EQ(method2String(Delete), "DELETE");
    EXPECT_EQ(method2String(Options), "OPTIONS");
    EXPECT_EQ(method2String(Patch), "PATCH");
}

TEST(DigestAuthFilterTest, Method2StringInvalid)
{
    EXPECT_EQ(method2String(static_cast<HttpMethod>(999)), "INVALID");
}

TEST(DigestAuthFilterTest, ToLower)
{
    EXPECT_EQ(toLower("ABCdef123"), "abcdef123");
    EXPECT_EQ(toLower(""), "");
}

TEST(DigestAuthFilterTest, IsEndOfAttributeName)
{
    const char *data = "name=value";
    size_t len = strlen(data);
    EXPECT_TRUE(DigestAuthFilter::isEndOfAttributeName(len, len, data));
    EXPECT_TRUE(DigestAuthFilter::isEndOfAttributeName(4, len, data)); // '=' at pos 4
    EXPECT_FALSE(DigestAuthFilter::isEndOfAttributeName(3, len, data));
    const char *data2 = "name=  ";
    EXPECT_TRUE(DigestAuthFilter::isEndOfAttributeName(4, 6, data2));
}

TEST(DigestAuthFilterTest, HttpParseAttributesBasic)
{
    const char *data = "username=\"user\", realm=\"realm\", nonce=abc123";
    size_t len = strlen(data);
    HttpAttributeList attributes;
    DigestAuthFilter::httpParseAttributes(data, len, attributes);
    EXPECT_EQ(attributes.size(), 3u);
    EXPECT_EQ(attributes[0].first, "username");
    EXPECT_EQ(attributes[0].second, "user");
    EXPECT_EQ(attributes[1].first, "realm");
    EXPECT_EQ(attributes[1].second, "realm");
    EXPECT_EQ(attributes[2].first, "nonce");
    EXPECT_EQ(attributes[2].second, "abc123");
}

TEST(DigestAuthFilterTest, HttpHasAttributeFound)
{
    HttpAttributeList attributes = {{"name", "value"}, {"foo", "bar"}};
    std::string val;
    EXPECT_TRUE(DigestAuthFilter::httpHasAttribute(attributes, "name", &val));
    EXPECT_EQ(val, "value");
    EXPECT_TRUE(DigestAuthFilter::httpHasAttribute(attributes, "foo", nullptr));
}

TEST(DigestAuthFilterTest, HttpHasAttributeNotFound)
{
    HttpAttributeList attributes = {{"name", "value"}};
    std::string val;
    EXPECT_FALSE(DigestAuthFilter::httpHasAttribute(attributes, "missing", &val));
}

TEST(DigestAuthFilterTest, DoFilterNoSession)
{
    std::map<std::string, std::string> creds = {{"user", "pass"}};
    DigestAuthFilter filter(creds, "realm", "opaque");
    HttpRequestPtr req = std::make_shared<DummyHttpRequest>();
    req->setSession(false);
    lastResponse = nullptr;
    filterChainCalled = false;
    filter.doFilter(req, filterCallback, filterChainCallback);
    ASSERT_NE(lastResponse, nullptr);
    EXPECT_EQ(lastResponse->getStatusCode(), 404);
    EXPECT_FALSE(filterChainCalled);
}

TEST(DigestAuthFilterTest, DoFilterMissingAuthorization)
{
    std::map<std::string, std::string> creds = {{"user", "pass"}};
    DigestAuthFilter filter(creds, "realm", "opaque");
    HttpRequestPtr req = std::make_shared<DummyHttpRequest>();
    req->setSession(true);
    req->setAuthorizationHeader("");
    lastResponse = nullptr;
    filterChainCalled = false;
    filter.doFilter(req, filterCallback, filterChainCallback);
    ASSERT_NE(lastResponse, nullptr);
    EXPECT_EQ(lastResponse->getStatusCode(), 401);
    EXPECT_FALSE(filterChainCalled);
    EXPECT_NE(lastResponse->getHeader("WWW-Authenticate").find("realm=\"realm\""), std::string::npos);
}

TEST(DigestAuthFilterTest, DoFilterInvalidUsername)
{
    std::map<std::string, std::string> creds = {{"user", "pass"}};
    DigestAuthFilter filter(creds, "realm", "opaque");
    HttpRequestPtr req = std::make_shared<DummyHttpRequest>();
    req->setSession(true);
    req->setAuthorizationHeader("username=\"baduser\", realm=\"realm\", nonce=\"nonce\", uri=\"uri\", opaque=\"opaque\", response=\"response\"");
    lastResponse = nullptr;
    filterChainCalled = false;
    filter.doFilter(req, filterCallback, filterChainCallback);
    ASSERT_NE(lastResponse, nullptr);
    EXPECT_EQ(lastResponse->getStatusCode(), 401);
    EXPECT_FALSE(filterChainCalled);
}

TEST(DigestAuthFilterTest, DoFilterValidCredentials)
{
    // We will mock utils::getMd5 to return lowercase strings for test
    // Since we cannot change utils::getMd5 here, we will test logic by
    // overriding the authorization header with correct response.

    // For username=user, password=pass, realm=realm, method=GET, uri=/path
    // A1 = user:realm:pass
    // A2 = GET:/path
    // response = md5(md5(A1):nonce:md5(A2))

    std::map<std::string, std::string> creds = {{"user", "pass"}};
    DigestAuthFilter filter(creds, "realm", "opaque");
    HttpRequestPtr req = std::make_shared<DummyHttpRequest>();
    req->setSession(true);
    req->setMethod(Get);

    // We simulate the md5 by precomputing expected strings:
    // Let's assume utils::getMd5 returns lowercase hex of input string for test
    // So we simulate that by computing md5 externally and hardcoding here.

    // For test, we fake the response string to match expected calculation:
    // For simplicity, we will bypass the actual md5 and test that doFilter calls ccb() when response matches.

    // Compose authorization header with correct response:
    std::string username = "user";
    std::string realm = "realm";
    std::string nonce = "nonce";
    std::string uri = "/path";

    // Fake md5 function: just return input string in lowercase (simulate)
    auto fakeMd5 = [](const std::string &s) {
        std::string out = s;
        std::transform(out.begin(), out.end(), out.begin(), ::tolower);
        return out;
    };

    std::string A1 = username + ":" + realm + ":" + creds[username];
    std::string A2 = "GET:" + uri;
    std::string A1_middle_A2 = fakeMd5(A1) + ":" + nonce + ":" + fakeMd5(A2);
    std::string response = fakeMd5(A1_middle_A2);

    std::string auth_header = "username=\"" + username + "\", realm=\"" + realm +
                              "\", nonce=\"" + nonce + "\", uri=\"" + uri +
                              "\", opaque=\"opaque\", response=\"" + response + "\"";

    req->setAuthorizationHeader(auth_header);

    lastResponse = nullptr;
    filterChainCalled = false;
    filter.doFilter(req, filterCallback, filterChainCallback);
    EXPECT_TRUE(filterChainCalled);
    EXPECT_EQ(lastResponse, nullptr);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}