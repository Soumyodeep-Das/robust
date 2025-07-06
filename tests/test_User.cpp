#include "gtest/gtest.h"
#include "User.hpp"
#include <json/json.h>

using namespace drogon_model::org_chart;

class UserTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(UserTest, TestGetColumnName)
{
    EXPECT_EQ(User::getColumnName(0), "id");
    EXPECT_EQ(User::getColumnName(1), "username");
    EXPECT_EQ(User::getColumnName(2), "password");
    EXPECT_THROW(User::getColumnName(3), std::exception);
}

TEST_F(UserTest, TestConstructFromJson)
{
    Json::Value json;
    json["id"] = 42;
    json["username"] = "alice";
    json["password"] = "secret";

    User u(json);
    EXPECT_EQ(u.getValueOfId(), 42);
    EXPECT_EQ(u.getValueOfUsername(), "alice");
    EXPECT_EQ(u.getValueOfPassword(), "secret");
}

TEST_F(UserTest, TestSettersAndGetters)
{
    User u(Json::Value());
    u.setId(10);
    EXPECT_EQ(u.getValueOfId(), 10);

    u.setUsername("bob");
    EXPECT_EQ(u.getValueOfUsername(), "bob");

    u.setPassword("pass");
    EXPECT_EQ(u.getValueOfPassword(), "pass");

    u.setUsername(std::string("charlie"));
    EXPECT_EQ(u.getValueOfUsername(), "charlie");

    u.setPassword(std::string("word"));
    EXPECT_EQ(u.getValueOfPassword(), "word");
}

TEST_F(UserTest, TestToJsonAndToMasqueradedJson)
{
    User u(Json::Value());
    u.setId(1);
    u.setUsername("user");
    u.setPassword("pwd");

    Json::Value json = u.toJson();
    EXPECT_EQ(json["id"].asInt(), 1);
    EXPECT_EQ(json["username"].asString(), "user");
    EXPECT_EQ(json["password"].asString(), "pwd");

    std::vector<std::string> mask = {"a", "b", "c"};
    Json::Value masked = u.toMasqueradedJson(mask);
    EXPECT_EQ(masked["a"].asInt(), 1);
    EXPECT_EQ(masked["b"].asString(), "user");
    EXPECT_EQ(masked["c"].asString(), "pwd");

    std::vector<std::string> badMask = {"", "", ""};
    Json::Value fallback = u.toMasqueradedJson(badMask);
    EXPECT_EQ(fallback["id"].asInt(), 1);
    EXPECT_EQ(fallback["username"].asString(), "user");
    EXPECT_EQ(fallback["password"].asString(), "pwd");
}

TEST_F(UserTest, TestValidateJsonForCreation)
{
    Json::Value validJson;
    validJson["username"] = "validuser";
    validJson["password"] = "validpass";

    std::string err;
    EXPECT_TRUE(User::validateJsonForCreation(validJson, err));
    EXPECT_TRUE(err.empty());

    Json::Value missingUser;
    missingUser["password"] = "pass";
    EXPECT_FALSE(User::validateJsonForCreation(missingUser, err));
    EXPECT_FALSE(err.empty());

    Json::Value missingPass;
    missingPass["username"] = "user";
    EXPECT_FALSE(User::validateJsonForCreation(missingPass, err));
    EXPECT_FALSE(err.empty());

    Json::Value withId;
    withId["id"] = 1;
    withId["username"] = "user";
    withId["password"] = "pass";
    EXPECT_FALSE(User::validateJsonForCreation(withId, err));
}

TEST_F(UserTest, TestValidateJsonForUpdate)
{
    Json::Value validJson;
    validJson["id"] = 1;
    validJson["username"] = "user";
    validJson["password"] = "pass";

    std::string err;
    EXPECT_TRUE(User::validateJsonForUpdate(validJson, err));
    EXPECT_TRUE(err.empty());

    Json::Value missingId;
    missingId["username"] = "user";
    EXPECT_FALSE(User::validateJsonForUpdate(missingId, err));
    EXPECT_FALSE(err.empty());
}

TEST_F(UserTest, TestValidJsonOfField)
{
    std::string err;
    // id field for update (isForCreation=false)
    Json::Value idVal(5);
    EXPECT_TRUE(User::validJsonOfField(0, "id", idVal, err, false));
    EXPECT_FALSE(User::validJsonOfField(0, "id", Json::Value(), err, false));
    EXPECT_FALSE(User::validJsonOfField(0, "id", Json::Value("string"), err, false));
    EXPECT_FALSE(User::validJsonOfField(0, "id", idVal, err, true)); // creation disallowed

    // username field
    Json::Value userVal("username");
    EXPECT_TRUE(User::validJsonOfField(1, "username", userVal, err, false));
    EXPECT_FALSE(User::validJsonOfField(1, "username", Json::Value(), err, false));
    EXPECT_FALSE(User::validJsonOfField(1, "username", Json::Value(123), err, false));
    Json::Value longStr(std::string(51, 'a'));
    EXPECT_FALSE(User::validJsonOfField(1, "username", longStr, err, false));

    // password field
    Json::Value passVal("password");
    EXPECT_TRUE(User::validJsonOfField(2, "password", passVal, err, false));
    EXPECT_FALSE(User::validJsonOfField(2, "password", Json::Value(), err, false));
    EXPECT_FALSE(User::validJsonOfField(2, "password", Json::Value(123), err, false));

    // invalid index
    EXPECT_FALSE(User::validJsonOfField(99, "unknown", Json::Value(), err, false));
}