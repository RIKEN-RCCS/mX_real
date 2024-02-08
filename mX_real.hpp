#ifndef MX_REAL_H
#define MX_REAL_H

#include <assert.h>

#include <iosfwd>
#include <sstream>
#include <bitset>
#include <limits>
#include <math.h>

#ifdef USE_MPREAL
#include "mpreal.h"
#endif

#define	INLINE	__always_inline
#define	MX_REAL_USE_INF_NAN_EXCEPTION	0

#if 0
#include <boost/type_index.hpp>
template < typename T > void printTYPE( void ) {
  std::cout << boost::typeindex::type_id_with_cvr<T>().pretty_name(); }
#endif

namespace mX_real {

#include "Ozaki-QW/qxw.hpp"

  //
  // Enum-Class definition of the internal Algorithms
  //
  enum class Algorithm {
                        //
                        Accurate = 0,
                        Sloppy   = 1,
                        Quasi    = 2,
                        //
                        // name aliasing
                        IEEE     = Accurate,
                        PairWise = Quasi,
                        PairArithmetic = Quasi,
                        Default  = Accurate
  };

  template < Algorithm Aa = Algorithm::Accurate, Algorithm Ab = Algorithm::Accurate >
  struct commonAlgorithm {
    static Algorithm constexpr algorithm =
      ( Aa == Algorithm::Accurate && Ab == Algorithm::Accurate ) ?
      Algorithm::Accurate :
      ( Aa == Algorithm::Quasi || Ab == Algorithm::Quasi ) ?
      Algorithm::Quasi : Algorithm::Sloppy;
  };

  template < Algorithm A >
  struct accurateAlgorithm {
    static Algorithm constexpr algorithm =
      A == Algorithm::Quasi ? Algorithm::Sloppy : Algorithm::Accurate;
  };

  template < Algorithm A >
  struct inaccurateAlgorithm {
    static Algorithm constexpr algorithm =
      A == Algorithm::Accurate ? Algorithm::Sloppy : Algorithm::Quasi;
  };

  std::string toString( Algorithm A ) {
    if ( A==Algorithm::Quasi ) return "Quasi";
    if ( A==Algorithm::Sloppy ) return "Sloppy";
    if ( A==Algorithm::Accurate ) return "Accurate";
    return "Unknow";
  }

  //
  // generic terms (const, func, and bit ops)
  //
#if defined(__INTEL_COMPILER)
#  define	STATIC_VAR	static inline
#elif defined(__INTEL_LLVM_COMPILER)||defined(__INTEL_CLANG_COMPILER)
#  define	STATIC_VAR	static
#elif __cplusplus < 201703L
#  define	STATIC_VAR	static
#else
#  define	STATIC_VAR	static inline
#endif
  template < typename T >
  struct fp { static bool constexpr value = false; };
  //
  template <>
  struct fp<float> {
    static bool   constexpr value = true;

    STATIC_VAR float constexpr zero    = float(0);
    STATIC_VAR float constexpr one     = float(1);
    STATIC_VAR float constexpr two     = float(2);
    STATIC_VAR float constexpr half    = one/two;

    STATIC_VAR float constexpr epsilon    = std::numeric_limits<float>::epsilon();
    STATIC_VAR float constexpr epsiloni   = one/std::numeric_limits<float>::epsilon();
    STATIC_VAR float constexpr nan        = std::numeric_limits<float>::quiet_NaN();
    STATIC_VAR float constexpr inf        = std::numeric_limits<float>::infinity();
    STATIC_VAR float constexpr denorm_min = std::numeric_limits<float>::denorm_min();
    STATIC_VAR float constexpr min        = std::numeric_limits<float>::min();
    STATIC_VAR float constexpr max        = std::numeric_limits<float>::max();

    static INLINE auto constexpr isinf       ( float  const a ) {
      return std::isinf( a );
    }
    static INLINE auto constexpr isnan       ( float  const a ) {
      return std::isnan( a );
    }
    static INLINE auto constexpr copysign    ( float  const a, float  const b ) {
      return std::copysign( a, b );
    }
    static INLINE auto constexpr signbit     ( float  const a ) {
      return std::signbit( a );
    }
    static INLINE auto constexpr is_zero     ( float  const a ) {
      return a == fp<float>::zero;
    }
    static INLINE auto constexpr is_positive ( float  const a ) {
      return a >  fp<float>::zero;
    }
    static INLINE auto constexpr is_negative ( float  const a ) {
      return a <  fp<float>::zero;
    }
    static INLINE auto constexpr fabs ( float const a ) {
      return std::fabs( a );
    }
    static INLINE auto constexpr sqrt ( float const a ) {
      return std::sqrt( a );
    }
    static INLINE auto constexpr fma ( float const a, float const b, float const c ) {
      return std::fma( a, b, c );
    }
    static INLINE auto const rand () {
      return std::rand();
    }

