#! /bin/sh
cc=reno
cc1=learning
param=theta
file1=${cc}_${cc1}_1_fair.txt
file3=${cc}_${cc1}_3_fair.txt
file6=${cc}_${cc1}_6_fair.txt
output=${param}-${cc}-ratio-3-6
gnuplot<<!
set grid
set key top left
set xlabel '{/Symbol q}' enhanced
set ylabel "throughput ratio"
set xtics 0.5,0.1,0.9
set yrange [0.3:2.0]
set grid
set term "pdf"
set output "${output}.pdf"
plot "${file1}" u 1:3 title "Case 1" with linespoints lw 2,\
"${file3}" u 1:3 title "Case 3" with linespoints lw 2,\
"${file6}" u 1:3 title "Case 6" with linespoints lw 2 
set output
exit
!
