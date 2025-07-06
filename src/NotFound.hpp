#ifndef NOTFOUND_HPP
#define NOTFOUND_HPP

#include <string>

namespace drogon
{
class HttpViewData;

class NotFound
{
  public:
    static std::string genText(const HttpViewData &NotFound_view_data);
};

}  // namespace drogon

#endif  // NOTFOUND_HPP