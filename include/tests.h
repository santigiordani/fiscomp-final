#ifndef TESTS_H
#define TESTS_H


/**
 * @brief Inicializamos un modelo y calculamos su tau_int
 * 
 * @note  Ya en este punto vendría bien graficar un par de cosas, pero tengo
 *        que pensar qué exactamente
 * 
 * @param T Temperatura (adimensionalizada)
 * @param L Lado de la matriz de partículas
 * @param m Cantidad de cadenas para Rhat
 * @param n Longitud de cadenas para el Rhat
 * @param tol Tolerancia para el Rhat (debe ser > 1)
 * @param n_data Número de muestras para calcular tau_int
 */
void test_tau(double T, int L, int m, int n, double tol, int n_data);

/**
 * @brief Creamos varios modelos y los inicializamos con gr_init.
 * 
 * @param T Temperatura (adimensionalizada)
 * @param L Lado de la matriz de partículas
 * @param m Cantidad de cadenas para Rhat
 * @param n Longitud de cadenas para Rhat
 * @param tol Tolerancia para el Rhat (debe ser > 1)
 */
void test_gr(double T, int L, int m, int n, double tol);

/**
 * @brief Creamos un modelo, lo inicializamos y hacemos varios sweeps, mostrando
 *        los estados generados en pantalla.
 * 
 * @param T Temperatura (adimensionalizada)
 * @param L Lado de la matriz de partículas
 */
void test_sweep(double T, int L);

/**
 * @brief Guardamos un arreglo en un archivo
 * 
 * @note  En caso de que haya demasiados términos no negativos en A(k), lanzamos
 *        una advertencia, pues esto podría significar que las estimaciones que
 *        se hagan usando autocorr no serán suficientemente precisas
 *
 * @param filename Nombre del archivo
 * @param data Arreglo a guardar
 * @param n Longitud de data
 */
static void save(char *filename, const double *data, int n);

#endif // TESTS_H