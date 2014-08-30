## GNUPLOT command file
set terminal postscript color
set style data lines
set key left bottom
set size 0.78, 1.0
set noxtics
set noytics
set title ''
set ylabel "Miss probability (in %)"
set xlabel "False Alarms probability (in %)"
set grid
set ytics ("0.001" -4.26, "0.01" -3.72, "0.1" -3.08, "0.5" -2.57, "1" -2.32635, "2" -2.05, "5" -1.64, "10" -1.28, "20" -0.84, "30" -0.52, "40" -0.25, "50" 0.0, "80" 0.84)
set xtics ("0.001" -4.26, "0.01" -3.72, "0.1" -3.08, "0.5" -2.57, "1" -2.32635, "2" -2.05, "5" -1.64, "10" -1.28, "20" -0.84, "30" -0.52, "40" -0.25, "50" 0.0, "80" 0.84)
plot [-4.40:0.] [-3.290527:1.0] "./plot/curves/score.txt.plot.DCF" using 3:2 notitle with points pt 7 ps 1 lc rgb "black" lw 2, \
"./plot/curves/score.txt.plot" using 2:1 title "score 1.642 0.994" with lines lc 1 lw 2 lt 1, \
"./plot/curves/score.txt.plot.BestDCF" using 3:2 notitle with points pt 1 ps 1 lc "black" lw 2