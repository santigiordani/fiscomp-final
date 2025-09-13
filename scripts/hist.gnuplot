# Archivo
set terminal pdfcairo size 7, 3
set output "img/hist.pdf"

# Parámetros
file_m = "data/mdata.txt"
file_g = "data/gdata.txt"
col_tau_int = 7
col_tau_exp = 8
col_Rhat = 3

# Hacemos 2 gráficas
set multiplot layout 1, 2

set grid

# Margenes
dx = 0.05
left = 0.08
right = 0.03
top = 0.02
bottom = 0.15
lenx = (1 - left - right - dx) / 2

# Hacer frecuencia relativa.
# Agrandar un poco la letra.
# Unificar rangos

# Rangos
set xrange [0:200]
set yrange [0:0.1]

# Definición de bins
bin_width = 3
bin(x,width) = width * floor(x / width + .5)

# Estilo
set style data histogram
set style histogram clustered gap 1
set boxwidth bin_width
set style fill transparent solid .3 noborder

# Graficamos los histogramas de tau_int
set tmargin at screen 1 - top
set bmargin at screen bottom
set lmargin at screen left
set rmargin at screen left + lenx
set xlabel "Tiempo integrado de autocorrelación"
set ylabel "Probabilidad"
plot file_m using (bin(column(col_tau_int), bin_width)):(.000061) smooth freq with boxes lc rgb "blue" title "Metrópolis", \
     file_g using (bin(column(col_tau_int), bin_width)):(.000061) smooth freq with boxes lc rgb "red"  title "Gibbs"

unset ylabel
set ytics format ""

# Graficamos los histogramas de tau_exp
set lmargin at screen left + lenx + dx
set rmargin at screen left + 2 * lenx + dx
set xlabel "Tiempo exponencial de autocorrelación"
plot file_m using (bin(column(col_tau_exp), bin_width)):(.000061) smooth freq with boxes lc rgb "blue" title "Metrópolis", \
     file_g using (bin(column(col_tau_exp), bin_width)):(.000061) smooth freq with boxes lc rgb "red"  title "Gibbs"