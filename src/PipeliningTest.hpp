#ifndef PIPELININGTEST_HPP
#define PIPELININGTEST_HPP

#include <functional>
#include <memory>

class HttpRequest;
using HttpRequestPtr = std::shared_ptr<HttpRequest>;

class HttpResponse;
using HttpResponsePtr = std::shared_ptr<HttpResponse>;

class PipeliningTest
{
public:
    void asyncHandleHttpRequest(
        const HttpRequestPtr &req,
        std::function<void(const HttpResponsePtr &)> &&callback);
};

#endif // PIPELININGTEST_HPP