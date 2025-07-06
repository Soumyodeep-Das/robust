#ifndef ACCESSLOGGER_HPP
#define ACCESSLOGGER_HPP

#include <drogon/drogon.h>
#include <drogon/plugins/AccessLogger.h>
#include <functional>
#include <map>
#include <string>
#include <vector>

namespace drogon
{
namespace plugin
{

class AccessLogger
{
  public:
    using LogFunction = std::function<void(trantor::LogStream &,
                                           const drogon::HttpRequestPtr &,
                                           const drogon::HttpResponsePtr &)>;

    void initAndStart(const Json::Value &config);
    void shutdown();

  private:
    void logging(trantor::LogStream &stream,
                 const drogon::HttpRequestPtr &req,
                 const drogon::HttpResponsePtr &resp);
    void createLogFunctions(std::string format);
    LogFunction newLogFunction(const std::string &placeholder);

    static void outputReqPath(trantor::LogStream &stream,
                              const drogon::HttpRequestPtr &req,
                              const drogon::HttpResponsePtr &resp);
    void outputDate(trantor::LogStream &stream,
                    const drogon::HttpRequestPtr &req,
                    const drogon::HttpResponsePtr &resp) const;
    void outputReqDate(trantor::LogStream &stream,
                       const drogon::HttpRequestPtr &req,
                       const drogon::HttpResponsePtr &resp) const;
    static void outputReqQuery(trantor::LogStream &stream,
                               const drogon::HttpRequestPtr &req,
                               const drogon::HttpResponsePtr &resp);
    static void outputReqURL(trantor::LogStream &stream,
                             const drogon::HttpRequestPtr &req,
                             const drogon::HttpResponsePtr &resp);
    static void outputVersion(trantor::LogStream &stream,
                              const drogon::HttpRequestPtr &req,
                              const drogon::HttpResponsePtr &resp);
    static void outputReqLine(trantor::LogStream &stream,
                              const drogon::HttpRequestPtr &req,
                              const drogon::HttpResponsePtr &resp);
    static void outputRemoteAddr(trantor::LogStream &stream,
                                 const drogon::HttpRequestPtr &req,
                                 const drogon::HttpResponsePtr &resp);
    static void outputLocalAddr(trantor::LogStream &stream,
                                const drogon::HttpRequestPtr &req,
                                const drogon::HttpResponsePtr &resp);
    static void outputReqLength(trantor::LogStream &stream,
                                const drogon::HttpRequestPtr &req,
                                const drogon::HttpResponsePtr &resp);
    static void outputRespLength(trantor::LogStream &stream,
                                 const drogon::HttpRequestPtr &req,
                                 const drogon::HttpResponsePtr &resp);
    static void outputMethod(trantor::LogStream &stream,
                             const drogon::HttpRequestPtr &req,
                             const drogon::HttpResponsePtr &resp);
    static void outputThreadNumber(trantor::LogStream &stream,
                                   const drogon::HttpRequestPtr &req,
                                   const drogon::HttpResponsePtr &resp);
    static void outputReqHeader(trantor::LogStream &stream,
                                const drogon::HttpRequestPtr &req,
                                const std::string &headerName);
    static void outputReqCookie(trantor::LogStream &stream,
                                const drogon::HttpRequestPtr &req,
                                const std::string &cookie);
    static void outputRespHeader(trantor::LogStream &stream,
                                 const drogon::HttpResponsePtr &resp,
                                 const std::string &headerName);
    static void outputStatusString(trantor::LogStream &stream,
                                   const drogon::HttpRequestPtr &req,
                                   const drogon::HttpResponsePtr &resp);
    static void outputStatusCode(trantor::LogStream &stream,
                                 const drogon::HttpRequestPtr &req,
                                 const drogon::HttpResponsePtr &resp);
    static void outputProcessingTime(trantor::LogStream &stream,
                                     const drogon::HttpRequestPtr &req,
                                     const drogon::HttpResponsePtr &resp);
    static void outputRespContentType(trantor::LogStream &stream,
                                      const drogon::HttpRequestPtr &req,
                                      const drogon::HttpResponsePtr &resp);

  private:
    std::map<std::string, LogFunction> logFunctionMap_;
    std::vector<LogFunction> logFunctions_;
    bool useLocalTime_{true};
    bool showMicroseconds_{true};
    std::string timeFormat_;
    bool useCustomTimeFormat_{false};
    int logIndex_{0};
    drogon::AsyncFileLogger asyncFileLogger_;
};

}  // namespace plugin
}  // namespace drogon

#endif  // ACCESSLOGGER_HPP