#ifndef CUSTOMHEADERFILTER_HPP
#define CUSTOMHEADERFILTER_HPP

#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <functional>

namespace drogon
{
class CustomHeaderFilter
{
  public:
    void doFilter(const HttpRequestPtr &req,
                  std::function<void(const HttpResponsePtr &)> &&fcb,
                  std::function<void()> &&fccb);

  private:
    std::string field_;
    std::string value_;
};
}  // namespace drogon

#endif  // CUSTOMHEADERFILTER_HPP