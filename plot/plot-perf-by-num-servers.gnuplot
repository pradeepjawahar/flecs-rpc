# Tested with gnuplot 4.4 patchlevel 3

set terminal pngcairo enhanced size 1200,600
FILEEXT = "png"

#set terminal pdfcairo enhanced dashed size 5.00in, 3.00in
#FILEEXT = "pdf"

set grid ytics lc rgb "#B0B0B0"

set key under nobox

set border back 
set grid front
set xlabel "Time (HH:MM:SS)"
set ylabel "Get / Append time (microsecond)"

set xdata time
set timefmt "%H:%M:%S"
set format x "%H:%M:%S"

set decimal locale
set format y "%'g"

#set samples 1000
#set pointsize 0.2

set logscale y

# used (), since curly bracket {} doesn't work.
set title "C1->S1->(S2, S3), Get:Append=1:9"
set output "get-append-perf." . FILEEXT
plot "/dev/shm/flecs-log/clientA" using ($4 + 0.0):3 with points linecolor rgb "blue" title "Append", \
"/dev/shm/flecs-log/clientG" using ($4 + 0.0):3 with points linecolor rgb "red" title "Get"
