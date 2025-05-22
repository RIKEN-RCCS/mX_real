#pragma once

#include <assert.h>
#include <cfenv>
#include <iosfwd>
#include <sstream>
#include <bitset>
#include <limits>
#include <math.h>
#include <cstdint>

#ifdef USE_MPREAL
#include <mpreal.h>
#endif

#if 0
#include <boost/type_index.hpp>
template < typename T > void printTYPE() {
  std::cout << boost::typeindex::type_id_with_cvr<T>().pretty_name(); }
#endif

#define MPFR_REAL_DATA_PUBLIC 1
#define MPFR_REAL_ENABLE_CONV_OPS 1
#include "real.hpp"
typedef mpfr::real<14, MPFR_RNDN> mpfrfp;

#if defined(__NVCC__)
#define	INLINE		__host__ __device__ __forceinline__
#define	NOEXCEPT	/**/
#else
#define	INLINE		//__always_inline
#define	NOEXCEPT	noexcept
#endif


#define	MX_REAL_USE_INF_NAN_EXCEPTION	0
#define	MX_REAL_OPTIMIZE_MUL_BY_SQR	1
#define	MX_REAL_OPTIMIZE_MUL_BY_FAST	1
#define	MX_REAL_OPTIMIZE_PROD_BY_POW2	1


//
// Trick to define MACROs for enabler in the template header part
// The macros defined following section return boolean value
//

//
// T_assert( boolean expression ): is the most common MACRO
//
#define T_assert(...)	std::enable_if_t< __VA_ARGS__, std::nullptr_t > = nullptr

//
// T_scalar( typename T ): checks whether T is included in scalar
//                         (arithmetic={float, double, int, long, ...}+fp)
// T_not_scalar( typename T ): returns not T_scalar
//
#define T_scalar(...)	T_assert( std::is_arithmetic< __VA_ARGS__ >::value || fp< __VA_ARGS__ >::value )
#define T_not_scalar(...)	T_assert( ! std::is_arithmetic< __VA_ARGS__ >::value || fp< __VA_ARGS__ >::value )

//
// T_eq_Ts( typename Ta, typename Tb ): returns whether Ta and Tb are the same
// T_neq_Ts( typename Ta, typename Tb ): return not T_eq_Ts
//
#define T_eq_Ts(...)	T_assert( std::is_same< __VA_ARGS__ >::value )
#define T_neq_Ts(...)	T_assert( ! std::is_same< __VA_ARGS__ >::value )

//
// T_fp( typename T ): returns T is fp defined in mX_real
// T_not_fp( typename T ): returns not T_fp
//
#define T_fp(...)	T_assert( fp< __VA_ARGS__ >::value )
#define T_not_fp(...)	T_assert( ! fp< __VA_ARGS__ >::value )

//
// T_mX( typename T ): returns T is the congruent type of {dtq}X_real
// T_mX( typename T ): returns not T_mX
//
#define T_mX(...)	T_assert( check_mX_real< __VA_ARGS__ >::value )
#define T_not_mX(...)	T_assert( ! check_mX_real< __VA_ARGS__ >::value )

//
// T_mX2( typename Ta, typename Tb ): returns Ta and Tb are T_mX and the both bases are the same
// T_not_mX2( typename Ta, typename Tb ): returns not T_mX2
//
#define T_mX2(...)	T_assert( check_mX_bases< __VA_ARGS__ >::value )
#define T_not_mX2(...)	T_assert( ! check_mX_bases< __VA_ARGS__ >::value )

//
// T_mX_fp( typename Ta, typename Tb ): returns whether Ta is T_mX satisfying Tb is its base
// T_not_mX_fp( typename Ta, typename Tb ): returns not T_mX_fp
//
#define T_mX_fp(...)	T_assert( check_mX_base_Tfp< __VA_ARGS__ >::value )
#define T_not_mX_fp(...)	T_assert( ! check_mX_base_Tfp< __VA_ARGS__ >::value )

//
// A_noQuasi( Algorithm A ): not Quasi { A != Quasi }
//
#define A_noQuasi(...)	T_assert( if_A_noQuasi< __VA_ARGS__ >::value )

//
// A_owAble( Algorithm Aa, Algorithm Ab ): possibility to use in the compound ops like <T,Aa>*=<T,Ab>
// it is allowed if both IN-OUT are the same
//                  (Aa == Ab)
//               if OUT does not care the Accuracy and Normalization
//                  (Aa == Quasi)
//		 if IN-OUT are Normalized but OUT is inaccurate than IN
//                  (Aa==Sloppy and Ab!=Quasi)
//
#define A_owAble(Aa,...)	T_assert( if_A_owAble< Aa,__VA_ARGS__ >::value )


  namespace mX_real {

    template < typename T >
    struct fp {
      static bool constexpr value = false;
    };
    template < typename TX >
    struct check_mX_real : std::false_type{};
    template < typename TX >
    struct base_mX_real { using type = std::nullptr_t; };

#include "Ozaki-QW/fp_const.hpp"

  }

#ifdef USE_MPREAL
#include "mX_real_mpreal.hpp"
#endif

namespace mX_real {

#include "Ozaki-QW/qxw.hpp"

  //
  // Enum-Class definition of the internal Algorithms
  //
  enum class Algorithm {
                        //
                        // smaller corresponds to an accurate algorithm
                        //
                        Accurate     =  0,
                        WeakAccurate =  5,
                        Sloppy       = 10,
                        Quasi        = 20,

                        //
                        // name aliasing
                        //
                        IEEE           = Accurate,
                        PairWise       = Quasi,
                        PairArithmetic = Quasi,
                        Default        = Accurate
  };
  std::string toString( Algorithm A ) NOEXCEPT {
    if ( A==Algorithm::Accurate ) return "Accurate";
    if ( A==Algorithm::WeakAccurate ) return "WeakAccurate";
    if ( A==Algorithm::Sloppy ) return "Sloppy";
    if ( A==Algorithm::Quasi ) return "Quasi";
    return "Unknow";
  }


  template < Algorithm Aa = Algorithm::Accurate, Algorithm Ab = Algorithm::Accurate >
  struct commonAlgorithm {
    static Algorithm constexpr algorithm =
#if 1
      Aa > Ab ? Aa : Ab;
#else
    ( Aa == Algorithm::Quasi || Ab == Algorithm::Quasi ) ?
    Algorithm::Quasi :
    ( Aa == Algorithm::Sloppy || Ab == Algorithm::Sloppy ) ?
    Algorithm::Sloppy :
    ( Aa == Algorithm::WeakAccurate || Ab == Algorithm::WeakAccurate ) ?
    Algorithm::WeakAccurate :
    Algorithm::Accurate;
#endif
  };

