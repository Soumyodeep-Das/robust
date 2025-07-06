#ifndef SLOWCTRL_HPP
#define SLOWCTRL_HPP

#include <drogon/HttpController.h>
#include <functional>
#include <string>

class SlowCtrl : public drogon::HttpController<SlowCtrl>
{
public:
    void hello(const drogon::HttpRequestPtr &req,
               std::function<void(const drogon::HttpResponsePtr &)> &&callback,
               std::string &&userid);

    drogon::AsyncTask observe(drogon::HttpRequestPtr req,
                             std::function<void(const drogon::HttpResponsePtr &)> callback,
                             std::string userid);
};

#endif // SLOWCTRL_HPP