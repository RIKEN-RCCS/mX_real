#ifndef	DX_REAL_H
#define	DX_REAL_H

#include <x86intrin.h>

#include "mX_real.hpp"


namespace mX_real {
namespace dX_real {

  template < typename T, Algorithm A=Algorithm::Accurate >
  struct dx_real {

    static_assert( fp<T>::value,
                   "The base type must be out of the defined types, such as float or double." );

    //
    //
    using base_T = T;
    static Algorithm constexpr base_A = A;
    //
    // {DX,TX,QX}_REAL are shortcut type only within dx_real
    //
  private:
    template < Algorithm _A_=A >
    using DX_REAL = dX_real::dx_real<T,_A_>;
    template < Algorithm _A_=A >
    using TX_REAL = tX_real::tx_real<T,_A_>;
    template < Algorithm _A_=A >
    using QX_REAL = qX_real::qx_real<T,_A_>;
    //
    //
  public:
    static int constexpr L = 2;
    T x[L];

    //
    // A special member function utilized in constructors
    //
    static INLINE T constexpr quick_Normalized ( DX_REAL<> const& a ) {
      T s = a.x[0];
      if ( A == Algorithm::Quasi ) { s += a.x[1]; }
      return s;
    }

    //
    // Simple constructors
    //
    INLINE dx_real() {
      x[0] = x[1] = fp<T>::zero;
    }
    INLINE dx_real( T const& x0, T const& x1 ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = x0 + x1;
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        x[0] = x[1] = t;
      } else
#endif
      { x[0] = x0; x[1] = x1; }
    }
    INLINE dx_real( int const &h ) {
      { x[0] = T(h); x[1] = fp<T>::zero; }
    }
    INLINE dx_real( T const *d ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = d[0] + d[1];
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        x[0] = x[1] = t;
      } else
#endif
      { x[0] = d[0]; x[1] = d[1]; }
    }

    //
    // Copy constructor with (or without) any Algorithm switch
    // auto zero_accurate = df_Real( 0 ); // constructor
    // auto zero_sloppy = df_Real_sloppy( zero_accurate ); // copy constructor
    //
    INLINE dx_real( T const& h ) { // constructor
      x[0] = h; x[1] = fp<T>::zero;
    }
    template < Algorithm _A_ >
    INLINE dx_real( DX_REAL<_A_> const& h ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = h.quick_Normalized();
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        x[0] = x[1] = t;
      } else
#endif
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1];
      } else {
        auto s = h;
	mX_real::Normalize<1>( s );
        x[0] = s.x[0]; x[1] = s.x[1];
      }
    }
    template < Algorithm _A_ >
    INLINE dx_real( TX_REAL<_A_> const& h ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = h.quick_Normalized();
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        x[0] = x[1] = t;
      } else
#endif
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1];
      } else {
        auto s = h;
	mX_real::Normalize<1>( s );
        x[0] = s.x[0]; x[1] = s.x[1];
      }
    }
    template < Algorithm _A_ >
    INLINE dx_real( QX_REAL<_A_> const& h ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = h.quick_Normalized();
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        x[0] = x[1] = t;
      } else
#endif
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1];
      } else {
        auto s = h;
	mX_real::Normalize<1>( s );
        x[0] = s.x[0]; x[1] = s.x[1];
      }
    }

    //
    // Copy-assignment operator
    // auto one = df_Real( 1 ); // a constructor
    // one = df_Real_sloppy( one ); // LHS is a copy-assignment operator
    //
    INLINE DX_REAL<> &operator=( int const& h )& {
      x[0] = T(h); x[1] = fp<T>::zero;
      return *this;
    }
    INLINE DX_REAL<> &operator=( T const& h )& {
      x[0] = h; x[1] = fp<T>::zero;
      return *this;
    }
    template < Algorithm _A_ >
    INLINE DX_REAL<> &operator=( DX_REAL<_A_> const& h )& {
      if ( A == _A_ && this == (DX_REAL<>*)(&h) ) { return *this; }
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = h.quick_Normalized();
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        x[0] = x[1] = t;
      } else
#endif
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1];
      } else {
        auto s = h;
	mX_real::Normalize<1>( s );
        x[0] = s.x[0]; x[1] = s.x[1];
      }
      return *this;
    }
    template < Algorithm _A_ >
    INLINE DX_REAL<> &operator=( TX_REAL<_A_> const& h )& {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = h.quick_Normalized();
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        x[0] = x[1] = t;
      } else
#endif
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1];
      } else {
        auto s = h;
	mX_real::Normalize<1>( s );
        x[0] = s.x[0]; x[1] = s.x[1];
      }
      return *this;
    }
    template < Algorithm _A_ >
    INLINE DX_REAL<> &operator=( QX_REAL<_A_> const& h )& {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = h.quick_Normalized();
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        x[0] = x[1] = t;
      } else
#endif
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1];
      } else {
        auto s = h;
	mX_real::Normalize<1>( s );
        x[0] = s.x[0]; x[1] = s.x[1];
      }
      return *this;
    }

    //
    // Casting to other datatypes like
    // auto one = df_Real( 1 ); // constructor with an argument
    // auto one_ = (sf_Real_quasi)one; // RHS is datacast while LHS is a copy constructor
    //
    explicit
    INLINE operator T() const noexcept {
      if ( A == Algorithm::Quasi ) {
        return this->quick_Normalized();
      } else {
        return x[0];
      }
    }
    template < Algorithm _A_ >
    explicit
    INLINE operator DX_REAL<_A_>() const noexcept {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = this->quick_Normalized();
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        DX_REAL<_A_> c;
        c.x[0] = c.x[1] = t;
        return c;
      } else