  template < Algorithm A >
  struct accurateAlgorithm {
    static Algorithm constexpr algorithm =
      A == Algorithm::Quasi  ? Algorithm::Sloppy :
      A == Algorithm::Sloppy ? Algorithm::WeakAccurate :
      Algorithm::Accurate;
  };

  template < Algorithm A >
  struct inaccurateAlgorithm {
    static Algorithm constexpr algorithm =
      A == Algorithm::Accurate     ? Algorithm::WeakAccurate :
      A == Algorithm::WeakAccurate ? Algorithm::Sloppy :
      Algorithm::Quasi;
  };

  template < Algorithm A >
  struct if_A_noQuasi { static bool constexpr value =
      ( A != Algorithm::Quasi ); };
  template < Algorithm Aa, Algorithm Ab >
  struct if_A_owAble { static bool constexpr value =
      //   A W S Q
      // A o x x x
      // W o o x x
      // S o o o x
      // Q o o o o
      ( Aa >= Ab ); };

  //
  // Enum-Class definition of the internal Normalization mechanisms
  //
  enum class NormalizeOption {
                              Regular      = 0,
                              Accurate     = 1,
                              VsumForward  = 2,
                              VQsumForward = 3,
                              VsumReverse  = 4,
                              VQsumReverse = 5,
                              Unknown      = -1,
                              Default      = Regular
  };
  std::string toString( NormalizeOption opt ) NOEXCEPT {
    if ( opt==NormalizeOption::Regular ) return "Regular";
    if ( opt==NormalizeOption::Accurate ) return "Accurate";
    if ( opt==NormalizeOption::VsumForward ) return "VsumForward";
    if ( opt==NormalizeOption::VQsumForward ) return "VQsumForward";
    if ( opt==NormalizeOption::VsumReverse ) return "VsumReverse";
    if ( opt==NormalizeOption::VQsumReverse ) return "VQsumReverse";
    return "Unknow";
  }


  //
  // generic terms (const, comparison funcs, and sign-bit ops)
  //
#if ( defined(__llvm__) || defined(__clang__) ) && (__cplusplus < 202302L)
#define	__constexpr__	const
#else
#define	__constexpr__	constexpr
#endif
  template <>
  struct fp<float> {
    static bool   constexpr value = true;

    using _fp_ = QxW::fp_const<float>;
    static INLINE auto constexpr zero()  NOEXCEPT { return _fp_::zero(); }
    static INLINE auto constexpr one()   NOEXCEPT { return _fp_::one(); }
    static INLINE auto constexpr two()   NOEXCEPT { return _fp_::two(); }
    static INLINE auto constexpr nhalf() NOEXCEPT { return _fp_::nhalf(); }
    static INLINE auto constexpr threehalves() NOEXCEPT { return _fp_::threehalves(); }

    static INLINE auto constexpr epsilon()    NOEXCEPT { return std::numeric_limits<float>::epsilon(); }
    static INLINE auto constexpr epsiloni()   NOEXCEPT { return one()/std::numeric_limits<float>::epsilon(); }
    static INLINE auto constexpr connect_fp() NOEXCEPT { return std::numeric_limits<float>::epsilon()*nhalf(); }
    static INLINE auto constexpr disconnect_fp() NOEXCEPT { return two()/std::numeric_limits<float>::epsilon(); }
    static INLINE auto constexpr nan()        NOEXCEPT { return std::numeric_limits<float>::quiet_NaN(); }
    static INLINE auto constexpr inf()        NOEXCEPT { return std::numeric_limits<float>::infinity(); }
    static INLINE auto constexpr denorm_min() NOEXCEPT { return std::numeric_limits<float>::denorm_min(); }
    static INLINE auto constexpr min()        NOEXCEPT { return std::numeric_limits<float>::min(); }
    static INLINE auto constexpr max()        NOEXCEPT { return std::numeric_limits<float>::max(); }

    static INLINE auto constexpr digits()       NOEXCEPT { return std::numeric_limits<float>::digits; }
    static INLINE auto constexpr digits10()     NOEXCEPT { return std::numeric_limits<float>::digits10; }
    static INLINE auto constexpr max_digits10() NOEXCEPT { return std::numeric_limits<float>::max_digits10; }

    static INLINE auto __constexpr__ isinf       ( float const& a ) NOEXCEPT { return std::isinf( a ); }
    static INLINE auto __constexpr__ isnan       ( float const& a ) NOEXCEPT { return std::isnan( a ); }
    static INLINE auto __constexpr__ copysign    ( float const& a, float const& b ) NOEXCEPT { return std::copysign( a, b ); }
    static INLINE auto __constexpr__ signbit     ( float const& a ) NOEXCEPT { return std::signbit( a ); }
    static INLINE auto __constexpr__ is_zero     ( float const& a ) NOEXCEPT { return (a == fp<float>::zero()); }
    static INLINE auto __constexpr__ is_positive ( float const& a ) NOEXCEPT { return (a >  fp<float>::zero()); }
    static INLINE auto __constexpr__ is_negative ( float const& a ) NOEXCEPT { return (a <  fp<float>::zero()); }
  };
  //
  template <>
  struct fp<double> {
    static bool   constexpr value = true;

    using _fp_ = QxW::fp_const<double>;
    static INLINE auto constexpr zero()  NOEXCEPT { return _fp_::zero(); }
    static INLINE auto constexpr one()   NOEXCEPT { return _fp_::one(); }
    static INLINE auto constexpr two()   NOEXCEPT { return _fp_::two(); }
    static INLINE auto constexpr nhalf() NOEXCEPT { return _fp_::nhalf(); }
    static INLINE auto constexpr threehalves() NOEXCEPT { return _fp_::threehalves(); }

    static INLINE auto constexpr epsilon()    NOEXCEPT { return std::numeric_limits<double>::epsilon(); }
    static INLINE auto constexpr epsiloni()   NOEXCEPT { return one()/std::numeric_limits<double>::epsilon(); }
    static INLINE auto constexpr connect_fp() NOEXCEPT { return std::numeric_limits<double>::epsilon()*nhalf(); }
    static INLINE auto constexpr disconnect_fp() NOEXCEPT { return two()/std::numeric_limits<double>::epsilon(); }
    static INLINE auto constexpr nan()        NOEXCEPT { return std::numeric_limits<double>::quiet_NaN(); }
    static INLINE auto constexpr inf()        NOEXCEPT { return std::numeric_limits<double>::infinity(); }
    static INLINE auto constexpr denorm_min() NOEXCEPT { return std::numeric_limits<double>::denorm_min(); }
    static INLINE auto constexpr min()        NOEXCEPT { return std::numeric_limits<double>::min(); }
    static INLINE auto constexpr max()        NOEXCEPT { return std::numeric_limits<double>::max(); }