    static INLINE auto constexpr hbit( float const a ) {
      return QxW::fp_const<float>::ulp( a );
    }
    static INLINE auto constexpr ulp( float const a ) {
      return QxW::fp_const<float>::ulp( a );
    }
    static INLINE auto constexpr exponent( float const a ) {
      return QxW::fp_const<float>::exponent( a );
    }
    static INLINE auto constexpr exponenti( float const a ) {
      return QxW::fp_const<float>::exponenti( a );
    }
  };
  template <>
  struct fp<double> {
    static bool   constexpr value = true;

    STATIC_VAR double constexpr zero    = double(0);
    STATIC_VAR double constexpr one     = double(1);
    STATIC_VAR double constexpr two     = double(2);
    STATIC_VAR double constexpr half    = one/two;

    STATIC_VAR double constexpr epsilon    = std::numeric_limits<double>::epsilon();
    STATIC_VAR double constexpr epsiloni   = one/std::numeric_limits<double>::epsilon();
    STATIC_VAR double constexpr nan        = std::numeric_limits<double>::quiet_NaN();
    STATIC_VAR double constexpr inf        = std::numeric_limits<double>::infinity();
    STATIC_VAR double constexpr denorm_min = std::numeric_limits<double>::denorm_min();
    STATIC_VAR double constexpr min        = std::numeric_limits<double>::min();
    STATIC_VAR double constexpr max        = std::numeric_limits<double>::max();

    static INLINE auto constexpr isinf       ( double  const a ) {
      return std::isinf( a );
    }
    static INLINE auto constexpr isnan       ( double  const a ) {
      return std::isnan( a );
    }
    static INLINE auto constexpr copysign    ( double  const a, double  const b ) {
      return std::copysign( a, b );
    }
    static INLINE auto constexpr signbit     ( double  const a ) {
      return std::signbit( a );
    }
    static INLINE auto constexpr is_zero     ( double  const a ) {
      return a == fp<double>::zero;
    }
    static INLINE auto constexpr is_positive ( double  const a ) {
      return a >  fp<double>::zero;
    }
    static INLINE auto constexpr is_negative ( double  const a ) {
      return a <  fp<double>::zero;
    }
    static INLINE auto constexpr fabs ( double const a ) {
      return std::fabs( a );
    }
    static INLINE auto constexpr sqrt ( double const a ) {
      return std::sqrt( a );
    }
    static INLINE auto constexpr fma ( double const a, double const b, double const c ) {
      return std::fma( a, b, c );
    }
    static INLINE auto const rand () {
      return std::rand();
    }

    static INLINE auto constexpr hbit( double const a ) {
      return QxW::fp_const<double>::ulp( a );
    }
    static INLINE auto constexpr ulp( double const a ) {
      return QxW::fp_const<double>::ulp( a );
    }
    static INLINE auto constexpr exponent( double const a ) {
      return QxW::fp_const<double>::exponent( a );
    }
    static INLINE auto constexpr exponenti( double const a ) {
      return QxW::fp_const<double>::exponenti( a );
    }
  };
  //
#undef STATIC_VAR
  //
#ifndef __INTEL_COMPILER
#if __cplusplus < 201703L
  float  constexpr fp<float>::zero;
  float  constexpr fp<float>::one;
  float  constexpr fp<float>::two;
  float  constexpr fp<float>::half;
  float  constexpr fp<float>::epsilon;
  float  constexpr fp<float>::inf;
  float  constexpr fp<float>::nan;
  float  constexpr fp<float>::denorm_min;
  float  constexpr fp<float>::min;
  float  constexpr fp<float>::max;
  double  constexpr fp<double>::zero;
  double  constexpr fp<double>::one;
  double  constexpr fp<double>::two;
  double  constexpr fp<double>::half;
  double  constexpr fp<double>::epsilon;
  double  constexpr fp<double>::inf;
  double  constexpr fp<double>::nan;
  double  constexpr fp<double>::denorm_min;
  double  constexpr fp<double>::min;
  double  constexpr fp<double>::max;
#endif
#endif
  //


