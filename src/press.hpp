#pragma once
#include <string>
#include <vector>
#include <string_view>
#include <memory>

namespace drogon_ctl
{
class press
{
  public:
    std::string detail();
    void handleCommand(std::vector<std::string> &parameters);

  private:
    void doTesting();
    void createRequestAndClients();
    void sendRequest(const class HttpClientPtr &client);
    void outputResults();

    std::string url_;
    std::string host_;
    std::string path_;
    size_t numOfRequests_{1};
    size_t numOfThreads_{1};
    size_t numOfConnections_{1};
    bool processIndication_{true};

    struct Statistics
    {
        size_t numOfRequestsSent_{0};
        size_t numOfGoodResponse_{0};
        size_t numOfBadResponse_{0};
        size_t bytesRecieved_{0};
        long long totalDelay_{0};
        class trantor::Date startDate_;
    } statistics_;

    std::unique_ptr<class trantor::EventLoopThreadPool> loopPool_;
    std::vector<class HttpClientPtr> clients_;
};

void outputErrorAndExit(const std::string_view &err);
}  // namespace drogon_ctl