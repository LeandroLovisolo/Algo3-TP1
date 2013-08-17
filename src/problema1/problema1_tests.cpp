#include "gtest/gtest.h"
#include "problema1.h"

TEST(problema1, CeroPaquetes) {
	EXPECT_EQ(0, problema1(1, std::vector<int>()));
}

TEST(problema1, UnSoloPaquete) {
	std::vector<int> paquetes;
	paquetes.push_back(1);
	EXPECT_EQ(1, problema1(1, paquetes));
}

GTEST_API_ int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}