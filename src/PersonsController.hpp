#ifndef PERSONSCONTROLLER_HPP
#define PERSONSCONTROLLER_HPP

#include <drogon/HttpController.h>
#include <functional>
#include <json/json.h>
#include "PersonsController.h" // For Person, PersonInfo, etc.

class PersonsController : public drogon::HttpController<PersonsController> {
public:
    void get(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;
    void getOne(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int personId) const;
    void createOne(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, Person &&pPerson) const;
    void updateOne(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int personId, Person &&pPerson) const;
    void deleteOne(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int personId) const;
    void getDirectReports(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int personId) const;

    struct PersonDetails {
        int id;
        std::string first_name;
        std::string last_name;
        drogon_model::org_chart::Date hire_date;
        Json::Value manager;
        Json::Value department;
        Json::Value job;

        explicit PersonDetails(const PersonInfo &personInfo);
        Json::Value toJson();
    };
};

namespace drogon {
    template<>
    Person fromRequest(const drogon::HttpRequest &req);
}

#endif // PERSONSCONTROLLER_HPP