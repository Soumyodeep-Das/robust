#ifndef API_V1_COROTEST_HPP
#define API_V1_COROTEST_HPP

#include <drogon/HttpController.h>
#include <drogon/HttpTypes.h>
#include <trantor/utils/Date.h>
#include <functional>
#include <stdexcept>
#include <chrono>
#include <memory>

namespace api::v1
{
    class CoroTest : public drogon::HttpController<CoroTest>
    {
    public:
        METHOD_LIST_BEGIN
        // Use METHOD_ADD for routing if needed, omitted here for brevity
        METHOD_ADD(CoroTest::get, "/get", drogon::Get);
        METHOD_ADD(CoroTest::get2, "/get2", drogon::Get);
        METHOD_ADD(CoroTest::this_will_fail, "/this_will_fail", drogon::Get);
        METHOD_ADD(CoroTest::this_will_fail2, "/this_will_fail2", drogon::Get);
        METHOD_LIST_END

        drogon::Task<> get(drogon::HttpRequestPtr req,
                           std::function<void(const drogon::HttpResponsePtr &)> callback);

        drogon::Task<drogon::HttpResponsePtr> get2(drogon::HttpRequestPtr req);

        drogon::Task<> this_will_fail(drogon::HttpRequestPtr req,
                                      std::function<void(const drogon::HttpResponsePtr &)> callback);

        drogon::Task<drogon::HttpResponsePtr> this_will_fail2(drogon::HttpRequestPtr req);
    };
} // namespace api::v1

#endif // API_V1_COROTEST_HPP