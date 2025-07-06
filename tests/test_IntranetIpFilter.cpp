#include "IntranetIpFilter.hpp"
#include <gtest/gtest.h>

using namespace drogon;

class MockPeerAddr
{
  public:
    explicit MockPeerAddr(bool intranet) : intranet_(intranet) {}
    bool isIntranetIp() const { return intranet_; }

  private:
    bool intranet_;
};

class MockHttpRequest : public HttpRequest
{
  public:
    explicit MockHttpRequest(bool intranet) : peerAddr_(intranet) {}
    const MockPeerAddr &peerAddr() const { return peerAddr_; }

  private:
    MockPeerAddr peerAddr_;
};

class IntranetIpFilterTest : public ::testing::Test
{
  protected:
    IntranetIpFilter filter_;
};

TEST_F(IntranetIpFilterTest, DoFilter_IntranetIp_CallsFilterChainCallback)
{
    MockHttpRequest req(true);
    bool filterCallbackCalled = false;
    bool filterChainCallbackCalled = false;

    filter_.doFilter(
        std::make_shared<MockHttpRequest>(req),
        [&](const HttpResponsePtr &) { filterCallbackCalled = true; },
        [&]() { filterChainCallbackCalled = true; });

    EXPECT_FALSE(filterCallbackCalled);
    EXPECT_TRUE(filterChainCallbackCalled);
}

TEST_F(IntranetIpFilterTest, DoFilter_NonIntranetIp_CallsFilterCallback)
{
    MockHttpRequest req(false);
    bool filterCallbackCalled = false;
    bool filterChainCallbackCalled = false;

    filter_.doFilter(
        std::make_shared<MockHttpRequest>(req),
        [&](const HttpResponsePtr &res) {
            filterCallbackCalled = true;
            EXPECT_EQ(res->statusCode(), k404NotFound);
        },
        [&]() { filterChainCallbackCalled = true; });

    EXPECT_TRUE(filterCallbackCalled);
    EXPECT_FALSE(filterChainCallbackCalled);
}