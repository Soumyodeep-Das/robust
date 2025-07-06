#include "gtest/gtest.h"
#include "AuthController.hpp"
#include "drogon_model/org_chart/User.h"
#include <string>

namespace drogon {

class AuthControllerTest : public ::testing::Test {
protected:
    AuthController controller;
};

TEST_F(AuthControllerTest, AreFieldsValid_ValidUser_ReturnsTrue) {
    drogon_model::org_chart::User user;
    user.setUsername("testuser");
    user.setPassword("testpass");
    EXPECT_TRUE(controller.areFieldsValid(user));
}

TEST_F(AuthControllerTest, AreFieldsValid_InvalidUser_ReturnsFalse) {
    drogon_model::org_chart::User user;
    // Assuming default constructor sets empty strings
    EXPECT_FALSE(controller.areFieldsValid(user));
}

TEST_F(AuthControllerTest, IsPasswordValid_CorrectPassword_ReturnsTrue) {
    std::string hash = "correctpassword";  // Mock hash for testing
    EXPECT_TRUE(controller.isPasswordValid("correctpassword", hash));
}

TEST_F(AuthControllerTest, IsPasswordValid_IncorrectPassword_ReturnsFalse) {
    std::string hash = "correctpassword";  // Mock hash for testing
    EXPECT_FALSE(controller.isPasswordValid("wrongpassword", hash));
}

}  // namespace drogon