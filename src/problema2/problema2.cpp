#include "problema2.h"
#include <algorithm>

bool cursos_comp_fecha_finalizacion_chica(Curso curso_uno, Curso curso_dos) {
	return curso_uno._final < curso_dos._final;
}

//Remuevo cursos constantes para poder ordenarlos con sort.
std::vector<Curso> *problema2(std::vector<Curso> &cursos) {
	//sorteo por fecha de finalización O(n log n)
	std::sort(cursos.begin(), cursos.end(), cursos_comp_fecha_finalizacion_chica);
	return new std::vector<Curso>;
}