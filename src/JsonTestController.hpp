#ifndef JSONTESTCONTROLLER_HPP
#define JSONTESTCONTROLLER_HPP

#include <functional>
#include <json/json.h>

class HttpRequestPtr;
class HttpResponsePtr;
class HttpResponse;

class JsonTestController
{
public:
    void asyncHandleHttpRequest(
        const HttpRequestPtr &req,
        std::function<void(const HttpResponsePtr &)> &&callback);
};

#endif // JSONTESTCONTROLLER_HPP