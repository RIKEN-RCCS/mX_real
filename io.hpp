#ifndef MX_REAL_IO_H
#define MX_REAL_IO_H

#include <stdio.h>
#include <omp.h>
#include <iomanip>
#include <quadmath.h>

#include <qd/dd_real.h>
#include <qd/qd_real.h>
#include "mX_real.hpp"


namespace mX_real {
  //
  //
  template < typename T, Algorithm A >
  inline auto debug_print ( std::string message, dX_real::dx_real<T,A> const &x, bool const flag = false )
  -> std::enable_if_t< std::is_same< T, float >::value, void > {
    uint32_t * d = (uint32_t *)(&x.x[0]);
#if 0
    long double xx =
          (long double)x.x[1] +
          (long double)x.x[0];
    std::cout << message;
    printf(" %cDF : %08x %08x %26.19Le\n",
                    toString(A)[0], d[0], d[1], xx);
#else
    __float128 xx = (double)x.x[0] + (double)x.x[1];
    char c[80];
    quadmath_snprintf(c, 80, "%40.33Qe", xx);
    std::cout << message;
    printf(" %cDF : %08x %08x %s\n",
                    toString(A)[0], d[0], d[1], c);
#endif
    if ( flag ) {
    std::cout << "  : " << std::bitset<32>(d[0])
            << "\n    " << std::bitset<32>(d[1]);
#if 0
    printf(" %26.19Le\n", xx);
#else
    quadmath_snprintf(c, 80, "%40.33Qe", xx);
    printf(" %s\n", c);
#endif
    }
  }

  template < typename T, Algorithm A >
  inline auto debug_print ( std::string message, dX_real::dx_real<T,A> const &x, bool const flag = false )
  -> std::enable_if_t< std::is_same< T, double >::value, void > {
    uint64_t * d = (uint64_t *)(&x.x[0]);
#if 0
    long double xx =
          (long double)x.x[1] +
          (long double)x.x[0];
    std::cout << message;
    printf(" %cDD : %016lx %016lx %26.19Le\n",
                    toString(A)[0], d[0], d[1], xx);
#else
    __float128 xx = (double)x.x[0] + (double)x.x[1];
    char c[80];
    quadmath_snprintf(c, 80, "%40.33Qe", xx);
    std::cout << message;
    printf(" %cDD : %016lx %016lx %s\n",
                    toString(A)[0], d[0], d[1], c);
#endif
    if ( flag ) {
    std::cout << "  : " << std::bitset<64>(d[0])
            << "\n    " << std::bitset<64>(d[1]);
#if 0
    printf(" %26.19Le\n", xx);
#else
    quadmath_snprintf(c, 80, "%40.33Qe", xx);
    printf(" %s\n", c);
#endif
    }
  }
}

