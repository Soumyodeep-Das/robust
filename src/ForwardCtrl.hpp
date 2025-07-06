#ifndef FORWARDCTRL_HPP
#define FORWARDCTRL_HPP

#include <functional>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>

class ForwardCtrl
{
public:
    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};

#endif // FORWARDCTRL_HPP