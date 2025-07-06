#ifndef JWTPPLUGIN_HPP
#define JWTPPLUGIN_HPP

#include <drogon/drogon.h>

class JwtPlugin
{
public:
    JwtPlugin();
    ~JwtPlugin();

    void initAndStart(const drogon::Json::Value &config);
    drogon::Json::Value init();
    void shutdown();
};

#endif // JWTPPLUGIN_HPP