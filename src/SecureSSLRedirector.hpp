#ifndef SECURESSLREDIRECTOR_HPP
#define SECURESSLREDIRECTOR_HPP

#include <drogon/drogon.h>
#include <drogon/plugins/SecureSSLRedirector.h>
#include <string>
#include <regex>

using namespace drogon;
using namespace drogon::plugin;

class SecureSSLRedirector
{
public:
    void initAndStart(const Json::Value &config);
    void shutdown();
    HttpResponsePtr redirectingAdvice(const HttpRequestPtr &req) const;
    HttpResponsePtr redirectToSSL(const HttpRequestPtr &req) const;

private:
    std::regex exemptPegex_;
    bool regexFlag_{false};
    std::string secureHost_;
};

#endif // SECURESSLREDIRECTOR_HPP