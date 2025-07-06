#include "gtest/gtest.h"
#include "Department.hpp"
#include <json/json.h>
#include <memory>

class DepartmentTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(DepartmentTest, ColumnNames)
{
    EXPECT_EQ(Department::Cols::_id, "id");
    EXPECT_EQ(Department::Cols::_name, "name");
    EXPECT_EQ(Department::primaryKeyName, "id");
    EXPECT_TRUE(Department::hasPrimaryKey);
    EXPECT_EQ(Department::tableName, "department");
    EXPECT_NO_THROW({
        for (size_t i = 0; i < Department::metaData_.size(); ++i)
        {
            const std::string &colName = Department::getColumnName(i);
            EXPECT_FALSE(colName.empty());
        }
    });
}

TEST_F(DepartmentTest, JsonConstructorAndGetters)
{
    Json::Value json;
    json["id"] = 123;
    json["name"] = "HR";

    Department d(json);

    EXPECT_EQ(d.getValueOfId(), 123);
    EXPECT_EQ(d.getValueOfName(), "HR");
    EXPECT_TRUE(d.getId() != nullptr);
    EXPECT_TRUE(d.getName() != nullptr);
}

TEST_F(DepartmentTest, SettersAndDirtyFlags)
{
    Department d(Json::Value());

    d.setId(456);
    EXPECT_EQ(d.getValueOfId(), 456);

    d.setName("Finance");
    EXPECT_EQ(d.getValueOfName(), "Finance");

    d.setName(std::string("Marketing"));
    EXPECT_EQ(d.getValueOfName(), "Marketing");
}

TEST_F(DepartmentTest, ToJsonAndToMasqueradedJson)
{
    Department d(Json::Value());
    d.setId(789);
    d.setName("Sales");

    Json::Value json = d.toJson();
    EXPECT_EQ(json["id"].asInt(), 789);
    EXPECT_EQ(json["name"].asString(), "Sales");

    std::vector<std::string> maskVec = {"masked_id", "masked_name"};
    Json::Value maskedJson = d.toMasqueradedJson(maskVec);
    EXPECT_TRUE(maskedJson.isMember("masked_id"));
    EXPECT_TRUE(maskedJson.isMember("masked_name"));
    EXPECT_EQ(maskedJson["masked_id"].asInt(), 789);
    EXPECT_EQ(maskedJson["masked_name"].asString(), "Sales");
}

TEST_F(DepartmentTest, ValidateJsonForCreation)
{
    std::string err;
    Json::Value validJson;
    validJson["name"] = "ValidName";

    EXPECT_TRUE(Department::validateJsonForCreation(validJson, err));
    EXPECT_TRUE(err.empty());

    Json::Value invalidJson;
    invalidJson["id"] = 1;
    invalidJson["name"] = "ValidName";
    EXPECT_FALSE(Department::validateJsonForCreation(invalidJson, err));
    EXPECT_FALSE(err.empty());

    Json::Value missingName;
    EXPECT_FALSE(Department::validateJsonForCreation(missingName, err));
    EXPECT_FALSE(err.empty());
}

TEST_F(DepartmentTest, ValidateJsonForUpdate)
{
    std::string err;
    Json::Value validJson;
    validJson["id"] = 1;
    validJson["name"] = "UpdatedName";

    EXPECT_TRUE(Department::validateJsonForUpdate(validJson, err));
    EXPECT_TRUE(err.empty());

    Json::Value missingId;
    missingId["name"] = "UpdatedName";
    EXPECT_FALSE(Department::validateJsonForUpdate(missingId, err));
    EXPECT_FALSE(err.empty());
}

TEST_F(DepartmentTest, ValidJsonOfField)
{
    std::string err;

    // id field for update (index 0)
    Json::Value idVal(10);
    EXPECT_TRUE(Department::validJsonOfField(0, "id", idVal, err, false));
    EXPECT_TRUE(err.empty());

    // id field for creation should fail
    EXPECT_FALSE(Department::validJsonOfField(0, "id", idVal, err, true));
    EXPECT_FALSE(err.empty());

    // name field valid
    Json::Value nameVal("TestName");
    EXPECT_TRUE(Department::validJsonOfField(1, "name", nameVal, err, true));
    EXPECT_TRUE(err.empty());

    // name field too long
    Json::Value longName(std::string(51, 'a'));
    EXPECT_FALSE(Department::validJsonOfField(1, "name", longName, err, true));
    EXPECT_FALSE(err.empty());

    // name field null
    Json::Value nullVal;
    nullVal = Json::Value::nullSingleton();
    EXPECT_FALSE(Department::validJsonOfField(1, "name", nullVal, err, true));
    EXPECT_FALSE(err.empty());
}

TEST_F(DepartmentTest, InsertColumns)
{
    const auto &cols = Department::insertColumns();
    ASSERT_EQ(cols.size(), 1);
    EXPECT_EQ(cols[0], "name");
}

TEST_F(DepartmentTest, UpdateColumnsAndArgs)
{
    Department d(Json::Value());
    d.setName("NewName");
    auto cols = d.updateColumns();
    ASSERT_EQ(cols.size(), 1);
    EXPECT_EQ(cols[0], "name");
}