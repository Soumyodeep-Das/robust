#ifndef CREATE_PROJECT_HPP
#define CREATE_PROJECT_HPP

#include <string>
#include <vector>

namespace drogon_ctl
{
class create_project
{
  public:
    void handleCommand(std::vector<std::string> &parameters);
    void createProject(const std::string &projectName);
};

}  // namespace drogon_ctl

#endif  // CREATE_PROJECT_HPP