#ifndef MX_REAL_MPREAL_H
#define MX_REAL_MPREAL_H

#include <mpreal.h>

namespace mX_real {

  template <> struct check_mX_real <mpfr::mpreal> : std::false_type{};

#if __cplusplus < 201703L || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_CLANG_COMPILER)
#  define       STATIC_VAR      static
#  define       STATIC_INIT(...)	/* */
#else
#  define       STATIC_VAR      static inline
#  define       STATIC_INIT(...)	= ( __VA_ARGS__ )
#endif
  template <>
  struct fp<mpfr::mpreal> {
    static bool   constexpr value = true;
    static bool   constexpr use_dynamic_digits = true;

    STATIC_VAR mpfr::mpreal const zero       STATIC_INIT( mpfr::mpreal(0.) );
    STATIC_VAR mpfr::mpreal const one        STATIC_INIT( mpfr::mpreal(1.) );
    STATIC_VAR mpfr::mpreal const two        STATIC_INIT( mpfr::mpreal(2.) );
    STATIC_VAR mpfr::mpreal const half       STATIC_INIT( mpfr::mpreal(.5) );

    STATIC_VAR mpfr::mpreal const epsilon    STATIC_INIT( mpfr::machine_epsilon() );
    STATIC_VAR mpfr::mpreal const epsiloni   STATIC_INIT( one/epsilon );
    STATIC_VAR mpfr::mpreal const nan        STATIC_INIT( mpfr::mpreal().setNan() );
    STATIC_VAR mpfr::mpreal const inf        STATIC_INIT( mpfr::const_infinity() );
    STATIC_VAR mpfr::mpreal const denorm_min STATIC_INIT( std::numeric_limits<mpfr::mpreal>::denorm_min() );
    STATIC_VAR mpfr::mpreal const min        STATIC_INIT( std::numeric_limits<mpfr::mpreal>::min() );
    STATIC_VAR mpfr::mpreal const max        STATIC_INIT( std::numeric_limits<mpfr::mpreal>::max() );

    STATIC_VAR int const digits              STATIC_INIT( std::numeric_limits<mpfr::mpreal>::digits() );
    STATIC_VAR int const digits10            STATIC_INIT( std::numeric_limits<mpfr::mpreal>::digits10() );
    STATIC_VAR int const max_digits10        STATIC_INIT( std::numeric_limits<mpfr::mpreal>::max_digits10() );
  };
#undef	STATIC_VAR
#undef	STATIC_INIT

#if __cplusplus < 201703L || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_CLANG_COMPILER)
  mpfr::mpreal  const fp<mpfr::mpreal>::zero         = mpfr::mpreal(0.);
  mpfr::mpreal  const fp<mpfr::mpreal>::one          = mpfr::mpreal(1.);
  mpfr::mpreal  const fp<mpfr::mpreal>::two          = mpfr::mpreal(2.);
  mpfr::mpreal  const fp<mpfr::mpreal>::half         = mpfr::mpreal(.5);
  mpfr::mpreal  const fp<mpfr::mpreal>::epsilon      = mpfr::machine_epsilon();
  mpfr::mpreal  const fp<mpfr::mpreal>::epsiloni     = mpfr::frac( mpfr::machine_epsilon() );
  mpfr::mpreal  const fp<mpfr::mpreal>::inf          = mpfr::const_infinity();
  mpfr::mpreal  const fp<mpfr::mpreal>::nan          = mpfr::mpreal().setNan();
  mpfr::mpreal  const fp<mpfr::mpreal>::denorm_min   = std::numeric_limits<mpfr::mpreal>::denorm_min();
  mpfr::mpreal  const fp<mpfr::mpreal>::min          = std::numeric_limits<mpfr::mpreal>::min();
  mpfr::mpreal  const fp<mpfr::mpreal>::max          = std::numeric_limits<mpfr::mpreal>::max();
  int           const fp<mpfr::mpreal>::digits       = std::numeric_limits<mpfr::mpreal>::digits();
  int           const fp<mpfr::mpreal>::digits10     = std::numeric_limits<mpfr::mpreal>::digits10();
  int           const fp<mpfr::mpreal>::max_digits10 = std::numeric_limits<mpfr::mpreal>::max_digits10();
#endif

#define T_mpreal(...)   T_assert( std::is_same< mpfr::mpreal, __VA_ARGS__ >::value )
  template < typename T, T_mpreal(T) > static INLINE auto zero()    NOEXCEPT { return fp<mpfr::mpreal>::zero; }
  template < typename T, T_mpreal(T) > static INLINE auto one()     NOEXCEPT { return fp<mpfr::mpreal>::one; }
  template < typename T, T_mpreal(T) > static INLINE auto two()     NOEXCEPT { return fp<mpfr::mpreal>::two; }
  template < typename T, T_mpreal(T) > static INLINE auto half()    NOEXCEPT { return fp<mpfr::mpreal>::half; }
  template < typename T, T_mpreal(T) > static INLINE auto epsilon() NOEXCEPT { return fp<mpfr::mpreal>::epsilon; }
  template < typename T, T_mpreal(T) > static INLINE auto nan()     NOEXCEPT { return fp<mpfr::mpreal>::nan; }
  template < typename T, T_mpreal(T) > static INLINE auto inf()     NOEXCEPT { return fp<mpfr::mpreal>::inf; }
#undef T_mpreal

}

namespace std {

  INLINE mpfr::mpreal const fma( mpfr::mpreal const& a, mpfr::mpreal const& b, mpfr::mpreal const& c ) NOEXCEPT {
    return mpfr::fma( a, b, c );
  }

}

#endif

