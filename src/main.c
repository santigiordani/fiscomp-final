#include <stdio.h>

#include <mtran.h>
#include <modelo.h>



int main() {

    printf(" [TEST] Creando un modelo\n");

    // Definimos parámetros y reservamos espacio
    double T = 1.1; int n = 16; short mat[n * n];

    // Creamos el modelo
    modelo m = {T, n, mat};

    printf(" [TEST] Inicializamos el modelo\n");

    // Inicializamos el modelo
    modelo_init(&m);

    // Imprimimos el modelo en pantalla
    modelo_print(&m);

    return 0;

}