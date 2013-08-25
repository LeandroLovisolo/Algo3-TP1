#include "problema3.h"

Piso::Piso(unsigned int filas, unsigned int columnas) {
	_filas = filas;
	_columnas = columnas;
	grilla = new Casilla[filas * columnas];
	for(unsigned int i = 0; i < filas * columnas; i++) grilla[i] = Libre;
}

Piso::Piso(const Piso &otro) {
	_filas = otro._filas;
	/*
	_columnas = otro._columnas;
	grilla = new Casilla[_filas * _columnas];
	for(unsigned int i = 0; i < _filas * _columnas; i++) grilla[i] = otro.grilla[i];
	*/
}

Piso::~Piso() {
	delete grilla;
}

Casilla& Piso::en(unsigned int fila, unsigned int columna) {
	return grilla[_columnas * (fila) + columna];
}



//Checkea si el casillero tiene un valor que podría llegar a ser una solución
bool casilleroCorrecto(Piso &piso, const unsigned int fila, const unsigned int columna) {
	/* sensoresAreaHorizontal cuenta la cantidad de sensores en el área horizontal (fila)
		y sensoresApuntandoHorizontal cuenta la cantidad de sensores apuntando a la casilla
		horizontalmente
	*/
	int sensoresAreaHorizontal = 0, sensoresAreaVertical = 0;
	int sensoresApuntandoHorizontal = 0, sensoresApuntandoVertical = 0;

	/*Separo el recorrido en izquierda, derecha, arriba y abajo
	de la celda para poder cortar la búsqueda al toparse con una pared (esto está hecho)
	para poder seguir bien la trayectoria de los láseres. */

	//Debajo de la posicion
	for(unsigned int i = fila+1; i < piso.filas(); ++i) {
		if(piso.en(i, columna) == Pared) break;
		switch(piso.en(i,columna)) {
			case SensorDobleVertical:
				sensoresApuntandoVertical++;
				break;
			case SensorCuadruple:
				sensoresApuntandoVertical++;
				break;
			case SensorDobleHorizontal:
				sensoresAreaVertical++;
				break;
			default:
				break;
		}
	}
	//Por arriba de la posicion
	for(unsigned int i = fila-1; i > 0; --i) {
		if(piso.en(i, columna) == Pared) break;
		switch(piso.en(i,columna)) {
			case SensorDobleVertical:
				sensoresApuntandoVertical++;
				break;
			case SensorCuadruple:
				sensoresApuntandoVertical++;
				break;
			case SensorDobleHorizontal:
				sensoresAreaVertical++;
				break;
			default:
				break;
		}
	}

	//Moviendome por las columnas
	//Por la derecha de la posición
	for(unsigned int j = columna+1; j < piso.columnas(); j++) {
		if(piso.en(fila, j) == Pared) break;
		switch(piso.en(fila,j)) {
			case SensorDobleVertical:
				sensoresAreaHorizontal++;
				break;
			case SensorCuadruple:
				sensoresApuntandoHorizontal++;
				break;
			case SensorDobleHorizontal:
				sensoresApuntandoHorizontal++;
				break;
			default:
				break;
		}
	}
	//Por la izquierda de la posición
	for(unsigned int j = columna-1; j > 0; j--) {
		if(piso.en(fila, j) == Pared) break;
		switch(piso.en(fila,j)) {
			case SensorDobleVertical:
				sensoresAreaHorizontal++;
				break;
			case SensorCuadruple:
				sensoresApuntandoHorizontal++;
				break;
			case SensorDobleHorizontal:
				sensoresApuntandoHorizontal++;
				break;
			default:
				break;
		}
	}
	
	//Checkeo las reglas que debería cumplir cada tipo de celda
	switch(piso.en(fila,columna)) {
		case Libre:
			//Si hay al menos un sensor vertical y uno horizontal y no apuntan, no es sol
			if(sensoresAreaHorizontal >= 1 && sensoresAreaVertical >= 1) return false;
			break;
		case SensorDobleHorizontal:
			if(sensoresAreaHorizontal >= 1 || sensoresApuntandoHorizontal >= 1) return false;
			if(sensoresApuntandoVertical >= 1) return false;
			break;
		case SensorDobleVertical:
			if(sensoresAreaVertical >= 1 || sensoresApuntandoVertical >= 1) return false;
			if(sensoresApuntandoHorizontal >= 1) return false;
			break;
		case SensorCuadruple:
			if(sensoresApuntandoVertical >= 1 || sensoresAreaVertical >= 1) return false;
			if(sensoresApuntandoHorizontal >= 1 || sensoresAreaHorizontal >= 1) return false;
			break;
		case Importante:
			//Los sensores en el área vertical y horizontal no apuntan al sensor importante
			//Aclaración, hay al menos 1 sensor horizontal en la columna o en la fila 1 vertical
			if(sensoresAreaHorizontal >= 1 || sensoresAreaVertical >= 1) return false;
			break;
		default:
			break;
	}
	return true;
}

bool checkPiso(Piso &piso) {
	for(unsigned int j=0;j<piso.filas();j++) {
		for(unsigned int i=0;i<piso.filas();i++) {
			if(!casilleroCorrecto(piso, i, j)) return false;
		}
	}
	return true;
} 


Piso *problema3(const Piso &piso) {

	return new Piso(piso);
}