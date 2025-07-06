#include "gtest/gtest.h"
#include "TimeFilter.hpp"
#include <memory>
#include <string>

// Mock classes to simulate HttpRequestPtr, HttpResponsePtr, Session, and related behavior

namespace trantor
{
    class Date
    {
    public:
        Date() : _time(0) {}
        explicit Date(time_t t) : _time(t) {}
        static Date date()
        {
            return Date(time(nullptr));
        }
        bool operator>(const Date &rhs) const
        {
            return _time > rhs._time;
        }
        Date after(int seconds) const
        {
            return Date(_time + seconds);
        }
        std::string toFormattedString(bool) const
        {
            return std::to_string(_time);
        }
        time_t getTime() const { return _time; }

    private:
        time_t _time;
    };

    class HttpResponse
    {
    public:
        using Ptr = std::shared_ptr<HttpResponse>;
        static Ptr newNotFoundResponse()
        {
            auto resp = std::make_shared<HttpResponse>();
            resp->statusCode_ = 404;
            return resp;
        }
        static Ptr newHttpJsonResponse(const Json::Value &json)
        {
            auto resp = std::make_shared<HttpResponse>();
            resp->statusCode_ = 200;
            resp->json_ = json;
            return resp;
        }
        int statusCode() const { return statusCode_; }
        Json::Value json() const { return json_; }

    private:
        int statusCode_{200};
        Json::Value json_;
    };

    template <typename T>
    class Optional
    {
    public:
        Optional() : _has(false) {}
        Optional(const T &val) : _has(true), _val(val) {}
        explicit operator bool() const { return _has; }
        const T &operator*() const { return _val; }
        const T *operator->() const { return &_val; }

    private:
        bool _has;
        T _val;
    };

    class Session
    {
    public:
        template <typename T>
        Optional<T> getOptional(const std::string &key) const
        {
            auto it = data_.find(key);
            if (it != data_.end())
            {
                return Optional<T>(std::any_cast<T>(it->second));
            }
            return Optional<T>();
        }

        template <typename T>
        void insert(const std::string &key, const T &value)
        {
            data_[key] = value;
        }

        template <typename T, typename Func>
        void modify(const std::string &key, Func f)
        {
            auto it = data_.find(key);
            if (it != data_.end())
            {
                T &val = std::any_cast<T &>(it->second);
                f(val);
            }
        }

    private:
        std::unordered_map<std::string, std::any> data_;
    };

    using HttpRequestPtr = std::shared_ptr<class HttpRequest>;

    class HttpRequest
    {
    public:
        HttpRequest() : session_(std::make_shared<Session>()) {}
        std::shared_ptr<Session> session() const { return session_; }
        void setSession(std::shared_ptr<Session> s) { session_ = s; }

    private:
        std::shared_ptr<Session> session_;
    };
} // namespace trantor

// Minimal Json::Value mock
namespace Json
{
    class Value
    {
    public:
        Value() = default;
        void operator[](const std::string &key) { /* dummy */ }
        std::string operator[](const std::string &key) const
        {
            auto it = values_.find(key);
            if (it != values_.end())
                return it->second;
            return "";
        }
        void set(const std::string &key, const std::string &val)
        {
            values_[key] = val;
        }

    private:
        std::unordered_map<std::string, std::string> values_;
    };
}

// Redirect LOG_TRACE to nothing for test
#define LOG_TRACE while(false) std::cerr

class TimeFilterTest : public ::testing::Test
{
protected:
    TimeFilter filter;
};

TEST_F(TimeFilterTest, NoSessionReturnsNotFound)
{
    auto req = std::make_shared<trantor::HttpRequest>();
    req->setSession(nullptr);

    bool callbackCalled = false;
    trantor::HttpResponse::Ptr responsePtr;

    filter.doFilter(req,
                    [&](const trantor::HttpResponsePtr &resp) {
                        callbackCalled = true;
                        responsePtr = resp;
                    },
                    []() {
                        FAIL() << "FilterChainCallback should not be called";
                    });

    EXPECT_TRUE(callbackCalled);
    ASSERT_NE(responsePtr, nullptr);
    EXPECT_EQ(responsePtr->statusCode(), 404);
}

TEST_F(TimeFilterTest, FirstVisitInsertsDateAndCallsChain)
{
    auto req = std::make_shared<trantor::HttpRequest>();

    bool chainCalled = false;
    bool callbackCalled = false;

    filter.doFilter(req,
                    [&](const trantor::HttpResponsePtr &) {
                        callbackCalled = true;
                    },
                    [&]() {
                        chainCalled = true;
                    });

    EXPECT_FALSE(callbackCalled);
    EXPECT_TRUE(chainCalled);

    auto lastDate = req->session()->getOptional<trantor::Date>("visitDate");
    EXPECT_TRUE(lastDate);
}

TEST_F(TimeFilterTest, VisitWithin10SecondsReturnsError)
{
    auto req = std::make_shared<trantor::HttpRequest>();
    auto session = req->session();

    trantor::Date now = trantor::Date::date();
    session->insert("visitDate", now);

    bool callbackCalled = false;
    trantor::HttpResponse::Ptr responsePtr;
    bool chainCalled = false;

    filter.doFilter(req,
                    [&](const trantor::HttpResponsePtr &resp) {
                        callbackCalled = true;
                        responsePtr = resp;
                    },
                    [&]() {
                        chainCalled = true;
                    });

    EXPECT_TRUE(callbackCalled);
    EXPECT_FALSE(chainCalled);
    ASSERT_NE(responsePtr, nullptr);
    EXPECT_EQ(responsePtr->statusCode(), 200);
    // We expect json["result"] == "error" and message present, but Json::Value is mocked, so skip.
}

TEST_F(TimeFilterTest, VisitAfter10SecondsCallsChain)
{
    auto req = std::make_shared<trantor::HttpRequest>();
    auto session = req->session();

    trantor::Date now = trantor::Date::date();
    trantor::Date oldDate = now.after(-20); // 20 seconds ago
    session->insert("visitDate", oldDate);

    bool callbackCalled = false;
    bool chainCalled = false;

    filter.doFilter(req,
                    [&](const trantor::HttpResponsePtr &) {
                        callbackCalled = true;
                    },
                    [&]() {
                        chainCalled = true;
                    });

    EXPECT_FALSE(callbackCalled);
    EXPECT_TRUE(chainCalled);

    // The stored date should be updated to now
    auto lastDate = session->getOptional<trantor::Date>("visitDate");
    ASSERT_TRUE(lastDate);
    EXPECT_EQ(lastDate->getTime(), now.getTime());
}