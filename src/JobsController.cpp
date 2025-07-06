#include "JobsController.hpp"

namespace drogon
{

JobsController::JobsController(const Json::Value &json)
{
    if (json.isMember("id") && json["id"].isInt())
    {
        id_ = json["id"].asInt();
    }
    if (json.isMember("title") && json["title"].isString())
    {
        title_ = json["title"].asString();
    }
}

int JobsController::getValueOfId() const
{
    return id_;
}

std::string JobsController::getValueOfTitle() const
{
    return title_;
}

JobsController JobsController::fromRequest(const HttpRequest &req)
{
    return JobsController(req.getJsonObject());
}

} // namespace drogon