#ifndef STATICFILEROUTER_HPP
#define STATICFILEROUTER_HPP

#include <string>
#include <vector>
#include <unordered_set>
#include <functional>
#include <memory>
#include <string_view>
#include <trantor/net/EventLoop.h>
#include "HttpRequestImpl.h"
#include "HttpResponseImpl.h"

namespace drogon
{
class StaticFileRouter
{
  public:
    void init(const std::vector<trantor::EventLoop *> &ioloops);

    void route(const HttpRequestImplPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback);

    void sendStaticFileResponse(const std::string &filePath,
                                const HttpRequestImplPtr &req,
                                std::function<void(const HttpResponsePtr &)> &&callback,
                                const std::string_view &defaultContentType);

    void setFileTypes(const std::vector<std::string> &types);

    void defaultHandler(const HttpRequestPtr &req,
                        std::function<void(const HttpResponsePtr &)> &&callback);

  private:
    // Private members omitted for brevity
};
}  // namespace drogon

#endif  // STATICFILEROUTER_HPP