#include "Cookie.hpp"
#include <gtest/gtest.h>

namespace drogon
{

class CookieTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CookieTest, CookieStringBasic)
{
    Cookie cookie;
    // Since key_, value_ and other members are private and no setters are provided,
    // we cannot set them directly. This test only checks that cookieString() runs without crash.
    // For full testing, public setters or constructor should be added.
    std::string result = cookie.cookieString();
    // The result should start with "Set-Cookie: "
    ASSERT_NE(result.find("Set-Cookie: "), std::string::npos);
}

}  // namespace drogon