  //
  // common operation functions
  //
  template < typename T >
  INLINE auto constexpr twoSum ( T const a, T const b, T & __restrict__ s, T & __restrict__ e ) {
    QxW::TwoSum( a, b, s, e );
  }
  template < typename T >
  INLINE auto constexpr twoSum ( T & __restrict__ a, T & __restrict__ b ) {
    twoSum( a, b, a, b );
  }

  //
  template < typename T >
  INLINE auto constexpr quickSum ( T const a, T const b, T & __restrict__ s, T & __restrict__ e ) {
    QxW::FastTwoSum( a, b, s, e );
  }
  template < typename T >
  INLINE auto constexpr quickSum ( T & __restrict__ a, T & __restrict__ b ) {
    quickSum( a, b, a, b );
  }

  //
  template < typename T >
  INLINE auto constexpr twoProdFMA ( T const a, T const b, T & __restrict__ s, T & __restrict__ e ) {
    QxW::TwoProductFMA( a, b, s, e );
  }

  //
  template < typename T >
  INLINE auto constexpr copy_with_rounding( T * __restrict__ dest, T const * __restrict__ src, int const L, int const LL ) {
    if ( L < LL ) {
      for(int i=0; i<L; i++) { dest[i] = src[i]; }

      if ( src[L] == fp<T>::zero ) return;
      if ( ( 0x1 & QxW::fp_const<T>::fp2uint( src[L-1] ) ) != 0x1 ) return;

      auto const a =  QxW::fp_const<T>::ulp( src[L-1] );
      auto const b =  QxW::fp_const<T>::hbit( src[L] ) * 2;
      if ( signbit( src[L-1] ) ^ signbit( src[L] ) ) {
        // +11.0/-.1 -> +10.1 -> + 10 ---
        // +10.0/-.1 -> +01.1 -> + 10
        // +01.0/-.1 -> +00.1 -> + 00 ---
        // +00.0/-.1 -> +11.1 -> + 00
        if ( -a == b ) { dest[L-1] -= a; }
      } else {
        // +11.0/+.1 -> +11.1 -> + 00 +++
        // +10.0/+.1 -> +10.1 -> + 10
        // +01.0/+.1 -> +01.1 -> + 10 +++
        // +00.0/+.1 -> +00.1 -> + 00
        if ( +a == b ) { dest[L-1] += a; }
      }
    } else if ( L == LL ) {
      for(int i=0; i<L; i++) { dest[i] = src[i]; }
    } else {
      for(int i=0; i<LL; i++) { dest[i] = src[i]; }
      for(int i=LL; i<L; i++) { dest[i] = fp<T>::zero; }
    }
  }


  // for cross-reference
  namespace dX_real { template < typename T, Algorithm A > struct dx_real; }
  namespace tX_real { template < typename T, Algorithm A > struct tx_real; }
  namespace qX_real { template < typename T, Algorithm A > struct qx_real; }


