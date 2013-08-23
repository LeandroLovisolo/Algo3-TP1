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

	Piso mapa(n,m);
	for (int i = 0; i < n; ++i) {
		for(int j = 0; j < m; ++j) {
			if(!cin.eof())	cin >> mapa.en(i, j);
		}
	}
	return 0;
}