    static INLINE auto constexpr digits()       NOEXCEPT { return std::numeric_limits<double>::digits; }
    static INLINE auto constexpr digits10()     NOEXCEPT { return std::numeric_limits<double>::digits10; }
    static INLINE auto constexpr max_digits10() NOEXCEPT { return std::numeric_limits<double>::max_digits10; }

    static INLINE auto __constexpr__ isinf       ( double const& a ) NOEXCEPT { return std::isinf( a ); }
    static INLINE auto __constexpr__ isnan       ( double const& a ) NOEXCEPT { return std::isnan( a ); }
    static INLINE auto __constexpr__ copysign    ( double const& a, double const& b ) NOEXCEPT { return std::copysign( a, b ); }
    static INLINE auto __constexpr__ signbit     ( double const& a ) NOEXCEPT { return std::signbit( a ); }
    static INLINE auto __constexpr__ is_zero     ( double const& a ) NOEXCEPT { return (a == fp<double>::zero()); }
    static INLINE auto __constexpr__ is_positive ( double const& a ) NOEXCEPT { return (a >  fp<double>::zero()); }
    static INLINE auto __constexpr__ is_negative ( double const& a ) NOEXCEPT { return (a <  fp<double>::zero()); }
  };

  template<>
  struct fp<mpfrfp> {
      //      explicit operator double() const { mpfrfp x = *this; double xx = mpfr_get_d(x._x, MPFR_RNDN); return xx;}	
      //      explicit operator float() const { mpfrfp x = *this; float xx = mpfr_get_flt(x._x, MPFR_RNDN); return xx;}

    static bool   constexpr value = true;

    static mpfrfp const zero() { return mpfrfp(0);}  //
    static mpfrfp const one() { return mpfrfp(1);}    //
    static mpfrfp const two() { return mpfrfp(2);}    //
    static mpfrfp const nhalf() { return mpfrfp(0.5);}  //
    static mpfrfp const threehalves() { return mpfrfp(1.5);}  //
    static mpfrfp const nan() { mpfrfp p; mpfr_set_nan(p._x); return p;}
    static mpfrfp const inf() { mpfrfp p; mpfr_set_inf(p._x, 1); return p;}    
    
    static INLINE bool isinf       ( mpfrfp  const a ) {
      return static_cast<bool>(mpfr_inf_p(a._x));
      //    return std::isinf( a );
    }
    static INLINE bool isnan       ( mpfrfp  const a ) {
      return static_cast<bool>(mpfr_nan_p(a._x));      
      //      return std::isnan( a );
    }
    static INLINE bool is_zero ( mpfrfp  const a ) {
      return static_cast<bool>(mpfr_zero_p(a._x));      
    }

    static INLINE mpfrfp copysign    ( mpfrfp  const a, mpfrfp  const b ) {
      mpfrfp c;
      mpfr_copysign(c._x, a._x, b._x, MPFR_RNDN);
      return c;
      //return std::copysign( a, b );
    }
    static INLINE bool signbit     ( mpfrfp  const a ) {
      return static_cast<bool>(mpfr_signbit(a._x));
      //      return std::signbit( a );
    }
    static INLINE bool s_zero     ( mpfrfp  const a ) {
      return a == mpfrfp(0); //fp<mpfrfp>::zero;
    }
    static INLINE bool is_positive ( mpfrfp  const a ) {
      return a >  mpfrfp(0); //fp<mpfrfp>::zero;
    }
    static INLINE bool is_negative ( mpfrfp  const a ) {
      return a <  mpfrfp(0); // fp<mpfrfp>::zero;
    }
    static INLINE mpfrfp fabs ( mpfrfp const a ) {
      mpfrfp b;
      mpfr_abs(b._x, a._x, MPFR_RNDN);
      return b;
    }
    static INLINE mpfrfp sqrt ( mpfrfp const a ) {
      mpfrfp b;
      mpfr_sqrt(b._x, a._x, MPFR_RNDN);
      return b;
    }
    static INLINE mpfrfp fma ( mpfrfp const a, mpfrfp const b, mpfrfp const c ) {
      mpfrfp d;
//      fprintf(stderr, "%s %d : fp_const<mpfrfp>::fma\n", __FILE__, __LINE__);
      mpfr_fma(d._x, a._x, b._x, c._x, MPFR_RNDN);
      return d;
    }
    static INLINE auto const rand () {
      mpfrfp a;
      gmp_randstate_t state;
      gmp_randinit_default(state);
      mpfr_urandom(a._x,state, MPFR_RNDN);
      return a;
    }
    static INLINE mpfrfp exponent( mpfrfp const a ) {
      if ( a == mpfrfp(0) ) return mpfrfp(1); // zero ) return one;
      mpfr_exp_t ix; // mpfr_exp_t = short/int/long
      ix = mpfr_get_exp(a._x);
      double y = pow(2.0, (double)ix);
      mpfrfp b;
      mpfr_init_set_d(b._x, y, MPFR_RNDN);
      return b;
      //      
      //      uint64_t e = *(uint64_t *)&a;
      //      e &= 0x7ff0000000000000;
      //      if ( e == 0x7ff0000000000000 ) return one;
      //      return *(mpfrfp *)&e;
    }
    static INLINE mpfrfp exponenti( mpfrfp const a ) {
      if ( a == mpfrfp(0) ) return mpfrfp(1); // if ( a == zero ) return one;
      mpfr_exp_t ix;  // mpfr_exp_t = short/int/long
      ix = mpfr_get_exp(a._x);
      double y = pow(0.5, (double)ix);
      mpfrfp b;
      mpfr_init_set_d(b._x, y, MPFR_RNDN);
      return b;
      //      uint64_t e = *(uint64_t *)&a;
      //      e &= 0x7ff0000000000000;
      //      if ( e == 0x7ff0000000000000 ) return one;
      //      e = 0x7fe0000000000000 - e;
      //      return *(mpfrfp *)&e;
    }

    static INLINE mpfrfp get_nan()
    {
      mpfrfp x;
      mpfr_set_nan(x._x);
      return x;
    }

    static INLINE mpfrfp get_inf()
    {
      mpfrfp x;
      mpfr_set_inf(x._x, 1);
      return x;
    }

