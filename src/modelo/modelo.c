#include <stdio.h>
#include <math.h>

#include <mtran.h>
#include <modelo.h>


void modelo_init(modelo *m) {
    for (int i = 0; i < m->n; ++i) {
        for (int j = 0; j < m->n; ++j) {
            *(m->mat + m->n * i + j) = (short) ((mtran() > .5) ? 1 : -1);
        }
    }
}

void modelo_print(modelo *m) {
    printf("Temperatura: %f\n", m->T);
    printf("Tamaño:      %d\n", m->n);
    printf("Espines:\n");
    for (int i = 0; i < m->n; ++i) {
        for (int j = 0; j < m->n; ++j) {
            printf("%s ", (*(m->mat + i * m->n + j) == 1) ? "##" : "  ");
        }
        printf("\n");
    }
}

void modelo_mstep(modelo *m) {

    // Seleccionamos un par de índices
    int i = (int) (m->n * mtran());
    int j = (int) (m->n * mtran());
    
    // Calculamos la diferencia de energía
    double dE = (double) 2 * *(m->mat + m->n * i + j) * (
        *(m->mat + m->n * i + ((j + 1) % m->n)) +
        *(m->mat + m->n * i + ((j + m->n - 1) % m->n)) +
        *(m->mat + m->n * ((i + 1) % m->n) + j) +
        *(m->mat + m->n * ((i + m->n - 1) % m->n) + j)
    );

    // Hacemos o no el flip
    if (- m->T * log(mtran()) > dE) *(m->mat + m->n * i + j) *= -1;

}

void modelo_gstep(modelo *m) {
    
    // Seleccionamos un par de índices
    int i = (int) (m->n * mtran());
    int j = (int) (m->n * mtran());
    
    // Calculamos la suma de los espines vecinos
    double h = (double) *(m->mat + m->n * i + ((j + 1) % m->n)) +
                        *(m->mat + m->n * i + ((j + m->n - 1) % m->n)) +
                        *(m->mat + m->n * ((i + 1) % m->n) + j) +
                        *(m->mat + m->n * ((i + m->n - 1) % m->n) + j);

    // Gibbs: probabilidad de que el nuevo spin sea +1
    double proba = 1.0 / (1.0 + exp(-2.0 * h / m->T));

    // Muestreamos
    *(m->mat + m->n * i + j) = (mtran() < proba) ? +1 : -1;

}

void modelo_msweep(modelo *m) {
    for (int n2 = m->n * m->n, i = 0; i < n2; ++i) modelo_mstep(m);
}

void modelo_gsweep(modelo *m) {
    for (int n2 = m->n * m->n, i = 0; i < n2; ++i) modelo_gstep(m);
}

double modelo_get_m(modelo *m) {

    int sum = 0;
    for (int i = 0; i < m->n; ++i) {
        for (int j = 0; j < m->n; ++j) {
            sum += (int) *(m->mat + m->n * i + j);
        }
    }

    return (double) sum;

}

double modelo_get_E(modelo *m) {

    int sum = 0;
    for (int i = 0; i < m->n; ++i) {
        for (int j = 0; j < m->n; ++j) {

            // Sumamos solo los vecinos de abajo y de la derecha
            sum += (int) *(m->mat + m->n * i + j) * (
                *(m->mat + m->n * ((i + 1) % m->n) + j) + 
                *(m->mat + m->n * i + ((j + 1) % m->n))
            );

        }
    } 

    return - (double) sum;

}