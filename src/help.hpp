#ifndef HELP_HPP
#define HELP_HPP

#include <string>
#include <vector>

namespace drogon_ctl
{
class help
{
  public:
    void handleCommand(std::vector<std::string> &parameters);
};

}  // namespace drogon_ctl

#endif  // HELP_HPP