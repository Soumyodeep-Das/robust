#include "gtest/gtest.h"
#include "AccessLogger.hpp"
#include <drogon/drogon.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <trantor/utils/Logger.h>
#include <trantor/net/InetAddress.h>
#include <trantor/utils/Date.h>
#include <sstream>

using namespace drogon;
using namespace drogon::plugin;

class AccessLoggerTest : public ::testing::Test
{
protected:
    AccessLogger logger_;
};

class DummyRequest : public HttpRequest
{
public:
    DummyRequest()
    {
        path_ = "/test/path";
        query_ = "key=value";
        version_ = Version::kHttp11;
        method_ = Method::Get;
        body_ = "bodycontent";
        peerAddr_ = trantor::InetAddress("127.0.0.1", 12345);
        localAddr_ = trantor::InetAddress("192.168.1.1", 80);
        creationDate_ = trantor::Date::now();
        headers_["X-Test-Header"] = "HeaderValue";
        cookies_["sessionid"] = "abc123";
    }
    std::string path() const override { return path_; }
    std::string query() const override { return query_; }
    Version version() const override { return version_; }
    std::string versionString() const override
    {
        switch (version_)
        {
        case Version::kHttp10:
            return "HTTP/1.0";
        case Version::kHttp11:
            return "HTTP/1.1";
        case Version::kHttp2:
            return "HTTP/2";
        default:
            return "HTTP/1.1";
        }
    }
    Method method() const override { return method_; }
    std::string methodString() const override
    {
        switch (method_)
        {
        case Method::Get:
            return "GET";
        case Method::Post:
            return "POST";
        default:
            return "GET";
        }
    }
    const std::string &body() const override { return body_; }
    const trantor::InetAddress &peerAddr() const override { return peerAddr_; }
    const trantor::InetAddress &localAddr() const override { return localAddr_; }
    trantor::Date creationDate() const override { return creationDate_; }
    std::string getHeader(const std::string &key) const override
    {
        auto it = headers_.find(key);
        if (it != headers_.end())
            return it->second;
        return "";
    }
    std::string getCookie(const std::string &key) const override
    {
        auto it = cookies_.find(key);
        if (it != cookies_.end())
            return it->second;
        return "";
    }

private:
    std::string path_;
    std::string query_;
    Version version_;
    Method method_;
    std::string body_;
    trantor::InetAddress peerAddr_;
    trantor::InetAddress localAddr_;
    trantor::Date creationDate_;
    std::map<std::string, std::string> headers_;
    std::map<std::string, std::string> cookies_;
};

class DummyResponse : public HttpResponse
{
public:
    DummyResponse()
    {
        body_ = "responsebody";
        statusCode_ = 200;
        contentType_ = "text/html";
        headers_["Content-Type"] = contentType_;
    }
    const std::string &body() const override { return body_; }
    int getStatusCode() const override { return statusCode_; }
    std::string contentTypeString() const override { return contentType_; }
    std::string getHeader(const std::string &key) const override
    {
        auto it = headers_.find(key);
        if (it != headers_.end())
            return it->second;
        return "";
    }

private:
    std::string body_;
    int statusCode_;
    std::string contentType_;
    std::map<std::string, std::string> headers_;
};

TEST_F(AccessLoggerTest, OutputReqPath)
{
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputReqPath(stream, std::make_shared<DummyRequest>(req),
                               std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), "/test/path");
}

TEST_F(AccessLoggerTest, OutputReqQuery)
{
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputReqQuery(stream, std::make_shared<DummyRequest>(req),
                                std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), "key=value");
}

TEST_F(AccessLoggerTest, OutputReqURLWithQuery)
{
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputReqURL(stream, std::make_shared<DummyRequest>(req),
                              std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), "/test/path?key=value");
}

TEST_F(AccessLoggerTest, OutputReqURLWithoutQuery)
{
    DummyRequest req;
    DummyResponse resp;
    req = DummyRequest();
    // Override query to empty
    struct EmptyQueryRequest : DummyRequest
    {
        std::string query() const override { return ""; }
    } emptyReq;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputReqURL(stream, std::make_shared<EmptyQueryRequest>(emptyReq),
                              std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), "/test/path");
}

TEST_F(AccessLoggerTest, OutputVersion)
{
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputVersion(stream, std::make_shared<DummyRequest>(req),
                               std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), "HTTP/1.1");
}

TEST_F(AccessLoggerTest, OutputReqLineWithQuery)
{
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputReqLine(stream, std::make_shared<DummyRequest>(req),
                               std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), "GET /test/path?key=value HTTP/1.1");
}

TEST_F(AccessLoggerTest, OutputReqLineWithoutQuery)
{
    DummyRequest req;
    DummyResponse resp;
    struct EmptyQueryRequest : DummyRequest
    {
        std::string query() const override { return ""; }
    } emptyReq;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputReqLine(stream, std::make_shared<EmptyQueryRequest>(emptyReq),
                               std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), "GET /test/path HTTP/1.1");
}

TEST_F(AccessLoggerTest, OutputRemoteAddr)
{
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputRemoteAddr(stream, std::make_shared<DummyRequest>(req),
                                  std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), "127.0.0.1:12345");
}

