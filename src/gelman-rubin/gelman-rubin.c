#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <modelo.h>
#include <gelman-rubin.h>


#define MAXITER 200 // Máxima cantidad de iteraciones para estabilizar Rhat


double gr_get_Rhat(double **data, int m, int n) {

    // Reservamos espacio para las medias
    double medias[m], media_global = 0;

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

void gr_init(modelo **mods, int m, void (*sweep)(modelo *), observable o, int n, double tol) {

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
        data[i] = malloc(n * sizeof(double));
    }

    // Loop de burn-in
    double Rhat;
    for (int k = 0; k < MAXITER; ++k) {

        // Muestreamos n observables de cada una de las m cadenas
        for (int j = 0; j < n; ++j) {
            for (int i = 0; i < m; ++i) {

                // Avanzamos cada modelo por un sweep
                sweep(mods[i]);

                // Muestreamos y calculamos el observable
                data[i][j] = o(mods[i]);

            }
        }

        // Calculamos Rhat
        if ((Rhat = gr_get_Rhat(data, m, n)) < tol) {

            // Mostramos el resultado final
            printf("\033[1A\033[2K \033[32m[GELMAN-RUBIN]\033[37m Burn-in completo.    Iteración: %3d     R-hat: %.3f.\n", k + 1, Rhat);

            // Guardamos datos
            for (int i = 0; i < m; ++i) {
                mods[i]->mon.chain_num = m;
                mods[i]->mon.chain_size = n;
                mods[i]->mon.gr_iter = k + 1;
                mods[i]->mon.Rhat = Rhat;
            }

            // Terminamos
            return;

        } else {

            // Mostramos el progreso en pantalla
            printf("\033[1A\033[2K \033[33m[GELMAN-RUBIN]\033[37m Burn-in en progreso. Iteración: %3d/%d R-hat: %.3f.\n", k + 1, MAXITER, Rhat);

        }

    }

    // Mensaje de error
    printf("\033[1A\033[2K \033[31m[GELMAN-RUBIN]\033[37m Burn-in incompleto.  Iteración: %3d     R-hat: %.3f.\n", MAXITER, Rhat);

    // Guardamos datos
    for (int i = 0; i < m; ++i) {
        mods[i]->mon.chain_num = m;
        mods[i]->mon.chain_size = n;
        mods[i]->mon.gr_iter = MAXITER;
        mods[i]->mon.Rhat = Rhat;
    }

}
