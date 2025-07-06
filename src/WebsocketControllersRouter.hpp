#ifndef WEBSOCKETCONTROLLERSROUTER_HPP
#define WEBSOCKETCONTROLLERSROUTER_HPP

#include <string>
#include <vector>
#include <tuple>
#include <functional>
#include <memory>
#include <unordered_map>
#include <drogon/HttpTypes.h>
#include <drogon/WebSocketController.h>

namespace drogon
{
class HttpRequestImpl;
class HttpResponse;
class WebSocketConnectionImpl;

using HttpRequestImplPtr = std::shared_ptr<HttpRequestImpl>;
using HttpResponsePtr = std::shared_ptr<HttpResponse>;
using WebSocketConnectionImplPtr = std::shared_ptr<WebSocketConnectionImpl>;

class WebsocketControllersRouter
{
  public:
    void registerWebSocketController(
        const std::string &pathName,
        const std::string &ctrlName,
        const std::vector<internal::HttpConstraint> &filtersAndMethods);

    void route(
        const HttpRequestImplPtr &req,
        std::function<void(const HttpResponsePtr &)> &&callback,
        const WebSocketConnectionImplPtr &wsConnPtr);

    std::vector<std::tuple<std::string, HttpMethod, std::string>> getHandlersInfo() const;

    void init();

  private:
    struct CtrlBinder;
    struct WebSocketControllerRouterItem;

    void doControllerHandler(
        const WebSocketControllerRouterItem &routerItem,
        std::string &wsKey,
        const HttpRequestImplPtr &req,
        std::function<void(const HttpResponsePtr &)> &&callback,
        const WebSocketConnectionImplPtr &wsConnPtr);

    std::unordered_map<std::string, WebSocketControllerRouterItem> wsCtrlMap_;

    std::vector<std::function<void(const HttpRequestImplPtr &)>> postRoutingObservers_;
    std::vector<std::function<void(const HttpRequestImplPtr &,
                                   std::shared_ptr<std::function<void(const HttpResponsePtr &)>> &,
                                   std::function<void()> &&)>> postRoutingAdvices_;
};

#endif  // WEBSOCKETCONTROLLERSROUTER_HPP