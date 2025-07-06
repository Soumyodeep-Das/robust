#ifndef LOCALHOSTFILTER_HPP
#define LOCALHOSTFILTER_HPP

#include <drogon/HttpTypes.h>
#include <functional>

namespace drogon
{
class LocalHostFilter
{
  public:
    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&fcb,
                  FilterChainCallback &&fccb);
};

}  // namespace drogon

#endif  // LOCALHOSTFILTER_HPP