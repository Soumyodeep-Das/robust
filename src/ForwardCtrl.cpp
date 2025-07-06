#include "ForwardCtrl.hpp"
#include <drogon/drogon.h>

void ForwardCtrl::asyncHandleHttpRequest(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback)
{
    req->setPath("/repos/an-tao/drogon/git/refs/heads/master");
    drogon::app().forward(
        req,
        [callback = std::move(callback)](const drogon::HttpResponsePtr &resp) {
            callback(resp);
        },
        "https://api.github.com");
}