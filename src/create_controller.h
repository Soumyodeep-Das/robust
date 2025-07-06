#ifndef CREATE_CONTROLLER_H
#define CREATE_CONTROLLER_H

#include <string>
#include <vector>
#include <fstream>

namespace drogon_ctl
{

enum ControllerType
{
    Simple,
    Http,
    WebSocket,
    Restful
};

class create_controller
{
  public:
    void handleCommand(std::vector<std::string> &parameters);

    void newSimpleControllerHeaderFile(std::ofstream &file,
                                       const std::string &className);
    void newSimpleControllerSourceFile(std::ofstream &file,
                                       const std::string &className,
                                       const std::string &filename);

    void newWebsockControllerHeaderFile(std::ofstream &file,
                                        const std::string &className);
    void newWebsockControllerSourceFile(std::ofstream &file,
                                        const std::string &className,
                                        const std::string &filename);

    void newHttpControllerHeaderFile(std::ofstream &file,
                                    const std::string &className);
    void newHttpControllerSourceFile(std::ofstream &file,
                                    const std::string &className,
                                    const std::string &filename);

    void createController(std::vector<std::string> &httpClasses,
                          ControllerType type);
    void createController(const std::string &className, ControllerType type);

    void createARestfulController(const std::string &className,
                                  const std::string &resource);
};

}  // namespace drogon_ctl

#endif  // CREATE_CONTROLLER_H