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
    // {DX,TX,QX}_REAL are shortcut type only within qx_real
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
    // A special member function utilized in constructors
    //
    static INLINE T constexpr quick_Normalized( QX_REAL<> const& a ) {
      auto s = a.x[0];
      if ( A == Algorithm::Quasi ) { s += a.x[1] + a.x[2] + a.x[3]; }
      return s;
    }

    //
    INLINE qx_real() {
      x[0] = x[1] = x[2] = x[3] = fp<T>::zero;
    }
    INLINE qx_real( T const& x0, T const& x1, T const& x2, T const& x3 ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = x0 + x1 + x2 + x3;
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        x[0] = x[1] = x[2] = x[3] = t;
      } else
#endif
      { x[0] = x0; x[1] = x1; x[2] = x2; x[3] = x3; }
    }
    INLINE qx_real(int const& h) {
      { x[0] = T(h); x[1] = x[2] = x[3] = fp<T>::zero; }
    }
    INLINE qx_real(T const *d) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = d[0] + d[1] + d[2] + d[3];
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        x[0] = x[1] = x[2] = x[3] = t;
      } else
#endif
      { x[0] = d[0]; x[1] = d[1]; x[2] = d[2]; x[3] = d[3]; }
    }

    //
    //
    INLINE qx_real( T const& h ) {
      x[0] = h; x[1] = x[2] = x[3] = fp<T>::zero;
    }
    template < Algorithm _A_ >
    INLINE qx_real( DX_REAL<_A_> const& h ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = h.quick_Normalized();
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        x[0] = x[1] = x[2] = x[3] = t;
      } else
#endif
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = x[3] = fp<T>::zero;
      } else {
        auto s = h;
	mX_real::Normalize<1>( s );
        x[0] = s.x[0]; x[1] = s.x[1]; x[2] = x[3] = fp<T>::zero;
      }
    }
    template < Algorithm _A_ >
    INLINE qx_real( TX_REAL<_A_> const& h ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = h.quick_Normalized();
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        x[0] = x[1] = x[2] = x[3] = t;
      } else
#endif
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2]; x[3] = fp<T>::zero;
      } else {
        auto s = h;
	mX_real::Normalize<1>( s );
        x[0] = s.x[0]; x[1] = s.x[1]; x[2] = s.x[2]; x[3] = fp<T>::zero;
      }
    }
    template < Algorithm _A_ >
    INLINE qx_real( QX_REAL<_A_> const& h ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = h.quick_Normalized();
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        x[0] = x[1] = x[2] = x[3] = t;
      } else
#endif
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2]; x[3] = h.x[3];
      } else {
        auto s = h;
	mX_real::Normalize<1>( s );
        x[0] = s.x[0]; x[1] = s.x[1]; x[2] = s.x[2]; x[3] = s.x[3];
      }
    }

    //
    //
    INLINE QX_REAL<> &operator=( int const& h )& {
      x[0] = T(h); x[1] = x[2] = x[3] = fp<T>::zero;
      return *this;
    }
    INLINE QX_REAL<> &operator=( T const& h )& {
      x[0] = h; x[1] = x[2] = x[3] = fp<T>::zero;
      return *this;
    }
    template < Algorithm _A_ >
    INLINE QX_REAL<> &operator=( DX_REAL<_A_> const& h )& {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = h.quick_Normalized();
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        x[0] = x[1] = x[2] = x[3] = t;
      } else
#endif
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = x[3] = fp<T>::zero;
      } else {
        auto s = h;
	mX_real::Normalize<1>( s );
        x[0] = s.x[0]; x[1] = s.x[1]; x[2] = x[3] = fp<T>::zero;
      }
      return *this;
    }
    template < Algorithm _A_ >
    INLINE QX_REAL<> &operator=( TX_REAL<_A_> const& h )& {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = h.quick_Normalized();
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        x[0] = x[1] = x[2] = x[3] = t;
      } else
#endif
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2]; x[3] = fp<T>::zero;
      } else {
        auto s = h;
	mX_real::Normalize<1>( s );
        x[0] = s.x[0]; x[1] = s.x[1]; x[2] = s.x[2]; x[3] = fp<T>::zero;
      }
      return *this;
    }
    template < Algorithm _A_ >
    INLINE QX_REAL<> &operator=( QX_REAL<_A_> const& h )& {
      if ( A == _A_ && this == (QX_REAL<>*)(&h) ) { return *this; }
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = h.quick_Normalized();
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        x[0] = x[1] = x[2] = x[3] = t;
      } else
#endif
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2]; x[3] = h.x[3];
      } else {
        auto s = h;
	mX_real::Normalize<1>( s );
        x[0] = s.x[0]; x[1] = s.x[1]; x[2] = s.x[2]; x[3] = s.x[3];
      }
      return *this;
    }

    //
    //
    explicit
    INLINE operator T() const noexcept {
      if ( A == Algorithm::Quasi ) {
        return x[0] + x[1] + x[2] + x[3];
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
        c.x[0] = c.x[1] = c.x[2] = c.x[3] = t;
        return c;
      } else
#endif
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        auto s = *this;
	mX_real::Normalize<1>( s );
	return DX_REAL<_A_>( s.x );
      } else {
	return DX_REAL<_A_>( x );
      }
    }
    template < Algorithm _A_ >
    explicit
    INLINE operator TX_REAL<_A_>() const noexcept {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = this->quick_Normalized();
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        DX_REAL<_A_> c;
        c.x[0] = c.x[1] = c.x[2] = c.x[3] = t;
        return c;
      } else
#endif
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        auto s = *this;
	mX_real::Normalize<1>( s );
	return TX_REAL<_A_>( s.x );
      } else {
	return TX_REAL<_A_>( x );
      }
    }
    template < Algorithm _A_ >
    explicit
    INLINE operator QX_REAL<_A_>() const noexcept {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      auto const t = this->quick_Normalized();
      if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
        DX_REAL<_A_> c;
        c.x[0] = c.x[1] = c.x[2] = c.x[3] = t;
        return c;
      } else
