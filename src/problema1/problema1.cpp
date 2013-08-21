#include "problema1.h"
#include <queue>          // std::priority_queue
#include <vector>         // std::vector
#include <functional>     // std::greater
#include <utility>

using namespace std;

int problema1(int capacidad_camiones, const std::vector<int> &paquetes) {
	priority_queue < pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > cola_camiones;
	for(unsigned int i=0;i<paquetes.size();i++) {
		if(cola_camiones.size() == 0) {
			cola_camiones.push(pair<int,int>(paquetes[i],1));
		}
		else if((cola_camiones.top().first + paquetes[i]) <= capacidad_camiones) {
			pair <int,int> camion_modificado = cola_camiones.top();
			camion_modificado.first += paquetes[i];
			cola_camiones.pop();
			cola_camiones.push(camion_modificado);
		}
		else {
			cola_camiones.push(pair<int,int>(paquetes[i], cola_camiones.size()+1));
		}
	}
	//Ordenar
	//Imprimir resultados va en main?
	//devolver un vector con los valores adecuados en caso de que no
	//Falta testear bien

	return 0;
}