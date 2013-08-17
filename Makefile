###############################################################################
# Flags                                                                       #
###############################################################################

# Directorio de la la librería googletest
GTEST_DIR = lib/gtest-1.6.0

# Flags del preprocesador C++.
CPPFLAGS += -I$(GTEST_DIR) -I$(GTEST_DIR)/include

# Flags del compilador C++.
CXXFLAGS += -g -Wall -Wextra

# Comando para generar código objeto (.o)
OBJ       = $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c

# Comando para generar los binarios de las soluciones
BIN_MAIN  = $(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

# Comando para generar los binarios de los tests
BIN_TEST  = $(BIN_MAIN) -lpthread

# Dependencias extra para los binarios de los tests
TEST_DEPS = gtest-all.o

# Binarios generados
BINS      = problema1 problema1_tests \
            problema2 problema2_tests \
            problema3 problema3_tests

###############################################################################
# Targets generales                                                           #
###############################################################################

all: $(BINS)

clean:
	rm -f *.o $(BINS)

gtest-all.o:
	$(OBJ) $(GTEST_DIR)/src/gtest-all.cc

###############################################################################
# Problema 1                                                                  #
###############################################################################

problema1: problema1.o problema1_main.o
	$(BIN_MAIN)

problema1.o: src/problema1/problema1.cpp src/problema1/problema1.h
	$(OBJ) src/problema1/problema1.cpp

problema1_main.o: src/problema1/problema1.h src/problema1/problema1_main.cpp
	$(OBJ) src/problema1/problema1_main.cpp

problema1_tests: $(TEST_DEPS) problema1.o problema1_tests.o
	$(BIN_TEST)

problema1_tests.o: src/problema1/problema1.h src/problema1/problema1_tests.cpp
	$(OBJ) src/problema1/problema1_tests.cpp

###############################################################################
# Problema 2                                                                  #
###############################################################################

problema2: problema2.o problema2_main.o
	$(BIN_MAIN)

problema2.o: src/problema2/problema2.cpp src/problema2/problema2.h
	$(OBJ) src/problema2/problema2.cpp

problema2_main.o: src/problema2/problema2.h src/problema2/problema2_main.cpp
	$(OBJ) src/problema2/problema2_main.cpp

problema2_tests: $(TEST_DEPS) problema2.o problema2_tests.o
	$(BIN_TEST)

problema2_tests.o: src/problema2/problema2.h src/problema2/problema2_tests.cpp
	$(OBJ) src/problema2/problema2_tests.cpp

###############################################################################
# Problema 3                                                                  #
###############################################################################

problema3: problema3.o problema3_main.o
	$(BIN_MAIN)

problema3.o: src/problema3/problema3.cpp src/problema3/problema3.h
	$(OBJ) src/problema3/problema3.cpp

problema3_main.o: src/problema3/problema3.h src/problema3/problema3_main.cpp
	$(OBJ) src/problema3/problema3_main.cpp

problema3_tests: $(TEST_DEPS) problema3.o problema3_tests.o
	$(BIN_TEST)

problema3_tests.o: src/problema3/problema3.h src/problema3/problema3_tests.cpp
	$(OBJ) src/problema3/problema3_tests.cpp