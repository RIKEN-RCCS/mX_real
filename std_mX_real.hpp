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
    
#if __cplusplus < 201703L || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_CLANG_COMPILER)
#  define       STATIC_VAR      static
#  define       STATIC_INIT(...)         /* */
#else
#  define       STATIC_VAR      static inline
#  define       STATIC_INIT(...)        = ( __VA_ARGS__ )
#endif
    STATIC_VAR int const digits STATIC_INIT( mX_real::fp<T>::digits() * TX::L );
    STATIC_VAR int const digits10 STATIC_INIT( mX_real::fp<T>::digits10() * TX::L );
    STATIC_VAR int const max_digits10 STATIC_INIT( mX_real::fp<T>::max_digits10() * TX::L );
  };
#undef STATIC_VAR
#undef STATIC_INIT

#if __cplusplus < 201703L || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_CLANG_COMPILER)
  template < typename T, mX_real::Algorithm A >
  int const numeric_limits<mX_real::dX_real::dx_real<T,A>>::digits = mX_real::fp<T>::digits() * mX_real::dX_real::dx_real<T,A>::L;
  template < typename T, mX_real::Algorithm A >
  int const numeric_limits<mX_real::dX_real::dx_real<T,A>>::digits10 = mX_real::fp<T>::digits10() * mX_real::dX_real::dx_real<T,A>::L;
  template < typename T, mX_real::Algorithm A >
  int const numeric_limits<mX_real::dX_real::dx_real<T,A>>::max_digits10 = mX_real::fp<T>::max_digits10() * mX_real::dX_real::dx_real<T,A>::L;
#endif
  
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
  INLINE mX_real::dX_real::dx_real<T,A> constexpr fabs ( mX_real::dX_real::dx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::dX_real::fabs (x);
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
    
#if __cplusplus < 201703L || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_CLANG_COMPILER)
#  define       STATIC_VAR      static
#  define       STATIC_INIT(...)         /* */
#else
#  define       STATIC_VAR      static inline
#  define       STATIC_INIT(...)        = ( __VA_ARGS__ )
#endif
    STATIC_VAR int const digits STATIC_INIT( mX_real::fp<T>::digits() * TX::L );
    STATIC_VAR int const digits10 STATIC_INIT( mX_real::fp<T>::digits10() * TX::L );
    STATIC_VAR int const max_digits10 STATIC_INIT( mX_real::fp<T>::max_digits10() * TX::L );
  };
#undef STATIC_VAR
#undef STATIC_INIT

#if __cplusplus < 201703L || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_CLANG_COMPILER)
  template < typename T, mX_real::Algorithm A >
  int const numeric_limits<mX_real::tX_real::tx_real<T,A>>::digits = mX_real::fp<T>::digits() * mX_real::tX_real::tx_real<T,A>::L;
  template < typename T, mX_real::Algorithm A >
  int const numeric_limits<mX_real::tX_real::tx_real<T,A>>::digits10 = mX_real::fp<T>::digits10() * mX_real::tX_real::tx_real<T,A>::L;
  template < typename T, mX_real::Algorithm A >
  int const numeric_limits<mX_real::tX_real::tx_real<T,A>>::max_digits10 = mX_real::fp<T>::max_digits10() * mX_real::tX_real::tx_real<T,A>::L;
#endif
  
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
  INLINE mX_real::tX_real::tx_real<T,A> constexpr fabs ( mX_real::tX_real::tx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::tX_real::fabs (x);
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
    
#if __cplusplus < 201703L || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_CLANG_COMPILER)
#  define       STATIC_VAR      static
#  define       STATIC_INIT(...)         /* */
#else
#  define       STATIC_VAR      static inline
#  define       STATIC_INIT(...)        = ( __VA_ARGS__ )
#endif
    STATIC_VAR int const digits STATIC_INIT( mX_real::fp<T>::digits() * TX::L );
    STATIC_VAR int const digits10 STATIC_INIT( mX_real::fp<T>::digits10() * TX::L );
    STATIC_VAR int const max_digits10 STATIC_INIT( mX_real::fp<T>::max_digits10() * TX::L );
  };
#undef STATIC_VAR
#undef STATIC_INIT

#if __cplusplus < 201703L || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_CLANG_COMPILER)
  template < typename T, mX_real::Algorithm A >
  int const numeric_limits<mX_real::qX_real::qx_real<T,A>>::digits = mX_real::fp<T>::digits() * mX_real::qX_real::qx_real<T,A>::L;
  template < typename T, mX_real::Algorithm A >
  int const numeric_limits<mX_real::qX_real::qx_real<T,A>>::digits10 = mX_real::fp<T>::digits10() * mX_real::qX_real::qx_real<T,A>::L;
  template < typename T, mX_real::Algorithm A >
  int const numeric_limits<mX_real::qX_real::qx_real<T,A>>::max_digits10 = mX_real::fp<T>::max_digits10() * mX_real::qX_real::qx_real<T,A>::L;
#endif
  
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
  INLINE mX_real::qX_real::qx_real<T,A> constexpr fabs ( mX_real::qX_real::qx_real<T,A> const& x ) NOEXCEPT {
    return mX_real::qX_real::fabs (x);
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
