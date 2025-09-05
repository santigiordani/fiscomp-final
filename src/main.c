#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <mtran.h>
#include <modelo.h>
#include <gelman-rubin.h>


// Funciones de prueba
void test_sweep(double T, int L);
void test_gr();

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
    int m = 16;             // Cantidad de cadenas para Rhat
    int n = 128;            // Longitud de las cadenas para Rhat
    double tol = 1.1023;    // Tolerancia para el Rhat

    // Test
    test_gr(T, L, m, n, tol);

    return 0;

}

/**
 * @brief Creamos varios modelos y los inicializamos con gr_init.
 * 
 * @param T Temperatura (adimensionalizada)
 * @param L Lado de la matriz de partículas
 * @param m Cantidad de cadenas para Rhat
 * @param n Longitud de cadenas para Rhat
 * @param tol Tolerancia para el Rhat (debe ser > 1)
 */
void test_gr(double T, int L, int m, int n, double tol) {

    printf(" [TEST] Creamos los modelos.\n");

    // Creamos varios modelos
    modelo *mods[m];
    for (int i = 0; i < m; ++i) {
        mods[i] = modelo_init(T, L);
    }
    
    printf(" [TEST] Inicializamos los modelos con Gelman-Rubin.\n");

    // Inicializamos los modelos
    gr_init(&mods[0], m, modelo_msweep, modelo_get_m, n, tol);

}

/**
 * @brief Creamos un modelo, lo inicializamos y hacemos varios sweeps, mostrando
 *        los estados generados en pantalla.
 * 
 * @param T Temperatura (adimensionalizada)
 * @param L Lado de la matriz de partículas
 */
void test_sweep(double T, int L) {

    // Creamos un modelo
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