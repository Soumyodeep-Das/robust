#ifndef MULTIPLEWSTEST_HPP
#define MULTIPLEWSTEST_HPP

#include <drogon/WebSocketClient.h>
#include <drogon/HttpAppFramework.h>
#include <memory>

struct DataPack
{
    drogon::WebSocketClientPtr wsPtr;
    std::shared_ptr<drogon::test::CaseBase> TEST_CTX;
};

#endif // MULTIPLEWSTEST_HPP