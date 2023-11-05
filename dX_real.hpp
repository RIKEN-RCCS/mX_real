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
    static int constexpr L = 2;
    T x[L];

    //
    // Simple constructor
    //
    inline dx_real() {
      x[0] = x[1] = fp<T>::zero;
    }
    inline dx_real( T const& x0, T const& x1 ) {
      x[0] = x0; x[1] = x1;
    }
    inline dx_real( int const &h ) {
      x[0] = T(h); x[1] = fp<T>::zero;
    }
    inline dx_real( T const *d ) {
      x[0] = d[0]; x[1] = d[1];
    }

    //
    // Copy constructor with (or without) any Algorithm switch
    // auto zero_accurate = df_Real( 0 ); // constructor
    // auto zero_sloppy = df_Real_sloppy( zero_accurate ); // copy constructor
    //
    inline dx_real( T const &h ) { // constructor
      x[0] = h; x[1] = fp<T>::zero;
    }
    template < Algorithm _A_ >
    inline dx_real( dx_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1];
      } else {
        twoSum( h.x[0], h.x[1], x[0], x[1] );
        VSEB_Sloppy<2>( x );
      }
    }
    template < Algorithm _A_ >
    inline dx_real<T,A>( tX_real::tx_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1];
      } else {
        T f[3] = { h.x[0], h.x[1], h.x[2] };
        vecSum<3>( f );
	VSEB_Sloppy<2,3>( f );
	x[0] = f[0]; x[1] = f[1];
      }
    }
    template < Algorithm _A_ >
    inline dx_real<T,A>( qX_real::qx_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1];
      } else {
        T f[4] = { h.x[0], h.x[1], h.x[2], h.x[3] };
        vecSum<4>( f );
        VSEB_Sloppy<2,4>( f );
	x[0] = f[0]; x[1] = f[1];
      }
    }

    //
    // Copy-assignment operator
    // auto one = df_Real( 1 ); // a constructor
    // one = df_Real_sloppy( one ); // LHS is a copy-assignment operator
    //
    inline dx_real<T,A> &operator=( int const &h ) {
      x[0] = T(h); x[1] = fp<T>::zero;
      return *this;
    }
    inline dx_real<T,A> &operator=( T const &h ) {
      x[0] = h; x[1] = fp<T>::zero;
      return *this;
    }
    template < Algorithm _A_ >
    inline dx_real<T,A> &operator=( dx_real<T,_A_> const &h ) {
      if ( A == _A_ && this == (dx_real<T,A>*)(&h) ) { return *this; }
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1];
      } else {
        twoSum( h.x[0], h.x[1], x[0], x[1] );
        quickSum( x[0], x[1] );
      }
      return *this;
    }
    template < Algorithm _A_ >
    inline dx_real<T,A> &operator=( tX_real::tx_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1];
      } else {
        T f[3] = { h.x[0], h.x[1], h.x[2] };
        vecSum<3>( f );
        VSEB_Sloppy<2,3>( f );
	x[0] = f[0]; x[1] = f[1];
      }
      return *this;
    }
    template < Algorithm _A_ >
    inline dx_real<T,A> &operator=( qX_real::qx_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1];
      } else {
        T f[4] = { h.x[0], h.x[1], h.x[2], h.x[3] };
        vecSum<4>( f );
        VSEB_Sloppy<2,4>( f );
	x[0] = f[0]; x[1] = f[1];
      }
      return *this;
    }

    //
    // Casting to other datatypes like
    // auto one = df_Real( 1 ); // constructor with an argument
    // auto one_ = (sf_Real_quasi)one; // RHS is datacast while LHS is a copy constructor
    //
    inline operator T() const {
      if ( A == Algorithm::Quasi ) {
        return x[0] + x[1];
      } else {
        return x[0];
      }
    }
    template < Algorithm _A_ >
    inline operator dx_real<T,_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[2];
        twoSum( x[0], x[1], f[0], f[1] );
        VSEB_Sloppy<2>( f );
        return dx_real<T,_A_>( f );
      } else {
        return dx_real<T,_A_>( x );
      }
    }
    template < Algorithm _A_ >
    inline operator tX_real::tx_real<T,_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[2];
        twoSum( x[0], x[1], f[0], f[1] );
        VSEB_Sloppy<2>( f );
        return tX_real::tx_real<T,_A_>( f[0], f[1], fp<T>::zero );
      } else {
        return tX_real::tx_real<T,_A_>( x[0], x[1], fp<T>::zero );
      }
    }
    template < Algorithm _A_ >
    inline operator qX_real::qx_real<T,_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[2];
        twoSum( x[0], x[1], f[0], f[1] );
        VSEB_Sloppy<2>( f );
        return qX_real::qx_real<T,_A_>( f[0], f[1], fp<T>::zero, fp<T>::zero );
      } else {
        return qX_real::qx_real<T,_A_>( x[0], x[1], fp<T>::zero, fp<T>::zero );
      }
    }

    //
    // unneccessary operators are invalidaded
    //
    auto operator!  () = delete;
    auto operator~  () = delete;
    auto operator++ () = delete;
    auto operator-- () = delete;
    template < typename _T_ > auto operator&&  ( _T_ a ) = delete;
    template < typename _T_ > auto operator||  ( _T_ a ) = delete;
    template < typename _T_ > auto operator&=  ( _T_ a ) = delete;
    template < typename _T_ > auto operator^=  ( _T_ a ) = delete;
    template < typename _T_ > auto operator<<= ( _T_ a ) = delete;
    template < typename _T_ > auto operator>>= ( _T_ a ) = delete;
    template < typename _T_ > auto operator,   ( _T_ a ) = delete;
    template < typename _T_ > auto operator+=  ( _T_ a ) = delete;
    template < typename _T_ > auto operator-=  ( _T_ a ) = delete;
    template < typename _T_ > auto operator*=  ( _T_ a ) = delete;
    template < typename _T_ > auto operator/=  ( _T_ a ) = delete;
    auto operator() (...) = delete;


    //
    // member constexpr functions or constatns
    // contents are always in Accurate format
    // but they follow in the Algorithm argument 'A'
    //
    static inline dx_real<T,A> constexpr zero () { return dx_real<T,A>( fp<T>::zero ); }
    static inline dx_real<T,A> constexpr one  () { return dx_real<T,A>( fp<T>::one );  }
    static inline dx_real<T,A> constexpr two  () { return dx_real<T,A>( fp<T>::two );  }
    static inline dx_real<T,A> constexpr half () { return dx_real<T,A>( fp<T>::half ); }

    static inline dx_real<T,A> constexpr epsilon () {
      T c = fp<T>::epsilon * fp<T>::half; c = (c * c) * 2; return dx_real<T,A>( c ); }

    static inline dx_real<T,A> constexpr nan  () { T c = fp<T>::nan; return dx_real<T,A>( c,c ); }
    static inline dx_real<T,A> constexpr inf  () { T c = fp<T>::inf; return dx_real<T,A>( c,c ); }

    static inline dx_real<T,A> constexpr denorm_min  () {
      T c0 = fp<T>::denorm_min;
      return dx_real<T,A>( c0 );
    }
    static inline dx_real<T,A> constexpr min  () {
      T c0 = (fp<T>::min * 2) * fp<T>::epsiloni;
      return dx_real<T,A>( c0 );
    }
    static inline dx_real<T,A> constexpr max  () {
      T c0 = fp<T>::max;
      T c1 = c0 * fp<T>::epsilon * fp<T>::half;
      return dx_real<T,A>( c0, c1 );
    }

    static inline dx_real<T,A> constexpr rand ();
    static inline dx_real<T,A> constexpr reversed_sign ( dx_real<T,A> const& a );
    static inline dx_real<T,A> constexpr sqrt ( dx_real<T,A> const& a );
    static inline T constexpr quick_Normalized( dx_real<T,A> const& a ) {
      T s = a.x[0];
      if ( A == Algorithm::Quasi ) { s += a.x[1]; }
      return s;
    }


    // operations to THIS object
    inline void constexpr reverse_sign () { x[0] = -x[0]; x[1] = -x[1]; }
    inline void constexpr zerofy () { x[0] = x[1] = fp<T>::zero; }
    //
    inline dx_real<T,A> reversed_sign () const { return reversed_sign( *this ); }
    inline dx_real<T,A> sqrt () const { return sqrt( *this ); }
    inline T const quick_Normalized () const { return quick_Normalized( *this ); }
    inline dx_real<T,A> element_rotate () const { return dx_real<T,A>( x[1], x[0] ); }

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
  // Bit-ops and Comparation
  //
  template < typename T, Algorithm Aa >
  inline auto const isinf ( dx_real<T,Aa> const& a ) {
    return fp<T>::isinf( a.quick_Normalized() );
  }
  template < typename T, Algorithm Aa >
  inline auto const isnan ( dx_real<T,Aa> const& a ) {
    return fp<T>::isnan( a.quick_Normalized() );
  }
  template < typename T, Algorithm Aa >
  inline auto const signbit ( dx_real<T,Aa> const& a ) {
    return fp<T>::signbit( a.quick_Normalized() );
  }
  template < typename T, Algorithm Aa >
  static inline bool constexpr is_zero ( dx_real<T,Aa> const& a ) {
    return a.quick_Normalized() == fp<T>::zero;
  }
  template < typename T, Algorithm Aa >
  static inline bool constexpr is_positive ( dx_real<T,Aa> const& a ) {
    return a.quick_Normalized() > fp<T>::zero;
  }
  template < typename T, Algorithm Aa >
  static inline bool constexpr is_negative ( dx_real<T,Aa> const& a ) {
    return a.quick_Normalized() < fp<T>::zero;
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator== ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    if ( Aa == Algorithm::Quasi ) {
      return b == dx_real<T,Algorithm::Accurate>( a );
    } else {
      return a.x[0] == b.x[0] && a.x[1] == b.x[1];
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator!= ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    return !(a == b);
  }


  //
  // Unitary operators
  //
  template < typename T, Algorithm Aa >
  inline auto const reversed_sign ( dx_real<T,Aa> const& a ) {
    return dx_real<T,Aa>( -a.x[0], -a.x[1] );
  }
  template < typename T, Algorithm Aa >
  inline dx_real<T,Aa> constexpr dx_real<T,Aa>::reversed_sign ( dx_real<T,Aa> const& a ) {
    return dX_real::reversed_sign( a );
  }
  //
  template < typename T, Algorithm Aa >
  inline auto operator+ ( dx_real<T,Aa> const& a ) {
    return a;
  }
  template < typename T, Algorithm Aa >
  inline auto operator- ( dx_real<T,Aa> const& a ) {
    return dX_real::reversed_sign( a );
  }


  //
  // Addition
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_add ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
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
  inline auto operator_add ( T const& a, dx_real<T,Ab> const& b ) {
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
  inline auto operator_add ( T const& a, T const& b ) {
    using TX = dx_real<T,A>;
    TX c;
    QxW::add_SW_SW_PA( a, b, c.x[0], c.x[1] );
    return c;
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator+ ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    return dX_real::operator_add( a, b );
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  inline auto operator+ ( Ts const& a, dx_real<T,Ab> const& b ) {
    return dX_real::operator_add( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  inline auto operator+ ( dx_real<T,Aa> const& a, Ts const& b ) {
    return b + a;
  }


  //
  // Substruction
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator- ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  inline auto operator- ( Ts const& a, dx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename Ts, typename T, Algorithm Aa , IF_T_scalar<Ts> >
  inline auto operator- ( dx_real<T,Aa> const& a, Ts const& b ) {
    return a + (-b);
  }


  //
  // Multiplication
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_mul ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
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
  inline auto operator_mul ( T const& a, dx_real<T,Ab> const& b ) {
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
  inline auto operator_mul ( T const& a, T const& b ) {
    using TX = dx_real<T,A>;
    TX c;
    QxW::mul_SW_SW_PA( a, b, c.x[0], c.x[1] );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
    return c;
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator* ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    return dX_real::operator_mul( a, b );
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  inline auto operator* ( Ts const& a, dx_real<T,Ab> const& b ) {
    return dX_real::operator_mul( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  inline auto operator* ( dx_real<T,Aa> const& a, Ts const& b ) {
    return b * a;
  }


  //
  // Division
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_div ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    using TX = dx_real<T,A>;
    {
      if ( fp<T>::isinf( b ) ) {
        auto c = fp<T>::copysign( fp<T>::zero, b ); return TX( c );
      }
      auto s = b.quick_Normalized();
      if ( fp<T>::is_zero( s ) ) {
        auto c = fp<T>::copysign( fp<T>::inf, s ); return TX( c,c );
      }
      if ( A == Algorithm::Quasi ) {
        if ( fp<T>::is_zero( b.x[0] ) ) { return dX_real::operator_div( a, b.element_rotate() ); }
      }
    }
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
  inline auto operator_div ( T const& a, dx_real<T,Ab> const& b ) {
    using TX = dx_real<T,Ab>;
    {
      if ( fp<T>::isinf( b ) ) {
        auto c = fp<T>::copysign( fp<T>::zero, b ); return TX( c );
      }
      auto s = b.quick_Normalized();
      if ( fp<T>::is_zero( s ) ) {
        auto c = fp<T>::copysign( fp<T>::inf, s ); return TX( c,c );
      }
      if ( Ab == Algorithm::Quasi ) {
        if ( fp<T>::is_zero( b.x[0] ) ) { return dX_real::operator_div( a, b.element_rotate() ); }
      }
    }
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
  inline auto operator_div ( dx_real<T,Aa> const& a, T const& b ) {
    using TX = dx_real<T,Aa>;
    {
      if ( fp<T>::isinf( b ) ) {
        auto c = fp<T>::copysign( fp<T>::zero, b ); return TX( c );
      }
      if ( fp<T>::is_zero( b ) ) {
        auto c = fp<T>::copysign( fp<T>::inf, b ); return TX( c,c );
      }
    }
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
  inline auto operator_div ( T const& a, T const& b ) {
    using TX = dx_real<T,A>;
    {
      if ( fp<T>::isinf( b ) ) {
        auto c = fp<T>::copysign( fp<T>::zero, b ); return TX( c );
      }
      if ( fp<T>::is_zero( b ) ) {
        auto c = fp<T>::copysign( fp<T>::inf, b ); return TX( c,c );
      }
    }
    TX c;
    QxW::div_SW_SW_PA( a, b, c.x[0], c.x[1] );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
    return c;
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator/ ( dx_real<T,Aa> const& a, dx_real<T,Ab> const& b ) {
    return dX_real::operator_div( a, b );
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  inline auto operator/ ( Ts const& a, dx_real<T,Ab> const& b ) {
    return dX_real::operator_div( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  inline auto operator/ ( dx_real<T,Aa> const& a, Ts const& b ) {
    return dX_real::operator_div( a, T(b) );
  }


  //
  // Absolute value
  //
  template < typename T, Algorithm Aa >
  inline auto const abs ( dx_real<T,Aa> const& a ) {
    if ( is_negative( a ) ) {
      return -a;
    } else {
      return  a;
    }
  }


  //
  // Square root
  //
  template < typename T, Algorithm Aa >
  inline dx_real<T,Aa> const sqrt ( dx_real<T,Aa> const& a ) {
    using TX = dx_real<T,Aa>;
    {
      auto s = a.quick_Normalized();
      if ( fp<T>::is_zero( s ) ) { return TX( s ); }
      if ( fp<T>::is_negative( s ) ) { return TX::nan(); }
      if ( Aa == Algorithm::Quasi ) {
        if ( fp<T>::is_zero( a.x[0] ) ) { return dX_real::sqrt( a.element_rotate() ); }
      }
    }
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
  inline auto const sqrt ( T const& a ) {
    using TX = dx_real<T,Aa>;
    {
      if ( fp<T>::is_zero( a ) ) { return TX( a ); }
      if ( fp<T>::is_negative( a ) ) { return TX::nan(); }
    }
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
  template < typename T, Algorithm Aa >
  inline dx_real<T,Aa> constexpr dx_real<T,Aa>::sqrt ( dx_real<T,Aa> const& a ) {
    return dX_real::sqrt( a );
  }


  //
  // f minimum
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto const fmin ( dx_real<T,Aa> const& a, dx_real<T,Ab> const &b ) {
    using TX = dx_real<T,A>;
    if ( A == Algorithm::Quasi ) {
      using T_ = dx_real<T,Algorithm::Accurate>;
      return TX( dX_real::fmin( T_(a), T_(b) ) );
    } else {
      int i=0; for(i;i<2-1;i++) {
        if ( a.x[i] != b.x[i] ) break;
      }
      if ( a.x[i] <= b.x[i] ) {
        return TX( a );
      } else {
        return TX( b );
      }
    }
  }


  //
  // f maximum
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto const fmax ( dx_real<T,Aa> const& a, dx_real<T,Ab> const &b ) {
    using TX = dx_real<T,A>;
    if ( A == Algorithm::Quasi ) {
      using T_ = dx_real<T,Algorithm::Accurate>;
      return TX( dX_real::fmax( T_(a), T_(b) ) );
    } else {
      int i=0; for(i;i<2-1;i++) {
        if ( a.x[i] != b.x[i] ) break;
      }
      if ( a.x[i] >= b.x[i] ) {
        return TX( a );
      } else {
        return TX( b );
      }
    }
  }


  //
  // PRNG
  //
  template < typename T, Algorithm A >
  inline auto const rand () -> if_T_double<T,dx_real<T,A>> {
    using TX = dx_real<T,A>;
    auto constexpr f = fp<T>::one / (1<<16) / (1<<15);
    auto g = f;
    auto r = TX::zero();
    auto bits = TX::L * 53;
    for(int i=0; i<bits; i+=31 ) {
      auto b_ = fp<T>::rand();
      auto b = T(b_);
      auto c = b * g;
           r = r + TX( c );
           g = g * f;
    }
    return r;
  }
  template < typename T, Algorithm A >
  inline auto const rand () -> if_T_float<T,dx_real<T,A>> {
    using TX = dx_real<T,A>;
    auto constexpr f = fp<T>::one / (1<<16) / (1<<15);
    auto g = f;
    auto r = TX::zero();
    auto bits = TX::L * 24;
    for(int i=0; i<bits; i+=31 ) {
      auto b_ = fp<T>::rand();
      auto b = T( b_ & 0x7fff0000 );
      auto c = b * g;
           r = r + TX( c );
           b = T( b_ & 0x0000ffff );
           c = b * g;
           r = r + TX( c );
           g = g * f;
    }
    return r;
  }
  template < typename T, Algorithm A >
  inline dx_real<T,A> constexpr dx_real<T,A>::rand () {
    return dX_real::rand<T,A>();
  }


  //
  //
  template < typename T, Algorithm Aa >
  inline std::ostream& operator<< ( std::ostream& stream, dx_real<T,Aa> const& a ) {
    stream << std::to_string(a.x[0])
    << " " << std::to_string(a.x[1]);
    return stream;
  }

}
}


#endif

