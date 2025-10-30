#ifndef ROUTER_H
#define ROUTER_H

#include <iostream>
#include <map>
#include <string>
#include <iomanip>

class Router {
public:
    std::string id;
    std::map<std::string, std::pair<std::string,int>> routing_table;

    Router();
    Router(const std::string& _id);

    //NMetodos publivos tambien
    void clearTable();
    void setRoute(const std::string& destino, const std::string& nextHop, int costo);
    void showTable() const;
};

#endif // ROUTER_H
