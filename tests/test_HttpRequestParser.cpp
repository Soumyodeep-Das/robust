#include "gtest/gtest.h"
#include "HttpRequestParser.hpp"
#include <memory>

using namespace drogon;

class HttpRequestParserTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Setup a dummy TcpConnectionPtr using a mock or nullptr for testing
        // Here we use nullptr because actual TcpConnection is complex to mock
        // and this test focuses on HttpRequestParser interface.
        // In real tests, a proper mock or fixture would be used.
        connPtr = nullptr;
    }

    void TearDown() override {}

    std::shared_ptr<HttpRequestParser> createParser()
    {
        // Since TcpConnectionPtr is a shared_ptr, nullptr is acceptable for interface testing
        return std::make_shared<HttpRequestParser>(connPtr);
    }

    trantor::TcpConnectionPtr connPtr;
};

TEST_F(HttpRequestParserTest, ConstructorAndReset)
{
    auto parser = createParser();
    ASSERT_NE(parser, nullptr);
    parser->reset();
}

TEST_F(HttpRequestParserTest, ProcessRequestLineValid)
{
    auto parser = createParser();
    parser->reset();
    const char *line = "GET /path?query HTTP/1.1";
    bool result = parser->processRequestLine(line, line + strlen(line));
    EXPECT_TRUE(result);
}

TEST_F(HttpRequestParserTest, ProcessRequestLineInvalid)
{
    auto parser = createParser();
    parser->reset();
    const char *line = "GET /path HTTP/2.0";
    bool result = parser->processRequestLine(line, line + strlen(line));
    EXPECT_FALSE(result);
}

TEST_F(HttpRequestParserTest, MakeRequestForPoolReturnsSharedPtr)
{
    auto parser = createParser();
    parser->reset();
    HttpRequestImpl *rawPtr = new HttpRequestImpl(nullptr);
    auto reqPtr = HttpRequestParser::makeRequestForPool(rawPtr);
    EXPECT_NE(reqPtr, nullptr);
}

TEST_F(HttpRequestParserTest, PushAndPopRequestPipelining)
{
    auto parser = createParser();
    parser->reset();
    HttpRequestPtr req = std::make_shared<HttpRequestImpl>(nullptr);
    parser->pushRequestToPipelining(req);
    auto firstReq = parser->getFirstRequest();
    EXPECT_EQ(firstReq, req);
    parser->popFirstRequest();
    EXPECT_EQ(parser->getFirstRequest(), nullptr);
}

TEST_F(HttpRequestParserTest, PushResponseToPipelining)
{
    auto parser = createParser();
    parser->reset();
    HttpRequestPtr req = std::make_shared<HttpRequestImpl>(nullptr);
    HttpResponsePtr resp = HttpResponse::newHttpResponse();
    parser->pushRequestToPipelining(req);
    parser->pushResponseToPipelining(req, resp, true);
    auto respPair = parser->getFirstResponse();
    EXPECT_EQ(respPair.first, resp);
    EXPECT_TRUE(respPair.second);
}

TEST_F(HttpRequestParserTest, ParseRequestEmptyBuffer)
{
    auto parser = createParser();
    parser->reset();
    trantor::MsgBuffer buf;
    bool result = parser->parseRequest(&buf);
    EXPECT_TRUE(result);
}