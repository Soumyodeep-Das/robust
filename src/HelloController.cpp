#include "HelloController.hpp"

void SayHello::genericHello(const HttpRequestPtr &,
                            std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody(
        "Hello, this is a generic hello message from the SayHello "
        "controller");
    callback(resp);
}

void SayHello::personalizedHello(const HttpRequestPtr &,
                                std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody(
        "Hi there, this is another hello from the SayHello Controller");
    callback(resp);
}