#include <iostream>
#include <vector>
#include <climits>
#include <omp.h>

#include "problema3.h"

using namespace std;

Piso::Piso(unsigned filas, unsigned columnas) {
    _filas = filas;
    _columnas = columnas;
    grilla = new Casilla[filas * columnas];
    for(unsigned i = 0; i < filas * columnas; i++) grilla[i] = Libre;
}

Piso::Piso(unsigned filas, unsigned columnas, string casillas) : Piso(filas, columnas) {
    for(unsigned i = 0; i < filas; i++) {
        for(unsigned j = 0; j < columnas; j++) {
            switch(casillas[columnas * i + j]) {
                case '#': en(i, j) = Pared;            break;
                case '*': en(i, j) = Importante;       break;
                case '|': en(i, j) = SensorVertical;   break;
                case '-': en(i, j) = SensorHorizontal; break;
                case '+': en(i, j) = SensorCuadruple;  break;
            }
        }
    }
}

Piso::Piso(const Piso &otro) {
    _filas = otro._filas;
    _columnas = otro._columnas;
    grilla = new Casilla[_filas * _columnas];
    for(unsigned i = 0; i < _filas * _columnas; i++) grilla[i] = otro.grilla[i];
}

Piso::~Piso() {
    delete grilla;
}

bool Piso::operator==(const Piso& otro) const {
    if(_filas != otro._filas || _columnas != otro._columnas) return false;
    for(unsigned i = 0; i < _filas * _columnas; i++)
        if(grilla[i] != otro.grilla[i]) return false;
    return true;
}

Casilla& Piso::en(unsigned fila, unsigned columna) {
    if(fila >= _filas || columna >= _columnas) {
        cout << "Acceso fuera de rango: (" << fila << ", " << columna << ")." << endl;;
        exit(-1);
    }

    return grilla[_columnas * (fila) + columna];
}

