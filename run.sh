#!/bin/bash

OMP_NUM_THREADS=4
HOST=`hostname`-${OMP_NUM_THREADS}th
G=`expr $OMP_NUM_THREADS - 1`

for cxx in g++ icpx; do
make clean; CXX=$cxx make sample.exe
cp sample.exe sample.exe-$cxx
done

for cxx in g++ icpx; do
OPT=$cxx
i=0
for MX in SW PA DW QTW TW QQW QW; do
numactl --physcpubind=0-$G ./sample.exe-$cxx $i < IN | tee log-$MX-$OPT-$HOST
i=`expr $i + 1`
done
done

