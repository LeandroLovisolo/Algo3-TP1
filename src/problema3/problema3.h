#ifndef PROBLEMA3_H
#define PROBLEMA3_H

enum Casilla { Pared, Libre, Importante, SensorDobleVertical, SensorDobleHorizontal, SensorCuadruple };

class Piso {
public:
	Piso(unsigned int filas, unsigned int columnas);
	Piso(const Piso &otro);
	~Piso();

	unsigned int filas() { return _filas; }
	unsigned int columnas() { return _columnas; }
	Casilla& en(unsigned int fila, unsigned int columna);

private:
	unsigned int _filas;
	unsigned int _columnas;
	Casilla *grilla;
};

Piso *problema3(Piso &piso);
bool checkPiso(Piso &piso);

#endif // PROBLEMA3_H