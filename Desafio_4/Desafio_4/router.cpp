#include "Router.h"
#include <iostream>

Router::Router(const std::string& id) : id(id) {}
Router::~Router() {}

std::string Router::getId() const { return id; }

void Router::addNeighbor(const std::string& neighborId, int cost) {
    neighbors[neighborId] = cost;
}

void Router::removeNeighbor(const std::string& neighborId) {
    neighbors.erase(neighborId);
}

const std::map<std::string,int>& Router::getNeighbors() const {
    return neighbors;
}

void Router::setCostTable(const std::map<std::string,int>& ct) {
    costTable = ct;
}

void Router::setPathTable(const std::map<std::string,std::vector<std::string>>& pt) {
    pathTable = pt;
}

const std::map<std::string,int>& Router::getCostTable() const {
    return costTable;
}

const std::map<std::string,std::vector<std::string>>& Router::getPathTable() const {
    return pathTable;
}

void Router::printNeighbors() const {
    std::cout << "Neighbors of " << id << ":\n";
    for (auto &p : neighbors) {
        std::cout << "  " << p.first << " cost=" << p.second << "\n";
    }
}

void Router::printTables() const {
    std::cout << "Cost Table for " << id << ":\n";
    for (auto &p : costTable) {
        std::cout << "  to " << p.first << " cost=" << p.second << "\n";
    }
    std::cout << "Path Table for " << id << ":\n";
    for (auto &p: pathTable) {
        std::cout << "  to " << p.first << " path=";
        for (size_t i=0;i<p.second.size();++i) {
            std::cout << p.second[i] << (i+1<p.second.size() ? "->" : "");
        }
        std::cout << "\n";
    }
}
