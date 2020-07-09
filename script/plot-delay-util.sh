#! /bin/sh
type=util
file1=learning_${type}.txt
file2=viva_${type}.txt
file3=reno_${type}.txt
file4=cubic_${type}.txt
output=good
gnuplot<<!
set grid
set key top left
set xlabel "index" 
set ylabel "utility"
set xtics -1,1,6
set yrange [0.9:1.0]
set grid
set term "pdf"
set output "${output}-${type}.pdf"
plot "${file1}" u 1:2 title "learningCC" with linespoints lw 2,\
"${file2}" u 1:2 title "Vivace" with linespoints lw 2 ,\
"${file3}" u 1:2 title "Reno" with linespoints lw 2 ,\
"${file4}" u 1:2 title "Cubic" with linespoints lw 2 
set output
exit
!
gnuplot<<!
set grid
set key top left
set xlabel "index" 
set ylabel "utility"
set xtics -1,1,6
set yrange [0.9:1.0]
set grid
set term "png"
set output "${output}-${type}.png"
plot "${file1}" u 1:2 title "learningCC" with linespoints lw 2,\
"${file2}" u 1:2 title "Vivace" with linespoints lw 2 ,\
"${file3}" u 1:2 title "Reno" with linespoints lw 2 ,\
"${file4}" u 1:2 title "Cubic" with linespoints lw 2 
set output
exit
!


type=owd
file1=learning_${type}.txt
file2=viva_${type}.txt
file3=reno_${type}.txt
file4=cubic_${type}.txt
output=good
gnuplot<<!
set grid
set key top left
set xlabel "index" 
set ylabel "delay/ms"
set xtics -1,1,6
set yrange [30:180]
set grid
set term "pdf"
set output "${output}-${type}.pdf"
plot "${file1}" u 1:2 title "learningCC" with linespoints lw 2,\
"${file2}" u 1:2 title "Vivace" with linespoints lw 2 ,\
"${file3}" u 1:2 title "Reno" with linespoints lw 2 ,\
"${file4}" u 1:2 title "Cubic" with linespoints lw 2 
set output
exit
!

gnuplot<<!
set grid
set key top left
set xlabel "index" 
set ylabel "delay/ms"
set xtics -1,1,6
set yrange [30:180]
set grid
set term "png"
set output "${output}-${type}.png"
plot "${file1}" u 1:2 title "learningCC" with linespoints lw 2,\
"${file2}" u 1:2 title "Vivace" with linespoints lw 2 ,\
"${file3}" u 1:2 title "Reno" with linespoints lw 2 ,\
"${file4}" u 1:2 title "Cubic" with linespoints lw 2 
set output
exit
!


