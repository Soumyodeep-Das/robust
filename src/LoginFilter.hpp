#ifndef LOGINFILTER_HPP
#define LOGINFILTER_HPP

#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <functional>

class LoginFilter
{
public:
    void doFilter(const drogon::HttpRequestPtr &req,
                  std::function<void(const drogon::HttpResponsePtr &)> &&fcb,
                  std::function<void()> &&fccb);
};

#endif // LOGINFILTER_HPP