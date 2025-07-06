#include "gtest/gtest.h"
#include "HttpRequestImpl.hpp"
#include <json/json.h>
#include <string>

using namespace drogon;

class HttpRequestImplTest : public ::testing::Test
{
protected:
    HttpRequestImpl req;

    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(HttpRequestImplTest, SetMethodValid)
{
    EXPECT_TRUE(req.setMethod("GET", "GET" + 3));
    EXPECT_STREQ(req.methodString(), "GET");

    EXPECT_TRUE(req.setMethod("POST", "POST" + 4));
    EXPECT_STREQ(req.methodString(), "POST");

    EXPECT_TRUE(req.setMethod("HEAD", "HEAD" + 4));
    EXPECT_STREQ(req.methodString(), "HEAD");

    EXPECT_TRUE(req.setMethod("PUT", "PUT" + 3));
    EXPECT_STREQ(req.methodString(), "PUT");

    EXPECT_TRUE(req.setMethod("DELETE", "DELETE" + 6));
    EXPECT_STREQ(req.methodString(), "DELETE");

    EXPECT_TRUE(req.setMethod("OPTIONS", "OPTIONS" + 7));
    EXPECT_STREQ(req.methodString(), "OPTIONS");

    EXPECT_TRUE(req.setMethod("PATCH", "PATCH" + 5));
    EXPECT_STREQ(req.methodString(), "PATCH");
}

TEST_F(HttpRequestImplTest, SetMethodInvalid)
{
    EXPECT_FALSE(req.setMethod("INVALID", "INVALID" + 7));
    EXPECT_STREQ(req.methodString(), "UNKNOWN");
}

TEST_F(HttpRequestImplTest, VersionString)
{
    req.setVersion(HttpRequest::Version::kHttp10);
    EXPECT_STREQ(req.versionString(), "HTTP/1.0");

    req.setVersion(HttpRequest::Version::kHttp11);
    EXPECT_STREQ(req.versionString(), "HTTP/1.1");
}

TEST_F(HttpRequestImplTest, AddHeaderCookieParsing)
{
    const char *header = "cookie: key1=value1; key2=value2; key3=value3";
    req.addHeader(header, header + 6, header + strlen(header));
    EXPECT_EQ(req.cookies().size(), 3);
    EXPECT_EQ(req.cookies().at("key1"), "value1");
    EXPECT_EQ(req.cookies().at("key2"), "value2");
    EXPECT_EQ(req.cookies().at("key3"), "value3");
}

TEST_F(HttpRequestImplTest, ParseJsonValid)
{
    Json::Value val;
    val["name"] = "test";
    auto reqPtr = HttpRequest::newHttpJsonRequest(val);
    auto *impl = dynamic_cast<HttpRequestImpl *>(reqPtr.get());
    ASSERT_NE(impl, nullptr);
    impl->parseJson();
    ASSERT_NE(impl->getJsonObject(), nullptr);
    EXPECT_EQ((*impl->getJsonObject())["name"].asString(), "test");
}

TEST_F(HttpRequestImplTest, ParseJsonInvalid)
{
    req.setContent("invalid json");
    req.setContentTypeString("application/json", strlen("application/json"));
    req.parseJson();
    EXPECT_EQ(req.getJsonObject(), nullptr);
    EXPECT_NE(req.getJsonParsingError(), nullptr);
}

TEST_F(HttpRequestImplTest, ReserveBodySizeAndAppend)
{
    size_t smallSize = 10;
    req.reserveBodySize(smallSize);
    req.appendToBody("12345", 5);
    EXPECT_EQ(req.getContent().size(), 5);

    size_t largeSize = HttpAppFrameworkImpl::instance().getClientMaxMemoryBodySize() + 1;
    req.reserveBodySize(largeSize);
    req.appendToBody("12345", 5);
    // Since large size triggers file cache, content should be empty
    EXPECT_TRUE(req.getContent().empty());
}

TEST_F(HttpRequestImplTest, AppendToBufferBasic)
{
    req.setMethod("GET", "GET" + 3);
    req.setVersion(HttpRequest::Version::kHttp11);
    req.setPath("/test");
    trantor::MsgBuffer buffer;
    req.appendToBuffer(&buffer);
    std::string result(buffer.peek(), buffer.readableBytes());
    EXPECT_NE(result.find("GET /test HTTP/1.1\r\n"), std::string::npos);
}

TEST_F(HttpRequestImplTest, SetContentTypeString)
{
    req.setContentTypeString("application/json", strlen("application/json"));
    EXPECT_EQ(req.getContentType(), CT_APPLICATION_JSON);

    req.setContentTypeString("unknown/type", strlen("unknown/type"));
    EXPECT_EQ(req.getContentType(), CT_CUSTOM);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}