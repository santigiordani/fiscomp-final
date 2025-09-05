#ifndef MODELO_H
#define MODELO_H


#include "mtran.h"


/**
 * @struct modelo
 * @brief  Estado del modelo de Ising
 */
typedef struct {
    double T;           // Temperatura (adimensionalizada)
    int L;              // Lado de la matriz
    short *mat;         // Matriz de espines
} modelo;

/**
 * @typedef observable
 * @brief   Tipo de dato para una función que toma un modelo y calcula un
 *          valor sin efectos secundarios.
 */
typedef double (*observable)(const modelo *);


/**
 * @brief Inicializamos un modelo de Ising de lado L a temperatura T con
 *        espines aleatorios
 * 
 * @note  Usamos mtran, que tiene estado static y no es thread-safe
 * @param T Temperatura del modelo
 * @param L Lado de la matriz de espines
 */
modelo *modelo_init(double T, int L);

/**
 * @brief Liberamos el espacio en memoria de un modelo de Ising.
 * 
 * @param mod Puntero al modelo
 */
void modelo_free(modelo *mod);

/**
 * @brief Imprimimos el modelo en pantalla
 * 
 * @param m Puntero al modelo
 */
void modelo_print(modelo *mod);
 
/**
 * @brief Cambiamos el estado del modelo según el algoritmo de Metrópolis
 * 
 * @note  Usamos mtran, que tiene estado static y no es thread-safe
 * @param m Puntero al modelo
 */
void modelo_mstep(modelo *mod);

/**
 * @brief Cambiamos el estado del modelo según el Gibbs sampler
 * 
 * @note  Usamos mtran, que tiene estado static y no es thread-safe
 * @param m Puntero al modelo
 */
void modelo_gstep(modelo *mod);

 /**
 * @brief Hacemos un sweep del modelo según el algoritmo de Metrópolis
 * 
 * @note  Usamos mtran, que tiene estado static y no es ghread-safe
 * @param m Puntero al modelo
 */
void modelo_msweep(modelo *mod);

/**
 * @brief Hacemos un sweep del modelo según el Gibbs sampler
 * 
 * @note  Usamos mtran, que tiene estado static y no es ghread-safe
 * @param m Puntero al modelo
 */
void modelo_gsweep(modelo *mod);

/**
 * @brief Calculamos la magnetización promedio del sistema
 * 
 * @param m Puntero al modelo
 */
double modelo_get_m(const modelo *mod);

/**
 * @brief Calculamos la energía del sistema
 * 
 * @param m Puntero al modelo
 */
double modelo_get_E(const modelo *mod);


#endif // MODELO_H