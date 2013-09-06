#include "gtest/gtest.h"
#include "problema3.h"

using namespace std;

TEST(problema3, en) {
    Piso p(1, 2);
    p.set(0, 0, Pared);

    EXPECT_EQ(Pared, p.get(0, 0));
    EXPECT_EQ(Libre, p.get(0, 1));
}

TEST(problema3, noTieneSolucion) {
    Piso p(3, 3, "###"
                 "#*#"
                 "###");
    EXPECT_EQ(0, problema3(p).filas());
    EXPECT_EQ(0, problema3(p).columnas());
}

TEST(problema3, tieneSolucionUnica) {
    Piso p(2, 2, "##"
                 "# ");
    EXPECT_EQ(SensorVertical, problema3(p).get(1,1));
}

TEST(problema3, EjemploDelEnuncaido) {
    Piso p(8, 8, "###  *##"
                 "#       "
                 "# * #  #"
                 "# * #  #"
                 "        "
                 "### ####"
                 " #     #"
                 "    *  #");
    problema3(p).imprimir();
}

//Otros tests

TEST(problema3, MiEjemplo) {
    // Piso p(2, 2, " *"
    //              "  ");
    
    // Piso p(2, 3, " * "
    //              "  *");

    // Piso p(3, 3, " # "
    //              "* *"
    //              " # ");

    // Piso p(3, 3, "  *"
    //              "   "
    //              "   ");

    // Piso p(4, 2, "* "
    //              "# "
    //              "* "
    //              "  ");

    // Piso p(4, 4, "### "
    //              "#   "
    //              "# * "
    //              "# * ");

    Piso p(5, 5, "###  "
                 "#    "
                 "# * #"
                 "# * #"
                 "     ");

    // Piso p(6, 6, "###  *"
    //              "#     "
    //              "# * # "
    //              "# * # "
    //              "      "
    //              "### ##");

    // Piso p(6, 7, "###  *#"
    //              "#      "
    //              "# * #  "
    //              "# * #  "
    //              "       "
    //              "### ###");

    // Piso p(7, 7, "###  *#"
    //              "#      "
    //              "# * #  "
    //              "# * #  "
    //              "       "
    //              "### ###"
    //              " # ####");

    // Piso p(7, 7, "###  *#"
    //              "#      "
    //              "# * #  "
    //              "# * #  "
    //              "       "
    //              "### ###"
    //              " #     ");

    // Piso p(7, 8, "###  *##"
    //              "#       "
    //              "# * #  #"
    //              "# * #  #"
    //              "        "
    //              "### ####"
    //              " #     #");

    // Piso p(8, 8, "###  *##"
    //              "#      #"
    //              "# * #  #"
    //              "# * #  #"
    //              "       #"
    //              "### ####"
    //              " #     #"
    //              "####* ##");

    // Piso p(8, 8, "###  *##"
    //              "#       "
    //              "# * #  #"
    //              "# * #  #"
    //              "        "
    //              "### ####"
    //              " #     #"
    //              "    *  #");

    problema3(p).imprimir();
}

GTEST_API_ int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
