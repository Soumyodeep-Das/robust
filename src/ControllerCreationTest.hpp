#ifndef CONTROLLERCREATIONTEST_HPP
#define CONTROLLERCREATIONTEST_HPP

#include <drogon/HttpController.h>
#include <drogon/HttpSimpleController.h>
#include <drogon/WebSocketController.h>

class Ctrl : public drogon::HttpController<Ctrl, false>
{
  public:
    static void initPathRouting();
    static bool created;
};

class SimpleCtrl : public drogon::HttpController<Ctrl, false>
{
  public:
    static void initPathRouting();
    static bool created;
};

class WsCtrl : public drogon::WebSocketController<WsCtrl, false>
{
  public:
    static void initPathRouting();
    static bool created;
};

#endif // CONTROLLERCREATIONTEST_HPP