namespace mX_real {
  //
  //
  template < typename T, Algorithm A >
  inline auto debug_print ( std::string message, tX_real::tx_real<T,A> const &x, bool const flag = false )
  -> std::enable_if_t< std::is_same< T, float >::value, void > {
    uint32_t * d = (uint32_t *)(&x.x[0]);
#if 0
    long double xx = (long double)x.x[2]
                   + (long double)x.x[1]
                   + (long double)x.x[0];
    std::cout << message;
    printf(" %cTF : %08x %08x %08x %26.19Le\n",
                    toString(A)[0], d[0], d[1], d[2], xx);
#else
    __float128 xx = (double)x.x[0] + (double)x.x[1] + (double)x.x[2];
    char c[80];
    quadmath_snprintf(c, 80, "%40.33Qe", xx);
    std::cout << message;
    printf(" %cTF : %08x %08x %08x %s\n",
                    toString(A)[0], d[0], d[1], d[2], c);
#endif
    if ( flag ) {
    std::cout << "  : " << std::bitset<32>(d[0])
            << "\n    " << std::bitset<32>(d[1])
            << "\n    " << std::bitset<32>(d[2]);
#if 0
    printf(" %26.19Le\n", xx);
#else
    quadmath_snprintf(c, 80, "%40.33Qe", xx);
    printf(" %s\n", c);
#endif
    }
  }
  template < typename T, Algorithm A >
  inline auto debug_print ( std::string message, tX_real::tx_real<T,A> const &x, bool const flag = false )
  -> std::enable_if_t< std::is_same< T, double >::value, void > {
    uint64_t * d = (uint64_t *)(&x.x[0]);
#if 0
    long double xx = (long double)x.x[2]
                   + (long double)x.x[1]
                   + (long double)x.x[0];
    std::cout << message;
    printf(" %cTD : %016lx %016lx %016lx %26.19Le\n",
                    toString(A)[0], d[0], d[1], d[2], xx);
#else
    __float128 xx = (double)x.x[0] + (double)x.x[1] + (double)x.x[2];
    char c[80];
    quadmath_snprintf(c, 80, "%40.33Qe", xx);
    std::cout << message;
    printf(" %cTD : %016lx %016lx %016lx %s\n",
                    toString(A)[0], d[0], d[1], d[2], c);
#endif
    if ( flag ) {
    std::cout << "  : " << std::bitset<64>(d[0])
            << "\n    " << std::bitset<64>(d[1])
            << "\n    " << std::bitset<64>(d[2]);
#if 0
    printf(" %26.19Le\n", xx);
#else
    quadmath_snprintf(c, 80, "%40.33Qe", xx);
    printf(" %s\n", c);
#endif
    }
  }
}

namespace mX_real {
  //
  //
  template < typename T, Algorithm A >
  inline auto debug_print ( std::string message, qX_real::qx_real<T,A> const &x, bool const flag = false )
  -> std::enable_if_t< std::is_same< T, float >::value, void > {
    uint32_t * d = (uint32_t *)(&x.x[0]);
#if 0
    long double xx = (long double)x.x[3]
                   + (long double)x.x[2]
                   + (long double)x.x[1]
                   + (long double)x.x[0];
    std::cout << message;
    printf(" %cQF : %08x %08x %08x %08x %26.19Le\n",
                    toString(A)[0], d[0], d[1], d[2], d[3], xx);
#else
    __float128 xx = (double)x.x[0] + (double)x.x[1] + (double)x.x[2] + (double)x.x[3];
    char c[80];
    quadmath_snprintf(c, 80, "%40.33Qe", xx);
    std::cout << message;
    printf(" %cQF : %08x %08x %08x %08x %s\n",
                    toString(A)[0], d[0], d[1], d[2], d[3], c);
#endif
    if ( flag ) {
    std::cout << "  : " << std::bitset<32>(d[0]) << "\n    " << std::bitset<32>(d[1])
            << "\n    " << std::bitset<32>(d[2]) << "\n    " << std::bitset<32>(d[3]);
#if 0
    printf(" %26.19Le\n", xx);
#else
    quadmath_snprintf(c, 80, "%40.33Qe", xx);
    printf(" %s\n", c);
#endif
    }
  }
  template < typename T, Algorithm A >
  inline auto debug_print ( std::string message, qX_real::qx_real<T,A> const &x, bool const flag = false )
  -> std::enable_if_t< std::is_same< T, double >::value, void > {
    uint64_t * d = (uint64_t *)(&x.x[0]);
#if 0
    long double xx = (long double)x.x[3]
                   + (long double)x.x[2]
                   + (long double)x.x[1]
                   + (long double)x.x[0];
    std::cout << message;
    printf(" %cQD : %016lx %016lx %016lx %016lx %26.19Le\n",
                    toString(A)[0], d[0], d[1], d[2], d[3], xx);
#else
    __float128 xx = (double)x.x[0] + (double)x.x[1] + (double)x.x[2] + (double)x.x[3];
    char c[80];
    quadmath_snprintf(c, 80, "%40.33Qe", xx);
    std::cout << message;
    printf(" %cQD : %016lx %016lx %016lx %016lx %s\n",
                    toString(A)[0], d[0], d[1], d[2], d[3], c);
#endif
    if ( flag ) {
    std::cout << "  : " << std::bitset<64>(d[0]) << "\n    " << std::bitset<64>(d[1])
            << "\n    " << std::bitset<64>(d[2]) << "\n    " << std::bitset<64>(d[3]);
#if 0
    printf(" %26.19Le\n", xx);
#else
    quadmath_snprintf(c, 80, "%40.33Qe", xx);
    printf(" %s\n", c);
#endif
    }
  }
}