    static INLINE mpfrfp get_epsilon()
    {
      mpfrfp x(0);
      return x;
    }

    static INLINE mpfrfp get_epsiloni()
    {
      mpfrfp x(0);
      return x;
    }

    static INLINE mpfrfp get_denorm_min()
    {
      mpfrfp x(0);
      return x;
    }
    static INLINE mpfrfp get_min()
    {
      mpfrfp x(0);
      return x;
    }
    static INLINE mpfrfp get_max()
    {
      mpfrfp x(0);
      return x;
    }
    };
  //
#undef	__constexpr__
  //
  //


  //
  // common operation functions
  //
  template < typename T >
  INLINE auto constexpr twoSum ( T const a, T const b, T & __restrict__ s, T & __restrict__ e ) NOEXCEPT {
    //
    //   Notice:: if a or b is nearly max(), result would be NaN
    //            if a or b is nearly min(), result would be underflow
    //
    QxW::TwoSum( a, b, s, e );
  }
  template < typename T >
  INLINE auto constexpr twoSum ( T & __restrict__ a, T & __restrict__ b ) NOEXCEPT {
    //
    //   Notice:: if a or b is nearly max(), result would be NaN
    //            if a or b is nearly min(), result would be underflow
    //
    twoSum( a, b, a, b );
  }

  //
  template < typename T >
  INLINE auto constexpr quickSum ( T const a, T const b, T & __restrict__ s, T & __restrict__ e ) NOEXCEPT {
    //
    //   Notice:: if a or b is nearly max(), result would be NaN
    //            if a or b is nearly min(), result would be underflow
    //
    QxW::FastTwoSum( a, b, s, e );
  }
  template < typename T >
  INLINE auto constexpr quickSum ( T & __restrict__ a, T & __restrict__ b ) NOEXCEPT {
    //
    //   Notice:: if a or b is nearly max(), result would be NaN
    //            if a or b is nearly min(), result would be underflow
    //
    quickSum( a, b, a, b );
  }

  //
  template < typename T >
  INLINE auto constexpr twoProdFMA ( T const a, T const b, T & __restrict__ s, T & __restrict__ e ) NOEXCEPT {
    //
    //   Notice:: if a*b is nearly max(), result would be INF overflow
    //
    QxW::TwoProductFMA( a, b, s, e );
  }

  //
  template < typename T >
  INLINE auto constexpr copy_with_rounding ( T * __restrict__ dest, T const * __restrict__ src, int const L, int const LL ) NOEXCEPT {
    if ( L <= LL ) {
      for(auto i=0; i<L; i++) { dest[i] = src[i]; }
      if ( L < LL ) { dest[L-1] += src[L]; }
    } else {
      for(auto i=0; i<LL; i++) { dest[i] = src[i]; }
      for(auto i=LL; i<L; i++) { dest[i] = fp<T>::zero(); }
    }
  }


  // for cross-reference
  namespace sX_real { template < typename T, Algorithm A > struct sx_real; }  
  namespace dX_real { template < typename T, Algorithm A > struct dx_real; }
  namespace tX_real { template < typename T, Algorithm A > struct tx_real; }
  namespace qX_real { template < typename T, Algorithm A > struct qx_real; }
  namespace pX_real { template < typename T, Algorithm A > struct px_real; }  

#if __cplusplus < 201703L
#define	_BOOL_const_type(...)	std::integral_constant<bool, __VA_ARGS__ >
#else
#define	_BOOL_const_type(...)	std::bool_constant< __VA_ARGS__ >
#endif
  //
  template < typename T, Algorithm A >
  struct check_mX_real< sX_real::sx_real<T,A> > : _BOOL_const_type(fp<T>::value){};  
  template < typename T, Algorithm A >
  struct check_mX_real< dX_real::dx_real<T,A> > : _BOOL_const_type(fp<T>::value){};
  template < typename T, Algorithm A >
  struct check_mX_real< tX_real::tx_real<T,A> > : _BOOL_const_type(fp<T>::value){};
  template < typename T, Algorithm A >
  struct check_mX_real< qX_real::qx_real<T,A> > : _BOOL_const_type(fp<T>::value){};
  template < typename T, Algorithm A >
  struct check_mX_real< pX_real::px_real<T,A> > : _BOOL_const_type(fp<T>::value){};  
  //
  template < typename T, Algorithm A >
  struct base_mX_real< sX_real::sx_real<T,A> > { using type = T; };
  template < typename T, Algorithm A >
  struct base_mX_real< dX_real::dx_real<T,A> > { using type = T; };
  template < typename T, Algorithm A >
  struct base_mX_real< tX_real::tx_real<T,A> > { using type = T; };
  template < typename T, Algorithm A >
  struct base_mX_real< qX_real::qx_real<T,A> > { using type = T; };
  template < typename T, Algorithm A >
  struct base_mX_real< pX_real::px_real<T,A> > { using type = T; };
  //
  template < typename TX, typename Ts >
  struct check_mX_base_Tfp : _BOOL_const_type(
                                              check_mX_real<TX>::value &&
                                              fp<Ts>::value &&
                                              std::is_same< typename base_mX_real<TX>::type, Ts >::value ){}; 
  template < typename TXa, typename TXb >
  struct check_mX_bases : _BOOL_const_type(
                                           check_mX_real<TXa>::value &&
                                           check_mX_real<TXb>::value &&
                                           ! std::is_same< typename base_mX_real<TXa>::type,
                                           std::nullptr_t >::value &&
                                           std::is_same< typename base_mX_real<TXa>::type,
                                           typename base_mX_real<TXb>::type >::value ){};
#undef _BOOL_const_type


  //
  // Normalization policy
  // if O(|x0|)>O(|x1|)>... => quickSum in a decending (w.r.t. O(e)) order once
  // not guaranteed => TwoSum in an ascending order, then quickSum up-and-down
  // order once or more like the bubble sort
  //
  template < typename TX, T_mX(TX) >
  INLINE auto constexpr quick_Normalized ( TX const& a ) NOEXCEPT {
    using T = typename TX::base_T;
    //
    // if a is Normalized s = a.x[0]
    // if a is not Normalized and INF s = sum a.x = INF
    // if a is not Normalized and NAN s = sum a.x = NAN
    //
    T s = a.x[0];
    if ( TX::base_A == Algorithm::Quasi ) {
      for(auto i=1; i<TX::L; i++) { s += a.x[i]; }
    }
    return s;
  }
  template < NormalizeOption Nopt = NormalizeOption::Regular, typename T, Algorithm A >
  INLINE auto constexpr Normalize ( sX_real::sx_real<T,A> & c ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    auto t = quick_Normalized( c );
    if ( fp<T>::isnan( t ) || fp<T>::isinf( t ) || fp<T>::is_zero( t ) ) {
      c.x[0] = t;
    } else
#endif
      { }
  }
  
