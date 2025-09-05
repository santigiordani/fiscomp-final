#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <mtran.h>
#include <modelo.h>


// Funciones de prueba
void test_sweep(double T, int L);
//void test_gr();

/*
    En un futuro (cercano) me gustaría armar un modulo a parte con las pruebas
*/


int main() {

    /*
        Parámetros del modelo

        En un futuro (cercano) quiero poder leer estos parametros de un archivo
        de configuración, o que el usuario los entre por consola.
    */
    double T = 1.5;         // Temperatura (adimensionalizada)
    int L = 16;             // Lado de la matriz de partículas
    //int m = 16;             // Cantidad de cadenas para Rhat
    //int n = 128;            // Longitud de las cadenas para Rhat

    // Test
    test_sweep(T, L);

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
 * @param T Temperatura (adimensionalizada)
 * @param L Lado de la matriz de partículas
 */
void test_sweep(double T, int L) {

    modelo *mod = modelo_init(T, L);

    // Limpiamos la pantalla (el if es para evitar un Warning unused value)
    if(system("clear")) {}

    // Damos varios pasos con Metrópolis
    for (int i = 0; i < 10000; ++i) {

        // Damos un paso y lo mostramos
        modelo_gsweep(mod);
        modelo_print(mod);

        // Mostramos el buffer por .1 segundos
        fflush(stdout);
        sleep(.1);

        // Movemos el cursor arriba a la derecha
        printf("\033[H");
    }

}