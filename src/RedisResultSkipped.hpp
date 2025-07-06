#pragma once

#include <string>
#include <vector>

namespace drogon
{
namespace nosql
{
enum class RedisResultType;

class RedisResult
{
  public:
    std::string getStringForDisplaying() const noexcept;
    std::string getStringForDisplayingWithIndent(size_t indent) const noexcept;
    std::string asString() const noexcept(false);
    RedisResultType type() const noexcept;
    std::vector<RedisResult> asArray() const noexcept(false);
    long long asInteger() const noexcept(false);
    bool isNil() const noexcept;
};
}  // namespace nosql
}  // namespace drogon