#endif
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        auto s = *this;
	mX_real::Normalize<1>( s );
	return QX_REAL<_A_>( s.x );
      } else {
	return QX_REAL<_A_>( x );
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


    // operations to THIS object
    INLINE void constexpr reverse_sign () { x[0] = -x[0]; x[1] = -x[1]; x[2] = -x[2]; x[3] = -x[3]; }
    INLINE void constexpr zerofy ()       { x[0] = x[1] = x[2] = x[3] = fp<T>::zero; }
    template < int N_itr = 0 >
    INLINE void constexpr Normalize ()    { mX_real::Normalize<N_itr>( *this ); }
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
  using qX_real_accurate = qX_real::qx_real<T,Algorithm::Accurate>;
  template < typename T >
  using qX_real_sloppy   = qX_real::qx_real<T,Algorithm::Sloppy>;
  template < typename T >
  using qX_real_quasi    = qX_real::qx_real<T,Algorithm::Quasi>;


  //
  // Bit-ops and Comparisons
  //
  template < typename T, Algorithm Aa >
  INLINE auto const isinf ( qX_real::qx_real<T,Aa> const& a ) {
    return fp<T>::isinf( a.quick_Normalized() );
  }
  template < typename T, Algorithm Aa >
  INLINE auto const isnan ( qX_real::qx_real<T,Aa> const& a ) {
    return fp<T>::isnan( a.quick_Normalized() );
  }
  template < typename T, Algorithm Aa >
  INLINE auto const signbit ( qX_real::qx_real<T,Aa> const& a ) {
    return fp<T>::signbit( a.quick_Normalized() );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr is_zero ( qX_real::qx_real<T,Aa> const& a ) {
    return a.quick_Normalized() == fp<T>::zero;
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr is_positive ( qX_real::qx_real<T,Aa> const& a ) {
    return a.quick_Normalized() > fp<T>::zero;
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr is_negative ( qX_real::qx_real<T,Aa> const& a ) {
    return a.quick_Normalized() < fp<T>::zero;
  }
  //
  template < typename T, Algorithm Aa >
  INLINE bool constexpr qX_real::qx_real<T,Aa>::isinf ( qX_real::qx_real<T,Aa> const& a ) {
    return qX_real::isinf<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr qX_real::qx_real<T,Aa>::isnan ( qX_real::qx_real<T,Aa> const& a ) {
    return qX_real::isnan<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr qX_real::qx_real<T,Aa>::signbit ( qX_real::qx_real<T,Aa> const& a ) {
    return qX_real::signbit<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr qX_real::qx_real<T,Aa>::is_zero ( qX_real::qx_real<T,Aa> const& a ) {
    return qX_real::is_zero<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr qX_real::qx_real<T,Aa>::is_positive ( qX_real::qx_real<T,Aa> const& a ) {
    return qX_real::is_positive<T,Aa>( a );
  }
  template < typename T, Algorithm Aa >
  INLINE bool constexpr qX_real::qx_real<T,Aa>::is_negative ( qX_real::qx_real<T,Aa> const& a ) {
    return qX_real::is_negative<T,Aa>( a );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_eq ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    return a.x[0] == b.x[0] && a.x[1] == b.x[1] && a.x[2] == b.x[2] && a.x[3] == b.x[3];
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator== ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    if ( Aa == Algorithm::Quasi ) {
      using TT = qx_real<T,Algorithm::Accurate>;
      return qX_real::operator_eq( TT{ a }, TT{ b } );
    } else {
      return qX_real::operator_eq( a, b );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator!= ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    return !(a == b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator_lt ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    if ( a.x[0] != b.x[0] ) {
      return a.x[0] < b.x[0];
    } else if ( a.x[1] != b.x[1] ) {
      return a.x[1] < b.x[1];
    } else if ( a.x[2] != b.x[2] ) {
      return a.x[2] < b.x[2];
    } else {
      return a.x[3] < b.x[3];
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator< ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    if ( A == Algorithm::Quasi ) {
      using TT = qX_real::qx_real<T,Algorithm::Accurate>;
      return qX_real::operator_lt( TT{ a }, TT{ b } );
    } else {
      return qX_real::operator_lt( a, b );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator_gt ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    if ( a.x[0] != b.x[0] ) {
      return a.x[0] > b.x[0];
    } else if ( a.x[1] != b.x[1] ) {
      return a.x[1] > b.x[1];
    } else if ( a.x[2] != b.x[2] ) {
      return a.x[2] > b.x[2];
    } else {
      return a.x[3] > b.x[3];
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator> ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    if ( A == Algorithm::Quasi ) {
      using TT = qX_real::qx_real<T,Algorithm::Accurate>;
      return qX_real::operator_gt( TT{ a }, TT{ b } );
    } else {
      return qX_real::operator_gt( a, b );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator>= ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    return !(a < b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator<= ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    return !(a > b);
  }


  //
  // Unitary operations
  //
  template < typename T, Algorithm Aa >
  INLINE auto const reversed_sign ( qX_real::qx_real<T,Aa> const& a ) {
    return qX_real::qx_real<T,Aa>( -a.x[0], -a.x[1], -a.x[2], -a.x[3] );
  }
  template < typename T, Algorithm Aa >
  INLINE qX_real::qx_real<T,Aa> constexpr qX_real::qx_real<T,Aa>::reversed_sign ( qX_real::qx_real<T,Aa> const& a ) {
    return qX_real::reversed_sign( a );
  }
  //
  template < typename T, Algorithm Aa >
  INLINE auto const operator+ ( qX_real::qx_real<T,Aa> const& a ) {
    return a;
  }
  template < typename T, Algorithm Aa >
  INLINE auto const operator- ( qX_real::qx_real<T,Aa> const& a ) {
    return qX_real::reversed_sign( a );
  }


  //
  // Addition
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_add ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
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
  INLINE auto const operator_add ( tX_real::tx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::add_TW_QW_QW( a.x[0], a.x[1], a.x[2], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::add_QTW_QQW_QQW( a.x[0], a.x[1], a.x[2], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_add ( dX_real::dx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::add_DW_QW_QW( a.x[0], a.x[1], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::add_PA_QQW_QQW( a.x[0], a.x[1], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Ab >
  INLINE auto const operator_add ( T const& a, qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,Ab>;
    TX c;
    if ( Ab == Algorithm::Accurate ) {
      QxW::add_SW_QW_QW( a.x[0], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::add_SW_QQW_QQW( a.x[0], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_add ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::add_TW_TW_QW( a.x[0], a.x[1], a.x[2], b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::add_QTW_QTW_QQW( a.x[0], a.x[1], a.x[2], b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_add ( dX_real::dx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::add_DW_TW_QW( a.x[0], a.x[1], b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::add_PA_QTW_QQW( a.x[0], a.x[1], b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Ab >
  INLINE auto const operator_add ( T const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,Ab>;
    TX c;
    if ( Ab == Algorithm::Accurate ) {
      QxW::add_SW_TW_QW( a, b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::add_SW_QTW_QQW( a, b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_add ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::add_PA_PA_QQW( a.x[0], a.x[1], b.x[0], b.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::add_PA_PA_QQW( a.x[0], a.x[1], b.x[0], b.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Ab >
  INLINE auto const operator_add ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,Ab>;
    TX c;
    if ( Ab == Algorithm::Accurate ) {
      QxW::add_SW_DW_QW( a, b.x[0], b.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::add_SW_PA_QQW( a, b.x[0], b.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm A, IF_T_fp<T> >
  INLINE auto const operator_add ( T const& a, T const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::add_SW_SW_QW( a, b, c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::add_SW_SW_QQW( a, b, c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator+ ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    return qX_real::operator_add( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator+ ( tX_real::tx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    return qX_real::operator_add( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator+ ( qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    return b + a;
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator+ ( dX_real::dx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    return qX_real::operator_add( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator+ ( qX_real::qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return b + a;
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  INLINE auto const operator+ ( Ts const& a, qX_real::qx_real<T,Ab> const& b ) {
    return qX_real::operator_add( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto const operator+ ( qX_real::qx_real<T,Aa> const& a, Ts const& b ) {
    return b + a;
  }
  //
  template < typename T, Algorithm Aa >
  INLINE auto const& operator_add_ow (  qX_real::qx_real<T,Aa>& a, qX_real::qx_real<T,Aa> const& b ) {
    if ( Aa == Algorithm::Accurate ) {
      QxW::add_QW_QW_QW( a.x[0], a.x[1], a.x[2], a.x[3], b.x[0], b.x[1], b.x[2], b.x[3], a.x[0], a.x[1], a.x[2], a.x[3] );
    } else {
      QxW::add_QQW_QQW_QQW( a.x[0], a.x[1], a.x[2], a.x[3], b.x[0], b.x[1], b.x[2], b.x[3], a.x[0], a.x[1], a.x[2], a.x[3] );
      if ( Aa != Algorithm::Quasi ) Normalize( a );
    }
    return a;
  }
  template < typename T, Algorithm Aa >
  INLINE auto const& operator_add_ow (  qX_real::qx_real<T,Aa>& a, tX_real::tx_real<T,Aa> const& b ) {
    if ( Aa == Algorithm::Accurate ) {
      QxW::add_QW_TW_QW( a.x[0], a.x[1], a.x[2], a.x[3], b.x[0], b.x[1], b.x[2], a.x[0], a.x[1], a.x[2], a.x[3] );
    } else {
      QxW::add_QQW_QTW_QQW( a.x[0], a.x[1], a.x[2], a.x[3], b.x[0], b.x[1], b.x[2], a.x[0], a.x[1], a.x[2], a.x[3] );
      if ( Aa != Algorithm::Quasi ) Normalize( a );
    }
    return a;
  }
  template < typename T, Algorithm Aa >
  INLINE auto const& operator_add_ow (  qX_real::qx_real<T,Aa>& a, dX_real::dx_real<T,Aa> const& b ) {
    if ( Aa == Algorithm::Accurate ) {
      QxW::add_QW_DW_QW( a.x[0], a.x[1], a.x[2], a.x[3], b.x[0], b.x[1], a.x[0], a.x[1], a.x[2], a.x[3] );
    } else {
      QxW::add_QQW_PA_QQW( a.x[0], a.x[1], a.x[2], a.x[3], b.x[0], b.x[1], a.x[0], a.x[1], a.x[2], a.x[3] );
      if ( Aa != Algorithm::Quasi ) Normalize( a );
    }
    return a;
  }
  template < typename T, Algorithm Aa >
  INLINE auto const& operator_add_ow (  qX_real::qx_real<T,Aa>& a, T const& b ) {
    if ( Aa == Algorithm::Accurate ) {
      QxW::add_QW_SW_QW( a.x[0], a.x[1], a.x[2], a.x[3], b, a.x[0], a.x[1], a.x[2], a.x[3] );
    } else {
      QxW::add_QQW_SW_QQW( a.x[0], a.x[1], a.x[2], a.x[3], b, a.x[0], a.x[1], a.x[2], a.x[3] );
      if ( Aa != Algorithm::Quasi ) Normalize( a );
    }
    return a;
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab, IF_A_owAble<Aa,Ab> >
    // ::Accurate += ::Sloppy or ::Quasi are not allowed
    // if neccessary, write as a += dx_real<T,Algorithm::Accurate>(b), explicitly.
  INLINE auto const& operator+= ( qX_real::qx_real<T,Aa>& a, qX_real::qx_real<T,Ab> const& b ) {
    return qX_real::operator_add_ow ( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, IF_A_owAble<Aa,Ab> >
    // ::Accurate += ::Sloppy or ::Quasi are not allowed
    // if neccessary, write as a += dx_real<T,Algorithm::Accurate>(b), explicitly.
  INLINE auto const& operator+= ( qX_real::qx_real<T,Aa>& a, tX_real::tx_real<T,Ab> const& b ) {
    return qX_real::operator_add_ow ( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, IF_A_owAble<Aa,Ab> >
    // ::Accurate += ::Sloppy or ::Quasi are not allowed
    // if neccessary, write as a += dx_real<T,Algorithm::Accurate>(b), explicitly.
  INLINE auto const& operator+= ( qX_real::qx_real<T,Aa>& a, dX_real::dx_real<T,Ab> const& b ) {
    return qX_real::operator_add_ow ( a, b );
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto const& operator+= ( qX_real::qx_real<T,Aa>& a, Ts const& b ) {
    return qX_real::operator_add_ow ( a, T(b) );
  }


  //
  // Substruction
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator- ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator- ( tX_real::tx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator- ( qX_real::qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator- ( dX_real::dx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator- ( qX_real::qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  INLINE auto const operator- ( Ts const& a, qX_real::qx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto const operator- ( qX_real::qx_real<T,Aa> const& a, Ts const& b ) {
    return a + (-b);
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const& operator-= ( qX_real::qx_real<T,Aa>& a, qX_real::qx_real<T,Ab> const& b ) {
    return a += (-b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const& operator-= ( qX_real::qx_real<T,Aa>& a, tX_real::tx_real<T,Ab> const& b ) {
    return a += (-b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const& operator-= ( qX_real::qx_real<T,Aa>& a, dX_real::dx_real<T,Ab> const& b ) {
    return a += (-b);
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto const& operator-= ( qX_real::qx_real<T,Aa>& a, Ts const& b ) {
    return a += (-b);
  }


  //
  // Multiplication
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_mul ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::mul_QW_QW_QW( a.x[0], a.x[1], a.x[2], a.x[3], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::mul_QQW_QQW_QQW( a.x[0], a.x[1], a.x[2], a.x[3], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_mul ( tX_real::tx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::mul_TW_QW_QW( a.x[0], a.x[1], a.x[2], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::mul_QTW_QQW_QQW( a.x[0], a.x[1], a.x[2], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_mul ( dX_real::dx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::mul_DW_QW_QW( a.x[0], a.x[1], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::mul_PA_QQW_QQW( a.x[0], a.x[1], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Ab >
  INLINE auto const operator_mul ( T const& a, qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,Ab>;
    TX c;
    if ( Ab == Algorithm::Accurate ) {
      QxW::mul_SW_QW_QW( a, b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::mul_SW_QQW_QQW( a, b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_mul ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::mul_TW_TW_QW( a.x[0], a.x[1], a.x[2], b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::mul_QTW_QTW_QQW( a.x[0], a.x[1], a.x[2], b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_mul ( dX_real::dx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::mul_DW_TW_QW( a.x[0], a.x[1], b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::mul_PA_QTW_QQW( a.x[0], a.x[1], b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Ab >
  INLINE auto const operator_mul ( T const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,Ab>;
    TX c;
    if ( Ab == Algorithm::Accurate ) {
      QxW::mul_SW_TW_QW( a, b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::mul_SW_QTW_QQW( a, b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_mul ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::mul_DW_DW_QW( a.x[0], a.x[1], b.x[0], b.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::mul_PA_PA_QQW( a.x[0], a.x[1], b.x[0], b.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Ab >
  INLINE auto const operator_mul ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,Ab>;
    TX c;
    if ( Ab == Algorithm::Accurate ) {
      QxW::mul_SW_DW_QW( a, b.x[0], b.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::mul_SW_PA_QQW( a, b.x[0], b.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm A, IF_T_fp<T> >
  INLINE auto const operator_mul ( T const& a, T const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::mul_SW_SW_QW( a, b, c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::mul_SW_SW_QQW( a, b, c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator* ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    return qX_real::operator_mul( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator* ( tX_real::tx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    return qX_real::operator_mul( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator* ( qX_real::qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    return b * a;
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator* ( dX_real::dx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    return qX_real::operator_mul( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto const operator* ( qX_real::qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return b * a;
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  INLINE auto const operator* ( Ts const& a, qX_real::qx_real<T,Ab> const& b ) {
    return qX_real::operator_mul( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto const operator* ( qX_real::qx_real<T,Aa> const& a, Ts const& b ) {
    return b * a;
  }
  //
  template < typename T, Algorithm Aa >
  INLINE auto const& operator_mul_ow (  qX_real::qx_real<T,Aa>& a, qX_real::qx_real<T,Aa> const& b ) {
    return (a = qX_real::operator_mul( a, b ));
  }
  template < typename T, Algorithm Aa >
  INLINE auto const& operator_mul_ow (  qX_real::qx_real<T,Aa>& a, tX_real::tx_real<T,Aa> const& b ) {
    return (a = qX_real::operator_mul( a, b ));
  }
  template < typename T, Algorithm Aa >
  INLINE auto const& operator_mul_ow (  qX_real::qx_real<T,Aa>& a, dX_real::dx_real<T,Aa> const& b ) {
    return (a = qX_real::operator_mul( a, b ));
  }
  template < typename T, Algorithm Aa >
  INLINE auto const& operator_mul_ow (  qX_real::qx_real<T,Aa>& a, T const& b ) {
    return (a = qX_real::operator_mul( a, b ));
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab, IF_A_owAble<Aa,Ab> >
    // ::Accurate += ::Sloppy or ::Quasi are not allowed
    // if neccessary, write as a += dx_real<T,Algorithm::Accurate>(b), explicitly.
  INLINE auto const& operator*= ( qX_real::qx_real<T,Aa>& a, qX_real::qx_real<T,Ab> const& b ) {
    return qX_real::operator_mul_ow ( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, IF_A_owAble<Aa,Ab> >
    // ::Accurate += ::Sloppy or ::Quasi are not allowed
    // if neccessary, write as a += dx_real<T,Algorithm::Accurate>(b), explicitly.
  INLINE auto const& operator*= ( qX_real::qx_real<T,Aa>& a, tX_real::tx_real<T,Ab> const& b ) {
    return qX_real::operator_mul_ow ( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, IF_A_owAble<Aa,Ab> >
    // ::Accurate += ::Sloppy or ::Quasi are not allowed
    // if neccessary, write as a += dx_real<T,Algorithm::Accurate>(b), explicitly.
  INLINE auto const& operator*= ( qX_real::qx_real<T,Aa>& a, dX_real::dx_real<T,Ab> const& b ) {
    return qX_real::operator_mul_ow ( a, b );
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  INLINE auto const& operator*= ( qX_real::qx_real<T,Aa>& a, Ts const& b ) {
    return qX_real::operator_mul_ow ( a, T(b) );
  }


  //
  // Division
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto const operator_div_body ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Quasi ) {
      QxW::div_QW_QW_QW( a.x[0], a.x[1], a.x[2], a.x[3], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::div_QQW_QQW_QQW( a.x[0], a.x[1], a.x[2], a.x[3], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto const operator_div_body ( tX_real::tx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::div_TW_QW_QW( a.x[0], a.x[1], a.x[2], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::div_QTW_QQW_QQW( a.x[0], a.x[1], a.x[2], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto const operator_div_body ( qX_real::qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::div_QW_TW_QW( a.x[0], a.x[1], a.x[2], a.x[3], b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::div_QQW_QTW_QQW( a.x[0], a.x[1], a.x[2], a.x[3], b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto const operator_div_body ( dX_real::dx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::div_DW_QW_QW( a.x[0], a.x[1], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::div_PA_QQW_QQW( a.x[0], a.x[1], b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto const operator_div_body ( qX_real::qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::div_QW_DW_QW( a.x[0], a.x[1], a.x[2], a.x[3], b.x[0], b.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::div_QQW_PA_QQW( a.x[0], a.x[1], a.x[2], a.x[3], b.x[0], b.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Ab, IF_T_fp<T> >
  inline auto const operator_div_body ( T const& a, qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,Ab>;
    TX c;
    if ( Ab == Algorithm::Accurate ) {
      QxW::div_SW_QW_QW( a, b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::div_SW_QQW_QQW( a, b.x[0], b.x[1], b.x[2], b.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, IF_T_fp<T> >
  inline auto const operator_div_body ( qX_real::qx_real<T,Aa> const& a, T const& b ) {
    using TX = qX_real::qx_real<T,Aa>;
    TX c;
    if ( Aa == Algorithm::Accurate ) {
      QxW::div_QW_SW_QW( a.x[0], a.x[1], a.x[2], a.x[3], b, c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::div_QQW_SW_QQW( a.x[0], a.x[1], a.x[2], a.x[3], b, c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto const operator_div_body ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::div_TW_TW_QW( a.x[0], a.x[1], a.x[2], b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::div_QTW_QTW_QQW( a.x[0], a.x[1], a.x[2], b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto const operator_div_body ( dX_real::dx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::div_DW_TW_QW( a.x[0], a.x[1], b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::div_PA_QTW_QQW( a.x[0], a.x[1], b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto const operator_div_body ( tX_real::tx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::div_TW_DW_QW( a.x[0], a.x[1], a.x[2], b.x[0], b.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::div_QTW_PA_QQW( a.x[0], a.x[1], a.x[2], b.x[0], b.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Ab >
  inline auto const operator_div_body ( T const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,Ab>;
    TX c;
    if ( Ab == Algorithm::Accurate ) {
      QxW::div_SW_TW_QW( a, b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::div_SW_QTW_QQW( a, b.x[0], b.x[1], b.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa >
  inline auto const operator_div_body ( tX_real::tx_real<T,Aa> const& a, T const& b ) {
    using TX = qX_real::qx_real<T,Aa>;
    TX c;
    if ( Aa == Algorithm::Accurate ) {
      QxW::div_TW_SW_QW( a.x[0], a.x[1], a.x[2], b, c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::div_QTW_SW_QQW( a.x[0], a.x[1], a.x[2], b, c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto const operator_div_body ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::div_DW_DW_QW( a.x[0], a.x[1], b.x[0], b.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::div_PA_PA_QQW( a.x[0], a.x[1], b.x[0], b.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Ab >
  inline auto const operator_div_body ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,Ab>;
    TX c;
    if ( Ab == Algorithm::Accurate ) {
      QxW::div_SW_DW_QW( a, b.x[0], b.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::div_SW_PA_QQW( a, b.x[0], b.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa >
  inline auto const operator_div_body ( dX_real::dx_real<T,Aa> const& a, T const& b ) {
    using TX = qX_real::qx_real<T,Aa>;
    TX c;
    if ( Aa == Algorithm::Accurate ) {
      QxW::div_DW_SW_QW( a.x[0], a.x[1], b, c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::div_PA_SW_QQW( a.x[0], a.x[1], b, c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm A, IF_T_fp<T> >
  inline auto const operator_div_body ( T const& a, T const& b ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::div_SW_SW_QW( a, b, c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::div_SW_SW_QQW( a, b, c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  //
#if MX_REAL_USE_INF_NAN_EXCEPTION
  template < Algorithm A, typename T, Algorithm Ab >
  INLINE auto const operator_div_exception ( qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    if ( fp<T>::isinf( b.x[0] ) ) {
      auto c = fp<T>::copysign( fp<T>::zero, b.x[0] ); throw TX{ c,c,c,c };
    }
    auto s = b.quick_Normalized();
    if ( fp<T>::is_zero( s ) ) {
      auto c = fp<T>::copysign( fp<T>::inf, s ); throw TX{ c,c,c,c };
    }
  }
  template < Algorithm A, typename T, Algorithm Ab >
  INLINE auto const operator_div_exception ( tX_real::tx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    if ( fp<T>::isinf( b.x[0] ) ) {
      auto c = fp<T>::copysign( fp<T>::zero, b.x[0] ); throw TX{ c,c,c,c };
    }
    auto s = b.quick_Normalized();
    if ( fp<T>::is_zero( s ) ) {
      auto c = fp<T>::copysign( fp<T>::inf, s ); throw TX{ c,c,c,c };
    }
  }
  template < Algorithm A, typename T, Algorithm Ab >
  INLINE auto const operator_div_exception ( dX_real::dx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
    if ( fp<T>::isinf( b.x[0] ) ) {
      auto c = fp<T>::copysign( fp<T>::zero, b.x[0] ); throw TX{ c,c,c,c };
    }
    auto s = b.quick_Normalized();
    if ( fp<T>::is_zero( s ) ) {
      auto c = fp<T>::copysign( fp<T>::inf, s ); throw TX{ c,c,c,c };
    }
  }
  template < Algorithm A, typename T >
  INLINE auto const operator_div_exception ( T const& b ) {
    using TX = qX_real::qx_real<T,A>;
    if ( fp<T>::isinf( b ) ) {
      auto c = fp<T>::copysign( fp<T>::zero, b ); throw TX{ c,c,c,c };
    }
    if ( fp<T>::is_zero( b ) ) {
      auto c = fp<T>::copysign( fp<T>::inf, b ); throw TX{ c,c,c,c };
    }
  }
#endif
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_div ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_div_exception<A>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( A != Algorithm::Quasi ) {
      return qX_real::operator_div_body( a, b );
    } else {
      return qX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_div ( tX_real::tx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_div_exception<A>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( A != Algorithm::Quasi ) {
      return qX_real::operator_div_body( a, b );
    } else {
      return qX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_div ( qX_real::qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_div_exception<A>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( A != Algorithm::Quasi ) {
      return qX_real::operator_div_body( a, b );
    } else {
      return qX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_div ( dX_real::dx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_div_exception<A>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( A != Algorithm::Quasi ) {
      return qX_real::operator_div_body( a, b );
    } else {
      return qX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_div ( qX_real::qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_div_exception<A>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( A != Algorithm::Quasi ) {
      return qX_real::operator_div_body( a, b );
    } else {
      return qX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm Ab >
  INLINE auto const operator_div ( T const& a, qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,Ab>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_div_exception<Ab>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( Ab != Algorithm::Quasi ) {
      return qX_real::operator_div_body( a, b );
    } else {
      return qX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm Aa >
  INLINE auto const operator_div ( qX_real::qx_real<T,Aa> const& a, T const& b ) {
    using TX = qX_real::qx_real<T,Aa>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_div_exception<Aa>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( Aa != Algorithm::Quasi ) {
      return qX_real::operator_div_body( a, b );
    } else {
      return qX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_div ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_div_exception<A>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( A != Algorithm::Quasi ) {
      return qX_real::operator_div_body( a, b );
    } else {
      return qX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_div ( dX_real::dx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_div_exception<A>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( A != Algorithm::Quasi ) {
      return qX_real::operator_div_body( a, b );
    } else {
      return qX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_div ( tX_real::tx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_div_exception<A>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( A != Algorithm::Quasi ) {
      return qX_real::operator_div_body( a, b );
    } else {
      return qX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm Ab >
  INLINE auto const operator_div ( T const& a, tX_real::tx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,Ab>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_div_exception<Ab>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( Ab != Algorithm::Quasi ) {
      return qX_real::operator_div_body( a, b );
    } else {
      return qX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm Aa >
  INLINE auto const operator_div ( tX_real::tx_real<T,Aa> const& a, T const& b ) {
    using TX = qX_real::qx_real<T,Aa>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_div_exception<Aa>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( Aa != Algorithm::Quasi ) {
      return qX_real::operator_div_body( a, b );
    } else {
      return qX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_div ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_div_exception<A>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( A != Algorithm::Quasi ) {
      return qX_real::operator_div_body( a, b );
    } else {
      return qX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm Ab >
  INLINE auto const operator_div ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,Ab>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_div_exception<Ab>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( Ab != Algorithm::Quasi ) {
      return qX_real::operator_div_body( a, b );
    } else {
      return qX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm Aa >
  INLINE auto const operator_div ( dX_real::dx_real<T,Aa> const& a, T const& b ) {
    using TX = qX_real::qx_real<T,Aa>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_div_exception<Aa>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( Aa != Algorithm::Quasi ) {
      return qX_real::operator_div_body( a, b );
    } else {
      return qX_real::operator_div_body( a, b.element_rotate() );
    }
  }
  template < typename T, Algorithm A, IF_T_fp<T> >
  INLINE auto const operator_div ( T const& a, T const& b ) {
    using TX = qX_real::qx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { tX_real::operator_div_exception<A>( b ); }
    catch ( TX const& e ) { return e; }
#endif
    return tX_real::operator_div_body( a, b );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto const operator/ ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b) {
    return qX_real::operator_div( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto const operator/ ( tX_real::tx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b) {
    return qX_real::operator_div( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto const operator/ ( qX_real::qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b) {
    return qX_real::operator_div( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto const operator/ ( dX_real::dx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b) {
    return qX_real::operator_div( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto const operator/ ( qX_real::qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b) {
    return qX_real::operator_div( a, b );
  }
  template < typename Ts, typename T, Algorithm Ab, IF_T_scalar<Ts> >
  inline auto const operator/ ( Ts const& a, qX_real::qx_real<T,Ab> const& b) {
    return qX_real::operator_div( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa, IF_T_scalar<Ts> >
  inline auto const operator/ ( qX_real::qx_real<T,Aa> const& a, Ts const& b) {
    return qX_real::operator_div( a, T(b) );
  }


  //
  // Absolute value
  //
  template < typename T, Algorithm Aa >
  INLINE auto const operator_abs_body ( qX_real::qx_real<T,Aa> const& a ) {
    if ( is_negative( a ) ) {
      return -a;
    } else {
      return  a;
    }
  }
  //
#if MX_REAL_USE_INF_NAN_EXCEPTION
  template < typename T, Algorithm Aa >
  INLINE auto const operator_abs_exception ( qX_real::qx_real<T,Aa> const& a ) {
    using TX = qX_real::qx_real<T,Aa>;
    if ( isnan( a ) ) { throw TX::nan(); }
    if ( isinf( a ) ) { throw TX::inf(); }
  }
#endif
  //
  template < typename T, Algorithm Aa >
  INLINE auto const operator_abs ( qX_real::qx_real<T,Aa> const& a ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    qX_real::operator_abs_exception( a );
#endif
    return qX_real::operator_abs_body( a );
  }
  template < typename T, Algorithm Aa >
  INLINE auto const abs ( qX_real::qx_real<T,Aa> const& a ) {
    return qX_real::operator_abs( a );
  }
  //
  template < typename T, Algorithm Aa >
  INLINE qX_real::qx_real<T,Aa> constexpr qX_real::qx_real<T,Aa>::abs ( qX_real::qx_real<T,Aa> const& a ) {
    return qX_real::abs( a );
  }


  //
  // Square root
  //
  template < typename T, Algorithm Aa >
  INLINE auto const operator_sqrt_body ( qX_real::qx_real<T,Aa> const& a ) {
    using TX = qX_real::qx_real<T,Aa>;
    TX c;
    if ( Aa == Algorithm::Accurate ) {
      QxW::sqrt_QW_QW( a.x[0], a.x[1], a.x[2], a.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::sqrt_QQW_QQW( a.x[0], a.x[1], a.x[2], a.x[3], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa >
  INLINE auto const operator_sqrt_body ( tX_real::tx_real<T,Aa> const& a ) {
    using TX = qX_real::qx_real<T,Aa>;
    TX c;
    if ( Aa == Algorithm::Accurate ) {
      QxW::sqrt_TW_QW( a.x[0], a.x[1], a.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::sqrt_QTW_QQW( a.x[0], a.x[1], a.x[2], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa >
  INLINE auto const operator_sqrt_body ( dX_real::dx_real<T,Aa> const& a ) {
    using TX = qX_real::qx_real<T,Aa>;
    TX c;
    if ( Aa == Algorithm::Accurate ) {
      QxW::sqrt_DW_QW( a.x[0], a.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::sqrt_PA_QQW( a.x[0], a.x[1], c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm A=Algorithm::Accurate >
  INLINE auto const operator_sqrt_body ( T const& a ) {
    using TX = qX_real::qx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::sqrt_SW_QW( a, c.x[0], c.x[1], c.x[2], c.x[3] );
    } else {
      QxW::sqrt_SW_QQW( a, c.x[0], c.x[1], c.x[2], c.x[3] );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  //
#if MX_REAL_USE_INF_NAN_EXCEPTION
  template < typename T, Algorithm Aa >
  INLINE auto const operator_sqrt_exception ( qX_real::qx_real<T,Aa> const& a ) {
    using TX = qX_real::qx_real<T,Aa>;
    if ( fp<T>::isinf( a.x[0] ) ) { throw TX::inf(); }
    auto s = a.quick_Normalized();
    if ( fp<T>::is_zero( s ) ) { throw TX{ s }; }
    if ( fp<T>::is_negative( s ) ) { throw TX::nan(); }
  }
  template < typename T, Algorithm Aa >
  INLINE auto const operator_sqrt_exception ( tX_real::tx_real<T,Aa> const& a ) {
    using TX = qX_real::qx_real<T,Aa>;
    if ( fp<T>::isinf( a.x[0] ) ) { throw TX::inf(); }
    auto s = a.quick_Normalized();
    if ( fp<T>::is_zero( s ) ) { throw TX{ s }; }
    if ( fp<T>::is_negative( s ) ) { throw TX::nan(); }
  }
  template < typename T, Algorithm Aa >
  INLINE auto const operator_sqrt_exception ( dX_real::dx_real<T,Aa> const& a ) {
    using TX = qX_real::qx_real<T,Aa>;
    if ( fp<T>::isinf( a.x[0] ) ) { throw TX::inf(); }
    auto s = a.quick_Normalized();
    if ( fp<T>::is_zero( s ) ) { throw TX{ s }; }
    if ( fp<T>::is_negative( s ) ) { throw TX::nan(); }
  }
  template < Algorithm A, typename T, IF_T_fp<T> >
  INLINE auto const operator_sqrt_exception ( T const& a ) {
    using TX = qX_real::qx_real<T,A>;
    if ( fp<T>::isinf( a ) ) { throw TX::inf(); }
    auto s = a;
    if ( fp<T>::is_zero( s ) ) { throw TX{ s }; }
    if ( fp<T>::is_negative( s ) ) { throw TX::nan(); }
  }
#endif
  //
  template < typename T, Algorithm Aa >
  INLINE auto const operator_sqrt ( qX_real::qx_real<T,Aa> const& a ) {
    using TX = qX_real::qx_real<T,Aa>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_sqrt_exception( a ); }
    catch ( TX const& e ) { return e; }
#else
    if ( qX_real::is_zero( a ) ) { return a; }
#endif
    if ( Aa != Algorithm::Quasi ) {
      return qX_real::operator_sqrt_body( a );
    } else {
      return qX_real::operator_sqrt_body( a.element_rotate() );
    }
  }
  template < typename T, Algorithm Aa >
  INLINE auto const operator_sqrt ( tX_real::tx_real<T,Aa> const& a ) {
    using TX = qX_real::qx_real<T,Aa>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_sqrt_exception( a ); }
    catch ( TX const& e ) { return e; }
#else
    if ( tX_real::is_zero( a ) ) { return TX{ a }; }
#endif
    if ( Aa != Algorithm::Quasi ) {
      return qX_real::operator_sqrt_body( a );
    } else {
      return qX_real::operator_sqrt_body( a.element_rotate() );
    }
  }
  template < typename T, Algorithm Aa >
  INLINE auto const operator_sqrt ( dX_real::dx_real<T,Aa> const& a ) {
    using TX = qX_real::qx_real<T,Aa>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_sqrt_exception( a ); }
    catch ( TX const& e ) { return e; }
#else
    if ( dX_real::is_zero( a ) ) { return TX{ a }; }
#endif
    if ( Aa != Algorithm::Quasi ) {
      return qX_real::operator_sqrt_body( a );
    } else {
      return qX_real::operator_sqrt_body( a.element_rotate() );
    }
  }
  template < typename T, Algorithm A=Algorithm::Accurate, IF_T_fp<T> >
  INLINE auto const operator_sqrt ( T const& a ) {
    using TX = qX_real::qx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_sqrt_exception<A>( a ); }
    catch ( TX const& e ) { return e; }
#else
    if ( fp<T>::is_zero( a ) ) { return TX{ a }; }
#endif
    return qX_real::operator_sqrt_body<T,A>( a );
  }
  //
  template < typename T, Algorithm Aa >
  INLINE auto const sqrt ( qX_real::qx_real<T,Aa> const& a ) {
    return qX_real::operator_sqrt( a );
  }
  template < typename T, Algorithm Aa >
  INLINE auto const sqrt ( tX_real::tx_real<T,Aa> const& a ) {
    return qX_real::operator_sqrt( a );
  }
  template < typename T, Algorithm Aa >
  INLINE auto const sqrt ( dX_real::dx_real<T,Aa> const& a ) {
    return qX_real::operator_sqrt( a );
  }
  template < typename T, Algorithm A=Algorithm::Accurate, IF_T_fp<T> >
  INLINE auto const sqrt ( T const& a ) {
    return qX_real::operator_sqrt<T,A>( a );
  }
  //
  template < typename T, Algorithm Aa >
  INLINE qX_real::qx_real<T,Aa> constexpr qX_real::qx_real<T,Aa>::sqrt ( qX_real::qx_real<T,Aa> const& a ) {
    return qX_real::sqrt( a );
  }


  //
  // f minimum
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_fmin_body ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
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
  INLINE auto const operator_fmin_exception ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const &b ) {
    using TX = qX_real::qx_real<T,A>;
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
  INLINE auto const operator_fmin ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const &b ) {
    using TX = qX_real::qx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_fmin_exception( a, b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( A == Algorithm::Quasi ) {
      using TT = qX_real::qx_real<T,Algorithm::Accurate>;
      return TX{ qX_real::operator_fmin_body( TT{ a }, TT{ b } ) };
    } else {
      return qX_real::operator_fmin_body( a, b );
    }
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const fmin ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const &b ) {
    return qX_real::operator_fmin( a, b );
  }


  //
  // f maximum
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_fmax_body ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
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
  INLINE auto const operator_fmax_exception ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const &b ) {
    using TX = qX_real::qx_real<T,A>;
    if ( isnan( a ) || isnan( b ) ) { throw TX::nan(); }
    if ( isinf( a ) && isinf( b ) ) {
      if ( is_negative( a ) && is_negative( b ) ) { throw a; }
      throw TX::inf();
    }
    if ( isinf( a ) ) { throw is_negative( a ) ? a : b; }
    if ( isinf( b ) ) { throw is_negative( b ) ? b : a; }
  }
#endif
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const operator_fmax ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    using TX = qX_real::qx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
    try { qX_real::operator_fmin_exception( a, b ); }
    catch ( TX const& e ) { return e; }
#endif
    if ( A == Algorithm::Quasi ) {
      using TT = qX_real::qx_real<T,Algorithm::Accurate>;
      return TX{ qX_real::operator_fmin_body( TT{ a }, TT{ b } ) };
    } else {
      return qX_real::operator_fmin_body( a, b );
    }
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  INLINE auto const fmax ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
    return qX_real::operator_fmax( a, b );
  }


  //
  // PRNG
  //
  template < typename T, Algorithm A >
  INLINE auto rand () -> if_T_double<T,qX_real::qx_real<T,A>> {
    using TX = qX_real::qx_real<T,A>;
    auto constexpr f = fp<T>::half / (1 << 30);
    auto g = f;
    auto r = TX::zero();
    auto constexpr bits = std::numeric_limits<qX_real::qx_real<T,A>>::digits;
    auto constexpr rand_bits = std::numeric_limits<int>::digits - 1;
    for(int i=0; i<bits; i+=rand_bits ) {
      auto b_ = fp<T>::rand();
      auto b = T (b_ );
      auto c = b * g;
           r = r + TX{ c };
           g = g * f;
    }
    return r;
  }
  template < typename T, Algorithm A >
  INLINE auto rand () -> if_T_float<T,qX_real::qx_real<T,A>> {
    using TX = qX_real::qx_real<T,A>;
    auto constexpr f = fp<T>::half / (1 << 30);
    auto g = f;
    auto r = TX::zero();
    auto constexpr bits = std::numeric_limits<qX_real::qx_real<T,A>>::digits;
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
  INLINE qX_real::qx_real<T,A> constexpr qX_real::qx_real<T,A>::rand () {
    return qX_real::rand<T,A>();
  }


  //
  //
  template < typename T, Algorithm Aa >
  INLINE std::ostream& operator<< ( std::ostream& stream, qX_real::qx_real<T,Aa> const& a ) {
    stream << std::to_string(a.x[0]) << " "
           << std::to_string(a.x[1]) << " "
           << std::to_string(a.x[2]) << " "
	   << std::to_string(a.x[3]);
    return stream;
  }

}
}


#endif

