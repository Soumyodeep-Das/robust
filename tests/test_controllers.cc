#include "test_controllers.hpp"

void RemoteAPITest()
{
    auto client = drogon::HttpClient::newHttpClient("http://localhost:3000");
    auto req = drogon::HttpRequest::newHttpRequest();
    req->setPath("/jobs");
    client->sendRequest(req, [](drogon::ReqResult res, const drogon::HttpResponsePtr& resp) {
        // There's nothing we can do if the request didn't reach the server
        // or the server generated garbage.
        if (res != drogon::ReqResult::Ok || resp == nullptr)
        {
            // Handle error or log
            return;
        }

        // Normally assertions or checks would be here but removed from source
    });
}