#ifndef MX_REAL_H
#define MX_REAL_H

#include <assert.h>

#include <iostream>
#include <sstream>
#include <bitset>
#include <limits>
#include <math.h>


namespace mX_real {


  //
  // Descrition of the Algorithm
  //
  enum class Algorithm {
  //
    Accurate = 0,
    Sloppy   = 1,
    Quasi    = 2,
  //
  // alias names
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

  std::string toString( Algorithm A ) {
    if ( A==Algorithm::Quasi ) return "Quasi";
    if ( A==Algorithm::Sloppy ) return "Sloppy";
    if ( A==Algorithm::Accurate ) return "Accurate";
    return "Unknow";
  }

  //
  // generic terms (const, func, and bit ops)
  //
#if __cplusplus < 201703L
#define	STATIC_VAR	static
#else
#define	STATIC_VAR	static inline
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

    STATIC_VAR float constexpr epsilon = std::numeric_limits<float>::epsilon();
    STATIC_VAR float constexpr epsiloni = one/std::numeric_limits<float>::epsilon();
    STATIC_VAR float constexpr nan     = std::numeric_limits<float>::quiet_NaN();
    STATIC_VAR float constexpr inf     = std::numeric_limits<float>::infinity();

    using uint_t = uint32_t;
    static inline float ufp( float const &a ) {
      if ( a == zero ) return a;
      uint32_t e = *(uint32_t *)&a;
      uint32_t s = e & 0x80000000;
      e &= 0x7f800000;
      e -= 0x0b800000;
      if ( e & 0x80000000 ) { e = 0; } // underflow
      e = e | s; // allow -0
      return *(float *)&e;
    }

    static inline float exponent( float const &a ) {
      if ( a == zero ) return one;
      uint32_t e = *(uint32_t *)&a;
      e &= 0x7f800000;
      if ( e == 0x7f800000 ) return one;
      return *(float *)&e;
    }
    static inline float exponenti( float const &a ) {
      if ( a == zero ) return one;
      uint32_t e = *(uint32_t *)&a;
      e &= 0x7f800000;
      if ( e == 0x7f800000 ) return one;
      e = 0x7f000000 - e;
      return *(float *)&e;
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

    using uint_t = uint64_t;
    static inline double ufp( double const &a ) {
      if ( a == zero ) return a;
      uint64_t e = *(uint64_t *)&a;
      uint64_t s = e & 0x8000000000000000;
      e &= 0xfff0000000000000;
      e -= 0x0340000000000000;
      if ( e & 0x8000000000000000 ) { e = 0; } // underflow
      e = e | s; // allow -0
      return *(double *)&e;
    }

    static inline double exponent( double const &a ) {
      if ( a == zero ) return one;
      uint64_t e = *(uint64_t *)&a;
      e &= 0x7ff0000000000000;
      if ( e == 0x7ff0000000000000 ) return one;
      return *(double *)&e;
    }
    static inline double exponenti( double const &a ) {
      if ( a == zero ) return one;
      uint64_t e = *(uint64_t *)&a;
      e &= 0x7ff0000000000000;
      if ( e == 0x7ff0000000000000 ) return one;
      e = 0x7fe0000000000000 - e;
      return *(double *)&e;
    }
  };
#undef STATIC_VAR
  //
#if __cplusplus < 201703L
  float  constexpr fp<float>::zero;
  float  constexpr fp<float>::one;
  float  constexpr fp<float>::two;
  float  constexpr fp<float>::half;
  float  constexpr fp<float>::epsilon;
  float  constexpr fp<float>::inf;
  float  constexpr fp<float>::nan;
  //
  double constexpr fp<double>::zero;
  double constexpr fp<double>::one;
  double constexpr fp<double>::two;
  double constexpr fp<double>::half;
  double constexpr fp<double>::epsilon;
  double constexpr fp<double>::inf;
  double constexpr fp<double>::nan;
#endif


  //
  // common operation functions
  //
  template < typename T >
  inline void twoSum ( T const a, T const b, T & s, T & e ) {
    s = a + b;
    auto u = s - a;
    e = s - u;
    e = a - e;
    u = b - u;
    e = e + u;
  }
  template < typename T >
  inline void twoSum ( T & a, T & b ) {
    twoSum( a, b, a, b );
  }

  template < typename T >
  inline void twoSub ( T const a, T const b, T & s, T & e ) {
    s = a - b;
    auto u = s - a;
    e = s - u;
    e = a - e;
    u = b + u;
    e = e - u;
  }
  template < typename T >
  inline void twoSub ( T & a, T & b ) {
    twoSub( a, b, a, b );
  }

  template < typename T >
  inline void threeSum ( T & _a_, T & _b_, T & _c_ ) {
    auto       a = _a_; 
    auto       b = _b_; 
    auto       c = _c_; 
    twoSum( a, b, a, b );
    twoSum( c, a, a, c );
    twoSum( b, c, b, c );
    _a_ = a;
    _b_ = b;
    _c_ = c;
  }
  template < typename T >
  inline void threeSum ( T * a ) {
    threeSum( a[0], a[1], a[2] );
  }