  template < NormalizeOption Nopt = NormalizeOption::Regular, typename T, Algorithm A >
  INLINE auto constexpr Normalize ( dX_real::dx_real<T,A> & c ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    auto t = quick_Normalized( c );
    if ( fp<T>::isnan( t ) || fp<T>::isinf( t ) || fp<T>::is_zero( t ) ) {
      c.x[0] = t;
    } else
#endif
      {
	switch ( Nopt ) {
	case NormalizeOption::VsumForward: {
          twoSum( c.x[0], c.x[1] );
	} break;
	case NormalizeOption::VQsumForward: {
          quickSum( c.x[0], c.x[1] );
	} break;
	case NormalizeOption::VsumReverse: {
          twoSum( c.x[0], c.x[1] );
	} break;
	case NormalizeOption::VQsumReverse: {
          quickSum( c.x[0], c.x[1] );
	} break;
	case NormalizeOption::Accurate: {
          twoSum( c.x[0], c.x[1] );
          quickSum( c.x[0], c.x[1] );
	} break;
	case NormalizeOption::Regular: {
          if ( A == Algorithm::Quasi ) {
            twoSum( c.x[0], c.x[1] );
          }
          quickSum( c.x[0], c.x[1] );
	} break;
	case NormalizeOption::Unknown: { } break;
	default: { } break;
	}
      }
  }
  //
  template < NormalizeOption Nopt = NormalizeOption::Regular, typename T, Algorithm A >
  INLINE auto constexpr Normalize ( tX_real::tx_real<T,A> & c ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    auto t = quick_Normalized( c );
    if ( fp<T>::isnan( t ) || fp<T>::isinf( t ) || fp<T>::is_zero( t ) ) {
      c.x[0] = c.x[1] = c.x[2] = t;
    } else
#endif
      {
	switch ( Nopt ) {
	case NormalizeOption::Accurate: {
          twoSum( c.x[0], c.x[2] );
          twoSum( c.x[0], c.x[1] );
          twoSum( c.x[1], c.x[2] );
          quickSum( c.x[1], c.x[2] );
          quickSum( c.x[0], c.x[1] );
          quickSum( c.x[1], c.x[2] );
	} break;
	case NormalizeOption::VsumForward: {
          twoSum( c.x[0], c.x[1] );
          twoSum( c.x[1], c.x[2] );
	} break;
	case NormalizeOption::VQsumForward: {
          quickSum( c.x[0], c.x[1] );
          quickSum( c.x[1], c.x[2] );
	} break;
	case NormalizeOption::VsumReverse: {
          twoSum( c.x[1], c.x[2] );
          twoSum( c.x[0], c.x[1] );
	} break;
	case NormalizeOption::VQsumReverse: {
          quickSum( c.x[1], c.x[2] );
          quickSum( c.x[0], c.x[1] );
	} break;
	case NormalizeOption::Regular: {
          if ( A == Algorithm::Quasi ) {
            twoSum( c.x[0], c.x[2] );
            twoSum( c.x[0], c.x[1] );
            twoSum( c.x[1], c.x[2] );
          }
          quickSum( c.x[1], c.x[2] );
          quickSum( c.x[0], c.x[1] );
          quickSum( c.x[1], c.x[2] );
	} break;
	case NormalizeOption::Unknown: { } break;
	default: { } break;
        }
      }
  }
  //
  template < NormalizeOption Nopt = NormalizeOption::Regular, typename T, Algorithm A >
  INLINE auto constexpr Normalize ( qX_real::qx_real<T,A> & c ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    auto t = quick_Normalized( c );
    if ( fp<T>::isnan( t ) || fp<T>::isinf( t ) || fp<T>::is_zero( t ) ) {
      c.x[0] = c.x[1] = c.x[2] = c.x[3] = t;
    } else
#endif
      {
	switch ( Nopt ) {
	case NormalizeOption::VsumForward: {
          twoSum( c.x[0], c.x[1] );
          twoSum( c.x[1], c.x[2] );
          twoSum( c.x[2], c.x[3] );
	} break;
	case NormalizeOption::VQsumForward: {
          quickSum( c.x[0], c.x[1] );
          quickSum( c.x[1], c.x[2] );
          quickSum( c.x[2], c.x[3] );
	} break;
	case NormalizeOption::VsumReverse: {
          twoSum( c.x[2], c.x[3] );
          twoSum( c.x[1], c.x[2] );
          twoSum( c.x[0], c.x[1] );
	} break;
	case NormalizeOption::VQsumReverse: {
          quickSum( c.x[2], c.x[3] );
          quickSum( c.x[1], c.x[2] );
          quickSum( c.x[0], c.x[1] );
	} break;
	case NormalizeOption::Accurate: {
          twoSum( c.x[0], c.x[3] );
          twoSum( c.x[0], c.x[2] );
          twoSum( c.x[0], c.x[1] );
          twoSum( c.x[1], c.x[3] );
          twoSum( c.x[1], c.x[2] );
          twoSum( c.x[2], c.x[3] );
          quickSum( c.x[2], c.x[3] );
          quickSum( c.x[1], c.x[2] );
          quickSum( c.x[0], c.x[1] );
          quickSum( c.x[2], c.x[3] );
          quickSum( c.x[1], c.x[2] );
          quickSum( c.x[2], c.x[3] );
	} break;
	case NormalizeOption::Regular: {
          if ( A == Algorithm::Quasi ) {
            twoSum( c.x[0], c.x[3] );
            twoSum( c.x[0], c.x[2] );
            twoSum( c.x[0], c.x[1] );
            twoSum( c.x[1], c.x[3] );
            twoSum( c.x[1], c.x[2] );
            twoSum( c.x[2], c.x[3] );
          }
          quickSum( c.x[2], c.x[3] );
          quickSum( c.x[1], c.x[2] );
          quickSum( c.x[0], c.x[1] );
          quickSum( c.x[2], c.x[3] );
          quickSum( c.x[1], c.x[2] );
          quickSum( c.x[2], c.x[3] );
	} break;
	case NormalizeOption::Unknown: { } break;
	default: { } break;
        }
      }
  }
  //
  template < NormalizeOption Nopt = NormalizeOption::Regular, typename T, Algorithm A >
  INLINE auto constexpr Normalize ( pX_real::px_real<T,A> & c ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    auto t = quick_Normalized( c );
    if ( fp<T>::isnan( t ) || fp<T>::isinf( t ) || fp<T>::is_zero( t ) ) {
      c.x[0] = c.x[1] = c.x[2] = c.x[3] = c.x[4] = t;
    } else
#endif
      {
	switch ( Nopt ) {
	case NormalizeOption::VsumForward: {
          twoSum( c.x[0], c.x[1] );
          twoSum( c.x[1], c.x[2] );
          twoSum( c.x[2], c.x[3] );
          twoSum( c.x[3], c.x[4] );	  
	} break;
	case NormalizeOption::VQsumForward: {
          quickSum( c.x[0], c.x[1] );
          quickSum( c.x[1], c.x[2] );
          quickSum( c.x[2], c.x[3] );
          quickSum( c.x[3], c.x[4] );	  
	} break;
	case NormalizeOption::VsumReverse: {
          twoSum( c.x[3], c.x[4] );	  
          twoSum( c.x[2], c.x[3] );
          twoSum( c.x[1], c.x[2] );
          twoSum( c.x[0], c.x[1] );
	} break;
	case NormalizeOption::VQsumReverse: {
          quickSum( c.x[3], c.x[4] );	  
          quickSum( c.x[2], c.x[3] );
          quickSum( c.x[1], c.x[2] );
          quickSum( c.x[0], c.x[1] );
	} break;
	case NormalizeOption::Accurate: {
          twoSum( c.x[0], c.x[4] );	  
          twoSum( c.x[0], c.x[3] );
          twoSum( c.x[0], c.x[2] );
          twoSum( c.x[0], c.x[1] );
          twoSum( c.x[1], c.x[4] );	  
          twoSum( c.x[1], c.x[3] );
          twoSum( c.x[1], c.x[2] );
          twoSum( c.x[3], c.x[4] );	  
          twoSum( c.x[2], c.x[3] );
          twoSum( c.x[3], c.x[4] );
          quickSum( c.x[3], c.x[4] );	  
          quickSum( c.x[2], c.x[3] );
          quickSum( c.x[1], c.x[2] );
          quickSum( c.x[0], c.x[1] );
          quickSum( c.x[3], c.x[4] );	  
          quickSum( c.x[2], c.x[3] );
          quickSum( c.x[1], c.x[2] );
          quickSum( c.x[3], c.x[4] );
          quickSum( c.x[2], c.x[3] );
          quickSum( c.x[3], c.x[4] );	  
	} break;
	case NormalizeOption::Regular: {
          if ( A == Algorithm::Quasi ) {
            twoSum( c.x[0], c.x[4] );	    
            twoSum( c.x[0], c.x[3] );
            twoSum( c.x[0], c.x[2] );
            twoSum( c.x[0], c.x[1] );
            twoSum( c.x[1], c.x[4] );	    
            twoSum( c.x[1], c.x[3] );
            twoSum( c.x[1], c.x[2] );
            twoSum( c.x[2], c.x[4] );	    
            twoSum( c.x[2], c.x[3] );
            twoSum( c.x[3], c.x[4] );	    
          }
          quickSum( c.x[3], c.x[4] );	  
          quickSum( c.x[2], c.x[3] );
          quickSum( c.x[1], c.x[2] );
          quickSum( c.x[0], c.x[1] );
          quickSum( c.x[3], c.x[4] );	  
          quickSum( c.x[2], c.x[3] );
          quickSum( c.x[1], c.x[2] );
          quickSum( c.x[3], c.x[4] );	  	  
          quickSum( c.x[2], c.x[3] );
          quickSum( c.x[3], c.x[4] );	  
	} break;
	case NormalizeOption::Unknown: { } break;
	default: { } break;
        }
      }
  }

}

