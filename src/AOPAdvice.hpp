#ifndef DROGON_AOPADVICE_HPP
#define DROGON_AOPADVICE_HPP

#include <vector>
#include <deque>
#include <functional>
#include <memory>

namespace drogon
{
class HttpRequestImpl;
class HttpResponse;
using HttpRequestPtr = std::shared_ptr<const HttpRequestImpl>;
using HttpRequestImplPtr = std::shared_ptr<HttpRequestImpl>;
using HttpResponsePtr = std::shared_ptr<HttpResponse>;

using AdviceCallback = std::function<void(const HttpResponsePtr &)>;
using AdviceChainCallback = std::function<void()>;

void doAdvicesChain(
    const std::vector<std::function<void(const HttpRequestPtr &,
                                         AdviceCallback &&,
                                         AdviceChainCallback &&)>> &advices,
    size_t index,
    const HttpRequestImplPtr &req,
    const std::shared_ptr<const std::function<void(const HttpResponsePtr &)>> &callbackPtr,
    std::function<void()> &&missCallback);

void doAdvicesChain(
    const std::deque<std::function<void(const HttpRequestPtr &,
                                        AdviceCallback &&,
                                        AdviceChainCallback &&)>> &advices,
    size_t index,
    const HttpRequestImplPtr &req,
    const std::shared_ptr<const std::function<void(const HttpResponsePtr &)>> &callbackPtr,
    std::function<void()> &&missCallback);

}  // namespace drogon

#endif  // DROGON_AOPADVICE_HPP