#include "help.hpp"
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

namespace drogon_ctl
{
class helpTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // Redirect std::cout to capture output for testing
        old_buf = std::cout.rdbuf();
        std::cout.rdbuf(ss.rdbuf());
    }

    void TearDown() override
    {
        // Restore std::cout buffer
        std::cout.rdbuf(old_buf);
    }

    std::string getOutput()
    {
        return ss.str();
    }

    void clearOutput()
    {
        ss.str("");
        ss.clear();
    }

  private:
    std::stringstream ss;
    std::streambuf *old_buf;
};

TEST_F(helpTest, HandleCommandNoParameters)
{
    help h;
    std::vector<std::string> params;
    h.handleCommand(params);
    std::string output = getOutput();
    // Check for usage string and commands list string presence
    EXPECT_NE(output.find("usage: drogon_ctl"), std::string::npos);
    EXPECT_NE(output.find("commands list:"), std::string::npos);
}

TEST_F(helpTest, HandleCommandWithUnknownCommand)
{
    help h;
    std::vector<std::string> params{"nonexistent_command"};
    h.handleCommand(params);
    std::string output = getOutput();
    // For unknown command, no output expected
    EXPECT_TRUE(output.empty());
}

}  // namespace drogon_ctl