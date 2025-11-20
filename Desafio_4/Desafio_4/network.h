#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

static const int INF_COST = 1000000000;

class Network {
public:
    Network();

    void addRouter(char id);
    void addLink(char a, char b, int cost);

    // carga desde archivo. Formato aceptado:
    // nRouters
    // <lista de routers separados por espacios o en una linea>
    // nConexiones
    // A B 5
    // B C 2
    bool loadFromFile(const std::string& filename);

    void printNetwork() const;

    // Calcula tablas (ejecuta Dijkstra desde cada router)
    void computeAllTables();

    // Imprime las tablas de enrutamiento (matriz de costos) y los caminos completos
    void printAllRoutingTables() const;

    // Consultas:
    // devuelve el camino (lista de nodos como strings) desde "from" hasta "to"
    std::vector<std::string> getShortestPath(const std::string& from, const std::string& to) const;

    // devuelve costo o -1 si no hay camino
    int getPathCost(const std::string& from, const std::string& to) const;

private:
    // lista de routers (por ejemplo: "A","B","C")
    std::vector<std::string> routers;

    // lista de adyacencia: node -> (neighbor -> cost)
    std::map<std::string, std::map<std::string,int>> adjList;

    // resultados de Dijkstra:
    // costTables[source][dest] = costo mínimo desde source a dest
    std::map<std::string, std::map<std::string,int>> costTables;

    // pathTables[source][dest] = vector<string> con el camino completo (incluye source y dest)
    std::map<std::string, std::map<std::string,std::vector<std::string>>> pathTables;
};

#endif // NETWORK_H
