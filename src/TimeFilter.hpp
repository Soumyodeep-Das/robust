#ifndef TIMEFILTER_HPP
#define TIMEFILTER_HPP

#include <trantor/net/HttpRequest.h>
#include <trantor/net/HttpResponse.h>
#include <trantor/utils/Date.h>
#include <functional>

class TimeFilter
{
public:
    using HttpRequestPtr = trantor::HttpRequestPtr;
    using FilterCallback = std::function<void(const trantor::HttpResponsePtr &)>;
    using FilterChainCallback = std::function<void()>;

    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&cb,
                  FilterChainCallback &&ccb);
};

#endif // TIMEFILTER_HPP