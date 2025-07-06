#ifndef DROGON_MULTIPART_HPP
#define DROGON_MULTIPART_HPP

#include <drogon/HttpTypes.h>
#include <drogon/HttpFile.h>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace drogon
{
class MultiPartParser
{
  public:
    const std::vector<HttpFile> &getFiles() const;
    std::unordered_map<std::string, HttpFile> getFilesMap() const;
    const std::map<std::string, std::string> &getParameters() const;

    int parse(const HttpRequestPtr &req);
    int parse(const HttpRequestPtr &req,
              const char *boundaryData,
              size_t boundaryLen);
    int parseEntity(const char *begin, const char *end);

  private:
    HttpRequestPtr requestPtr_;
    std::vector<HttpFile> files_;
    std::map<std::string, std::string> parameters_;
};
}  // namespace drogon

#endif  // DROGON_MULTIPART_HPP