#ifndef RED_H
#define RED_H
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm>
#include "router.h"

class red {
public:
    std::vector<Router> routers;
    std::map<std::string, std::vector<std::pair<std::string,int>>> adj;

    //Estos son los metodos publicos
    void addEdge(const std::string& a, const std::string& b, int cost);
    bool loadFromFile(const std::string& filename);
    void updateRoutingTables();
    std::pair<int, std::vector<std::string>> getCostAndPath(const std::string& origen, const std::string& destino);
    void showRouterTable(const std::string& id) const;
    void listRouters() const;
    void addRouter(const std::string& id);
    void removeRouter(const std::string& id);
    void addOrUpdateEdge(const std::string& a, const std::string& b, int cost);

private:
    int findRouterIndex(const std::string& id) const;
    std::pair<std::map<std::string,int>, std::map<std::string,std::string>> dijkstra(const std::string& source);
};
#endif // RED_H
