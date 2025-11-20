#include <iostream>
#include <string>
#include "network.h"

int main() {
    Network net;

    // 1. Pedir ruta al usuario
    std::string rutaArchivo;
    std::cout << "Ingrese la ruta del archivo de rutas (por ejemplo: C:/Users/.../rutas.txt):\n> ";
    std::getline(std::cin, rutaArchivo);

    // 2. Intentar cargar archivo
    if (!net.loadFromFile(rutaArchivo)) {
        std::cout << "\nERROR: No se pudo abrir el archivo.\n";
        return 1;
    }

    std::cout << "\nArchivo cargado correctamente.\n\n";

    // Mostrar topología
    net.printNetwork();

    // Calcular tablas
    net.computeAllTables();

    // Mostrar tablas de enrutamiento
    std::cout << "\nTablas de enrutamiento:\n";
    net.printAllRoutingTables();

    // Ejemplo: Obtener camino de un nodo a otro
    std::string from = "A", to = "C";
    auto path = net.getShortestPath(from, to);
    if (path.empty()) {
        std::cout << "\nNo hay camino entre " << from << " y " << to << "\n";
    } else {
        std::cout << "\nCamino más corto de " << from << " a " << to << ": ";
        for (size_t i = 0; i < path.size(); i++) {
            std::cout << path[i] << (i + 1 < path.size() ? " -> " : "");
        }
        std::cout << "\n";
    }

    return 0;
}
