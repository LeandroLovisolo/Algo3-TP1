#include "gtest/gtest.h"
#include "problema1.h"
#include <ctime>
#include <iostream>
#include <fstream>
using namespace std;

TEST(problema1, CeroPaquetes) {
	std::vector<pair<int,int> > *res = problema1(100, std::vector<int>());
	EXPECT_EQ(0,res->size());
	delete res;
}

TEST(problema1, UnSoloPaquete) {
	std::vector<int> paquetes;
	paquetes.push_back(100);
	std::vector<pair<int,int> > *res = problema1(100, paquetes);
	EXPECT_EQ(1, res->size());
	EXPECT_EQ(1, (*res)[0].first);
	EXPECT_EQ(100, (*res)[0].second);
	delete res;
}

TEST(problema1, PaqueteAgregaNuevoCamion) {
	std::vector<int> paquetes;
	paquetes.push_back(80);
	paquetes.push_back(40);
	std::vector<pair<int,int> > *res = problema1(100, paquetes);
	EXPECT_EQ(2, res->size());
	EXPECT_EQ(1, (*res)[0].first);
	EXPECT_EQ(80, (*res)[0].second);
	EXPECT_EQ(2, (*res)[1].first);
	EXPECT_EQ(40, (*res)[1].second);
	delete res;
}

TEST(problema1, PaqueteSeAgregaAlMismoCamion) {
	std::vector<int> paquetes;
	paquetes.push_back(80);
	paquetes.push_back(20);
	std::vector<pair<int,int> > *res = problema1(100, paquetes);
	EXPECT_EQ(1, res->size());
	EXPECT_EQ(1, (*res)[0].first);
	EXPECT_EQ(100,(*res)[0].second);
	delete res;
}

TEST(problema1, AgregoKVecesELMismoPaquete) {
	for (int k = 0; k < 1000; k++){
		std::vector<int> paquetes;
		for (int i = 0; i < k; ++i) {
				paquetes.push_back(100);
		}
		std::vector<pair<int,int> > *res = problema1(100, paquetes);
		EXPECT_EQ(k, res->size());
		for (int i = 0; i < res->size(); ++i) {
			EXPECT_EQ(i+1, (*res)[i].first);
			EXPECT_EQ(100,(*res)[i].second);
		}
		delete res;
	}
}

TEST(problema1, PaquetesSeAgreganADistintosCamiones) {
	std::vector<int> paquetes;
	paquetes.push_back(80);
	paquetes.push_back(40);
	paquetes.push_back(20);
	paquetes.push_back(30);
	paquetes.push_back(15);
	paquetes.push_back(80);
	std::vector<pair<int,int> > *res = problema1(100, paquetes);
	EXPECT_EQ(3, res->size());
	EXPECT_EQ(1, (*res)[0].first);
	EXPECT_EQ(95,(*res)[0].second);
	EXPECT_EQ(2, (*res)[1].first);
	EXPECT_EQ(90,(*res)[1].second);
	EXPECT_EQ(3, (*res)[2].first);
	EXPECT_EQ(80,(*res)[2].second);
	delete res;	
}

double performance(int iteraciones, int limite, vector<int> paquetes) {
	clock_t start = clock();
	double time = 0;
	for (int i = 0; i < iteraciones; ++i) {
		// Ejecuto el problema y libero la memoria del vector devuelto.
		delete problema1(limite, paquetes);
		clock_t end = clock();
		time += (double) (end-start) / CLOCKS_PER_SEC * 1000;
	}
	return time/iteraciones;
}


TEST(problema1, PerformancePeorCaso) {
	ofstream fs("perf/problema1/peor-caso.csv");
	int limite = 1;
	std::vector<int> paquetes;
	for (int i = 0; i <= 1000; i++){
		paquetes.push_back(limite);
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
