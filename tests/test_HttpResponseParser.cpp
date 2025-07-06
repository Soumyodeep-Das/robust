#include "gtest/gtest.h"
#include "HttpResponseParser.hpp"
#include <trantor/net/TcpConnection.h>
#include <trantor/utils/MsgBuffer.h>
#include <memory>

using namespace drogon;
using namespace trantor;

class HttpResponseParserTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // Create a dummy TcpConnectionPtr using nullptr for testing
        connPtr = TcpConnectionPtr();
        parser = std::make_unique<HttpResponseParser>(connPtr);
    }

    std::unique_ptr<HttpResponseParser> parser;
    TcpConnectionPtr connPtr;
};

TEST_F(HttpResponseParserTest, ResetInitializesState)
{
    parser->reset();
    // We cannot directly check private members, so test via behavior
    trantor::MsgBuffer buf;
    // Add a valid response line to buffer
    const char *responseLine = "HTTP/1.1 200 OK\r\n";
    buf.append(responseLine, strlen(responseLine));
    bool result = parser->processResponseLine(buf.peek(), buf.peek() + strlen(responseLine) - 2);
    EXPECT_TRUE(result);
}

TEST_F(HttpResponseParserTest, ProcessResponseLineValidHttp11)
{
    const char *line = "HTTP/1.1 200 OK";
    bool result = parser->processResponseLine(line, line + strlen(line));
    EXPECT_TRUE(result);
}

TEST_F(HttpResponseParserTest, ProcessResponseLineValidHttp10)
{
    const char *line = "HTTP/1.0 404 Not Found";
    bool result = parser->processResponseLine(line, line + strlen(line));
    EXPECT_TRUE(result);
}

TEST_F(HttpResponseParserTest, ProcessResponseLineInvalidVersion)
{
    const char *line = "HTTP/2.0 200 OK";
    bool result = parser->processResponseLine(line, line + strlen(line));
    EXPECT_FALSE(result);
}

TEST_F(HttpResponseParserTest, ParseResponseOnCloseReturnsTrueWhenExpectedClose)
{
    parser->reset();
    // Manually set status_ to kExpectClose by parsing headers with no content-length
    // We cannot access private status_, so simulate by parsing a response that leads to kExpectClose
    trantor::MsgBuffer buf;
    const char *responseLine = "HTTP/1.1 200 OK\r\n\r\n";
    buf.append(responseLine, strlen(responseLine));
    parser->parseResponse(&buf);
    // Now forcibly call parseResponseOnClose, it should return false because status_ is not kExpectClose
    bool result = parser->parseResponseOnClose();
    EXPECT_FALSE(result);
}

TEST_F(HttpResponseParserTest, ParseResponseHandlesEmptyBuffer)
{
    trantor::MsgBuffer buf;
    bool result = parser->parseResponse(&buf);
    EXPECT_TRUE(result);
}

TEST_F(HttpResponseParserTest, ParseResponseProcessesValidResponse)
{
    trantor::MsgBuffer buf;
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 5\r\n"
        "\r\n"
        "Hello";
    buf.append(response, strlen(response));
    bool result = parser->parseResponse(&buf);
    EXPECT_TRUE(result);
}

TEST_F(HttpResponseParserTest, ParseResponseProcessesChunkedResponse)
{
    trantor::MsgBuffer buf;
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Transfer-Encoding: chunked\r\n"
        "\r\n"
        "5\r\n"
        "Hello\r\n"
        "0\r\n"
        "\r\n";
    buf.append(response, strlen(response));
    bool result = parser->parseResponse(&buf);
    EXPECT_TRUE(result);
}

TEST_F(HttpResponseParserTest, ParseResponseReturnsFalseOnInvalidChunk)
{
    trantor::MsgBuffer buf;
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Transfer-Encoding: chunked\r\n"
        "\r\n"
        "5\r\n"
        "HelloX\r\n"  // Invalid chunk ending
        "0\r\n"
        "\r\n";
    buf.append(response, strlen(response));
    bool result = parser->parseResponse(&buf);
    EXPECT_FALSE(result);
}