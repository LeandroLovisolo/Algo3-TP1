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
	Piso mapa(n,m);
	for (int i = 0; i < n; ++i) {
		for(int j = 0; j < m; ++j) {
			if(!cin.eof()) {
				cin >> casilla;
				mapa.set(i, j, (Casilla) casilla);
			}
		}
	}
	Piso* test = problema3(mapa);
	delete test;
	/* Test de lenctura
	for (int i = 0; i < n; ++i) {
		for(int j = 0; j < m; ++j) {
			cout << mapa.get(i,j);
		}
		cout << endl;
	}
	*/
	return 0;
}