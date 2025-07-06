#include "HttpPipeliningTest.hpp"
#include <drogon/HttpClient.h>
#include <drogon/HttpAppFramework.h>
#include <string>
#include <atomic>
#include <cstdlib>

using namespace drogon;

static int counter = -1;

void runHttpPipeliningTest()
{
    auto client = HttpClient::newHttpClient("127.0.0.1", 8848);
    client->setPipeliningDepth(64);

    auto request1 = HttpRequest::newHttpRequest();
    request1->setPath("/pipe");
    request1->setMethod(Head);

    client->sendRequest(
        request1, [](ReqResult r, const HttpResponsePtr &resp) {
            if (r != ReqResult::Ok)
            {
                throw std::runtime_error("Request1 failed");
            }
            auto counterHeader = resp->getHeader("counter");
            int c = std::atoi(counterHeader.data());
            if (c <= counter)
            {
                throw std::runtime_error("The response was received in the wrong order!");
            }
            counter = c;
            if (!resp->body().empty())
            {
                throw std::runtime_error("Response body is not empty");
            }
        });

    auto request2 = HttpRequest::newHttpRequest();
    request2->setPath("/drogon.jpg");
    client->sendRequest(request2,
                        [](ReqResult r, const HttpResponsePtr &resp) {
                            if (r != ReqResult::Ok)
                            {
                                throw std::runtime_error("Request2 failed");
                            }
                            if (resp->getBody().length() != 44618UL)
                            {
                                throw std::runtime_error("Response body length mismatch");
                            }
                        });

    auto request = HttpRequest::newHttpRequest();
    request->setPath("/pipe");
    for (int i = 0; i < 19; ++i)
    {
        client->sendRequest(
            request1, [](ReqResult r, const HttpResponsePtr &resp) {
                if (r != ReqResult::Ok)
                {
                    throw std::runtime_error("Pipelined request failed");
                }
                auto counterHeader = resp->getHeader("counter");
                int c = std::atoi(counterHeader.data());
                if (c <= counter)
                {
                    throw std::runtime_error("The response was received in the wrong order!");
                }
                counter = c;
                if (!resp->body().empty())
                {
                    throw std::runtime_error("Response body is not empty");
                }
            });
    }
}