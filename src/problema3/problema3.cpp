#include <iostream>
#include <vector>
#include <climits>
#include <omp.h>

#include "problema3.h"

using namespace std;

Piso::Piso(unsigned filas, unsigned columnas)
        : grilla(filas * columnas, Libre) {
    _filas = filas;
    _columnas = columnas;
    _costo = 0;
}

Piso::Piso(unsigned filas, unsigned columnas, string casillas)
        : Piso(filas, columnas) {
    for(unsigned i = 0; i < filas; i++) {
        for(unsigned j = 0; j < columnas; j++) {
            switch(casillas[columnas * i + j]) {
                case '#': set(i, j, Pared);            break;
                case '*': set(i, j, Importante);       break;
                case '|': set(i, j, SensorVertical);   break;
                case '-': set(i, j, SensorHorizontal); break;
                case '+': set(i, j, SensorCuadruple);  break;
            }
        }
    }
}

Piso::Piso(const Piso &otro)
        : grilla(otro.grilla) {
    _filas = otro._filas;
    _columnas = otro._columnas;
    _costo = otro._costo;
}

bool Piso::operator==(const Piso& otro) const {
    if(_filas != otro._filas || _columnas != otro._columnas) return false;
    for(unsigned i = 0; i < _filas * _columnas; i++)
        if(grilla[i] != otro.grilla[i]) return false;
    return true;
}

Casilla Piso::get(unsigned fila, unsigned columna) const {
    if(fila >= _filas || columna >= _columnas) {
        cout << "Acceso fuera de rango: (" << fila << ", " << columna << ")." << endl;
        exit(-1);
    }

    return grilla[_columnas * (fila) + columna];
}

void Piso::set(unsigned fila, unsigned columna, Casilla valor) {
    if(fila >= _filas || columna >= _columnas) {
        cout << "Acceso fuera de rango: (" << fila << ", " << columna << ")." << endl;;
        exit(-1);
    }

    // Restamos el costo original de la posición.
    switch(get(fila, columna)) {
        case SensorVertical:
        case SensorHorizontal:
        _costo -= 4000;
        break;

        case SensorCuadruple:
        _costo -= 6000;
        break;

        default: break;        
    }

    // Asignamos el valor de la posición.
    grilla[_columnas * (fila) + columna] = valor;

    // Sumamos su costo.
    switch(valor) {
        case SensorVertical:
        case SensorHorizontal:
        _costo += 4000;
        break;

        case SensorCuadruple:
        _costo += 6000;
        break;

        default: break;        
    }
}

unsigned Piso::costo() const {
    return _costo;
}

