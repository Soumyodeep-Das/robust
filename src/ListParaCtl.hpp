#ifndef LISTPARACTL_HPP
#define LISTPARACTL_HPP

#include <functional>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>

class ListParaCtl
{
public:
    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};

#endif // LISTPARACTL_HPP