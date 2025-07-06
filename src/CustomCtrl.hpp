#ifndef CUSTOMCTRL_HPP
#define CUSTOMCTRL_HPP

#include <string>
#include <functional>

class HttpRequestPtr;
class HttpResponsePtr;
class HttpResponse;

class CustomCtrl {
public:
    void hello(const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback,
               const std::string &userName) const;

private:
    std::string greetings_;
};

#endif // CUSTOMCTRL_HPP