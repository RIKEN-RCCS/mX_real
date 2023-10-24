CXX = g++ --std=c++14
#CXX = g++ --std=c++17
CCFLAGS := $(CCFLAGS) -O3 -g
#CCFLAGS := $(CCFLAGS) -g
LDFLAGS = -g -fopenmp

# Optimizations
CCFLAGS := $(CCFLAGS) -mfma -mavx2 -mavx512f
CCFLAGS := $(CCFLAGS) -fopenmp
CCFLAGS := $(CCFLAGS) -fsel-sched-pipelining --param max-inline-insns-single=128 --param max-inline-insns-size=256

# QD
CCFLAGS := $(CCFLAGS) -I../tm_blas/others/qd-2.3.23/include/
LDFLAGS := $(LDFLAGS) ../tm_blas/others/qd-2.3.23/src/.libs/libqd.a

# MPFR/MPREAL
CCFLAGS := $(CCFLAGS) -I../tm_blas/include/
LDFLAGS := $(LDFLAGS) -lmpfr -lgmp


a.out: main.o
	$(CXX) -g -o a.out main.o $(LDFLAGS)
main.o: main.cpp
	$(CXX) -S main.cpp $(CCFLAGS)
	$(CXX) -c main.cpp $(CCFLAGS)


clean:
	\rm *.o a.out *.s

