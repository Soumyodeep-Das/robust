#ifndef DONOTHINGPLUGIN_HPP
#define DONOTHINGPLUGIN_HPP

#include <json/json.h>

namespace drogon
{
class DoNothingPlugin
{
  public:
    void initAndStart(const Json::Value &config);
    void shutdown();
};

}  // namespace drogon

#endif  // DONOTHINGPLUGIN_HPP