  //
  // Normalization policy
  // if O(|x0|)>O(|x1|)>... => quickSum in a decending (w.r.t. O(e)) order once
  // not guaranteed => TwoSum in an ascending order, then quickSum up-and-down order once or more
  // N_itr :  0 => Sloppy
  //       : >1 => Quasi
  //
  template < int N_accuracy = 0, typename T, Algorithm A >
  INLINE auto constexpr Normalize( dX_real::dx_real<T,A> & c ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    auto t = c.x[0] + c.x[1];
    if ( std::isnan( t ) || std::isinf( t ) ) {
      c.x[0] = c.x[1] = t;
    } else
#endif
      {
        if ( N_accuracy > 0 || A == Algorithm::Quasi ) {
          twoSum( c.x[0], c.x[1] );
        } else {
          quickSum( c.x[0], c.x[1] );
        }
      }
  }
  //
  template < int N_accuracy = 0, typename T, Algorithm A >
  INLINE auto constexpr Normalize( tX_real::tx_real<T,A> & c ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    auto t = c.x[0] + c.x[1] + c.x[2];
    if ( std::isnan( t ) || std::isinf( t ) ) {
      c.x[0] = c.x[1] = c.x[2] = t;
    } else
#endif
      {
        if ( N_accuracy > 0 || A == Algorithm::Quasi ) {
          twoSum( c.x[0], c.x[1] );
          twoSum( c.x[1], c.x[2] );
        } else {
          quickSum( c.x[0], c.x[1] );
          quickSum( c.x[1], c.x[2] );
        }
        if ( N_accuracy > 1 ) {
          quickSum( c.x[0], c.x[1] );
          quickSum( c.x[1], c.x[2] );
	}
        quickSum( c.x[0], c.x[1] );
      }
  }
  //
  template < int N_accuracy = 0, typename T, Algorithm A >
  INLINE auto constexpr Normalize( qX_real::qx_real<T,A> & c ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    auto t = c.x[0] + c.x[1] + c.x[2] + c.x[3];
    if ( std::isnan( t ) || std::isinf( t ) ) {
      c.x[0] = c.x[1] = c.x[2] = c.x[3] = t;
    } else
#endif
      {
        if ( N_accuracy > 0 || A == Algorithm::Quasi ) {
          twoSum( c.x[0], c.x[1] );
          twoSum( c.x[1], c.x[2] );
          twoSum( c.x[2], c.x[3] );
        } else {
          quickSum( c.x[0], c.x[1] );
          quickSum( c.x[1], c.x[2] );
          quickSum( c.x[2], c.x[3] );
        }
        quickSum( c.x[0], c.x[1] );
        quickSum( c.x[1], c.x[2] );
        if ( N_accuracy > 1 ) {
          quickSum( c.x[2], c.x[3] );
          quickSum( c.x[0], c.x[1] );
          quickSum( c.x[1], c.x[2] );
	}
        quickSum( c.x[0], c.x[1] );
      }
  }


#if __cplusplus < 201703L
#define	_BOOL_const_type(x)	std::integral_constant<bool,(x)>
#else
#define	_BOOL_const_type(x)	std::bool_constant<(x)>
#endif
  //
  template < typename TX >
  struct check_mX_real : std::false_type{};
  template < typename T, Algorithm A >
  struct check_mX_real< dX_real::dx_real<T,A> > : _BOOL_const_type(fp<T>::value){};
  template < typename T, Algorithm A >
  struct check_mX_real< tX_real::tx_real<T,A> > : _BOOL_const_type(fp<T>::value){};
  template < typename T, Algorithm A >
  struct check_mX_real< qX_real::qx_real<T,A> > : _BOOL_const_type(fp<T>::value){};
#ifdef __MPREAL_H__
  template <> struct check_mX_real <mpfr::mpreal> : std::false_type{};
#endif
#ifdef  _QD_DD_REAL_H
  template <> struct check_mX_real <dd_real> : std::false_type{};
#endif
#ifdef  _QD_QD_REAL_H
  template <> struct check_mX_real <qd_real> : std::false_type{};
#endif
#undef _BOOL_const_type


  //
  template < Algorithm A >
  struct if_A_noQuasi { static bool constexpr value = ( A != Algorithm::Quasi ); };
  template < Algorithm Aa, Algorithm Ab >
  struct if_A_owAble { static bool constexpr value =
      ( Aa==Ab ) ||
    ( Aa==Algorithm::Quasi ) ||
    ( Aa==Algorithm::Sloppy && Ab!=Algorithm::Quasi ); };

  // Trick to define MACROs for enabler in the template header part
#define T_assert(...)	std::enable_if_t< __VA_ARGS__, std::nullptr_t > = nullptr
#define T_scalar(...)	T_assert( std::is_arithmetic< __VA_ARGS__ >::value )
#define T_fp(...)	T_assert( fp< __VA_ARGS__ >::value )
#define T_mX(...)	T_assert( check_mX_real< __VA_ARGS__ >::value )
#define A_noQuasi(...)	T_assert( if_A_noQuasi< __VA_ARGS__ >::value )
#define A_owAble(A,...)	T_assert( if_A_owAble< A,__VA_ARGS__ >::value )

}

// helper macros to extend argument list elements passing into the raw QxW style
#define _SX_(a)	a
#define _DX_(a) a.x[0], a.x[1]
#define _TX_(a) a.x[0], a.x[1], a.x[2]
#define _QX_(a) a.x[0], a.x[1], a.x[2], a.x[3]

//
#include "dX_real.hpp"
#include "tX_real.hpp"
#include "qX_real.hpp"
//

#undef	_SX_
#undef	_DX_
#undef	_TX_
#undef	_QX_


namespace mX_real {


  //
  // APIs for common constatnt number functions
  // such as Type::Num_API()
  // C++17 makes these functions simpler by using ifconstexpr
  //
  template < typename T, T_fp(T) > static INLINE auto constexpr zero() { return T(0); }
  template < typename T, T_mX(T) > static INLINE auto constexpr zero() { return T::zero(); }

