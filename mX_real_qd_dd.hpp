#ifndef MX_REAL_QD_DD_H
#define MX_REAL_QD_DD_H

#include <qd/dd_real.h>
#include <qd/qd_real.h>

namespace mX_real {

  template <> struct check_mX_real <dd_real> : std::false_type{};
  template <> struct check_mX_real <qd_real> : std::false_type{};

#define T_DD(...)       T_assert( std::is_same< dd_real, __VA_ARGS__ >::value )
  template < typename T, T_DD(T) > static INLINE auto constexpr zero() NOEXCEPT { return dd_real(0.); }
  template < typename T, T_DD(T) > static INLINE auto constexpr one() NOEXCEPT { return dd_real(1.); }
  template < typename T, T_DD(T) > static INLINE auto constexpr two() NOEXCEPT { return dd_real(2.); }
  template < typename T, T_DD(T) > static INLINE auto constexpr half() NOEXCEPT { return dd_real(.5); }
  template < typename T, T_DD(T) > static INLINE auto constexpr threehalves() NOEXCEPT { return dd_real(1.5); }
  template < typename T, T_DD(T) > static INLINE auto constexpr epsilon() NOEXCEPT { return dd_real::_eps; }
  template < typename T, T_DD(T) > static INLINE auto constexpr inf() NOEXCEPT { return dd_real::_inf; }
  template < typename T, T_DD(T) > static INLINE auto constexpr nan() NOEXCEPT { return dd_real::_nan; }
#undef T_DD

#define T_QD(...)       T_assert( std::is_same< qd_real, __VA_ARGS__ >::value )
  template < typename T, T_QD(T) > static INLINE auto constexpr zero() NOEXCEPT { return qd_real(0.); }
  template < typename T, T_QD(T) > static INLINE auto constexpr one() NOEXCEPT { return qd_real(1.); }
  template < typename T, T_QD(T) > static INLINE auto constexpr two() NOEXCEPT { return qd_real(2.); }
  template < typename T, T_QD(T) > static INLINE auto constexpr half() NOEXCEPT { return qd_real(.5); }
  template < typename T, T_QD(T) > static INLINE auto constexpr threehalves() NOEXCEPT { return qd_real(1.5); }
  template < typename T, T_QD(T) > static INLINE auto constexpr epsilon() NOEXCEPT { return qd_real::_eps; }
  template < typename T, T_QD(T) > static INLINE auto constexpr inf() NOEXCEPT { return qd_real::_inf; }
  template < typename T, T_QD(T) > static INLINE auto constexpr nan() NOEXCEPT { return qd_real::_nan; }
#undef T_QD

}

namespace std {

  INLINE dd_real const sqrt( dd_real const& a ) NOEXCEPT {
    return ::sqrt( a );
  }
  INLINE dd_real const fabs( dd_real const& a ) NOEXCEPT {
    return ::fabs( a );
  }
  INLINE dd_real const abs( dd_real const& a ) NOEXCEPT {
    return std::fabs( a );
  }

  INLINE qd_real const sqrt( qd_real const& a ) NOEXCEPT {
    return ::sqrt( a );
  }
  INLINE qd_real const fabs( qd_real const& a ) NOEXCEPT {
    return ::fabs( a );
  }
  INLINE qd_real const abs( qd_real const& a ) NOEXCEPT {
    return std::fabs( a );
  }

}

#endif

