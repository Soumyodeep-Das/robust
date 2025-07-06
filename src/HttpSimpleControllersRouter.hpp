#ifndef HTTPSIMPLECONTROLLERSROUTER_HPP
#define HTTPSIMPLECONTROLLERSROUTER_HPP

#include <string>
#include <vector>
#include <tuple>
#include <functional>
#include <memory>
#include <mutex>
#include <map>
#include <algorithm>
#include <drogon/HttpSimpleController.h>
#include <drogon/HttpTypes.h>

namespace drogon
{
class HttpRequestImpl;
class HttpResponse;
using HttpRequestImplPtr = std::shared_ptr<HttpRequestImpl>;
using HttpResponsePtr = std::shared_ptr<HttpResponse>;

class HttpSimpleControllersRouter
{
  public:
    void registerHttpSimpleController(
        const std::string &pathName,
        const std::string &ctrlName,
        const std::vector<internal::HttpConstraint> &filtersAndMethods);

    void route(
        const HttpRequestImplPtr &req,
        std::function<void(const HttpResponsePtr &)> &&callback);

    std::vector<std::tuple<std::string, HttpMethod, std::string>> getHandlersInfo() const;

    void init(const std::vector<trantor::EventLoop *> &ioLoops);

  private:
    struct CtrlBinder;
    using CtrlBinderPtr = std::shared_ptr<CtrlBinder>;

    struct SimpleControllerRouterItem
    {
        std::shared_ptr<CtrlBinder> binders_[Invalid];
    };

    void doControllerHandler(
        const CtrlBinderPtr &ctrlBinderPtr,
        const HttpRequestImplPtr &req,
        std::function<void(const HttpResponsePtr &)> &&callback);

    void doPreHandlingAdvices(
        const CtrlBinderPtr &ctrlBinderPtr,
        const SimpleControllerRouterItem &routerItem,
        const HttpRequestImplPtr &req,
        std::function<void(const HttpResponsePtr &)> &&callback);

    void invokeCallback(
        const std::function<void(const HttpResponsePtr &)> &callback,
        const HttpRequestImplPtr &req,
        const HttpResponsePtr &resp);

    std::map<std::string, SimpleControllerRouterItem> simpleCtrlMap_;
    std::mutex simpleCtrlMutex_;

    std::vector<std::function<void(const HttpRequestImplPtr &)>> preHandlingObservers_;
    std::vector<std::function<void(const HttpRequestImplPtr &)>> postRoutingObservers_;
    std::vector<std::function<void(const HttpRequestImplPtr &, std::function<void(const HttpResponsePtr &)> &&)>> preHandlingAdvices_;
    std::vector<std::function<void(const HttpRequestImplPtr &, std::function<void(const HttpResponsePtr &)> &&)>> postRoutingAdvices_;
    std::vector<std::function<void(const HttpRequestImplPtr &, const HttpResponsePtr &)>> postHandlingAdvices_;

    class CtrlBinder
    {
      public:
        std::string controllerName_;
        std::vector<std::string> filterNames_;
        std::vector<std::function<void(const HttpRequestImplPtr &, std::function<void(const HttpResponsePtr &)> &&)>> filters_;
        std::shared_ptr<HttpSimpleControllerBase> controller_;
        IOThreadStorage<HttpResponsePtr> responseCache_;
        bool isCORS_{false};
    };

    HttpControllersRouter httpCtrlsRouter_;

    template <typename T>
    void doAdvicesChain(const std::vector<T> &advices,
                        size_t index,
                        const HttpRequestImplPtr &req,
                        std::shared_ptr<std::function<void(const HttpResponsePtr &)>> callbackPtr,
                        std::function<void()> finalCallback);
};

}  // namespace drogon

#endif  // HTTPSIMPLECONTROLLERSROUTER_HPP