#ifndef JOBSCONTROLLER_HPP
#define JOBSCONTROLLER_HPP

#include <string>
#include <json/json.h>

namespace drogon
{
class HttpRequest
{
public:
    virtual ~HttpRequest() = default;
    virtual Json::Value getJsonObject() const = 0;
};

class JobsController
{
public:
    JobsController() = default;
    explicit JobsController(const Json::Value &json);

    int getValueOfId() const;
    std::string getValueOfTitle() const;

    static JobsController fromRequest(const HttpRequest &req);

private:
    int id_{0};
    std::string title_;
};

} // namespace drogon

#endif // JOBSCONTROLLER_HPP