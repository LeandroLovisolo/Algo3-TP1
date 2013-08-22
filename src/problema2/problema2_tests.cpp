#include "gtest/gtest.h"
#include "problema2.h"

TEST(problema2, CeroCursos) {
	std::vector<Curso> *cursos = problema2(std::vector<Curso>());
	EXPECT_EQ(0, cursos->size());
	delete cursos;
}

TEST(problema2, UnCurso) {
	std::vector<Curso> cursos;
	cursos.push_back(Curso(1, 2));

	std::vector<Curso> *max_cursos = problema2(cursos);
	EXPECT_EQ(1, max_cursos->size());
	delete max_cursos;
}

GTEST_API_ int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}