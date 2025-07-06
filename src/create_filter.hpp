#ifndef CREATE_FILTER_HPP
#define CREATE_FILTER_HPP

#include <string>
#include <vector>

namespace drogon_ctl
{
class create_filter
{
  public:
    void handleCommand(std::vector<std::string> &parameters);
};

}  // namespace drogon_ctl

#endif  // CREATE_FILTER_HPP