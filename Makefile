CXX = g++-11 --std=c++14
#CXX = g++-10 --std=c++14
#CXX = g++ --std=c++14
#CXX = g++ --std=c++17
CCFLAGS := $(CCFLAGS) -O3
CCFLAGS := $(CCFLAGS) -g
LDFLAGS = -fopenmp -lgomp

# Optimizations
CCFLAGS := $(CCFLAGS) -mfma -mavx2
#-mavx512f
CCFLAGS := $(CCFLAGS) -fopenmp
#CCFLAGS := $(CCFLAGS) -fsel-sched-pipelining --param max-inline-insns-single=1024 --param max-inline-insns-size=1024

# QD
QD_CCFLAGS = -I../tm_blas/others/qd-2.3.23/include/
QD_LDFLAGS = ../tm_blas/others/qd-2.3.23/src/.libs/libqd.a

# MPFR/MPREAL
MPFR_CCFLAGS = -I./mpreal/
MPFR_LDFLAGS = -lmpfr -lgmp

all: a.out sample.exe

a.out: main.o
	$(CXX) -g -o a.out main.o $(LDFLAGS) $(QD_LDFLAGS) $(MPFR_LDFLAGS)
main.o: main.cpp mpreal
	$(CXX) -S main.cpp $(CCFLAGS) $(QD_CCFLAGS) $(MPFR_CCFLAGS)
	$(CXX) -c main.cpp $(CCFLAGS) $(QD_CCFLAGS) $(MPFR_CCFLAGS)

sample.exe: sample.cpp mpreal
	$(CXX) -S            sample.cpp $(CCFLAGS)
	$(CXX) -o sample.exe sample.cpp $(CCFLAGS)

mpreal:
	mkdir mpreal
	git clone https://github.com/advanpix/mpreal.git


clean:
	\rm *.o a.out *.s sample.exe

