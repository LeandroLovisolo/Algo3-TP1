#ifndef PROBLEMA3_H
#define PROBLEMA3_H

enum Casilla { Libre, Pared, Importante, SensorVertical,
               SensorHorizontal, SensorCuadruple };

class Piso {
public:
    Piso(unsigned filas, unsigned columnas);
    Piso(const Piso &otro);
    ~Piso();

    unsigned filas() { return _filas; }
    unsigned columnas() { return _columnas; }
    Casilla& en(unsigned fila, unsigned columna);
    void imprimir();

private:
    unsigned _filas;
    unsigned _columnas;
    Casilla *grilla;
};

Piso *problema3(Piso &piso);
bool checkPiso(Piso &piso);

#endif // PROBLEMA3_H