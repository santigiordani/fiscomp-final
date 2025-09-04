# Trabajo final

El objetivo de este trabajo es explorar algunos métodos de muestreo basados en
_cadenas de Markov_. Concretamente, vamos a comparar el _burn-in_ y el _tiempo_
_de autocorrelación_ del algoritmo de Metrópolis y el Gibbs sampler al generar
muestras del ensamble canónico del _modelo de Ising_.

Para medir el burn-in usamos el $\tau_\text{exp}$ y el estimador $\hat{R}$ de
Gelman-Rubin; y para medir el tiempo de autocorrelación usamos el _running_
_autocorrelation estimator_. Estos estimadores se calculan sobre una función
de la muestra (es decir, sobre un observable del sistema termodinámico). Para
esto elegimos la magnetización promedio, por ser un buen indicador de la 
bimodalidad de la distribución de Boltzman.

# Documentación

Para el trabajo debemos implementar un generador de números aleatorios (para
esto elegimos el Merssenne Twister), el modelo de Ising, los algoritmos de
Metrópolis y Gibbs sampler, y los tests: el tiempo integrado de autocorrelación,
el $\hat{R}$ y un regresor lineal para el tiempo exponencial de autocorrelación.

> [!NOTE]
> Estamos trabajando en esto, aún no implementamos nada.

> **Observación.** Los problemas de sampleo cerca de la región crítica tienen
> que ver mucho con la forma de la distribución que queremos samplear, así que
> ningún método de sampleo debería poder solucionar eso, según Janke al menos.

> **Observación.** Voy a tener que usar estimadores de observables para medir
> cosas como al autocorrelación (no la puedo medir en el espacio de estados
> directamente), existen cosas llamadas _improved estimators_ que son tan solo
> estimadores con menos varianza (lo que todo estadista querría), no creo que
> me ponga a buscar esto para mi trabajo, pero estaría bueno mencioanrlo como
> trabajo futuro.

> **Observación.** Una cita interesante:
> 
> To summarize this section, any realization of a Markov chain Monte Carlo
> update algorithm is characterised by autocorrelation times which enter
> directly into the statistical errors of Monte Carlo estimates. Since
> temporal correlations always increase the statistical errors, it is thus a
> very important issue to develop Monte Carlo update algorithms that keep
> autocorrelation times as small as possible. This is the reason why cluster
> and other non-local algorithms are so important.