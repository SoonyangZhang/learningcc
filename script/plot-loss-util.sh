#! /bin/sh
loss=l35
file1=learning${loss}_util.txt
file2=viva${loss}_util.txt
file3=reno${loss}_util.txt
file4=cubic${loss}_util.txt
output=${loss}
gnuplot<<!
set key bottom left
set xlabel "index" 
set ylabel "utility"
set xtics -1,1,6
set yrange [0.2:1.0]
set grid
set term "pdf"
set output "${output}-util.pdf"
plot "${file1}" u 1:2 title "learningCC" with linespoints lw 2,\
"${file2}" u 1:2 title "Vivace" with linespoints lw 2,\
"${file3}" u 1:2 title "Reno" with linespoints lw 2,\
"${file4}" u 1:2 title "Cubic" with linespoints lw 2 
set output
exit
!
gnuplot<<!
set key bottom left
set xlabel "index" 
set ylabel "utility"
set xtics -1,1,6
set yrange [0.2:1.0]
set grid
set term "png"
set output "${output}-util.png"
plot "${file1}" u 1:2 title "learningCC" with linespoints lw 2,\
"${file2}" u 1:2 title "Vivace" with linespoints lw 2,\
"${file3}" u 1:2 title "Reno" with linespoints lw 2,\
"${file4}" u 1:2 title "Cubic" with linespoints lw 2 
set output
exit
!

