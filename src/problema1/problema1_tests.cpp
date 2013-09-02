#include <ctime>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <climits>


#include "gtest/gtest.h"
#include "problema1.h"

using namespace std;

TEST(problema1, CeroPaquetes) {
    vector<int> solucion = problema1(100, vector<int>());

    EXPECT_EQ(0, solucion.size());
}

TEST(problema1, UnSoloPaquete) {
    vector<int> paquetes;
    paquetes.push_back(100);

    vector<int> solucion = problema1(100, paquetes);

    EXPECT_EQ(1,   solucion.size());
    EXPECT_EQ(100, solucion[0]);
}

TEST(problema1, DosPaquetesUnCamion) {
    vector<int> paquetes;
    paquetes.push_back(80);
    paquetes.push_back(20);

    vector<int> solucion = problema1(100, paquetes);

    EXPECT_EQ(1,   solucion.size());
    EXPECT_EQ(100, solucion[0]);
}

TEST(problema1, DosPaquetesDosCamiones) {
    vector<int> paquetes;
    paquetes.push_back(80);
    paquetes.push_back(40);

    vector<int> solucion = problema1(100, paquetes);

    EXPECT_EQ(2,  solucion.size());
    EXPECT_EQ(80, solucion[0]);
    EXPECT_EQ(40, solucion[1]);
}

TEST(problema1, SeisPaquetesTresCamiones) {
    vector<int> paquetes;
    paquetes.push_back(80);
    paquetes.push_back(40);
    paquetes.push_back(20);
    paquetes.push_back(30);
    paquetes.push_back(15);
    paquetes.push_back(80);

    vector<int> solucion = problema1(100, paquetes);

    EXPECT_EQ(3,  solucion.size());
    EXPECT_EQ(95, solucion[0]);
    EXPECT_EQ(90, solucion[1]);
    EXPECT_EQ(80, solucion[2]);
}

double performance(int iteraciones, int limite, vector<int> paquetes) {
    struct timespec st,en;
    unsigned long res = UINT_MAX;
    for (int i = 0; i < iteraciones; ++i) {
        clock_gettime(CLOCK_THREAD_CPUTIME_ID ,&st);
        problema1(limite, paquetes);
        clock_gettime(CLOCK_THREAD_CPUTIME_ID ,&en);
        unsigned long diff = en.tv_nsec - st.tv_nsec;
        res = min(res,diff);
    }
    return res;
}

TEST(problema1, PerformancePeorCaso) {
    ofstream fs("perf/problema1/peor-caso.csv");
    vector<int> paquetes;
    int limite = 1000;

    for (int i = 0; i <= 1000; i++){
        cout << "Iteración " << i << endl;
        paquetes.push_back(limite - i*0.5);
        if(i % 10 == 0)
            fs << i << ", " << performance(20, limite, paquetes) << endl;
    }

    fs.close();
}


vector<int>* paquetesAleatorios(const int limite_max, const int cantidad_paquetes) {
    //srand(time(NULL));
    //int cant_paquetes = (rand() % cantidad_max);
    vector<int>* paquetes = new vector<int>;
    for(int j=0;j<cantidad_paquetes;j++) {
        srand(time(NULL));
        paquetes->push_back(rand() % limite_max);
    }
    return paquetes;
}

GTEST_API_ int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}