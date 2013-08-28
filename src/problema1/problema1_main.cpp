#include <iostream>
#include <vector>
#include "problema1.h"
#include <utility>

using namespace std;
int main() {
    while(true) {
        vector<int> paquetes;

        // Leo el límite de carga de los camiones.
        int limite;
        cin >> limite;

        // Termino el ciclo si la lectura anterior fracasó.
        if(cin.eof()) break;

        // Leo el tamaño del problema.
        int n;
        cin >> n;

        // Leo n paquetes.
        for(int i = 0; i < n; i++) {
            int paquete;
            cin >> paquete;
            paquetes.push_back(paquete);
        }

        // Ignoro el resto de la línea (símbolo # y comentarios posteriores.)
        string s;
        getline(cin, s);

        // Resuelvo el problema
        vector<int> solucion = problema1(limite, paquetes);

        // Imprimo la cantidad de camiones usados.
        cout << solucion.size() << " ";

        // Imprimo la carga de los camiones usados en el orden correspondiente.
        for(size_t i = 0; i < solucion.size(); i++) cout << solucion[i] << " ";

        cout << endl;
    }

    return 0;
}
