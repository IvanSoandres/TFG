set title "Conexiones activas en tiempo real"
set xlabel "Tiempo (s)"
set ylabel "Número de conexiones"
set grid
set term x11 persist
plot "connections.dat" using ($1 - firstcolumn):2 with lines title "Conexiones activas"
set terminal png size 800,700
set output "ddos100_asus_hardened.png"
replot
