#ifndef HELLOVIEWCONTROLLER_HPP
#define HELLOVIEWCONTROLLER_HPP

#include <drogon/HttpSimpleController.h>
#include <drogon/HttpResponse.h>
#include <functional>

using namespace drogon;

class HelloViewController : public HttpSimpleController<HelloViewController>
{
  public:
    PATH_LIST_BEGIN
    PATH_ADD("/view")
    PATH_LIST_END

    void asyncHandleHttpRequest(
        const HttpRequestPtr &req,
        std::function<void(const HttpResponsePtr &)> &&callback) override;
};

#endif // HELLOVIEWCONTROLLER_HPP