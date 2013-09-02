#ifndef PROBLEMA3_H
#define PROBLEMA3_H

enum Casilla { Libre, Pared, Importante, SensorVertical,
               SensorHorizontal, SensorCuadruple, Sensado };

class Piso {
public:
    Piso(unsigned filas, unsigned columnas);
    Piso(unsigned filas, unsigned columnas, std::string casillas);
    Piso(const Piso &otro);
    ~Piso();

    bool operator==(const Piso& otro) const;
    unsigned filas() const { return _filas; }
    unsigned columnas() const { return _columnas; }
    Casilla& en(unsigned fila, unsigned columna);
    Casilla en(unsigned fila, unsigned columna) const;
    void imprimir() const;

private:
    unsigned _filas;
    unsigned _columnas;
    Casilla *grilla;
};

Piso *problema3(Piso &piso);
bool checkPiso(const Piso &piso);

Piso resolver(const Piso& p);
int costo(const Piso& p);


#endif // PROBLEMA3_H