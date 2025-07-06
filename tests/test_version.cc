#include "version.hpp"
#include <gtest/gtest.h>
#include <vector>
#include <string>

class VersionTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(VersionTest, HandleCommandDoesNotThrow)
{
    drogon_ctl::version v;
    std::vector<std::string> params;
    EXPECT_NO_THROW(v.handleCommand(params));
}

TEST_F(VersionTest, HandleCommandWithParametersDoesNotThrow)
{
    drogon_ctl::version v;
    std::vector<std::string> params = {"param1", "param2"};
    EXPECT_NO_THROW(v.handleCommand(params));
}