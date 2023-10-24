#ifndef MX_REAL_IO_H
#define MX_REAL_IO_H

#include <stdio.h>
#include <omp.h>
#include <iomanip>

#include <qd/dd_real.h>
#include <qd/qd_real.h>
#include "mX_real.hpp"


template < typename T >
auto print( std::string message, T const &x, bool const flag = false )
-> std::enable_if_t<!check_sX_real<T>::value,void> {
  mX_real::debug_print<typename T::base_T>( message, x, flag );
}

void print( std::string message, float const &x, bool const flag = false ) {
  unsigned int * d = (unsigned int *)&x;
  long double xx = (long double)x;
  std::cout << message;
  printf(" _SF : %08x %26.19Le\n", d[0], xx);
  if ( flag ) {
  std::cout << "  : " << std::bitset<32>(d[0]);
  printf(" %26.19Le\n", xx);
  }
}

void print( std::string message, double const &x, bool const flag = false ) {
  unsigned long * d = (unsigned long *)&x;
  long double xx = (long double)x;
  std::cout << message;
  printf(" _SD : %016lx  %26.19Le\n", d[0], xx);
  if ( flag ) {
  double t = x;
  float c0, c1;
  c0 = (float)t; t -= (double)c0;
  c1 = (float)t;
  xx = (long double)c1 + (long double)c0;
  printf("  : %08x %08x %26.19Le\n", *(unsigned int *)&c0, *(unsigned int *)&c1, xx);
  std::cout << "  : " << std::bitset<64>(d[0]);
  printf(" %26.19Le\n", xx);
  }
}

void print( std::string message, dd_real const &x, bool const flag = false ) {
  unsigned long * d = (unsigned long *)&x;
  long double xx = (long double)x.x[1] + (long double)x.x[0];
  std::cout << message;
  printf(" _DD : %016lx %016lx %26.19Le\n", d[0], d[1], xx);
}

void print( std::string message, qd_real const &x, bool const flag = false ) {
  unsigned long * d = (unsigned long *)&x;
  long double xx = (long double)x.x[3] + (long double)x.x[2] + (long double)x.x[1] + (long double)x.x[0];
  std::cout << message;
  printf(" _QD : %016lx %016lx %016lx %016lx %26.19Le\n", d[0], d[1], d[2], d[3], xx);
}

#endif
