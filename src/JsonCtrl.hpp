#ifndef JSONCTRL_HPP
#define JSONCTRL_HPP

#include <functional>
#include <json/json.h>

class HttpRequestPtr;
class HttpResponsePtr;
class HttpResponse
{
public:
    static HttpResponsePtr newHttpJsonResponse(const Json::Value &);
};

class JsonCtrl
{
public:
    void asyncHandleHttpRequest(
        const HttpRequestPtr &,
        std::function<void(const HttpResponsePtr &)> &&callback);
};

#endif // JSONCTRL_HPP