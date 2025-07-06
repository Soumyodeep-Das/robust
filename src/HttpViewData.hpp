#ifndef DROGON_HTTPVIEWDATA_HPP
#define DROGON_HTTPVIEWDATA_HPP

#include <string>
#include <cstddef>

namespace drogon
{
class HttpViewData
{
  public:
    static std::string htmlTranslate(const char *str, size_t length);
};

}  // namespace drogon

#endif  // DROGON_HTTPVIEWDATA_HPP