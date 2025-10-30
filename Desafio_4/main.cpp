#include <iostream>
#include "red.h"
#include <cstdlib>  // Para system("pause")
using namespace std;

int main() {
    red miRed;

    // Ruta fija al archivo topologia.txt
    string rutaArchivo = "C:\\Users\\JuanManuel\\Downloads\\Informatica\\Proyecto 4\\Desafio_4\\build\\Desktop_Qt_6_9_2_MinGW_64_bit-Debug\\debug\\topologia.txt";

    cout << "Intentando cargar la topologia desde:\n" << rutaArchivo << "\n\n";

    // Intentar abrir el archivo
    if (!miRed.loadFromFile(rutaArchivo)) {
        cerr << "No se pudo abrir el archivo en la ruta fija.\n";
        cerr << "Verifica que el archivo exista, tenga permisos de lectura y que el nombre sea exactamente 'topologia.txt'.\n";
        system("pause");
        return 1;
    }

    cout << "Topologia cargada correctamente.\n\n";
    miRed.listRouters();

    string origen, destino;
    cout << "Ingrese router de origen: ";
    cin >> origen;
    cout << "Ingrese router de destino: ";
    cin >> destino;

    auto [costo, camino] = miRed.getCostAndPath(origen, destino);

    if (costo == -1) {
        cout << "No existe un camino entre " << origen << " y " << destino << ".\n";
    } else {
        cout << "Costo total desde " << origen << " hasta " << destino << ": " << costo << "\n";
        cout << "Camino: ";
        for (size_t i = 0; i < camino.size(); ++i) {
            cout << camino[i];
            if (i < camino.size() - 1) cout << " -> ";
        }
        cout << "\n";
    }

    cout << "\nPresione ENTER para salir...";
    cin.ignore();
    cin.get();
    system("pause");
    return 0;
}
