#ifndef MONITOR_H
#define MONITOR_H


/**
 * @struct monitor_data
 * @brief  Bloque para monitorear los tests estadísticos de un modelo.
 * 
 * @note   Guardamos los bufsize de autocorrelación porque nos indican si el
 *         estimador esta estimando con precisión suficiente.
 */
typedef struct {

    // Información relacionada con el Rhat Gelman-Rubin
    int chain_num;              // Número de cadenas
    int chain_size;             // Tamaño de las cadenas
    int gr_iter;                // Número de iteraciones
    double Rhat;                // Rhat

    // Información relacionada con los tau
    int autocorr_bufsize;       // Tamaño del buffer para calcular autocorr
    int autocorr_nn_bufsize;    // Tamaño del buffer no negativo para autocorr
    double tau_int;             // Tiempo integrado de autocorrelación
    double tau_exp;             // Tiempo exponencial de autocorrelación

} monitor_data;


#endif // MONITOR_H