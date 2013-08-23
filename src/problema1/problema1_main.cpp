#include <iostream>
#include <vector>
#include "problema1.h"
#include <utility>

using namespace std;
int main() {
	//Main para la lectura
	int n, limite;
	if(cin.eof()) {
		cout << "Error en el formato de entrada" << endl;
		return 0;
	}
	cin >> limite;
	if(cin.eof()) {
		cout << "Error en el formato de entrada" << endl;
		return 0;
	}
	cin >> n;
	//Construyo un vector de n elementos en 0
	vector<int> paquetes(n, 0);
	int i=0;
	while(!cin.eof() && i < n) {
		cin >> paquetes[i];
		i++;
	}
	vector<pair<int, int> >* camiones = problema1(limite, paquetes);
	cout << camiones->size() << " ";
	for(unsigned j=0;j<camiones->size();j++) {
		cout << (*camiones)[j].second << " ";
	}
	cout << endl;
	return 0;
}