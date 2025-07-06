#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <functional>
#include <json/json.h>
#include <drogon/HttpResponse.h>

Json::Value makeErrResp(const std::string &errorMsg);

void badRequest(std::function<void(const drogon::HttpResponsePtr &)> callback,
                const std::string &errorMsg,
                drogon::HttpStatusCode code);

#endif // UTILS_HPP