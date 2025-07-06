#include "PluginsManager.hpp"
#include <gtest/gtest.h>
#include <json/json.h>

using namespace drogon;

class PluginsManagerTest : public ::testing::Test
{
protected:
    PluginsManager pluginsManager;

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

class DummyPlugin : public PluginBase
{
public:
    DummyPlugin() : initialized_(false), shutdown_(false) {}
    void initialize() override { initialized_ = true; if (initializedCallback_) initializedCallback_(this);}
    void shutdown() override { shutdown_ = true; }
    void setConfig(const Json::Value &config) override { config_ = config; }
    void addDependency(PluginBase *plugin) override { dependencies_.push_back(plugin); }
    void setInitializedCallback(std::function<void(PluginBase *)> cb) override { initializedCallback_ = cb; }
    std::string className() const override { return "DummyPlugin"; }

    bool initialized() const { return initialized_; }
    bool shutdownCalled() const { return shutdown_; }
    Json::Value config() const { return config_; }
    std::vector<PluginBase *> dependencies() const { return dependencies_; }

private:
    bool initialized_;
    bool shutdown_;
    Json::Value config_;
    std::vector<PluginBase *> dependencies_;
    std::function<void(PluginBase *)> initializedCallback_;
};

// Mock DrClassMap::newObject to return DummyPlugin for test
namespace drogon
{
namespace DrClassMap
{
inline void *newObject(const std::string &className)
{
    if (className == "DummyPlugin")
        return new DummyPlugin();
    return nullptr;
}
}  // namespace DrClassMap
}  // namespace drogon

TEST_F(PluginsManagerTest, GetPluginReturnsPlugin)
{
    PluginBase *plugin = pluginsManager.getPlugin("DummyPlugin");
    ASSERT_NE(plugin, nullptr);
    EXPECT_EQ(plugin->className(), "DummyPlugin");
}

TEST_F(PluginsManagerTest, GetPluginReturnsSameInstance)
{
    PluginBase *plugin1 = pluginsManager.getPlugin("DummyPlugin");
    PluginBase *plugin2 = pluginsManager.getPlugin("DummyPlugin");
    EXPECT_EQ(plugin1, plugin2);
}

TEST_F(PluginsManagerTest, InitializeAllPluginsProcessesPlugins)
{
    Json::Value configs(Json::arrayValue);
    Json::Value pluginConfig;
    pluginConfig["name"] = "DummyPlugin";
    pluginConfig["config"] = Json::Value();
    pluginConfig["dependencies"] = Json::Value(Json::arrayValue);
    configs.append(pluginConfig);

    int callbackCount = 0;
    pluginsManager.initializeAllPlugins(configs, [&](PluginBase *p) {
        callbackCount++;
        EXPECT_EQ(p->className(), "DummyPlugin");
    });

    EXPECT_EQ(callbackCount, 1);
}

TEST_F(PluginsManagerTest, DestructorCallsShutdown)
{
    PluginBase *plugin = pluginsManager.getPlugin("DummyPlugin");
    DummyPlugin *dummy = dynamic_cast<DummyPlugin *>(plugin);
    ASSERT_NE(dummy, nullptr);

    // Initialize to add to initializedPlugins_
    Json::Value configs(Json::arrayValue);
    Json::Value pluginConfig;
    pluginConfig["name"] = "DummyPlugin";
    pluginConfig["config"] = Json::Value();
    pluginConfig["dependencies"] = Json::Value(Json::arrayValue);
    configs.append(pluginConfig);

    pluginsManager.initializeAllPlugins(configs, [](PluginBase *) {});

    // Destructor should call shutdown on dummy plugin
    pluginsManager.~PluginsManager();
    EXPECT_TRUE(dummy->shutdownCalled());
}