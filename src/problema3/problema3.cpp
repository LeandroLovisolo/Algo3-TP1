#include "problema3.h"

Piso::Piso(unsigned int filas, unsigned int columnas) {
	_filas = filas;
	_columnas = columnas;
	grilla = new Casilla[filas * columnas];
	for(unsigned int i = 0; i < filas * columnas; i++) grilla[i] = Libre;
}

Piso::Piso(const Piso &otro) {
	_filas = otro._filas;
	_columnas = otro._columnas;
	grilla = new Casilla[_filas * _columnas];
	for(unsigned int i = 0; i < _filas * _columnas; i++) grilla[i] = otro.grilla[i];
}

Piso::~Piso() {
	delete grilla;
}

Casilla& Piso::en(unsigned int fila, unsigned int columna) {
	return grilla[_columnas * (fila) + columna];
}

Piso *problema3(const Piso &piso) {
	return new Piso(piso);
}