  template < typename T >
  inline void threeSum2 ( T & _a_, T & _b_, T const& _c_ ) {
    auto       a = _a_; 
    auto       b = _b_; 
    auto       c = _c_; 
    twoSum( a, b, a, b );
    twoSum( c, a, a, c );
    b = b + c;
    _a_ = a;
    _b_ = b;
  }
  template < typename T >
  inline void threeSum2 ( T * a ) {
    threeSum2( a[0], a[1], a[2] );
  }

  template < typename T >
  inline void quickSum ( T const a, T const b, T & s, T & e ) {
    s = a + b;
    e = s - a;
    e = b - e;
  }
  template < typename T >
  inline void quickSum ( T & a, T & b ) {
    quickSum( a, b, a, b );
  }

  template < typename T >
  inline void twoProdFMA ( T const a, T const b, T & s, T & e ) {
    s = a * b;
    auto const t = -s;
    e = std::fma( a, b, t );
  }

  template < int n, int m = n, typename T >
  inline void vecMerge( T * f, T const * a, T const * b ) {
    int ia, ib, j; for(ia=ib=j=0; ia<n && ib<m; j++ ) {
      if ( std::fabs(a[ia]) >= std::fabs(b[ib]) ) {
        f[j] = a[ia++];
      } else {
        f[j] = b[ib++];
      }
    }
    if (j<n+m) {
      if ( ia < n ) for(;ia<n;ia++) { f[j++] = a[ia]; }
      if ( ib < m ) for(;ib<m;ib++) { f[j++] = b[ib]; }
    }
  }

  template < int n, typename T >
  inline void vecSum( T * f ) {
    if ( n < 0 ) { // f[0] 
      for(int i=(-n)-2; i>=0; i--) {
        twoSum( f[i], f[i+1] );
      }
    } else { // f[n-1]
      for(int i=1; i<n; i++) {
        twoSum( f[i], f[i-1] );
      }
    }
  }

  template < int n, typename T >
  inline void vecSum_Sloppy( T * f ) {
    if ( n < 0 ) { // f[0]
      for(int i=(-n)-2; i>=0; i--) {
        quickSum( f[i], f[i+1] );
      }
    } else { // f[n-1]
      for(int i=1; i<n; i++) {
        quickSum( f[i], f[i-1] );
      }
    }
  }

  template < int L, int n=L, typename T >
  inline void VSEB( T * f ) {
  //
  //  N. Fabiano, J-M Muller, and J. Picot
  //  this function is not the same as F-M-P's, it only returns N elements
  //  F-M-P suggest to be able to use twoSum or quickSum so that
  //  it acclerates performance while it can switch twoSum and quickcwSum.
  //
    static_assert( n >= L, "The v-length of f[] >= the v-length of return." );
    T e = f[0];
    int i, j; for(i=j=0; i<=n-3 && j<L; i++ ) {
      T r, t;
      twoSum( e, f[i+1], r, t );
      if ( t != fp<T>::zero ) {
        f[j++] = r;
        e = t;
      } else {
        e = r;
      }
    }
    if ( j < L ) { twoSum( e, f[i+1], f[j], f[j+1] ); j=(j+1)+1; }
    if ( j < n ) { for(int i=j; i<n; i++) { f[i] = fp<T>::zero; } }
  }
  template < int L, int n=L, typename T >
  inline void VSEB_Sloppy( T * f ) {
  //
  //  N. Fabiano, J-M Muller, and J. Picot
  //  this function is not the same as F-M-P's, it only returns N elements
  //  F-M-P suggest to be able to use twoSum or quickSum so that
  //  it acclerates performance while it can switch twoSum and quickcwSum.
  //
    static_assert( n >= L, "The v-length of f[] >= the v-length of return." );
    T e = f[0];
    int i, j; for(i=j=0; i<=n-3 && j<L; i++ ) {
      T r, t;
      quickSum( e, f[i+1], r, t );
      if ( t != fp<T>::zero ) {
        f[j++] = r;
        e = t;
      } else {
        e = r;
      }
    }
    if ( j < L ) { quickSum( e, f[i+1], f[j], f[j+1] ); j=(j+1)+1; }
    if ( j < n ) { for(int i=j; i<n; i++) { f[i] = fp<T>::zero; } }
  }


  // for cross-reference
  namespace dX_real { template < typename T, Algorithm A > struct dx_real; }
  namespace tX_real { template < typename T, Algorithm A > struct tx_real; }
  namespace qX_real { template < typename T, Algorithm A > struct qx_real; }


