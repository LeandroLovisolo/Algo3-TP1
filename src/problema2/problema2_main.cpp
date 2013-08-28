#include <iostream>

#include "problema2.h"

using namespace std;

int main() {
    while(true) {
        vector<Curso> cursos;

        // Leo el tamaño del problema.
        int n;
        cin >> n;

        // Termino el ciclo si la lectura anterior fracasó.
        if(cin.eof()) break;

        // Leo n cursos.
        for(int i = 1; i <= n; i++) {
            int inicio, fin;
            cin >> inicio;
            cin >> fin;
            cursos.push_back(Curso(inicio, fin, i));
        }

        // Ignoro el resto de la línea (símbolo # y comentarios posteriores.)
        string s;
        getline(cin, s);

        // Resuelvo el problema
        vector<Curso> solucion = problema2(cursos);

        // Imprimo los índices de los cursos de la solución hallada.
        for(size_t i = 0; i < solucion.size(); i++)
            cout << indice(solucion[i]) << " ";
        cout << endl;
    }

    return 0;
}