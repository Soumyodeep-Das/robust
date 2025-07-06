#ifndef SESSIONMANAGER_HPP
#define SESSIONMANAGER_HPP

#include <memory>
#include <string>
#include <functional>
#include <cassert>
#include <drogon/Session.h>
#include <trantor/net/EventLoop.h>
#include <drogon/utils/Utilities.h>
#include <drogon/CacheMap.h>

namespace drogon
{
class SessionManager
{
  public:
    SessionManager(trantor::EventLoop *loop, size_t timeout);

    SessionPtr getSession(const std::string &sessionID, bool needToSet);

    void changeSessionId(const SessionPtr &sessionPtr);

  private:
    trantor::EventLoop *loop_;
    size_t timeout_;
    std::unique_ptr<CacheMap<std::string, SessionPtr>> sessionMapPtr_;
};

}  // namespace drogon

#endif  // SESSIONMANAGER_HPP