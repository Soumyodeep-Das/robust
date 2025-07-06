#include "SlowCtrl.hpp"
#include "RedisCache.h"
#include "DateFuncs.h"
#include <drogon/drogon.h>
#define VDate "visitDate"

void SlowCtrl::hello(const drogon::HttpRequestPtr &req,
                     std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                     std::string &&userid)
{
    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setBody("hello, " + userid);
    callback(resp);
}

drogon::AsyncTask SlowCtrl::observe(
    drogon::HttpRequestPtr req,
    std::function<void(const drogon::HttpResponsePtr &)> callback,
    std::string userid)
{
    auto key = userid + "." + VDate;
    auto redisPtr = drogon::app().getFastRedisClient();
    try
    {
        auto date = co_await getFromCache<trantor::Date>(key, redisPtr);
        auto resp = drogon::HttpResponse::newHttpResponse();
        std::string body{"your last visit time: "};
        body.append(date.toFormattedStringLocal(false));
        resp->setBody(std::move(body));
        callback(resp);
    }
    catch (const std::exception &err)
    {
        LOG_ERROR << err.what();
        callback(drogon::HttpResponse::newNotFoundResponse());
    }
}