#endif
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        auto s = *this;
	mX_real::Normalize<1>( s );
        return DX_REAL<_A_>{ s.x };
      } else {
        return DX_REAL<_A_>{ x };
      }
    }
    template < Algorithm _A_ >
    explicit
    INLINE operator TX_REAL<_A_>() const noexcept {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = this->quick_Normalized();
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        DX_REAL<_A_> c;
        c.x[0] = c.x[1] = t;
        return c;
      } else
#endif
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        auto s = *this;
	mX_real::Normalize<1>( s );
        return TX_REAL<_A_>{ s.x[0], s.x[1], fp<T>::zero };
      } else {
        T c = std::isinf(x[0]) ? x[0] : fp<T>::zero;
        return TX_REAL<_A_>{ x[0], x[1], c };
      }
    }
    template < Algorithm _A_ >
    explicit
    INLINE operator QX_REAL<_A_>() const noexcept {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = this->quick_Normalized();
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        DX_REAL<_A_> c;
        c.x[0] = c.x[1] = t;
        return c;
      } else
#endif
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        auto s = *this;
	mX_real::Normalize<1>( s );
        return QX_REAL<_A_>( s.x[0], s.x[1], fp<T>::zero, fp<T>::zero );
      } else {
        T c = std::isinf(x[0]) ? x[0] : fp<T>::zero;
        return QX_REAL<_A_>{ x[0], x[1], c, c };
      }
    }


    //
    // unneccessary operators are invalidaded
    //
    auto const operator!  () = delete;
    auto const operator~  () = delete;
    auto const operator++ () = delete;
    auto const operator-- () = delete;
    template < typename _T_ > auto const operator&&  ( _T_ a ) = delete;
    template < typename _T_ > auto const operator||  ( _T_ a ) = delete;
    template < typename _T_ > auto const operator&=  ( _T_ a ) = delete;
    template < typename _T_ > auto const operator^=  ( _T_ a ) = delete;
    template < typename _T_ > auto const operator<<= ( _T_ a ) = delete;
    template < typename _T_ > auto const operator>>= ( _T_ a ) = delete;
    template < typename _T_ > auto const operator,   ( _T_ a ) = delete;
    auto const operator() (...) = delete;
    template < typename _T_ > auto const operator[]  ( _T_ a ) = delete;

    //
    template < typename _T_ > auto const operator/=  ( _T_ a ) = delete;


    //
    // member constexpr functions or constatns
    // contents are always in Accurate format
    // but they follow in the Algorithm argument 'A'
    //
    static INLINE DX_REAL<> constexpr zero () { return DX_REAL<>{ fp<T>::zero }; }
    static INLINE DX_REAL<> constexpr one  () { return DX_REAL<>{ fp<T>::one  }; }
    static INLINE DX_REAL<> constexpr two  () { return DX_REAL<>{ fp<T>::two  }; }
    static INLINE DX_REAL<> constexpr half () { return DX_REAL<>{ fp<T>::half }; }

    static INLINE DX_REAL<> constexpr epsilon () {
      T c = fp<T>::epsilon * fp<T>::half; c = (c * c) * 2; return DX_REAL<>{ c }; }

    static INLINE DX_REAL<> constexpr nan  () { T c = fp<T>::nan; return DX_REAL<>{ c,c }; }
    static INLINE DX_REAL<> constexpr inf  () { T c = fp<T>::inf; return DX_REAL<>{ c,c }; }

    static INLINE DX_REAL<> constexpr denorm_min  () {
      T c0 = fp<T>::denorm_min;
      return DX_REAL<>{ c0 };
    }
    static INLINE DX_REAL<> constexpr min  () {
      T c0 = (fp<T>::min * 2) * fp<T>::epsiloni;
      return DX_REAL<>{ c0 };
    }
    static INLINE DX_REAL<> constexpr max  () {
      T c0 = fp<T>::max;
      T c1 = c0 * fp<T>::epsilon * fp<T>::half;
      return DX_REAL<>{ c0, c1 };
    }


    static INLINE DX_REAL<> constexpr rand ();
    //
    static INLINE DX_REAL<> constexpr reversed_sign ( DX_REAL<> const& a );
    static INLINE bool      constexpr signbit       ( DX_REAL<> const& a );
    static INLINE bool      constexpr isinf         ( DX_REAL<> const& a );
    static INLINE bool      constexpr isnan         ( DX_REAL<> const& a );
    static INLINE bool      constexpr is_zero       ( DX_REAL<> const& a );
    static INLINE bool      constexpr is_positive   ( DX_REAL<> const& a );
    static INLINE bool      constexpr is_negative   ( DX_REAL<> const& a );
    static INLINE DX_REAL<> constexpr sqrt          ( DX_REAL<> const& a );
    static INLINE DX_REAL<> constexpr abs           ( DX_REAL<> const& a );


    // operations to THIS object
    INLINE void constexpr reverse_sign () { x[0] = -x[0]; x[1] = -x[1]; }
    INLINE void constexpr zerofy ()       { x[0] = x[1] = fp<T>::zero; }
    template < int N_itr = 0 >
    INLINE void constexpr Normalize ()    { mX_real::Normalize<N_itr>( *this ); }
    //
    INLINE DX_REAL<> reversed_sign () const { return reversed_sign( *this ); }
    INLINE bool      signbit ()       const { return signbit( *this ); }
    INLINE bool      isinf ()         const { return isinf( *this ); }
    INLINE bool      isnan ()         const { return isnan( *this ); }
    INLINE bool      is_zero ()       const { return is_zero( *this ); }
    INLINE bool      is_positive ()   const { return is_positive( *this ); }
    INLINE bool      is_negative ()   const { return is_negative( *this ); }
    INLINE DX_REAL<> sqrt ()          const { return sqrt( *this ); }
    INLINE DX_REAL<> abs ()           const { return abs( *this ); }
    INLINE T         quick_Normalized () const { return quick_Normalized( *this ); }
    //
    INLINE DX_REAL<> element_rotate () const {
      T y[L]; y[0] = x[0]; y[1] = x[1];
      for(int i=0; i<L-1; i++) {
        if ( ! fp<T>::is_zero( y[0] ) ) { return DX_REAL<>( y ); }
        T t = y[0]; y[0] = y[1]; y[1] = t;
      }
      return DX_REAL<>( y );
    }

  };


  //
  // Alias names
  //
  template < typename T >
  using dX_real_accurate = dX_real::dx_real<T,Algorithm::Accurate>;
  template < typename T >
  using dX_real_sloppy   = dX_real::dx_real<T,Algorithm::Sloppy>;
  template < typename T >
  using dX_real_quasi    = dX_real::dx_real<T,Algorithm::Quasi>;


  //
  // Bit-ops and Comparisons
  //
  template < typename T, Algorithm Aa >
  INLINE auto const isinf ( dX_real::dx_real<T,Aa> const& a ) {
    return fp<T>::isinf( a.quick_Normalized() );
  }
  template < typename T, Algorithm Aa >
  INLINE auto const isnan ( dX_real::dx_real<T,Aa> const& a ) {
    return fp<T>::isnan( a.quick_Normalized() );
  }
  template < typename T, Algorithm Aa >
  INLINE auto const signbit ( dX_real::dx_real<T,Aa> const& a ) {
    return fp<T>::signbit( a.quick_Normalized() );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr is_zero ( dX_real::dx_real<T,Aa> const& a ) {
    return a.quick_Normalized() == fp<T>::zero;
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr is_positive ( dX_real::dx_real<T,Aa> const& a ) {
    return a.quick_Normalized() > fp<T>::zero;
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr is_negative ( dX_real::dx_real<T,Aa> const& a ) {
    return a.quick_Normalized() < fp<T>::zero;
  }
  //
  template < typename T, Algorithm Aa >
  INLINE bool constexpr dX_real::dx_real<T,Aa>::isinf ( dX_real::dx_real<T,Aa> const& a ) {
    return dX_real::isinf<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr dX_real::dx_real<T,Aa>::isnan ( dX_real::dx_real<T,Aa> const& a ) {
    return dX_real::isnan<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr dX_real::dx_real<T,Aa>::signbit ( dX_real::dx_real<T,Aa> const& a ) {
    return dX_real::signbit<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr dX_real::dx_real<T,Aa>::is_zero ( dX_real::dx_real<T,Aa> const& a ) {
    return dX_real::is_zero<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr dX_real::dx_real<T,Aa>::is_positive ( dX_real::dx_real<T,Aa> const& a ) {
    return dX_real::is_positive<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr dX_real::dx_real<T,Aa>::is_negative ( dX_real::dx_real<T,Aa> const& a ) {
    return dX_real::is_negative<T,Aa>( a );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_eq ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return a.x[0] == b.x[0] && a.x[1] == b.x[1];
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator== ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    if ( A == Algorithm::Quasi ) {
      using TT = dX_real::dx_real<T,Algorithm::Accurate>;
      return dX_real::operator_eq( TT{ a }, TT{ b } );
    } else {
      return dX_real::operator_eq( a, b );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator!= ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return !(a == b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator_lt ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    if ( a.x[0] != b.x[0] ) {
      return a.x[0] < b.x[0];
    } else {
      return a.x[1] < b.x[1];
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator< ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    if ( A == Algorithm::Quasi ) {
      using TT = dX_real::dx_real<T,Algorithm::Accurate>;
      return dX_real::operator_lt( TT{ a }, TT{ b } );
    } else {
      return dX_real::operator_lt( a, b );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator_gt ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    if ( a.x[0] != b.x[0] ) {
      return a.x[0] > b.x[0];
    } else {
      return a.x[1] > b.x[1];
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator> ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    if ( A == Algorithm::Quasi ) {
      using TT = dX_real::dx_real<T,Algorithm::Accurate>;
      return dX_real::operator_gt( TT{ a }, TT{ b } );
    } else {
      return dX_real::operator_gt( a, b );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator>= ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return !(a < b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator<= ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return !(a > b);
  }


  //
  // Unitary operators
  //
  template < typename T, Algorithm Aa >
  INLINE auto const reversed_sign ( dX_real::dx_real<T,Aa> const& a ) {
    return dX_real::dx_real<T,Aa>( -a.x[0], -a.x[1] );
  }
  template < typename T, Algorithm Aa >
  INLINE dX_real::dx_real<T,Aa> constexpr dX_real::dx_real<T,Aa>::reversed_sign ( dX_real::dx_real<T,Aa> const& a ) {
    return dX_real::reversed_sign( a );
  }
  //
  template < typename T, Algorithm Aa >
  INLINE auto const operator+ ( dX_real::dx_real<T,Aa> const& a ) {
    return a;
  }
  template < typename T, Algorithm Aa >
  INLINE auto const operator- ( dX_real::dx_real<T,Aa> const& a ) {
    return dX_real::reversed_sign( a );
  }


  //
  // Addition
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_add_body ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = dX_real::dx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::add_DW_DW_DW( a.x[0], a.x[1], b.x[0], b.x[1], c.x[0], c.x[1] );
    } else {
      QxW::add_PA_PA_PA( a.x[0], a.x[1], b.x[0], b.x[1], c.x[0], c.x[1] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Ab >
  INLINE auto const operator_add_body ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = dX_real::dx_real<T,Ab>;
    TX c;
    if ( Ab == Algorithm::Accurate ) {
      QxW::add_SW_DW_DW( a, b.x[0], b.x[1], c.x[0], c.x[1] );
    } else {
      QxW::add_SW_PA_PA( a, b.x[0], b.x[1], c.x[0], c.x[1] );
      if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm A=Algorithm::Accurate, IF_T_fp<T> >
  INLINE auto const operator_add_body ( T const& a, T const& b ) {
    using TX = dX_real::dx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::add_SW_SW_DW( a, b, c.x[0], c.x[1] );
    } else {
      QxW::add_SW_SW_PA( a, b, c.x[0], c.x[1] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  //
#if MX_REAL_USE_INF_NAN_EXCEPTION
  template < Algorithm A, typename T >
  INLINE auto const operator_add_exception ( T const& a, T const& b ) {
    using TX = dX_real::dx_real<T,A>;
    if ( fp<T>::isnan( a ) || fp<T>::isnan( b ) ) {
        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b ); throw TX{ c,c };
    }
    if ( fp<T>::isinf( a ) && fp<T>::isinf( b ) ) {
        auto sign = std::signbit( a ) ^ std::signbit( b );
	if ( sign ) {
          auto c = fp<T>::nan; throw TX{ c,c };
        } else {
          auto c = fp<T>::copysign( fp<T>::inf, a ); throw TX{ c,c };
	}
    }
    if ( fp<T>::isinf( a ) ) {
        auto c = fp<T>::copysign( inf, a ); throw TX{ c,c };
    }
    if ( fp<T>::isinf( b ) ) {
        auto c = fp<T>::copysign( inf, b ); throw TX{ c,c };
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_add_exception ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    auto sa = a.quick_Normalized();
    auto sb = b.quick_Normalized();
    dX_real::operator_add_exception <A>( sa, sb );
  }
  template < typename T, Algorithm Ab >
  INLINE auto const operator_add_exception ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
    auto sb = a.quick_Normalized();
    dX_real::operator_add_exception <Ab>( a, sb );
  }
#endif
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_add ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    using TX = dX_real::dx_real<T,A>;
    try { dX_real::operator_add_exception( a, b ); }
    catch ( TX const& e ) { return e; }
#endif
    return dX_real::operator_add_body( a, b );
  }
  template < typename T, Algorithm Ab >
  INLINE auto const operator_add ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    using TX = dX_real::dx_real<T,Ab>;
    try { dX_real::operator_add_exception( a, b ); }
    catch ( TX const& e ) { return e; }
#endif
    return dX_real::operator_add_body( a, b );
  }
  template < Algorithm A=Algorithm::Accurate, typename T, IF_T_fp<T> >
  INLINE auto const operator_add ( T const& a, T const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    using TX = dX_real::dx_real<T,A>;
    try { dX_real::operator_add_exception<A>( a, b ); }
    catch ( TX const& e ) { return e; }
#endif
    return dX_real::operator_add_body( a, b );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator+ ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return dX_real::operator_add( a, b );
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  INLINE auto const operator+ ( Ts const& a, dX_real::dx_real<T,Ab> const& b ) {
    return dX_real::operator_add( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto const operator+ ( dX_real::dx_real<T,Aa> const& a, Ts const& b ) {
    return b + a;
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const& operator_add_ow ( dX_real::dx_real<T,Aa>& a, dX_real::dx_real<T,Ab> const& b ) {
    if ( Aa == Algorithm::Accurate ) {
      QxW::add_DW_DW_DW( a.x[0], a.x[1], b.x[0], b.x[1], a.x[0], a.x[1] );
    } else {
      QxW::add_PA_PA_PA( a.x[0], a.x[1], b.x[0], b.x[1], a.x[0], a.x[1] );
      if ( Aa != Algorithm::Quasi ) Normalize( a );
    }
    return a;
  }
  template < typename T, Algorithm Aa >
  INLINE auto const& operator_add_ow ( dX_real::dx_real<T,Aa>& a, T const& b ) {
    if ( Aa == Algorithm::Accurate ) {
      QxW::add_DW_SW_DW( a.x[0], a.x[1], b, a.x[0], a.x[1] );
    } else {
      QxW::add_PA_SW_PA( a.x[0], a.x[1], b, a.x[0], a.x[1] );
      if ( Aa != Algorithm::Quasi ) Normalize( a );
    }
    return a;
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab, IF_A_owAble<Aa,Ab> >
    // ::Accurate += (::Sloppy or ::Quasi) : not allowed
    // if neccessary, write as a += dx_real<T,Algorithm::Accurate>(b), explicitly.
  INLINE auto const& operator+= ( dX_real::dx_real<T,Aa>& a, dX_real::dx_real<T,Ab> const& b ) {
    return dX_real::operator_add_ow ( a, b );;
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto const& operator+= ( dX_real::dx_real<T,Aa>& a, Ts const& b ) {
    return dX_real::operator_add_ow ( a, T(b) );;
  }


  //
  // Substruction
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator- ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  INLINE auto const operator- ( Ts const& a, dX_real::dx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename Ts, typename T, Algorithm Aa , IF_T_scalar<Ts> >
  INLINE auto const operator- ( dX_real::dx_real<T,Aa> const& a, Ts const& b ) {
    return a + (-b);
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const& operator-= ( dX_real::dx_real<T,Aa>& a, dX_real::dx_real<T,Ab> const& b ) {
    return a += (-b);
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto const& operator-= ( dX_real::dx_real<T,Aa>& a, Ts const& b ) {
    return a += (-b);
  }
   

  //
  // Multiplication
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_mul_body ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = dX_real::dx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::mul_DW_DW_DW( a.x[0], a.x[1], b.x[0], b.x[1], c.x[0], c.x[1] );
    } else {
      QxW::mul_PA_PA_PA( a.x[0], a.x[1], b.x[0], b.x[1], c.x[0], c.x[1] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Ab >
  INLINE auto const operator_mul_body ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = dX_real::dx_real<T,Ab>;
    TX c;
    if ( Ab == Algorithm::Accurate ) {
      QxW::mul_SW_DW_DW( a, b.x[0], b.x[1], c.x[0], c.x[1] );
    } else {
      QxW::mul_SW_PA_PA( a, b.x[0], b.x[1], c.x[0], c.x[1] );
      if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm A=Algorithm::Accurate, IF_T_fp<T> >
  INLINE auto const operator_mul_body ( T const& a, T const& b ) {
    using TX = dX_real::dx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::mul_SW_SW_DW( a, b, c.x[0], c.x[1] );
    } else {
      QxW::mul_SW_SW_PA( a, b, c.x[0], c.x[1] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  //
#if MX_REAL_USE_INF_NAN_EXCEPTION
  template < Algorithm A, typename T >
  INLINE auto const operator_mul_exception ( T const& a, T const& b ) {
    using TX = dX_real::dx_real<T,A>;
    if ( fp<T>::isnan( a ) || fp<T>::isnan( b ) ) {
        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b ); throw TX{ c,c };
    }
    if ( fp<T>::isinf( a ) && fp<T>::is_zero( b ) ) {
        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b ); throw TX{ c,c };
    }
    if ( fp<T>::isinf( b ) && fp<T>::is_zero( a ) ) {
        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b ); throw TX{ c,c };
    }
    if ( fp<T>::isinf( a ) || fp<T>::isinf( b ) ) {
        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::inf, a ), b ); throw TX{ c,c };
    }
    if ( fp<T>::is_zero( a ) || fp<T>::is_zero( b ) ) {
        auto c = fp<T>::copysign( inf, b ); throw TX{ c,c };
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_mul_exception ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    auto sa = a.quick_Normalized();
    auto sb = b.quick_Normalized();
    dX_real::operator_mul_exception <A>( sa, sb );
  }
  template < typename T, Algorithm Ab >
  INLINE auto const operator_mul_exception ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
    auto sb = a.quick_Normalized();
    dX_real::operator_mul_exception <Ab>( a, sb );
  }
#endif
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_mul ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    using TX = dX_real::dx_real<T,A>;
    try { dX_real::operator_mul_exception<A>( a, b ); }
    catch ( TX const& e ) { return e; }
#endif
    return dX_real::operator_mul_body( a, b );
  }
  template < typename T, Algorithm Ab >
  INLINE auto const operator_mul ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    using TX = dX_real::dx_real<T,Ab>;
    try { dX_real::operator_mul_exception<Ab>( a, b ); }
    catch ( TX const& e ) { return e; }
#endif
    return dX_real::operator_mul_body( a, b );
  }
  template < typename T, Algorithm A=Algorithm::Accurate, IF_T_fp<T> >
  INLINE auto const operator_mul ( T const& a, T const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    using TX = dX_real::dx_real<T,A>;
    try { dX_real::operator_mul_exception<A>( a, b ); }
    catch ( TX const& e ) { return e; }
#endif
    return dX_real::operator_mul_body( a, b );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator* ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return dX_real::operator_mul( a, b );
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  INLINE auto const operator* ( Ts const& a, dX_real::dx_real<T,Ab> const& b ) {
    return dX_real::operator_mul( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto const operator* ( dX_real::dx_real<T,Aa> const& a, Ts const& b ) {
    return b * a;
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const& operator_mul_ow ( dX_real::dx_real<T,Aa>& a, dX_real::dx_real<T,Ab> const& b ) {
    if ( Aa == Algorithm::Accurate ) {
      QxW::mul_DW_DW_DW( a.x[0], a.x[1], b.x[0], b.x[1], a.x[0], a.x[1] );
    } else {
      QxW::mul_PA_PA_PA( a.x[0], a.x[1], b.x[0], b.x[1], a.x[0], a.x[1] );
      if ( Aa != Algorithm::Quasi ) Normalize( a );
    }
    return a;
  }
  template < typename T, Algorithm Aa >
  INLINE auto const& operator_mul_ow ( dX_real::dx_real<T,Aa>& a, T const& b ) {
    if ( Aa == Algorithm::Accurate ) {
      QxW::mul_DW_SW_DW( a.x[0], a.x[1], b, a.x[0], a.x[1] );
    } else {
      QxW::mul_PA_SW_PA( a.x[0], a.x[1], b, a.x[0], a.x[1] );
      if ( Aa != Algorithm::Quasi ) Normalize( a );
    }
    return a;
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab, IF_A_owAble<Aa,Ab> >
    // ::Accurate *= (::Sloppy or ::Quasi) : not allowed
    // if neccessary, write as a *= dx_real<T,Algorithm::Accurate>(b), explicitly.
  INLINE auto const& operator*= ( dX_real::dx_real<T,Aa>& a, dX_real::dx_real<T,Ab> const& b ) {
    return dX_real::operator_mul_ow ( a, b );;
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto const& operator*= ( dX_real::dx_real<T,Aa>& a, Ts const& b ) {
    return dX_real::operator_mul_ow ( a, T(b) );;
  }


  //
  // Division
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_div_body ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = dX_real::dx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::div_DW_DW_DW( a.x[0], a.x[1], b.x[0], b.x[1], c.x[0], c.x[1] );
    } else {
      QxW::div_PA_PA_PA( a.x[0], a.x[1], b.x[0], b.x[1], c.x[0], c.x[1] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Ab >
  INLINE auto const operator_div_body ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = dX_real::dx_real<T,Ab>;
    TX c;
    if ( Ab == Algorithm::Accurate ) {
      QxW::div_SW_DW_DW( a, b.x[0], b.x[1], c.x[0], c.x[1] );
    } else {
      QxW::div_SW_PA_PA( a, b.x[0], b.x[1], c.x[0], c.x[1] );
      if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa >
  INLINE auto const operator_div_body ( dX_real::dx_real<T,Aa> const& a, T const& b ) {
    using TX = dX_real::dx_real<T,Aa>;
    TX c;
    if ( Aa == Algorithm::Accurate ) {
      QxW::div_DW_SW_DW( a.x[0], a.x[1], b, c.x[0], c.x[1] );
    } else {
      QxW::div_PA_SW_PA( a.x[0], a.x[1], b, c.x[0], c.x[1] );
      if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm A, IF_T_fp<T> >
  INLINE auto const operator_div_body ( T const& a, T const& b ) {
    using TX = dX_real::dx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::div_SW_SW_DW( a, b, c.x[0], c.x[1] );
    } else {
      QxW::div_SW_SW_PA( a, b, c.x[0], c.x[1] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  //
#if MX_REAL_USE_INF_NAN_EXCEPTION
  template < Algorithm A, typename T >
  INLINE auto const operator_div_exception ( T const& a, T const& b ) {
    using TX = dX_real::dx_real<T,A>;
    if ( fp<T>::isnan( a ) || fp<T>::isnan( b ) ) {
        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b ); throw TX{ c,c };
    }
    if ( fp<T>::isinf( a ) && fp<T>::isinf( b ) ) {
        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b ); throw TX{ c,c };
    }
    if ( fp<T>::isinf( a ) ) {
        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::inf, a ), b ); throw TX{ c,c };
    }
    if ( fp<T>::isinf( b ) ) {
        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::zero, a ), b ); throw TX{ c,c };
    }
    if ( fp<T>::is_zero( b ) ) {
      if ( fp<T>::is_zero( a ) ) {
        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b ); throw TX{ c,c };
      } else {
        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::inf, a ), b ); throw TX{ c,c };
      }
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_div_exception ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    auto sa = a.quick_Normalized();
    auto sb = b.quick_Normalized();
    dX_real::operator_div_exception <A>( sa, sb );
  }
#endif
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_div ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    using TX = dX_real::dx_real<T,A>;
    try { dX_real::operator_div_exception( a, b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( Ab != Algorithm::Quasi ) {
      return dX_real::operator_div_body( a, b );
    } else {
      return dX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm Ab >
  INLINE auto const operator_div ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    using TX = dX_real::dx_real<T,Ab>;
    try { dX_real::operator_div_exception<Ab>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( Ab != Algorithm::Quasi ) {
      return dX_real::operator_div_body( a, b );
    } else {
      return dX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm Aa >
  INLINE auto const operator_div ( dX_real::dx_real<T,Aa> const& a, T const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    using TX = dX_real::dx_real<T,Aa>;
    try { dX_real::operator_div_exception<Aa>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    return dX_real::operator_div_body( a, b );
  }
  template < typename T, Algorithm A, IF_T_fp<T> >
  INLINE auto const operator_div ( T const& a, T const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    using TX = dX_real::dx_real<T,A>;
    try { dX_real::operator_div_exception<A>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    return dX_real::operator_div_body( a, b );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator/ ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return dX_real::operator_div( a, b );
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  INLINE auto const operator/ ( Ts const& a, dX_real::dx_real<T,Ab> const& b ) {
    return dX_real::operator_div( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto const operator/ ( dX_real::dx_real<T,Aa> const& a, Ts const& b ) {
    return dX_real::operator_div( a, T(b) );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const& operator_div_ow ( dX_real::dx_real<T,Aa>& a, dX_real::dx_real<T,Ab> const& b ) {
    if ( Aa == Algorithm::Accurate ) {
      QxW::div_DW_DW_DW( a.x[0], a.x[1], b.x[0], b.x[1], a.x[0], a.x[1] );
    } else {
      QxW::div_PA_PA_PA( a.x[0], a.x[1], b.x[0], b.x[1], a.x[0], a.x[1] );
      if ( Aa != Algorithm::Quasi ) Normalize( a );
    }
    return a;
  }
  template < typename T, Algorithm Aa >
  INLINE auto const& operator_div_ow ( dX_real::dx_real<T,Aa>& a, T const& b ) {
    if ( Aa == Algorithm::Accurate ) {
      QxW::div_DW_SW_DW( a.x[0], a.x[1], b, a.x[0], a.x[1] );
    } else {
      QxW::div_PA_SW_PA( a.x[0], a.x[1], b, a.x[0], a.x[1] );
      if ( Aa != Algorithm::Quasi ) Normalize( a );
    }
    return a;
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab, IF_A_owAble<Aa,Ab> >
    // ::Accurate /= (::Sloppy or ::Quasi) : not allowed
    // if neccessary, write as a /= dx_real<T,Algorithm::Accurate>(b), explicitly.
  INLINE auto const& operator/= ( dX_real::dx_real<T,Aa>& a, dX_real::dx_real<T,Ab> const& b ) {
    return dX_real::operator_div_ow ( a, b );;
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto const& operator/= ( dX_real::dx_real<T,Aa>& a, Ts const& b ) {
    return dX_real::operator_div_ow ( a, T(b) );;
  }


  //
  // Absolute value
  //
  template < typename T, Algorithm Aa >
  INLINE auto const operator_abs_body ( dX_real::dx_real<T,Aa> const& a ) {
    if ( is_negative( a ) ) {
      return -a;
    } else {
      return  a;
    }
  }
  //
#if MX_REAL_USE_INF_NAN_EXCEPTION
  template < typename T, Algorithm Aa >
  INLINE auto const operator_abs_exception ( dX_real::dx_real<T,Aa> const& a ) {
    using TX = dX_real::dx_real<T,Aa>;
    if ( isnan( a ) ) { throw TX::nan(); }
    if ( isinf( a ) ) { throw TX::inf(); }
  }
#endif
  //
  template < typename T, Algorithm Aa >
  INLINE auto const operator_abs ( dX_real::dx_real<T,Aa> const& a ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    dX_real::operator_abs_exception( a );
#endif
    return dX_real::operator_abs_body( a );
  }
  template < typename T, Algorithm Aa >
  INLINE auto const abs ( dX_real::dx_real<T,Aa> const& a ) {
    return dX_real::operator_abs( a );
  }
  template < typename T, Algorithm Aa >
  INLINE dX_real::dx_real<T,Aa> constexpr dX_real::dx_real<T,Aa>::abs ( dX_real::dx_real<T,Aa> const& a ) {
    return dX_real::abs( a );
  }


  //
  // Square root
  //
  template < typename T, Algorithm Aa >
  INLINE auto const operator_sqrt_body ( dX_real::dx_real<T,Aa> const& a ) {
    using TX = dX_real::dx_real<T,Aa>;
    TX c;
    if ( Aa == Algorithm::Accurate ) {
      QxW::sqrt_DW_DW( a.x[0], a.x[1], c.x[0], c.x[1] );
    } else {
      QxW::sqrt_PA_PA( a.x[0], a.x[1], c.x[0], c.x[1] );
      if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa=Algorithm::Accurate, IF_T_fp<T> >
  INLINE auto const operator_sqrt_body ( T const& a ) {
    using TX = dX_real::dx_real<T,Aa>;
    TX c;
    if ( Aa == Algorithm::Accurate ) {
      QxW::sqrt_SW_DW( a, c.x[0], c.x[1] );
    } else {
      QxW::sqrt_SW_PA( a, c.x[0], c.x[1] );
      if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  //
#if MX_REAL_USE_INF_NAN_EXCEPTION
  template < typename T, Algorithm Aa >
  INLINE auto const operator_sqrt_exception ( dX_real::dx_real<T,Aa> const& a ) {
    using TX = dX_real::dx_real<T,Aa>;
    if ( fp<T>::isinf( a.x[0] ) ) { throw TX::inf(); }
    auto s = a.quick_Normalized();
    if ( fp<T>::is_zero( s ) ) { throw TX{ s }; }
    if ( fp<T>::is_negative( s ) ) { throw TX::nan(); }
  }
  template < Algorithm A, typename T, IF_T_fp<T> >
  INLINE auto const operator_sqrt_exception ( T const& a ) {
    using TX = dX_real::dx_real<T,A>;
    if ( fp<T>::isinf( a ) ) { throw TX::inf(); }
    auto s = a;
    if ( fp<T>::is_zero( s ) ) { throw TX{ s }; }
    if ( fp<T>::is_negative( s ) ) { throw TX::nan(); }
  }
#endif
  //
  template < typename T, Algorithm Aa >
  INLINE auto const operator_sqrt ( dX_real::dx_real<T,Aa> const& a ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    using TX = dX_real::dx_real<T,Aa>;
    try { dX_real::operator_sqrt_exception( a ); }
    catch ( TX const& e ) { return e; }
#else
    if ( dX_real::is_zero( a ) ) { return a; }
#endif
    if ( Aa != Algorithm::Quasi ) {
      return dX_real::operator_sqrt_body( a );
    } else {
      return dX_real::operator_sqrt_body( a.element_rotate() );
    }
  }
  template < typename T, Algorithm A, IF_T_fp<T> >
  INLINE auto const operator_sqrt ( T const& a ) {
    using TX = dX_real::dx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { dX_real::operator_sqrt_exception<A>( a ); }
    catch ( TX const& e ) { return e; }
#else
    if ( fp<T>::is_zero( a ) ) { return TX{ a }; }
#endif
    return dX_real::operator_sqrt_body<T,A>( a );
  }
  //
  template < typename T, Algorithm Aa >
  INLINE auto const sqrt ( dX_real::dx_real<T,Aa> const& a ) {
    return dX_real::operator_sqrt( a );
  }
  template < typename T, Algorithm A=Algorithm::Accurate, IF_T_fp<T> >
  INLINE auto const sqrt ( T const& a ) {
    return dX_real::operator_sqrt<T,A>( a );
  }
  //
  template < typename T, Algorithm Aa >
  INLINE dX_real::dx_real<T,Aa> constexpr dX_real::dx_real<T,Aa>::sqrt ( dX_real::dx_real<T,Aa> const& a ) {
    return dX_real::sqrt( a );
  }


  //
  // f minimum
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm, IF_A_noQuasi<A> >
  INLINE auto const operator_fmin_body ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const &b ) {
    using TX = dX_real::dx_real<T,A>;
    int i; for(i=0;i<TX::L-1;i++) {
      if ( a.x[i] != b.x[i] ) break;
    }
    if ( a.x[i] <= b.x[i] ) {
      return TX{ a };
    } else {
      return TX{ b };
    }
  }
  //
#if MX_REAL_USE_INF_NAN_EXCEPTION
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_fmin_exception ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const &b ) {
    using TX = dX_real::dx_real<T,A>;
    if ( isnan( a ) || isnan( b ) ) { throw TX::nan(); }
    if ( isinf( a ) && isinf( b ) ) {
      if ( is_positive( a ) && is_positive( b ) ) { throw a; }
      throw -TX::inf();
    }
    if ( isinf( a ) ) { throw is_negative( a ) ? a : b; }
    if ( isinf( b ) ) { throw is_negative( b ) ? b : a; }
  }
#endif
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_fmin ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const &b ) {
    using TX = dX_real::dx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { dX_real::operator_fmin_exception( a, b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( A == Algorithm::Quasi ) {
      using TT = dX_real::dx_real<T,Algorithm::Accurate>;
      return TX{ dX_real::operator_fmin_body( TT{ a }, TT{ b } ) };
    } else {
      return dX_real::operator_fmin_body( a, b );
    }
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const fmin ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const &b ) {
    return dX_real::operator_fmin( a, b );
  }


  //
  // f maximum
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm, IF_A_noQuasi<A> >
  INLINE auto const operator_fmax_body ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const &b ) {
    using TX = dX_real::dx_real<T,A>;
    int i; for(i=0;i<TX::L-1;i++) {
      if ( a.x[i] != b.x[i] ) break;
    }
    if ( a.x[i] >= b.x[i] ) {
      return TX{ a };
    } else {
      return TX{ b };
    }
  }
  //
#if MX_REAL_USE_INF_NAN_EXCEPTION
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_fmax_exception ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const &b ) {
    using TX = dX_real::dx_real<T,A>;
    if ( isnan( a ) || isnan( b ) ) { throw TX::nan(); }
    if ( isinf( a ) && isinf( b ) ) {
      if ( is_negative( a ) && is_negative( b ) ) { throw a; }
      throw TX::inf();
    }
    if ( isinf( a ) ) { throw is_positive( a ) ? a : b; }
    if ( isinf( b ) ) { throw is_positive( b ) ? b : a; }
  }
#endif
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_fmax ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const &b ) {
    using TX = dX_real::dx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { dX_real::operator_fmax_exception( a, b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( A == Algorithm::Quasi ) {
      using TT = dX_real::dx_real<T,Algorithm::Accurate>;
      return TX{ dX_real::operator_fmax_body( TT{ a }, TT{ b } ) };
    } else {
      return dX_real::operator_fmax_body( a, b );
    }
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const fmax ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const &b ) {
    return dX_real::operator_fmax( a, b );
  }


  //
  // PRNG
  //
  template < typename T, Algorithm A >
  INLINE auto rand () -> if_T_double<T,dX_real::dx_real<T,A>> {
    using TX = dX_real::dx_real<T,A>;
    auto constexpr f = fp<T>::half / (1 << 30);
    auto g = f;
    auto r = TX::zero();
    auto constexpr bits = std::numeric_limits<dX_real::dx_real<T,A>>::digits;
    auto constexpr rand_bits = std::numeric_limits<int>::digits - 1;
    for(int i=0; i<bits; i+=rand_bits ) {
      auto b_ = fp<T>::rand();
      auto b = T(b_);
      auto c = b * g;
           r = r + TX{ c };
           g = g * f;
    }
    return r;
  }
  template < typename T, Algorithm A >
  INLINE auto rand () -> if_T_float<T,dX_real::dx_real<T,A>> {
    using TX = dX_real::dx_real<T,A>;
    auto constexpr f = fp<T>::half / (1 << 30);
    auto g = f;
    auto r = TX::zero();
    auto constexpr bits = std::numeric_limits<dX_real::dx_real<T,A>>::digits;
    auto constexpr rand_bits = std::numeric_limits<int>::digits - 1;
    for(int i=0; i<bits; i+=rand_bits ) {
      auto b_ = fp<T>::rand();
      auto b = T( ( b_ & 0x7fff0000 ) >> 16 ) * (1 << 16);
      auto c = b * g;
           r = r + TX{ c };
           b = T( b_ & 0x0000ffff );
           c = b * g;
           r = r + TX{ c };
           g = g * f;
    }
    return r;
  }
  template < typename T, Algorithm A >
  INLINE dX_real::dx_real<T,A> constexpr dX_real::dx_real<T,A>::rand () {
    return dX_real::rand<T,A>();
  }


  //
  //
  template < typename T, Algorithm Aa >
  INLINE std::ostream& operator<< ( std::ostream& stream, dX_real::dx_real<T,Aa> const& a ) {
    stream << std::to_string(a.x[0])
    << " " << std::to_string(a.x[1]);
    return stream;
  }

}
}


#endif