template < typename T >
auto print( std::string message, T const &x, bool const flag = false )
-> std::enable_if_t< check_mX_real<T>::value,void> {
  mX_real::debug_print<typename T::base_T>( message, x, flag );
}

void print( std::string message, mpfr::mpreal const &x, bool const flag = false ) { }

void print( std::string message, float const &x, bool const flag = false ) {
  unsigned int * d = (unsigned int *)&x;
#if 0
  long double xx = (long double)x;
  std::cout << message;
  printf(" _SF : %08x %26.19Le\n", d[0], xx);
#else
  __float128 xx = (double)x;
  char c[80];
  quadmath_snprintf(c, 80, "%40.33Qe", xx);
  std::cout << message;
  printf(" _SF : %08x %s\n", d[0], c);
#endif
  if ( flag ) {
  std::cout << "  : " << std::bitset<32>(d[0]);
#if 0
  printf(" %26.19Le\n", xx);
#else
  printf(" %s\n", c);
#endif
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
#if 0
  xx = (long double)c1 + (long double)c0;
  union { unsigned int e; double a; } x;
  x.a = c0; auto u0 = x.e;
  x.a = c1; auto u1 = x.e;
  printf("  : %08x %08x %26.19Le\n", u0, u1, xx);
  std::cout << "  : " << std::bitset<64>(d[0]);
  printf(" %26.19Le\n", xx);
#else
  __float128 xx = x;
  char c[80];
  quadmath_snprintf(c, 80, "%40.33Qe", xx);
  union { unsigned int e; double a; } x;
  x.a = c0; auto u0 = x.e;
  x.a = c1; auto u1 = x.e;
  printf("  : %08x %08x %s\n", u0, u1, c);
  std::cout << "  : " << std::bitset<64>(d[0]);
  printf(" %s\n", c);
#endif
  }
}

#ifdef  _QD_DD_REAL_H
void print( std::string message, dd_real const &x, bool const flag = false ) {
  unsigned long * d = (unsigned long *)&x;
#if 0
  long double xx = (long double)x.x[0] + (long double)x.x[1];
  std::cout << message;
  printf(" _DD : %016lx %016lx %26.19Le\n", d[0], d[1], xx);
#else
  __float128 xx = (double)x.x[0] + (double)x.x[1];
  char c[80];
  quadmath_snprintf(c, 80, "%40.33Qe", xx);
  std::cout << message;
  printf(" _DD : %016lx %016lx %s\n", d[0], d[1], c);
#endif
}
#endif

#ifdef  _QD_QD_REAL_H
void print( std::string message, qd_real const &x, bool const flag = false ) {
  unsigned long * d = (unsigned long *)&x;
#if 0
  long double xx = (long double)x.x[0] + (long double)x.x[1] + (long double)x.x[2] + (long double)x.x[3];
  std::cout << message;
  printf(" _QD : %016lx %016lx %016lx %016lx %26.19Le\n", d[0], d[1], d[2], d[3], xx);
#else
  __float128 xx = (double)x.x[0] + (double)x.x[1] + (double)x.x[2] + (double)x.x[3];
  char c[80];
  quadmath_snprintf(c, 80, "%40.33Qe", xx);
  std::cout << message;
  printf(" _QD : %016lx %016lx %016lx %016lx %s\n", d[0], d[1], d[2], d[3], c);
#endif
}
#endif

#endif
