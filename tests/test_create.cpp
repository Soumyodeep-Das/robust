#include "gtest/gtest.h"
#include "create.hpp"

using namespace drogon_ctl;

class CreateTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CreateTest, DetailReturnsExpectedString)
{
    std::string detailStr = create::detail();
    // Check that the returned string contains expected substrings
    EXPECT_NE(detailStr.find("Use create command"), std::string::npos);
    EXPECT_NE(detailStr.find("Usage:drogon_ctl create"), std::string::npos);
    EXPECT_NE(detailStr.find("create view"), std::string::npos);
    EXPECT_NE(detailStr.find("create controller"), std::string::npos);
    EXPECT_NE(detailStr.find("create filter"), std::string::npos);
    EXPECT_NE(detailStr.find("create plugin"), std::string::npos);
    EXPECT_NE(detailStr.find("create project"), std::string::npos);
    EXPECT_NE(detailStr.find("create model"), std::string::npos);
}

TEST_F(CreateTest, HandleCommandModifiesParametersCorrectly)
{
    create c;
    std::vector<std::string> params = {"controller", "other", "args"};
    c.handleCommand(params);
    // The first parameter should be prefixed with "create_"
    EXPECT_EQ(params[0], "create_controller");
    // The rest of the parameters remain unchanged
    EXPECT_EQ(params[1], "other");
    EXPECT_EQ(params[2], "args");
}