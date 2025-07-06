#ifndef CREATE_PLUGIN_HPP
#define CREATE_PLUGIN_HPP

#include <string>
#include <vector>

namespace drogon_ctl
{
class create_plugin
{
  public:
    void handleCommand(std::vector<std::string> &parameters);
};

}  // namespace drogon_ctl

#endif  // CREATE_PLUGIN_HPP