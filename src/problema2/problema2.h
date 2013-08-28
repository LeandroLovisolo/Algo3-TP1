#ifndef PROBLEMA2_H
#define PROBLEMA2_H

#include <tuple>
#include <vector>

// Campos: <fecha inicial, fecha final, índice>
typedef std::tuple<int, int, int> Curso;

// Macros para acceder a los campos de la tupla Curso
#define inicio(c) get<0>(c)
#define fin(c)    get<1>(c)
#define indice(c) get<2>(c)

std::vector<Curso> problema2(const std::vector<Curso> cursos);

#endif // PROBLEMA2_H