// helper macros to extend argument list elements passing into the raw QxW style
#define _SX_(a)	a.x[0]
#define _DX_(a) a.x[0], a.x[1]
#define _TX_(a) a.x[0], a.x[1], a.x[2]
#define _QX_(a) a.x[0], a.x[1], a.x[2], a.x[3]
#define _PX_(a) a.x[0], a.x[1], a.x[2], a.x[3], a.x[4]

namespace mX_real {
  template<typename T, typename std::enable_if<
			 std::is_same<T, mpfrfp>::value, 
			 std::nullptr_t>::type = nullptr > 
  INLINE void clear_underflow()
  {
      mpfr_clear_underflow();
  }
  template<typename T, typename std::enable_if<
			 std::is_same<T, float>::value ||
			 std::is_same<T, double>::value,
			 std::nullptr_t>::type = nullptr > 
  INLINE void clear_underflow()
  {
    std::feclearexcept(FE_UNDERFLOW);
  }

  template<typename T,	typename std::enable_if<
			  std::is_same<T, mpfrfp>::value, 
			  std::nullptr_t>::type = nullptr > 
  INLINE void underflow_p(char *st)
  {
    if (mpfr_underflow_p()) {
      fprintf(stderr, "%s underflow\n", st);
    }      
  }
  template<typename T, typename std::enable_if<
			 std::is_same<T, float>::value ||
			 std::is_same<T, double>::value,
			 std::nullptr_t>::type = nullptr > 
  INLINE void underflow_p(char *st)
  {
    if ( (bool)std::fetestexcept(FE_UNDERFLOW)) {
      fprintf(stderr, "%s underflow\n", st);
    }      
  }

  template<typename T>
  int16_t getexp(T &a)
  {
    return a.iexp;
  }

  template<typename T>  
  void setexp(T &a, int16_t ia)
  {
    a.iexp = ia;
  }
#if 0
  template <typename T, Algorithm A, typename std::enable_if<
					std::is_same<T, float>::value,
					std::nullptr_t>::type = nullptr>
  void trunclast(pX_real::px_real<T, A> &x)
  {
    // truncate L=4 th (last 5th) by 7 bits
  }
#endif					
  template<typename T, typename std::enable_if<
			std::is_same<typename T::base_T, mpfrfp>::value,
			 std::nullptr_t>::type = nullptr > 
  INLINE void trunclast(T &x)
  {
#if 0
    mpfr::real<10, MPFR_RNDN> tmp10;
    mpfrfp tmp14;
    const int L1 = T::L - 1;
    mpfr_set(tmp10._x, (x.x[L1])._x, MPFR_RNDN);
    mpfr_set(tmp14._x, tmp10._x, MPFR_RNDN);
    x.x[L1] = tmp14;
#endif
  }
  template<typename T, typename std::enable_if<
			!std::is_same<typename T::base_T, mpfrfp>::value,
			 std::nullptr_t>::type = nullptr >
  INLINE void trunclast(T &a)
  { } 					