  //
  template < typename T, Algorithm A >
  inline auto Normalize( dX_real::dx_real<T,A> & c ) {
    quickSum( c.x[0], c.x[1] );
  }
  template < typename T, Algorithm A >
  inline auto NormalizeStrict( dX_real::dx_real<T,A> & c ) {
    twoSum( c.x[0], c.x[1] );
  }
  //
  template < typename T, Algorithm A >
  inline auto Normalize( tX_real::tx_real<T,A> & c ) {
    VSEB_Sloppy<3>( c.x );
  }
  template < typename T, Algorithm A >
  inline auto NormalizeStrict( tX_real::tx_real<T,A> & c ) {
    VSEB<3>( c.x );
  }
  //
  template < typename T, Algorithm A >
  inline auto Normalize( qX_real::qx_real<T,A> & c ) {
    VSEB_Sloppy<4>( c.x );
  }
  template < typename T, Algorithm A >
  inline auto NormalizeStrict( qX_real::qx_real<T,A> & c ) {
    VSEB<4>( c.x );
  }


}


#include "Ozaki-QW/qxw.hpp"

//
#include "dX_real.hpp"
#include "tX_real.hpp"
#include "qX_real.hpp"
//


namespace mX_real {


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
  template <>struct check_mX_real <mpfr::mpreal> { static bool constexpr value  = false; };
#endif
#ifdef  _QD_DD_REAL_H
  template <> struct check_mX_real <dd_real> { static bool constexpr value  = false; };
#endif
#ifdef  _QD_QD_REAL_H
  template <> struct check_mX_real <qd_real> { static bool constexpr value  = false; };
#endif
#undef _BOOL_const_type
  //


  //
  // APIs for common constatnt number functions
  // such as Type::Num_API()
  //
  template < typename T > static inline auto constexpr zero(void)
  -> std::enable_if_t<!check_mX_real<T>::value,T> { return T(0); }
  template < typename T > static inline auto constexpr zero(void)
  -> std::enable_if_t< check_mX_real<T>::value,T> { return T::zero(); }

  //
  template < typename T > static inline auto constexpr one(void)
  -> std::enable_if_t<!check_mX_real<T>::value,T> { return T(1); }
  template < typename T > static inline auto constexpr one(void)
  -> std::enable_if_t< check_mX_real<T>::value,T> { return T::one(); }

  //
  template < typename T > static inline auto constexpr two(void)
  -> std::enable_if_t<!check_mX_real<T>::value,T> { return T(2); }
  template < typename T > static inline auto constexpr two(void)
  -> std::enable_if_t< check_mX_real<T>::value,T> { return T::two(); }

  //
  template < typename T > static inline auto constexpr half(void)
  -> std::enable_if_t<!check_mX_real<T>::value,T> { return T(1)/T(2); }
  template < typename T > static inline auto constexpr half(void)
  -> std::enable_if_t< check_mX_real<T>::value,T> { return T::half(); }

  //
  template < typename T > static inline auto constexpr epsilon(void)
  -> std::enable_if_t<!check_mX_real<T>::value,T> { return std::numeric_limits<T>::epsilon(); }
  template < typename T > static inline auto constexpr epsilon(void)
  -> std::enable_if_t< check_mX_real<T>::value,T> { return T::epsilon(); }

  //
  template < typename T > static inline auto constexpr nan(void)
  -> std::enable_if_t<!check_mX_real<T>::value,T> { return std::numeric_limits<T>::quiet_NaN(); }
  template < typename T > static inline auto constexpr nan(void)
  -> std::enable_if_t< check_mX_real<T>::value,T> { return T::nan(); }

  //
  template < typename T > static inline auto constexpr inf(void)
  -> std::enable_if_t<!check_mX_real<T>::value,T> { return std::numeric_limits<T>::infinity(); }
  template < typename T > static inline auto constexpr inf(void)
  -> std::enable_if_t< check_mX_real<T>::value,T> { return T::inf(); }


#ifdef __MPREAL_H__
  template <> inline mpfr::mpreal epsilon <mpfr::mpreal> (void) { return mpfr::machine_epsilon(); }
  template <> inline mpfr::mpreal inf <mpfr::mpreal> (void) { return mpfr::const_infinity(); }
  template <> inline mpfr::mpreal nan <mpfr::mpreal> (void) { return mpfr::mpreal().setNan(); }
#endif
#ifdef  _QD_DD_REAL_H
  template <> inline dd_real epsilon <dd_real> (void) { return dd_real::_eps; }
  template <> inline dd_real inf <dd_real> (void) { return dd_real::_inf; }
  template <> inline dd_real nan <dd_real> (void) { return dd_real::_nan; }
#endif
#ifdef  _QD_QD_REAL_H
  template <> inline qd_real epsilon <qd_real> (void) { return qd_real::_eps; }
  template <> inline qd_real inf <qd_real> (void) { return qd_real::_inf; }
  template <> inline qd_real nan <qd_real> (void) { return qd_real::_nan; }
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

