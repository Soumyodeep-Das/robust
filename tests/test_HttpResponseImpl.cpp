#include "gtest/gtest.h"
#include "HttpResponseImpl.hpp"
#include <json/json.h>

using namespace drogon;

class HttpResponseImplTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(HttpResponseImplTest, VersionString)
{
    HttpResponseImpl resp;
    resp.setVersion(HttpResponse::Version::kHttp10);
    EXPECT_STREQ(resp.versionString(), "HTTP/1.0");
    resp.setVersion(HttpResponse::Version::kHttp11);
    EXPECT_STREQ(resp.versionString(), "HTTP/1.1");
}

TEST_F(HttpResponseImplTest, GenerateBodyFromJson)
{
    HttpResponseImpl resp;
    Json::Value val;
    val["key"] = "value";
    resp.setJsonObject(val);
    resp.generateBodyFromJson();
    EXPECT_FALSE(resp.getBody().empty());
}

TEST_F(HttpResponseImplTest, AddHeaderAndCookies)
{
    HttpResponseImpl resp;
    const char *header = "Content-Type: application/json";
    resp.addHeader(header, strchr(header, ':'), header + strlen(header));
    EXPECT_EQ(resp.getHeaderBy("content-type"), "application/json");

    const char *cookieHeader = "Set-Cookie: key=value; Path=/; HttpOnly";
    resp.addHeader(cookieHeader, strchr(cookieHeader, ':'), cookieHeader + strlen(cookieHeader));
    auto cookie = resp.getCookie("key");
    EXPECT_EQ(cookie.key(), "key");
    EXPECT_EQ(cookie.value(), "value");
    EXPECT_TRUE(cookie.httpOnly());
    EXPECT_EQ(cookie.path(), "/");
}

TEST_F(HttpResponseImplTest, ClearAndSwap)
{
    HttpResponseImpl resp1;
    resp1.setStatusCode(k200OK);
    resp1.setVersion(HttpResponse::Version::kHttp11);
    resp1.setStatusMessage("OK");

    HttpResponseImpl resp2;
    resp2.clear();

    resp1.swap(resp2);

    EXPECT_EQ(resp2.statusCode(), k200OK);
    EXPECT_EQ(resp2.version(), HttpResponse::Version::kHttp11);
    EXPECT_EQ(resp2.statusMessage(), "OK");
    EXPECT_EQ(resp1.statusCode(), kUnknown);
}

TEST_F(HttpResponseImplTest, ParseJsonValidAndInvalid)
{
    HttpResponseImpl resp;
    resp.setBody("{\"key\":\"value\"}");
    resp.parseJson();
    EXPECT_NE(resp.jsonObject(), nullptr);
    EXPECT_TRUE(resp.jsonParsingError().empty());

    resp.setBody("invalid json");
    resp.parseJson();
    EXPECT_EQ(resp.jsonObject(), nullptr);
    EXPECT_FALSE(resp.jsonParsingError().empty());
}

TEST_F(HttpResponseImplTest, ShouldBeCompressed)
{
    HttpResponseImpl resp;
    resp.setBody("short");
    EXPECT_FALSE(resp.shouldBeCompressed());

    resp.setBody(std::string(2000, 'a'));
    resp.setContentTypeCode(CT_TEXT_HTML);
    EXPECT_TRUE(resp.shouldBeCompressed());

    resp.setSendfile("filename");
    EXPECT_FALSE(resp.shouldBeCompressed());
}

TEST_F(HttpResponseImplTest, SetContentTypeString)
{
    HttpResponseImpl resp;
    resp.setContentTypeString("application/json", strlen("application/json"));
    EXPECT_EQ(resp.contentTypeCode(), CT_APPLICATION_JSON);
    EXPECT_EQ(resp.getHeaderBy("content-type"), "");
}