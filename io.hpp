#ifndef MX_REAL_IO_H
#define MX_REAL_IO_H

#include <stdio.h>
#include <omp.h>
#include <iomanip>

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
    long double xx =
          (long double)x.x[1] +
          (long double)x.x[0];
    std::cout << message;
    printf(" %cDF : %08x %08x %26.19Le\n",
                    toString(A)[0], d[0], d[1], xx);
    if ( flag ) {
    std::cout << "  : " << std::bitset<32>(d[0])
            << "\n    " << std::bitset<32>(d[1]);
    printf(" %26.19Le\n", xx);
    }
  }

  template < typename T, Algorithm A >
  inline auto debug_print ( std::string message, dX_real::dx_real<T,A> const &x, bool const flag = false )
  -> std::enable_if_t< std::is_same< T, double >::value, void > {
    uint64_t * d = (uint64_t *)(&x.x[0]);
    long double xx =
          (long double)x.x[1] +
          (long double)x.x[0];
    std::cout << message;
    printf(" %cDD : %016lx %016lx %26.19Le\n",
                    toString(A)[0], d[0], d[1], xx);
    if ( flag ) {
    std::cout << "  : " << std::bitset<64>(d[0])
            << "\n    " << std::bitset<64>(d[1]);
    printf(" %26.19Le\n", xx);
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
    long double xx = (long double)x.x[2]
                   + (long double)x.x[1]
                   + (long double)x.x[0];
    std::cout << message;
    printf(" %cTF : %08x %08x %08x %26.19Le\n",
                    toString(A)[0], d[0], d[1], d[2], xx);
    if ( flag ) {
    std::cout << "  : " << std::bitset<32>(d[0])
            << "\n    " << std::bitset<32>(d[1])
            << "\n    " << std::bitset<32>(d[2]);
    printf(" %26.19Le\n", xx);
    }
  }
  template < typename T, Algorithm A >
  inline auto debug_print ( std::string message, tX_real::tx_real<T,A> const &x, bool const flag = false )
  -> std::enable_if_t< std::is_same< T, double >::value, void > {
    uint64_t * d = (uint64_t *)(&x.x[0]);
    long double xx = (long double)x.x[2]
                   + (long double)x.x[1]
                   + (long double)x.x[0];
    std::cout << message;
    printf(" %cTD : %016lx %016lx %016lx %26.19Le\n",
                    toString(A)[0], d[0], d[1], d[2], xx);
    if ( flag ) {
    std::cout << "  : " << std::bitset<64>(d[0])
            << "\n    " << std::bitset<64>(d[1])
            << "\n    " << std::bitset<64>(d[2]);
    printf(" %26.19Le\n", xx);
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
    long double xx = (long double)x.x[3]
                   + (long double)x.x[2]
                   + (long double)x.x[1]
                   + (long double)x.x[0];
    std::cout << message;
    printf(" %cQF : %08x %08x %08x %08x %26.19Le\n",
                    toString(A)[0], d[0], d[1], d[2], d[3], xx);
    if ( flag ) {
    std::cout << "  : " << std::bitset<32>(d[0]) << "\n    " << std::bitset<32>(d[1])
            << "\n    " << std::bitset<32>(d[2]) << "\n    " << std::bitset<32>(d[3]);
    printf(" %26.19Le\n", xx);
    }
  }
  template < typename T, Algorithm A >
  inline auto debug_print ( std::string message, qX_real::qx_real<T,A> const &x, bool const flag = false )
  -> std::enable_if_t< std::is_same< T, double >::value, void > {
    uint64_t * d = (uint64_t *)(&x.x[0]);
    long double xx = (long double)x.x[3]
                   + (long double)x.x[2]
                   + (long double)x.x[1]
                   + (long double)x.x[0];
    std::cout << message;
    printf(" %cQD : %016lx %016lx %016lx %016lx %26.19Le\n",
                    toString(A)[0], d[0], d[1], d[2], d[3], xx);
    if ( flag ) {
    std::cout << "  : " << std::bitset<64>(d[0]) << "\n    " << std::bitset<64>(d[1])
            << "\n    " << std::bitset<64>(d[2]) << "\n    " << std::bitset<64>(d[3]);
    printf(" %26.19Le\n", xx);
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
  union { unsigned int e; double a; } x; x.a = c0;
  x.a = c0; auto u0 = x.e;
  x.a = c1; auto u1 = x.e;
  printf("  : %08x %08x %26.19Le\n", u0, u1, xx);
  std::cout << "  : " << std::bitset<64>(d[0]);
  printf(" %26.19Le\n", xx);
  }
}

#ifdef  _QD_DD_REAL_H
void print( std::string message, dd_real const &x, bool const flag = false ) {
  unsigned long * d = (unsigned long *)&x;
  long double xx = (long double)x.x[1] + (long double)x.x[0];
  std::cout << message;
  printf(" _DD : %016lx %016lx %26.19Le\n", d[0], d[1], xx);
}
#endif

#ifdef  _QD_QD_REAL_H
void print( std::string message, qd_real const &x, bool const flag = false ) {
  unsigned long * d = (unsigned long *)&x;
  long double xx = (long double)x.x[3] + (long double)x.x[2] + (long double)x.x[1] + (long double)x.x[0];
  std::cout << message;
  printf(" _QD : %016lx %016lx %016lx %016lx %26.19Le\n", d[0], d[1], d[2], d[3], xx);
}
#endif

#endif
