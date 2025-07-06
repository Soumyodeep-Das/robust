#include "create_filter.hpp"
#include <gtest/gtest.h>
#include <vector>
#include <string>

namespace drogon_ctl
{
class CreateFilterTest : public ::testing::Test
{
  protected:
    create_filter filter;
};

TEST_F(CreateFilterTest, HandleCommandWithEmptyParameters)
{
    std::vector<std::string> params;
    testing::internal::CaptureStdout();
    filter.handleCommand(params);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Invalid parameters!"), std::string::npos);
}

TEST_F(CreateFilterTest, HandleCommandCreatesFiles)
{
    // This test cannot fully test file creation or user input without mocks.
    // We test that no exceptions thrown and output contains expected text for valid input.
    std::vector<std::string> params = {"MyNamespace::MyFilter"};
    // Redirect stdin to simulate user input 'y' for overwrite prompt if needed
    // But since files likely don't exist, no prompt expected.
    testing::internal::CaptureStdout();
    filter.handleCommand(params);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("create a http filter:"), std::string::npos);
    EXPECT_NE(output.find("MyNamespace::MyFilter"), std::string::npos);
}

}  // namespace drogon_ctl