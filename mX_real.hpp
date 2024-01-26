#ifndef MX_REAL_H
#define MX_REAL_H

#include <assert.h>

//#include <iostream>
#include <iosfwd>
#include <sstream>
#include <bitset>
#include <limits>
#include <math.h>

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
    static INLINE auto const fabs ( float const a ) {
      return std::fabs( a );
    }
    static INLINE auto const sqrt ( float const a ) {
      return std::sqrt( a );
    }
    static INLINE auto const fma ( float const a, float const b, float const c ) {
      return std::fma( a, b, c );
    }
    static INLINE auto const rand () {
      return std::rand();
    }

    static INLINE float ulp( float const a ) {
      return QxW::fp_const<float>::ulp( a );
    }
    static INLINE float exponent( float const a ) {
      return QxW::fp_const<float>::exponent( a );
    }
    static INLINE float exponenti( float const a ) {
      return QxW::fp_const<float>::exponenti( a );
    }
  };
  //
  template <>
  struct fp<double>  {
    static bool   constexpr value = true;

    STATIC_VAR double constexpr zero    = double(0);
    STATIC_VAR double constexpr one     = double(1);
    STATIC_VAR double constexpr two     = double(2);
    STATIC_VAR double constexpr half    = one/two;

    STATIC_VAR double constexpr epsilon = std::numeric_limits<double>::epsilon();
    STATIC_VAR double constexpr epsiloni = one/std::numeric_limits<double>::epsilon();
    STATIC_VAR double constexpr nan     = std::numeric_limits<double>::quiet_NaN();
    STATIC_VAR double constexpr inf     = std::numeric_limits<double>::infinity();
    STATIC_VAR double constexpr denorm_min = std::numeric_limits<double>::denorm_min();
    STATIC_VAR double constexpr min     = std::numeric_limits<double>::min();
    STATIC_VAR double constexpr max     = std::numeric_limits<double>::max();

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

    static INLINE double ulp( double const a ) {
      return QxW::fp_const<double>::ulp( a );
    }
    static INLINE double exponent( double const a ) {
      return QxW::fp_const<double>::exponent( a );
    }
    static INLINE double exponenti( double const a ) {
      return QxW::fp_const<double>::exponenti( a );
    }
  };
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
  //
  double constexpr fp<double>::zero;
  double constexpr fp<double>::one;
  double constexpr fp<double>::two;
  double constexpr fp<double>::half;
  double constexpr fp<double>::epsilon;
  double constexpr fp<double>::inf;
  double constexpr fp<double>::nan;
  double constexpr fp<double>::denorm_min;
  double constexpr fp<double>::min;
  double constexpr fp<double>::max;
