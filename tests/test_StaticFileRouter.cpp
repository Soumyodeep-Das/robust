#include "gtest/gtest.h"
#include "StaticFileRouter.hpp"
#include <memory>
#include <string>
#include <functional>

using namespace drogon;

class StaticFileRouterTest : public ::testing::Test
{
protected:
    StaticFileRouter router;

    void SetUp() override
    {
        // Setup code if needed
    }

    void TearDown() override
    {
        // Cleanup code if needed
    }
};

TEST_F(StaticFileRouterTest, SetFileTypesClearsAndInserts)
{
    std::vector<std::string> types = {"html", "css", "js"};
    router.setFileTypes(types);
    // No direct getter, so no direct assert possible here
    // Just ensure no crash and function executes
    SUCCEED();
}

TEST_F(StaticFileRouterTest, DefaultHandlerReturnsNotFound)
{
    bool callbackCalled = false;
    router.defaultHandler(nullptr, [&](const HttpResponsePtr &resp) {
        ASSERT_NE(resp, nullptr);
        ASSERT_EQ(resp->statusCode(), k404NotFound);
        callbackCalled = true;
    });
    ASSERT_TRUE(callbackCalled);
}

TEST_F(StaticFileRouterTest, SendStaticFileResponseMethodNotGet)
{
    // Create a dummy HttpRequestImplPtr with method != Get
    class DummyRequest : public HttpRequestImpl
    {
    public:
        HttpMethod method() const override { return Post; }
        const std::string &getHeaderBy(const std::string &) const override
        {
            static std::string empty;
            return empty;
        }
        const std::string &path() const override
        {
            static std::string p = "/test";
            return p;
        }
    };
    auto req = std::make_shared<DummyRequest>();
    bool callbackCalled = false;
    router.sendStaticFileResponse("dummy", req,
                                  [&](const HttpResponsePtr &resp) {
                                      ASSERT_NE(resp, nullptr);
                                      ASSERT_EQ(resp->statusCode(),
                                                k405MethodNotAllowed);
                                      callbackCalled = true;
                                  },
                                  "");
    ASSERT_TRUE(callbackCalled);
}

TEST_F(StaticFileRouterTest, RoutePathTraversalForbidden)
{
    class DummyRequest : public HttpRequestImpl
    {
    public:
        HttpMethod method() const override { return Get; }
        const std::string &getHeaderBy(const std::string &) const override
        {
            static std::string empty;
            return empty;
        }
        const std::string &path() const override
        {
            static std::string p = "/../etc/passwd";
            return p;
        }
    };
    auto req = std::make_shared<DummyRequest>();
    bool callbackCalled = false;
    router.route(req, [&](const HttpResponsePtr &resp) {
        ASSERT_NE(resp, nullptr);
        ASSERT_EQ(resp->statusCode(), k403Forbidden);
        callbackCalled = true;
    });
    ASSERT_TRUE(callbackCalled);
}