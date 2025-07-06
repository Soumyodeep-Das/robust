#ifndef TESTVIEWCTL_HPP
#define TESTVIEWCTL_HPP

#include <functional>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>

class TestViewCtl
{
public:
    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};

#endif // TESTVIEWCTL_HPP