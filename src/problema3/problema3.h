#ifndef PROBLEMA3_H
#define PROBLEMA3_H

#include <vector>

enum Casilla { Pared, Libre, Importante, SensorVertical,
               SensorHorizontal, SensorCuadruple, Sensado };

struct laser {
    unsigned _fila;
    unsigned _columna;
    Casilla _tipo;
    laser(unsigned fila, unsigned columna, Casilla tipo) {
        _fila = fila;
        _columna = columna;
        _tipo = tipo;
    }
};

class Piso {
public:
    Piso(unsigned filas, unsigned columnas);
    Piso(unsigned filas, unsigned columnas, std::string casillas);
    Piso(const Piso &otro);

    std::vector<laser> getLasers() const;
    bool operator==(const Piso& otro) const;
    unsigned filas() const { return _filas; }
    unsigned columnas() const { return _columnas; }
    Casilla get(unsigned fila, unsigned columna) const;
    void set (unsigned fila, unsigned columna, Casilla valor);
    unsigned costo() const;
    void imprimir() const;

private:
    unsigned _filas;
    unsigned _columnas;
    std::vector<Casilla> grilla;
    unsigned _costo;
};

Piso problema3(const Piso &piso);

#endif // PROBLEMA3_H