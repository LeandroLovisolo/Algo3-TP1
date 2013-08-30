#include <iostream>
#include <vector>

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


///////////////////////////////////////////////////////////////////////////////
// Implementación alternativa                                                //
///////////////////////////////////////////////////////////////////////////////

// Niveles de impresión de información de debugging
#define NONE   0
#define NORMAL 1
#define FULL   2

// Nivel elegido
#define DEBUG_LEVEL NORMAL

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

bool esSensor(const Piso& p, unsigned fila, unsigned columna) {
    Casilla c = p.en(fila, columna);
    return c == SensorVertical ||
           c == SensorHorizontal ||
           c == SensorCuadruple;
}

bool esPared(const Piso& p, unsigned fila, unsigned columna) {
    return p.en(fila, columna) == Pared;
}

// Devuelve true si no hay ningún sensor apuntando a otro.
bool esValido(const Piso& p) {

    // Macro para informar el motivo cuando un piso es inválido.
    #if DEBUG_LEVEL == FULL
        #define pisoInvalido(p, f1, c1, f2, c2) \
            cout << "Piso inválido. El sensor en (" << f2 << ", " << c2 << ") " \
                 << "está en conflicto con (" << f1 << ", " << c1 << ")." << endl;
    #else
        #define pisoInvalido(p, f1, c1, f2, c2) { }
    #endif

    // Recorro todas las posiciones
    for(unsigned i = 0; i < p.filas(); i++) {
        for(unsigned j = 0; j < p.columnas(); j++) {

            // Si la posición actual contiene un sensor,
            // miro en las 4 direcciones y verifico que
            // no exista ningún sensor apuntando al actual.
            if(esSensor(p, i, j)) {
                
                // Miro hacia arriba.
                for(int k = i - 1; k >= 0; k--) {
                    if(esPared(p, k, j)) break;
                    if(esSensorVertical(p, k, j)) {
                        pisoInvalido(p, i, j, k, j);
                        return false;
                    }
                }

                // Miro hacia abajo.
                for(int k = i + 1; k < (int) p.filas(); k++) {
                    if(esPared(p, k, j)) break;
                    if(esSensorVertical(p, k, j)) {
                        pisoInvalido(p, i, j, k, j);
                        return false;
                    }
                }

                // Miro hacia la izquierda.
                for(int k = j - 1; k >= 0; k--) {
                    if(esPared(p, i, k)) break;
                    if(esSensorHorizontal(p, i, k)) {
                        pisoInvalido(p, i, j, i, k);
                        return false;
                    }
                }

                // Miro hacia la derecha.
                for(int k = j + 1; k < (int) p.columnas(); k++) {
                    if(esPared(p, i, k)) break;
                    if(esSensorHorizontal(p, i, k)) {
                        pisoInvalido(p, i, j, i, k);
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool requiereSensado(const Piso& p, unsigned fila, unsigned columna) {
    return p.en(fila, columna) == Libre ||
           p.en(fila, columna) == Importante;
}

// Devuelve true si todas las casillas libres están sensadas y
// todas las casillas importantes están doblemente sensadas.
// Asume que el piso recibido es válido. (Ver esValido().)
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
}

// Verifica la validez del piso, agrega el piso a la lista de soluciones
// en caso de ser solución, y si la posición dada es válida, continúa la
// recursión probando cada tipo de sensor en la posición dada.
void recorrer(const Piso& p, unsigned fila, unsigned columna,
              vector<Piso>& soluciones, int& iteracion) {
    iteracion++;

    // Imprimimos información de debugging.
    #if DEBUG_LEVEL == FULL
        cout << endl << "Iteración " << iteracion << ". Evaluando posición "
             << "(" << fila << ", " << columna << ") en el siguiente piso:" << endl;
        p.imprimir();
    #elif DEBUG_LEVEL == NORMAL
        if(iteracion % 10000 == 0) cout << "Iteración " << iteracion << endl;
    #endif

    // Verificamos que no hayan sensores apuntándose entre sí.
    if(!esValido(p)) return;

    // Si es solución, terminamos.
    if(esSolucion(p)) {
        #if DEBUG_LEVEL >= NORMAL
            cout << "Se halló una solución." << endl;
        #endif

        soluciones.push_back(p);
        return;
    }

    // Verificamos que no hayamos terminado de recorrer todo el piso.
    if(fila >= p.filas() || columna >= p.columnas()) return;

    // Decidimos las coordenadas de la próxima posición a evaluar.
    unsigned filaSig    = fila;
    unsigned columnaSig = columna;
    do {
        filaSig    = columnaSig + 1 == p.columnas() ? filaSig + 1 : filaSig;
        columnaSig = columnaSig + 1 == p.columnas() ? 0           : columnaSig + 1;
    } while(filaSig < p.filas() && p.en(filaSig, columnaSig) != Libre);

    // Continuamos ubicando un sensor vertical.
    {
        Piso q(p);
        ubicarSensor(q, fila, columna, SensorVertical);
        recorrer(q, filaSig, columnaSig, soluciones, iteracion);
    }

    // Continuamos ubicando un sensor horizontal.
    {
        Piso q(p);
        ubicarSensor(q, fila, columna, SensorHorizontal);
        recorrer(q, filaSig, columnaSig, soluciones, iteracion);
    }

    // Continuamos ubicando un sensor cuádruple.
    {
        Piso q(p);
        ubicarSensor(q, fila, columna, SensorCuadruple);
        recorrer(q, filaSig, columnaSig, soluciones, iteracion);
    }

    // Continuamios dejando la posición actual intacta.
    recorrer(p, filaSig, columnaSig, soluciones, iteracion);
}

void resolver(const Piso& p) {
    cout << "Iniciando búsqueda de soluciones para el siguiente piso:" << endl;
    p.imprimir();
    cout << endl;

    vector<Piso> soluciones;
    int iteraciones = 0;
    recorrer(p, 0, 0, soluciones, iteraciones);

    cout << endl;

    if(soluciones.size() == 0) {
        cout << "No se encontraron soluciones." << endl;
    } else {
        cout << "Se encontraron " << soluciones.size() << " soluciones:" << endl;
        for(size_t i = 0; i < soluciones.size(); i++) soluciones[i].imprimir();
    }

    cout << "Iteraciones realizadas: " << iteraciones << endl;
}