Casilla Piso::en(unsigned fila, unsigned columna) const {
    if(fila >= _filas || columna >= _columnas) {
        cout << "Acceso fuera de rango: (" << fila << ", " << columna << ")." << endl;
        exit(-1);
    }

    return grilla[_columnas * (fila) + columna];
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
            switch(en(i, j)) {
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
#define DEBUG_LEVEL NORMAL

// Tipos de poda
#define ORIGINAL    0
#define ALTERNATIVA 1

// Poda elegida
// #define PODA ORIGINAL
#define PODA ALTERNATIVA

bool esLibre(const Piso& p, unsigned fila, unsigned columna) {
    return p.en(fila, columna) == Libre;
}

bool esPared(const Piso& p, unsigned fila, unsigned columna) {
    return p.en(fila, columna) == Pared;
}

bool esSensor(const Piso& p, unsigned fila, unsigned columna) {
    Casilla c = p.en(fila, columna);
    return c == SensorVertical ||
           c == SensorHorizontal ||
           c == SensorCuadruple;
}

bool esSensorVertical(const Piso& p, unsigned fila, unsigned columna) {
    Casilla c = p.en(fila, columna);
    return c == SensorVertical ||
           c == SensorCuadruple;
}

bool esSensorHorizontal(const Piso& p, unsigned fila, unsigned columna) {
    Casilla c = p.en(fila, columna);
    return c == SensorHorizontal ||
           c == SensorCuadruple;
}

bool esSensorCuadruple(const Piso& p, unsigned fila, unsigned columna) {
    return p.en(fila, columna) == SensorCuadruple;
}

// Decide si las posiciones (f1, c1), (f2, c2) están en conflicto.
bool noSonCompatibles(const Piso& p, unsigned f1, unsigned c1, unsigned f2, unsigned c2) {
    return (c1 == c2 && (esSensorVertical(p, f1, c1)   || esSensorVertical(p, f2, c2))) ||
           (f1 == f2 && (esSensorHorizontal(p, f1, c1) || esSensorHorizontal(p, f2, c2)));
}

// Devuelve true si (fila, columna) es compatible con todas las posiciones
// en la misma fila o columna.
bool esCompatible(const Piso& p, unsigned fila, unsigned columna) {

    // Macro para informar el motivo cuando un piso no es compatible.
    #if DEBUG_LEVEL == FULL
        #define pisoInvalido(f1, c1, f2, c2) \
            cout << "Piso no compatible. El sensor en (" << f2 << ", " << c2 << ") " \
                 << "está en conflicto con (" << f1 << ", " << c1 << ")." << endl;
    #else
        #define pisoInvalido(f1, c1, f2, c2) { }
    #endif

    // Si la posición contiene un sensor,
    // miro en las 4 direcciones y verifico que
    // no exista ningún sensor apuntando al actual.
    if(esSensor(p, fila, columna)) {
        
        // Miro hacia arriba.
        for(int k = fila - 1; k >= 0; k--) {
            if(esPared(p, k, columna)) break;
            if(esSensor(p, k, columna) && noSonCompatibles(p, fila, columna, k, columna)) {
                pisoInvalido(fila, columna, k, columna);
                return false;                
            }
        }

        // Miro hacia abajo.
        for(int k = fila + 1; k < (int) p.filas(); k++) {
            if(esPared(p, k, columna)) break;
            if(esSensor(p, k, columna) && noSonCompatibles(p, fila, columna, k, columna)) {
                pisoInvalido(fila, columna, k, columna);
                return false;                
            }
        }

        // Miro hacia la izquierda.
        for(int k = columna - 1; k >= 0; k--) {
            if(esPared(p, fila, k)) break;
            if(esSensor(p, fila, k) && noSonCompatibles(p, fila, columna, fila, k)) {
                pisoInvalido(fila, columna, fila, k);
                return false;                
            }            
        }

        // Miro hacia la derecha.
        for(int k = columna + 1; k < (int) p.columnas(); k++) {
            if(esPared(p, fila, k)) break;
            if(esSensor(p, fila, k) && noSonCompatibles(p, fila, columna, fila, k)) {
                pisoInvalido(fila, columna, fila, k);
                return false;                
            }
        }
    }

    return true;
}

// Devuelve true si el sensor ubicado en (fila, columna) no genera posiciones
// libres imposibles de sensar, o si (fila, columna) es una posición libre.
bool esCandidatoASolucion(const Piso& p, unsigned fila, unsigned columna) {

    // Macro para informar el motivo cuando un piso no es compatible.
    #if DEBUG_LEVEL == FULL
        #define pisoInvalido(f0, c0, f1, c1, f2, c2) \
            cout << "El piso no es candidato a solución. Los sensores en " \
                 << "(" << f1 << ", " << c1 << ") y " \
                 << "(" << f2 << ", " << c2 << ")" << endl \
                 << "producen que la posición " \
                 << "(" << f0 << ", " << c0 << ") sea imposible de sensar." << endl;
    #else
        #define pisoInvalido(f0, c0, f1, c1, f2, c2) { }
    #endif

    // Si es un sensor vertical, recorro su fila y me aseguro que no haya
    // en ésta ninguna posición libre con un sensor horizontal en su columna.
    if(p.en(fila, columna) == SensorVertical) {

        // Miro hacia la izquierda.
        for(int k = columna - 1; k >= 0; k--) {
            if(esPared(p, fila, k)) break;
            if(!esLibre(p, fila, k)) continue;

            // Miro hacia arriba
            for(int q = fila - 1; q >= 0; q--) {
                if(esPared(p, q, k)) break;
                if(p.en(q, k) == SensorHorizontal) {
                    pisoInvalido(fila, k, fila, columna, q, k);
                    return false;
                }
            }

            // Miro hacia abajo
            for(int q = fila + 1; q < (int) p.filas(); q++) {
                if(esPared(p, q, k)) break;
                if(p.en(q, k) == SensorHorizontal) {
                    pisoInvalido(fila, k, fila, columna, q, k);
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
                if(p.en(q, k) == SensorHorizontal) {
                    pisoInvalido(fila, k, fila, columna, q, k);
                    return false;
                }
            }

            // Miro hacia abajo
            for(int q = fila + 1; q < (int) p.filas(); q++) {
                if(esPared(p, q, k)) break;
                if(p.en(q, k) == SensorHorizontal) {
                    pisoInvalido(fila, k, fila, columna, q, k);
                    return false;
                }
            }
        }
    }

    // Si es un sensor horizontal, recorro su columna y me aseguro que no haya
    // en ésta ninguna posición libre con un sensor vertical en su fila.
    if(p.en(fila, columna) == SensorHorizontal) {

        // Miro hacia arriba.
        for(int k = fila - 1; k >= 0; k--) {
            if(esPared(p, k, columna)) break;
            if(!esLibre(p, k, columna)) continue;

            // Miro hacia la izquierda
            for(int q = columna - 1; q >= 0; q--) {
                if(esPared(p, k, q)) break;
                if(p.en(k, q) == SensorVertical) {
                    pisoInvalido(k, columna, fila, columna, k, q);
                    return false;
                }
            }

            // Miro hacia abajo
            for(int q = columna + 1; q < (int) p.columnas(); q++) {
                if(esPared(p, k, q)) break;
                if(p.en(k, q) == SensorVertical) {
                    pisoInvalido(k, columna, fila, columna, k, q);
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
                if(p.en(k, q) == SensorVertical) {
                    pisoInvalido(k, columna, fila, columna, k, q);
                    return false;
                }
            }

            // Miro hacia abajo
            for(int q = columna + 1; q < (int) p.columnas(); q++) {
                if(esPared(p, k, q)) break;
                if(p.en(k, q) == SensorVertical) {
                    pisoInvalido(k, columna, fila, columna, k, q);
                    return false;
                }
            }
        }
    }

    return true;
}

// Devuelve true si la posición libre ubicada en (fila, columna) tiene alguna posición
// libre en su entorno donde poder poner un sensor que la cubra.
bool esSensable(const Piso& p, unsigned fila, unsigned columna) {
    if(!esLibre(p, fila, columna)) return true;

    // Miro hacia arriba.
    for(int k = fila - 1; k >= 0; k--) {
        if(esPared(p, k, columna)) break;
        if(esLibre(p, k, columna)) {
            Piso q(p);
            q.en(k, columna) = SensorVertical;
            if(esCompatible(q, k, columna)) return true;
        }
    }

    // Miro hacia abajo.
    for(int k = fila + 1; k < (int) p.filas(); k++) {
        if(esPared(p, k, columna)) break;
        if(esLibre(p, k, columna)) {
            Piso q(p);
            q.en(k, columna) = SensorVertical;
            if(esCompatible(q, k, columna)) return true;
        }
    }

    // Miro hacia la izquierda.
    for(int k = columna - 1; k >= 0; k--) {
        if(esPared(p, fila, k)) break;
        if(esLibre(p, fila, k)) {
            Piso q(p);
            q.en(fila, k) = SensorHorizontal;
            if(esCompatible(q, fila, k)) return true;
        }
    }

    // Miro hacia la derecha.
    for(int k = columna + 1; k < (int) p.columnas(); k++) {
        if(esPared(p, fila, k)) break;
        if(esLibre(p, fila, k)) {
            Piso q(p);
            q.en(fila, k) = SensorHorizontal;
            if(esCompatible(q, fila, k)) return true;
        }
    }

    return false;
}

// Devuelve false si la modificación del piso p en (fila, columna)
// no conduce a una solución.
bool podar(const Piso& p, unsigned fila, unsigned columna) {
    #if PODA == ORIGINAL
        Piso copia(p);
        return !casilleroCorrecto(copia, fila, columna);
    #else
        return !esCompatible(p, fila, columna) ||
               // !esCandidatoASolucion(p, fila, columna);
               !esCandidatoASolucion(p, fila, columna) ||
               !esSensable(p, fila, columna);
    #endif
}

bool requiereSensado(const Piso& p, unsigned fila, unsigned columna) {
    return p.en(fila, columna) == Libre ||
           p.en(fila, columna) == Importante;
}

// Devuelve true si todas las casillas libres están sensadas y
// todas las casillas importantes están doblemente sensadas.
// Asume que el piso recibido es válido. (Ver esCompatible().)
bool esSolucion(const Piso& p) {

    // Recorro todas las posiciones
    for(unsigned i = 0; i < p.filas(); i++) {
        for(unsigned j = 0; j < p.columnas(); j++) {

            // Si la posición actual requiere sensado, cuento los
            // sensores actuando sobre esta y verifico que sea
            // la cantidad correcta.
            if(requiereSensado(p, i, j)) {
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
                if(p.en(i, j) == Libre && sensores == 0) return false;
                if(p.en(i, j) == Importante && sensores < 2) return false;
            }
        }
    }

    return true;
}

// Ubica un sensor del tipo especificado en la posición dada,
// y marca como sensadas todas las posiciones afectadas por este.
void ubicarSensor(Piso& p, unsigned fila, unsigned columna, Casilla sensor) {
    p.en(fila, columna) = sensor;

    #if PODA == ALTERNATIVA
        if(esSensorVertical(p, fila, columna)) {
            // Senso hacia arriba.
            for(int k = fila - 1; k >= 0; k--) {
                if(esPared(p, k, columna)) break;
                if(p.en(k, columna) == Libre) p.en(k, columna) = Sensado;
            }

            // Senso hacia abajo.
            for(int k = fila + 1; k < (int) p.filas(); k++) {
                if(esPared(p, k, columna)) break;
                if(p.en(k, columna) == Libre) p.en(k, columna) = Sensado;
            }
        }

        if(esSensorHorizontal(p, fila, columna)) {
            // Senso hacia la izquierda.
            for(int k = columna - 1; k >= 0; k--) {
                if(esPared(p, fila, k)) break;
                if(p.en(fila, k) == Libre) p.en(fila, k) = Sensado;
            }

            // Senso hacia la derecha.
            for(int k = columna + 1; k < (int) p.columnas(); k++) {
                if(esPared(p, fila, k)) break;
                if(p.en(fila, k) == Libre) p.en(fila, k) = Sensado;
            }
        }
    #endif
}

// Busca la siguiente posición libre desde (fila, columna) recorriendo el piso
// de izquierda a derecha y de arriba a abajo, sin tener a esa posición en cuenta,
// y devuelve el resultado en (filaSig, columnaSig).
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
    } while(filaSig < p.filas() && p.en(filaSig, columnaSig) != Libre);
}

// 1. Decide si se realiza la poda:
//    - Verifica la compatibilidad de la posición (fila, columna); es decir, que no haya
//      un sensor en esa posición en conflicto con otro sensor.
//    - Verifica que la posición (fila, columna) no produzca posiciones libres
//      imposibles de sensar.
// 2. Decide la siguiente posición a evaluar, recorriendo de izquierda a derecha
//    y de arriba a abajo hasta encontrar una posición libre.
// 3. Si no quedan más posiciones por evaluar, verifica si el piso recibido es
//    solución, y en tal caso lo agrega al vector de soluciones.
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
void recorrer(const Piso& p, unsigned fila, unsigned columna,
              vector<Piso>& soluciones, int& iteracion) {
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
            cout << "Iteración " << iteracion << ". "
                 << "Soluciones halladas: " << soluciones.size() << endl;
    #endif

    // Paso 1: Verificamos la validez de la posición (fila, columna).
    if(fila    != UINT_MAX &&
       columna != UINT_MAX &&
       podar(p, fila, columna)) return;

    // Paso 2: Decidimos las coordenadas de la próxima posición a evaluar.
    unsigned filaSig, columnaSig;
    if(fila == UINT_MAX || columna == UINT_MAX) {
        if(p.en(0, 0) != Libre) {
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
        if(esSolucion(p)) {
            #pragma omp critical
            soluciones.push_back(p);
            #if DEBUG_LEVEL == FULL
                cout << "Se halló una solución." << endl;
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
                recorrer(q, filaSig, columnaSig, soluciones, iteracion);
            }

            // Continuamos ubicando un sensor horizontal.
            #pragma omp section
            {
                Piso q(p);
                ubicarSensor(q, filaSig, columnaSig, SensorHorizontal);
                recorrer(q, filaSig, columnaSig, soluciones, iteracion);
            }

            // Continuamos ubicando un sensor cuádruple.
            #pragma omp section
            {
                Piso q(p);
                ubicarSensor(q, filaSig, columnaSig, SensorCuadruple);
                recorrer(q, filaSig, columnaSig, soluciones, iteracion);
            }

            // Continuamios dejando la posición intacta.
            #pragma omp section
            {
                recorrer(p, filaSig, columnaSig, soluciones, iteracion);
            }
        }
    }
}

// Devuelve el costo de una solución
int costo(const Piso& p) {
    int costo = 0;

    for(unsigned i = 0; i < p.filas(); i++) {
        for(unsigned j = 0; j < p.columnas(); j++) {
            switch(p.en(i, j)) {
                case SensorVertical:
                case SensorHorizontal:
                costo += 4000;
                break;

                case SensorCuadruple:
                costo += 6000;
                break;

                default: break;
            }
        }
    }

    return costo;
}

Piso resolver(const Piso& p) {
    #if DEBUG_LEVEL >= NORMAL
        cout << "Iniciando búsqueda de soluciones para el siguiente piso:" << endl;
        p.imprimir();
        cout << endl;
    #endif

    vector<Piso> soluciones;
    int iteraciones = 0;
    recorrer(p, UINT_MAX, UINT_MAX, soluciones, iteraciones);

    #if DEBUG_LEVEL >= NORMAL
        cout << "Iteraciones realizadas: " << iteraciones << endl;
    #endif

    if(soluciones.size() == 0) {
        #if DEBUG_LEVEL >= NORMAL
            cout << "No se encontraron soluciones." << endl;
        #endif

        return p;
    } else {
        #if DEBUG_LEVEL >= NORMAL
            cout << "Se encontraron " << soluciones.size() << " soluciones." << endl;
        #endif

        #if DEBUG_LEVEL == FULL
            cout << "Soluciones halladas:" << endl;
            for(size_t i = 0; i < soluciones.size(); i++) soluciones[i].imprimir();
        #endif

        for(size_t i = 0; i < soluciones.size(); i++)
            for(size_t j = 0; j < soluciones.size(); j++)
                if(i != j && soluciones[i] == soluciones[j]) {
                    cout << "Las soluciones " << i << " y " << j << " están repetidas:" << endl;
                    soluciones[i].imprimir();
                }

        int mejorPiso = 0;
        int mejorCosto = costo(soluciones[mejorPiso]);

        for(size_t i = 0; i < soluciones.size(); i++) {
            int otroCosto = costo(soluciones[i]);

            if(otroCosto < mejorCosto) {
                mejorCosto = otroCosto;
                mejorPiso = i;
            }
        }

        #if DEBUG_LEVEL >= NORMAL
            cout << "Mejor solución:" << endl;
            soluciones[mejorPiso].imprimir();
            cout << "Costo: $" << mejorCosto << endl;
        #endif

        return soluciones[mejorPiso];
    }
}
