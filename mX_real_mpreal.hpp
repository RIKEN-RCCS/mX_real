#pragma once

#include <mpreal.h>


namespace mX_real {

  template <> struct check_mX_real <mpfr::mpreal> : std::false_type{};


namespace QxW {

  template <> struct fp_const<mpfr::mpreal> {
    static INLINE auto const zero() NOEXCEPT { return mpfr::mpreal(0.); }
    static INLINE auto const one() NOEXCEPT { return mpfr::mpreal(1.); }
    static INLINE auto const two() NOEXCEPT { return mpfr::mpreal(2.); }
    static INLINE auto const nhalf() NOEXCEPT { return mpfr::mpreal(.5); }
    static INLINE auto const threehalves() NOEXCEPT { return mpfr::mpreal(1.5); }
    static INLINE auto const near_pow2(const mpfr::mpreal& x, mp_rnd_t r = mpfr::mpreal::get_default_rnd())
    {
      mpfr::mpreal y(1, x.getPrecision());
      if(!iszero(x)) y = floor(log2(abs(x,r),r));
      return pow(mpfr::mpreal(2,x.getPrecision()),y,r);
    }
    static INLINE auto const exponent( mpfr::mpreal const& a ) NOEXCEPT { return near_pow2( a ); }
    static INLINE auto const exponenti( mpfr::mpreal const& a ) NOEXCEPT { return one()/exponent( a ); }
  };

}

  template <>
  struct fp<mpfr::mpreal> {
    static bool   constexpr value = true;
    using _fp_ = QxW::fp_const<mpfr::mpreal>;

    static INLINE auto const zero()       NOEXCEPT { return _fp_::zero(); }
    static INLINE auto const one()        NOEXCEPT { return _fp_::one(); }
    static INLINE auto const two()        NOEXCEPT { return _fp_::two(); }
    static INLINE auto const nhalf()      NOEXCEPT { return _fp_::nhalf(); }
    static INLINE auto const threehalves() NOEXCEPT { return _fp_::threehalves(); }

    static INLINE auto const epsilon()    NOEXCEPT { return mpfr::machine_epsilon(); }
    static INLINE auto const epsiloni()   NOEXCEPT { return one()/epsilon(); }
    static INLINE auto const nan()        NOEXCEPT { return mpfr::mpreal().setNan(); }
    static INLINE auto const inf()        NOEXCEPT { return mpfr::const_infinity(); }
    static INLINE auto const denorm_min() NOEXCEPT { return std::numeric_limits<mpfr::mpreal>::denorm_min(); }
    static INLINE auto const min()        NOEXCEPT { return std::numeric_limits<mpfr::mpreal>::min(); }
    static INLINE auto const max()        NOEXCEPT { return std::numeric_limits<mpfr::mpreal>::max(); }

    static INLINE auto const digits()       NOEXCEPT { return std::numeric_limits<mpfr::mpreal>::digits(); }
    static INLINE auto const digits10()     NOEXCEPT { return std::numeric_limits<mpfr::mpreal>::digits10(); }
    static INLINE auto const max_digits10() NOEXCEPT { return std::numeric_limits<mpfr::mpreal>::max_digits10(); }

    static INLINE auto const isinf ( mpfr::mpreal const& a ) NOEXCEPT { return mpfr::isinf( a ); }
    static INLINE auto const isnan ( mpfr::mpreal const& a ) NOEXCEPT { return mpfr::isnan( a ); }
    static INLINE auto const copysign ( mpfr::mpreal const& a, mpfr::mpreal const& b ) NOEXCEPT { return mpfr::copysign( a, b ); }
    static INLINE auto const signbit ( mpfr::mpreal const& a ) NOEXCEPT { return mpfr::signbit( a ); }
    static INLINE auto const is_zero ( mpfr::mpreal const& a ) NOEXCEPT { return mpfr::iszero( a ); }
    static INLINE auto const is_positive ( mpfr::mpreal const& a ) NOEXCEPT { return (zero() < a); }
    static INLINE auto const is_negative ( mpfr::mpreal const& a ) NOEXCEPT { return (zero() > a); }
  };
#undef	STATIC_VAR
#undef	STATIC_INIT

#define T_mpreal(...)   T_assert( std::is_same< mpfr::mpreal, __VA_ARGS__ >::value )
  //
  // templated fuction style
  // zero<mpfr::mpreal>()
  //
  template < typename T, T_mpreal(T) > static INLINE auto const zero()    NOEXCEPT { return fp<mpfr::mpreal>::zero(); }
  template < typename T, T_mpreal(T) > static INLINE auto const one()     NOEXCEPT { return fp<mpfr::mpreal>::one(); }
  template < typename T, T_mpreal(T) > static INLINE auto const two()     NOEXCEPT { return fp<mpfr::mpreal>::two(); }
  template < typename T, T_mpreal(T) > static INLINE auto const nhalf()   NOEXCEPT { return fp<mpfr::mpreal>::nhalf(); }
  template < typename T, T_mpreal(T) > static INLINE auto const epsilon() NOEXCEPT { return fp<mpfr::mpreal>::epsilon(); }
  template < typename T, T_mpreal(T) > static INLINE auto const nan()     NOEXCEPT { return fp<mpfr::mpreal>::nan(); }
  template < typename T, T_mpreal(T) > static INLINE auto const inf()     NOEXCEPT { return fp<mpfr::mpreal>::inf(); }
#undef T_mpreal

}

namespace std {

#if 0
  template < >
  struct is_arithmetic < mpfr::mpreal > { static bool constexpr value = true; };
#endif

  INLINE mpfr::mpreal const fabs( mpfr::mpreal const& a ) NOEXCEPT {
    return mpfr::fabs( a );
  }
  INLINE mpfr::mpreal const sqrt( mpfr::mpreal const& a ) NOEXCEPT {
    return mpfr::sqrt( a );
  }
  INLINE mpfr::mpreal const fma( mpfr::mpreal const& a, mpfr::mpreal const& b, mpfr::mpreal const& c ) NOEXCEPT {
    return mpfr::fma( a, b, c );
  }
  INLINE auto const signbit( mpfr::mpreal const& a ) NOEXCEPT {
    return mpfr::signbit( a );
  }

}


