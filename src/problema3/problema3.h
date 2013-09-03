#ifndef PROBLEMA3_H
#define PROBLEMA3_H

#include <vector>

enum Casilla { Libre, Pared, Importante, SensorVertical,
               SensorHorizontal, SensorCuadruple, Sensado };

class Piso {
public:
    Piso(unsigned filas, unsigned columnas);
    Piso(unsigned filas, unsigned columnas, std::string casillas);
    Piso(const Piso &otro);

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

Piso *problema3(Piso &piso);
bool checkPiso(const Piso &piso);

Piso resolver(const Piso& p);
unsigned costo(const Piso& p);


#endif // PROBLEMA3_H