#ifndef PERSONINFO_HPP
#define PERSONINFO_HPP

#include <string>
#include <memory>
#include "drogon/orm/Row.h"
#include "trantor/utils/Date.h"
#include "json/json.h"

namespace drogon_model {
namespace org_chart {

class PersonInfo
{
public:
    PersonInfo(const drogon::orm::Row &r, int index);

    int getValueOfId() const;
    const int* getId() const;

    int getValueOfJobId() const;
    const int* getJobId() const;

    const std::string& getValueOfJobTitle() const;
    const std::string* getJobTitle() const;

    int getValueOfDepartmentId() const;
    const int* getDepartmentId() const;

    const std::string& getValueOfDepartmentName() const;
    const std::string* getDepartmentName() const;

    int getValueOfManagerId() const;
    const int* getManagerId() const;

    const std::string& getValueOfManagerFullName() const;
    const std::string* getManagerFullName() const;

    const std::string& getValueOfFirstName() const;
    const std::string* getFirstName() const;

    const std::string& getValueOfLastName() const;
    const std::string* getLastName() const;

    const trantor::Date& getValueOfHireDate() const;
    const trantor::Date* getHireDate() const;

    Json::Value toJson() const;

private:
    std::unique_ptr<int> id_;
    std::unique_ptr<int> job_id_;
    std::unique_ptr<std::string> job_title_;
    std::unique_ptr<int> department_id_;
    std::unique_ptr<std::string> department_name_;
    std::unique_ptr<int> manager_id_;
    std::unique_ptr<std::string> manager_full_name_;
    std::unique_ptr<std::string> first_name_;
    std::unique_ptr<std::string> last_name_;
    std::unique_ptr<trantor::Date> hire_date_;

    int id_value_;
    int job_id_value_;
    std::string job_title_value_;
    int department_id_value_;
    std::string department_name_value_;
    int manager_id_value_;
    std::string manager_full_name_value_;
    std::string first_name_value_;
    std::string last_name_value_;
    trantor::Date hire_date_value_;
};

} // namespace org_chart
} // namespace drogon_model

#endif // PERSONINFO_HPP