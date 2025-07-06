#ifndef HELLOCONTROLLER_HPP
#define HELLOCONTROLLER_HPP

#include <drogon/HttpController.h>

using namespace drogon;

class SayHello : public HttpController<SayHello>
{
  public:
    METHOD_LIST_BEGIN
    METHOD_ADD(SayHello::genericHello, "/", Get);
    METHOD_ADD(SayHello::personalizedHello, "/hello", Get);
    METHOD_LIST_END

  protected:
    void genericHello(const HttpRequestPtr &,
                      std::function<void(const HttpResponsePtr &)> &&callback);

    void personalizedHello(const HttpRequestPtr &,
                           std::function<void(const HttpResponsePtr &)> &&callback);
};

#endif // HELLOCONTROLLER_HPP