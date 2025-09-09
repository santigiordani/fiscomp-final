set terminal "pdfcairo"

set output "data.pdf"
plot "data.txt" with linespoints

set output "autocorr.pdf"
plot "autocorr.txt" with linespoints

set output "log_autocorr.pdf"
plot "log_autocorr.txt" with linespoints
