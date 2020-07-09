#! /bin/sh
file1=learning_cubic_fair.txt
file2=viva_cubic_fair.txt
output=ratio-cubic
gnuplot<<!
set grid
set key bottom left
set xlabel "index" 
set ylabel "throughput ratio"
set xtics -1,1,6
set yrange [0.0:1.8]
set term "pdf"
set output "${output}.pdf"
plot "${file1}" u 1:3 title "learningCC" with linespoints lw 2,\
"${file2}" u 1:3 title "Vivace" with linespoints lw 2
set output
exit
!

gnuplot<<!
set grid
set key bottom left
set xlabel "index" 
set ylabel "throughput ratio"
set xtics -1,1,6
set yrange [0.0:1.8]
set term "png"
set output "${output}.png"
plot "${file1}" u 1:3 title "learningCC" with linespoints lw 2,\
"${file2}" u 1:3 title "Vivace" with linespoints lw 2
set output
exit
!

