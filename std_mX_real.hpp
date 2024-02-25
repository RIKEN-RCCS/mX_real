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
    INLINE static TX constexpr epsilon () NOEXCEPT { return TX::epsilon (); }
    INLINE static TX constexpr denorm_min () NOEXCEPT { return TX::denorm_min (); }
    INLINE static TX constexpr min () NOEXCEPT { return TX::min (); }
    INLINE static TX constexpr max () NOEXCEPT { return TX::max (); }
    INLINE static TX constexpr infinity() NOEXCEPT { return TX::inf(); }
    INLINE static TX constexpr quiet_NaN() NOEXCEPT { return TX::nan(); }
    
    static int constexpr digits = numeric_limits<T>::digits * TX::L;
    static int constexpr digits10 = numeric_limits<T>::digits10 * TX::L;
    static int constexpr max_digits10 = numeric_limits<T>::max_digits10 * TX::L;
  };
  
  template < typename T, mX_real::Algorithm A >
  INLINE bool constexpr isinf ( mX_real::dX_real::dx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::dX_real::isinf (x);
  }
  template < typename T, mX_real::Algorithm A >
  INLINE bool constexpr isnan ( mX_real::dX_real::dx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::dX_real::isnan (x);
  }
  template < typename T, mX_real::Algorithm A >
  INLINE bool constexpr signbit ( mX_real::dX_real::dx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::dX_real::signbit (x);
  }
  template < typename T, mX_real::Algorithm A >
  INLINE mX_real::dX_real::dx_real<T,A> constexpr abs ( mX_real::dX_real::dx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::dX_real::abs (x);
  }
  template < typename T, mX_real::Algorithm A >
  INLINE mX_real::dX_real::dx_real<T,A> constexpr fmin ( mX_real::dX_real::dx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::dX_real::fmin (x);
  }
  template < typename T, mX_real::Algorithm A >
  INLINE mX_real::dX_real::dx_real<T,A> constexpr fmax ( mX_real::dX_real::dx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::dX_real::fmax (x);
  }
  template < typename T, mX_real::Algorithm A >
  INLINE mX_real::dX_real::dx_real<T,A> constexpr sqrt ( mX_real::dX_real::dx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::dX_real::sqrt (x);
  }
  
}

namespace std {
  
  template < typename T, mX_real::Algorithm A >
  class numeric_limits<mX_real::tX_real::tx_real<T,A>> {
    using TX = mX_real::tX_real::tx_real<T,A>;
  public:
    INLINE static TX constexpr epsilon () NOEXCEPT { return TX::epsilon (); }
    INLINE static TX constexpr denorm_min () NOEXCEPT { return TX::denorm_min (); }
    INLINE static TX constexpr min () NOEXCEPT { return TX::min (); }
    INLINE static TX constexpr max () NOEXCEPT { return TX::max (); }
    INLINE static TX constexpr infinity() NOEXCEPT { return TX::inf(); }
    INLINE static TX constexpr quiet_NaN() NOEXCEPT { return TX::nan(); }
    
    static int constexpr digits = numeric_limits<T>::digits * TX::L;
    static int constexpr digits10 = numeric_limits<T>::digits10 * TX::L;
    static int constexpr max_digits10 = numeric_limits<T>::max_digits10 * TX::L;
  };
  
  template < typename T, mX_real::Algorithm A >
  INLINE bool constexpr isinf ( mX_real::tX_real::tx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::tX_real::isinf (x);
  }
  template < typename T, mX_real::Algorithm A >
  INLINE bool constexpr isnan ( mX_real::tX_real::tx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::tX_real::isnan (x);
  }
  template < typename T, mX_real::Algorithm A >
  INLINE bool constexpr signbit ( mX_real::tX_real::tx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::tX_real::signbit (x);
  }
  template < typename T, mX_real::Algorithm A >
  INLINE mX_real::tX_real::tx_real<T,A> constexpr abs ( mX_real::tX_real::tx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::tX_real::abs (x);
  }
  template < typename T, mX_real::Algorithm A >
  INLINE mX_real::tX_real::tx_real<T,A> constexpr fmin ( mX_real::tX_real::tx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::tX_real::fmin (x);
  }
  template < typename T, mX_real::Algorithm A >
  INLINE mX_real::tX_real::tx_real<T,A> constexpr fmax ( mX_real::tX_real::tx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::tX_real::fmax (x);
  }
  template < typename T, mX_real::Algorithm A >
  INLINE mX_real::tX_real::tx_real<T,A> constexpr sqrt ( mX_real::tX_real::tx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::tX_real::sqrt (x);
  }
  
}

namespace std {
  
  template < typename T, mX_real::Algorithm A >
  class numeric_limits<mX_real::qX_real::qx_real<T,A>> {
    using TX = mX_real::qX_real::qx_real<T,A>;
  public:
    INLINE static TX constexpr epsilon () NOEXCEPT { return TX::epsilon (); }
    INLINE static TX constexpr denorm_min () NOEXCEPT { return TX::denorm_min (); }
    INLINE static TX constexpr min () NOEXCEPT { return TX::min (); }
    INLINE static TX constexpr max () NOEXCEPT { return TX::max (); }
    INLINE static TX constexpr infinity() NOEXCEPT { return TX::inf(); }
    INLINE static TX constexpr quiet_NaN() NOEXCEPT { return TX::nan(); }
    
    static int constexpr digits = numeric_limits<T>::digits * TX::L;
    static int constexpr digits10 = numeric_limits<T>::digits10 * TX::L;
    static int constexpr max_digits10 = numeric_limits<T>::max_digits10 * TX::L;
  };
  
  template < typename T, mX_real::Algorithm A >
  INLINE bool constexpr isinf ( mX_real::qX_real::qx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::qX_real::isinf (x);
  }
  template < typename T, mX_real::Algorithm A >
  INLINE bool constexpr isnan ( mX_real::qX_real::qx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::qX_real::isnan (x);
  }
  template < typename T, mX_real::Algorithm A >
  INLINE bool constexpr signbit ( mX_real::qX_real::qx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::qX_real::signbit (x);
  }
  template < typename T, mX_real::Algorithm A >
  INLINE mX_real::qX_real::qx_real<T,A> constexpr abs ( mX_real::qX_real::qx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::qX_real::abs (x);
  }
  template < typename T, mX_real::Algorithm A >
  INLINE mX_real::qX_real::qx_real<T,A> constexpr fmin ( mX_real::qX_real::qx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::qX_real::fmin (x);
  }
  template < typename T, mX_real::Algorithm A >
  INLINE mX_real::qX_real::qx_real<T,A> constexpr fmax ( mX_real::qX_real::qx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::qX_real::fmax (x);
  }
  template < typename T, mX_real::Algorithm A >
  INLINE mX_real::qX_real::qx_real<T,A> constexpr sqrt ( mX_real::qX_real::qx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::qX_real::sqrt (x);
  }
  
}

#endif
