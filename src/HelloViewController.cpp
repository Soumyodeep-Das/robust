#include "HelloViewController.hpp"

void HelloViewController::asyncHandleHttpRequest(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback)
{
    HttpViewData data;
    data["name"] = req->getParameter("name");
    auto resp = HttpResponse::newHttpViewResponse("HelloView", data);
    callback(resp);
}