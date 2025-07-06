#include "main.hpp"

#include <drogon/drogon.h>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace drogon;

void A::handle(const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback,
               int p1,
               const std::string &p2,
               const std::string &p3,
               int p4) const
{
    HttpViewData data;
    data.insert("title", std::string("ApiTest::get"));
    std::unordered_map<std::string, std::string> para;
    para["int p1"] = std::to_string(p1);
    para["string p2"] = p2;
    para["string p3"] = p3;
    para["int p4"] = std::to_string(p4);

    data.insert("parameters", para);
    auto res = HttpResponse::newHttpViewResponse("ListParaView", data);
    callback(res);
}

void A::staticHandle(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    int p1,
    const std::string &p2,
    const std::string &p3,
    int p4)
{
    HttpViewData data;
    data.insert("title", std::string("ApiTest::get"));
    std::unordered_map<std::string, std::string> para;
    para["int p1"] = std::to_string(p1);
    para["string p2"] = p2;
    para["string p3"] = p3;
    para["int p4"] = std::to_string(p4);

    data.insert("parameters", para);
    auto res = HttpResponse::newHttpViewResponse("ListParaView", data);
    callback(res);
}

void B::operator()(const HttpRequestPtr &req,
                   std::function<void(const HttpResponsePtr &)> &&callback,
                   int p1,
                   int p2)
{
    HttpViewData data;
    data.insert("title", std::string("ApiTest::get"));
    std::unordered_map<std::string, std::string> para;
    para["p1"] = std::to_string(p1);
    para["p2"] = std::to_string(p2);
    data.insert("parameters", para);
    auto res = HttpResponse::newHttpViewResponse("ListParaView", data);
    callback(res);
}

void C::priv(const HttpRequestPtr &req,
             std::function<void(const HttpResponsePtr &)> &&callback) const
{
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody("<P>private content, only for authenticated users</P>");
    callback(resp);
}

namespace api
{
namespace v1
{
void Test::get(const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback,
               int p1,
               int p2) const
{
    HttpViewData data;
    data.insert("title", std::string("ApiTest::get"));
    std::unordered_map<std::string, std::string> para;
    para["p1"] = std::to_string(p1);
    para["p2"] = std::to_string(p2);
    data.insert("parameters", para);
    auto res = HttpResponse::newHttpViewResponse("ListParaView", data);
    callback(res);
}

void Test::list(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback,
                int p1,
                int p2) const
{
    HttpViewData data;
    data.insert("title", std::string("ApiTest::get"));
    std::unordered_map<std::string, std::string> para;
    para["p1"] = std::to_string(p1);
    para["p2"] = std::to_string(p2);
    data.insert("parameters", para);
    auto res = HttpResponse::newHttpViewResponse("ListParaView", data);
    callback(res);
}
}  // namespace v1
}  // namespace api

namespace drogon
{
template <>
std::string_view fromRequest(const HttpRequest &req)
{
    return req.body();
}
}  // namespace drogon