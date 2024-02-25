#ifndef MX_REAL_MPFR_H
#define MX_REAL_MPFR_H

#include <mpreal.h>

namespace mX_real {

  template <> struct check_mX_real <mpfr::mpreal> : std::false_type{};

#define T_mpreal(...)   T_assert( std::is_same< mpfr::mpreal, __VA_ARGS__ >::value )
  template < typename T, T_mpreal(T) > static INLINE auto zero() NOEXCEPT { return mpfr::mpreal(0.); }
  template < typename T, T_mpreal(T) > static INLINE auto one() NOEXCEPT { return mpfr::mpreal(1.); }
  template < typename T, T_mpreal(T) > static INLINE auto two() NOEXCEPT { return mpfr::mpreal(2.); }
  template < typename T, T_mpreal(T) > static INLINE auto epsilon() NOEXCEPT { return mpfr::machine_epsilon(); }
  template < typename T, T_mpreal(T) > static INLINE auto inf() NOEXCEPT { return mpfr::const_infinity(); }
  template < typename T, T_mpreal(T) > static INLINE auto nan() NOEXCEPT { return mpfr::mpreal().setNan(); }
#undef T_mpreal

}

#endif

