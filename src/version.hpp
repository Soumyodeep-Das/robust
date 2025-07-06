#ifndef VERSION_HPP
#define VERSION_HPP

#include <vector>
#include <string>

namespace drogon_ctl
{
class version
{
  public:
    void handleCommand(std::vector<std::string> &parameters);
};

}  // namespace drogon_ctl

#endif  // VERSION_HPP