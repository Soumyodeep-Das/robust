#ifndef DROGON_HTTPBINDER_HPP
#define DROGON_HTTPBINDER_HPP

#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <functional>
#include <exception>

namespace drogon
{
namespace internal
{
void handleException(const std::exception& e,
                     const HttpRequestPtr& req,
                     std::function<void(const HttpResponsePtr&)>&& callback);
}  // namespace internal
}  // namespace drogon

#endif  // DROGON_HTTPBINDER_HPP