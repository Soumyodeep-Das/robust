#include "ListParaCtl.hpp"

void ListParaCtl::asyncHandleHttpRequest(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback)
{
    // write your application logic here
    drogon::HttpViewData data;
    data.insert("title", "list parameters");
    data.insert("parameters", req->getParameters());
    auto res =
        drogon::HttpResponse::newHttpViewResponse("ListParaView.csp", data);
    callback(res);
}