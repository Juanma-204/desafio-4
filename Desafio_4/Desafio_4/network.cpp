#include "network.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <limits>
#include <algorithm>

Network::Network() {}

// agrega un router (si ya existe, lo ignora)
void Network::addRouter(char id) {
    std::string s(1, id);
    // evitar duplicados
    if (std::find(routers.begin(), routers.end(), s) == routers.end()) {
        routers.push_back(s);
        adjList[s] = {};
    }
}

void Network::addLink(char a, char b, int cost) {
    std::string A(1, a), B(1, b);
    // asegurarse que los routers existan
    addRouter(a);
    addRouter(b);
    adjList[A][B] = cost;
    adjList[B][A] = cost;
}

bool Network::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "No se pudo abrir el archivo: " << filename << "\n";
        return false;
    }

    // Leer número de routers
    int numRouters;
    if (!(file >> numRouters)) {
        std::cout << "Formato inválido: no se leyó número de routers\n";
        return false;
    }

    // Leer lista de routers: puede estar en una línea con separadores
    routers.clear();
    adjList.clear();

    // Intentamos leer hasta tener numRouters ids (saltando posibles saltos de línea)
    for (int i = 0; i < numRouters; ++i) {
        char id;
        if (!(file >> id)) {
            std::cout << "Formato inválido: faltan identificadores de routers\n";
            return false;
        }
        addRouter(id);
    }

    int conexiones;
    if (!(file >> conexiones)) {
        std::cout << "Formato inválido: no se leyó número de conexiones\n";
        return false;
    }

    for (int i = 0; i < conexiones; ++i) {
        char a, b;
        int cost;
        if (!(file >> a >> b >> cost)) {
            std::cout << "Formato inválido en conexión " << i << "\n";
            return false;
        }
        addLink(a, b, cost);
    }

    return true;
}

void Network::printNetwork() const {
    std::cout << "Topología de red:\n";
    for (const auto& r : adjList) {
        std::cout << r.first << " -> ";
        bool first = true;
        for (const auto& nb : r.second) {
            if (!first) std::cout << " ";
            std::cout << "(" << nb.first << ", " << nb.second << ")";
            first = false;
        }
        std::cout << "\n";
    }
}

// Implementación de Dijkstra desde un nodo source
static void dijkstra_single_source(
    const std::map<std::string, std::map<std::string,int>>& adj,
    const std::string& source,
    std::map<std::string,int>& dist,
    std::map<std::string,std::string>& prev)
{
    // inicializar
    dist.clear();
    prev.clear();
    for (const auto& p : adj) {
        dist[p.first] = INF_COST;
        prev[p.first] = ""; // sin predecesor
    }

    if (adj.find(source) == adj.end()) return;
    dist[source] = 0;

    // min-heap por costo
    using Pair = std::pair<int, std::string>;
    auto cmp = [](const Pair& a, const Pair& b){ return a.first > b.first; };
    std::priority_queue<Pair, std::vector<Pair>, decltype(cmp)> pq(cmp);

    pq.push({0, source});

    while (!pq.empty()) {
        auto [d,u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;

        // para cada vecino
        const auto& neighbors = adj.at(u);
        for (const auto& nb : neighbors) {
            const std::string& v = nb.first;
            int w = nb.second;
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

void Network::computeAllTables() {
    costTables.clear();
    pathTables.clear();

    // ejecutar Dijkstra desde cada router registrado
    for (const auto& src : routers) {
        std::map<std::string,int> dist;
        std::map<std::string,std::string> prev;
        dijkstra_single_source(adjList, src, dist, prev);

        // guardar costos
        costTables[src] = dist;

        // reconstruir paths para cada destino
        std::map<std::string,std::vector<std::string>> pathsForSrc;
        for (const auto& destPair : adjList) {
            const std::string& dest = destPair.first;
            std::vector<std::string> path;
            if (dist.find(dest) == dist.end() || dist[dest] >= INF_COST) {
                // sin camino -> path vacía
            } else {
                // reconstruir desde dest hasta src usando prev
                std::string cur = dest;
                while (cur != "" && cur != src) {
                    path.insert(path.begin(), cur);
                    cur = prev[cur];
                }
                if (cur == src) {
                    path.insert(path.begin(), src);
                } else {
                    path.clear(); // no hay camino
                }
            }
            pathsForSrc[dest] = path;
        }

        pathTables[src] = pathsForSrc;
    }
}

void Network::printAllRoutingTables() const {
    if (routers.empty()) {
        std::cout << "(No hay routers)\n";
        return;
    }

    // Imprimir cabecera
    std::cout << "    ";
    for (const auto& col : routers) {
        std::cout << col << "   ";
    }
    std::cout << "\n";

    // Cada fila: origen
    for (const auto& src : routers) {
        std::cout << src << " | ";
        // por cada destino
        for (const auto& dest : routers) {
            int cost = INF_COST;
            auto itSrc = costTables.find(src);
            if (itSrc != costTables.end()) {
                auto itCost = itSrc->second.find(dest);
                if (itCost != itSrc->second.end()) cost = itCost->second;
            }
            if (cost >= INF_COST) {
                std::cout << "INF ";
            } else {
                // ajustar ancho para que la matriz quede legible
                if (cost < 10) std::cout << " ";
                std::cout << cost << "  ";
            }
        }
        std::cout << "\n";
    }

    // Imprimir caminos completos (opcional, uno por origen)
    std::cout << "\nCaminos completos por origen:\n";
    for (const auto& src : routers) {
        std::cout << "Desde " << src << ":\n";
        auto itPaths = pathTables.find(src);
        if (itPaths == pathTables.end()) continue;
        for (const auto& dest : routers) {
            const auto& path = itPaths->second.at(dest);
            if (path.empty()) {
                std::cout << "  a " << dest << ": (sin camino)\n";
            } else {
                std::cout << "  a " << dest << ": ";
                for (size_t i = 0; i < path.size(); ++i) {
                    std::cout << path[i] << (i + 1 < path.size() ? "->" : "");
                }
                std::cout << "\n";
            }
        }
    }
}

std::vector<std::string> Network::getShortestPath(const std::string& from, const std::string& to) const {
    auto it = pathTables.find(from);
    if (it == pathTables.end()) return {};
    auto it2 = it->second.find(to);
    if (it2 == it->second.end()) return {};
    return it2->second;
}

int Network::getPathCost(const std::string& from, const std::string& to) const {
    auto it = costTables.find(from);
    if (it == costTables.end()) return -1;
    auto it2 = it->second.find(to);
    if (it2 == it->second.end()) return -1;
    int c = it2->second;
    if (c >= INF_COST) return -1;
    return c;
}
