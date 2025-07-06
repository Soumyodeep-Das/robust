#include <drogon/CacheMap.h>
#include <trantor/net/EventLoopThread.h>
#include <string>
#include <thread>
#include <chrono>

using namespace drogon;
using namespace std::chrono_literals;

void runCacheMapTest()
{
    trantor::EventLoopThread loopThread;
    loopThread.run();
    drogon::CacheMap<std::string, std::string> cache(loopThread.getLoop(),
                                                     0.1,
                                                     4,
                                                     30);

    for (size_t i = 1; i < 40; i++)
        cache.insert(std::to_string(i), "a", i);
    cache.insert("bla", "");
    cache.insert("zzz", "-");
    std::this_thread::sleep_for(3s);
    (void)(cache.find("0") == false);  // doesn't exist
    (void)(cache.find("1") == false);  // timeout
    (void)(cache.find("15") == true);
    (void)(cache.find("bla") == true);

    cache.erase("30");
    (void)(cache.find("30") == false);

    cache.modify("bla", [](std::string& s) { s = "asd"; });
    (void)(cache["bla"] == "asd");

    std::string content;
    cache.findAndFetch("zzz", content);
    (void)(content == "-");
}