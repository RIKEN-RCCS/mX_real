#!/bin/bash

OMP_NUM_THREADS=4
HOST=`hostname`-${OMP_NUM_THREADS}th
G=`expr $OMP_NUM_THREADS - 1`

numactl --physcpubind=0-$G ./sample.exe 0 < IN | tee log-SW-$HOST
numactl --physcpubind=0-$G ./sample.exe 1 < IN | tee log-PA-$HOST
numactl --physcpubind=0-$G ./sample.exe 2 < IN | tee log-DW-$HOST
numactl --physcpubind=0-$G ./sample.exe 3 < IN | tee log-QTW-$HOST
numactl --physcpubind=0-$G ./sample.exe 4 < IN | tee log-TW-$HOST
numactl --physcpubind=0-$G ./sample.exe 5 < IN | tee log-QQW-$HOST
numactl --physcpubind=0-$G ./sample.exe 6 < IN | tee log-QW-$HOST

