#include "router.h"
using namespace std;

Router::Router() = default;
Router::Router(const string& _id) : id(_id) {}

void Router::clearTable() {
    routing_table.clear();
}

void Router::setRoute(const string& destino, const string& nextHop, int costo) {
    routing_table[destino] = {nextHop, costo};
}

void Router::showTable() const {
    cout << "Tabla de enrutamiento de " << id << ":\n";
    cout << left << setw(12) << "Destino" << setw(12) << "Siguiente" << setw(8) << "Costo" << "\n";
    cout << string(32, '-') << "\n";
    for (auto &p : routing_table) {
        cout << left << setw(12) << p.first
             << setw(12) << p.second.first
             << setw(8) << p.second.second << "\n";
    }
    cout << endl;
}
