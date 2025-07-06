#ifndef DEPARTMENT_HPP
#define DEPARTMENT_HPP

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <json/json.h>
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Result.h>
#include <drogon/orm/Exception.h>
#include "Person.h"

class Department
{
public:
    struct Cols
    {
        static const std::string _id;
        static const std::string _name;
    };

    using PrimaryKeyType = int32_t;
    struct MetaData
    {
        std::string colName_;
        std::string cppType_;
        std::string dbType_;
        size_t length_;
        bool isPrimaryKey_;
        bool isNullable_;
        bool isAutoIncrement_;
        MetaData(std::string colName, std::string cppType, std::string dbType, size_t length, bool isPrimaryKey, bool isNullable, bool isAutoIncrement)
            : colName_(std::move(colName)), cppType_(std::move(cppType)), dbType_(std::move(dbType)), length_(length), isPrimaryKey_(isPrimaryKey), isNullable_(isNullable), isAutoIncrement_(isAutoIncrement) {}
    };

    static const std::string primaryKeyName;
    static const bool hasPrimaryKey;
    static const std::string tableName;
    static const std::vector<MetaData> metaData_;

    static const std::string &getColumnName(size_t index) noexcept(false);

    Department(const drogon::orm::Row &r, const ssize_t indexOffset = -1) noexcept;
    Department(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false);
    Department(const Json::Value &pJson) noexcept(false);

    void updateByMasqueradedJson(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false);
    void updateByJson(const Json::Value &pJson) noexcept(false);

    const int32_t &getValueOfId() const noexcept;
    const std::shared_ptr<int32_t> &getId() const noexcept;
    void setId(const int32_t &pId) noexcept;
    const PrimaryKeyType &getPrimaryKey() const;

    const std::string &getValueOfName() const noexcept;
    const std::shared_ptr<std::string> &getName() const noexcept;
    void setName(const std::string &pName) noexcept;
    void setName(std::string &&pName) noexcept;

    void updateId(const uint64_t id);

    static const std::vector<std::string> &insertColumns() noexcept;

    void outputArgs(drogon::orm::internal::SqlBinder &binder) const;
    const std::vector<std::string> updateColumns() const;
    void updateArgs(drogon::orm::internal::SqlBinder &binder) const;

    Json::Value toJson() const;
    Json::Value toMasqueradedJson(const std::vector<std::string> &pMasqueradingVector) const;

    static bool validateJsonForCreation(const Json::Value &pJson, std::string &err);
    static bool validateMasqueradedJsonForCreation(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector, std::string &err);
    static bool validateJsonForUpdate(const Json::Value &pJson, std::string &err);
    static bool validateMasqueradedJsonForUpdate(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector, std::string &err);

    static bool validJsonOfField(size_t index, const std::string &fieldName, const Json::Value &pJson, std::string &err, bool isForCreation);

    void getPersons(const drogon::orm::DbClientPtr &clientPtr,
                    const std::function<void(std::vector<Person>)> &rcb,
                    const drogon::orm::ExceptionCallback &ecb) const;

private:
    std::shared_ptr<int32_t> id_;
    std::shared_ptr<std::string> name_;
    bool dirtyFlag_[2]{false, false};
};

#endif // DEPARTMENT_HPP