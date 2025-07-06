#include "gtest/gtest.h"
#include "PersonInfo.hpp"
#include "drogon/orm/Row.h"
#include "json/json.h"
#include "trantor/utils/Date.h"

namespace drogon_model {
namespace org_chart {

class MockRow : public drogon::orm::Row
{
public:
    MockRow() : drogon::orm::Row() {}
    bool isNull(size_t idx) const override { return true; }
};

} // namespace org_chart
} // namespace drogon_model

TEST(PersonInfoTest, DefaultValuesWithNullRow) {
    drogon_model::org_chart::MockRow r; // Row with all null values
    drogon_model::org_chart::PersonInfo pi(r, 0);

    // Check integer fields
    EXPECT_EQ(pi.getValueOfId(), 0);
    EXPECT_EQ(pi.getId(), nullptr);
    EXPECT_EQ(pi.getValueOfJobId(), 0);
    EXPECT_EQ(pi.getJobId(), nullptr);
    EXPECT_EQ(pi.getValueOfDepartmentId(), 0);
    EXPECT_EQ(pi.getDepartmentId(), nullptr);
    EXPECT_EQ(pi.getValueOfManagerId(), 0);
    EXPECT_EQ(pi.getManagerId(), nullptr);

    // Check string fields
    EXPECT_EQ(pi.getValueOfJobTitle(), "");
    EXPECT_EQ(pi.getJobTitle(), nullptr);
    EXPECT_EQ(pi.getValueOfDepartmentName(), "");
    EXPECT_EQ(pi.getDepartmentName(), nullptr);
    EXPECT_EQ(pi.getValueOfManagerFullName(), "");
    EXPECT_EQ(pi.getManagerFullName(), nullptr);
    EXPECT_EQ(pi.getValueOfFirstName(), "");
    EXPECT_EQ(pi.getFirstName(), nullptr);
    EXPECT_EQ(pi.getValueOfLastName(), "");
    EXPECT_EQ(pi.getLastName(), nullptr);

    // Check date field
    EXPECT_EQ(pi.getValueOfHireDate(), ::trantor::Date());
    EXPECT_EQ(pi.getHireDate(), nullptr);

    // Check toJson for null values
    Json::Value json = pi.toJson();
    EXPECT_TRUE(json["id"].isNull());
    EXPECT_TRUE(json["job_id"].isNull());
    EXPECT_TRUE(json["job_title"].isNull());
    EXPECT_TRUE(json["department_id"].isNull());
    EXPECT_TRUE(json["department_name"].isNull());
    EXPECT_TRUE(json["manager_id"].isNull());
    EXPECT_TRUE(json["manager_full_name"].isNull());
    EXPECT_TRUE(json["first_name"].isNull());
    EXPECT_TRUE(json["last_name"].isNull());
    EXPECT_TRUE(json["hire_date"].isNull());
}