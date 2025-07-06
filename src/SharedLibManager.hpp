#ifndef SHARED_LIB_MANAGER_HPP
#define SHARED_LIB_MANAGER_HPP

#include <functional>
#include <string>
#include <vector>
#include <sys/stat.h>

namespace drogon
{
class SharedLibManager
{
  public:
    explicit SharedLibManager(const std::vector<std::string> &libPaths,
                              const std::string &outputPath);
    ~SharedLibManager();

    void managerLibs();

  private:
    struct DLStat
    {
#if defined __linux__ || defined __HAIKU__
        struct timespec mTime;
#elif defined _WIN32
        struct
        {
            time_t tv_sec;
        } mTime;
#else
        struct timespec mTime;
#endif
        void *handle{nullptr};
    };

    void *compileAndLoadLib(const std::string &sourceFile, void *oldHld);
    bool shouldCompileLib(const std::string &soFile, const struct stat &sourceStat);
    void *loadLib(const std::string &soFile, void *oldHld);

    std::vector<std::string> libPaths_;
    std::string outputPath_;

    class WorkingThreadWrapper;
    WorkingThreadWrapper workingThread_;

    int timeId_{-1};

    std::unordered_map<std::string, DLStat> dlMap_;
};

}  // namespace drogon

#endif  // SHARED_LIB_MANAGER_HPP