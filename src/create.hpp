#ifndef CREATE_HPP
#define CREATE_HPP

#include <string>
#include <vector>

namespace drogon_ctl
{
class create
{
  public:
    static std::string detail();
    void handleCommand(std::vector<std::string> &parameters);
};
}  // namespace drogon_ctl

#endif  // CREATE_HPP