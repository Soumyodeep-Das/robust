#include "TestPlugin.hpp"
#include <gtest/gtest.h>
#include <json/json.h>
#include <chrono>
#include <thread>

using namespace drogon;

class TestPluginTest : public ::testing::Test
{
protected:
    TestPlugin plugin;
};

TEST_F(TestPluginTest, InitAndStartWithNullConfig)
{
    Json::Value nullConfig;
    nullConfig = Json::Value(); // null
    plugin.initAndStart(nullConfig);
    // Since the plugin runs a thread, we shutdown immediately to avoid hanging
    plugin.shutdown();
    SUCCEED();
}

TEST_F(TestPluginTest, InitAndStartWithCustomInterval)
{
    Json::Value config;
    config["heartbeat_interval"] = 1;
    plugin.initAndStart(config);
    // Let the thread run briefly
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    plugin.shutdown();
    SUCCEED();
}

TEST_F(TestPluginTest, ShutdownStopsThread)
{
    Json::Value config;
    config["heartbeat_interval"] = 1;
    plugin.initAndStart(config);
    plugin.shutdown();
    // If shutdown completes, thread joined successfully
    SUCCEED();
}