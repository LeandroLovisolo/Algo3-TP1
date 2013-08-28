#include <tuple>
#include <queue>
#include <algorithm>

#include "problema1.h"

using namespace std;

// Campos: <carga, índice>
typedef std::tuple<int, int> Camion;

// Macros para acceder a los campos de la tupla Camion
#define carga(c)  get<0>(c)
#define indice(c) get<1>(c)

// Relación de orden de la priority_queue de tuplas Camion
class orden {
public:
  bool operator() (const Camion &x, const Camion &y) const {
    return carga(x) > carga(y);
  }
};

vector<int> problema1(int limite, const vector<int> &paquetes) {
    priority_queue<Camion, vector<Camion>, orden> cola;

    for(size_t i = 0; i < paquetes.size(); i++) {
        // Primer camión.
        if(cola.empty()) cola.push(Camion(paquetes[i], 1));

        // Se carga el paquete en el camión menos cargado.
        else if(carga(cola.top()) + paquetes[i] <= limite) {
            Camion c = cola.top();
            carga(c) += paquetes[i];
            cola.pop();
            cola.push(c);
        }

        // Se carga el paquete en un nuevo camión.
        else cola.push(Camion(paquetes[i], cola.size() + 1));
    }

    // Convertimos la cola a un vector de camiones.
    vector<Camion> camiones;
    while(!cola.empty()) {
        camiones.push_back(cola.top());
        cola.pop();
    }

    // Ordenamos el vector de camiones por índice.
    sort(camiones.begin(),
         camiones.end(),
         [] (Camion c, Camion d) { return indice(c) < indice(d); } );

    // Convertimos el vector de camiones a un vector de cargas.
    vector<int> cargas;
    for(size_t i = 0; i < camiones.size(); i++)
        cargas.push_back(carga(camiones[i]));

    return cargas;
}