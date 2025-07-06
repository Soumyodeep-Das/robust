#ifndef DROGON_NOSQL_REDISCLIENTSKIPPED_HPP
#define DROGON_NOSQL_REDISCLIENTSKIPPED_HPP

#include <memory>
#include <string>
#include "trantor/net/InetAddress.h"

namespace drogon
{
namespace nosql
{
class RedisClient
{
  public:
    static std::shared_ptr<RedisClient> newRedisClient(
        const trantor::InetAddress& serverAddress,
        size_t numberOfConnections,
        const std::string& password,
        const unsigned int db);
};
}  // namespace nosql
}  // namespace drogon

#endif  // DROGON_NOSQL_REDISCLIENTSKIPPED_HPP