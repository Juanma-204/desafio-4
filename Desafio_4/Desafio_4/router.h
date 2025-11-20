#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <map>
#include <vector>

class Router {
public:
    Router(const std::string& id);
    ~Router();

    std::string getId() const;

    // vecinos directos: id -> costo
    void addNeighbor(const std::string& neighborId, int cost);
    void removeNeighbor(const std::string& neighborId);
    const std::map<std::string,int>& getNeighbors() const;

    // tablas calculadas por Network
    void setCostTable(const std::map<std::string,int>& costTable);
    void setPathTable(const std::map<std::string,std::vector<std::string>>& pathTable);

    const std::map<std::string,int>& getCostTable() const;
    const std::map<std::string,std::vector<std::string>>& getPathTable() const;

    void printNeighbors() const;
    void printTables() const;

private:
    std::string id;
    std::map<std::string,int> neighbors;

    // tablas de enrutamiento locales
    std::map<std::string,int> costTable;
    std::map<std::string,std::vector<std::string>> pathTable;
};

#endif // ROUTER_H
