#ifndef SIMPLEREVERSEPROXY_HPP
#define SIMPLEREVERSEPROXY_HPP

#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <json/json.h>
#include <drogon/HttpClient.h>
#include <drogon/HttpTypes.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/Plugin.h>

namespace my_plugin
{

class SimpleReverseProxy : public drogon::Plugin<SimpleReverseProxy>
{
  public:
    void initAndStart(const Json::Value &config) override;
    void shutdown() override;

  private:
    void preRouting(const drogon::HttpRequestPtr &req,
                    drogon::AdviceCallback &&callback,
                    drogon::AdviceChainCallback &&pass);

    std::vector<std::string> backendAddrs_;
    drogon::ThreadSafe<std::vector<drogon::HttpClientPtr>> clients_;
    drogon::ThreadSafe<size_t> clientIndex_;
    int pipeliningDepth_{0};
    bool sameClientToSameBackend_{false};
    int connectionFactor_{1};
};

}  // namespace my_plugin

#endif  // SIMPLEREVERSEPROXY_HPP