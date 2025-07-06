#include "gtest/gtest.h"
#include "HttpViewData.hpp"

using namespace drogon;

TEST(HttpViewDataTest, HtmlTranslateEmpty)
{
    std::string result = HttpViewData::htmlTranslate("", 0);
    EXPECT_EQ(result, "");
}

TEST(HttpViewDataTest, HtmlTranslateNoSpecialChars)
{
    const char *input = "Hello World";
    std::string result = HttpViewData::htmlTranslate(input, 11);
    EXPECT_EQ(result, "Hello World");
}

TEST(HttpViewDataTest, HtmlTranslateQuotes)
{
    const char *input = "\"";
    std::string result = HttpViewData::htmlTranslate(input, 1);
    EXPECT_EQ(result, "&quot;");
}

TEST(HttpViewDataTest, HtmlTranslateAmpersand)
{
    const char *input = "&";
    std::string result = HttpViewData::htmlTranslate(input, 1);
    EXPECT_EQ(result, "&amp;");
}

TEST(HttpViewDataTest, HtmlTranslateLessThan)
{
    const char *input = "<";
    std::string result = HttpViewData::htmlTranslate(input, 1);
    EXPECT_EQ(result, "&lt;");
}

TEST(HttpViewDataTest, HtmlTranslateGreaterThan)
{
    const char *input = ">";
    std::string result = HttpViewData::htmlTranslate(input, 1);
    EXPECT_EQ(result, "&gt;");
}

TEST(HttpViewDataTest, HtmlTranslateMixed)
{
    const char *input = "\"&<>";
    std::string result = HttpViewData::htmlTranslate(input, 4);
    EXPECT_EQ(result, "&quot;&amp;&lt;&gt;");
}

TEST(HttpViewDataTest, HtmlTranslateLongString)
{
    const char *input = "This is a test string with \"quotes\", & ampersand, < less than and > greater than.";
    std::string expected = "This is a test string with &quot;quotes&quot;, &amp; ampersand, &lt; less than and &gt; greater than.";
    std::string result = HttpViewData::htmlTranslate(input, strlen(input));
    EXPECT_EQ(result, expected);
}