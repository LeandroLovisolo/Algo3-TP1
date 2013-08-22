#include <iostream>
#include "problema2.h"
using namespace std;

int main() {
	//Main para la lectura
	int n;
	if(cin.eof()) {
		cout << "Error en el formato de entrada" << endl;
		return 0;
	}
	cin >> n;
	//Construyo un vector de pairs de n elementos en 0
	vector<Curso> cursos(n, Curso(0,0,0));
	int i=0;
	while(!cin.eof() && i < n) {
		//Cargo fecha de inicio y de finalización
		cin >> cursos[i]._inicio;
		cin >> cursos[i]._final;
		cursos[i]._numero = i+1;
		i++;
	}
	vector<Curso>* resultado = problema2(cursos);
	/* Funcion para ver si fueron agregados correctamente y ordenados para probar el sort
	for(unsigned int j=0;j<cursos.size();j++) {
		cout << cursos[j]._numero << ": [" << cursos[j]._inicio << "," << cursos[j]._final << "] ";
	}
	cout << endl;
	*/

	/* Imprimo el numero del curso
	for(i=0;i<resultado.size();i++) {
		cout << resuldato[i]._numero << " ";
	}
	cout << endl;
	*/
	delete resultado;
	return 0;
}