#ifndef AUTOCORR_H
#define AUTOCORR_H


/**
 * @brief Calculamos el tiempo de autocorrelación de un arreglo de n datos
 * 
 * @note  Calculamos sumando A(k) para k desde 1 hasta lag_max. Para tener
 *        una buena estimación, n debe ser bastante mayor que lag_max, que 
 *        a su vez debe ser bastante mayor que tau_int. Por ejemplo, si
 *        tau_int es 10, valores razonables son lag_max = 100 y n = 1000.
 * 
 * @param data Arreglo de datos
 * @param n Longigud de data
 * @param lag_max Máximo valor de k para el cual se calcula A(k)
 */
double get_tau_int(const double *data, int n, int lag_max);


/**
 * @brief Calculamos el tiempo de autocorrelación de un arreglo de n datos
 * 
 * @note  Es una versión adaptativa de get_tau_int()
 * 
 * @param data Arreglo de datos
 * @param n Longitud de data
 */
double get_tau_int_adapt(const double *data, int n);


/**
 * @brief Calculamos el tiemo de autocorrelación exponencial
 * 
 * @note  Es un indicador útil para el burn-in
 * 
 * @param data Arreglo de datos
 * @param n Longitud de data
 */
double get_tau_exp(const double *data, int n);


/**
 * @brief  Calculamos un arreglo de autocorrelaciones en función de un arreglo
 *         de datos y devolvemos la parte positiva de la autocorrelación.
 * 
 * @param  data Arreglo de datos
 * @param  n Longitud de data
 * @param  autocorr Puntero al arreglo de resultados
 * 
 * @return Longitud de autocorr
 */
int get_autocorr(const double *data, int n, double *autocorr);


#endif // AUTOCORR_H