TEST_F(AccessLoggerTest, OutputLocalAddr)
{
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputLocalAddr(stream, std::make_shared<DummyRequest>(req),
                                 std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), "192.168.1.1:80");
}

TEST_F(AccessLoggerTest, OutputReqLength)
{
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputReqLength(stream, std::make_shared<DummyRequest>(req),
                                 std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), std::to_string(req.body().length()));
}

TEST_F(AccessLoggerTest, OutputRespLength)
{
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputRespLength(stream, std::make_shared<DummyRequest>(req),
                                  std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), std::to_string(resp.body().length()));
}

TEST_F(AccessLoggerTest, OutputMethod)
{
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputMethod(stream, std::make_shared<DummyRequest>(req),
                              std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), "GET");
}

TEST_F(AccessLoggerTest, OutputReqHeader)
{
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputReqHeader(stream, std::make_shared<DummyRequest>(req),
                                 "X-Test-Header");
    EXPECT_EQ(ss.str(), "X-Test-Header: HeaderValue");
}

TEST_F(AccessLoggerTest, OutputReqCookie)
{
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputReqCookie(stream, std::make_shared<DummyRequest>(req),
                                 "sessionid");
    EXPECT_EQ(ss.str(), "(cookie)sessionid=abc123");
}

TEST_F(AccessLoggerTest, OutputRespHeader)
{
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputRespHeader(stream, std::make_shared<DummyResponse>(resp),
                                  "Content-Type");
    EXPECT_EQ(ss.str(), "Content-Type: text/html");
}

TEST_F(AccessLoggerTest, OutputStatusString)
{
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputStatusString(stream, std::make_shared<DummyRequest>(req),
                                    std::make_shared<DummyResponse>(resp));
    std::string expected = std::to_string(resp.getStatusCode()) + " " +
                           statusCodeToString(resp.getStatusCode());
    EXPECT_EQ(ss.str(), expected);
}

TEST_F(AccessLoggerTest, OutputStatusCode)
{
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputStatusCode(stream, std::make_shared<DummyRequest>(req),
                                  std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), std::to_string(resp.getStatusCode()));
}

TEST_F(AccessLoggerTest, OutputProcessingTime)
{
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputProcessingTime(stream, std::make_shared<DummyRequest>(req),
                                      std::make_shared<DummyResponse>(resp));
    std::string output = ss.str();
    // Should be a floating point number > 0
    double val = 0.0;
    EXPECT_TRUE((std::istringstream(output) >> val) && val >= 0.0);
}

TEST_F(AccessLoggerTest, OutputRespContentType)
{
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    AccessLogger::outputRespContentType(stream, std::make_shared<DummyRequest>(req),
                                       std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), resp.contentTypeString());
}

TEST_F(AccessLoggerTest, NewLogFunctionKnownPlaceholder)
{
    logger_.initAndStart(Json::Value());
    auto func = logger_.newLogFunction("$request_path");
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    func(stream, std::make_shared<DummyRequest>(req), std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), "/test/path");
}

TEST_F(AccessLoggerTest, NewLogFunctionHttpHeader)
{
    logger_.initAndStart(Json::Value());
    auto func = logger_.newLogFunction("$http_X-Test-Header");
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    func(stream, std::make_shared<DummyRequest>(req), std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), "X-Test-Header: HeaderValue");
}

TEST_F(AccessLoggerTest, NewLogFunctionCookie)
{
    logger_.initAndStart(Json::Value());
    auto func = logger_.newLogFunction("$cookie_sessionid");
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    func(stream, std::make_shared<DummyRequest>(req), std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), "(cookie)sessionid=abc123");
}

TEST_F(AccessLoggerTest, NewLogFunctionUpstreamHttpHeader)
{
    logger_.initAndStart(Json::Value());
    auto func = logger_.newLogFunction("$upstream_http_Content-Type");
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    func(stream, std::make_shared<DummyRequest>(req), std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), "Content-Type: text/html");
}

TEST_F(AccessLoggerTest, NewLogFunctionUnknownPlaceholder)
{
    logger_.initAndStart(Json::Value());
    auto func = logger_.newLogFunction("$unknown_placeholder");
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    func(stream, std::make_shared<DummyRequest>(req), std::make_shared<DummyResponse>(resp));
    EXPECT_EQ(ss.str(), "$unknown_placeholder");
}

TEST_F(AccessLoggerTest, CreateLogFunctionsAddsFunctions)
{
    std::string format = "$request_path $method $status";
    logger_.createLogFunctions(format);
    EXPECT_FALSE(logger_.logFunctions_.empty());
}

TEST_F(AccessLoggerTest, LoggingCallsAllFunctions)
{
    std::string format = "$request_path $method $status";
    logger_.createLogFunctions(format);
    DummyRequest req;
    DummyResponse resp;
    std::stringstream ss;
    trantor::LogStream stream(ss);
    logger_.logging(stream, std::make_shared<DummyRequest>(req), std::make_shared<DummyResponse>(resp));
    std::string output = ss.str();
    EXPECT_NE(output.find("/test/path"), std::string::npos);
    EXPECT_NE(output.find("GET"), std::string::npos);
    EXPECT_NE(output.find(std::to_string(resp.getStatusCode())), std::string::npos);
}