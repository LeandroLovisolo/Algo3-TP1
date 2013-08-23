#include "gtest/gtest.h"
#include "problema1.h"
using namespace std;

TEST(problema1, CeroPaquetes) {
	std::vector<pair<int,int> > *res = problema1(100, std::vector<int>())
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

GTEST_API_ int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}