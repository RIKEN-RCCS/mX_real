#ifndef MX_REAL_MPFR_CONVERT_H
#define MX_REAL_MPFR_CONVERT_H


//
// MPFR extensions
//
#include "mpreal.h"

namespace mX_real {

  //
  void print_sub( mpfr::mpreal const x ) {
    int const digits = (int)ceil( 256 * log((double)2.0) / log((double)10.0) );
    std::cout.precision(digits);
    std::cout << std::scientific << std::showpos << x;
    std::cout << std::endl; }


  //
  template < typename T > auto convert( T const& x )
  -> std::enable_if_t<std::is_same<T,mpfr::mpreal>::value, mpfr::mpreal> { return x; }
  //
  template < typename T > auto convert( T const& x )
  -> std::enable_if_t<!check_mX_real<T>::value,mpfr::mpreal> { return mpfr::mpreal((double)x); }

  //
  template < typename T > auto convert( T const& x )
  -> std::enable_if_t< check_mX_real<T>::value,mpfr::mpreal> {
    mpfr::mpreal s = mpfr::mpreal(0); for(int i=0;i<T::L;i++) s = s + (double)x.x[i]; return s; }
  //
#ifdef  _QD_DD_REAL_H
  template < > mpfr::mpreal convert<dd_real>( dd_real const& x ) {
    mpfr::mpreal s = mpfr::mpreal(0); for(int i=0;i<2;i++) s = s + x.x[i]; return s; }
#endif
#ifdef  _QD_QD_REAL_H
  template < > mpfr::mpreal convert<qd_real>( qd_real const& x ) {
    mpfr::mpreal s = mpfr::mpreal(0); for(int i=0;i<4;i++) s = s + x.x[i]; return s; }
#endif


  template < typename T > auto convert( mpfr::mpreal const& x )
  -> std::enable_if_t<!check_mX_real<T>::value,T> {
    auto s = (T)((double)x);
    if ( s != zero<T>() ) {
      mpfr::mpreal S = (double)s;
      if ( std::is_same<T,float>::value ) {
        if ( (s > 0 && S > x) || (s < 0 && S < x) ) { // to avoid double rounding
          s = s - fp<T>::ufp( s );
        }
      }
    }
    return s;
  }

  //
  template < typename T > auto convert( mpfr::mpreal const& x )
  -> std::enable_if_t< check_mX_real<T>::value,T> {
    mpfr::mpreal X = x;
    using _T  = typename T::base_T; T t;
    for(int i=0; i<T::L; i++) {
      auto s = (_T)((double)X);
      if ( i<T::L-1 && s != zero<_T>() ) {

        auto e  = fp<_T>::exponent( s );
        auto ex = fp<_T>::exponenti( s );

	//
	// exponent-shift arround one
	//
        s = s * ex;
        X = X * ex;
        mpfr::mpreal S = (double)s;

        //
	// check whether rounding up already,
	// if so rounding back to avoid double-rounding
	//
        if ( (s > 0 && S > X) || (s < 0 && S < X) ) {
          s = s - fp<_T>::ufp( s );
        }

	//
	// eliminate the higher bit part from the originial
	// then exponent-shift back
	//
        X = (X - (double)s) * e;
        s = s * e;

      }
      t.x[i] = s;
    }
    return t;
  }
  //
#ifdef  _QD_DD_REAL_H
  template < > dd_real convert<dd_real>( mpfr::mpreal const& x ) {
    mpfr::mpreal X = x;

    dd_real t;
    for(int i=0; i<2; i++) {
      auto s = (double)X;
      mpfr::mpreal S = s;
      if ( i<2-1 ) {
        if ( (X > 0 && S > X) || (X < 0 && S < X) ) {
           s = s - fp<double>::ufp( s );
        }
        X = X - (double)s;
      }
      t.x[i] = s;
    }
    return t;
  }
#endif
#ifdef  _QD_QD_REAL_H
  template < > qd_real convert<qd_real>( mpfr::mpreal const& x ) {
    mpfr::mpreal X = x;

    qd_real t;
    for(int i=0; i<4; i++) {
      auto s = (double)X;
      mpfr::mpreal S = s;
      if ( i<4-1 ) {
        if ( (X > 0 && S > X) || (X < 0 && S < X) ) {
           s = s - fp<double>::ufp( s );
        }
        X = X - (double)s;
      }
      t.x[i] = s;
    }
    return t;
  }
#endif

}
#endif

