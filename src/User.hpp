#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <json/json.h>
#include <drogon/orm/Row.h>
#include <drogon/orm/SqlBinder.h>

namespace drogon_model {
namespace org_chart {

class User
{
public:
    struct Cols
    {
        static const std::string _id;
        static const std::string _username;
        static const std::string _password;
    };

    using PrimaryKeyType = int32_t;

    struct MetaData
    {
        std::string colName_;
        std::string cppType_;
        std::string sqlType_;
        int size_;
        bool isPrimaryKey_;
        bool isNullable_;
        bool isAutoIncrement_;
        MetaData(const std::string &colName,
                 const std::string &cppType,
                 const std::string &sqlType,
                 int size,
                 bool isPrimaryKey,
                 bool isNullable,
                 bool isAutoIncrement)
            : colName_(colName),
              cppType_(cppType),
              sqlType_(sqlType),
              size_(size),
              isPrimaryKey_(isPrimaryKey),
              isNullable_(isNullable),
              isAutoIncrement_(isAutoIncrement)
        {}
    };

    static const std::string primaryKeyName;
    static const bool hasPrimaryKey;
    static const std::string tableName;
    static const std::vector<MetaData> metaData_;

    static const std::string &getColumnName(size_t index) noexcept(false);

    User(const drogon::orm::Row &r, const ssize_t indexOffset = -1) noexcept;
    User(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false);
    User(const Json::Value &pJson) noexcept(false);

    void updateByMasqueradedJson(const Json::Value &pJson,
                                 const std::vector<std::string> &pMasqueradingVector) noexcept(false);
    void updateByJson(const Json::Value &pJson) noexcept(false);

    const int32_t &getValueOfId() const noexcept;
    const std::shared_ptr<int32_t> &getId() const noexcept;
    void setId(const int32_t &pId) noexcept;
    const PrimaryKeyType &getPrimaryKey() const;

    const std::string &getValueOfUsername() const noexcept;
    const std::shared_ptr<std::string> &getUsername() const noexcept;
    void setUsername(const std::string &pUsername) noexcept;
    void setUsername(std::string &&pUsername) noexcept;

    const std::string &getValueOfPassword() const noexcept;
    const std::shared_ptr<std::string> &getPassword() const noexcept;
    void setPassword(const std::string &pPassword) noexcept;
    void setPassword(std::string &&pPassword) noexcept;

    void updateId(const uint64_t id);

    static const std::vector<std::string> &insertColumns() noexcept;

    void outputArgs(drogon::orm::internal::SqlBinder &binder) const;
    const std::vector<std::string> updateColumns() const;
    void updateArgs(drogon::orm::internal::SqlBinder &binder) const;

    Json::Value toJson() const;
    Json::Value toMasqueradedJson(const std::vector<std::string> &pMasqueradingVector) const;

    static bool validateJsonForCreation(const Json::Value &pJson, std::string &err);
    static bool validateMasqueradedJsonForCreation(const Json::Value &pJson,
                                                  const std::vector<std::string> &pMasqueradingVector,
                                                  std::string &err);
    static bool validateJsonForUpdate(const Json::Value &pJson, std::string &err);
    static bool validateMasqueradedJsonForUpdate(const Json::Value &pJson,
                                                 const std::vector<std::string> &pMasqueradingVector,
                                                 std::string &err);

    static bool validJsonOfField(size_t index,
                                 const std::string &fieldName,
                                 const Json::Value &pJson,
                                 std::string &err,
                                 bool isForCreation);

private:
    std::shared_ptr<int32_t> id_;
    std::shared_ptr<std::string> username_;
    std::shared_ptr<std::string> password_;
    bool dirtyFlag_[3] = {false, false, false};
};

} // namespace org_chart
} // namespace drogon_model

#endif // USER_HPP