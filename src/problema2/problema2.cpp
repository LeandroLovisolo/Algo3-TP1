#include <algorithm>

#include "problema2.h"

using namespace std;

vector<Curso> problema2(vector<Curso> cursos) {
    // Caso trivial.
    if(cursos.size() < 2) {
        return cursos;
    } 

    // Ordenamos los cursos por fecha de finalización en forma creciente.
    sort(cursos.begin(), cursos.end(), [] (Curso c, Curso d) { return fin(c) < fin(d); });

    // Solución inicial. Incluye el curso que termina primero.
    vector<Curso> solucion;
    solucion.push_back(cursos[0]);

    for(size_t i = 1; i < cursos.size(); i++) {
        // Si la fecha de inicio del curso actual es mayor que
        // la fecha de finalización del último curso agregado,
        // lo incluimos en la solución.
        
        int inicio_curso_actual       = inicio(cursos[i]);
        int fin_ultimo_curso_agregado = fin(solucion[solucion.size() - 1]);

        if(inicio_curso_actual > fin_ultimo_curso_agregado) {
            solucion.push_back(cursos[i]);
        }
    }

    return solucion;
}