#include <cmath>

#include "gtest/gtest.h"
#include "medir.h"
#include "problema3.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Funciones para generar tamaños crecientes de pisos, a partir de una        //
// secuencia de enteros 1, ..., n.                                            //
////////////////////////////////////////////////////////////////////////////////

unsigned filas(int n) {
    if(n == 1) return 1;
    if(n % 2 == 0) return filas(n - 1);
    return (n + 1) / 2;
}

unsigned columnas(int n) {
    if(n == 1) return 1;
    if(n % 2 != 0) return filas(n - 1);
    return (n + 1) / 2;
}

string indice(int n) {
    return to_string(filas(n)) + "x" + to_string(columnas(n));
}

////////////////////////////////////////////////////////////////////////////////
// Tests                                                                      //
////////////////////////////////////////////////////////////////////////////////

TEST(problema3, PerformancePeorCaso) {
    medir(string("perf/problema3/peor-caso.csv"),
          10,
          10,
          [&] (int n) {
              // Peor caso: todas las posiciones están libres.
              Piso p(filas(n), columnas(n));
              for(unsigned i = 0; i < filas(n); i++)
                  for(unsigned j = 0; j < columnas(n); j++)
                    p.set(i, j, Libre);
              return p;
          },
          [&] (const Piso& piso) { resolver(piso); },
          indice);
}

TEST(problema3, PerformanceMejorCaso) {
    medir(string("perf/problema3/mejor-caso.csv"),
          500,
          20,
          [&] (int n) {
              // Mejor caso: todas las posiciones son pared.
              Piso p(filas(n), columnas(n));
              for(unsigned i = 0; i < filas(n); i++)
                  for(unsigned j = 0; j < columnas(n); j++)
                      p.set(i, j, Pared);
              return p;
          },
          [&] (const Piso& piso) { resolver(piso); },
          indice);
}

TEST(problema3, PerformanceCasoPromedio) {
    medir(string("perf/problema3/caso-promedio.csv"),
          10,
          10,
          [&] (int n) {
              // Creo un piso *siempre* de 10x10, donde todas las posiciones son pared.
              Piso p(10, 10);
              for(unsigned i = 0; i < p.filas(); i++)
                  for(unsigned j = 0; j < p.columnas(); j++)
                      p.set(i, j, Pared);

              // Libero n posiciones al azar.
              for(int k = 0; k < n; k++) {

                  // Me aseguro de no estar marcando como libre una posición que ya
                  // liberé antes, para asegurarme de tener n posiciones libres.
                  unsigned i, j;
                  do {
                      i = rand() % 10;
                      j = rand() % 10;
                  } while(p.get(i, j) == Libre);
                  p.set(i, j, Libre);
              }

              return p;
          },
          [&] (const Piso& piso) { resolver(piso); },
          indice);
}

GTEST_API_ int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}