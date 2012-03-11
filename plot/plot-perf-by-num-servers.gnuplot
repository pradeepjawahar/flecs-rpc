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

set title "C1->S1, Get:Append=1:9"
set output "g-a-perf-1." . FILEEXT
plot "/home/hobin/work/flecs-rpc-log/p1/client1A-sec" using ($5 + 0.0):3 with points linecolor rgb "blue" title "Append", \
"/home/hobin/work/flecs-rpc-log/p1/client1G-sec" using ($5 + 0.0):3 with points linecolor rgb "red" title "Get"

set title "C1->S1->S2, Get:Append=1:9"
set output "g-a-perf-2." . FILEEXT
plot "/home/hobin/work/flecs-rpc-log/p1/client2A-sec" using ($5 + 0.0):3 with points linecolor rgb "blue" title "Append", \
"/home/hobin/work/flecs-rpc-log/p1/client2G-sec" using ($5 + 0.0):3 with points linecolor rgb "red" title "Get"

# used (), since curly bracket {} doesn't work.
set title "C1->S1->(S2, S3), Get:Append=1:9"
set output "g-a-perf-3." . FILEEXT
plot "/home/hobin/work/flecs-rpc-log/p1/client3A-sec" using ($5 + 0.0):3 with points linecolor rgb "blue" title "Append", \
"/home/hobin/work/flecs-rpc-log/p1/client3G-sec" using ($5 + 0.0):3 with points linecolor rgb "red" title "Get"

set title "C1->S1->(S2, S3, S4), Get:Append=1:9"
set output "g-a-perf-4." . FILEEXT
plot "/home/hobin/work/flecs-rpc-log/p1/client4A-sec" using ($5 + 0.0):3 with points linecolor rgb "blue" title "Append", \
"/home/hobin/work/flecs-rpc-log/p1/client4G-sec" using ($5 + 0.0):3 with points linecolor rgb "red" title "Get"
