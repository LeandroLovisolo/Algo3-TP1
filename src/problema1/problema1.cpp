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