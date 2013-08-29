#include <iostream>

#include "problema3.h"

using namespace std;

Piso::Piso(unsigned filas, unsigned columnas) {
    _filas = filas;
    _columnas = columnas;
    grilla = new Casilla[filas * columnas];
    for(unsigned i = 0; i < filas * columnas; i++) grilla[i] = Libre;
}

Piso::Piso(const Piso &otro) {
    _filas = otro._filas;
    /*
    _columnas = otro._columnas;
    grilla = new Casilla[_filas * _columnas];
    for(unsigned i = 0; i < _filas * _columnas; i++) grilla[i] = otro.grilla[i];
    */
}

Piso::~Piso() {
    delete grilla;
}

Casilla& Piso::en(unsigned fila, unsigned columna) {
    return grilla[_columnas * (fila) + columna];
}

void Piso::imprimir() {
    // Límite superior
    cout << "\u2554";
    for(unsigned i = 0; i < _columnas; i++) cout << "\u2550";
    cout << "\u2557" << endl;

    for(unsigned i = 0; i < _filas; i++) {
        // Límite izquierdo
        cout << "\u2551";

        for(unsigned j = 0; j < _columnas; j++) {
            switch(en(i, j)) {
                case Libre:            cout << " "; break;
                case Pared:            cout << "#"; break;
                case Importante:       cout << "*"; break;
                case SensorVertical:   cout << "|"; break;
                case SensorHorizontal: cout << "-"; break;
                case SensorCuadruple:  cout << "+"; break;
            }
        }

        // Límite derecho
        cout << "\u2551" << endl;
    }

    // Límite inferior
    cout << "\u255a";
    for(unsigned i = 0; i < _columnas; i++) cout << "\u2550";
    cout << "\u255d" << endl;    
}

