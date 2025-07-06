#include "TaskTimeoutFlag.hpp"
using namespace drogon;

TaskTimeoutFlag::TaskTimeoutFlag(trantor::EventLoop *loop,
                                 const std::chrono::duration<double> &timeout,
                                 std::function<void()> timeoutCallback)
    : loop_(loop), timeout_(timeout), timeoutFunc_(timeoutCallback)
{
}

void TaskTimeoutFlag::runTimer()
{
    std::weak_ptr<TaskTimeoutFlag> weakPtr = shared_from_this();
    loop_->runAfter(timeout_, [weakPtr]() {
        auto thisPtr = weakPtr.lock();
        if (!thisPtr)
            return;
        if (thisPtr->done())
            return;
        thisPtr->timeoutFunc_();
    });
}

bool TaskTimeoutFlag::done()
{
    return isDone_.exchange(true);
}