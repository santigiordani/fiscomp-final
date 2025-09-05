#ifndef MODELO_H
#define MODELO_H


#include "mtran.h"


/**
 * @struct modelo
 * @brief  Estado del modelo de Ising
 * 
 * @note   El usuario es responsable de reservar espacio en memoria para la
 *         la matriz del modelo. Esto es por dos razones: para evitar memory
 *         leaks, y para que el usuario pueda ubicar el modelo en el stack,
 *         quedando posiblemente en el cache
 * 
 * @note   Un uso típico del modelo sería
 * 
 *         // Definimos parámetros
 *         double T = 1.1;
 *         int n = 10;
 *         
 *         // Reservamos espacio
 *         short mat[n * n];
 *  
 *         // Creamos el modelo
 *         modelo m = {T, n, mat};
 * 
 *         // Inicializamos el modelo
 *         modelo_init(&m);
 */
typedef struct {
    double T;           // Temperatura (adimensionalizada)
    int n;              // Tamaño de la matriz
    short *mat;         // Matriz de espines
} modelo;

/**
 * @typedef observable
 * @brief   Tipo de dato para una función que toma un modelo y calcula un
 *          valor sin efectos secundarios.
 */
typedef double (*observable)(const modelo *);


/**
 * @brief Inicializamos el modelo de Ising con espines aleatorios
 * 
 * @note  Usamos mtran, que tiene estado static y no es thread-safe
 * @param m Puntero al modelo
 */
void modelo_init(modelo *m);

/**
 * @brief Imprimimos el modelo en pantalla
 * 
 * @param m Puntero al modelo
 */
void modelo_print(modelo *m);
 
/**
 * @brief Cambiamos el estado del modelo según el algoritmo de Metrópolis
 * 
 * @note  Usamos mtran, que tiene estado static y no es thread-safe
 * @param m Puntero al modelo
 */
void modelo_mstep(modelo *m);

/**
 * @brief Cambiamos el estado del modelo según el Gibbs sampler
 * 
 * @note  Usamos mtran, que tiene estado static y no es thread-safe
 * @param m Puntero al modelo
 */
void modelo_gstep(modelo *m);

 /**
 * @brief Hacemos un sweep del modelo según el algoritmo de Metrópolis
 * 
 * @note  Usamos mtran, que tiene estado static y no es ghread-safe
 * @param m Puntero al modelo
 */
void modelo_msweep(modelo *m);

/**
 * @brief Hacemos un sweep del modelo según el Gibbs sampler
 * 
 * @note  Usamos mtran, que tiene estado static y no es ghread-safe
 * @param m Puntero al modelo
 */
void modelo_gsweep(modelo *m);

/**
 * @brief Calculamos la magnetización promedio del sistema
 * 
 * @param m Puntero al modelo
 */
double modelo_get_m(const modelo *m);

/**
 * @brief Calculamos la energía del sistema
 * 
 * @param m Puntero al modelo
 */
double modelo_get_E(const modelo *m);


#endif // MODELO_H