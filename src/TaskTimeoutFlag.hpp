#ifndef DROGON_TASKTIMEOUTFLAG_HPP
#define DROGON_TASKTIMEOUTFLAG_HPP

#include <chrono>
#include <functional>
#include <memory>
#include <atomic>

namespace trantor
{
class EventLoop;
}

namespace drogon
{
class TaskTimeoutFlag : public std::enable_shared_from_this<TaskTimeoutFlag>
{
  public:
    TaskTimeoutFlag(trantor::EventLoop *loop,
                    const std::chrono::duration<double> &timeout,
                    std::function<void()> timeoutCallback);

    void runTimer();

    bool done();

  private:
    trantor::EventLoop *loop_;
    std::chrono::duration<double> timeout_;
    std::function<void()> timeoutFunc_;
    std::atomic<bool> isDone_{false};
};

}  // namespace drogon

#endif  // DROGON_TASKTIMEOUTFLAG_HPP