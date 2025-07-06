#include "AuthController.hpp"

namespace drogon {

bool AuthController::areFieldsValid(const drogon_model::org_chart::User& user) const {
    return !user.getUsername().empty() && !user.getPassword().empty();
}

bool AuthController::isPasswordValid(const std::string& password, const std::string& hash) const {
    return password == hash;
}

}  // namespace drogon