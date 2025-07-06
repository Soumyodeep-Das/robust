#include "MethodTest.hpp"
#include <drogon/HttpResponse.h>
#include <drogon/HttpRequest.h>
#include <trantor/utils/Logger.h>

static void makeGetRespose(
    const std::function<void(const drogon::HttpResponsePtr &)> &callback)
{
    callback(drogon::HttpResponse::newHttpJsonResponse("GET"));
}

static void makePostRespose(
    const std::function<void(const drogon::HttpResponsePtr &)> &callback)
{
    callback(drogon::HttpResponse::newHttpJsonResponse("POST"));
}

void MethodTest::get(const drogon::HttpRequestPtr &req,
                    std::function<void(const drogon::HttpResponsePtr &)> &&callback)
{
    LOG_DEBUG;
    makeGetRespose(callback);
}

void MethodTest::post(const drogon::HttpRequestPtr &req,
                     std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                     std::string str)
{
    LOG_DEBUG << str;
    makePostRespose(callback);
}

void MethodTest::getReg(const drogon::HttpRequestPtr &req,
                       std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                       std::string regStr)
{
    LOG_DEBUG << regStr;
    makeGetRespose(callback);
}

void MethodTest::postReg(const drogon::HttpRequestPtr &req,
                        std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                        std::string regStr,
                        std::string str)
{
    LOG_DEBUG << regStr;
    LOG_DEBUG << str;
    makePostRespose(callback);
}

void MethodTest::postRegex(const drogon::HttpRequestPtr &req,
                          std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                          std::string regStr)
{
    LOG_DEBUG << regStr;
    makePostRespose(callback);
}