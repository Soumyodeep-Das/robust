#ifndef DIGESTAUTHFILTER_HPP
#define DIGESTAUTHFILTER_HPP

#include <map>
#include <string>
#include <vector>

enum HttpMethod
{
    Get,
    Post,
    Head,
    Put,
    Delete,
    Options,
    Patch
};

using HttpAttribute = std::pair<std::string, std::string>;
using HttpAttributeList = std::vector<HttpAttribute>;

class HttpRequestPtr;
class HttpResponse;
using FilterCallback = void (*)(const std::shared_ptr<HttpResponse> &);
using FilterChainCallback = void (*)();

class DigestAuthFilter
{
public:
    DigestAuthFilter(const std::map<std::string, std::string> &credentials,
                     const std::string &realm,
                     const std::string &opaque);

    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&cb,
                  FilterChainCallback &&ccb);

    static bool isEndOfAttributeName(size_t pos,
                                     size_t len,
                                     const char *data);

    static void httpParseAttributes(const char *data,
                                    size_t len,
                                    HttpAttributeList &attributes);

    static bool httpHasAttribute(const HttpAttributeList &attributes,
                                 const std::string &name,
                                 std::string *value);

private:
    std::map<std::string, std::string> credentials;
    std::string realm;
    std::string opaque;
};

std::string method2String(HttpMethod m);
std::string toLower(const std::string &in);

#endif // DIGESTAUTHFILTER_HPP