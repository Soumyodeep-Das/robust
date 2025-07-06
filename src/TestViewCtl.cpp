#include "TestViewCtl.hpp"

void TestViewCtl::asyncHandleHttpRequest(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback)
{
    // write your application logic here
    drogon::HttpViewData data;
    data.insert("title", std::string("TestView"));
    auto res = drogon::HttpResponse::newHttpViewResponse("TestView", data);
    callback(res);
}