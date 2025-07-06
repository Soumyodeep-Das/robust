#include "RedisClientManagerSkipped.hpp"
#include <drogon/config.h>
#include <drogon/utils/Utilities.h>
#include <algorithm>
#include <cstdlib>

using namespace drogon::nosql;
using namespace drogon;

void RedisClientManager::createRedisClients(
    const std::vector<trantor::EventLoop *> & /*ioloops*/)
{
    return;
}

void RedisClientManager::createRedisClient(const std::string & /*name*/,
                                           const std::string & /*host*/,
                                           unsigned short /*port*/,
                                           const std::string & /*password*/,
                                           size_t /*connectionNum*/,
                                           bool /*isFast*/,
                                           double /*timeout*/,
                                           unsigned int /*db*/)
{
    LOG_FATAL << "Redis is not supported by drogon, please install the "
                 "hiredis library first.";
    abort();
}