  template<typename T, T_fp(T)>
  INLINE void trunclast(T &a)
  {  }
#if 0  
  template <typename T, Algorithm A, typename std::enable_if<
					! std::is_same<T, mpfrfp>::value,
					std::nullptr_t>::type = nullptr >
  void trunclast(pX_real::px_real<T, A> &x)
  {}

  template <typename T, Algorithm A, typename std::enable_if<
					std::is_same<T, mpfrfp>::value,
					std::nullptr_t>::type = nullptr
  > void trunclast(pX_real::px_real<T, A> &x)
  {}
  #endif
  template<typename T, typename std::enable_if<
			 (std::is_same<typename T::base_T, float>::value ||
			  std::is_same<typename T::base_T, double>::value), 
			 std::nullptr_t>::type = nullptr > 
  INLINE void scaling(T &a,
		      const int16_t ia = static_cast<int16_t>(0))
  {
    using Ts = typename T::base_T;
    int iexp0, iexp1, iexp2 ;
    {
      Ts y = frexp(a.x[0], &iexp0);    // frexp for both float and double
      a.iexp = static_cast<int16_t>(iexp0) + ia; // ia is used as an offset
      Ts a0 = a.x[0];                         // needs to be replaced by 
      for (int i = 1; i < T::L; i++) {       // quickNormalize
	a0 += a.x[i];
      }
      iexp2 = iexp1 = a.iexp;
//    iexp1 *= static_cast<int>(a0 * 9007199254740992.0); // 2^53      
//    iexp1 *= static_cast<int>(a0 * 16777216.0f);// 2^24 idea from Nitadori-san
//    a.iexp = (iexp1 == 0) ? 0 : iexp2;     // a[] == 0 => a.iexp = 0
      a.iexp = (a0 == fp<Ts>::zero()) ? 0 : iexp2;
      a.x[0] = y;
      for (int i = 1; i < T::L; i++) {
	int iexpi;
	Ts yi = frexp(a.x[i], &iexpi);        // frexp for both float and double
	a.x[i] = ldexp(yi, (iexpi - iexp0)); // ldexp for both float and double
      }
    }
  }

  template<typename T, typename std::enable_if<
			 std::is_same<typename T::base_T, mpfrfp>::value, 
			 std::nullptr_t>::type = nullptr >
  INLINE void scaling(T &a, 
		      const int16_t ia = static_cast<int16_t>(0))
  {
    int iexp0, iexp1, iexp2;
    {
      iexp0 = mpfr_get_exp(a.x[0]._x);
      a.iexp = static_cast<int16_t>(iexp0) + ia; // ia is used as an offset
      mpfrfp a0 = a.x[0];
      for (int i = 1; i < T::L; i++) {
	a0 += a.x[i];  // floating point addition
      }
      iexp2 = iexp1 = a.iexp;
//    iexp1 *= static_cast<int>(a0 * 9007199254740992.0); // 2^53            
//    iexp1 *= static_cast<int>(mpfr_get_d(a0._x, MPFR_RNDN) * 16384.0); // 2^14
//    a.iexp = (iexp1 == 0) ? 0 : iexp2;     // a[] == 0 => a.iexp = 0  
      a.iexp = (a0 == fp<mpfrfp>::zero() ? 0 : iexp2);
      //      fprintf(stderr, "%s %d %d %d\n", __FILE__, __LINE__, static_cast<int>(ia), static_cast<int>(a.iexp));
      mpfr_set_exp(a.x[0]._x, 0);
      for (int i = 1; i < T::L; i++) {
	int iexpi = mpfr_get_exp(a.x[i]._x) - iexp0;
	mpfr_set_exp(a.x[i]._x, iexpi);
      }
    }
  }

  template<typename T, typename std::enable_if<
			 (!std::is_same<typename T::base_T, float>::value &&
			  !std::is_same<typename T::base_T, double>::value &&
			  !std::is_same<typename T::base_T, mpfrfp>::value),
			 std::nullptr_t>::type = nullptr > 
  INLINE void sclaing(T &a, const int16_t ia  = static_cast<int16_t>(0))
  {  }

  template<typename T, T_fp(T)>
  INLINE void sclaing(T &a, const int16_t ia = static_cast<int16_t>(0))
  {  }
					
  template<typename T,  typename std::enable_if<
			  (std::is_same<typename T::base_T, double>::value ||
			   std::is_same<typename T::base_T, float>::value ),
			   std::nullptr_t>::type = nullptr > 
INLINE void rescaling(T &a, const int16_t ia)
  {
    for (int i = 0; i < T::L; i++) {
      int iexpi;
      double yi = frexp(a.x[i], &iexpi);  // frexp for both float/double
      iexpi += static_cast<int>(ia);
      a.x[i] = ldexp(yi, iexpi);          // ldexp for both float/double
    }
  }

  template<typename T, 
           typename std::enable_if<std::is_same<typename T::base_T, mpfrfp>::value, std::nullptr_t>::type = nullptr > INLINE void rescaling(T &a, const int16_t ia)
  {
    for (int i = 0; i < T::L; i++) {
      int iexpi = mpfr_get_exp(a.x[i]._x) + static_cast<int>(ia);
      mpfr_set_exp(a.x[i]._x, iexpi);
    }
  }

  template<typename T, typename std::enable_if<
			 (!std::is_same<typename T::base_T, float>::value &&
			  !std::is_same<typename T::base_T, double>::value &&
			  !std::is_same<typename T::base_T, mpfrfp>::value),
			 std::nullptr_t>::type = nullptr > 
INLINE void resclaing(T &a, const int16_t ia)
  {  }
}
//
#include "sX_real.hpp"
#include "dX_real.hpp"
#include "tX_real.hpp"
#include "qX_real.hpp"
#include "pX_real.hpp"
//

#undef	_SX_
#undef	_DX_
#undef	_TX_
#undef	_QX_
#undef	_PX_
#undef	_HX_


namespace mX_real {

