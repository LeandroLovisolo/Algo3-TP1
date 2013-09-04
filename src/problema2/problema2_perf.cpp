#include "gtest/gtest.h"
#include "medir.h"
#include "problema2.h"

using namespace std;

TEST(problema2, PerformancePeorCaso) {
    vector<Curso> cursos;

    medir(string("perf/problema2/peor-caso.csv"),
          1000,
          20,
          [&] (int n) {
              if((unsigned) n > cursos.size()) cursos.push_back(Curso(1000 - n, 1000 - n, n));
              return cursos;
          },
          [&] (const vector<Curso>& cursos) { problema2(cursos); }
          );
}

TEST(problema2, PerformanceMejorCaso) {
    vector<Curso> cursos;

    medir(string("perf/problema2/mejor-caso.csv"),
          1000,
          20,
          [&] (int n) {
              if((unsigned) n > cursos.size()) cursos.push_back(Curso(n, n, n));
              return cursos;
          },
          [&] (const vector<Curso>& cursos) { problema2(cursos); }
          );
}

vector<Curso> cursosAleatorios(int cantidad) {
    vector<Curso> cursos;
    for(int i = 0; i < cantidad; i++) {
        int fecha1 = rand() % 1000;
        int fecha2 = rand() % 1000;
        cursos.push_back(Curso(min(fecha1, fecha2), max(fecha1, fecha2), i));
    }
    return cursos;
}

TEST(problema2, PerformanceCasoPromedio) {
    vector<Curso> cursos;
    int veces_que_se_uso = 0;

    medir(string("perf/problema2/caso-promedio.csv"),
          1000,
          100,
          [&] (int n) {
              veces_que_se_uso++;
              if((unsigned) n > cursos.size() || veces_que_se_uso > 10) {
                  cursos = cursosAleatorios(n);
                  veces_que_se_uso = 1;
              }
              return cursos;
          },
          [&] (const vector<Curso>& cursos) { problema2(cursos); }
          );
}

GTEST_API_ int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}