#include "HttpViewData.hpp"

using namespace drogon;

std::string HttpViewData::htmlTranslate(const char *str, size_t length)
{
    std::string ret;
    ret.reserve(length + 64);
    auto end = str + length;
    while (str != end)
    {
        switch (*str)
        {
            case '"':
                ret.append("&quot;", 6);
                break;
            case '&':
                ret.append("&amp;", 5);
                break;
            case '<':
                ret.append("&lt;", 4);
                break;
            case '>':
                ret.append("&gt;", 4);
                break;
            default:
                ret.push_back(*str);
                break;
        }
        ++str;
    }
    return ret;
}