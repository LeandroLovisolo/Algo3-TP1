#include "problema1.h"
#include <queue>          // std::priority_queue
#include <vector>         // std::vector
#include <functional>     // std::greater
#include <utility>
#include <algorithm>
#include <iostream>
using namespace std;

//Clase de comparación utilizada para ordenar los elementos de la priority_queue
class comparacion {
public:
  bool operator() (const pair<int,int> &x, const pair<int,int> &y) const {
    return (x.second > y.second);
  }
};

//Función utilizada para ordenar los camiones por número
bool ordenarCamiones(pair<int,int> x, pair<int,int> y){
    return(x.first<y.first);
}

vector<pair<int, int> >* problema1(const int capacidad_camiones, const vector<int> &paquetes) {
	priority_queue < pair<int, int>, vector<pair<int, int> >, comparacion > cola_camiones;
	for(unsigned int i=0;i<paquetes.size();i++) {
        //Es el primer paquete? 
		if(cola_camiones.size() == 0) {
        	cola_camiones.push(pair<int,int>(1, paquetes[i]));
		}
        //Entra el paquete en el camión de menor peso disponible?
		else if((cola_camiones.top().second + paquetes[i]) <= capacidad_camiones) {
			pair <int,int> camion_modificado = cola_camiones.top();
			camion_modificado.second += paquetes[i];
			cola_camiones.pop();
			cola_camiones.push(camion_modificado);
		}
        //Se crea un nuevo camión con el paquete
		else {
			cola_camiones.push(make_pair(cola_camiones.size()+1, paquetes[i]));
		}
	}
	//Ordena los camiones por su número (camión 1, camión 2, etc), para tener en la salida ordenada
    vector<pair<int, int> >* camiones = new vector<pair<int, int> >(cola_camiones.size(),make_pair(0,0));
    unsigned int size = cola_camiones.size();
    for(unsigned int j=0;j<size;j++) {
        (*camiones)[j] = cola_camiones.top();
        cola_camiones.pop();
    }
    sort(camiones->begin(),camiones->end(),ordenarCamiones);
    return camiones;
}