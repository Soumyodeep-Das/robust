#include "HttpBinder.hpp"
#include <drogon/HttpAppFramework.h>

namespace drogon
{
namespace internal
{
void handleException(const std::exception& e,
                     const HttpRequestPtr& req,
                     std::function<void(const HttpResponsePtr&)>&& callback)
{
    app().getExceptionHandler()(e, req, std::move(callback));
}
}  // namespace internal
}  // namespace drogon