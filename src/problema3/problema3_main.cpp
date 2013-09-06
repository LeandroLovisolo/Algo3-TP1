#include <iostream>
#include "problema3.h"
using namespace std;

int main() {
	int n;
	if(cin.eof()) {
		cout << "Error en el formato de entrada" << endl;
		return 0;
	}
	cin >> n;
	int m;
	if(cin.eof()) {
		cout << "Error en el formato de entrada" << endl;
		return 0;
	}
	cin >> m;
	int casilla;
	Piso piso(n,m);
	for (int i = 0; i < n; ++i) {
		for(int j = 0; j < m; ++j) {
			if(!cin.eof()) {
				cin >> casilla;
				piso.set(i, j, (Casilla) casilla);
			}
		}
	}
	Piso solucion = problema3(piso);
	//Veo si hay una solución
	if(solucion.filas() != 0 && solucion.columnas() != 0) {
		vector<laser> lasersUtilizados = solucion.getLasers();
		cout << lasersUtilizados.size() << " " << solucion.costo() << endl;
		for(unsigned i = 0; i < lasersUtilizados.size(); ++i) {
			switch(lasersUtilizados[i]._tipo) {
				case SensorCuadruple:
					cout << 1;
					break;
				case SensorHorizontal:
					cout << 2;
					break;
				case SensorVertical:
					cout << 3;
					break;
				default:
					break;
			}
			cout << " " << lasersUtilizados[i]._fila + 1;
			cout << " " << lasersUtilizados[i]._columna + 1 << endl;
		}
	}
	else {
		cout << -1 << endl;
	}
	return 0;
}