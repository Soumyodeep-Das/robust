#ifndef API_V1_APITEST_HPP
#define API_V1_APITEST_HPP

#include <string>
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace api {
namespace v1 {

class HttpRequestPtr;
class HttpResponsePtr;
class HttpViewData;
class AttributesPtr;

class ApiTest {
public:
    void rootGet(const HttpRequestPtr &req,
                 std::function<void(const HttpResponsePtr &)> &&callback);
    void rootPost(const HttpRequestPtr &req,
                  std::function<void(const HttpResponsePtr &)> &&callback);
    void get(const HttpRequestPtr &req,
             std::function<void(const HttpResponsePtr &)> &&callback,
             int p1,
             std::string &&p2);
    void your_method_name(const HttpRequestPtr &req,
                          std::function<void(const HttpResponsePtr &)> &&callback,
                          double p1,
                          int p2) const;
    void staticApi(const HttpRequestPtr &req,
                   std::function<void(const HttpResponsePtr &)> &&callback);
    void get2(const HttpRequestPtr &req,
              std::function<void(const HttpResponsePtr &)> &&callback,
              std::string &&p1);
    void jsonTest(std::shared_ptr<class Json::Value> &&json,
                  std::function<void(const HttpResponsePtr &)> &&callback);
    void formTest(const HttpRequestPtr &req,
                  std::function<void(const HttpResponsePtr &)> &&callback);
    void attributesTest(const HttpRequestPtr &req,
                        std::function<void(const HttpResponsePtr &)> &&callback);
    void regexTest(const HttpRequestPtr &req,
                   std::function<void(const HttpResponsePtr &)> &&callback,
                   int p1,
                   std::string &&p2);
    void cacheTest(const HttpRequestPtr &req,
                   std::function<void(const HttpResponsePtr &)> &&callback);
    void cacheTest2(const HttpRequestPtr &req,
                    std::function<void(const HttpResponsePtr &)> &&callback);
    void cacheTestRegex(const HttpRequestPtr &req,
                        std::function<void(const HttpResponsePtr &)> &&callback);
};

} // namespace v1
} // namespace api

#endif // API_V1_APITEST_HPP