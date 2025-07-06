#include "api_v1_CoroTest.hpp"
#include <drogon/drogon.h>
#include <trantor/utils/Logger.h>
#include <trantor/net/EventLoop.h>
#include <drogon/HttpResponse.h>
#include <drogon/utils/Utilities.h>
#include <chrono>

using namespace api::v1;

drogon::Task<> CoroTest::get(drogon::HttpRequestPtr req,
                             std::function<void(const drogon::HttpResponsePtr &)> callback)
{
    // Force co_await to test awaiting works
    co_await drogon::sleepCoro(
        trantor::EventLoop::getEventLoopOfCurrentThread(),
        std::chrono::milliseconds(100));

    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setBody("DEADBEEF");
    callback(resp);
    co_return;
}

drogon::Task<drogon::HttpResponsePtr> CoroTest::get2(drogon::HttpRequestPtr req)
{
    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setBody("BADDBEEF");
    co_return resp;
}

drogon::Task<> CoroTest::this_will_fail(drogon::HttpRequestPtr req,
                                        std::function<void(const drogon::HttpResponsePtr &)> callback)
{
    throw std::runtime_error("This is an excpected exception");
    callback(drogon::HttpResponse::newHttpResponse());
}

drogon::Task<drogon::HttpResponsePtr> CoroTest::this_will_fail2(drogon::HttpRequestPtr req)
{
    throw std::runtime_error("This is an excpected exception");
    co_return drogon::HttpResponse::newHttpResponse();
}