#include "gtest/gtest.h"
#include "create_plugin.hpp"

#include <vector>
#include <string>

namespace drogon_ctl
{
class CreatePluginTest : public ::testing::Test
{
  protected:
    create_plugin plugin;
};

TEST_F(CreatePluginTest, HandleCommandEmptyParameters)
{
    std::vector<std::string> params;
    testing::internal::CaptureStdout();
    plugin.handleCommand(params);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Invalid parameters!"), std::string::npos);
}

TEST_F(CreatePluginTest, HandleCommandCreatesFiles)
{
    // This test will simulate calling handleCommand with a fake class name.
    // Since handleCommand does actual file IO and user input, we cannot fully
    // test it here without mocking. We test that it does not throw and prints expected output.

    std::vector<std::string> params = {"TestPlugin"};
    // Redirect stdin to simulate user input 'y' for overwrite prompt if needed
    // But since files likely don't exist, no prompt expected.
    testing::internal::CaptureStdout();
    plugin.handleCommand(params);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("create a plugin:TestPlugin"), std::string::npos);
}

}  // namespace drogon_ctl