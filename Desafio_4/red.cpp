#include "red.h"
using namespace std;

int red::findRouterIndex(const string& id) const {
    for (size_t i = 0; i < routers.size(); ++i)
        if (routers[i].id == id) return i;
    return -1;
}

void red::addEdge(const string& a, const string& b, int cost) {
    adj[a].push_back({b, cost});
    adj[b].push_back({a, cost});

    if (findRouterIndex(a) == -1) routers.push_back(Router(a));
    if (findRouterIndex(b) == -1) routers.push_back(Router(b));
}

bool red::loadFromFile(const string& filename) {
    ifstream f(filename);
    if (!f.is_open()) {
        cerr << "No se pudo abrir el archivo: " << filename << "\n";
        return false;
    }
    adj.clear();
    routers.clear();
    string a, b;
    int cost;
    while (f >> a >> b >> cost) addEdge(a, b, cost);
    f.close();
    updateRoutingTables();
    return true;
}

pair<map<string,int>, map<string,string>> red::dijkstra(const string& source) {
    map<string,int> dist;
    map<string,string> prev;
    for (auto &p : adj) {
        dist[p.first] = INT_MAX;
        prev[p.first] = "";
    }

    if (!adj.count(source)) return {dist, prev};
    dist[source] = 0;

    priority_queue<pair<int,string>, vector<pair<int,string>>, greater<pair<int,string>>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;

        for (auto &edge : adj[u]) {
            string v = edge.first;
            int w = edge.second;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    return {dist, prev};
}

void red::updateRoutingTables() {
    for (auto &r : routers) r.clearTable();

    for (auto &r : routers) {
        string source = r.id;
        auto [dist, prev] = dijkstra(source);

        for (auto &q : adj) {
            string destino = q.first;
            if (destino == source) {
                r.setRoute(destino, source, 0);
                continue;
            }
            if (dist[destino] == INT_MAX) {
                r.setRoute(destino, "-", -1);
                continue;
            }
            string cur = destino;
            while (prev[cur] != "" && prev[cur] != source) cur = prev[cur];
            string nextHop = (prev[cur] == source) ? cur : destino;
            r.setRoute(destino, nextHop, dist[destino]);
        }
    }
}

void red::showRouterTable(const string& id) const {
    int idx = findRouterIndex(id);
    if (idx == -1) {
        cout << "Router " << id << " no existe.\n\n";
        return;
    }
    routers[idx].showTable();
}

void red::listRouters() const {
    cout << "Routers en la red:\n";
    for (auto &r : routers) cout << "- " << r.id << "\n";
    cout << endl;
}

pair<int, vector<string>> red::getCostAndPath(const string& origen, const string& destino) {
    pair<int, vector<string>> result;
    if (!adj.count(origen) || !adj.count(destino)) {
        result.first = -1;
        return result;
    }

    auto [dist, prev] = dijkstra(origen);
    if (dist[destino] == INT_MAX) {
        result.first = -1;
        return result;
    }

    vector<string> path;
    string cur = destino;
    while (cur != "") {
        path.push_back(cur);
        if (cur == origen) break;
        cur = prev[cur];
    }
    reverse(path.begin(), path.end());

    result.first = dist[destino];
    result.second = path;
    return result;
}

void red::addRouter(const string& id) {
    if (findRouterIndex(id) == -1) {
        routers.push_back(Router(id));
        if (!adj.count(id)) adj[id] = {};
        updateRoutingTables();
    }
}

void red::removeRouter(const string& id) {
    int idx = findRouterIndex(id);
    if (idx == -1) return;

    routers.erase(routers.begin() + idx);
    adj.erase(id);

    for (auto &p : adj) {
        auto &vec = p.second;
        vec.erase(remove_if(vec.begin(), vec.end(),
                            [&](const pair<string,int>& e){ return e.first == id; }), vec.end());
    }
    updateRoutingTables();
}

void red::addOrUpdateEdge(const string& a, const string& b, int cost) {
    auto removeEdge = [&](const string& x, const string& y) {
        auto &vec = adj[x];
        vec.erase(remove_if(vec.begin(), vec.end(),
                            [&](const pair<string,int>& e){ return e.first == y; }), vec.end());
    };
    removeEdge(a,b);
    removeEdge(b,a);
    addEdge(a,b,cost);
    updateRoutingTables();
}
