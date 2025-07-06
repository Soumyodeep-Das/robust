#include "gtest/gtest.h"
#include "Job.hpp"
#include <json/json.h>

class JobTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JobTest, DefaultConstructorFromJson) {
    Json::Value json;
    json["id"] = 123;
    json["title"] = "Developer";
    Job job(json);
    EXPECT_EQ(job.getValueOfId(), 123);
    EXPECT_EQ(job.getValueOfTitle(), "Developer");
}

TEST_F(JobTest, ConstructorFromJsonWithMasqueradingVector) {
    Json::Value json;
    json["alias_id"] = 456;
    json["alias_title"] = "Manager";
    std::vector<std::string> vec{"alias_id", "alias_title"};
    Job job(json, vec);
    EXPECT_EQ(job.getValueOfId(), 456);
    EXPECT_EQ(job.getValueOfTitle(), "Manager");
}

TEST_F(JobTest, SettersAndGetters) {
    Job job(Json::Value());
    job.setId(789);
    EXPECT_EQ(job.getValueOfId(), 789);
    job.setTitle("Tester");
    EXPECT_EQ(job.getValueOfTitle(), "Tester");
    job.setTitle(std::string("Lead"));
    EXPECT_EQ(job.getValueOfTitle(), "Lead");
}

TEST_F(JobTest, ToJsonAndMasqueradedJson) {
    Job job(Json::Value());
    job.setId(1);
    job.setTitle("Engineer");
    Json::Value json = job.toJson();
    EXPECT_TRUE(json.isMember("id"));
    EXPECT_TRUE(json.isMember("title"));
    EXPECT_EQ(json["id"].asInt(), 1);
    EXPECT_EQ(json["title"].asString(), "Engineer");

    std::vector<std::string> vec{"alias_id", "alias_title"};
    Json::Value masqueraded = job.toMasqueradedJson(vec);
    EXPECT_TRUE(masqueraded.isMember("alias_id"));
    EXPECT_TRUE(masqueraded.isMember("alias_title"));
    EXPECT_EQ(masqueraded["alias_id"].asInt(), 1);
    EXPECT_EQ(masqueraded["alias_title"].asString(), "Engineer");
}

TEST_F(JobTest, ValidateJsonForCreation) {
    Json::Value json;
    std::string err;
    json["title"] = "ValidTitle";
    EXPECT_TRUE(Job::validateJsonForCreation(json, err));
    json["id"] = 10;
    EXPECT_FALSE(Job::validateJsonForCreation(json, err));
    EXPECT_EQ(err, "The automatic primary key cannot be set");

    Json::Value json2;
    EXPECT_FALSE(Job::validateJsonForCreation(json2, err));
    EXPECT_EQ(err, "The title column cannot be null");
}

TEST_F(JobTest, ValidateJsonForUpdate) {
    Json::Value json;
    std::string err;
    json["id"] = 5;
    json["title"] = "UpdateTitle";
    EXPECT_TRUE(Job::validateJsonForUpdate(json, err));

    Json::Value json2;
    json2["title"] = "NoId";
    EXPECT_FALSE(Job::validateJsonForUpdate(json2, err));
    EXPECT_EQ(err, "The value of primary key must be set in the json object for update");
}

TEST_F(JobTest, ValidateMasqueradedJsonForCreation) {
    std::string err;
    std::vector<std::string> vec{"alias_id", "alias_title"};
    Json::Value json;
    json["alias_title"] = "Title";
    EXPECT_TRUE(Job::validateMasqueradedJsonForCreation(json, vec, err));

    Json::Value json2;
    EXPECT_FALSE(Job::validateMasqueradedJsonForCreation(json2, vec, err));
    EXPECT_EQ(err, "The alias_title column cannot be null");

    std::vector<std::string> badVec{"only_one"};
    EXPECT_FALSE(Job::validateMasqueradedJsonForCreation(json, badVec, err));
    EXPECT_EQ(err, "Bad masquerading vector");
}

TEST_F(JobTest, ValidateMasqueradedJsonForUpdate) {
    std::string err;
    std::vector<std::string> vec{"alias_id", "alias_title"};
    Json::Value json;
    json["alias_id"] = 1;
    json["alias_title"] = "Title";
    EXPECT_TRUE(Job::validateMasqueradedJsonForUpdate(json, vec, err));

    Json::Value json2;
    json2["alias_title"] = "Title";
    EXPECT_FALSE(Job::validateMasqueradedJsonForUpdate(json2, vec, err));
    EXPECT_EQ(err, "The value of primary key must be set in the json object for update");

    std::vector<std::string> badVec{"only_one"};
    EXPECT_FALSE(Job::validateMasqueradedJsonForUpdate(json, badVec, err));
    EXPECT_EQ(err, "Bad masquerading vector");
}

TEST_F(JobTest, InsertAndUpdateColumns) {
    Job job(Json::Value());
    job.setTitle("Title");
    auto insertCols = Job::insertColumns();
    ASSERT_EQ(insertCols.size(), 1);
    EXPECT_EQ(insertCols[0], "title");

    auto updateCols = job.updateColumns();
    ASSERT_EQ(updateCols.size(), 1);
    EXPECT_EQ(updateCols[0], "title");
}