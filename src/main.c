#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <mtran.h>
#include <modelo.h>



int main() {

    printf(" [TEST] Creando un modelo\n");

    // Definimos parámetros y reservamos espacio
    double T = 1.5; int n = 16; short mat[n * n];

    // Creamos el modelo
    modelo m = {T, n, mat};

    printf(" [TEST] Inicializamos el modelo\n");

    // Inicializamos el modelo
    modelo_init(&m);

    // Limpiamos la pantalla (el if es para evitar un Warning unused value)
    if(system("clear")) {}

    // Damos varios pasos con Metrópolis
    for (int i = 0; i < 100000; ++i) {

        // Damos un paso y lo mostramos
        modelo_gstep(&m);
        modelo_print(&m);

        // Mostramos el buffer por .1 segundos
        fflush(stdout);
        sleep(.1);

        // Movemos el cursor arriba a la derecha
        printf("\033[H");
    }

    return 0;

}