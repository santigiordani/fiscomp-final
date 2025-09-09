#include <tests.h>


int main() {

    /*
        Parámetros del modelo

        En un futuro (cercano) quiero poder leer estos parametros de un archivo
        de configuración, o que el usuario los entre por consola.
    */
    double T = 2.5;         // Temperatura (adimensionalizada)
    int L = 16;             // Lado de la matriz de partículas
    int m = 8;              // Cantidad de cadenas para Rhat
    int n = 1024;           // Longitud de las cadenas para Rhat
    double tol = 1.05;      // Tolerancia para el Rhat
    int n_data = 1024;      // Número de muestras para calcular tau_int

    // Test
    test_tau(T, L, m, n, tol, n_data);

    return 0;

}