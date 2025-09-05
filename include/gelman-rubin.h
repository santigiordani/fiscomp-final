#ifndef GR_H
#define GR_H


#include <modelo.h>


/**
 * En el futuro, estaría bueno armar una versión multi-observable de estas
 * mismas funciones.
 */


/**
 * @brief  Calculamos el R-hat de Gelman-Rubin
 * 
 * @note   Hay una parte donde reservabamos espacio con malloc, lo cambié por
 *         un arreglo dinámico, en caso de que algo no funcione, por ahí es
 *         eso.
 * 
 * @param  data Observables de las mn muestras
 * @param  m Número de cadenas
 * @param  n Número de muestras por cadena
 * @return Estadístico R-hat de Gelman-Rubin
 */ 
double gr_get_Rhat(double **cadenas, int m, int n);

/**
 * @brief  Generamos muestras con una familia de modelos hasta que el R-hat de
 *         Gelman-Rubin se encuentre por debajo de una tolerancia.
 * 
 * @param  mods Puntero a un arreglo de punteros a modelos (ya inicializados)
 * @param  m Número de modelos en el arreglo
 * @param  sweep Función con la cual actualizar los estados de los modelos
 * @param  o Observable a calcular sobre cada muestra
 * @param  n Número de muestras por cadena para calcular R-hat
 * @param  tol Tolerancia
 */
void gr_init(modelo **mods, int m, void (*sweep)(modelo *), observable o, int n, double tol);


#endif // GR_H