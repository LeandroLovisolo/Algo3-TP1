#include "gtest/gtest.h"
#include "problema3.h"

TEST(problema3, en) {
	Piso p(1, 2);
	p.en(0, 0) = Pared;

	EXPECT_EQ(Pared, p.en(0, 0));
	EXPECT_EQ(Libre, p.en(0, 1));
}

TEST(problema3, UnaSolaCasilla) {
	Piso p(1, 1);
	Piso *p2 = problema3(p);
	EXPECT_EQ(SensorDoble, p2->en(0, 0));
	delete p2;
}

GTEST_API_ int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}