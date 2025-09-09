#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <tests.h>
#include <modelo.h>
#include <gelman-rubin.h>
#include <autocorr.h>


void test_tau(double T, int L, int m, int n, double tol, int n_data) {

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

    // Calculamos el tau_int
    double tau_int, tau_exp;
    get_tau(data, n_data, &tau_int, &tau_exp);

    printf(" [TEST] El tiempo integrado de autocorrelación es %f.\n", tau_int);
    printf(" [TEST] El tiempo exponencial de autocorrelación es %f.\n", tau_exp);

    // Liberamos el último modelo
    modelo_free(mod);

}

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

static void save(char *filename, const double *data, int n) {
    
    // Abrimos el archivo
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        fprintf(stderr, " [ERROR] No se pudo abrir el archivo %s.\n", filename);
        return;
    }

    // Guardamos los datos
    for (int i = 0; i < n; ++i) fprintf(f, "%.4f\n", data[i]);

    // Cerramos el archivo
    fclose(f);

}