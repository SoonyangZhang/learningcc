#! /bin/sh
file1=learning_fair.txt
file2=viva_fair.txt
file3=reno_fair.txt
file4=cubic_fair.txt
output=fairness
gnuplot<<!
set key top left
set grid
set xlabel "index" 
set ylabel "Jain's fairness index"
set xtics -1,1,6
set yrange [0.99:1.01]
set term "pdf"
set output "${output}.pdf"
plot "${file1}" u 1:2 title "learningCC" with linespoints lw 2,\
"${file2}" u 1:2 title "Vivace" with linespoints lw 2,\
"${file3}" u 1:2 title "Reno" with linespoints lw 2,\
"${file4}" u 1:2 title "Cubic" with linespoints lw 2
set output
exit
!


gnuplot<<!
set key top left
set grid
set xlabel "index" 
set ylabel "Jain's fairness index"
set xtics -1,1,6
set yrange [0.99:1.01]
set term "png"
set output "${output}.png"
plot "${file1}" u 1:2 title "learningCC" with linespoints lw 2,\
"${file2}" u 1:2 title "Vivace" with linespoints lw 2,\
"${file3}" u 1:2 title "Reno" with linespoints lw 2,\
"${file4}" u 1:2 title "Cubic" with linespoints lw 2
set output
exit
!
