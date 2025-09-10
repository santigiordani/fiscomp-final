#include <tests.h>


int main() {

    /*
        Parámetros del modelo

        En un futuro (cercano) quiero poder leer estos parametros de un archivo
        de configuración, o que el usuario los entre por consola.
    */

    double T = 2.5;                     // Temperatura (adimensionalizada)
    int L = 16;                         // Lado de la matriz de partículas
    int m = 8;                          // Cantidad de cadenas para Rhat
    int n = 1024;                       // Longitud de las cadenas para Rhat
    double tol = 1.05;                  // Tolerancia para el Rhat
    int n_data = 1024;                  // Número de muestras para calcular tau_int
    int n_mod = 4000;                   // Número de modelos
    char *filename = "data/gtest.txt";

    // Test
    test(T, L, m, n, tol, n_data, n_mod, filename);

    /*
        T = 2.5
        L = 16
        m = 8
        n = 1024
        tol = 1.05
        n_data = 1024
        n_mod = 800
        Con estos parámetros, Gibbs tardó   3m2.205s
                                            3m5.306s
                                            3m16.233s

        Con estos parámetros, Metrópolis    3m20.290s
                                            3m0.828s
                                            3m2.078s
    */

    return 0;

}