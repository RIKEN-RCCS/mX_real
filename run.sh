#!/bin/bash

OMP_NUM_THREADS=4
HOST=`hostname`-${OMP_NUM_THREADS}th
G=`expr $OMP_NUM_THREADS - 1`

for cxx in g++ icpx; do
make clean; CXX=$cxx make

OPT=$cxx

numactl --physcpubind=0-$G ./sample.exe 0 < IN | tee log-SW-$OPT-$HOST
numactl --physcpubind=0-$G ./sample.exe 1 < IN | tee log-PA-$OPT-$HOST
numactl --physcpubind=0-$G ./sample.exe 2 < IN | tee log-DW-$OPT-$HOST
numactl --physcpubind=0-$G ./sample.exe 3 < IN | tee log-QTW-$OPT-$HOST
numactl --physcpubind=0-$G ./sample.exe 4 < IN | tee log-TW-$OPT-$HOST
numactl --physcpubind=0-$G ./sample.exe 5 < IN | tee log-QQW-$OPT-$HOST
numactl --physcpubind=0-$G ./sample.exe 6 < IN | tee log-QW-$OPT-$HOST

done