//Checkea si el casillero tiene un valor que podría llegar a ser una solución
//Complejidad O(n + m) n filas, m columnas
bool casilleroCorrecto(Piso &piso, const unsigned fila, const unsigned columna) {
    /* sensoresAreaHorizontal cuenta la cantidad de sensores en el área horizontal (fila)
        y sensoresApuntandoHorizontal cuenta la cantidad de sensores apuntando a la casilla
        horizontalmente
    */
    //Si es una pared, no hay nada para hacer
    if(piso.en(fila, columna) == Pared) return true;
    int sensoresAreaHorizontal = 0, sensoresAreaVertical = 0;
    int sensoresApuntandoHorizontal = 0, sensoresApuntandoVertical = 0;

    /*Separo el recorrido en izquierda, derecha, arriba y abajo
    de la celda para poder cortar la búsqueda al toparse con una pared (esto está hecho)
    para poder seguir bien la trayectoria de los láseres. */

    //Los dos fors siguientes tienen complejidad O(n) con n como filas
    //Debajo de la posicion
    for(unsigned i = fila+1; i < piso.filas(); ++i) {
        if(piso.en(i, columna) == Pared) break;
        switch(piso.en(i,columna)) {
            case SensorVertical:
                sensoresApuntandoVertical++;
                break;
            case SensorCuadruple:
                sensoresApuntandoVertical++;
                break;
            case SensorHorizontal:
                sensoresAreaVertical++;
                break;
            default:
                break;
        }
    }
    //Por arriba de la posicion
    for(int i = fila-1; i >= 0; --i) {
        if(piso.en(i, columna) == Pared) break;
        switch(piso.en(i,columna)) {
            case SensorVertical:
                sensoresApuntandoVertical++;
                break;
            case SensorCuadruple:
                sensoresApuntandoVertical++;
                break;
            case SensorHorizontal:
                sensoresAreaVertical++;
                break;
            default:
                break;
        }
    }

    //Los dos fors siguientes tienen complejidad O(m) con m columnas

    //Moviendome por las columnas
    //Por la derecha de la posición
    for(unsigned j = columna+1; j < piso.columnas(); j++) {
        if(piso.en(fila, j) == Pared) break;
        switch(piso.en(fila,j)) {
            case SensorVertical:
                sensoresAreaHorizontal++;
                break;
            case SensorCuadruple:
                sensoresApuntandoHorizontal++;
                break;
            case SensorHorizontal:
                sensoresApuntandoHorizontal++;
                break;
            default:
                break;
        }
    }
    //Por la izquierda de la posición
    for(int j = columna-1; j >= 0; j--) {
        if(piso.en(fila, j) == Pared) break;
        switch(piso.en(fila,j)) {
            case SensorVertical:
                sensoresAreaHorizontal++;
                break;
            case SensorCuadruple:
                sensoresApuntandoHorizontal++;
                break;
            case SensorHorizontal:
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
            if(sensoresApuntandoHorizontal >= 1 || sensoresApuntandoVertical >= 1) return true;
            if(sensoresAreaVertical == 0) {
                //Complejidad O(n) para los dos fors
                //En este caso checkeo también la posición dónde estoy
                for(unsigned i = fila; i < piso.filas(); ++i) {
                    if(piso.en(i, columna) == Pared) break;
                    if(piso.en(i, columna) == Importante) continue;
                    //Veo si entra un sensor vertical que lo apunte
                    piso.en(i, columna) = SensorVertical;
                    if(casilleroCorrecto(piso, i, columna)) {
                        piso.en(i, columna) = Libre;
                        return true;
                    }
                }
                for(int i = fila-1; i >= 0; --i) {
                    if(piso.en(i, columna) == Pared) break;
                    if(piso.en(i, columna) == Importante) continue;
                    //Veo si entra un sensor vertical que lo apunte
                    piso.en(i, columna) = SensorVertical;
                    if(casilleroCorrecto(piso, i, columna)) {
                        piso.en(i, columna) = Libre;
                        return true;
                    }
                }
            }
            if(sensoresAreaHorizontal == 0) {
                //En este caso checkeo también la posición dónde estoy
                //Complejidad O(m) para los dos fors
                for(unsigned j = columna; j < piso.columnas(); j++) {
                    if(piso.en(fila, j) == Pared) break;
                    if(piso.en(fila, j) == Importante) continue;
                    //Veo si entra un sensor horizontal que lo apunte
                    piso.en(fila, j) = SensorHorizontal;
                    if(casilleroCorrecto(piso, fila, j)) {
                        piso.en(fila, j) = Libre;
                        return true;
                    }
                }
                for(int j = columna-1; j >= 0; j--) {
                    if(piso.en(fila, j) == Pared) break;
                    if(piso.en(fila, j) == Importante) continue;
                    //Veo si entra un sensor horizontal que lo apunte
                    piso.en(fila, j) = SensorHorizontal;
                    if(casilleroCorrecto(piso, fila, j)) {
                        piso.en(fila, j) = Libre;
                        return true;
                    }
                }
            }
            return false;
            break;
        case SensorHorizontal:
            if(sensoresAreaHorizontal >= 1 || sensoresApuntandoHorizontal >= 1) return false;
            if(sensoresApuntandoVertical >= 1) return false;
            break;
        case SensorVertical:
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
            if(sensoresApuntandoHorizontal >= 1 && sensoresApuntandoVertical >= 1) return true;
            //Complejidad O(n) para los dos fors
            //Si esto no se da, hay que checkear si es posible cubrirlo con las casillas libres que queden
            if(sensoresApuntandoVertical == 0) {
                for(unsigned i = fila+1; i < piso.filas(); ++i) {
                    if(piso.en(i, columna) == Pared) break;
                    if(piso.en(i, columna) == Importante) continue;
                    //Veo si entra un sensor vertical que lo apunte
                    piso.en(i, columna) = SensorVertical;
                    if(casilleroCorrecto(piso, i, columna)) {
                        piso.en(i, columna) = Libre;
                        sensoresApuntandoVertical++;
                        break;
                    }

                }
                //Por arriba de la posicion
                //Me fijo si ya encontró un espacio libre antes
                if(sensoresApuntandoVertical == 0) {
                    for(int i = fila-1; i >= 0; --i) {
                        if(piso.en(i, columna) == Pared) break;
                        if(piso.en(i, columna) == Importante) continue;
                        //Veo si entra un sensor vertical que lo apunte
                        piso.en(i, columna) = SensorVertical;
                        if(casilleroCorrecto(piso, i, columna)) {
                            piso.en(i, columna) = Libre;
                            sensoresApuntandoVertical++;
                            break;
                        }
                    }
                    //Termina el for y no encontré ningún lugar libre, falso
                    if(sensoresApuntandoVertical == 0) return false;
                }
            }
            //Complejidad O(m) para los dos fors
            //Moviendome por las columnas
            //Por la derecha de la posición
            if(sensoresApuntandoHorizontal == 0)  {
                for(unsigned j = columna+1; j < piso.columnas(); j++) {
                    if(piso.en(fila, j) == Pared) break;
                    if(piso.en(fila, j) == Importante) continue;
                    //Veo si entra un sensor horizontal que lo apunte
                    piso.en(fila, j) = SensorHorizontal;
                    if(casilleroCorrecto(piso, fila, j)) {
                        piso.en(fila, j) = Libre;
                        sensoresApuntandoHorizontal++;
                        break;
                    }
                }
                //Por la izquierda de la posición
                if(sensoresApuntandoHorizontal == 0) {
                    for(int j = columna-1; j >= 0; j--) {
                        if(piso.en(fila, j) == Pared) break;
                        if(piso.en(fila, j) == Importante) continue;
                        //Veo si entra un sensor horizontal que lo apunte
                        piso.en(fila, j) = SensorHorizontal;
                        if(casilleroCorrecto(piso, fila, j)) {
                            piso.en(fila, j) = Libre;
                            sensoresApuntandoHorizontal++;
                            break;
                        }
                    }
                    //Termina el for y no encontré ningún lugar libre, falso
                    if(sensoresApuntandoHorizontal == 0) return false;
                }
            }
            //Si no cayó antes en un caso verdadero, devuelve falso
            return true;
            break;
        default:
            break;
    }
    return true;
}

//Complejidad O(n*m(n+m))
bool checkPiso(Piso &piso) {
    for(unsigned j=0;j<piso.filas();j++) {
        for(unsigned i=0;i<piso.columnas();i++) {
            if(!casilleroCorrecto(piso, j, i)) return false;
        }
    }
    return true;
}

Piso *problema3(Piso &piso) {
    if(checkPiso(piso)) {
        cout << "Resulto valido" << endl;
    }
    else {
        cout << "Resulto invalido" << endl;
    }
    return new Piso(piso);
}