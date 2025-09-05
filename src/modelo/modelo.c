#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <mtran.h>
#include <modelo.h>


modelo *modelo_init(double T, int L) {

    // Reservamos espacio para la matriz de espines
    short *mat = (short *) malloc(L * L * sizeof(short));
    if (mat == NULL) {
        fprintf(stderr, " [ERROR] No pudimos reservar espacio para la matriz de espines.");
        return NULL;
    }

    // Reservamos espacio para el modelo
    modelo *mod = (modelo *) malloc(sizeof(modelo));
    if (mod == NULL) {
        fprintf(stderr, " [ERROR] No pudimos reservar espacio para el modelo.");
        return NULL;
    }

    // Inicializamos los parametros
    mod->T = T;
    mod->L = L;
    mod->mat = mat;
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            *(mat + L * i + j) = (short) ((mtran() > .5) ? 1 : -1);
        }
    }

    // Devolvemos el modelo
    return mod;

}

void modelo_free(modelo *mod) {
    free(mod->mat);
    free(mod);
}

void modelo_print(modelo *mod) {
    printf("Temperatura: %f\n", mod->T);
    printf("Tamaño:      %d\n", mod->L);
    printf("Espines:\n");
    for (int i = 0; i < mod->L; ++i) {
        for (int j = 0; j < mod->L; ++j) {
            printf("%s ", (*(mod->mat + i * mod->L + j) == 1) ? "##" : "  ");
        }
        printf("\n");
    }
}

void modelo_mstep(modelo *mod) {

    // Seleccionamos un par de índices
    int i = (int) (mod->L * mtran());
    int j = (int) (mod->L * mtran());
    
    // Calculamos la diferencia de energía
    double dE = (double) 2 * *(mod->mat + mod->L * i + j) * (
        *(mod->mat + mod->L * i + ((j + 1) % mod->L)) +
        *(mod->mat + mod->L * i + ((j + mod->L - 1) % mod->L)) +
        *(mod->mat + mod->L * ((i + 1) % mod->L) + j) +
        *(mod->mat + mod->L * ((i + mod->L - 1) % mod->L) + j)
    );

    // Hacemos o no el flip
    if (- mod->T * log(mtran()) > dE) *(mod->mat + mod->L * i + j) *= -1;

}

void modelo_gstep(modelo *mod) {
    
    // Seleccionamos un par de índices
    int i = (int) (mod->L * mtran());
    int j = (int) (mod->L * mtran());
    
    // Calculamos la suma de los espines vecinos
    double h = (double) *(mod->mat + mod->L * i + ((j + 1) % mod->L)) +
                        *(mod->mat + mod->L * i + ((j + mod->L - 1) % mod->L)) +
                        *(mod->mat + mod->L * ((i + 1) % mod->L) + j) +
                        *(mod->mat + mod->L * ((i + mod->L - 1) % mod->L) + j);

    // Gibbs: probabilidad de que el nuevo spin sea +1
    double proba = 1.0 / (1.0 + exp(-2.0 * h / mod->T));

    // Muestreamos
    *(mod->mat + mod->L * i + j) = (mtran() < proba) ? 1 : -1;

}

void modelo_msweep(modelo *mod) {
    for (int L2 = mod->L * mod->L, i = 0; i < L2; ++i) modelo_mstep(mod);
}

void modelo_gsweep(modelo *mod) {
    for (int L2 = mod->L * mod->L, i = 0; i < L2; ++i) modelo_gstep(mod);
}

double modelo_get_m(const modelo *mod) {

    int sum = 0;
    for (int i = 0; i < mod->L; ++i) {
        for (int j = 0; j < mod->L; ++j) {
            sum += (int) *(mod->mat + mod->L * i + j);
        }
    }

    return (double) sum;

}

double modelo_get_E(const modelo *mod) {

    int sum = 0;
    for (int i = 0; i < mod->L; ++i) {
        for (int j = 0; j < mod->L; ++j) {

            // Sumamos solo los vecinos de abajo y de la derecha
            sum += (int) *(mod->mat + mod->L * i + j) * (
                *(mod->mat + mod->L * ((i + 1) % mod->L) + j) + 
                *(mod->mat + mod->L * i + ((j + 1) % mod->L))
            );

        }
    } 

    return - (double) sum;

}