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

/* otra forma de resolver el ej1 con make_heap()

#include <iostream>
#include <fstream>
#include <vector>
#include<algorithm>

int limite;

using namespace std;

vector<int> leerDatos (istream& file) {
    int peso;
    int n;
    file >> limite;
    file >> n;
    vector<int> res = vector<int>();

    for (int i=0; i<n; i++) {
        file >> peso;
        res.push_back(peso);
    }

    return res;
}

bool ordenarCamiones(pair<int,int> x, pair<int,int> y){
    return(x.second<y.second);
}

int main()
{
    string archivo = "prueba1.txt";
    ifstream ifs (archivo.c_str(), ifstream::in);
    if (!ifs.is_open()) {
    cout << "no se pudo abrir el archivo " << archivo << endl;
    return 0;
    }
    vector<int> paquetes = leerDatos(ifs);
    vector<pair<int, int> > camiones = vector<pair<int, int> >();

    make_heap(camiones.begin(),camiones.end());

    for (int i = 0; i<paquetes.size(); i++) {
        if(camiones.size() == 0) {
            camiones.push_back(pair<int,int> (limite-paquetes[i],1));
        }
        else if(paquetes[i]<=camiones.front().first) {
            camiones.front().first -= paquetes[i];
        }
        else {
            camiones.push_back(pair<int,int> (limite - paquetes[i], camiones.size()+1));
            push_heap(camiones.begin(),camiones.end());
        }
    }

    cout << "cantidad de camiones " << camiones.size() << endl;

    sort(camiones.begin(),camiones.end(),ordenarCamiones);

    for (int i = 0; i<camiones.size(); i++){
        cout << limite - camiones[i].first << " ";
    }


}

*/