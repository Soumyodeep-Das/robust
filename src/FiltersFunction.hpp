#ifndef DROGON_FILTERS_FUNCTION_HPP
#define DROGON_FILTERS_FUNCTION_HPP

#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <drogon/HttpFilter.h>

namespace drogon
{
namespace filters_function
{
std::vector<std::shared_ptr<HttpFilterBase>> createFilters(
    const std::vector<std::string> &filterNames);

void doFilters(
    const std::vector<std::shared_ptr<HttpFilterBase>> &filters,
    const HttpRequestImplPtr &req,
    const std::shared_ptr<const std::function<void(const HttpResponsePtr &)>>
        &callbackPtr,
    std::function<void()> &&missCallback);

}  // namespace filters_function
}  // namespace drogon

#endif  // DROGON_FILTERS_FUNCTION_HPP