#endif
#endif


  //
  // common operation functions
  //
  template < typename T >
  INLINE void twoSum ( T const a, T const b, T & __restrict__ s, T & __restrict__ e ) {
    QxW::TwoSum( a, b, s, e );
  }
  template < typename T >
  INLINE void twoSum ( T & __restrict__ a, T & __restrict__ b ) {
    twoSum( a, b, a, b );
  }

  //
  template < typename T >
  INLINE void quickSum ( T const a, T const b, T & __restrict__ s, T & __restrict__ e ) {
    QxW::FastTwoSum( a, b, s, e );
  }
  template < typename T >
  INLINE void quickSum ( T & __restrict__ a, T & __restrict__ b ) {
    quickSum( a, b, a, b );
  }

  //
  template < typename T >
  INLINE void twoProdFMA ( T const a, T const b, T & __restrict__ s, T & __restrict__ e ) {
    QxW::TwoProductFMA( a, b, s, e );
  }


  // for cross-reference
  namespace dX_real { template < typename T, Algorithm A > struct dx_real; }
  namespace tX_real { template < typename T, Algorithm A > struct tx_real; }
  namespace qX_real { template < typename T, Algorithm A > struct qx_real; }


  //
  // Normalization policy
  // if O(|x0|)>O(|x1|)>... => quickSum in a decending (w.r.t. O(e)) order once
  // not guaranteed => TwoSum in an ascending order, then quickSum reversely once
  // N_itr :  0 => Sloppy
  //       : >1 => Quasi
  //
  template < int N_itr = 0, typename T, Algorithm A >
  INLINE void Normalize( dX_real::dx_real<T,A> & c ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    auto t = c.x[0] + c.x[1];
    if ( std::isnan( t ) || std::isinf( t ) ) {
      c.x[0] = c.x[1] = t;
    } else
#endif
      {
        if ( N_itr > 0 || A == Algorithm::Quasi ) {
          twoSum( c.x[0], c.x[1] );
        } else {
          quickSum( c.x[0], c.x[1] );
        }
      }
  }
  //
  template < int N_itr = 0, typename T, Algorithm A >
  INLINE void Normalize( tX_real::tx_real<T,A> & c ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    auto t = c.x[0] + c.x[1] + c.x[2];
    if ( std::isnan( t ) || std::isinf( t ) ) {
      c.x[0] = c.x[1] = c.x[2] = t;
    } else
#endif
      {
        if ( N_itr > 0 || A == Algorithm::Quasi ) {
          twoSum( c.x[0], c.x[1] );
          twoSum( c.x[1], c.x[2] );
        } else {
          quickSum( c.x[0], c.x[1] );
          quickSum( c.x[1], c.x[2] );
        }
          quickSum( c.x[0], c.x[1] );
      }
  }
  //
  template < int N_itr = 0, typename T, Algorithm A >
  INLINE void Normalize( qX_real::qx_real<T,A> & c ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    auto t = c.x[0] + c.x[1] + c.x[2] + c.x[3];
    if ( std::isnan( t ) || std::isinf( t ) ) {
      c.x[0] = c.x[1] = c.x[2] = c.x[3] = t;
    } else
#endif
      {
        if ( N_itr > 0 || A == Algorithm::Quasi ) {
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
  // check whether TXa ~ {m}X_real::{m}x_real<T,A> => value
  // then if(value) TX<T,A> else nullptr_t => type
  //
  template < typename TXa, typename T, Algorithm A, template < typename _T_, Algorithm _A_ > class TX >
  struct check_TX_T_A_impl {
    //
    static bool constexpr value = std::is_same< typename TXa::base_T, T >::value;
    //
    using type_to_be = TX<T,A>;
    using type = typename std::enable_if_t< value, type_to_be >;
  };
  template < typename TXa, typename T, Algorithm A, template < typename _T_, Algorithm _A_ > class TX >
  using check_TX_T_A = std::enable_if_t< check_TX_T_A_impl<TXa,T,A,TX>::value >;
  template < typename TXa, typename T, Algorithm A, template < typename _T_, Algorithm _A_ > class TX >
  using return_TX_T_A = std::enable_if_t< check_TX_T_A_impl<TXa,T,A,TX>::value,
                                       typename check_TX_T_A_impl<TXa,T,A,TX>::type >;
  template < bool F, typename TXa, typename T, Algorithm A, template < typename _T_, Algorithm _A_ > class TX >
  using cond_return_TX_T_A = std::enable_if_t< F && check_TX_T_A_impl<TXa,T,A,TX>::value,
                                            typename check_TX_T_A_impl<TXa,T,A,TX>::type >;

  //
  // check whether TXa ~ {m}X_real::{m}x_real<T,*> => value
  // then if(value) TX<T,Aa> else nullptr_t = type
  //
  template < typename TXa, typename T, template < typename _T_, Algorithm _A_ > class TX >
  struct check_TX_T_impl {
    static Algorithm constexpr A = TXa::base_A;
    //
    static bool constexpr value = check_TX_T_A_impl<TXa,T,A,TX>::value;
    //
    using type_to_be = typename check_TX_T_A_impl<TXa,T,A,TX>::type_to_be;
    using type = typename std::enable_if_t< value, type_to_be >;
  };
  template < typename TXa, typename T, template < typename _T_, Algorithm _A_ > class TX >
  using check_TX_T = typename std::enable_if_t< check_TX_T_impl<TXa,T,TX>::value >;
  template < typename TXa, typename T, template < typename _T_, Algorithm _A_ > class TX >
  using return_TX_T = std::enable_if_t< check_TX_T_impl<TXa,T,TX>::value,
                                      typename check_TX_T_impl<TXa,T,TX>::type >;
  template < bool F, typename TXa, typename T, template < typename _T_, Algorithm _A_ > class TX >
  using cond_return_TX_T = std::enable_if_t< F && check_TX_T_impl<TXa,T,TX>::value,
                                           typename check_TX_T_impl<TXa,T,TX>::type >;

  //
  // check whether TXa ~ {m}X_real::{m}x_real<T,*> ~ TXb => value
  // then if(value) TX<T,Aa> else nullptr_t => type
  //
  template < typename TXa, typename TXb, template < typename _T_, Algorithm _A_ > class TX >
  struct check_TX2_impl {
    using T = typename TXa::base_T;
    static Algorithm constexpr A = commonAlgorithm< TXa::base_A, TXb::base_A >::algorithm;
    //
    static bool constexpr value = check_TX_T_A_impl<TXa,T,A,TX>::value && check_TX_T_A_impl<TXa,T,A,TX>::value;
    //
    using type_to_be = typename check_TX_T_A_impl<TXa,T,A,TX>::type_to_be;
    using type = typename std::enable_if_t< value, type_to_be >;
  };
  template < typename TXa, typename TXb, template < typename _T_, Algorithm _A_ > class TX >
  using check_TX2 = std::enable_if_t< check_TX2_impl<TXa,TXb,TX>::value >;
  template < typename TXa, typename TXb, template < typename _T_, Algorithm _A_ > class TX >
  using return_TX2 = std::enable_if_t< check_TX2_impl<TXa,TXb,TX>::value,
                                       typename check_TX2_impl<TXa,TXb,TX>::type >;
  template < bool F, typename TXa, typename TXb, template < typename _T_, Algorithm _A_ > class TX >
  using cond_return_TX2 = std::enable_if_t< F && check_TX2_impl<TXa,TXb,TX>::value,
                                            typename check_TX2_impl<TXa,TXb,TX>::type >;



  //
  template < typename TX >
  using if_T_mX = typename std::enable_if_t<check_mX_real<TX>::value>;
  template < typename TXa, typename TXb >
  using if_T_mX2 = typename std::enable_if_t<check_mX_real<TXa>::value && check_mX_real<TXb>::value>;
  template < typename Ta, typename Tb >
  using if_T_same = typename std::enable_if_t<std::is_same<Ta,Tb>::value>;

  // Definition of shortcut structs
  template < Algorithm A >
  using if_A_noQuasi = typename std::enable_if_t< A != Algorithm::Quasi >;
  template < Algorithm Aa, Algorithm Ab >
  using if_A2_noQuasi = typename std::enable_if_t< Aa != Algorithm::Quasi && Ab != Algorithm::Quasi >;
  template < Algorithm Aa, Algorithm Ab >
  using if_A_owAble = typename std::enable_if_t<
        ( Aa==Ab ) ||
 	( Aa==Algorithm::Quasi ) ||
	( Aa==Algorithm::Sloppy && Ab!=Algorithm::Quasi ) >;

  //
  template < typename Ts >
  using if_T_fp = typename std::enable_if_t<fp<Ts>::value>;
  template < typename Tsa, typename Tsb >
  using if_T_fp2 = typename std::enable_if_t<if_T_fp<Tsa>::value && if_T_fp<Tsb>::value>;
  template < typename Ts >
  using if_T_scalar = typename std::enable_if_t<std::is_arithmetic<Ts>::value>;
  template < typename Ts, typename T >
  using if_T_float = typename std::enable_if_t<std::is_same<Ts,float>::value, T>;
  template < typename Ts, typename T >
  using if_T_double = typename std::enable_if_t<std::is_same<Ts,double>::value, T>;



}


// very short MACRO which mush be used in the if_constexpr/C++17
#define is_A_noQuasi(A)		(A!=Algorithm::Quasi)
#define is_A2_noQuasi(Aa,Ab)	(Aa!=Algorithm::Quasi && Ab!=Algorithm::Quasi)
#define is_A_owAble(Aa,Ab)	(Aa==Ab || Aa!=Algorithm::Accurate)
#define	is_T_fp(Ts)		(fp<Ts>::value)
#define	is_T_fp2(Tsa,Tsb)	(is_T_fp(Tsa) && is_T_fp(Tsb))
#define	is_T_scalar(Ts)		(std::is_arithmetic<Ts>::value)
#define	is_T_float(Ts)		(std::is_same<Ts,float>::value)
#define	is_T_double(Ts)		(std::is_same<Ts,double>::value)

// very short MACRO which mush be used in the template parameter definition
#define IF_A_noQuasi	typename __dummy_A0__=if_A_noQuasi
#define IF_A2_noQuasi	typename __dummy_A00__=if_A2_noQuasi
#define IF_A_owAble	typename __dummy_A1__=if_A_owAble
#define IF_T_fp		typename __dummy_T0__=if_T_fp
#define IF_T_fp2	typename __dummy_T00__=if_T_fp2
#define IF_T_scalar	typename __dummy_T1__=if_T_scalar
#define IF_T_mX		typename __dummy_T2__=if_T_mX
#define IF_T_mX2	typename __dummy_T22__=if_T_mX2
#define IF_T_same	typename __dummy_T3__=if_T_same

// helper macro to extend argument list elements into the raw QxW style
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

#undef IF_A_noQuasi
#undef IF_A_owAble
#undef IF_T_fp
#undef IF_T_scalar


namespace mX_real {


  //
  // APIs for common constatnt number functions
  // such as Type::Num_API()
  //
  template < typename T > static INLINE auto constexpr zero(void)
    -> std::enable_if_t<!check_mX_real<T>::value,T> { return T(0); }
  template < typename T > static INLINE auto constexpr zero(void)
    -> std::enable_if_t< check_mX_real<T>::value,T> { return T::zero(); }

  //
  template < typename T > static INLINE auto constexpr one(void)
    -> std::enable_if_t<!check_mX_real<T>::value,T> { return T(1); }
  template < typename T > static INLINE auto constexpr one(void)
    -> std::enable_if_t< check_mX_real<T>::value,T> { return T::one(); }

  //
  template < typename T > static INLINE auto constexpr two(void)
    -> std::enable_if_t<!check_mX_real<T>::value,T> { return T(2); }
  template < typename T > static INLINE auto constexpr two(void)
    -> std::enable_if_t< check_mX_real<T>::value,T> { return T::two(); }

  //
  template < typename T > static INLINE auto constexpr half(void)
    -> std::enable_if_t<!check_mX_real<T>::value,T> { return T(1)/T(2); }
  template < typename T > static INLINE auto constexpr half(void)
    -> std::enable_if_t< check_mX_real<T>::value,T> { return T::half(); }

  //
  template < typename T > static INLINE auto constexpr epsilon(void)
    -> std::enable_if_t<!check_mX_real<T>::value,T> { return std::numeric_limits<T>::epsilon(); }
  template < typename T > static INLINE auto constexpr epsilon(void)
    -> std::enable_if_t< check_mX_real<T>::value,T> { return T::epsilon(); }

  //
  template < typename T > static INLINE auto constexpr nan(void)
    -> std::enable_if_t<!check_mX_real<T>::value,T> { return std::numeric_limits<T>::quiet_NaN(); }
  template < typename T > static INLINE auto constexpr nan(void)
    -> std::enable_if_t< check_mX_real<T>::value,T> { return T::nan(); }

  //
  template < typename T > static INLINE auto constexpr inf(void)
    -> std::enable_if_t<!check_mX_real<T>::value,T> { return std::numeric_limits<T>::infinity(); }
  template < typename T > static INLINE auto constexpr inf(void)
    -> std::enable_if_t< check_mX_real<T>::value,T> { return T::inf(); }


#ifdef __MPREAL_H__
  template <> INLINE mpfr::mpreal epsilon <mpfr::mpreal> (void) { return mpfr::machine_epsilon(); }
  template <> INLINE mpfr::mpreal inf <mpfr::mpreal> (void) { return mpfr::const_infinity(); }
  template <> INLINE mpfr::mpreal nan <mpfr::mpreal> (void) { return mpfr::mpreal().setNan(); }
#endif
#ifdef  _QD_DD_REAL_H
  template <> INLINE dd_real epsilon <dd_real> (void) { return dd_real::_eps; }
  template <> INLINE dd_real inf <dd_real> (void) { return dd_real::_inf; }
  template <> INLINE dd_real nan <dd_real> (void) { return dd_real::_nan; }
#endif
#ifdef  _QD_QD_REAL_H
  template <> INLINE qd_real epsilon <qd_real> (void) { return qd_real::_eps; }
  template <> INLINE qd_real inf <qd_real> (void) { return qd_real::_inf; }
  template <> INLINE qd_real nan <qd_real> (void) { return qd_real::_nan; }
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

