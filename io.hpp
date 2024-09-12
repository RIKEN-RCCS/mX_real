#ifndef MX_REAL_IO_H
#define MX_REAL_IO_H

#include <stdio.h>
#include <omp.h>
#include <iomanip>

#include "mX_real.hpp"


namespace mX_real {

  template < typename TX, T_assert( check_mX_real<TX>::value ) >
  inline auto TYPE_name ( char * x ) {
    using T = typename TX::base_T;
    Algorithm constexpr A = TX::base_A;
    int constexpr L = TX::L;
    std::string X = "_SDTQ";
    x[0] = toString(A)[0];
    x[1] = X[L];
    x[2] = 0;
    if ( std::is_same<T,double>::value ) { x[2] = 'D'; }
    if ( std::is_same<T,float>::value ) { x[2] = 'F'; }
    x[3] = 0;
  }
  template < typename T, T_assert( fp<T>::value ) >
  inline auto TYPE_name ( char * x ) {
    strcpy( x, "___" );
    if ( std::is_same<T,float>::value ) strcpy( x, "_SF" );
    if ( std::is_same<T,double>::value ) strcpy( x, "_SD" );
  }
#ifdef  _QD_DD_REAL_H
  template < typename T, T_assert( std::is_same<T, dd_real>::value ) >
  inline auto TYPE_name ( char * x ) { strcpy( x, "_DD" ); }
#endif
#ifdef  _QD_QD_REAL_H
  template < typename T, T_assert( std::is_same<T, qd_real>::value ) >
  inline auto TYPE_name ( char * x ) { strcpy( x, "_QD" ); }
#endif
    
  template < typename T, Algorithm A, T_eq_Ts(T,float) >
  inline auto debug_print ( std::string message, dX_real::dx_real<T,A> const &x ) {
    uint32_t * d = (uint32_t *)(&x.x[0]);
#if USE_MPREAL
    mpfr::mpreal xx = double(x.x[0]); xx += double(x.x[1]);
    std::cout << message;
    printf(" %cDF : %08x %08x ", toString(A)[0], d[0], d[1]);
    std::cout << xx << "\n";
#else
    long double xx =
      (long double)x.x[1] +
      (long double)x.x[0];
    std::cout << message;
    printf(" %cDF : %08x %08x %26.19Le\n",
           toString(A)[0], d[0], d[1], xx);
#endif
  }
  
  template < typename T, Algorithm A, T_eq_Ts(T,double) >
  inline auto debug_print ( std::string message, dX_real::dx_real<T,A> const &x ) {
    uint64_t * d = (uint64_t *)(&x.x[0]);
#if USE_MPREAL
    mpfr::mpreal xx = double(x.x[0]); xx += double(x.x[1]);
    std::cout << message;
    printf(" %cTD : %016lx %016lx ", toString(A)[0], d[0], d[1]);
    std::cout << xx << "\n";
#else
    long double xx =
      (long double)x.x[1] +
      (long double)x.x[0];
    std::cout << message;
    printf(" %cDD : %016lx %016lx %26.19Le\n",
           toString(A)[0], d[0], d[1], xx);
#endif
  }
}

namespace mX_real {
  //
  //
  template < typename T, Algorithm A, T_eq_Ts(T,float) >
  inline auto debug_print ( std::string message, tX_real::tx_real<T,A> const &x ) {
    uint32_t * d = (uint32_t *)(&x.x[0]);
#if USE_MPREAL
    mpfr::mpreal xx = double(x.x[0]); xx += double(x.x[1]); xx += double(x.x[2]);
    std::cout << message;
    printf(" %cTF : %08x %08x %08x ", toString(A)[0], d[0], d[1], d[2]);
    std::cout << xx << "\n";
#else
    long double xx = (long double)x.x[2]
      + (long double)x.x[1]
      + (long double)x.x[0];
    std::cout << message;
    printf(" %cTF : %08x %08x %08x %26.19Le\n",
           toString(A)[0], d[0], d[1], d[2], xx);
#endif
  }
  template < typename T, Algorithm A, T_eq_Ts(T,double) >
  inline auto debug_print ( std::string message, tX_real::tx_real<T,A> const &x ) {
    uint64_t * d = (uint64_t *)(&x.x[0]);
#if USE_MPREAL
    mpfr::mpreal xx = double(x.x[0]); xx += double(x.x[1]); xx += double(x.x[2]);
    std::cout << message;
    printf(" %cTD : %016lx %016lx %016lx ", toString(A)[0], d[0], d[1], d[2]);
    std::cout << xx << "\n";
#else
    long double xx = (long double)x.x[2]
      + (long double)x.x[1]
      + (long double)x.x[0];
    std::cout << message;
    printf(" %cTD : %016lx %016lx %016lx %26.19Le\n",
           toString(A)[0], d[0], d[1], d[2], xx);
#endif
  }
}

