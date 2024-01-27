#CXX = g++-11 --std=c++14
#CXX = g++-10 --std=c++14
#CXX = g++ --std=c++14
#CXX = g++ --std=c++17
#CXX = icpx --std=c++14 -fp-model strict -vec
ifeq (x$(CXX),x)
	CXX = g++
endif
ifeq (x$(CXX),xg++)
	CXX = g++-11
	cxx = g++
endif
ifeq (x$(CXX),xicpx)
	CXX = icpx -fp-model strict #-fast #-qopt-report-stdout -ipo
	cxx = icpx
endif
ifneq (x$(shell which $(CXX) 2>&1 | grep 'which'),x)
	CXX = g++
endif

CXX := $(CXX) --std=c++14

CCFLAGS := $(CCFLAGS) -O3 -Wall -I./ -include mX_real.hpp #-ftime-report
LDFLAGS = -fopenmp -lquadmath -lm

# Optimizations
CCFLAGS := $(CCFLAGS) -mfma -mavx2
#-mavx512f
CCFLAGS := $(CCFLAGS) -fopenmp

# QD
QD_CCFLAGS = -I../tm_blas/others/qd-2.3.23/include/
QD_LDFLAGS = ../tm_blas/others/qd-2.3.23/src/.libs/libqd.a

# MPFR/MPREAL
MPFR_CCFLAGS = -I./mpreal/
MPFR_LDFLAGS = -lmpfr -lgmp

all: a.out sample.exe mX_real.hpp.gch

a.out: main.o
	$(CXX) -o a.out main.o $(LDFLAGS) $(QD_LDFLAGS) $(MPFR_LDFLAGS)
main.o: main.cpp mpreal mX_real.hpp.gch
	$(CXX) -S main.cpp $(CCFLAGS) $(QD_CCFLAGS) $(MPFR_CCFLAGS)
	$(CXX) -c main.cpp $(CCFLAGS) $(QD_CCFLAGS) $(MPFR_CCFLAGS)

sample.exe: sample.cpp mpreal mX_real.hpp.gch
	$(CXX) -S            sample.cpp $(CCFLAGS)
	$(CXX) -o sample.exe sample.cpp $(CCFLAGS)

mX_real.hpp.gch: mX_real.hpp
	$(CXX) -c  -x c++-header mX_real.hpp $(CCFLAGS)

qd_real:

mpreal:
	mkdir mpreal
	git clone https://github.com/advanpix/mpreal.git


clean:
	\rm *.o a.out *.s sample.exe sample.exe-* *.gch *.tmp

