#include "NotFound.hpp"
#include "gtest/gtest.h"

namespace drogon
{

class HttpViewDataMock
{
  public:
    template <typename T>
    T get(const std::string &key) const
    {
        if (key == "version")
        {
            return static_cast<T>("1.0.0");
        }
        return T{};
    }
};

TEST(NotFoundTest, GenTextContainsVersion)
{
    HttpViewDataMock mockData;
    std::string result = NotFound::genText(mockData);
    EXPECT_NE(result.find("<html>"), std::string::npos);
    EXPECT_NE(result.find("<head><title>404 Not Found</title></head>"), std::string::npos);
    EXPECT_NE(result.find("<center><h1>404 Not Found</h1></center>"), std::string::npos);
    EXPECT_NE(result.find("drogon/1.0.0"), std::string::npos);
    EXPECT_NE(result.find("<!-- a padding to disable MSIE and Chrome friendly error page -->"), std::string::npos);
}

}  // namespace drogon