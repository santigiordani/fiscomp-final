# Trabajo final

El objetivo de este trabajo es explorar el test de Gelman-Rubin para estimar el
tamaño del _burn-in_ en los métodos de muestreo de tipo MCMC. Para ello,
implementamos
- Un generador de números aleatorios (el Mersenne Twister)
- El modelo de Ising
- El algoritmo de Metrópolis para tomar muestras del espacio de estados del
  modelo de Ising
- El Gibbs sampler para tomar muestras del espacio de estados del modelo de
  Ising
- El cálculo del `Rhat` de Gelman-Rubin
- El cálculo del tiempo integrado de autocorrelación
- El cálculo del tiempo exponencial de autocorrelación

La función principal es `test`, que inicializa una cantidad de modelos de
Ising, usando el test de Gelman-Rubin para asegurar que el burn-in ha pasado,
y muestrea estos modelos para calcular los tiempos de autocorrelación.