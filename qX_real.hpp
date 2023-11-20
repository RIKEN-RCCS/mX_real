#ifndef	QX_REAL_H
#define	QX_REAL_H

#include "mX_real.hpp"


namespace mX_real {
namespace qX_real {

  template < typename T, Algorithm A=Algorithm::Accurate >
  struct qx_real {

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
    static int constexpr L = 4;
    T x[L];

    //
    INLINE qx_real() {
      x[0] = x[1] = x[2] = x[3] = fp<T>::zero;
    }
    INLINE qx_real( T const& x0, T const& x1, T const& x2, T const& x3 ) {
      x[0] = x0; x[1] = x1; x[2] = x2; x[3] = x3;
    }
    INLINE qx_real(int const& h) {
      x[0] = T(h); x[1] = x[2] = x[3] = fp<T>::zero;
    }
    INLINE qx_real(T const *d) {
      x[0] = d[0]; x[1] = d[1]; x[2] = d[2]; x[3] = d[3];
    }

    //
    //
    INLINE qx_real( T const& h ) {
      x[0] = h; x[1] = x[2] = x[3] = fp<T>::zero;
    }
    template < Algorithm _A_ >
    INLINE qx_real( DX_REAL<_A_> const& h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = x[3] = fp<T>::zero;
      } else {
        twoSum( h.x[0], h.x[1], x[0], x[1] );
        quickSum( x[0], x[1] );
       	x[2] = x[3] = fp<T>::zero;
      }
    }
    template < Algorithm _A_ >
    INLINE qx_real( TX_REAL<_A_> const& h ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2]; x[3] = fp<T>::zero;
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
      } else {
	vecSum<-3>( x );
	VSEB_Sloppy<3>( x );
      }
    }
    template < Algorithm _A_ >
    INLINE qx_real( QX_REAL<_A_> const& h ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2]; x[3] = h.x[3];
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
      } else {
	vecSum<-4>( x );
	VSEB_Sloppy<4>( x );
      }
    }

    //
    //
    INLINE QX_REAL<> &operator=( int const& h ) {
      x[0] = T(h); x[1] = x[2] = x[3] = fp<T>::zero;
      return *this;
    }
    INLINE QX_REAL<> &operator=( T const& h ) {
      x[0] = h; x[1] = x[2] = x[3] = fp<T>::zero;
      return *this;
    }
    template < Algorithm _A_ >
    INLINE QX_REAL<> &operator=( DX_REAL<_A_> const& h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = x[3] = fp<T>::zero;
      } else {
        twoSum( h.x[0], h.x[1], x[0], x[1] );
        quickSum( x[0], x[1] );
        x[2] = x[3] = fp<T>::zero;
      }
      return *this;
    }
    template < Algorithm _A_ >
    INLINE QX_REAL<> &operator=( TX_REAL<_A_> const& h ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2]; x[3] = fp<T>::zero;
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
      } else {
        vecSum<-3>( x );
        VSEB_Sloppy<3>( x );
      }
      return *this;
    }
    template < Algorithm _A_ >
    INLINE QX_REAL<> &operator=( QX_REAL<_A_> const& h ) {
      if ( A == _A_ && this == (QX_REAL<>*)(&h) ) { return *this; }
      x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2]; x[3] = h.x[3];
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
      } else {
        vecSum<-4>( x );
        VSEB_Sloppy<4>( x );
      }
      return *this;
    }

    //
    //
    INLINE operator T() const {
      if ( A == Algorithm::Quasi ) {
        return x[0] + x[1] + x[2] + x[3];
      } else {
        return x[0];
      }
    }
    template < Algorithm _A_ >
    INLINE operator DX_REAL<_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[4] = { x[0], x[1], x[2], x[3] };
	vecSum<-4>( f );
	VSEB_Sloppy<2,4>( f );
	return DX_REAL<_A_>( f );
      } else {
	return DX_REAL<_A_>( x );
      }
    }
    template < Algorithm _A_ >
    INLINE operator TX_REAL<_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[4] = { x[0], x[1], x[2], x[3] };
	vecSum<-4>( f );
	VSEB_Sloppy<3,4>( f );
	return TX_REAL<_A_>( f );
      } else {
	return TX_REAL<_A_>( x );
      }
    }
    template < Algorithm _A_ >
    INLINE operator QX_REAL<_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[4] = { x[0], x[1], x[2], x[3] };
	vecSum<-4>( f );
	VSEB_Sloppy<4>( f );
	return QX_REAL<_A_>( f );
      } else {
	return QX_REAL<_A_>( x );
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
    // member constexpr functions
    // contents are always in Accurate format
    // but they follow in the Algorithm argument 'A'
    //
    static INLINE QX_REAL<> constexpr zero () { return QX_REAL<>( fp<T>::zero ); }
    static INLINE QX_REAL<> constexpr one  () { return QX_REAL<>( fp<T>::one ); }
    static INLINE QX_REAL<> constexpr two  () { return QX_REAL<>( fp<T>::two ); }
    static INLINE QX_REAL<> constexpr half () { return QX_REAL<>( fp<T>::half ); }

    static INLINE QX_REAL<> constexpr epsilon () {
	    T c = fp<T>::epsilon * fp<T>::half; c = (c * c); c = (c * c) * 2; return QX_REAL<>( c ); }

    static INLINE QX_REAL<> constexpr nan  () { T c = fp<T>::nan; return QX_REAL<>( c,c,c,c ); }
    static INLINE QX_REAL<> constexpr inf  () { T c = fp<T>::inf; return QX_REAL<>( c,c,c,c ); }

    static INLINE QX_REAL<> constexpr denorm_min  () {
      T c0 = fp<T>::denorm_min();
      return QX_REAL<>( c0 );
    }
    static INLINE QX_REAL<> constexpr min  () {
      T c0 = (fp<T>::min() * 8) * fp<T>::epsiloni * fp<T>::epsiloni * fp<T>::epsiloni;
      return QX_REAL<>( c0 );
    }
    static INLINE QX_REAL<> constexpr max  () {
      T c0 = fp<T>::max();
      T c1 = c0 * fp<T>::epsilon * fp<T>::half;
      T c2 = c1 * fp<T>::epsilon * fp<T>::half;
      T c3 = c2 * fp<T>::epsilon * fp<T>::half;
      return QX_REAL<>( c0, c1, c2, c3 );
    }


    static INLINE QX_REAL<> constexpr rand ();
    //
    static INLINE QX_REAL<> constexpr reversed_sign ( QX_REAL<> const& a );
    static INLINE bool      constexpr signbit       ( QX_REAL<> const& a );
    static INLINE bool      constexpr isinf         ( QX_REAL<> const& a );
    static INLINE bool      constexpr isnan         ( QX_REAL<> const& a );
    static INLINE bool      constexpr is_zero       ( QX_REAL<> const& a );
    static INLINE bool      constexpr is_positive   ( QX_REAL<> const& a );
    static INLINE bool      constexpr is_negative   ( QX_REAL<> const& a );
    static INLINE QX_REAL<> constexpr sqrt          ( QX_REAL<> const& a );
    static INLINE QX_REAL<> constexpr abs           ( QX_REAL<> const& a );
    static INLINE T         constexpr quick_Normalized( QX_REAL<> const& a ) {
      auto s = a.x[0];
      if ( A == Algorithm::Quasi ) { s += a.x[1] + a.x[2] + a.x[3]; }
      return s;
    }


    // operations to THIS object
    INLINE void constexpr reverse_sign () { x[0] = -x[0]; x[1] = -x[1]; x[2] = -x[2]; x[3] = -x[3]; }
    INLINE void constexpr zerofy () { x[0] = x[1] = x[2] = x[3] = fp<T>::zero; }
    //
    INLINE QX_REAL<> reversed_sign () const { return reversed_sign( *this ); }
    INLINE bool      signbit ()       const { return signbit( *this ); }
    INLINE bool      isinf ()         const { return isinf( *this ); }
    INLINE bool      isnan ()         const { return isnan( *this ); }
    INLINE bool      is_zero ()       const { return is_zero( *this ); }
    INLINE bool      is_positive ()   const { return is_positive( *this ); }
    INLINE bool      is_negative ()   const { return is_negative( *this ); }
    INLINE QX_REAL<> sqrt ()          const { return sqrt( *this ); }
    INLINE QX_REAL<> abs ()           const { return abs( *this ); }
    INLINE T         quick_Normalized () const { return quick_Normalized( *this ); }
    //
    INLINE QX_REAL<> element_rotate () const {
      T y[L]; y[0] = x[0]; y[1] = x[1]; y[2] = x[2]; y[3] = x[3];
      for(int i=0; i<L-1; i++) {
        if ( ! fp<T>::is_zero( y[0] ) ) { return QX_REAL<>( y ); }
        T t = y[0]; y[0] = y[1]; y[1] = y[2]; y[2] = y[3]; y[3] = t;
      }
      return QX_REAL<>( y );
    }

  };


  //
  // Alias names
  //
  template < typename T >
  using qX_real_accurate = qx_real<T,Algorithm::Accurate>;
  template < typename T >
  using qX_real_sloppy   = qx_real<T,Algorithm::Sloppy>;
  template < typename T >
  using qX_real_quasi    = qx_real<T,Algorithm::Quasi>;


  //
  // Bit-ops and Comparisons
  //
  template < typename T, Algorithm Aa >
  INLINE auto const isinf ( qx_real<T,Aa> const& a ) {
    return fp<T>::isinf( a.quick_Normalized() );
  }
  template < typename T, Algorithm Aa >
  INLINE auto const isnan ( qx_real<T,Aa> const& a ) {
    return fp<T>::isnan( a.quick_Normalized() );
  }
  template < typename T, Algorithm Aa >
  INLINE auto const signbit ( qx_real<T,Aa> const& a ) {
    return fp<T>::signbit( a.quick_Normalized() );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr is_zero ( qx_real<T,Aa> const& a ) {
    return a.quick_Normalized() == fp<T>::zero;
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr is_positive ( qx_real<T,Aa> const& a ) {
    return a.quick_Normalized() > fp<T>::zero;
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr is_negative ( qx_real<T,Aa> const& a ) {
    return a.quick_Normalized() < fp<T>::zero;
  }
  //
  template < typename T, Algorithm Aa >
  INLINE bool constexpr qx_real<T,Aa>::isinf ( qx_real<T,Aa> const& a ) {
    return qX_real::isinf<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr qx_real<T,Aa>::isnan ( qx_real<T,Aa> const& a ) {
    return qX_real::isnan<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr qx_real<T,Aa>::signbit ( qx_real<T,Aa> const& a ) {
    return qX_real::signbit<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr qx_real<T,Aa>::is_zero ( qx_real<T,Aa> const& a ) {
    return qX_real::is_zero<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr qx_real<T,Aa>::is_positive ( qx_real<T,Aa> const& a ) {
    return qX_real::is_positive<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr qx_real<T,Aa>::is_negative ( qx_real<T,Aa> const& a ) {
    return qX_real::is_negative<T,Aa>( a );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto operator== ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    if ( Aa == Algorithm::Quasi ) {
      return b == qx_real<T,Algorithm::Accurate>( a );
    } else {
      return a.x[0] == b.x[0] && a.x[1] == b.x[1] && a.x[2] == b.x[2] && a.x[3] == b.x[3];
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto operator!= ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    return !(a == b);
  }


  //
  // Unitary operations
  //
  template < typename T, Algorithm Aa >
  INLINE auto const reversed_sign ( qx_real<T,Aa> const& a ) {
    return qx_real<T,Aa>( -a.x[0], -a.x[1], -a.x[2], -a.x[3] );
  }
  template < typename T, Algorithm Aa >
  INLINE qx_real<T,Aa> constexpr qx_real<T,Aa>::reversed_sign ( qx_real<T,Aa> const& a ) {
    return qX_real::reversed_sign( a );
  }
  //
  template < typename T, Algorithm Aa >
  INLINE auto operator+ ( qx_real<T,Aa> const& a ) {
    return a;
  }
  template < typename T, Algorithm Aa >
  INLINE auto operator- ( qx_real<T,Aa> const& a ) {
    return qX_real::reversed_sign( a );
  }


  //
  // Addition
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto operator_add ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    using TX = qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::add_QW_QW_QW( a.x[0], a.x[1], a.x[2], a.x[3], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::add_QQW_QQW_QQW( a.x[0], a.x[1], a.x[2], a.x[3], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto operator_add ( tX_real::tx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    using TX = qx_real<T,A>;
    if ( A == Algorithm::Accurate ) {

      T f[6];
      twoSum( a.x[0], b.x[0], f[0], f[1] ); // 1 e
      twoSum( a.x[1], b.x[1], f[2], f[3] ); // e e^2
      twoSum( a.x[2], b.x[2], f[4], f[5] ); // e^2 e^3
      f[5] = f[5] + b.x[3]; // e^3

      twoSum( f[1], f[2], f[1], f[2] ); // e e^2
      twoSum( f[2], f[3], f[2], f[3] ); // e^2 e^3
      twoSum( f[2], f[4], f[2], f[4] ); // e^2 e^3
      f[3] = f[3] + f[4] + f[5];

      auto c = TX( f );
      Normalize( c );
      return c;
    } else {
      TX c;
      QxW::add_QTW_QQW_QQW( a.x[0], a.x[1], a.x[2], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
      return c;
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto operator_add ( dX_real::dx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    using TX = qx_real<T,A>;
    if ( A == Algorithm::Accurate ) {

      T f[5];
      twoSum( a.x[0], b.x[0], f[0], f[1] ); // 1 e
      twoSum( a.x[1], b.x[1], f[2], f[3] ); // e e^2
      twoSum( f[3], b.x[2], f[3], f[4] ); // e^2 e^3

      twoSum( f[1], f[2], f[1], f[2] ); // e e^2
      twoSum( f[2], f[3], f[2], f[3] ); // e^2 e^3
      f[3] = f[3] + f[4];

      auto c = TX( f );
      Normalize( c );
      return c;
    } else {
      TX c;
      QxW::add_PA_QQW_QQW( a.x[0], a.x[1], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
      return c;
    }
  }
  template < typename T, Algorithm Ab >
  INLINE auto operator_add ( T const& a, qx_real<T,Ab> const& b ) {
    using TX = qx_real<T,Ab>;
    if ( Ab == Algorithm::Accurate ) {

      T f[4];
      twoSum( a.x[0], b.x[0], f[0], f[1] ); // 1 e
      twoSum( f[1], b.x[1], f[1], f[2] ); // e e^2
      twoSum( f[2], b.x[2], f[2], f[3] ); // e^2 e^3

      auto c = TX( f );
      Normalize( c );
      return c;
    } else {
      TX c;
      QxW::add_SW_QQW_QQW( a.x[0], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
      return c;
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto operator_add ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qx_real<T,A>;
    if ( A == Algorithm::Accurate ) {
      T f[6];
      vecMerge<3,3>( f, a.x, b.x );
      vecSum_Sloppy<-6>( f );
      VSEB_Sloppy<4,6>( f );
      return TX( f );
    } else {
      TX c;
      QxW::add_QTW_QTW_QQW( a.x[0], a.x[1], a.x[2], b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
      return c;
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto operator_add ( dX_real::dx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qx_real<T,A>;
    if ( A == Algorithm::Accurate ) {
      T f[5];
      vecMerge<2,3>( f, a.x, b.x );
      vecSum_Sloppy<-5>( f );
      VSEB_Sloppy<4,5>( f );
      return TX( f );
    } else {
      TX c;
      QxW::add_PA_QTW_QQW( a.x[0], a.x[1], b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
      return c;
    }
  }
  template < typename T, Algorithm Ab >
  INLINE auto operator_add ( T const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qx_real<T,Ab>;
    if ( Ab == Algorithm::Accurate ) {
      T f[4];
      vecMerge<1,3>( f, a.x, b.x );
      vecSum_Sloppy<-4>( f );
      VSEB_Sloppy<4,4>( f );
      return TX( f );
    } else {
      TX c;
      QxW::add_SW_QTW_QQW( a, b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
      return c;
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto operator_add ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = qx_real<T,A>;
    if ( A == Algorithm::Accurate ) {
      T f[4];
      vecMerge<2,2>( f, a.x, b.x );
      vecSum_Sloppy<-4>( f );
      VSEB_Sloppy<4,4>( f );
      return TX( f );
    } else {
      TX c;
      QxW::add_PA_PA_QQW( a.x[0], a.x[1], b.x[0], b.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
      return c;
    }
  }
  template < typename T, Algorithm Ab >
  INLINE auto operator_add ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = qx_real<T,Ab>;
    TX c;
    QxW::add_SW_PA_QQW( a, b.x[0], b.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
    if ( Ab != Algorithm::Quasi ) { VSEB_Sloppy<3>( c.x ); }
    return c;
  }
  template < typename T, Algorithm A, IF_T_fp<T> >
  INLINE auto operator_add ( T const& a, T const& b ) {
    using TX = qx_real<T,A>;
    TX c;
    QxW::add_SW_SW_QQW( a, b, c.x[0], c.x[1], c.x[2], c.x[3] );
    return c;
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto operator+ ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    return operator_add( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto operator+ ( tX_real::tx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    return operator_add( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto operator+ ( qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    return b + a;
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto operator+ ( dX_real::dx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    return operator_add( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto operator+ ( qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return b + a;
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  INLINE auto operator+ ( Ts const& a, qx_real<T,Ab> const& b ) {
    return operator_add( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto operator+ ( qx_real<T,Aa> const& a, Ts const& b ) {
    return b + a;
  }


  //
  // Substruction
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto operator- ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto operator- ( tX_real::tx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto operator- ( qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto operator- ( dX_real::dx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto operator- ( qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  INLINE auto operator- ( Ts const& a, qx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto operator- ( qx_real<T,Aa> const& a, Ts const& b ) {
    return a + (-b);
  }


  //
  // Multiplication
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto operator_mul ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    using TX = qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::mul_QW_QW_QW( a.x[0], a.x[1], a.x[2], a.x[3], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::mul_QQW_QQW_QQW( a.x[0], a.x[1], a.x[2], a.x[3], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto operator* ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    return operator_mul( a, b );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto operator_mul ( tX_real::tx_real<T,Aa> const& a, qx_real<T,Ab> const& b )
  -> qx_real<T,A> {
    return qx_real<T,A>( a ) * b;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto operator_mul ( dX_real::dx_real<T,Aa> const& a, qx_real<T,Ab> const& b )
  -> qx_real<T,A> {
    return qx_real<T,A>( a ) * b;
  }
  template < typename T, Algorithm Ab >
  INLINE auto operator_mul ( T const& a, qx_real<T,Ab> const& b )
  -> qx_real<T,Ab> {
    return qx_real<T,Ab>( a ) * b;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto operator_mul ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b )
  -> qx_real<T,A> {
    return qx_real<T,A>( a ) * qx_real<T,A>( b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto operator_mul ( dX_real::dx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b )
  -> qx_real<T,A> {
    return qx_real<T,A>( a ) * qx_real<T,A>( b );
  }
  template < typename T, Algorithm Ab >
  INLINE auto operator_mul ( T const& a, tX_real::tx_real<T,Ab> const& b )
  -> qx_real<T,Ab> {
    return qx_real<T,Ab>( a ) * qx_real<T,Ab>( b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto operator_mul ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b )
  -> qx_real<T,A> {
    return qx_real<T,A>( a ) * qx_real<T,A>( b );
  }
  template < typename T, Algorithm A, IF_T_fp<T> >
  INLINE auto operator_mul ( T const& a, T const& b ) {
    return qx_real<T,A>( a ) * qx_real<T,A>( b );
  }
  //
//  template < typename T, Algorithm Aa, Algorithm Ab >
//  INLINE auto operator* ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
//    return operator_mul( a, b );
//  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto operator* ( tX_real::tx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    using TX = qx_real<T,Aa>;
    return operator_mul( TX( a ), b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto operator* ( qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    return b * a;
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto operator* ( dX_real::dx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    using TX = qx_real<T,Aa>;
    return operator_mul( TX( a ), b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto operator* ( qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return b * a;
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  INLINE auto operator* ( Ts const& a, qx_real<T,Ab> const& b ) {
    using TX = qx_real<T,Ab>;
    return operator_mul( TX( T(a) ), b );
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto operator* ( qx_real<T,Aa> const& a, Ts const& b ) {
    return b * a;
  }


  //
  // Division
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_div ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b) {
  if ( A != Algorithm::Quasi ) {
  //
  // Accurate in QD by Bailey and Hida
  //
    using TX = qx_real<T,A>;
    {
      if ( b.x[0] == fp<T>::inf ) { return TX::zero(); }
      if ( b.x[0] == -fp<T>::inf ) { return -TX::zero(); }
      if ( b.x[0] == fp<T>::zero ) { auto c = fp<T>::copysign( fp<T>::inf, b.x[0] ); return TX( c,c,c,c ); }
    }
    auto q0 = a.x[0] / b.x[0];
    auto r = a - (b * q0);
      //
    auto q1 = r.x[0] / b.x[0];
    r = r - (b * q1);
      //
    auto q2 = r.x[0] / b.x[0];
    r = r - (b * q2);
      //
    auto q3 = r.x[0] / b.x[0];
    r = r - (b * q3);
      //
    if ( A == Algorithm::Sloppy ) {
      T f[4] = { q0, q1, q2, q3 };
      VSEB_Sloppy<4>( f );
      return TX( f );
    } else {
      auto q4 = r.x[0] / b.x[0];
      //
      T f[5] = { q0, q1, q2, q3, q4 };
      VSEB_Sloppy<5>( f );
      return TX( f );
    }
  } else {
  //
  // Quasi by Ozaki
  //
    using TX = qx_real<T,A>;
    {
      if ( b.x[0] == fp<T>::inf ) { return TX::zero(); }
      if ( b.x[0] == -fp<T>::inf ) { return -TX::zero(); }
    }
    auto s = b.x[0] + b.x[1] + b.x[2];
    {
      if ( s + b.x[3] == fp<T>::zero ) {
        s = s + b.x[3]; auto c = fp<T>::copysign( fp<T>::inf, s ); return TX( c,c,c,c ); }
      if ( b.x[0] == fp<T>::zero ) { return operator_div( a, b.element_rotate() ); }
    }
    auto c = tX_real::tx_real<T,A>( a ) / tX_real::tx_real<T,A>( b );
    auto t = a - c * b;
    auto r = TX( c );
    r.x[3] = (t.x[0] + t.x[1] + t.x[2] + t.x[3]) / s;
    return r;
  }
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator/ ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b) {
    return operator_div( a, b );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_div ( tX_real::tx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    using TX = qx_real<T,A>;
    return (TX(a) / b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_div ( qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qx_real<T,A>;
    return (a / TX(b));
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_div ( dX_real::dx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    using TX = qx_real<T,A>;
    return (TX(a) / b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_div ( qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = qx_real<T,A>;
    return (a / TX(b));
  }
  template < typename T, Algorithm Ab, IF_T_fp<T> >
  inline auto operator_div ( T const& a, qx_real<T,Ab> const& b ) {
    using TX = qx_real<T,Ab>;
    return (TX(a) / b);
  }
  template < typename T, Algorithm Aa, IF_T_fp<T> >
  inline auto operator_div ( qx_real<T,Aa> const& a, T const& b ) {
    using TX = qx_real<T,Aa>;
    return (a / TX(b));
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_div ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qx_real<T,A>;
    TX c;
    QxW::mul_QTW_QTW_QQW( a.x[0], a.x[1], a.x[2], b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_div ( dX_real::dx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qx_real<T,A>;
    return (TX(a) / TX(b));
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_div ( tX_real::tx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = qx_real<T,A>;
    return (TX(a) / TX(b));
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_div ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = qx_real<T,A>;
    return (TX(a) / TX(b));
  }
  template < typename T, Algorithm Ab >
  inline auto operator_div ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = qx_real<T,Ab>;
    return (TX(a) / TX(b));
  }
  template < typename T, Algorithm Aa >
  inline auto operator_div ( dX_real::dx_real<T,Aa> const& a, T const& b ) {
    using TX = qx_real<T,Aa>;
    return (TX(a) / TX(b));
  }
  template < typename T, Algorithm A, IF_T_fp<T> >
  inline auto operator_div ( T const& a, T const& b ) {
    using TX = qx_real<T,A>;
    return (TX(a) / TX(b));
  }
  //
//  template < typename T, Algorithm Aa, Algorithm Ab >
//  INLINE auto operator/ ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b) {
//    return operator_div( a, b );
//  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator/ ( tX_real::tx_real<T,Aa> const& a, qx_real<T,Ab> const& b) {
    return operator_div( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator/ ( qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b) {
    return operator_div( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator/ ( dX_real::dx_real<T,Aa> const& a, qx_real<T,Ab> const& b) {
    return operator_div( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator/ ( qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b) {
    return operator_div( a, b );
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  inline auto operator/ ( Ts const& a, qx_real<T,Ab> const& b) {
    return operator_div( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  inline auto operator/ ( qx_real<T,Aa> const& a, Ts const& b) {
    return operator_div( a, T(b) );
  }


  //
  // Absolute value
  //
  template < typename T, Algorithm Aa >
  INLINE auto const abs ( qx_real<T,Aa> const& a ) {
    if ( is_negative( a ) ) {
      return -a;
    } else {
      return  a;
    }
  }
  //
  template < typename T, Algorithm Aa >
  INLINE qx_real<T,Aa> constexpr qx_real<T,Aa>::abs ( qX_real::qx_real<T,Aa> const& a ) {
    return qX_real::abs( a );
  }


  //
  // Square root
  //
  template < typename T, Algorithm Aa >
  INLINE auto const operator_sqrt_ ( qx_real<T,Aa> const& a ) {
    using TX = qx_real<T,Aa>;
    if ( Aa == Algorithm::Accurate ) {
      // scaling
      auto as = fp<T>::sqrt( a.x[0] );
      auto e  = fp<T>::exponent( as );
      auto ex = fp<T>::exponenti( as );
      auto ex2 = ex * fp<T>::half;

      auto ax = TX( (a.x[0]*ex)*ex2,
                    (a.x[1]*ex)*ex2,
                    (a.x[2]*ex)*ex2,
                    (a.x[3]*ex)*ex2 );

      auto r  = TX( e / as );
      r = (3*TX::half() - ax * (r * r)) * r;
      r = (3*TX::half() - ax * (r * r)) * r;
      r = (3*TX::half() - ax * (r * r)) * r;

      // scaling back
      r = ax * r;
      e = 2 * e;
      r.x[0] *= e;
      r.x[1] *= e;
      r.x[2] *= e;
      r.x[3] *= e;

      return r;
    } else {
      auto r = tX_real::operator_sqrt_body( tX_real::tx_real<T,Aa>( a ) );
      auto t = a - qX_real::operator_mul( r, r );
      auto c = TX( r );
      c.x[3] = (t.x[0] + t.x[1] + t.x[2] + t.x[3]) / (2*(c.x[0] + c.x[1] + c.x[2]));
      if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
      return c;
    }
  }
  template < typename T, Algorithm Aa >
  INLINE auto const operator_sqrt_ ( tX_real::tx_real<T,Aa> const& a ) {
    using TX = qx_real<T,Aa>;
    if ( Aa == Algorithm::Accurate ) {
      return qX_real::operator_sqrt_( qx_real<T,Aa>( a ) );
    } else {
      TX c;
      QxW::sqrt_QTW_QQW( a.x[0], a.x[1], a.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
      return c;
    }
  }
  template < typename T, Algorithm Aa >
  INLINE auto const operator_sqrt_ ( dX_real::dx_real<T,Aa> const& a ) {
    using TX = qx_real<T,Aa>;
    if ( Aa == Algorithm::Accurate ) {
      return qX_real::operator_sqrt_( qx_real<T,Aa>( a ) );
    } else {
      TX c;
      QxW::sqrt_PA_QQW( a.x[0], a.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
      return c;
    }
  }
  template < typename T, Algorithm Aa=Algorithm::Accurate >
  INLINE auto const operator_sqrt_ ( T const& a ) {
    using TX = qx_real<T,Aa>;
    if ( Aa == Algorithm::Accurate ) {
      return qX_real::operator_sqrt_( qx_real<T,Aa>( a ) );
    } else {
      TX c;
      QxW::sqrt_SW_QQW( a, c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
      return c;
    }
  }
  //
  template < typename T, Algorithm Aa >
  INLINE auto const sqrt ( qx_real<T,Aa> const& a ) {
    using TX = qx_real<T,Aa>;
    auto s = a.quick_Normalized();
    if ( fp<T>::is_zero( s ) ) { return TX( s ); }
    if ( fp<T>::is_negative( s ) ) { return TX::nan(); }

    return qX_real::operator_sqrt_( a );
  }
  template < typename T, Algorithm Aa >
  INLINE auto const operator_sqrt ( tX_real::tx_real<T,Aa> const& a ) {
    using TX = qx_real<T,Aa>;
    auto s = a.quick_Normalized();
    if ( fp<T>::is_zero( s ) ) { return TX( s ); }
    if ( fp<T>::is_negative( s ) ) { return TX::nan(); }

    return qX_real::operator_sqrt_( a );
  }
  template < typename T, Algorithm Aa >
  INLINE auto const sqrt ( dX_real::dx_real<T,Aa> const& a ) {
    using TX = qx_real<T,Aa>;
    auto s = a.quick_Normalized();
    if ( fp<T>::is_zero( s ) ) { return TX( s ); }
    if ( fp<T>::is_negative( s ) ) { return TX::nan(); }

    return qX_real::operator_sqrt_( a );
  }
  template < typename T, Algorithm Aa=Algorithm::Accurate >
  INLINE auto const sqrt ( T const& a ) {
    using TX = qx_real<T,Aa>;
    auto s = a;
    if ( fp<T>::is_zero( s ) ) { return TX( s ); }
    if ( fp<T>::is_negative( s ) ) { return TX::nan(); }

    return qX_real::operator_sqrt_( a );
  }
  //
  template < typename T, Algorithm Aa >
  INLINE qx_real<T,Aa> constexpr qx_real<T,Aa>::sqrt ( qX_real::qx_real<T,Aa> const& a ) {
    return qX_real::sqrt( a );
  }


  //
  // f minimum
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const fmin ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    using TX = qx_real<T,A>;
    if ( A == Algorithm::Quasi ) {
      using T_ = qx_real<T,Algorithm::Accurate>;
      return TX( qX_real::fmin( T_(a), T_(b) ) );
    } else {
      int i; for(i=0;i<4-1;i++) {
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
  INLINE auto const fmax ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    using TX = qx_real<T,A>;
    if ( A == Algorithm::Quasi ) {
      using T_ = qx_real<T,Algorithm::Accurate>;
      return TX( fmax( T_(a), T_(b) ) );
    } else {
      int i; for(i=0;i<4-1;i++) {
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
  INLINE auto rand () -> if_T_float<T,qx_real<T,A>> {
    using TX = qx_real<T,A>;
    auto constexpr f = fp<T>::one / (1<<16) / (1<<15);
    auto g = f;
    auto r = TX::zero();
    auto bits = TX::L * 53;
    for(int i=0; i<bits; i+=31 ) {
      auto b_ = fp<T>::rand();
      auto b = T (b_ );
      auto c = b * g;
           r = r + TX( c );
           g = g * f;
    }
    return r;
  }
  template < typename T, Algorithm A >
  INLINE auto rand () -> if_T_double<T,qx_real<T,A>> {
    using TX = qx_real<T,A>;
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
  INLINE qx_real<T,A> constexpr qx_real<T,A>::rand () {
    return qX_real::rand<T,A>();
  }


  //
  //
  template < typename T, Algorithm Aa >
  INLINE std::ostream& operator<< ( std::ostream& stream, qx_real<T,Aa> const& a ) {
    stream << std::to_string(a.x[0]) << " "
           << std::to_string(a.x[1]) << " "
           << std::to_string(a.x[2]) << " "
	   << std::to_string(a.x[3]);
    return stream;
  }

}
}


#endif

