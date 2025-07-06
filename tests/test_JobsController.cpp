#include "gtest/gtest.h"
#include "JobsController.hpp"

namespace drogon
{

class MockHttpRequest : public HttpRequest
{
public:
    Json::Value getJsonObject() const override
    {
        Json::Value json;
        json["id"] = 1;
        json["title"] = "Engineer";
        return json;
    }
};

TEST(JobsControllerTest, TestFromRequestWithValidJson)
{
    MockHttpRequest req;
    auto job = JobsController::fromRequest(req);
    ASSERT_EQ(job.getValueOfId(), 1);
    ASSERT_EQ(job.getValueOfTitle(), "Engineer");
}

} // namespace drogon