#ifndef AUTHCONTROLLER_HPP
#define AUTHCONTROLLER_HPP

#include "drogon_model/org_chart/User.h"
#include <string>

namespace drogon {

class AuthController {
public:
    bool areFieldsValid(const drogon_model::org_chart::User& user) const;
    bool isPasswordValid(const std::string& password, const std::string& hash) const;
};

}  // namespace drogon

#endif  // AUTHCONTROLLER_HPP