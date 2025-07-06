#ifndef MAIN_HPP
#define MAIN_HPP

#include <drogon/HttpController.h>
#include <drogon/HttpTypes.h>
#include <functional>
#include <string>

class A : public drogon::DrObjectBase
{
  public:
    void handle(const drogon::HttpRequestPtr &req,
                std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                int p1,
                const std::string &p2,
                const std::string &p3,
                int p4) const;
    static void staticHandle(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback,
        int p1,
        const std::string &p2,
        const std::string &p3,
        int p4);
};

class B : public drogon::DrObjectBase
{
  public:
    void operator()(const drogon::HttpRequestPtr &req,
                    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                    int p1,
                    int p2);
};

class C : public drogon::HttpController<C>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(C::priv, "/priv/resource", Get, "DigestAuthFilter");
    METHOD_LIST_END
    void priv(const drogon::HttpRequestPtr &req,
              std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;
};

namespace api
{
namespace v1
{
class Test : public drogon::HttpController<Test>
{
  public:
    METHOD_LIST_BEGIN
    METHOD_ADD(Test::get,
               "get/{2}/{1}",
               Get);
    METHOD_ADD(Test::list,
               "/{2}/info",
               Get);
    METHOD_LIST_END
    void get(const drogon::HttpRequestPtr &req,
             std::function<void(const drogon::HttpResponsePtr &)> &&callback,
             int p1,
             int p2) const;
    void list(const drogon::HttpRequestPtr &req,
              std::function<void(const drogon::HttpResponsePtr &)> &&callback,
              int p1,
              int p2) const;
};
}  // namespace v1
}  // namespace api

namespace drogon
{
template <>
std::string_view fromRequest(const drogon::HttpRequest &req);
}  // namespace drogon

#endif // MAIN_HPP