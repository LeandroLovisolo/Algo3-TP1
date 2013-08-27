#include "gtest/gtest.h"
#include "problema3.h"


TEST(problema3, en) {
	Piso p(1, 2);
	p.en(0, 0) = Pared;

	EXPECT_EQ(Pared, p.en(0, 0));
	EXPECT_EQ(Libre, p.en(0, 1));
}

/*
TEST(problema3, UnaSolaCasilla) {
	Piso p(1, 1);
	Piso *p2 = problema3(p);
	EXPECT_EQ(SensorDoble, p2->en(0, 0));
	delete p2;
}
*/

TEST(problema3, okPeroQuedanCasillerosLibres) {
/*
	L 	L 	L
	L 	I 	SC
	L 	SC 	I
*/

	Piso p(3,3);
	// en (0, 0) o (0,1) puede haber un sensorVertical
	p.en(0, 0) = Libre;
	p.en(0, 1) = Libre;
	p.en(0, 2) = Libre;
	p.en(1, 0) = Libre;
	p.en(1, 1) = Importante;
	p.en(1, 2) = SensorCuadruple;
	p.en(2, 0) = Libre;
	p.en(2, 1) = SensorCuadruple;
	p.en(2, 2) = Importante;

	EXPECT_EQ(true, checkPiso(p));

}

TEST(problema3, chocanSensores) {
/*
	L 	L 
	SDV	SC 
*/

	Piso p(2,2);
	
	p.en(0, 0) = Libre;
	p.en(0, 1) = Libre;
	p.en(1, 0) = SensorDobleVertical;
	p.en(1, 1) = SensorCuadruple;

	EXPECT_EQ(false, checkPiso(p));

}

TEST(problema3, todoParedes) {
/*
	P 	P
	P	P 
*/

	Piso p(2,2);
	
	p.en(0, 0) = Pared;
	p.en(0, 1) = Pared;
	p.en(1, 0) = Pared;
	p.en(1, 1) = Pared;

	EXPECT_EQ(false, checkPiso(p));

}

TEST(problema3, todoParedeMenosUnElementoImportante) {
/*
	P 	P
	P	I 
*/

	Piso p(2,2);
	
	p.en(0, 0) = Pared;
	p.en(0, 1) = Pared;
	p.en(1, 0) = Pared;
	p.en(1, 1) = Pared;

	EXPECT_EQ(false, checkPiso(p));

}

TEST(problema3, todoImportante) {
/*
	I 	I
	I	I 
*/

	Piso p(2,2);
	
	p.en(0, 0) = Importante;
	p.en(0, 1) = Importante;
	p.en(1, 0) = Importante;
	p.en(1, 1) = Importante;

	EXPECT_EQ(false, checkPiso(p));

}

TEST(problema3, esSolucion) {
/*
	I 	SC
	SC	I 
*/

	Piso p(2,2);
	
	p.en(0, 0) = Importante;
	p.en(0, 1) = SensorCuadruple;
	p.en(1, 0) = SensorCuadruple;
	p.en(1, 1) = Importante;

	EXPECT_EQ(true, checkPiso(p));

}

TEST(problema3, sensoresNoTraspasanLaPared) {
/*
	L 	SDH	L
	I 	SDH	I
	SC 	P 	SC
*/

	Piso p(3,3);
	
	p.en(0, 0) = Libre;
	p.en(0, 1) = SensorDobleHorizontal;
	p.en(0, 2) = Libre;
	p.en(1, 0) = Importante;
	p.en(1, 1) = SensorDobleHorizontal;
	p.en(1, 2) = Importante;
	p.en(2, 0) = SensorCuadruple;
	p.en(2, 1) = Pared;
	p.en(2, 2) = SensorCuadruple;

	EXPECT_EQ(true, checkPiso(p));

}

// Quedan lugares libres pero no es valida
TEST(problema3, solucionNoValida) {
/*
	L 	L 	L
	I 	P 	I
	SC 	P 	L
*/

	Piso p(3,3);
	
	p.en(0, 0) = Libre;
	p.en(0, 1) = Libre;
	p.en(0, 2) = Libre;
	p.en(1, 0) = Importante;
	p.en(1, 1) = Pared;
	p.en(1, 2) = Importante;
	p.en(2, 0) = SensorCuadruple;
	p.en(2, 1) = Pared;
	p.en(2, 2) = Libre;

	EXPECT_EQ(false, checkPiso(p));

}

// No hay 2 sensores apuntando a los casilleros importantes
TEST(problema3, noApuntan2Sensores) {
/*
	L 	L 	SC
	I 	P 	I
	SC 	L 	L
*/

	Piso p(3,3);
	
	p.en(0, 0) = Libre;
	p.en(0, 1) = Libre;
	p.en(0, 2) = SensorCuadruple;
	p.en(1, 0) = Importante;
	p.en(1, 1) = Pared;
	p.en(1, 2) = Importante;
	p.en(2, 0) = SensorCuadruple;
	p.en(2, 1) = Libre;
	p.en(2, 2) = Libre;

	EXPECT_EQ(false, checkPiso(p));

}

GTEST_API_ int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
