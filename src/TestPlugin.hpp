#ifndef TESTPLUGIN_HPP
#define TESTPLUGIN_HPP

#include <thread>
#include <json/json.h>

namespace drogon
{

class TestPlugin
{
public:
    void initAndStart(const Json::Value &config);
    void shutdown();

private:
    int interval_{1};
    bool stop_{false};
    std::thread workThread_;
};

} // namespace drogon

#endif // TESTPLUGIN_HPP