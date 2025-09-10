#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <tests.h>
#include <modelo.h>
#include <gelman-rubin.h>
#include <autocorr.h>


void test(double T, int L, int m, int n, double tol, int n_data, int n_mod, char *filename) {

    // Presentamos el test
    printf(" -------------------------------------------------------------------------------\n");
    printf("                                Prueba general\n");
    printf(" -------------------------------------------------------------------------------\n");
    printf("\n");
    printf(" Inicializamos %6d modelos hasta que el Rhat de Gelman-Rubin esté por debajo\n", n_mod);
    printf(" de %6.3f. Luego estimamos tau_int y tau_exp para cada modelo. Guardamos los\n", tol);
    printf(" resultados en %s.", filename);
    printf("\n");

    /*
        Quisiera leer la configuración de un archivo, pero eso lo dejo para
        mas tarde
    */

    // Verificamos que n_mod sea múltiplo de m
    int resto;
    if ((resto = n_mod % m)) {
        fprintf(stderr, " \033[33m[WARNING]\033[37m n_mod = %6d no es divisible por m = %6d.\n", n_mod, m);
        fprintf(stderr, " \033[33m[WARNING]\033[37m Lo cambiamos por n_mod = %6d\n", n_mod += m - resto);
    }

    // Mostramos en pantalla los parámetros
    printf("                            Parámetro: valor\n");
    printf("                          Temperatura: %f\n", T);
    printf("                       Long de matriz: %d\n", L);
    printf("                  Modelos por gr_init: %d\n", m);
    printf("                  Buffer para gr_init: %d\n", n);
    printf("                Tolerancia de gr_init: %f\n", tol);
    printf("                      Buffer para tau: %d\n", n_data);
    printf("                    Número de modelos: %d\n", n_mod);
    printf("\n\n");

    // Inicializamos los modelos
    modelo *mods[n_mod];
    for (int i = 0; i < n_mod; ++i) mods[i] = modelo_init(T, L);

    // Inicializamos con Gelman-Rubin
    for (int i = 0; i < n_mod; i += m) {
        gr_init(&mods[i], m, modelo_msweep, modelo_get_m, n, tol);
        printf("\n");
    }

    // Calculamos tau_int y tau_exp
    for (int i = 0; i < n_mod; ++i) {

        // Llenamos un buffer con data del modelo
        double data[n_data];
        for (int j = 0; j < n_data; ++j) {
            modelo_gsweep(mods[i]); // .................................................... <- Acá para cambiar el tipo de sweep!
            data[j] = modelo_get_m(mods[i]);
        }

        // Calculamos los tau
        get_tau(data, n_data, &(mods[i]->mon.tau_int), &(mods[i]->mon.tau_exp), &(mods[i]->mon.autocorr_nn_bufsize));
        mods[i]->mon.autocorr_bufsize = n_data;

        /*
            Poner acá alguna animación de que estamos calculando los datos
        */

    }

    // Guardamos los resultados
    FILE *f = fopen(filename, "w");
    for (int i = 0; i < n_mod; ++i) {
        fprintf(f, "%d %d %d %f %d %d %f %f\n",
            mods[i]->mon.chain_num,
            mods[i]->mon.chain_size,
            mods[i]->mon.gr_iter,
            mods[i]->mon.Rhat,
            mods[i]->mon.autocorr_bufsize,
            mods[i]->mon.autocorr_nn_bufsize,
            mods[i]->mon.tau_int,
            mods[i]->mon.tau_exp
        );
    }
    fclose(f);

    // Liberamos espacio
    for (int i = 0; i < n_mod; ++i) modelo_free(mods[i]);

}


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
    int autocorr_nn_bufsize;
    get_tau(data, n_data, &tau_int, &tau_exp, &autocorr_nn_bufsize);

    // Guardamos datos de diagnóstico
    mod->mon.autocorr_bufsize = n_data;
    mod->mon.autocorr_nn_bufsize = autocorr_nn_bufsize;
    mod->mon.tau_int = tau_int;
    mod->mon.tau_exp = tau_exp;

    printf(" [TEST] El tiempo integrado de autocorrelación es %f.\n", tau_int);
    printf(" [TEST] El tiempo exponencial de autocorrelación es %f.\n", tau_exp);

    // Mostramos algunos datos de monitoreo
    printf(" [TEST] Algunos datos de monitoreo:\n");
    printf(" [TEST]               chain_num: %d\n", mod->mon.chain_num);
    printf(" [TEST]              chain_size: %d\n", mod->mon.chain_size);
    printf(" [TEST]                 gr_iter: %d\n", mod->mon.gr_iter);
    printf(" [TEST]                    Rhat: %f\n", mod->mon.Rhat);
    printf(" [TEST]\n");
    printf(" [TEST]        autocorr_bufsize: %d\n", mod->mon.autocorr_bufsize);
    printf(" [TEST]     autocorr_nn_bufsize: %d\n", mod->mon.autocorr_nn_bufsize);
    printf(" [TEST]                 tau_int: %f\n", mod->mon.tau_int);
    printf(" [TEST]                 tau_exp: %f\n", mod->mon.tau_exp);

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