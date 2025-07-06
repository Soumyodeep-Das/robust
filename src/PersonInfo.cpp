#include "PersonInfo.hpp"

namespace drogon_model {
namespace org_chart {

PersonInfo::PersonInfo(const drogon::orm::Row &r, int index)
{
    if (r.isNull(index))
    {
        id_ = nullptr;
        id_value_ = 0;
    }
    else
    {
        id_value_ = r[index].template as<int>();
        id_ = std::make_unique<int>(id_value_);
    }

    if (r.isNull(index + 1))
    {
        job_id_ = nullptr;
        job_id_value_ = 0;
    }
    else
    {
        job_id_value_ = r[index + 1].template as<int>();
        job_id_ = std::make_unique<int>(job_id_value_);
    }

    if (r.isNull(index + 2))
    {
        job_title_ = nullptr;
        job_title_value_ = "";
    }
    else
    {
        job_title_value_ = r[index + 2].template as<std::string>();
        job_title_ = std::make_unique<std::string>(job_title_value_);
    }

    if (r.isNull(index + 3))
    {
        department_id_ = nullptr;
        department_id_value_ = 0;
    }
    else
    {
        department_id_value_ = r[index + 3].template as<int>();
        department_id_ = std::make_unique<int>(department_id_value_);
    }

    if (r.isNull(index + 4))
    {
        department_name_ = nullptr;
        department_name_value_ = "";
    }
    else
    {
        department_name_value_ = r[index + 4].template as<std::string>();
        department_name_ = std::make_unique<std::string>(department_name_value_);
    }

    if (r.isNull(index + 5))
    {
        manager_id_ = nullptr;
        manager_id_value_ = 0;
    }
    else
    {
        manager_id_value_ = r[index + 5].template as<int>();
        manager_id_ = std::make_unique<int>(manager_id_value_);
    }

    if (r.isNull(index + 6))
    {
        manager_full_name_ = nullptr;
        manager_full_name_value_ = "";
    }
    else
    {
        manager_full_name_value_ = r[index + 6].template as<std::string>();
        manager_full_name_ = std::make_unique<std::string>(manager_full_name_value_);
    }

    if (r.isNull(index + 7))
    {
        first_name_ = nullptr;
        first_name_value_ = "";
    }
    else
    {
        first_name_value_ = r[index + 7].template as<std::string>();
        first_name_ = std::make_unique<std::string>(first_name_value_);
    }

    if (r.isNull(index + 8))
    {
        last_name_ = nullptr;
        last_name_value_ = "";
    }
    else
    {
        last_name_value_ = r[index + 8].template as<std::string>();
        last_name_ = std::make_unique<std::string>(last_name_value_);
    }

    if (r.isNull(index + 9))
    {
        hire_date_ = nullptr;
        hire_date_value_ = trantor::Date();
    }
    else
    {
        hire_date_value_ = r[index + 9].template as<trantor::Date>();
        hire_date_ = std::make_unique<trantor::Date>(hire_date_value_);
    }
}

int PersonInfo::getValueOfId() const
{
    return id_value_;
}

const int* PersonInfo::getId() const
{
    return id_ ? id_.get() : nullptr;
}

int PersonInfo::getValueOfJobId() const
{
    return job_id_value_;
}

const int* PersonInfo::getJobId() const
{
    return job_id_ ? job_id_.get() : nullptr;
}

const std::string& PersonInfo::getValueOfJobTitle() const
{
    return job_title_value_;
}

const std::string* PersonInfo::getJobTitle() const
{
    return job_title_ ? job_title_.get() : nullptr;
}

int PersonInfo::getValueOfDepartmentId() const
{
    return department_id_value_;
}

const int* PersonInfo::getDepartmentId() const
{
    return department_id_ ? department_id_.get() : nullptr;
}

const std::string& PersonInfo::getValueOfDepartmentName() const
{
    return department_name_value_;
}

const std::string* PersonInfo::getDepartmentName() const
{
    return department_name_ ? department_name_.get() : nullptr;
}

int PersonInfo::getValueOfManagerId() const
{
    return manager_id_value_;
}

const int* PersonInfo::getManagerId() const
{
    return manager_id_ ? manager_id_.get() : nullptr;
}

const std::string& PersonInfo::getValueOfManagerFullName() const
{
    return manager_full_name_value_;
}

const std::string* PersonInfo::getManagerFullName() const
{
    return manager_full_name_ ? manager_full_name_.get() : nullptr;
}

const std::string& PersonInfo::getValueOfFirstName() const
{
    return first_name_value_;
}

const std::string* PersonInfo::getFirstName() const
{
    return first_name_ ? first_name_.get() : nullptr;
}

const std::string& PersonInfo::getValueOfLastName() const
{
    return last_name_value_;
}

const std::string* PersonInfo::getLastName() const
{
    return last_name_ ? last_name_.get() : nullptr;
}

const trantor::Date& PersonInfo::getValueOfHireDate() const
{
    return hire_date_value_;
}

const trantor::Date* PersonInfo::getHireDate() const
{
    return hire_date_ ? hire_date_.get() : nullptr;
}

Json::Value PersonInfo::toJson() const
{
    Json::Value json;
    if (id_)
        json["id"] = *id_;
    else
        json["id"] = Json::Value::null;

    if (job_id_)
        json["job_id"] = *job_id_;
    else
        json["job_id"] = Json::Value::null;

    if (job_title_)
        json["job_title"] = *job_title_;
    else
        json["job_title"] = Json::Value::null;

    if (department_id_)
        json["department_id"] = *department_id_;
    else
        json["department_id"] = Json::Value::null;

    if (department_name_)
        json["department_name"] = *department_name_;
    else
        json["department_name"] = Json::Value::null;

    if (manager_id_)
        json["manager_id"] = *manager_id_;
    else
        json["manager_id"] = Json::Value::null;

    if (manager_full_name_)
        json["manager_full_name"] = *manager_full_name_;
    else
        json["manager_full_name"] = Json::Value::null;

    if (first_name_)
        json["first_name"] = *first_name_;
    else
        json["first_name"] = Json::Value::null;

    if (last_name_)
        json["last_name"] = *last_name_;
    else
        json["last_name"] = Json::Value::null;

    if (hire_date_)
        json["hire_date"] = hire_date_value_.toFormattedString();
    else
        json["hire_date"] = Json::Value::null;

    return json;
}

} // namespace org_chart
} // namespace drogon_model