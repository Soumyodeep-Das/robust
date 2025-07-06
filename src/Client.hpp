#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <functional>
#include <string>
#include "drogon/HttpTypes.h"

namespace drogon
{
class Client
{
public:
    void get(const HttpRequestPtr &req,
             std::function<void(const HttpResponsePtr &)> &&callback,
             std::string key);

    void post(const HttpRequestPtr &req,
              std::function<void(const HttpResponsePtr &)> &&callback,
              std::string key);
};
}  // namespace drogon

#endif  // CLIENT_HPP