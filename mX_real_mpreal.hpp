#ifndef MX_REAL_MPFR_H
#define MX_REAL_MPFR_H

#include <mpreal.h>

namespace mX_real {

  template <> struct check_mX_real <mpfr::mpreal> : std::false_type{};

#define T_mpreal(...)   T_assert( std::is_same< mpfr::mpreal, __VA_ARGS__ >::value )
  template < typename T, T_mpreal(T) > static INLINE auto zero() { return fp<mpfr::mpreal>::zero(); }
  template < typename T, T_mpreal(T) > static INLINE auto one()  { return fp<mpfr::mpreal>::one(); }
  template < typename T, T_mpreal(T) > static INLINE auto two()  { return fp<mpfr::mpreal>::two(); }
  template < typename T, T_mpreal(T) > static INLINE auto half() { return fp<mpfr::mpreal>::half(); }
  template < typename T, T_mpreal(T) > static INLINE auto epsilon() { return fp<mpfr::mpreal>::epsilon(); }
  template < typename T, T_mpreal(T) > static INLINE auto nan()  { return fp<mpfr::mpreal>::nan(); }
  template < typename T, T_mpreal(T) > static INLINE auto inf()  { return fp<mpfr::mpreal>::inf(); }
#undef T_mpreal

}
#endif

