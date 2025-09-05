#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <modelo.h>
#include <gelman-rubin.h>


/**
 * @note Para trabajar un poco mas cómodos, definimos un tipo booleano.
 */
typedef enum { FALSE = 0, TRUE = 1 } boolean;


double gr_get_Rhat(double **data, int m, int n) {

    // Por si el arreglo dinámico no funciona
    printf(" [DEBUG] Estamos por reservar espacio para las medias.");

    // Reservamos espacio para las medias
    double medias[m], media_global = 0;

    // Por si el arreglo dinámico no funciona
    printf(" [DEBUG] Reservamos espacio para las medias exitosamente.");
    
    // Calculamos las medias
    for (int i = 0; i < m; ++i) {

        // Calculamos las medias de cada cadena
        double sum = 0;
        for (int j = 0; j < n; j++) sum += data[i][j];
        medias[i] = sum / n;

        media_global += medias[i];

    }
    media_global /= m;

    // Calculamos la "between-chain variance"
    double B = 0, diff = 0;
    for (int i = 0; i < m; ++i) {
        diff = medias[i] - media_global;
        B += diff * diff;
    }
    B *= (double) n / (m - 1);

    // Calculamos la "within-chain variance"
    double W = 0;
    for (int i = 0; i < m; ++i) {
        double var = 0;
        for (int j = 0; j < n; ++j) {
            double diff = data[i][j] - medias[i];
            var += diff * diff;
        }
        var /= (n - 1);
        W += var;
    }
    W /= m;

    // Calculamos el estadístico final
    double Vhat = ((double) (n - 1) / n) * W + (1.0 / n) * B;
    double Rhat = sqrt(Vhat / W);

    // Devolvemos el R-hat
    return Rhat;

}

void rg_init(modelo *modelos, int m, void (*sweep)(modelo *), observable o, int n, double tol) {

    /*
        Para calcular Rhat necesitamos almacenar una matriz de mn datls.
        Usualmente m <= 32 y n <= 1000. Para estos tamaños, reservar espacio en
        el stack o el heap no hace demasiada diferencia.

        Lo implemente en el heap, pero estoy pensando en pasarlo al stack.
        Debería cambiar tanto esta función como rg_get_Rhat.
    */

    // Reservamos espacio para los observables
    double *data[m];
    for (int i = 0; i < m; i++) {
        data[i] = malloc(m * sizeof(double));
    }

    // Loop de burn in
    do {

        // Muestreamos n observables de cada una de las m cadenas
        for (int j = 0; j < n; ++j) {
            for (int i = 0; i < m; ++i) {

                // Avanzamos cada modelo por un sweep
                sweep(&modelos[i]);

                // Muestreamos y calculamos el observable
                data[i][j] = o(&modelos[i]);

            }
        }

    } while (gr_get_Rhat(data, m, n) < tol);

    // Liberamos el espacio reservado
    for (int i = 0; i < m; i++) free(data[i]);

}
