#ifndef CACHEFILE_HPP
#define CACHEFILE_HPP

#include <string>
#include <cstddef>

namespace drogon
{
class CacheFile
{
  public:
    CacheFile(const std::string &path, bool autoDelete);
    ~CacheFile();

    void append(const char *data, size_t length);
    size_t length();
    char *data();

  private:
    bool autoDelete_;
    std::string path_;
    FILE *file_{nullptr};
    char *data_{nullptr};
    size_t dataLength_{0};
};
}  // namespace drogon

#endif  // CACHEFILE_HPP