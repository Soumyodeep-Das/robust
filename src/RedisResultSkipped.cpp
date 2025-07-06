#include "RedisResultSkipped.hpp"
#include "drogon/nosql/RedisResult.h"
#include "trantor/utils/Logger.h"
#include <cstdlib>

namespace drogon
{
namespace nosql
{
std::string RedisResult::getStringForDisplaying() const noexcept
{
    LOG_FATAL << "Redis is not supported by drogon, please install the "
                 "hiredis library first.";
    abort();
}

std::string RedisResult::getStringForDisplayingWithIndent(
    size_t /*indent*/) const noexcept
{
    LOG_FATAL << "Redis is not supported by drogon, please install the "
                 "hiredis library first.";
    abort();
}

std::string RedisResult::asString() const noexcept(false)
{
    LOG_FATAL << "Redis is not supported by drogon, please install the "
                 "hiredis library first.";
    abort();
}

RedisResultType RedisResult::type() const noexcept
{
    LOG_FATAL << "Redis is not supported by drogon, please install the "
                 "hiredis library first.";
    abort();
}

std::vector<RedisResult> RedisResult::asArray() const noexcept(false)
{
    LOG_FATAL << "Redis is not supported by drogon, please install the "
                 "hiredis library first.";
    abort();
}
long long RedisResult::asInteger() const noexcept(false)
{
    LOG_FATAL << "Redis is not supported by drogon, please install the "
                 "hiredis library first.";
    abort();
}
bool RedisResult::isNil() const noexcept
{
    LOG_FATAL << "Redis is not supported by drogon, please install the "
                 "hiredis library first.";
    abort();
}
}  // namespace nosql
}  // namespace drogon