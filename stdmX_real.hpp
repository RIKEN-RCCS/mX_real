#ifndef	MX_REAL_STD_H
#define	MX_REAL_STD_H

#include "mX_real.hpp"


//
// std:: compatibility of the constant numbers
//
namespace std {

  template < typename T, mX_real::Algorithm A >
  class numeric_limits<mX_real::dX_real<T,A>> {
    using TX = mX_real::dX_real<T,A>;
  public:
    inline static TX epsilon() { return TX::epsilon(); }
    inline static TX infinity() { return TX::inf(); }
    inline static TX quiet_NaN() { return TX::nan(); }

    static int const digits = numeric_limits<T>::digits * TX::L;
  };

  template < typename T, mX_real::Algorithm A >
  bool isnan( mX_real::dX_real<T,A> const& x ) {
    return mX_real::dX_real<T,A>::isnan(x);
  }
  template < typename T, mX_real::Algorithm A >
  bool signbit( mX_real::dX_real<T,A> const& x ) {
    return mX_real::dX_real<T,A>::signbit(x);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::dX_real<T,A> abs( mX_real::dX_real<T,A> const& x ) {
    return mX_real::dX_real<T,A>::abs(x);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::dX_real<T,A> fmin( mX_real::dX_real<T,A> const& x, mX_real::dX_real<T,A> const& y ) {
    return mX_real::dX_real<T,A>::fmin(x,y);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::dX_real<T,A> fmax( mX_real::dX_real<T,A> const& x, mX_real::dX_real<T,A> const& y ) {
    return mX_real::dX_real<T,A>::fmax(x,y);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::dX_real<T,A> sqrt( mX_real::dX_real<T,A> const& x ) {
    return mX_real::dX_real<T,A>::sqrt(x);
  }

}


namespace std {

  template < typename T, mX_real::Algorithm A >
  class numeric_limits<mX_real::tX_real<T,A>> {
    using TX = mX_real::tX_real<T,A>;
  public:
    inline static TX epsilon() { return TX::epsilon(); }
    inline static TX infinity() { return TX::inf(); }
    inline static TX quiet_NaN() { return TX::nan(); }

    static int const digits = numeric_limits<T>::digits * TX::L;
  };

  template < typename T, mX_real::Algorithm A >
  bool isnan( mX_real::tX_real<T,A> const& x ) {
    return mX_real::tX_real<T,A>::isnan(x);
  }
  template < typename T, mX_real::Algorithm A >
  bool sigbit( mX_real::tX_real<T,A> const& x ) {
    return mX_real::tX_real<T,A>::signbit(x);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::tX_real<T,A> abs( mX_real::tX_real<T,A> const& x ) {
    return mX_real::tX_real<T,A>::abs(x);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::tX_real<T,A> fmin( mX_real::tX_real<T,A> const& x, mX_real::tX_real<T,A> const& y ) {
    return mX_real::tX_real<T,A>::fmin(x,y);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::tX_real<T,A> fmax( mX_real::tX_real<T,A> const& x, mX_real::tX_real<T,A> const& y ) {
    return mX_real::tX_real<T,A>::fmax(x,y);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::tX_real<T,A> sqrt( mX_real::tX_real<T,A> const& x ) {
    return mX_real::tX_real<T,A>::sqrt(x);
  }

}


namespace std {

  template < typename T, mX_real::Algorithm A >
  class numeric_limits<mX_real::qX_real<T,A>> {
    using TX = mX_real::qX_real<T,A>;
  public:
    inline static TX epsilon() { return TX::epsilon(); }
    inline static TX infinity() { return TX::inf(); }
    inline static TX quiet_NaN() { return TX::nan(); }

    static int const digits = numeric_limits<T>::digits * TX::L;
  };

  template < typename T, mX_real::Algorithm A >
  bool isnan( mX_real::qX_real<T,A> const& x ) {
    return mX_real::qX_real<T,A>::isnan(x);
  }
  template < typename T, mX_real::Algorithm A >
  bool signbit( mX_real::qX_real<T,A> const& x ) {
    return mX_real::qX_real<T,A>::signbit(x);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::qX_real<T,A> abs( mX_real::qX_real<T,A> const& x ) {
    return mX_real::qX_real<T,A>::abs(x);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::qX_real<T,A> fmin( mX_real::qX_real<T,A> const& x, mX_real::qX_real<T,A> const& y ) {
    return mX_real::qX_real<T,A>::fmin(x,y);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::qX_real<T,A> fmax( mX_real::qX_real<T,A> const& x, mX_real::qX_real<T,A> const& y ) {
    return mX_real::qX_real<T,A>::fmax(x,y);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::qX_real<T,A> sqrt( mX_real::qX_real<T,A> const& x ) {
    return mX_real::qX_real<T,A>::sqrt(x);
  }

}


#endif

