#include <stdio.h>
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