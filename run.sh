#!/bin/sh

OMP_NUM_THREADS=`lscpu | awk '/^Core\(s/{Cores=$4}/^Socket\(s/{Socket=$2}END{print Cores*Socket}'`
HOST=`hostname`-${OMP_NUM_THREADS}th
G=`expr $OMP_NUM_THREADS - 1`

for cxx in g++ icpx; do
	\rm mX_real.hpp.gch
	\rm sample.exe
	CXX=$cxx make sample.exe
	cp sample.exe sample.exe-$cxx
done

for cxx in g++ icpx; do
	OPT=$cxx
	i=0
	for MX in SW PA DWS DWA QTW TWS TWA QQW QWS QWA; do
		numactl --physcpubind=0-$G ./sample.exe-$cxx $i < IN | tee log-$MX-$OPT-$HOST
		i=`expr $i + 1`
	done
done

