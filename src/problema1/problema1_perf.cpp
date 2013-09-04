#include "gtest/gtest.h"
#include "medir.h"
#include "problema1.h"

using namespace std;

TEST(problema1, PerformancePeorCaso) {
    int limite = 1000;
    vector<int> paquetes;

    medir(string("perf/problema1/peor-caso.csv"),
          1000,
          20,
          [&] (int n) {
              if((unsigned) n > paquetes.size()) paquetes.push_back(limite - n * 0.5);
              return paquetes;
          },
          [&] (const vector<int>& paquetes) { problema1(limite, paquetes); }
          );
}

TEST(problema1, PerformanceMejorCaso) {
    int limite = 1000;
    vector<int> paquetes;

    medir(string("perf/problema1/mejor-caso.csv"),
          1000,
          20,
          [&] (int n) {
              if((unsigned) n > paquetes.size()) paquetes.push_back(1);
              return paquetes;
          },
          [&] (const vector<int>& paquetes) { problema1(limite, paquetes); }
          );
}

vector<int> paquetesAleatorios(int cantidad, int peso_maximo) {
    vector<int> paquetes;
    for(int i = 0; i < cantidad; i++) {
        paquetes.push_back(rand() % peso_maximo);
    }
    return paquetes;
}

TEST(problema1, PerformanceCasoPromedio) {
    int limite = 1000;

    medir(string("perf/problema1/caso-promedio.csv"),
          1000,
          20,
          [&] (int n) { return paquetesAleatorios(limite, n); },
          [&] (const vector<int>& paquetes) { problema1(limite, paquetes); }
          );
}

GTEST_API_ int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}