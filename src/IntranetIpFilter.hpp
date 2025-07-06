#ifndef INTRANETIPFILTER_HPP
#define INTRANETIPFILTER_HPP

#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <functional>

namespace drogon
{
class IntranetIpFilter
{
  public:
    using HttpRequestPtr = std::shared_ptr<const HttpRequest>;
    using FilterCallback = std::function<void(const HttpResponsePtr &)>;
    using FilterChainCallback = std::function<void()>;

    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&fcb,
                  FilterChainCallback &&fccb);
};

}  // namespace drogon

#endif  // INTRANETIPFILTER_HPP