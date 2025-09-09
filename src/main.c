#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <mtran.h>
#include <modelo.h>
#include <gelman-rubin.h>
#include <autocorr.h>


// Funciones de prueba
void test_sweep(double T, int L);
void test_gr(double T, int L, int m, int n, double tol);
void test_tau_int(double T, int L, int m, int n, double tol, int n_data);

/*
    En un futuro (cercano) me gustaría armar un modulo a parte con las pruebas
*/


int main() {

    /*
        Parámetros del modelo

        En un futuro (cercano) quiero poder leer estos parametros de un archivo
        de configuración, o que el usuario los entre por consola.
    */
    double T = 2.5;         // Temperatura (adimensionalizada)
    int L = 16;             // Lado de la matriz de partículas
    int m = 8;              // Cantidad de cadenas para Rhat
    int n = 1024;           // Longitud de las cadenas para Rhat
    double tol = 1.05;      // Tolerancia para el Rhat
    int n_data = 1024;      // Número de muestras para calcular tau_int

    // Test
    test_tau_int(T, L, m, n, tol, n_data);

    return 0;

}

/**
 * @brief Inicializamos un modelo y calculamos su tau_int
 * 
 * @note  Ya en este punto vendría bien graficar un par de cosas, pero tengo
 *        que pensar qué exactamente
 * 
 * @param T Temperatura (adimensionalizada)
 * @param L Lado de la matriz de partículas
 * @param m Cantidad de cadenas para Rhat
 * @param n Longitud de cadenas para el Rhat
 * @param tol Tolerancia para el Rhat (debe ser > 1)
 * @param n_data Número de muestras para calcular tau_int
 */
void test_tau_int(double T, int L, int m, int n, double tol,
                  int n_data) {

    printf(" [TEST] Creamos los modelos.\n");

    // Creamos varios modelos
    modelo *mods[m];
    for (int i = 0; i < m; ++i) {
        mods[i] = modelo_init(T, L);
    }
    
    printf(" [TEST] Inicializamos los modelos con Gelman-Rubin.\n");

    // Inicializamos los modelos
    gr_init(&mods[0], m, modelo_msweep, modelo_get_m, n, tol);

    // Nos quedamos con el primero únicamente
    modelo *mod = mods[0];

    // Liberamos el espacio de los demas
    for (int i = 1; i < m; ++i) modelo_free(mods[i]);

    // Llenamos un buffer con data del modelo
    double data[n_data];
    for (int i = 0; i < n_data; ++i) {
        modelo_msweep(mod);
        data[i] = modelo_get_m(mod);
    }

    printf(" [TEST] Tomamos una muestra de tamaño %d.\n", n_data);

    /*
        Sería mas útil esta parte cambiarla por un guardado en algún archivo,
        así podemos graficarlo después.
    */

    // Mostramos en pantalla la data
    for (int i = 0; i < n_data; ++i) {
        
        // Salto de linea
        if (!(i % 10)) printf("\n [TEST] ");

        // Mostramos un dato
        printf("%6.2f  ", data[i]);

    }
    printf("\n");

    // Calculamos el tau_int
    double tau_int = get_tau_int_adapt(data, n_data);

    printf(" [TEST] El tiempo integrado de autocorrelación es %f.\n", tau_int);

    // Calculamos el tau_exp
    double tau_exp = get_tau_exp(data, n_data);

    printf(" [TEST] El tiempo exponencial de autocorrelación es %f.\n", tau_exp);

    // Liberamos el último modelo
    modelo_free(mod);

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

    // Liberamos el espacio
    for (int i = 0; i < m; ++i) modelo_free(mods[i]);

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