void Piso::imprimir() const {
    // Límite superior
    cout << "\u2554";
    for(unsigned i = 0; i < _columnas; i++) cout << "\u2550";
    cout << "\u2557" << endl;

    for(unsigned i = 0; i < _filas; i++) {
        // Límite izquierdo
        cout << "\u2551";

        for(unsigned j = 0; j < _columnas; j++) {
            switch(get(i, j)) {
                case Libre:            cout << " "; break;
                case Pared:            cout << "\u2588"; break;
                case Importante:       cout << "*"; break;
                case SensorVertical:   cout << "|"; break;
                case SensorHorizontal: cout << "-"; break;
                case SensorCuadruple:  cout << "+"; break;
                case Sensado:          cout << "·"; break;
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
    if(piso.get(fila, columna) == Pared) return true;
    int sensoresAreaHorizontal = 0, sensoresAreaVertical = 0;
    int sensoresApuntandoHorizontal = 0, sensoresApuntandoVertical = 0;

    /*Separo el recorrido en izquierda, derecha, arriba y abajo
    de la celda para poder cortar la búsqueda al toparse con una pared (esto está hecho)
    para poder seguir bien la trayectoria de los láseres. */

    //Los dos fors siguientes tienen complejidad O(n) con n como filas
    //Debajo de la posicion
    for(unsigned i = fila+1; i < piso.filas(); ++i) {
        if(piso.get(i, columna) == Pared) break;
        switch(piso.get(i,columna)) {
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
        if(piso.get(i, columna) == Pared) break;
        switch(piso.get(i,columna)) {
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
        if(piso.get(fila, j) == Pared) break;
        switch(piso.get(fila,j)) {
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
        if(piso.get(fila, j) == Pared) break;
        switch(piso.get(fila,j)) {
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
    switch(piso.get(fila,columna)) {
        case Libre:
            //Si hay al menos un sensor vertical y uno horizontal y no apuntan, no es sol
            if(sensoresAreaHorizontal >= 1 && sensoresAreaVertical >= 1) return false;
            if(sensoresApuntandoHorizontal >= 1 || sensoresApuntandoVertical >= 1) return true;
            if(sensoresAreaVertical == 0) {
                //Complejidad O(n) para los dos fors
                //En este caso checkeo también la posición dónde estoy
                for(unsigned i = fila; i < piso.filas(); ++i) {
                    if(piso.get(i, columna) == Pared) break;
                    if(piso.get(i, columna) == Importante) continue;
                    //Veo si entra un sensor vertical que lo apunte
                    piso.set(i, columna, SensorVertical);
                    if(casilleroCorrecto(piso, i, columna)) {
                        piso.set(i, columna, Libre);
                        return true;
                    }
                }
                for(int i = fila-1; i >= 0; --i) {
                    if(piso.get(i, columna) == Pared) break;
                    if(piso.get(i, columna) == Importante) continue;
                    //Veo si entra un sensor vertical que lo apunte
                    piso.set(i, columna, SensorVertical);
                    if(casilleroCorrecto(piso, i, columna)) {
                        piso.set(i, columna, Libre);
                        return true;
                    }
                }
            }
            if(sensoresAreaHorizontal == 0) {
                //En este caso checkeo también la posición dónde estoy
                //Complejidad O(m) para los dos fors
                for(unsigned j = columna; j < piso.columnas(); j++) {
                    if(piso.get(fila, j) == Pared) break;
                    if(piso.get(fila, j) == Importante) continue;
                    //Veo si entra un sensor horizontal que lo apunte
                    piso.set(fila, j, SensorHorizontal);
                    if(casilleroCorrecto(piso, fila, j)) {
                        piso.set(fila, j, Libre);
                        return true;
                    }
                }
                for(int j = columna-1; j >= 0; j--) {
                    if(piso.get(fila, j) == Pared) break;
                    if(piso.get(fila, j) == Importante) continue;
                    //Veo si entra un sensor horizontal que lo apunte
                    piso.set(fila, j, SensorHorizontal);
                    if(casilleroCorrecto(piso, fila, j)) {
                        piso.set(fila, j, Libre);
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
                    if(piso.get(i, columna) == Pared) break;
                    if(piso.get(i, columna) == Importante) continue;
                    //Veo si entra un sensor vertical que lo apunte
                    piso.set(i, columna, SensorVertical);
                    if(casilleroCorrecto(piso, i, columna)) {
                        piso.set(i, columna, Libre);
                        sensoresApuntandoVertical++;
                        break;
                    }

                }
                //Por arriba de la posicion
                //Me fijo si ya encontró un espacio libre antes
                if(sensoresApuntandoVertical == 0) {
                    for(int i = fila-1; i >= 0; --i) {
                        if(piso.get(i, columna) == Pared) break;
                        if(piso.get(i, columna) == Importante) continue;
                        //Veo si entra un sensor vertical que lo apunte
                        piso.set(i, columna, SensorVertical);
                        if(casilleroCorrecto(piso, i, columna)) {
                            piso.set(i, columna, Libre);
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
                    if(piso.get(fila, j) == Pared) break;
                    if(piso.get(fila, j) == Importante) continue;
                    //Veo si entra un sensor horizontal que lo apunte
                    piso.set(fila, j, SensorHorizontal);
                    if(casilleroCorrecto(piso, fila, j)) {
                        piso.set(fila, j, Libre);
                        sensoresApuntandoHorizontal++;
                        break;
                    }
                }
                //Por la izquierda de la posición
                if(sensoresApuntandoHorizontal == 0) {
                    for(int j = columna-1; j >= 0; j--) {
                        if(piso.get(fila, j) == Pared) break;
                        if(piso.get(fila, j) == Importante) continue;
                        //Veo si entra un sensor horizontal que lo apunte
                        piso.set(fila, j, SensorHorizontal);
                        if(casilleroCorrecto(piso, fila, j)) {
                            piso.set(fila, j, Libre);
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
bool checkPiso(const Piso &piso) {
    Piso copia(piso);
    for(unsigned j=0;j<copia.filas();j++) {
        for(unsigned i=0;i<copia.columnas();i++) {
            if(!casilleroCorrecto(copia, j, i)) return false;
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


///////////////////////////////////////////////////////////////////////////////
// Implementación alternativa                                                //
///////////////////////////////////////////////////////////////////////////////

// Niveles de impresión de información de debugging
#define NONE   0
#define NORMAL 1
#define FULL   2

// Nivel elegido
#define DEBUG_LEVEL NONE

// Tipos de poda
#define ORIGINAL    0
#define ALTERNATIVA 1

// Poda elegida
// #define PODA ORIGINAL
#define PODA ALTERNATIVA

// Funciones para obtener o asignar la mejor solución hallada hasta el momento
Piso mejorSolucion(0, 0);
const Piso& getMejorSolucion() { return mejorSolucion; }
void setMejorSolucion(const Piso& p) { mejorSolucion = Piso(p); }
void limpiarMejorSolucion() { mejorSolucion = Piso(0, 0); }
unsigned getCostoMejorSolucion() {
    return mejorSolucion.filas() == 0 ? UINT_MAX : mejorSolucion.costo();
}

bool esLibre(const Piso& p, unsigned fila, unsigned columna) {
    return p.get(fila, columna) == Libre;
}

bool esPared(const Piso& p, unsigned fila, unsigned columna) {
    return p.get(fila, columna) == Pared;
}

bool esSensor(const Piso& p, unsigned fila, unsigned columna) {
    Casilla c = p.get(fila, columna);
    return c == SensorVertical ||
           c == SensorHorizontal ||
           c == SensorCuadruple;
}

bool esSensorVertical(const Piso& p, unsigned fila, unsigned columna) {
    Casilla c = p.get(fila, columna);
    return c == SensorVertical ||
           c == SensorCuadruple;
}

bool esSensorHorizontal(const Piso& p, unsigned fila, unsigned columna) {
    Casilla c = p.get(fila, columna);
    return c == SensorHorizontal ||
           c == SensorCuadruple;
}

bool esSensorCuadruple(const Piso& p, unsigned fila, unsigned columna) {
    return p.get(fila, columna) == SensorCuadruple;
}

// Decide si las posiciones (f1, c1), (f2, c2) están en conflicto.
bool noSonCompatibles(const Piso& p, unsigned f1, unsigned c1, unsigned f2, unsigned c2) {
    return (c1 == c2 && (esSensorVertical(p, f1, c1)   || esSensorVertical(p, f2, c2))) ||
           (f1 == f2 && (esSensorHorizontal(p, f1, c1) || esSensorHorizontal(p, f2, c2)));
}

// Devuelve false sólo si (fila, columna) es un sensor y apunta o es apuntado por otro sensor.
bool esCompatible(const Piso& p, unsigned fila, unsigned columna) {

    // Macro para informar el motivo cuando un piso no es compatible.
    #if DEBUG_LEVEL == FULL
        #define pisoNoCompatible(f1, c1, f2, c2) \
            cout << "Piso no compatible. El sensor en (" << f2 << ", " << c2 << ") " \
                 << "está en conflicto con (" << f1 << ", " << c1 << ")." << endl;
    #else
        #define pisoNoCompatible(f1, c1, f2, c2) { }
    #endif

    // Si la posición contiene un sensor,
    // miro en las 4 direcciones y verifico que
    // no exista ningún sensor apuntando al actual.
    if(esSensor(p, fila, columna)) {
        
        // Miro hacia arriba.
        for(int k = fila - 1; k >= 0; k--) {
            if(esPared(p, k, columna)) break;
            if(esSensor(p, k, columna) && noSonCompatibles(p, fila, columna, k, columna)) {
                pisoNoCompatible(fila, columna, k, columna);
                return false;                
            }
        }

        // Miro hacia abajo.
        for(int k = fila + 1; k < (int) p.filas(); k++) {
            if(esPared(p, k, columna)) break;
            if(esSensor(p, k, columna) && noSonCompatibles(p, fila, columna, k, columna)) {
                pisoNoCompatible(fila, columna, k, columna);
                return false;                
            }
        }

        // Miro hacia la izquierda.
        for(int k = columna - 1; k >= 0; k--) {
            if(esPared(p, fila, k)) break;
            if(esSensor(p, fila, k) && noSonCompatibles(p, fila, columna, fila, k)) {
                pisoNoCompatible(fila, columna, fila, k);
                return false;                
            }            
        }

        // Miro hacia la derecha.
        for(int k = columna + 1; k < (int) p.columnas(); k++) {
            if(esPared(p, fila, k)) break;
            if(esSensor(p, fila, k) && noSonCompatibles(p, fila, columna, fila, k)) {
                pisoNoCompatible(fila, columna, fila, k);
                return false;                
            }
        }
    }

    return true;
}

// Devuelve false sólo si se cumplen todas las condiciones a continuación:
// - (fila, columna) es un sensor unidireccional.
// - Existe una posición *p* libre en su misma fila o columna con línea de visión al sensor.
// - La posición *p* no es sensada por el sensor en (fila, columna).
// - Existe otro sensor con línea de visión a la posición *p* que no la sensa, ubicado en:
//   - Su misma columna, si *p* está en la misma fila que el sensor en (fila, columna), o bien
//   - Su misma fila, si *p* está en la misma columna que el sensor en (fila, columna).
bool esCandidatoASolucion(const Piso& p, unsigned fila, unsigned columna) {

    // Macro para informar el motivo cuando un piso no es compatible.
    #if DEBUG_LEVEL == FULL
        #define pisoNoCandidatoASolucion(f0, c0, f1, c1, f2, c2) \
            cout << "El piso no es candidato a solución. Los sensores en " \
                 << "(" << f1 << ", " << c1 << ") y " \
                 << "(" << f2 << ", " << c2 << ")" << endl \
                 << "producen que la posición " \
                 << "(" << f0 << ", " << c0 << ") sea imposible de sensar." << endl;
    #else
        #define pisoNoCandidatoASolucion(f0, c0, f1, c1, f2, c2) { }
    #endif

    // Si es un sensor vertical, recorro su fila y me aseguro que no haya
    // en ésta ninguna posición libre con un sensor horizontal en su columna.
    if(p.get(fila, columna) == SensorVertical) {

        // Miro hacia la izquierda.
        for(int k = columna - 1; k >= 0; k--) {
            if(esPared(p, fila, k)) break;
            if(!esLibre(p, fila, k)) continue;

            // Miro hacia arriba
            for(int q = fila - 1; q >= 0; q--) {
                if(esPared(p, q, k)) break;
                if(p.get(q, k) == SensorHorizontal) {
                    pisoNoCandidatoASolucion(fila, k, fila, columna, q, k);
                    return false;
                }
            }

            // Miro hacia abajo
            for(int q = fila + 1; q < (int) p.filas(); q++) {
                if(esPared(p, q, k)) break;
                if(p.get(q, k) == SensorHorizontal) {
                    pisoNoCandidatoASolucion(fila, k, fila, columna, q, k);
                    return false;
                }
            }
        }

        // Miro hacia la derecha.
        for(int k = columna + 1; k < (int) p.columnas(); k++) {
            if(esPared(p, fila, k)) break;
            if(!esLibre(p, fila, k)) continue;

            // Miro hacia arriba
            for(int q = fila - 1; q >= 0; q--) {
                if(esPared(p, q, k)) break;
                if(p.get(q, k) == SensorHorizontal) {
                    pisoNoCandidatoASolucion(fila, k, fila, columna, q, k);
                    return false;
                }
            }

            // Miro hacia abajo
            for(int q = fila + 1; q < (int) p.filas(); q++) {
                if(esPared(p, q, k)) break;
                if(p.get(q, k) == SensorHorizontal) {
                    pisoNoCandidatoASolucion(fila, k, fila, columna, q, k);
                    return false;
                }
            }
        }
    }

    // Si es un sensor horizontal, recorro su columna y me aseguro que no haya
    // en ésta ninguna posición libre con un sensor vertical en su fila.
    if(p.get(fila, columna) == SensorHorizontal) {

        // Miro hacia arriba.
        for(int k = fila - 1; k >= 0; k--) {
            if(esPared(p, k, columna)) break;
            if(!esLibre(p, k, columna)) continue;

            // Miro hacia la izquierda
            for(int q = columna - 1; q >= 0; q--) {
                if(esPared(p, k, q)) break;
                if(p.get(k, q) == SensorVertical) {
                    pisoNoCandidatoASolucion(k, columna, fila, columna, k, q);
                    return false;
                }
            }

            // Miro hacia abajo
            for(int q = columna + 1; q < (int) p.columnas(); q++) {
                if(esPared(p, k, q)) break;
                if(p.get(k, q) == SensorVertical) {
                    pisoNoCandidatoASolucion(k, columna, fila, columna, k, q);
                    return false;
                }
            }
        }

        // Miro hacia abajo.
        for(int k = fila + 1; k < (int) p.filas(); k++) {
            if(esPared(p, k, columna)) break;
            if(!esLibre(p, k, columna)) continue;

            // Miro hacia la izquierda
            for(int q = columna - 1; q >= 0; q--) {
                if(esPared(p, k, q)) break;
                if(p.get(k, q) == SensorVertical) {
                    pisoNoCandidatoASolucion(k, columna, fila, columna, k, q);
                    return false;
                }
            }

            // Miro hacia abajo
            for(int q = columna + 1; q < (int) p.columnas(); q++) {
                if(esPared(p, k, q)) break;
                if(p.get(k, q) == SensorVertical) {
                    pisoNoCandidatoASolucion(k, columna, fila, columna, k, q);
                    return false;
                }
            }
        }
    }

    return true;
}

// Devuelve false sólo si la posición (fila, columna) es un sensor y no existe ninguna
// posición libre en su misma fila/columna donde se pueda ubicar un sensor que la afecte
// y que no esté en conflicto con otros sensores.
bool esSensable(const Piso& p, unsigned fila, unsigned columna) {
    if(!esLibre(p, fila, columna)) return true;

    // Miro hacia arriba.
    for(int k = fila - 1; k >= 0; k--) {
        if(esPared(p, k, columna)) break;
        if(esLibre(p, k, columna)) {
            Piso q(p);
            q.set(k, columna, SensorVertical);
            if(esCompatible(q, k, columna)) return true;
        }
    }

    // Miro hacia abajo.
    for(int k = fila + 1; k < (int) p.filas(); k++) {
        if(esPared(p, k, columna)) break;
        if(esLibre(p, k, columna)) {
            Piso q(p);
            q.set(k, columna, SensorVertical);
            if(esCompatible(q, k, columna)) return true;
        }
    }

    // Miro hacia la izquierda.
    for(int k = columna - 1; k >= 0; k--) {
        if(esPared(p, fila, k)) break;
        if(esLibre(p, fila, k)) {
            Piso q(p);
            q.set(fila, k, SensorHorizontal);
            if(esCompatible(q, fila, k)) return true;
        }
    }

    // Miro hacia la derecha.
    for(int k = columna + 1; k < (int) p.columnas(); k++) {
        if(esPared(p, fila, k)) break;
        if(esLibre(p, fila, k)) {
            Piso q(p);
            q.set(fila, k, SensorHorizontal);
            if(esCompatible(q, fila, k)) return true;
        }
    }

    return false;
}

// Devuelve true si el costo total de los sensores del piso no excede
// el costo de la mejor solución hallada hasta el momento.
bool esMasBaratoQueMejorSolucion(const Piso& piso) {
    return piso.costo() < getCostoMejorSolucion();
}

// Devuelve false si la modificación del piso en (fila, columna)
// no conduce a una solución.
bool podar(const Piso& p, unsigned fila, unsigned columna) {
    #if PODA == ORIGINAL
        Piso copia(p);
        return !casilleroCorrecto(copia, fila, columna);
    #else
        return !esCompatible(p, fila, columna) ||
               !esCandidatoASolucion(p, fila, columna) ||
               !esSensable(p, fila, columna) ||
               !esMasBaratoQueMejorSolucion(p);
    #endif
}

// Devuelve true si todas las casillas libres están sensadas y
// todas las casillas importantes están doblemente sensadas.
// Asume que el piso recibido es válido.
bool esSolucion(const Piso& p) {

    // Recorro todas las posiciones
    for(unsigned i = 0; i < p.filas(); i++) {
        for(unsigned j = 0; j < p.columnas(); j++) {

            // Si fuera solución, no podría tener ninguna posición sin sensar.
            if(esLibre(p, i, j)) return false;

            // Si la posición actual requiere doble sensado, cuento
            // los sensores actuando sobre esta y verifico que sea
            // la cantidad correcta.
            if(p.get(i,j) == Importante) {
                int sensores = 0;

                // Miro hacia arriba.
                for(int k = i - 1; k >= 0; k--) {
                    if(esPared(p, k, j)) break;
                    if(esSensorVertical(p, k, j)) {
                        sensores++;
                        break;
                    }
                }

                // Miro hacia abajo.
                for(int k = i + 1; k < (int) p.filas(); k++) {
                    if(esPared(p, k, j)) break;
                    if(esSensorVertical(p, k, j)) {
                        sensores++;
                        break;
                    }
                }

                // Miro hacia la izquierda.
                for(int k = j - 1; k >= 0; k--) {
                    if(esPared(p, i, k)) break;
                    if(esSensorHorizontal(p, i, k)) {
                        sensores++;
                        break;
                    }
                }

                // Miro hacia la derecha.
                for(int k = j + 1; k < (int) p.columnas(); k++) {
                    if(esPared(p, i, k)) break;
                    if(esSensorHorizontal(p, i, k)) {
                        sensores++;
                        break;
                    }
                }

                // Verifico que la cantidad de sensores sea correcta.
                if(sensores < 2) return false;
            }
        }
    }

    return true;
}

// Ubica un sensor del tipo especificado en la posición dada,
// y marca como sensadas todas las posiciones afectadas por este.
void ubicarSensor(Piso& p, unsigned fila, unsigned columna, Casilla sensor) {
    p.set(fila, columna, sensor);

    #if PODA == ALTERNATIVA
        if(esSensorVertical(p, fila, columna)) {
            // Senso hacia arriba.
            for(int k = fila - 1; k >= 0; k--) {
                if(esPared(p, k, columna)) break;
                if(p.get(k, columna) == Libre) p.set(k, columna, Sensado);
            }

            // Senso hacia abajo.
            for(int k = fila + 1; k < (int) p.filas(); k++) {
                if(esPared(p, k, columna)) break;
                if(p.get(k, columna) == Libre) p.set(k, columna, Sensado);
            }
        }

        if(esSensorHorizontal(p, fila, columna)) {
            // Senso hacia la izquierda.
            for(int k = columna - 1; k >= 0; k--) {
                if(esPared(p, fila, k)) break;
                if(p.get(fila, k) == Libre) p.set(fila, k, Sensado);
            }

            // Senso hacia la derecha.
            for(int k = columna + 1; k < (int) p.columnas(); k++) {
                if(esPared(p, fila, k)) break;
                if(p.get(fila, k) == Libre) p.set(fila, k, Sensado);
            }
        }
    #endif
}

// Busca la siguiente posición libre desde (fila, columna) recorriendo el piso
// de izquierda a derecha y de arriba a abajo, sin tener a esa posición en cuenta.
// Devuelve el resultado en (filaSig, columnaSig).
//
// Si no encontró ninguna posición libre, devuelve una posición fuera de rango.
void buscarSiguientePosicionLibre(
        const Piso& p, unsigned fila, unsigned columna,
        unsigned& filaSig, unsigned& columnaSig) {

    filaSig = fila;
    columnaSig = columna;

    do {
        filaSig    = columnaSig + 1 == p.columnas() ? filaSig + 1 : filaSig;
        columnaSig = columnaSig + 1 == p.columnas() ? 0           : columnaSig + 1;
    } while(filaSig < p.filas() && p.get(filaSig, columnaSig) != Libre);
}

// 1. Decide si se realiza la poda:
//    - Verifica la compatibilidad de la posición (fila, columna); es decir, que no haya
//      un sensor en esa posición en conflicto con otro sensor.
//    - Verifica que la posición (fila, columna) no produzca posiciones libres
//      imposibles de sensar.
// 2. Decide la siguiente posición a evaluar, recorriendo de izquierda a derecha
//    y de arriba a abajo hasta encontrar una posición libre.
// 3. Si no quedan más posiciones por evaluar, verifica si el piso recibido es
//    solución, y si lo es, compara el costo con el de la mejor solución hasta el momento
//    y la reemplaza en caso de ser más barato.
// 4. En caso contrario, evalúa cada posible modificación en la posición elegida,
//    y se llama recursivamente pasándose como parámetro el piso resultante.
//
// Caso especial (para iniciar la recursión):
// Si la posición recibida es (UINT_MAX, UINT_MAX), se saltea el paso 1. y comienza
// en la posición (0, 0) cuando busca una posición libre en el paso 2.
//
// Invariante:
// El piso recibido *siempre* es compatible si reemplazamos la posición (fila, columna)
// por una casilla Libre.
void recorrer(const Piso& p, unsigned fila, unsigned columna, int& iteracion) {
    iteracion++;

    // Imprimimos información de debugging.
    #if DEBUG_LEVEL == FULL
        cout << endl << "Iteración " << iteracion << ". ";
        if(fila != UINT_MAX && columna != UINT_MAX) {
            cout << "Evaluando posición (" << fila << ", " << columna << ") "
                 << "en el siguiente piso:" << endl;  
            p.imprimir();
        } else {
            cout << "Iniciando recursión." << endl;
        }
    #elif DEBUG_LEVEL == NORMAL
        if(iteracion % 10000 == 0)
            cout << "Iteración " << iteracion << "." << endl;

    #endif

    // Paso 1: Verificamos la validez de la posición (fila, columna).
    if(fila    != UINT_MAX &&
       columna != UINT_MAX &&
       podar(p, fila, columna)) return;

    // Paso 2: Decidimos las coordenadas de la próxima posición a evaluar.
    unsigned filaSig, columnaSig;
    if(fila == UINT_MAX || columna == UINT_MAX) {
        if(p.get(0, 0) != Libre) {
            buscarSiguientePosicionLibre(p, 0, 0, filaSig, columnaSig);
        } else {
            filaSig = columnaSig = 0;
        }
    } else {
        buscarSiguientePosicionLibre(p, fila, columna, filaSig, columnaSig);
    }

    // Paso 3: Si no quedan más posiciones por evaluar, decidimos si hallamos una solución.
    if(filaSig >= p.filas() || columnaSig >= p.columnas()) {
        // Si es solución, terminamos.
        #pragma omp critical
        if(esSolucion(p) && p.costo() < getCostoMejorSolucion()) {
            setMejorSolucion(p);

            #if DEBUG_LEVEL == FULL
                cout << "Se halló una solución con costo " << p.costo() << "." << endl;
            #endif
        }
    }

    // Paso 4: Evaluamos cada modificación posible en la posición elegida
    // y continuamos la recursión.
    else {
        #pragma omp parallel sections
        {
            // Continuamos ubicando un sensor vertical.
            #pragma omp section
            {
                Piso q(p);
                ubicarSensor(q, filaSig, columnaSig, SensorVertical);
                recorrer(q, filaSig, columnaSig, iteracion);
            }

            // Continuamos ubicando un sensor horizontal.
            #pragma omp section
            {
                Piso q(p);
                ubicarSensor(q, filaSig, columnaSig, SensorHorizontal);
                recorrer(q, filaSig, columnaSig, iteracion);
            }

            // Continuamos ubicando un sensor cuádruple.
            #pragma omp section
            {
                Piso q(p);
                ubicarSensor(q, filaSig, columnaSig, SensorCuadruple);
                recorrer(q, filaSig, columnaSig, iteracion);
            }

            // Continuamios dejando la posición intacta.
            #pragma omp section
            {
                recorrer(p, filaSig, columnaSig, iteracion);
            }
        }
    }
}

Piso resolver(const Piso& p) {
    #if DEBUG_LEVEL >= NORMAL
        cout << "Iniciando búsqueda de soluciones para el siguiente piso:" << endl;
        p.imprimir();
        cout << endl;
    #endif

    int iteraciones = 0;
    limpiarMejorSolucion();
    recorrer(p, UINT_MAX, UINT_MAX, iteraciones);

    #if DEBUG_LEVEL >= NORMAL
        cout << "Iteraciones realizadas: " << iteraciones << endl;
    #endif

    if(getCostoMejorSolucion() == UINT_MAX) {
        #if DEBUG_LEVEL >= NORMAL
            cout << "No se encontraron soluciones." << endl;
        #endif
    } else {
        #if DEBUG_LEVEL >= NORMAL
            cout << "Mejor solución hallada:" << endl;
            getMejorSolucion().imprimir();
            cout << "Costo: $" << getCostoMejorSolucion() << endl;
        #endif
    }

    return getMejorSolucion();
}
