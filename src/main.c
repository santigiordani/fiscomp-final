#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <mtran.h>
#include <modelo.h>


// Funciones de prueba
void test_sweep();
//void test_gr();

/*
    En un futuro (cercano) me gustaría armar un modulo a parte con las pruebas
*/

/*
    En un futuro (cercano) me gustaría delegar la responsabilidad de reservar
    espacio al modelo_init (lo que conlleva hacer un modelo_free).

    Esto haría las cosas mas lentas, pero facilitaría mucho laburar con las
    simulaciones. Ademas, debería agregar checks para ver si ptr == NULL
*/


int main() {

    /*
        Parámetros del modelo
    */
    double T = 1.5;         // Temperatura (adimensionalizada)
    int L = 16;             // Lado de la matriz de partículas
    int m = 16;             // Cantidad de cadenas para Rhat
    int n = 128;            // Longitud de las cadenas para Rhat

    // Test
    test_sweep;//(T, L, m, n);

    return 0;

}

/**
 * @brief Creamos varios modelos y los inicializamos con gr_init.
 * 
 * @param T Temperatura (adimensionalizada)
 * @param L Lado de la matriz de partículas
 * @param m Cantidad de cadenas para Rhat
 * @param n Longitud de cadenas para Rhat
 *
void test_gr(double T, int L, int m, int n) {

    printf(" [TEST] Reservamos espacio.");
    
    // Reservamos espacio para los modelos
    short **mats = (short **) malloc(m * sizeof(short *));
    for (int i = 0; i < m; ++i) mats[i] = (short *) malloc(L * L * sizeof(short));

    printf(" [TEST] Creamos los modelos.");

    // Inicializamos los modelos
    modelo mods[m];
    for (int i = 0; i < m; ++i) {
        mods[i] = {T, L, mats[i]};
    }



}*/

/**
 * @brief Creamos un modelo, lo inicializamos y hacemos varios sweeps, mostrando
 *        los estados generados en pantalla.
 * 
 * @note  Hay que cambiar el tamaño del modelo de n a L
 *        Hay que cambiar el nombre del modelo de m a mod
 */
void test_sweep() {

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
        modelo_gsweep(&m);
        modelo_print(&m);

        // Mostramos el buffer por .1 segundos
        fflush(stdout);
        sleep(.1);

        // Movemos el cursor arriba a la derecha
        printf("\033[H");
    }

}