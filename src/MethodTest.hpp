#ifndef METHODTEST_HPP
#define METHODTEST_HPP

#include <functional>
#include <string>
#include <drogon/HttpController.h>

class MethodTest
{
public:
    void get(const drogon::HttpRequestPtr &req,
             std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void post(const drogon::HttpRequestPtr &req,
              std::function<void(const drogon::HttpResponsePtr &)> &&callback,
              std::string str);
    void getReg(const drogon::HttpRequestPtr &req,
                std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                std::string regStr);
    void postReg(const drogon::HttpRequestPtr &req,
                 std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                 std::string regStr,
                 std::string str);
    void postRegex(const drogon::HttpRequestPtr &req,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                   std::string regStr);
};

#endif // METHODTEST_HPP