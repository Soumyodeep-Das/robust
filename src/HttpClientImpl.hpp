#ifndef HTTPCLIENTIMPL_HPP
#define HTTPCLIENTIMPL_HPP

#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <functional>
#include <utility>
#include <trantor/net/EventLoop.h>
#include <trantor/net/InetAddress.h>
#include <trantor/net/TcpClient.h>
#include <drogon/HttpTypes.h>
#include "HttpClient.h"

namespace drogon
{
class HttpResponseImpl;
using HttpResponseImplPtr = std::shared_ptr<HttpResponseImpl>;

class HttpClientImpl : public std::enable_shared_from_this<HttpClientImpl>
{
  public:
    HttpClientImpl(trantor::EventLoop *loop,
                   const trantor::InetAddress &addr,
                   bool useSSL,
                   bool useOldTLS,
                   bool validateCert);
    HttpClientImpl(trantor::EventLoop *loop,
                   const std::string &hostString,
                   bool useOldTLS,
                   bool validateCert);
    ~HttpClientImpl();

    void sendRequest(const drogon::HttpRequestPtr &req,
                     const drogon::HttpReqCallback &callback,
                     double timeout = 0);
    void sendRequest(const drogon::HttpRequestPtr &req,
                     drogon::HttpReqCallback &&callback,
                     double timeout = 0);

    void setCertPath(const std::string &cert, const std::string &key);
    void addSSLConfigs(
        const std::vector<std::pair<std::string, std::string>> &sslConfCmds);

  private:
    void createTcpClient();
    void sendRequestInLoop(const drogon::HttpRequestPtr &req,
                           drogon::HttpReqCallback &&callback);
    void sendRequestInLoop(const drogon::HttpRequestPtr &req,
                           drogon::HttpReqCallback &&callback,
                           double timeout);
    void sendReq(const trantor::TcpConnectionPtr &connPtr,
                 const drogon::HttpRequestPtr &req);
    void handleResponse(const HttpResponseImplPtr &resp,
                        std::pair<drogon::HttpRequestPtr, drogon::HttpReqCallback> &&reqAndCb,
                        const trantor::TcpConnectionPtr &connPtr);
    void onRecvMessage(const trantor::TcpConnectionPtr &connPtr,
                       trantor::MsgBuffer *msg);
    void onError(ReqResult result);
    void handleCookies(const HttpResponseImplPtr &resp);

    trantor::EventLoop *loop_;
    trantor::InetAddress serverAddr_;
    bool useSSL_{false};
    bool validateCert_{true};
    bool useOldTLS_{false};
    std::string domain_;
    std::string userAgent_;
    std::string clientCertPath_;
    std::string clientKeyPath_;
    std::vector<std::pair<std::string, std::string>> sslConfCmds_;
    std::shared_ptr<trantor::TcpClient> tcpClientPtr_;
    std::shared_ptr<trantor::Resolver> resolverPtr_;
    bool dns_{false};
    std::deque<std::pair<drogon::HttpRequestPtr, drogon::HttpReqCallback>> requestsBuffer_;
    std::queue<std::pair<drogon::HttpRequestPtr, drogon::HttpReqCallback>> pipeliningCallbacks_;
    size_t pipeliningDepth_{10};
    std::vector<drogon::Cookie> validCookies_;
    bool enableCookies_{false};
    size_t bytesSent_{0};
    size_t bytesReceived_{0};

    bool onDefaultPort() const;
    std::string host() const;
    uint16_t port() const;
};

}  // namespace drogon