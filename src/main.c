#include <tests.h>


int main() {

    /*
        Parámetros del modelo

        En un futuro (cercano) quiero poder leer estos parametros de un archivo
        de configuración, o que el usuario los entre por consola. Ahora mismo,
        si queremos hacer pruebas con parámetros diferentes tenemos que
        - Cambiar los parámetros aca abajo
        - Cambiar filename (sino el nuevo test sobreescribiría los resultados
          de tests anteriores)
        - Ir a src/tests/test.c y cambiar, en cas de ser necesario, el algoritmo
          que se usa para generar las muestras
        - Recompilar el programa
    */

    double T = 2.6;                     // Temperatura (adimensionalizada)
    int L = 16;                         // Lado de la matriz de partículas
    int m = 16;                       // Cantidad de cadenas para Rhat
    int n = 1024;                       // Longitud de las cadenas para Rhat
    double tol = 1.05;                  // Tolerancia para el Rhat
    int n_data = 1024;                  // Número de muestras para calcular tau_int
    int n_mod = 1024 * 16;                     // Número de modelos
    char *filename = "data/mdata.txt";

    // Test
    test(T, L, m, n, tol, n_data, n_mod, filename);

    return 0;

}