#ifndef AUTOCORR_H
#define AUTOCORR_H


/**
 * @brief Calculamos los tiempos de autocorrelación integrado y exponencial
 * 
 * @param data Arreglo de datos
 * @param n Longitud de data
 * @param tau_int Puntero para guardar tau_int
 * @param tau_exp Puntero para guardar tau_exp
 * @param m Puntero para guardar la longitud de autocorr
 */
void get_tau(const double *data, int n, double *tau_int, double *tau_exp, int *m);


#endif // AUTOCORR_H