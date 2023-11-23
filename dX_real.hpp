#ifndef	DX_REAL_H
#define	DX_REAL_H

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
        twoSum( h.x[0], h.x[1], x[0], x[1] );
	quickSum( x[0], x[1] );
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
        T f[3];
        twoSum( h.x[1], h.x[2], f[1], f[2] );
        twoSum( h.x[0], f[1], f[0], f[1] );
	quickSum( f[0], f[1], x[0], x[1] );
	quickSum( f[1], f[2], x[1], f[2] );
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
        T f[4];
        twoSum( h.x[2], h.x[3], f[2], f[3] );
        twoSum( h.x[1], f[2], f[1], f[2] );
        twoSum( h.x[0], f[1], f[0], f[1] );
	quickSum( f[0], f[1], x[0], x[1] );
	quickSum( f[1], f[2], x[1], f[2] );
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
        twoSum( h.x[0], h.x[1], x[0], x[1] );
        quickSum( x[0], x[1] );
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
        T f[3];
        twoSum( h.x[1], h.x[2], f[1], f[2] );
        twoSum( h.x[0], f[1], f[0], f[1] );
	quickSum( f[0], f[1], x[0], f[1] );
	quickSum( f[1], f[2], x[1], f[2] );
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
        T f[4];
        twoSum( h.x[2], h.x[3], f[2], f[3] );
        twoSum( h.x[1], f[2], f[1], f[2] );
        twoSum( h.x[0], f[1], f[0], f[1] );
	quickSum( f[0], f[1], x[0], f[1] );
	quickSum( f[1], f[2], x[1], f[2] );
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
        T f[2];
        twoSum( x[0], x[1], f[0], f[1] );
	quickSum( f[0], f[1] );
        return DX_REAL<_A_>{ f };
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
        T f[2];
        twoSum( x[0], x[1], f[0], f[1] );
	quickSum( f[0], f[1] );
        return TX_REAL<_A_>( f[0], f[1], fp<T>::zero );
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
        T f[2];
        twoSum( x[0], x[1], f[0], f[1] );
	quickSum( f[0], f[1] );
        return QX_REAL<_A_>( f[0], f[1], fp<T>::zero, fp<T>::zero );
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
    template < typename _T_ > auto const operator+=  ( _T_ a ) = delete;
    template < typename _T_ > auto const operator-=  ( _T_ a ) = delete;
    template < typename _T_ > auto const operator*=  ( _T_ a ) = delete;
    template < typename _T_ > auto const operator/=  ( _T_ a ) = delete;
    auto const operator() (...) = delete;
    template < typename _T_ > auto const operator[]  ( _T_ a ) = delete;


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
    INLINE void constexpr Normalize ()    { mX_real::Normalize( *this ); }
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
  using dX_real_accurate = dx_real<T,Algorithm::Accurate>;
  template < typename T >
  using dX_real_sloppy   = dx_real<T,Algorithm::Sloppy>;
  template < typename T >
  using dX_real_quasi    = dx_real<T,Algorithm::Quasi>;


  //
  // Bit-ops and Comparisons
  //
  template < typename T, Algorithm Aa >
  INLINE auto const isinf ( dx_real<T,Aa> const& a ) {
    return fp<T>::isinf( a.quick_Normalized() );
  }
  template < typename T, Algorithm Aa >
  INLINE auto const isnan ( dx_real<T,Aa> const& a ) {
    return fp<T>::isnan( a.quick_Normalized() );
  }
  template < typename T, Algorithm Aa >
  INLINE auto const signbit ( dx_real<T,Aa> const& a ) {
    return fp<T>::signbit( a.quick_Normalized() );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr is_zero ( dx_real<T,Aa> const& a ) {
    return a.quick_Normalized() == fp<T>::zero;
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr is_positive ( dx_real<T,Aa> const& a ) {
    return a.quick_Normalized() > fp<T>::zero;
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr is_negative ( dx_real<T,Aa> const& a ) {
    return a.quick_Normalized() < fp<T>::zero;
  }
  //
  template < typename T, Algorithm Aa >
  INLINE bool constexpr dx_real<T,Aa>::isinf ( dx_real<T,Aa> const& a ) {
    return dX_real::isinf<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr dx_real<T,Aa>::isnan ( dx_real<T,Aa> const& a ) {
    return dX_real::isnan<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr dx_real<T,Aa>::signbit ( dx_real<T,Aa> const& a ) {
    return dX_real::signbit<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr dx_real<T,Aa>::is_zero ( dx_real<T,Aa> const& a ) {
    return dX_real::is_zero<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr dx_real<T,Aa>::is_positive ( dx_real<T,Aa> const& a ) {
    return dX_real::is_positive<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr dx_real<T,Aa>::is_negative ( dx_real<T,Aa> const& a ) {
    return dX_real::is_negative<T,Aa>( a );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_eq ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    return a.x[0] == b.x[0] && a.x[1] == b.x[1];
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator== ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    if ( A == Algorithm::Quasi ) {
      using TT=dx_real<T,Algorithm::Accurate>;
      return dX_real::operator_eq( TT{a}, TT{b} );
    } else {
      return dX_real::operator_eq( a, b );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator!= ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    return !(a == b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator_lt ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    if ( a.x[0] != b.x[0] ) {
      return a.x[0] < b.x[0];
    } else {
      return a.x[1] < b.x[1];
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator< ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    if ( A == Algorithm::Quasi ) {
      using TT=dx_real<T,Algorithm::Accurate>;
      return dX_real::operator_lt( TT{a}, TT{b} );
    } else {
      return dX_real::operator_lt( a, b );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator_gt ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    if ( a.x[0] != b.x[0] ) {
      return a.x[0] > b.x[0];
    } else {
      return a.x[1] > b.x[1];
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator> ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    if ( A == Algorithm::Quasi ) {
      using TT=dx_real<T,Algorithm::Accurate>;
      return dX_real::operator_gt( TT{a}, TT{b} );
    } else {
      return dX_real::operator_gt( a, b );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator>= ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    return !(a < b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator<= ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    return !(a > b);
  }


  //
  // Unitary operators
  //
  template < typename T, Algorithm Aa >
  INLINE auto const reversed_sign ( dx_real<T,Aa> const& a ) {
    return dx_real<T,Aa>( -a.x[0], -a.x[1] );
  }
  template < typename T, Algorithm Aa >
  INLINE dx_real<T,Aa> constexpr dx_real<T,Aa>::reversed_sign ( dx_real<T,Aa> const& a ) {
    return dX_real::reversed_sign( a );
  }
  //
  template < typename T, Algorithm Aa >
  INLINE auto const operator+ ( dx_real<T,Aa> const& a ) {
    return a;
  }
  template < typename T, Algorithm Aa >
  INLINE auto const operator- ( dx_real<T,Aa> const& a ) {
    return dX_real::reversed_sign( a );
  }


  //
  // Addition
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_add ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    using TX = dx_real<T,A>;
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
  INLINE auto const operator_add ( T const& a, dx_real<T,Ab> const& b ) {
    using TX = dx_real<T,Ab>;
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
  INLINE auto const operator_add ( T const& a, T const& b ) {
    using TX = dx_real<T,A>;
    TX c;
    QxW::add_SW_SW_PA( a, b, c.x[0], c.x[1] );
    return c;
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator+ ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    return dX_real::operator_add( a, b );
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  INLINE auto const operator+ ( Ts const& a, dx_real<T,Ab> const& b ) {
    return dX_real::operator_add( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto const operator+ ( dx_real<T,Aa> const& a, Ts const& b ) {
    return b + a;
  }


  //
  // Substruction
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator- ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  INLINE auto const operator- ( Ts const& a, dx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename Ts, typename T, Algorithm Aa , IF_T_scalar<Ts> >
  INLINE auto const operator- ( dx_real<T,Aa> const& a, Ts const& b ) {
    return a + (-b);
  }


  //
  // Multiplication
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_mul ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    using TX = dx_real<T,A>;
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
  INLINE auto const operator_mul ( T const& a, dx_real<T,Ab> const& b ) {
    using TX = dx_real<T,Ab>;
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
  INLINE auto const operator_mul ( T const& a, T const& b ) {
    using TX = dx_real<T,A>;
    TX c;
    QxW::mul_SW_SW_PA( a, b, c.x[0], c.x[1] );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
    return c;
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator* ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    return dX_real::operator_mul( a, b );
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  INLINE auto const operator* ( Ts const& a, dx_real<T,Ab> const& b ) {
    return dX_real::operator_mul( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto const operator* ( dx_real<T,Aa> const& a, Ts const& b ) {
    return b * a;
  }


  //
  // Division
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_div_body ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    using TX = dx_real<T,A>;
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
  INLINE auto const operator_div_body ( T const& a, dx_real<T,Ab> const& b ) {
    using TX = dx_real<T,Ab>;
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
  INLINE auto const operator_div_body ( dx_real<T,Aa> const& a, T const& b ) {
    using TX = dx_real<T,Aa>;
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
    using TX = dx_real<T,A>;
    TX c;
    QxW::div_SW_SW_PA( a, b, c.x[0], c.x[1] );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
    return c;
  }
  //
#if MX_REAL_USE_INF_NAN_EXCEPTION
  template < Algorithm A, typename T, Algorithm Ab >
  INLINE auto const operator_div_exception ( dx_real<T,Ab> const& b ) {
    using TX = dx_real<T,A>;
    if ( fp<T>::isinf( b.x[0] ) ) {
      auto c = fp<T>::copysign( fp<T>::zero, b.x[0] ); throw TX{ c,c };
    }
    auto s = b.quick_Normalized();
    if ( fp<T>::is_zero( s ) ) {
      auto c = fp<T>::copysign( fp<T>::inf, s ); throw TX{ c,c };
    }
  }
  template < Algorithm A, typename T >
  INLINE auto const operator_div_exception ( T const& b ) {
    using TX = dx_real<T,A>;
    if ( fp<T>::isinf( b ) ) {
      auto c = fp<T>::copysign( fp<T>::zero, b ); throw TX{ c,c };
    }
    if ( fp<T>::is_zero( b ) ) {
      auto c = fp<T>::copysign( fp<T>::inf, b ); throw TX{ c,c };
    }
  }
#endif
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_div ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    using TX = dx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { dX_real::operator_div_exception<A>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( Ab != Algorithm::Quasi ) {
      return dX_real::operator_div_body( a, b );
    } else {
      return dX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_div ( T const& a, dx_real<T,Ab> const& b ) {
    using TX = dx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { dX_real::operator_div_exception<A>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( Ab != Algorithm::Quasi ) {
      return dX_real::operator_div_body( a, b );
    } else {
      return dX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm Aa >
  INLINE auto const operator_div ( dx_real<T,Aa> const& a, T const& b ) {
    using TX = dx_real<T,Aa>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { dX_real::operator_div_exception<Aa>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    return dX_real::operator_div_body( a, b );
  }
  template < typename T, Algorithm A, IF_T_fp<T> >
  INLINE auto const operator_div ( T const& a, T const& b ) {
    using TX = dx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { dX_real::operator_div_exception<A>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    return dX_real::operator_div_body( a, b );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator/ ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    return dX_real::operator_div( a, b );
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  INLINE auto const operator/ ( Ts const& a, dx_real<T,Ab> const& b ) {
    return dX_real::operator_div( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto const operator/ ( dx_real<T,Aa> const& a, Ts const& b ) {
    return dX_real::operator_div( a, T(b) );
  }


  //
  // Absolute value
  //
  template < typename T, Algorithm Aa >
  INLINE auto const operator_abs_body ( dx_real<T,Aa> const& a ) {
    if ( is_negative( a ) ) {
      return -a;
    } else {
      return  a;
    }
  }
  //
#if MX_REAL_USE_INF_NAN_EXCEPTION
  template < typename T, Algorithm Aa >
  INLINE auto const operator_abs_exception ( dx_real<T,Aa> const& a ) {
    using TX = dx_real<T,Aa>;
    if ( isnan( a ) ) { throw TX::nan(); }
    if ( isinf( a ) ) { throw TX::inf(); }
  }
#endif
  //
  template < typename T, Algorithm Aa >
  INLINE auto const operator_abs ( dx_real<T,Aa> const& a ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    dX_real::operator_abs_exception( a );
#endif
    return dX_real::operator_abs_body( a );
  }
  template < typename T, Algorithm Aa >
  INLINE auto const abs ( dx_real<T,Aa> const& a ) {
    return dX_real::operator_abs( a );
  }
  template < typename T, Algorithm Aa >
  INLINE dx_real<T,Aa> constexpr dx_real<T,Aa>::abs ( dx_real<T,Aa> const& a ) {
    return dX_real::abs( a );
  }


  //
  // Square root
  //
  template < typename T, Algorithm Aa >
  INLINE auto const operator_sqrt_body ( dx_real<T,Aa> const& a ) {
    using TX = dx_real<T,Aa>;
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
    using TX = dx_real<T,Aa>;
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
  INLINE auto const operator_sqrt_exception ( dx_real<T,Aa> const& a ) {
    using TX = dx_real<T,Aa>;
    if ( fp<T>::isinf( a.x[0] ) ) { throw TX::inf(); }
    auto s = a.quick_Normalized();
    if ( fp<T>::is_zero( s ) ) { throw TX{ s }; }
    if ( fp<T>::is_negative( s ) ) { throw TX::nan(); }
  }
  template < Algorithm A, typename T, IF_T_fp<T> >
  INLINE auto const operator_sqrt_exception ( T const& a ) {
    using TX = dx_real<T,A>;
    if ( fp<T>::isinf( a ) ) { throw TX::inf(); }
    auto s = a;
    if ( fp<T>::is_zero( s ) ) { throw TX{ s }; }
    if ( fp<T>::is_negative( s ) ) { throw TX::nan(); }
  }
#endif
  //
  template < typename T, Algorithm Aa >
  INLINE auto const operator_sqrt ( dx_real<T,Aa> const& a ) {
    using TX = dx_real<T,Aa>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
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
    using TX = dx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { dX_real::operator_sqrt_exception<A>( a ); }
    catch ( TX const& e ) { return e; }
#else
    if ( fp<T>::is_zero( a ) ) { return TX{ a }; }
#endif
    return dX_real::operator_sqrt_body( a );
  }
  //
  template < typename T, Algorithm Aa >
  INLINE auto const sqrt ( dx_real<T,Aa> const& a ) {
    return dX_real::operator_sqrt( a );
  }
  template < typename T, Algorithm Aa=Algorithm::Accurate, IF_T_fp<T> >
  INLINE auto const sqrt ( T const& a ) {
    return dX_real::operator_sqrt<T,Aa>( a );
  }
  //
  template < typename T, Algorithm Aa >
  INLINE dx_real<T,Aa> constexpr dx_real<T,Aa>::sqrt ( dx_real<T,Aa> const& a ) {
    return dX_real::sqrt( a );
  }


  //
  // f minimum
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm, IF_A_noQuasi<A> >
  INLINE auto const operator_fmin_body ( dx_real<T,Aa> const& a, dx_real<T,Ab> const &b ) {
    using TX = dx_real<T,A>;
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
  INLINE auto const operator_fmin_exception ( dx_real<T,Aa> const& a, dx_real<T,Ab> const &b ) {
    using TX = dx_real<T,A>;
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
  INLINE auto const operator_fmin ( dx_real<T,Aa> const& a, dx_real<T,Ab> const &b ) {
    using TX = dx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { dX_real::operator_fmin_exception( a, b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( A == Algorithm::Quasi ) {
      using TT = dx_real<T,Algorithm::Accurate>;
      return TX{ dX_real::operator_fmin_body( TT{ a }, TT{ b } ) };
    } else {
      return dX_real::operator_fmin_body( a, b );
    }
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const fmin ( dx_real<T,Aa> const& a, dx_real<T,Ab> const &b ) {
    return dX_real::operator_fmin( a, b );
  }


  //
  // f maximum
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm, IF_A_noQuasi<A> >
  INLINE auto const operator_fmax_body ( dx_real<T,Aa> const& a, dx_real<T,Ab> const &b ) {
    using TX = dx_real<T,A>;
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
  INLINE auto const operator_fmax_exception ( dx_real<T,Aa> const& a, dx_real<T,Ab> const &b ) {
    using TX = dx_real<T,A>;
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
  INLINE auto const operator_fmax ( dx_real<T,Aa> const& a, dx_real<T,Ab> const &b ) {
    using TX = dx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { dX_real::operator_fmax_exception( a, b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( A == Algorithm::Quasi ) {
      using TT = dx_real<T,Algorithm::Accurate>;
      return TX{ dX_real::operator_fmax_body( TT{ a }, TT{ b } ) };
    } else {
      return dX_real::operator_fmax_body( a, b );
    }
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const fmax ( dx_real<T,Aa> const& a, dx_real<T,Ab> const &b ) {
    return dX_real::operator_fmax( a, b );
  }


  //
  // PRNG
  //
  template < typename T, Algorithm A >
  INLINE auto rand () -> if_T_double<T,dx_real<T,A>> {
    using TX = dx_real<T,A>;
    auto constexpr f = fp<T>::one / (1<<16) / (1<<15);
    auto g = f;
    auto r = TX::zero();
    auto bits = TX::L * 53;
    for(int i=0; i<bits; i+=31 ) {
      auto b_ = fp<T>::rand();
      auto b = T(b_);
      auto c = b * g;
           r = r + TX{ c };
           g = g * f;
    }
    return r;
  }
  template < typename T, Algorithm A >
  INLINE auto rand () -> if_T_float<T,dx_real<T,A>> {
    using TX = dx_real<T,A>;
    auto constexpr f = fp<T>::one / (1<<16) / (1<<15);
    auto g = f;
    auto r = TX::zero();
    auto bits = TX::L * 24;
    for(int i=0; i<bits; i+=31 ) {
      auto b_ = fp<T>::rand();
      auto b = T( b_ & 0x7fff0000 );
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
  INLINE dx_real<T,A> constexpr dx_real<T,A>::rand () {
    return dX_real::rand<T,A>();
  }


  //
  //
  template < typename T, Algorithm Aa >
  INLINE std::ostream& operator<< ( std::ostream& stream, dx_real<T,Aa> const& a ) {
    stream << std::to_string(a.x[0])
    << " " << std::to_string(a.x[1]);
    return stream;
  }

}
}


#endif