  //
  template < typename T, T_fp(T) > static INLINE auto constexpr one() { return T(1); }
  template < typename T, T_mX(T) > static INLINE auto constexpr one() { return T::one(); }

  //
  template < typename T, T_fp(T) > static INLINE auto constexpr two() { return T(2); }
  template < typename T, T_mX(T) > static INLINE auto constexpr two() { return T::two(); }

  //
  template < typename T, T_fp(T) > static INLINE auto constexpr half() { return T(1)/T(2); }
  template < typename T, T_mX(T) > static INLINE auto constexpr half() { return T::half(); }

  //
  template < typename T, T_fp(T) > static INLINE auto constexpr epsilon() { return std::numeric_limits<T>::epsilon(); }
  template < typename T, T_mX(T) > static INLINE auto constexpr epsilon() { return T::epsilon(); }

  //
  template < typename T, T_fp(T) > static INLINE auto constexpr nan() { return std::numeric_limits<T>::quiet_NaN(); }
  template < typename T, T_mX(T) > static INLINE auto constexpr nan() { return T::nan(); }

  //
  template < typename T, T_fp(T) > static INLINE auto constexpr inf() { return std::numeric_limits<T>::infinity(); }
  template < typename T, T_mX(T) > static INLINE auto constexpr inf() { return T::inf(); }


#ifdef __MPREAL_H__
#define T_mpreal(...)   T_assert( std::is_same< mpfr::mpreal, __VA_ARGS__ >::value )
  template < typename T, T_mpreal(T) > static INLINE auto const epsilon() { return mpfr::machine_epsilon(); }
  template < typename T, T_mpreal(T) > static INLINE auto const inf() { return mpfr::const_infinity(); }
  template < typename T, T_mpreal(T) > static INLINE auto const nan() { return mpfr::mpreal().setNan(); }
#undef T_mpreal
#endif
#ifdef  _QD_DD_REAL_H
#define T_DD(...)       T_assert( std::is_same< dd_real, __VA_ARGS__ >::value )
  template < typename T, T_DD(T) > static INLINE auto constexpr epsilon() { return dd_real::_eps; }
  template < typename T, T_DD(T) > static INLINE auto constexpr inf() { return dd_real::_inf; }
  template < typename T, T_DD(T) > static INLINE auto constexpr nan() { return dd_real::_nan; }
#undef T_DD
#endif
#ifdef  _QD_QD_REAL_H
#define T_QD(...)       T_assert( std::is_same< dd_real, __VA_ARGS__ >::value )
  template < typename T, T_QD(T) > static INLINE auto constexpr epsilon() { return qd_real::_eps; }
  template < typename T, T_QD(T) > static INLINE auto constexpr inf() { return qd_real::_inf; }
  template < typename T, T_QD(T) > static INLINE auto constexpr nan() { return qd_real::_nan; }
#undef T_QD
#endif

}


#include "std_mX_real.hpp"


//
using df_Real        = mX_real::dX_real::dX_real_accurate<float>;
using df_Real_sloppy = mX_real::dX_real::dX_real_sloppy<float>;
using df_Real_quasi  = mX_real::dX_real::dX_real_quasi<float>;

using dd_Real        = mX_real::dX_real::dX_real_accurate<double>;
using dd_Real_sloppy = mX_real::dX_real::dX_real_sloppy<double>;
using dd_Real_quasi  = mX_real::dX_real::dX_real_quasi<double>;

using tf_Real        = mX_real::tX_real::tX_real_accurate<float>;
using tf_Real_sloppy = mX_real::tX_real::tX_real_sloppy<float>;
using tf_Real_quasi  = mX_real::tX_real::tX_real_quasi<float>;

using td_Real        = mX_real::tX_real::tX_real_accurate<double>;
using td_Real_sloppy = mX_real::tX_real::tX_real_sloppy<double>;
using td_Real_quasi  = mX_real::tX_real::tX_real_quasi<double>;

using qf_Real        = mX_real::qX_real::qX_real_accurate<float>;
using qf_Real_sloppy = mX_real::qX_real::qX_real_sloppy<float>;
using qf_Real_quasi  = mX_real::qX_real::qX_real_quasi<float>;

using qd_Real        = mX_real::qX_real::qX_real_accurate<double>;
using qd_Real_sloppy = mX_real::qX_real::qX_real_sloppy<double>;
using qd_Real_quasi  = mX_real::qX_real::qX_real_quasi<double>;

#endif

