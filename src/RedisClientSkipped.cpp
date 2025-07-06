#include "RedisClientSkipped.hpp"
#include <cstdlib>
#include <drogon/Logger.h>

namespace drogon
{
namespace nosql
{
std::shared_ptr<RedisClient> RedisClient::newRedisClient(
    const trantor::InetAddress& /*serverAddress*/,
    size_t /*numberOfConnections*/,
    const std::string& /*password*/,
    const unsigned int /*db*/)
{
    LOG_FATAL << "Redis is not supported by drogon, please install the "
                 "hiredis library first.";
    abort();
}
}  // namespace nosql
}  // namespace drogon