  template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb), T_assert( std::is_same< typename TXa::base_T, typename TXb::base_T >::value ) >
  struct largeType_impl {
    using type = typename std::conditional< ( TXa::L > TXb::L ), TXa, TXb >::type;
  };
  template < typename TXa, typename TXb >
  using largeType = typename largeType_impl<TXa,TXb>::type;

  template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb), T_assert( std::is_same< typename TXa::base_T, typename TXb::base_T >::value ) >
  struct smallType_impl {
    using type = typename std::conditional< ( TXa::L < TXb::L ), TXa, TXb >::type;
  };
  template < typename TXa, typename TXb >
  using smallType = typename smallType_impl<TXa,TXb>::type;

  template < typename TXa, typename TXb >
  using commonType = largeType<TXa,TXb>;

  //
  //
  //
  template < typename Ta, Algorithm Aa, int La, T_fp(Ta), T_assert( (2 - 1 <= La) && (La <= 4 + 1) ) >
  struct mx_real_impl {
    using type = typename 
      std::conditional_t< (La <= 2), dX_real::dx_real<Ta,Aa>,
      std::conditional_t< (La == 3), tX_real::tx_real<Ta,Aa>,
                          std::conditional_t< (La >= 4), qX_real::qx_real<Ta,Aa>,
                          std::nullptr_t >>>;
  };
  template < typename Ta, Algorithm Aa, int La >
  using mx_real = typename mx_real_impl<Ta,Aa,La>::type;
  template < typename TXa, T_mX(TXa) >
  using narrowerType = mx_real<typename TXa::base_T,TXa::base_A,TXa::L-1>;
  template < typename TXa, T_mX(TXa) >
  using widerType = mx_real<typename TXa::base_T,TXa::base_A,TXa::L+1>;


  //
  // APIs for common constatnt number functions
  // such as ConstNum_func_name<Type>()
  // C++17 makes these functions simpler by using ifconstexpr
  //
  template < typename T, T_fp(T) > static INLINE auto constexpr zero() NOEXCEPT { return fp<T>::zero(); }
  template < typename T, T_mX(T) > static INLINE auto constexpr zero() NOEXCEPT { return T::zero(); }
  //
  template < typename T, T_fp(T) > static INLINE auto constexpr one() NOEXCEPT { return fp<T>::one(); }
  template < typename T, T_mX(T) > static INLINE auto constexpr one() NOEXCEPT { return T::one(); }
  //
  template < typename T, T_fp(T) > static INLINE auto constexpr two() NOEXCEPT { return fp<T>::two(); }
  template < typename T, T_mX(T) > static INLINE auto constexpr two() NOEXCEPT { return T::two(); }
  //
  template < typename T, T_fp(T) > static INLINE auto constexpr nhalf() NOEXCEPT { return fp<T>::nhalf(); }
  template < typename T, T_mX(T) > static INLINE auto constexpr nhalf() NOEXCEPT { return T::nhalf(); }
  //
  template < typename T, T_fp(T) > static INLINE auto constexpr epsilon() NOEXCEPT { return fp<T>::epsilon(); }
  template < typename T, T_mX(T) > static INLINE auto constexpr epsilon() NOEXCEPT { return T::epsilon(); }
  //
  template < typename T, T_fp(T) > static INLINE auto constexpr nan() NOEXCEPT { return fp<T>::nan(); }
  template < typename T, T_mX(T) > static INLINE auto constexpr nan() NOEXCEPT { return T::nan(); }
  //
  template < typename T, T_fp(T) > static INLINE auto constexpr inf() NOEXCEPT { return fp<T>::inf(); }
  template < typename T, T_mX(T) > static INLINE auto constexpr inf() NOEXCEPT { return T::inf(); }

}


#include "std_mX_real.hpp"


//
using df_Real        = mX_real::dX_real::dX_real_accurate<float>;
using df_Real_sloppy = mX_real::dX_real::dX_real_sloppy<float>;
using df_Real_quasi  = mX_real::dX_real::dX_real_quasi<float>;

using dd_Real        = mX_real::dX_real::dX_real_accurate<double>;
using dd_Real_sloppy = mX_real::dX_real::dX_real_sloppy<double>;
using dd_Real_quasi  = mX_real::dX_real::dX_real_quasi<double>;

using dmpfr_Real        = mX_real::dX_real::dX_real_accurate<mpfrfp>;
using dmpfr_Real_sloppy = mX_real::dX_real::dX_real_sloppy<mpfrfp>;
using dmpfr_Real_quasi  = mX_real::dX_real::dX_real_quasi<mpfrfp>;


using tf_Real        = mX_real::tX_real::tX_real_accurate<float>;
using tf_Real_sloppy = mX_real::tX_real::tX_real_sloppy<float>;
using tf_Real_quasi  = mX_real::tX_real::tX_real_quasi<float>;

using td_Real        = mX_real::tX_real::tX_real_accurate<double>;
using td_Real_sloppy = mX_real::tX_real::tX_real_sloppy<double>;
using td_Real_quasi  = mX_real::tX_real::tX_real_quasi<double>;

using tmpfr_Real        = mX_real::tX_real::tX_real_accurate<mpfrfp>;
using tmpfr_Real_sloppy = mX_real::tX_real::tX_real_sloppy<mpfrfp>;
using tmpfr_Real_quasi  = mX_real::tX_real::tX_real_quasi<mpfrfp>;

using qf_Real        = mX_real::qX_real::qX_real_accurate<float>;
using qf_Real_sloppy = mX_real::qX_real::qX_real_sloppy<float>;
using qf_Real_quasi  = mX_real::qX_real::qX_real_quasi<float>;

using qd_Real        = mX_real::qX_real::qX_real_accurate<double>;
using qd_Real_sloppy = mX_real::qX_real::qX_real_sloppy<double>;
using qd_Real_quasi  = mX_real::qX_real::qX_real_quasi<double>;

using qmpfr_Real        = mX_real::qX_real::qX_real_accurate<mpfrfp>;
using qmpfr_Real_sloppy = mX_real::qX_real::qX_real_sloppy<mpfrfp>;
using qmpfr_Real_quasi  = mX_real::qX_real::qX_real_quasi<mpfrfp>;

using pf_Real        = mX_real::pX_real::pX_real_accurate<float>;
using pf_Real_sloppy = mX_real::pX_real::pX_real_sloppy<float>;
using pf_Real_quasi  = mX_real::pX_real::pX_real_quasi<float>;

using pd_Real        = mX_real::pX_real::pX_real_accurate<double>;
using pd_Real_sloppy = mX_real::pX_real::pX_real_sloppy<double>;
using pd_Real_quasi  = mX_real::pX_real::pX_real_quasi<double>;

using pmpfr_Real        = mX_real::pX_real::pX_real_accurate<mpfrfp>;
using pmpfr_Real_sloppy = mX_real::pX_real::pX_real_sloppy<mpfrfp>;
using pmpfr_Real_quasi  = mX_real::pX_real::pX_real_quasi<mpfrfp>;

