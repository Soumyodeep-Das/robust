#ifndef LISTENERMANAGER_HPP
#define LISTENERMANAGER_HPP

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <utility>
#include <cstdint>
#include <future>
#include <trantor/net/EventLoop.h>
#include <trantor/net/InetAddress.h>

namespace drogon
{
class HttpServer;
class ListenerManager
{
  public:
    using HttpAsyncCallback = std::function<void()>;
    using WebSocketNewAsyncCallback = std::function<void()>;
    using ConnectionCallback = std::function<void()>;
    using HttpRequestPtr = std::shared_ptr<void>;
    using HttpResponsePtr = std::shared_ptr<void>;

    void addListener(
        const std::string &ip,
        uint16_t port,
        bool useSSL,
        const std::string &certFile,
        const std::string &keyFile,
        bool useOldTLS,
        const std::vector<std::pair<std::string, std::string>> &sslConfCmds);

    std::vector<trantor::EventLoop *> createListeners(
        const HttpAsyncCallback &httpCallback,
        const WebSocketNewAsyncCallback &webSocketCallback,
        const ConnectionCallback &connectionCallback,
        size_t connectionTimeout,
#ifdef OpenSSL_FOUND
        const std::string &globalCertFile,
        const std::string &globalKeyFile,
        const std::vector<std::pair<std::string, std::string>> &sslConfCmds,
#else
        const std::string &globalCertFile,
        const std::string &globalKeyFile,
        const std::vector<std::pair<std::string, std::string>> &sslConfCmds,
#endif
        size_t threadNum,
        const std::vector<std::function<HttpResponsePtr(const HttpRequestPtr &)>>
            &syncAdvices,
        const std::vector<
            std::function<void(const HttpRequestPtr &, const HttpResponsePtr &)>>
            &preSendingAdvices);

    void startListening();

    trantor::EventLoop *getIOLoop(size_t id) const;

    void stopListening();

    std::vector<trantor::InetAddress> getListeners() const;

  private:
    struct Listener
    {
        std::string ip_;
        uint16_t port_;
        bool useSSL_;
        std::string certFile_;
        std::string keyFile_;
        bool useOldTLS_;
        std::vector<std::pair<std::string, std::string>> sslConfCmds_;
        Listener(const std::string &ip,
                 uint16_t port,
                 bool useSSL,
                 const std::string &certFile,
                 const std::string &keyFile,
                 bool useOldTLS,
                 const std::vector<std::pair<std::string, std::string>> &sslConfCmds)
            : ip_(ip),
              port_(port),
              useSSL_(useSSL),
              certFile_(certFile),
              keyFile_(keyFile),
              useOldTLS_(useOldTLS),
              sslConfCmds_(sslConfCmds)
        {
        }
    };

    std::vector<Listener> listeners_;
    std::vector<std::shared_ptr<trantor::EventLoopThread>> listeningloopThreads_;
    std::vector<trantor::EventLoop *> ioLoops_;
    std::shared_ptr<trantor::EventLoopThreadPool> ioLoopThreadPoolPtr_;
    std::vector<std::shared_ptr<HttpServer>> servers_;
};

}  // namespace drogon

#endif  // LISTENERMANAGER_HPP