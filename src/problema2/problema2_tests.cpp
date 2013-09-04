#include <ctime>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <climits>

#include "gtest/gtest.h"
#include "problema2.h"

using namespace std;

TEST(problema2, CeroCursos) {
    vector<Curso> solucion = problema2(vector<Curso>());

    EXPECT_EQ(0, solucion.size());
}

TEST(problema2, UnCurso) {
    vector<Curso> cursos;
    cursos.push_back(Curso(1, 2, 0));

    vector<Curso> solucion = problema2(cursos);
    EXPECT_EQ(1, solucion.size());
    EXPECT_EQ(0, indice(solucion[0]));
}

TEST(problema2, SolucionUnica) {
    vector<Curso> cursos;
    cursos.push_back(Curso(4, 4, 1));
    cursos.push_back(Curso(1, 3, 2));
    cursos.push_back(Curso(2, 5, 3));

    vector<Curso> solucion = problema2(cursos);
    EXPECT_EQ(2, solucion.size());
    EXPECT_EQ(2, indice(solucion[0]));
    EXPECT_EQ(1, indice(solucion[1]));
}

TEST(problema2, SolucionMultiple) {
    vector<Curso> cursos;
    cursos.push_back(Curso(4, 10, 1));
    cursos.push_back(Curso(5, 6,  2));
    cursos.push_back(Curso(2, 3,  3));
    cursos.push_back(Curso(7, 8,  4));
    cursos.push_back(Curso(3, 4,  5));
    cursos.push_back(Curso(1, 15, 6));

    vector<Curso> solucion = problema2(cursos);

    bool solucion1 = indice(solucion[0]) == 3 &&
                     indice(solucion[1]) == 2 &&
                     indice(solucion[2]) == 4;

    bool solucion2 = indice(solucion[0]) == 5 &&
                     indice(solucion[1]) == 2 &&
                     indice(solucion[2]) == 4;

    EXPECT_EQ(3, solucion.size());
    EXPECT_TRUE(solucion1 || solucion2);
}

double performance(int iteraciones, vector<Curso> cursos) {
    struct timespec st,en;
    unsigned long res = UINT_MAX;
    for (int i = 0; i < iteraciones; ++i) {
        clock_gettime(CLOCK_THREAD_CPUTIME_ID ,&st);
        problema2(cursos);
        clock_gettime(CLOCK_THREAD_CPUTIME_ID ,&en);
        unsigned long diff = en.tv_nsec - st.tv_nsec;
        res = min(res,diff);
    }
    return res;
}

GTEST_API_ int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}