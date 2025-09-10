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
 */
static void linear_regression(const double *data, int n, double *a, double *b);

/**
 * @brief  Calculamos la autocorrelación A(k) de un arreglo de datos
 *
 * @param  data Arreglo de datos
 * @param  n Longitud de data
 * @param  autocorr Arreglo donde guardar los resultados
 * 
 * @return Longitud del subarreglo de autocorr que contiene entradas positivas
 */
static int get_autocorr(const double *data, int n, double *autocorr);


void get_tau(const double *data, int n, double *tau_int, double *tau_exp, int *m) {
    
    // Calculamos la autocorrelación
    double autocorr[n];
    *m = get_autocorr(data, n, autocorr);

    // Calculamos tau_int
    *tau_int = 0.5;
    for (int i = 0; i < *m; ++i) *tau_int += autocorr[i];

    // Tomamos lograitmo de las autocorrelaciones
    double log_autocorr[*m];
    for (int i = 0; i < *m; ++i) log_autocorr[i] = log(autocorr[i]);

    // Hacemos regresión lineal (log A(k) = c - k/tau_exp)
    double a = 0, b = 0;
    linear_regression(log_autocorr, *m, &a, &b);

    // Guardamos tau_exp = -1 / a
    *tau_exp = - 1 / a;

}

static int get_autocorr(const double *data, int n, double *autocorr) {

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

    // Calculamos autocorrelaciones
    for (int k = 0; k < n; ++k) {

        double sum = 0.0;
        for (int t = 0; t < n - k; t++) {
            sum += (data[t] - media) * (data[t + k] - media);
        }
        autocorr[k] = sum / var;

    }

    // Buscamos el primer índice negativo y lo devolvemos
    int m = 0;
    for (int i = 0; i < n; ++i) {
        if (autocorr[i] <= 0) {
            m = i;
            break;
        }
    }

    // En caso de que m sea bastante grande
    if (m >= n / 2) fprintf(stderr, " [WARNING] %d de %d términos son no negativos en la autocorrelación. Recomendamos aumentar n.", m, n);

    return m;

}

static void linear_regression(const double *data, int n, double *a, double *b) {

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

}