#include <stdint.h>
#include <mtran.h>


void mtran_set(mtran_state *state, uint32_t seed) {

    // Puntero al arreglo de estados
    uint32_t *state_array = &(state->state_array[0]);

    // Primer valor del arreglo de estados
    state_array[0] = seed;

    // Llenamos el resto del arreglo de estados
    for (int i = 1; i < MTRAN_N; i++) {
        seed = MTRAN_F * (seed ^ (seed >> (MTRAN_W - 2))) + i;
        state_array[i] = seed;
    }

    // Seteamos el indice de estados
    state->state_index = 0;

}

double mtsran(void *state) {

    // Estado del generador
    mtran_state *s = (mtran_state *) state;

    // Extraemos info del estado
    uint32_t *state_array = &(s->state_array[0]);
    int k = s->state_index;

    // Indice de otro número en el arreglo de estados
    int j = k - (MTRAN_N - 1);
    if (j < 0) j += MTRAN_N;

    // Fusionamos los bits altos y bajos de esos dos números
    uint32_t x = (state_array[k] & MTRAN_UMASK) | (state_array[j] & MTRAN_LMASK);

    // Twist
    uint32_t xA = x >> 1;
    if (x & 1UL) xA ^= MTRAN_A;

    j = k - (MTRAN_N - MTRAN_M);
    if (j < 0) j += MTRAN_N;

    x = state_array[j] ^ xA;
    state_array[k++] = x;

    // Actualizamos el índice de estado
    if (k >= MTRAN_N) k = 0;
    s->state_index = k;

    // Tempering
    uint32_t y = x ^ (x >> MTRAN_U);
    y = y ^ ((y << MTRAN_S) & MTRAN_B);
    y = y ^ ((y << MTRAN_T) & MTRAN_C);
    uint32_t z = y ^ (y >> MTRAN_L);

    // Normalizamos antes de devolver
    return z / 4294967296.0;

}

double mtran() {

    // Creamos e inicializamos un estado
    static int initialized = 0;
    static mtran_state default_state;
    static uint32_t default_seed = 12345;
    if (!initialized) {
        mtran_set(&default_state, default_seed);
        initialized = 1;
    }

    // Generamos un número y lo devolvemos
    return mtsran(&default_state);
    
}