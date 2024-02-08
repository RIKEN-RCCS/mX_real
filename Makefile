ifeq (x$(CXX),x)
	CXX = g++
endif
ifeq (x$(CXX),xg++)
	CXX = g++-11
	cxx = g++
endif
ifeq (x$(CXX),xicpx)
	CXX = icpx
	cxx = icpx
endif
ifneq (x$(shell which $(CXX) 2>&1 | grep 'which'),x)
	CXX = g++
endif

CXX := $(CXX) --std=c++14
ifeq (x$(cxx),xicpx)
        CXX := $(CXX) -fp-model strict
endif


CCFLAGS := $(CCFLAGS) -O3 -Wall -I./ -include mX_real.hpp
LDFLAGS = -fopenmp -lquadmath -lm

# Optimizations
CCFLAGS := $(CCFLAGS) -mfma -mavx2
#CCFLAGS := $(CCFLAGS) -mfma -mavx2 -mavx512f
CCFLAGS := $(CCFLAGS) -fopenmp

# QD
QD_CCFLAGS = -I./qd_real/include/
QD_LDFLAGS = ./qd_real/src/.libs/libqd.a

# MPFR/MPREAL
CCFLAGS := $(CCFLAGS) -DUSE_MPREAL=1
MPFR_CCFLAGS = -I./mpreal/
MPFR_LDFLAGS = -lmpfr -lgmp


all: Ozaki-QW/qxw.hpp mX_real.hpp dX_real.hpp tX_real.hpp qX_real.hpp


bench: a.out sample.exe
a.out: main.o
	$(CXX) -o a.out main.o $(LDFLAGS) $(QD_LDFLAGS) $(MPFR_LDFLAGS)
main.o: mpreal qd_real main.cpp mX_real.hpp.gch
	$(CXX) -S main.cpp $(CCFLAGS) $(QD_CCFLAGS) $(MPFR_CCFLAGS)
	$(CXX) -c main.cpp $(CCFLAGS) $(QD_CCFLAGS) $(MPFR_CCFLAGS)
sample.exe: mpreal sample.cpp mX_real.hpp.gch
	$(CXX) -S            sample.cpp $(CCFLAGS) $(MPFR_CCFLAGS)
	$(CXX) -o sample.exe sample.cpp $(CCFLAGS) $(MPFR_LDFLAGS)


mX_real.hpp.gch: mX_real.hpp Ozaki-QW/qxw.hpp dX_real.hpp tX_real.hpp qX_real.hpp
	$(CXX) -c  -x c++-header mX_real.hpp $(CCFLAGS) $(MPFR_CCFLAGS)
mX_real.hpp dX_real.hpp tX_real.hpp qX_real.hpp:
	cd etc; make clean; make
Ozaki-QW/qxw.hpp:
	cd Ozaki-QW; make


qd_real:
	cd etc; /bin/sh ./qd_install.sh
	if [ ! -e qd_real ]; then \
		ln -s etc/qd_real .; \
	fi
mpreal:
	cd etc; /bin/sh ./mp_install.sh
	if [ ! -e mpreal ]; then \
		ln -s etc/mpreal .; \
	fi


clean:
	-\rm *.o a.out *.s sample.exe sample.exe-* *.gch *.tmp
       
distclean:
	-make clean
	-cd etc; make distclean
	-cd Ozaki-QW; make distclean
	-\rm mpreal
	-\rm qd_real
	-\rm dX_real.hpp tX_real.hpp qX_real.hpp
	-\rm mX_real.hpp
	make

