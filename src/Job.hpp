#ifndef JOB_HPP
#define JOB_HPP

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <cassert>
#include <json/json.h>
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Result.h>
#include <drogon/orm/Row.h>
#include <drogon/orm/internal/SqlBinder.h>
#include "Person.h"

class Job
{
public:
    using PrimaryKeyType = int32_t;

    struct Cols
    {
        static const std::string _id;
        static const std::string _title;
    };

    struct MetaData
    {
        std::string colName_;
        std::string cppType_;
        std::string dbType_;
        size_t length_;
        bool isPrimaryKey_;
        bool isNullable_;
        bool isUnique_;
        MetaData(std::string colName, std::string cppType, std::string dbType, size_t length, bool isPrimaryKey, bool isNullable, bool isUnique)
            : colName_(std::move(colName)), cppType_(std::move(cppType)), dbType_(std::move(dbType)), length_(length), isPrimaryKey_(isPrimaryKey), isNullable_(isNullable), isUnique_(isUnique) {}
    };

    static const std::string primaryKeyName;
    static const bool hasPrimaryKey;
    static const std::string tableName;
    static const std::vector<MetaData> metaData_;

    explicit Job(const drogon::orm::Row &r, ssize_t indexOffset = -1) noexcept;
    explicit Job(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false);
    explicit Job(const Json::Value &pJson) noexcept(false);

    void updateByMasqueradedJson(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false);
    void updateByJson(const Json::Value &pJson) noexcept(false);

    const int32_t &getValueOfId() const noexcept;
    const std::shared_ptr<int32_t> &getId() const noexcept;
    void setId(const int32_t &pId) noexcept;
    const PrimaryKeyType &getPrimaryKey() const;

    const std::string &getValueOfTitle() const noexcept;
    const std::shared_ptr<std::string> &getTitle() const noexcept;
    void setTitle(const std::string &pTitle) noexcept;
    void setTitle(std::string &&pTitle) noexcept;

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

    void getPersons(const drogon::orm::DbClientPtr &clientPtr,
                    const std::function<void(std::vector<Person>)> &rcb,
                    const drogon::orm::ExceptionCallback &ecb) const;

private:
    bool validJsonOfField(size_t index,
                          const std::string &fieldName,
                          const Json::Value &pJson,
                          std::string &err,
                          bool isForCreation);

    std::shared_ptr<int32_t> id_;
    std::shared_ptr<std::string> title_;
    bool dirtyFlag_[2] = {false, false};
};

#endif // JOB_HPP