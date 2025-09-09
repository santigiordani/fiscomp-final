#include <stdlib.h>
#include <stdio.h>
#include <math.h>


/**
 * @brief  Regresión lineal a datos de la forma (i, data[i])
 * 
 * @param  data Arreglo de datos
 * @param  n Longitud de data
 * @param  a Puntero para guardar el coeficiente principal
 * @param  b Puntero para guardar la ordenada al origan
 * 
 * @return 0 si tiene exito, 1 si no
 */
int linear_regression(const double *data, int n, double *a, double *b);

static void save(char *filename, const double *data, int n) {
    FILE *f = fopen(filename, "w");
    for (int i = 0; i < n; ++i) {
        fprintf(f, "%.4f\n", data[i]);
    }
    fclose(f);
}

double get_tau_int(const double *data, int n, int max_lag) {
    
    // Calculamos la media
    double media = 0;
    for (int i = 0; i < n; ++i) media += data[i];
    media /= (double) n;

    // Calculamos la varianza
    double err, var = 0;
    for (int i = 0; i < n; ++i) {
        err = data[i] - media;
        var += err * err;
    }
    var /= (double) (n - 1);

    // Caso degenerado
    if (var <= 0.0) return 0.5;

    // Sumamos las autocorrelaciones A(k) para k <= max_lag
    double cov, rho, tau = 0.5;
    for (int k = 1; k <= max_lag; ++k) {

        // Calculamos la covarianza
        cov = 0.0;
        for (int t = 0; t < n - k; ++t) {
            cov += (data[t] - media) * (data[t + k] - media);
        }
        cov /= (double) (n - k);

        // Normalizamos la covarianza para obtener la autocorrelación
        rho = cov / var;

        // Si la autocorrelación es negativa salimos
        if (rho <= 0.0) break;

        // Sumamos
        tau += rho;

    }

    return tau;

}

double get_tau_int_adapt(const double *data, int n) {

    // Inicialmente estimamos con lag_max = n / 10
    int lag_max = n / 10;
    double tau = get_tau_int(data, n, lag_max);

    printf(" [DEBUG] Primer tiempo de autocorrelación: %f.\n", tau);

    // A partir de esto, proponemos un nuevo lag_max y recalculamos
    int new_lag = (int) fmin(n / 2, ceil(10 * tau));
    if (new_lag > lag_max) tau = get_tau_int(data, n, new_lag);

    printf(" [DEBUG] Cambiamos el lag_max a %d.\n", new_lag);

    return tau;

}

int get_autocorr(const double *data, int n, double *autocorr) {

    // Calculamos la media
    double media = 0.0;
    for (int i = 0; i < n; ++i) media += data[i];
    media /= n;

    // Calculamos la varianza
    double var = 0.0;
    for (int i = 0; i < n; ++i) {
        double d = data[i] - media;
        var += d * d;
    }

    // En caso de que la serie sea constante
    // if (var == 0.0) {
    //     autocorr[0] = 1.0;
    //    for (int k = 1; k < n; k++) autocorr[k] = 0.0;
    //    return;
    // }

    // Calculamos autocorrelaciones
    for (int k = 0; k < n; ++k) {
        double num = 0.0;
        for (int t = 0; t < n - k; t++) {
            num += (data[t] - media) * (data[t + k] - media);
        }
        autocorr[k] = num / var;
    }

    // Buscamos el primer índice negativo y lo devolvemos
    int m = 0;
    for (int i = 0; i < n; ++i) {
        if (autocorr[i] <= 0) {
            m = i;
            break;
        }
    }

    return m;

}


double get_tau_exp(const double *data, int n) {

    save("data.txt", data, n);

    // Calculamos la autocorrelación
    double autocorr[n];
    int m = get_autocorr(data, n, autocorr);

    save("autocorr.txt", autocorr, m);

    // Tomamos logaritmo de los datos
    double log_autocorr[m];
    for (int i = 0; i < m; ++i) log_autocorr[i] = log(autocorr[i]);

    save("log_autocorr.txt", log_autocorr, m);

    // Hacemos regresión lineal (log A(k) = c - k/tau_exp)
    double a = 0, b = 0;
    if (linear_regression(log_autocorr, m, &a, &b)) {
        fprintf(stderr, " [ERROR] Algo salió mal en la regresión lineal.\n");
        return 0;
    }

    // Devolvemos tau_exp = -1 / a
    return - 1 / a;

}



int linear_regression(const double *data, int n, double *a, double *b) {

    // Calculamos las medias
    double media_x = (n - 1) / 2.0;
    double sum_y = 0.0;
    for (int i = 0; i < n; i++) {
        sum_y += data[i];
    }
    double media_y = sum_y / n;

    // Calculamos numerador y denominador
    double num = 0.0, den = 0.0;
    for (int i = 0; i < n; i++) {
        double dx = i - media_x;
        double dy = data[i] - media_y;
        num += dx * dy;
        den += dx * dx;
    }

    *a = num / den;
    *b = media_y - (*a) * media_x;

    return 0;

}
