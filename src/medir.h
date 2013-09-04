#ifndef MEDIR_H
#define MEDIR_H

#include <iostream>
#include <fstream>
#include <climits>

template <typename generar_func,
          typename resolver_func>
void medir(std::string archivo_csv,
           unsigned maximo_problema,
           unsigned mediciones_por_problema,
           generar_func generar,
           resolver_func resolver,
           std::string (*indice)(int i) = std::to_string) {

    std::ofstream fs(archivo_csv);

    for(int i = 1; i <= maximo_problema; i++) {
        std::cout << "Midiendo: problema de tamaño " << indice(i)
                  << " (" << i << "/" << maximo_problema << ")...\r" << std::flush;

        auto problema = generar(i);

        // unsigned long tiempo = UINT_MAX;
        // for(int j = 0; j < mediciones_por_problema; j++) {
        //     struct timespec inicio, fin;
        //     clock_gettime(CLOCK_THREAD_CPUTIME_ID, &inicio);
        //     resolver(problema);
        //     clock_gettime(CLOCK_THREAD_CPUTIME_ID, &fin);
        //     tiempo = min(tiempo, (unsigned long)(fin.tv_nsec - inicio.tv_nsec));
        // }

        // double tiempo = 0;
        // for(int j = 0; j < mediciones_por_problema; j++) {
        //     clock_t inicio = clock();
        //     resolver(problema);
        //     tiempo += (clock() - inicio);
        // }
        // tiempo /= mediciones_por_problema;

        clock_t tiempo = UINT_MAX;
        for(int j = 0; j < mediciones_por_problema; j++) {
            clock_t inicio = clock();
            resolver(problema);
            tiempo = std::min(tiempo, clock() - inicio);
        }

        double milisegundos = tiempo * 1000.0 / CLOCKS_PER_SEC;

        fs << indice(i) << ", " << milisegundos << std::endl;
    }

    fs.close();

    std::cout << '\r' << std::flush;
}

#endif