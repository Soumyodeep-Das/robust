#include "gtest/gtest.h"
#include "DoNothingPlugin.hpp"
#include <json/json.h>

namespace drogon
{
class DoNothingPluginTest : public ::testing::Test
{
  protected:
    DoNothingPlugin plugin;
};

TEST_F(DoNothingPluginTest, InitAndStartDoesNotThrow)
{
    Json::Value config;
    EXPECT_NO_THROW(plugin.initAndStart(config));
}

TEST_F(DoNothingPluginTest, ShutdownDoesNotThrow)
{
    EXPECT_NO_THROW(plugin.shutdown());
}

}  // namespace drogon