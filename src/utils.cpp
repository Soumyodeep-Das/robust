#include "utils.hpp"

Json::Value makeErrResp(const std::string &errorMsg)
{
    Json::Value resp;
    resp["error"] = errorMsg;
    return resp;
}

void badRequest(std::function<void(const drogon::HttpResponsePtr &)> callback,
                const std::string &errorMsg,
                drogon::HttpStatusCode code)
{
    auto resp = drogon::HttpResponse::newHttpJsonResponse(makeErrResp(errorMsg));
    resp->setStatusCode(code);
    callback(resp);
}