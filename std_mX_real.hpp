#ifndef	MX_REAL_STD_H
#define	MX_REAL_STD_H

#include "mX_real.hpp"

//
// std:: compatibility of the constant numbers and math-fuctions
//

namespace std {
  
  template < typename T, mX_real::Algorithm A >
  class numeric_limits<mX_real::dX_real::dx_real<T,A>> {
    using TX = mX_real::dX_real::dx_real<T,A>;
  public:
    inline static TX constexpr epsilon () { return TX::epsilon (); }
    inline static TX constexpr denorm_min () { return TX::denorm_min (); }
    inline static TX constexpr min () { return TX::min (); }
    inline static TX constexpr max () { return TX::max (); }
    inline static TX constexpr infinity() { return TX::inf(); }
    inline static TX constexpr quiet_NaN() { return TX::nan(); }
    
    static int constexpr digits = numeric_limits<T>::digits * TX::L;
    static int constexpr digits10 = numeric_limits<T>::digits10 * TX::L;
    static int constexpr max_digits10 = numeric_limits<T>::max_digits10 * TX::L;
  };
  
  template < typename T, mX_real::Algorithm A >
  bool constexpr isinf ( mX_real::dX_real::dx_real<T,A> const& x ) {
    return mX_real::dX_real::isinf (x);
  }
  template < typename T, mX_real::Algorithm A >
  bool constexpr isnan ( mX_real::dX_real::dx_real<T,A> const& x ) {
    return mX_real::dX_real::isnan (x);
  }
  template < typename T, mX_real::Algorithm A >
  bool constexpr signbit ( mX_real::dX_real::dx_real<T,A> const& x ) {
    return mX_real::dX_real::signbit (x);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::dX_real::dx_real<T,A> constexpr abs ( mX_real::dX_real::dx_real<T,A> const& x ) {
    return mX_real::dX_real::abs (x);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::dX_real::dx_real<T,A> constexpr fmin ( mX_real::dX_real::dx_real<T,A> const& x ) {
    return mX_real::dX_real::fmin (x);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::dX_real::dx_real<T,A> constexpr fmax ( mX_real::dX_real::dx_real<T,A> const& x ) {
    return mX_real::dX_real::fmax (x);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::dX_real::dx_real<T,A> constexpr sqrt ( mX_real::dX_real::dx_real<T,A> const& x ) {
    return mX_real::dX_real::sqrt (x);
  }
  
}

namespace std {
  
  template < typename T, mX_real::Algorithm A >
  class numeric_limits<mX_real::tX_real::tx_real<T,A>> {
    using TX = mX_real::tX_real::tx_real<T,A>;
  public:
    inline static TX constexpr epsilon () { return TX::epsilon (); }
    inline static TX constexpr denorm_min () { return TX::denorm_min (); }
    inline static TX constexpr min () { return TX::min (); }
    inline static TX constexpr max () { return TX::max (); }
    inline static TX constexpr infinity() { return TX::inf(); }
    inline static TX constexpr quiet_NaN() { return TX::nan(); }
    
    static int constexpr digits = numeric_limits<T>::digits * TX::L;
    static int constexpr digits10 = numeric_limits<T>::digits10 * TX::L;
    static int constexpr max_digits10 = numeric_limits<T>::max_digits10 * TX::L;
  };
  
  template < typename T, mX_real::Algorithm A >
  bool constexpr isinf ( mX_real::tX_real::tx_real<T,A> const& x ) {
    return mX_real::tX_real::isinf (x);
  }
  template < typename T, mX_real::Algorithm A >
  bool constexpr isnan ( mX_real::tX_real::tx_real<T,A> const& x ) {
    return mX_real::tX_real::isnan (x);
  }
  template < typename T, mX_real::Algorithm A >
  bool constexpr signbit ( mX_real::tX_real::tx_real<T,A> const& x ) {
    return mX_real::tX_real::signbit (x);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::tX_real::tx_real<T,A> constexpr abs ( mX_real::tX_real::tx_real<T,A> const& x ) {
    return mX_real::tX_real::abs (x);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::tX_real::tx_real<T,A> constexpr fmin ( mX_real::tX_real::tx_real<T,A> const& x ) {
    return mX_real::tX_real::fmin (x);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::tX_real::tx_real<T,A> constexpr fmax ( mX_real::tX_real::tx_real<T,A> const& x ) {
    return mX_real::tX_real::fmax (x);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::tX_real::tx_real<T,A> constexpr sqrt ( mX_real::tX_real::tx_real<T,A> const& x ) {
    return mX_real::tX_real::sqrt (x);
  }
  
}

namespace std {
  
  template < typename T, mX_real::Algorithm A >
  class numeric_limits<mX_real::qX_real::qx_real<T,A>> {
    using TX = mX_real::qX_real::qx_real<T,A>;
  public:
    inline static TX constexpr epsilon () { return TX::epsilon (); }
    inline static TX constexpr denorm_min () { return TX::denorm_min (); }
    inline static TX constexpr min () { return TX::min (); }
    inline static TX constexpr max () { return TX::max (); }
    inline static TX constexpr infinity() { return TX::inf(); }
    inline static TX constexpr quiet_NaN() { return TX::nan(); }
    
    static int constexpr digits = numeric_limits<T>::digits * TX::L;
    static int constexpr digits10 = numeric_limits<T>::digits10 * TX::L;
    static int constexpr max_digits10 = numeric_limits<T>::max_digits10 * TX::L;
  };
  
  template < typename T, mX_real::Algorithm A >
  bool constexpr isinf ( mX_real::qX_real::qx_real<T,A> const& x ) {
    return mX_real::qX_real::isinf (x);
  }
  template < typename T, mX_real::Algorithm A >
  bool constexpr isnan ( mX_real::qX_real::qx_real<T,A> const& x ) {
    return mX_real::qX_real::isnan (x);
  }
  template < typename T, mX_real::Algorithm A >
  bool constexpr signbit ( mX_real::qX_real::qx_real<T,A> const& x ) {
    return mX_real::qX_real::signbit (x);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::qX_real::qx_real<T,A> constexpr abs ( mX_real::qX_real::qx_real<T,A> const& x ) {
    return mX_real::qX_real::abs (x);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::qX_real::qx_real<T,A> constexpr fmin ( mX_real::qX_real::qx_real<T,A> const& x ) {
    return mX_real::qX_real::fmin (x);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::qX_real::qx_real<T,A> constexpr fmax ( mX_real::qX_real::qx_real<T,A> const& x ) {
    return mX_real::qX_real::fmax (x);
  }
  template < typename T, mX_real::Algorithm A >
  mX_real::qX_real::qx_real<T,A> constexpr sqrt ( mX_real::qX_real::qx_real<T,A> const& x ) {
    return mX_real::qX_real::sqrt (x);
  }
  
}

#endif
