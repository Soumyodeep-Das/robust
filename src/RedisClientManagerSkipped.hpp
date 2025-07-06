#ifndef REDISCLIENTMANAGERSKIPPED_HPP
#define REDISCLIENTMANAGERSKIPPED_HPP

#include <string>
#include <vector>

namespace trantor
{
class EventLoop;
}

namespace drogon
{
namespace nosql
{
class RedisClientManager
{
  public:
    void createRedisClients(const std::vector<trantor::EventLoop *> &ioloops);
    void createRedisClient(const std::string &name,
                           const std::string &host,
                           unsigned short port,
                           const std::string &password,
                           size_t connectionNum,
                           bool isFast,
                           double timeout,
                           unsigned int db);
};
}  // namespace nosql
}  // namespace drogon

#endif  // REDISCLIENTMANAGERSKIPPED_HPP