#ifndef CREATE_VIEW_H
#define CREATE_VIEW_H

#include <string>
#include <vector>

namespace drogon_ctl
{
class create_view
{
  public:
    void handleCommand(std::vector<std::string> &parameters);
    void createViewFiles(std::vector<std::string> &cspFileNames);
    int createViewFile(const std::string &script_filename);

  private:
    void newViewHeaderFile(std::ofstream &file, const std::string &className);
    void newViewSourceFile(std::ofstream &file,
                           const std::string &className,
                           const std::string &namespacePrefix,
                           std::ifstream &infile);

    std::string outputPath_;
    std::vector<std::string> namespaces_;
    bool pathToNamespaceFlag_{false};
};
}  // namespace drogon_ctl

#endif  // CREATE_VIEW_H