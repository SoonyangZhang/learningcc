#! /bin/sh
instance=1
algo=learning
file1=${instance}_${algo}_1_dur_bw.txt
file2=${instance}_${algo}_2_dur_bw.txt
file3=${instance}_${algo}_3_dur_bw.txt
file4=${instance}_${algo}_4_dur_bw.txt
output=${algo}-${instance}
gnuplot<<!
set grid
set xlabel "time/s" 
set ylabel "rate/kbps"
set xrange [0:400]
set yrange [0:5000]
set term "png"
set output "${output}-sp-recv.png"
plot "${file1}" u 1:2 title "flow1" with lines lw 2 ,\
"${file2}" u 1:2 title "flow2" with lines lw 2 ,\
"${file3}" u 1:2 title "flow3" with lines lw 2 ,\
"${file4}" u 1:2 title "flow4" with lines lw 2
set output
exit
!
gnuplot<<!
set grid
set xlabel "time/s" 
set ylabel "rate/kbps"
set xrange [0:400]
set yrange [0:5000]
set term "pdf"
set output "${output}-sp-recv.pdf"
plot "${file1}" u 1:2 title "flow1" with lines lw 2 ,\
"${file2}" u 1:2 title "flow2" with lines lw 2 ,\
"${file3}" u 1:2 title "flow3" with lines lw 2 ,\
"${file4}" u 1:2 title "flow4" with lines lw 2
set output
exit
!

