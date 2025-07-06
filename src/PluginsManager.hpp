#ifndef DROGON_PLUGINS_MANAGER_HPP
#define DROGON_PLUGINS_MANAGER_HPP

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <json/json.h>
#include "PluginBase.h"
#include "DrClassMap.h"

namespace drogon
{
class PluginsManager
{
  public:
    ~PluginsManager();

    void initializeAllPlugins(
        const Json::Value &configs,
        const std::function<void(PluginBase *)> &forEachCallback);

    PluginBase *getPlugin(const std::string &pluginName);

  private:
    std::vector<PluginBase *> initializedPlugins_;
    std::unordered_map<std::string, std::unique_ptr<PluginBase>> pluginsMap_;
};
}  // namespace drogon

#endif  // DROGON_PLUGINS_MANAGER_HPP