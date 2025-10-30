#include <iostream>
#include "red.h"
#include <string> // Aseguramos la inclusión de string
#include <sstream> // Para manejo de entrada de comandos
using namespace std;

int main(int argc, char** argv) {
    // Optimización de I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    red net;
    string filename;

    // 1. Carga del archivo de topología
    if (argc >= 2) {
        filename = argv[1];
    } else {
        cout << "C:\Users\JuanManuel\Downloads\Informatica\Proyecto 4\Desafio_4\build\Desktop_Qt_6_9_2_MinGW_64_bit-Debug";
    }

    // Intenta cargar el archivo
    if (!net.loadFromFile(filename)) {
        cerr << "Error: No se pudo cargar la topología desde " << filename << ". Terminando el programa.\n";
        return 1;
    }

    cout << "Topología cargada correctamente desde " << filename << ".\n\n";

    // 2. Listado de comandos
    cout << "=================================================\n";
    cout << "Comandos disponibles:\n";
    cout << "  list                             - Listar routers\n";
    cout << "  table <R>                        - Mostrar tabla de enrutamiento de R\n";
    cout << "  cost <ORIG> <DEST>               - Mostrar costo solo\n";
    cout << "  path <ORIG> <DEST>               - Mostrar camino y costo\n";
    cout << "  addrouter <R>                    - Agregar router (sin enlaces)\n";
    cout << "  removerouter <R>                 - Eliminar router\n";
    cout << "  addedge <A> <B> <cost>           - Agregar o actualizar enlace\n";
    cout << "  exit                             - Salir\n";
    cout << "=================================================\n\n";

    // 3. Bucle de comandos interactivo
    string cmd;
    while (true) {
        cout << "> ";
        // Leemos el comando principal
        if (!(cin >> cmd)) break;

        if (cmd == "list") {
            net.listRouters();
        } else if (cmd == "table") {
            string r;
            if (!(cin >> r)) break;
            net.showRouterTable(r);
        } else if (cmd == "cost" || cmd == "path") {
            string o,d;
            if (!(cin >> o >> d)) break;
            auto res = net.getCostAndPath(o,d);

            if (res.first < 0) {
                cout << "No se puede alcanzar o el router no existe.\n\n";
            } else if (cmd == "cost") {
                cout << "El costo mínimo de " << o << " a " << d << " = " << res.first << "\n\n";
            } else { // path
                cout << "El camino de " << o << " a " << d << ": ";
                for (size_t i=0;i<res.second.size();++i) {
                    if (i) cout << " -> ";
                    cout << res.second[i];
                }
                cout << " tiene un costo de = " << res.first << "\n\n";
            }
        } else if (cmd == "addrouter") {
            string r;
            if (!(cin >> r)) break;
            net.addRouter(r);
            cout << "El router " << r << " se agregó.\n\n";
        } else if (cmd == "removerouter") {
            string r;
            if (!(cin >> r)) break;
            net.removeRouter(r);
            cout << "El router " << r << " se ha removido.\n\n";
        } else if (cmd == "addedge") {
            string a,b; int c;
            if (!(cin >> a >> b >> c)) break;
            net.addOrUpdateEdge(a,b,c);
            cout << "Enlace " << a << "-" << b << " se agregó o actualizó con costo " << c << ".\n\n";
        } else if (cmd == "exit") {
            cout << "Saliendo del programa...\n";
            break;
        } else {
            cout << "Comando no reconocido. Escriba 'list' para ver los comandos.\n\n";
        }
    }

    return 0;
}
