#ifndef DBCLIENTMANAGERSKIPPED_HPP
#define DBCLIENTMANAGERSKIPPED_HPP

#include <string>
#include <vector>

namespace trantor
{
class EventLoop;
}

namespace drogon
{
namespace orm
{
class DbClientManager
{
  public:
    void createDbClients(const std::vector<trantor::EventLoop *> &ioloops);
    void createDbClient(const std::string &dbType,
                        const std::string &host,
                        unsigned short port,
                        const std::string &databaseName,
                        const std::string &userName,
                        const std::string &password,
                        size_t connectionNum,
                        const std::string &filename,
                        const std::string &name,
                        bool isFast,
                        const std::string &characterSet,
                        double timeout);
    bool areAllDbClientsAvailable() const noexcept;
};
}  // namespace orm
}  // namespace drogon

#endif  // DBCLIENTMANAGERSKIPPED_HPP