namespace mX_real {
  //
  //
  template < typename T, Algorithm A, T_eq_Ts(T,float) >
  inline auto debug_print ( std::string message, qX_real::qx_real<T,A> const &x ) {
    uint32_t * d = (uint32_t *)(&x.x[0]);
#if USE_MPREAL
    mpfr::mpreal xx = double(x.x[0]); xx += double(x.x[1]); xx += double(x.x[2]); xx += double(x.x[3]);
    std::cout << message;
    printf(" %cQF : %08x %08x %08x %08x ", toString(A)[0], d[0], d[1], d[2], d[3]);
    std::cout << xx << "\n";
#else
    long double xx = (long double)x.x[3]
      + (long double)x.x[2]
      + (long double)x.x[1]
      + (long double)x.x[0];
    std::cout << message;
    printf(" %cQF : %08x %08x %08x %08x %26.19Le\n",
           toString(A)[0], d[0], d[1], d[2], d[3], xx);
#endif
  }
  template < typename T, Algorithm A, T_eq_Ts(T,double) >
  inline auto debug_print ( std::string message, qX_real::qx_real<T,A> const &x ) {
    uint64_t * d = (uint64_t *)(&x.x[0]);
#if USE_MPREAL
    mpfr::mpreal xx = double(x.x[0]); xx += double(x.x[1]); xx += double(x.x[2]); xx += double(x.x[3]);
    std::cout << message;
    printf(" %cQD : %016lx %016lx %016lx %016lx ", toString(A)[0], d[0], d[1], d[2], d[3]);
    std::cout << xx << "\n";
#else
    long double xx = (long double)x.x[3]
      + (long double)x.x[2]
      + (long double)x.x[1]
      + (long double)x.x[0];
    std::cout << message;
    printf(" %cQD : %016lx %016lx %016lx %016lx %26.19Le\n",
           toString(A)[0], d[0], d[1], d[2], d[3], xx);
#endif
  }
}


template < typename T, T_mX(T) >
auto print ( std::string message, T const &x ) {
  mX_real::debug_print<typename T::base_T>( message, x );
}


template < typename T, T_eq_Ts(T,mpfr::mpreal) >
auto print ( std::string message, T const &x ) { }

template < typename T, T_eq_Ts(T,float) >
auto print ( std::string message, T const &x ) {
  unsigned int * d = (unsigned int *)&x;
#if USE_MPREAL
  mpfr::mpreal xx = double(x);
  std::cout << message;
  printf(" _SF : %08x ", d[0]);
  std::cout << xx << "\n";
#else
  long double xx = (long double)x;
  std::cout << message;
  printf(" _SF : %08x %26.19Le\n", d[0], xx);
#endif
}

template < typename T, T_eq_Ts(T,double) >
auto print ( std::string message, T const &x ) {
  unsigned long * d = (unsigned long *)&x;
#if USE_MPREAL
  mpfr::mpreal xx = double(x);
  std::cout << message;
  printf(" _SD : %016lx ", d[0]);
  std::cout << xx << "\n";
#else
  long double xx = (long double)x;
  std::cout << message;
  printf(" _SD : %016lx  %26.19Le\n", d[0], xx);
#endif
}

#ifdef  _QD_DD_REAL_H
template < typename T, T_eq_Ts(T,dd_real) >
auto print ( std::string message, T const &x ) {
  unsigned long * d = (unsigned long *)&x;
#if USE_MPREAL
  mpfr::mpreal xx = double(x.x[0]); xx += double(x.x[1]);
  std::cout << message;
  printf(" _DD : %016lx %016lx ", d[0], d[1]);
  std::cout << xx << "\n";
#else
  long double xx = (long double)x.x[0] + (long double)x.x[1];
  std::cout << message;
  printf(" _DD : %016lx %016lx %26.19Le\n", d[0], d[1], xx);
#endif
}
#endif

#ifdef  _QD_QD_REAL_H
template < typename T, T_eq_Ts(T,qd_real) >
auto print ( std::string message, T const &x ) {
  unsigned long * d = (unsigned long *)&x;
#if USE_MPREAL
  mpfr::mpreal xx = double(x.x[0]); xx += double(x.x[1]); xx += double(x.x[2]); xx += double(x.x[3]);
  std::cout << message;
  printf(" _QD : %016lx %016lx %016lx %016lx ", d[0], d[1], d[2], d[3]);
  std::cout << xx << "\n";
#else
  long double xx = (long double)x.x[0] + (long double)x.x[1] + (long double)x.x[2] + (long double)x.x[3];
  std::cout << message;
  printf(" _QD : %016lx %016lx %016lx %016lx %26.19Le\n", d[0], d[1], d[2], d[3], xx);
#endif
}
#endif

#endif

