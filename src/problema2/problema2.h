#ifndef PROBLEMA2_H
#define PROBLEMA2_H

#include <utility>
#include <vector>

class Curso {
public:
	int _numero, _inicio, _final;
	Curso(int numero, int inicio, int final) {
		_numero = numero;
		_inicio = inicio;
		_final = final;
	}
};

//typedef std::pair<int, int> Curso;

std::vector<Curso> *problema2(std::vector<Curso> &cursos);

#endif // PROBLEMA2_H