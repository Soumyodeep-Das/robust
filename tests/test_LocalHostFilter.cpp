#include "LocalHostFilter.hpp"
#include <gtest/gtest.h>

using namespace drogon;

class LocalHostFilterTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

class DummyPeerAddr
{
  public:
    explicit DummyPeerAddr(bool isLoopback) : _isLoopback(isLoopback) {}
    bool isLoopbackIp() const { return _isLoopback; }

  private:
    bool _isLoopback;
};

class DummyHttpRequest
{
  public:
    explicit DummyHttpRequest(bool isLoopback) : _peerAddr(isLoopback) {}
    DummyPeerAddr peerAddr() const { return _peerAddr; }

  private:
    DummyPeerAddr _peerAddr;
};

TEST_F(LocalHostFilterTest, DoFilter_LoopbackCallsFilterChainCallback)
{
    LocalHostFilter filter;
    DummyHttpRequest req(true);  // loopback ip

    bool filterChainCalled = false;
    bool filterCallbackCalled = false;

    filter.doFilter(
        req,
        [&](const HttpResponsePtr &) { filterCallbackCalled = true; },
        [&]() { filterChainCalled = true; });

    EXPECT_TRUE(filterChainCalled);
    EXPECT_FALSE(filterCallbackCalled);
}

TEST_F(LocalHostFilterTest, DoFilter_NonLoopbackCallsFilterCallbackWithNotFound)
{
    LocalHostFilter filter;
    DummyHttpRequest req(false);  // not loopback ip

    bool filterChainCalled = false;
    bool filterCallbackCalled = false;
    bool gotNotFoundResponse = false;

    filter.doFilter(
        req,
        [&](const HttpResponsePtr &res) {
            filterCallbackCalled = true;
            if (res && res->statusCode() == k404NotFound)
            {
                gotNotFoundResponse = true;
            }
        },
        [&]() { filterChainCalled = true; });

    EXPECT_FALSE(filterChainCalled);
    EXPECT_TRUE(filterCallbackCalled);
    EXPECT_TRUE(gotNotFoundResponse);
}