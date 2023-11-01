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
    static int constexpr L = 4;
    T x[L];

    //
    inline qx_real() {
      x[0] = x[1] = x[2] = x[3] = fp<T>::zero;
    }
    inline qx_real( T const& x0, T const& x1, T const& x2, T const& x3 ) {
      x[0] = x0; x[1] = x1; x[2] = x2; x[3] = x3;
    }
    inline qx_real(int const &h) {
      x[0] = T(h); x[1] = x[2] = x[3] = fp<T>::zero;
    }
    inline qx_real(T const *d) {
      x[0] = d[0]; x[1] = d[1]; x[2] = d[2]; x[3] = d[3];
    }

    //
    //
    inline qx_real( T const &h ) {
      x[0] = h; x[1] = x[2] = x[3] = fp<T>::zero;
    }
    template < Algorithm _A_ >
    inline qx_real( dX_real::dx_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = x[3] = fp<T>::zero;
      } else {
        twoSum( h.x[0], h.x[1], x[0], x[1] );
        quickSum( x[0], x[1], x[0], x[1] );
       	x[2] = x[3] = fp<T>::zero;
      }
    }
    template < Algorithm _A_ >
    inline qx_real( tX_real::tx_real<T,_A_> const &h ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2]; x[3] = fp<T>::zero;
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
      } else {
	vecSum<-3>( x );
	VSEB_Sloppy<3,3>( x );
      }
    }
    template < Algorithm _A_ >
    inline qx_real( qx_real<T,_A_> const &h ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2]; x[3] = h.x[3];
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
      } else {
	vecSum<-4>( x );
	VSEB_Sloppy<4,4>( x );
      }
    }

    //
    //
    inline qx_real<T,A> &operator=( int const &h ) {
      x[0] = T(h); x[1] = x[2] = x[3] = fp<T>::zero;
      return *this;
    }
    inline qx_real<T,A> &operator=( T const &h ) {
      x[0] = h; x[1] = x[2] = x[3] = fp<T>::zero;
      return *this;
    }
    template < Algorithm _A_ >
    inline qx_real<T,A> &operator=( dX_real::dx_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = x[3] = fp<T>::zero;
      } else {
        twoSum( h.x[0], h.x[1], x[0], x[1] );
        quickSum( x[0], x[1], x[0], x[1] );
        x[2] = x[3] = fp<T>::zero;
      }
      return *this;
    }
    template < Algorithm _A_ >
    inline qx_real<T,A> &operator=( tX_real::tx_real<T,_A_> const &h ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2]; x[3] = fp<T>::zero;
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
      } else {
        vecSum<-3>( x );
        VSEB_Sloppy<3,3>( x );
      }
      return *this;
    }
    template < Algorithm _A_ >
    inline qx_real<T,A> &operator=( qx_real<T,_A_> const &h ) {
      if ( A == _A_ && this == (qx_real<T,A>*)(&h) ) { return *this; }
      x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2]; x[3] = h.x[3];
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
      } else {
        vecSum<-4>( x );
        VSEB_Sloppy<4,4>( x );
      }
      return *this;
    }

    //
    //
    inline operator T() const {
      if ( A == Algorithm::Quasi ) {
        return x[0] + x[1] + x[2] + x[3];
      } else {
        return x[0];
      }
    }
    template < Algorithm _A_ >
    inline operator dX_real::dx_real<T,_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[4] = { x[0], x[1], x[2], x[3] };
	vecSum<-4>( f );
	VSEB_Sloppy<2,4>( f );
	return dX_real::dx_real<T,_A_>( f );
      } else {
	return dX_real::dx_real<T,_A_>( x );
      }
    }
    template < Algorithm _A_ >
    inline operator tX_real::tx_real<T,_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[4] = { x[0], x[1], x[2], x[3] };
	vecSum<-4>( f );
	VSEB_Sloppy<3,4>( f );
	return tX_real::tx_real<T,_A_>( f );
      } else {
	return tX_real::tx_real<T,_A_>( x );
      }
    }
    template < Algorithm _A_ >
    inline operator qx_real<T,_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[4] = { x[0], x[1], x[2], x[3] };
	vecSum<-4>( f );
	VSEB_Sloppy<4,4>( f );
	return qx_real<T,_A_>( f );
      } else {
	return qx_real<T,_A_>( x );
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
    static inline qx_real<T,A> constexpr zero () { return qx_real<T,A>( fp<T>::zero ); }
    static inline qx_real<T,A> constexpr one  () { return qx_real<T,A>( fp<T>::one ); }
    static inline qx_real<T,A> constexpr two  () { return qx_real<T,A>( fp<T>::two ); }
    static inline qx_real<T,A> constexpr half () { return qx_real<T,A>( fp<T>::half ); }

    static inline qx_real<T,A> constexpr epsilon () {
	    T c = fp<T>::epsilon * fp<T>::half; c = (c * c); c = (c * c) * 2; return qx_real<T,A>( c ); }

    static inline qx_real<T,A> constexpr nan  () { T c = fp<T>::nan; return qx_real<T,A>( c,c,c,c ); }
    static inline qx_real<T,A> constexpr inf  () { T c = fp<T>::inf; return qx_real<T,A>( c,c,c,c ); }

    static inline qx_real<T,A> constexpr denorm_min  () {
      T c0 = std::numeric_limits<T>::denorm_min();
      return qx_real<T,A>( c0 );
    }
    static inline qx_real<T,A> constexpr min  () {
      T c0 = (std::numeric_limits<T>::min() * 8) * fp<T>::epsiloni * fp<T>::epsiloni * fp<T>::epsiloni;
      return qx_real<T,A>( c0 );
    }
    static inline qx_real<T,A> constexpr max  () {
      T c0 = std::numeric_limits<T>::max();
      T c1 = c0 * fp<T>::epsilon * fp<T>::half;
      T c2 = c1 * fp<T>::epsilon * fp<T>::half;
      T c3 = c2 * fp<T>::epsilon * fp<T>::half;
      return qx_real<T,A>( c0, c1, c2, c3 );
    }

    static inline qx_real<T,A> const rand ();

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
  // Comparation
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator== ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b )
  -> std::enable_if_t< A!=Algorithm::Quasi,bool > {
    return a.x[0] == b.x[0] && a.x[1] == b.x[1] && a.x[2] == b.x[2] && a.x[3] == b.x[3];
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator== ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b )
  -> std::enable_if_t< Aa==Algorithm::Quasi,bool > {
    return b == qx_real<T,Algorithm::Accurate>( a );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline bool operator!= ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    return !(a == b);
  }


  //
  // Unitary operations
  //
  template < typename T, Algorithm Aa >
  inline auto operator+ ( qx_real<T,Aa> const& a ) {
    using TX = qx_real<T,Aa>;
    TX r = a;
    return r;
  }
  template < typename T, Algorithm Aa >
  inline auto operator- ( qx_real<T,Aa> const& a ) {
    using TX = qx_real<T,Aa>;
    return TX( -a.x[0], -a.x[1], -a.x[2], -a.x[3] );
  }


  //
  // Addition
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_add ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Accurate, qx_real<T,A> > {
    using TX = qx_real<T,A>;
    T f[8];
    vecMerge<4,4>( f, a.x, b.x );
    vecSum<-8>( f );
    VSEB_Sloppy<4,8>( f );
    return TX( f );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_add ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b )
  -> std::enable_if_t< A!=Algorithm::Accurate, qx_real<T,A> > {
  //
  // Sloppy
  // Quasi by Ozaki
  //
    using TX = qx_real<T,A>; 
    T c[4], e[6];
    twoSum( a.x[0], b.x[0], c[0], e[0] );
    twoSum( a.x[1], b.x[1], c[1], e[1] );
    twoSum( c[1], e[0], c[1], e[2] );
    twoSum( a.x[2], b.x[2], c[2], e[3] );
    twoSum( c[2], e[1], c[2], e[4] );
    twoSum( c[2], e[2], c[2], e[5] );
    c[3] = a.x[3] + b.x[3] + e[3] + e[4] + e[5];
    if ( A != Algorithm::Quasi ) Normalize<4>( c[0], c[1], c[2], c[3] );
    return TX( c );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_add ( tX_real::tx_real<T,Aa> const& a, qx_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Accurate, qx_real<T,A> > {
    using TX = qx_real<T,A>; 
    T f[8];
    vecMerge<3,4>( f, a.x, b.x );
    vecSum<-7>( f );
    VSEB_Sloppy<4,7>( f );
    return TX( f );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_add ( tX_real::tx_real<T,Aa> const& a, qx_real<T,Ab> const& b )
  -> std::enable_if_t< A!=Algorithm::Accurate, qx_real<T,A> > {
    using TX = qx_real<T,A>; 
    T c[4], e[6];
    twoSum( a.x[0], b.x[0], c[0], e[0] );
    twoSum( a.x[1], b.x[1], c[1], e[1] );
    twoSum( c[1], e[0], c[1], e[2] );
    twoSum( a.x[2], b.x[2], c[2], e[3] );
    twoSum( c[2], e[1], c[2], e[4] );
    twoSum( c[2], e[2], c[2], e[5] );
    c[3] = b.x[3] + e[3] + e[4] + e[5];
    if ( A != Algorithm::Quasi ) Normalize<4>( c[0], c[1], c[2], c[3] );
    return TX( c );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_add ( dX_real::dx_real<T,Aa> const& a, qx_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Accurate, qx_real<T,A> > {
    using TX = qx_real<T,A>; 
    T f[8];
    vecMerge<2,4>( f, a.x, b.x );
    vecSum<-6>( f );
    VSEB_Sloppy<4,6>( f );
    return TX( f );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_add ( dX_real::dx_real<T,Aa> const& a, qx_real<T,Ab> const& b )
  -> std::enable_if_t< A!=Algorithm::Accurate, qx_real<T,A> > {
    using TX = qx_real<T,A>; 
    T c[4], e[6];
    twoSum( a.x[0], b.x[0], c[0], e[0] );
    twoSum( a.x[1], b.x[1], c[1], e[1] );
    twoSum( c[1], e[0], c[1], e[2] );
    twoSum( b.x[2], e[1], c[2], e[4] );
    twoSum( c[2], e[2], c[2], e[5] );
    c[3] = b.x[3] + e[4] + e[5];
    if ( A != Algorithm::Quasi ) Normalize<4>( c[0], c[1], c[2], c[3] );
    return TX( c );
  }
  template < typename T, Algorithm Ab >
  inline auto operator_add ( T const& a, qx_real<T,Ab> const& b ) 
  -> std::enable_if_t< Ab==Algorithm::Accurate, qx_real<T,Ab> > {
    using TX = qx_real<T,Ab>; 
    T f[8];
    vecMerge<1,4>( f, &a, b.x );
    vecSum<-5>( f );
    VSEB_Sloppy<4,5>( f );
    return TX( f );
  }
  template < typename T, Algorithm Ab >
  inline auto operator_add ( T const& a, qx_real<T,Ab> const& b ) 
  -> std::enable_if_t< Ab!=Algorithm::Accurate, qx_real<T,Ab> > {
    using TX = qx_real<T,Ab>; 
    T c[4], e[6];
    twoSum( a, b.x[0], c[0], e[0] );
    twoSum( e[0], b.x[1], c[1], e[1] );
    twoSum( e[1], b.x[2], c[2], e[2] );
    c[3] = b.x[3] + e[2];
    if ( Ab != Algorithm::Quasi ) Normalize<4>( c[0], c[1], c[2], c[3] );
    return TX( c );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_add ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b )
  -> qx_real<T,A> {
    using TX = qx_real<T,A>; 
    return operator_add( TX( a ), TX( b ) );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_add ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b )
  -> qx_real<T,A> {
    using TX = qx_real<T,A>; 
    return operator_add( TX( a ), TX( b ) );
  }
  template < typename T, Algorithm A >
  inline qx_real<T,A> operator_add ( T const& a, T const& b ) {
    using TX = qx_real<T,A>; 
    return operator_add( TX( a ), TX( b ) );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator+ ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    return operator_add( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator+ ( tX_real::tx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    return operator_add( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator+ ( qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    return b + a;
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator+ ( dX_real::dx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    return operator_add( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator+ ( qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return b + a;
  }
  template < typename Ts, typename T, Algorithm Ab >
  inline auto operator+ ( Ts const& a, qx_real<T,Ab> const& b )
  -> std::enable_if_t< std::is_arithmetic<Ts>::value, qx_real<T,Ab> > {
    return operator_add( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa >
  inline auto operator+ ( qx_real<T,Aa> const& a, Ts const& b ) {
    return b + a;
  }


  //
  // Substruction
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator- ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator- ( tX_real::tx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator- ( qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator- ( dX_real::dx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator- ( qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename Ts, typename T, Algorithm Ab >
  inline auto operator- ( Ts const& a, qx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename Ts, typename T, Algorithm Aa >
  inline auto operator- ( qx_real<T,Aa> const& a, Ts const& b ) {
    return a + (-b);
  }


  //
  // Multiplication
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_mul ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b )
  -> std::enable_if_t< A!=Algorithm::Quasi, qx_real<T,A> > {
  //
  // ported from qd_real::accurate_mul in the QD library
  //
    using TX = qx_real<T,A>;

    T p0, p1, p2, p3, p4;
    T q1[3];
    T q2[6];
    T q3[7];

    /* O(1) order term */
    twoProdFMA( a.x[0], b.x[0], p0, q1[0] ); // O(1,e)

    /* O(eps) order terms */
    twoProdFMA( a.x[0], b.x[1], q1[1], q2[0] ); // O(e,e^2)
    twoProdFMA( a.x[1], b.x[0], q1[2], q2[1] ); // O(e,e^2)

    /* accumulate O(e) */
    threeSum( q1[0], q1[1], q1[2] ); // O(e,e^2,e^3)
    p1 = q1[0]; q2[2] = q1[1]; q3[0] = q1[2];

    /* O(eps^2) order terms */
    twoProdFMA( a.x[0], b.x[2], q2[3], q3[1] ); // O(e^2,e^3)
    twoProdFMA( a.x[1], b.x[1], q2[4], q3[2] ); // O(e^2,e^3)
    twoProdFMA( a.x[2], b.x[0], q2[5], q3[3] ); // O(e^2,e^3)

    if ( A == Algorithm::Sloppy ) {

      p3 = q3[0] + q3[1] + q3[2] + q3[3];
    /* accumulate O(e^2) */
      twoSum( q2[0], q2[1], q2[0], q3[0] ); // O(e^2,e^3)
      twoSum( q2[2], q2[3], q2[1], q3[1] ); // O(e^2,e^3)
      twoSum( q2[4], q2[5], q2[2], q3[2] );

      twoSum( q2[0], q2[1], q2[0], q3[3] );
      twoSum( q2[0], q2[2], p2, q3[4] );

    /* O(eps^3) order terms */
      p3 = p3 + a.x[0]*b.x[3] + a.x[1]*b.x[2] + a.x[2]*b.x[1] + a.x[3]*b.x[0]
         + q3[0] + q3[1] + q3[2] + q3[3] + q3[4];
      Normalize<4>( p0, p1, p2, p3 );
      return TX( p0, p1, p2, p3 );

    } else {

      T q4[4];

    /* accumulate O(e^2) */
      threeSum( q2[0], q2[1], q2[2] ); // O(e^2,e^3,e^4)
      threeSum( q2[3], q2[4], q2[5] ); // O(e^2,e^3,e^4)
      twoSum( q2[0], q2[3], p2, q3[4] ); // O(e^2,e^3)
      q3[5] = q2[1]; q3[6] = q2[4];
      p4 = q2[2] + q2[5];

    /* O(e^3,e^4) */
      twoSum( q3[0], q3[1], q3[0], q4[0] );
      twoSum( q3[2], q3[3], q3[1], q4[1] );
      twoSum( q3[4], q3[5], q3[2], q4[2] );
      p4 = p4 + q4[0] + q4[1] + q4[2];
      twoSum( q3[6], q3[0], q3[0], q4[0] );
      twoSum( q3[1], q3[2], q3[1], q4[1] );
      p4 = p4 + q4[0] + q4[1];

    /* O(eps^3) order terms */
      twoProdFMA( a.x[0], b.x[3], q3[2], q4[0] );
      twoProdFMA( a.x[1], b.x[2], q3[3], q4[1] );
      twoProdFMA( a.x[2], b.x[1], q3[4], q4[2] );
      twoProdFMA( a.x[3], b.x[0], q3[5], q4[3] );
      p4 = p4 + q4[0] + q4[1] + q4[2] + q4[3];

      twoSum( q3[0], q3[1], q3[0], q4[0] );
      twoSum( q3[2], q3[3], q3[1], q4[1] );
      twoSum( q3[4], q3[5], q3[2], q4[2] );
      p4 = p4 + q4[0] + q4[1] + q4[2];

      twoSum( q3[0], q3[1], q3[0], q4[0] );
      twoSum( q3[0], q3[2], q3[0], q4[1] );
      p4 = p4 + q4[0] + q4[1];

      p3 = q3[0];

    /* O(eps^4) terms */
      p4 = std::fma( a.x[1], b.x[3],
           std::fma( a.x[2], b.x[2],
           std::fma( a.x[3], b.x[1], p4 ) ) );

      Normalize<4>( p0, p1, p2, p3, p4 );
      return TX( p0, p1, p2, p3 );

    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_mul ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, qx_real<T,A> > {
  //
  // Quasi by Ozaki
  //
    using TX = qx_real<T,A>;

    T c[4], e[14], t[6];
    
    twoProdFMA( a.x[0], b.x[0], c[0], e[0] );
    twoProdFMA( a.x[0], b.x[1], t[1], e[1] );
    twoProdFMA( a.x[1], b.x[0], t[2], e[2] );

    twoSum( t[1], t[2], c[1], e[3] );
    twoSum( c[1], e[0], c[1], e[4] );

    twoProdFMA( a.x[0], b.x[2], t[3], e[5] );
    twoProdFMA( a.x[1], b.x[1], t[4], e[6] );
    twoProdFMA( a.x[2], b.x[0], t[5], e[7] );

    twoSum( t[3], t[4], c[2], e[8] );
    twoSum( c[2], t[5], c[2], e[9] );
    twoSum( c[2], e[1], c[2], e[10] );
    twoSum( c[2], e[2], c[2], e[11] );
    twoSum( c[2], e[3], c[2], e[12] );
    twoSum( c[2], e[4], c[2], e[13] );

    c[3] = std::fma( a.x[3], b.x[0],
           std::fma( a.x[2], b.x[1],
           std::fma( a.x[1], b.x[2], a.x[0]*b.x[3] ) ) )
	    + e[5] + e[6] + e[7] + e[8] + e[9] + e[10] + e[11] + e[12] + e[13];

    return TX( c );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_mul ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, qx_real<T,A> > {
    return qx_real<T,Aa>( a ) * qx_real<T,Ab>( b );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator* ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    return operator_mul( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator* ( tX_real::tx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    using TX = qx_real<T,Aa>;
    return operator_mul( TX( a ), b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator* ( qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
    return b * a;
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator* ( dX_real::dx_real<T,Aa> const& a, qx_real<T,Ab> const& b ) {
    using TX = qx_real<T,Aa>;
    return operator_mul( TX( a ), b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator* ( qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return b * a;
  }
  template < typename Ts, typename T, Algorithm Ab >
  inline auto operator* ( Ts const& a, qx_real<T,Ab> const& b )
  -> std::enable_if_t< std::is_arithmetic<Ts>::value, qx_real<T,Ab> > {
    using TX = qx_real<T,Ab>;
    return operator_mul( TX( T(a) ), b );
  }
  template < typename Ts, typename T, Algorithm Aa >
  inline auto operator* ( qx_real<T,Aa> const& a, Ts const& b ) {
    return b * a;
  }


  //
  // Division
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_div ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b)
  -> std::enable_if_t< A!=Algorithm::Quasi, qx_real<T,A> > {
  //
  // Accurate in QD by Bailey and Hida
  //
    using TX = qx_real<T,A>;
    {
      if ( b.x[0] == fp<T>::inf ) { return TX::zero(); }
      if ( b.x[0] == -fp<T>::inf ) { return -TX::zero(); }
      if ( b.x[0] == fp<T>::zero ) { auto c = std::copysign( fp<T>::inf, b.x[0] ); return TX( c,c,c,c ); }
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
      Normalize<4>( q0, q1, q2, q3 );
    } else {
      auto q4 = r.x[0] / b.x[0];
      //
      Normalize<4>( q0, q1, q2, q3, q4 );
    }
    return TX( q0, q1, q2, q3 );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_div ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b)
  -> std::enable_if_t< A==Algorithm::Quasi, qx_real<T,A> > {
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
        s = s + b.x[3]; auto c = std::copysign( fp<T>::inf, s ); return TX( c,c,c,c ); }
      if ( b.x[0] == fp<T>::zero ) { return a / qx_real<T,A>( b.x[1], b.x[2], b.x[3], b.x[0] ); }
    }
    auto c = tX_real::tx_real<T,A>( a ) / tX_real::tx_real<T,A>( b );
    auto t = a - c * b;
    auto r = TX( c );
    r.x[3] = (t.x[0] + t.x[1] + t.x[2] + t.x[3]) / s;
    return r;
  }
  template < typename T, Algorithm Ab >
  inline qx_real<T,Ab> operator_div ( T const& a, qx_real<T,Ab> const& b ) {
    using TX = qx_real<T,Ab>;
    return (TX(a) / b);
  }
  template < typename T, Algorithm Aa >
  inline qx_real<T,Aa> operator_div ( qx_real<T,Aa> const& a, T const& b ) {
    using TX = qx_real<T,Aa>;
    return (a / TX(b));
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator/ ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b) {
    return operator_div( a, b );
  }
  template < typename Ts, typename T, Algorithm Ab >
  inline auto operator/ ( Ts const& a, qx_real<T,Ab> const& b)
  -> std::enable_if_t< std::is_arithmetic<Ts>::value, qx_real<T,Ab> > {
    return operator_div( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa >
  inline auto operator/ ( qx_real<T,Aa> const& a, Ts const& b)
  -> std::enable_if_t< std::is_arithmetic<Ts>::value, qx_real<T,Aa> > {
    return operator_div( a, T(b) );
  }


  //
  //
  template < typename T, Algorithm Aa >
  inline bool const isnan ( qx_real<T,Aa> const& a ) {
    auto s = a.x[0];
    if ( Aa == Algorithm::Quasi ) { s += a.x[1] + a.x[2] + a.x[3]; }
    return std::isnan( s );
  }
  template < typename T, Algorithm Aa >
  inline bool const signbit ( qx_real<T,Aa> const& a ) {
    auto s = a.x[0];
    if ( Aa == Algorithm::Quasi ) { s += a.x[1] + a.x[2] + a.x[3]; }
    return std::signbit( s );
  }


  //
  // Absolute value
  //
  template < typename T, Algorithm Aa >
  inline qx_real<T,Aa> const abs ( qx_real<T,Aa> const& a ) {
    using TX = qx_real<T,Aa>;
    if ( a.x[0] == fp<T>::inf ) { return TX::inf(); }
    auto s = a.x[0];
    if ( Aa == Algorithm::Quasi ) { s += a.x[1] + a.x[2] + a.x[3]; }
    if ( s >= fp<T>::zero ) {
      return a;
    } else {
      return -a;
    }
  }


  //
  // Square root
  //
  template < typename T, Algorithm Aa >
  inline auto const sqrt ( qx_real<T,Aa> const& a )
  -> std::enable_if_t< Aa!=Algorithm::Quasi, qx_real<T,Aa> > {
    using TX = qx_real<T,Aa>;
    {
      auto s = a.x[0];
      if ( s == fp<T>::zero ) { return a; }
      if ( s < fp<T>::zero ) { return TX::nan(); }
    }

    // scaling
    auto as = std::sqrt( a.x[0] );
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
  }
  template < typename T, Algorithm Aa >
  inline auto const sqrt ( qx_real<T,Aa> const& a )
  -> std::enable_if_t< Aa==Algorithm::Quasi, qx_real<T,Aa> > {
  //
  // Quasi by Ozaki
  //
    using TX = qx_real<T,Aa>;
    {
      auto s = a.x[0] + a.x[1] + a.x[2] + a.x[3];
      if ( s == fp<T>::zero ) { return a; }
      if ( s < fp<T>::zero ) { return TX::nan(); }
      if ( a.x[0] == fp<T>::zero ) { return sqrt( qx_real<T,Aa>( a.x[1], a.x[2], a.x[3], a.x[0] ) ); };
    }
    auto r = sqrt( tX_real::tx_real<T,Aa>( a ) );
    auto t = a - qX_real::operator_mul( r, r );
    auto c = TX( r );
    c.x[3] = (t.x[0] + t.x[1] + t.x[2] + t.x[3]) / (2*(c.x[0] + c.x[1] + c.x[2]));
    return c;
  }
  template < typename T, Algorithm Aa >
  inline qx_real<T,Aa> const sqrt ( tX_real::tx_real<T,Aa> const& a ) {
    return sqrt( qx_real<T,Aa>( a ) );
  }
  template < typename T, Algorithm Aa >
  inline qx_real<T,Aa> const sqrt ( dX_real::dx_real<T,Aa> const& a ) {
    return sqrt( qx_real<T,Aa>( a ) );
  }
  template < typename T, Algorithm Aa=Algorithm::Accurate >
  inline qx_real<T,Aa> const sqrt ( T const& a ) {
    return sqrt( qx_real<T,Aa>( a ) );
  }


  //
  // f minimum
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto const fmin ( qx_real<T,Aa> const& a, qx_real<T,Ab> const &b )
  -> std::enable_if_t< A!=Algorithm::Quasi, qx_real<T,A> > {
    using TX = qx_real<T,A>;
    int i=0; for(i;i<4-1;i++) {
      if ( a.x[i] != b.x[i] ) break;
    }
    if ( a.x[i] <= b.x[i] ) {
      return TX( a );
    } else {
      return TX( b );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto const fmin ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, qx_real<T,A> > {
  //
  // Quasi
  //
    using TX = qx_real<T,Algorithm::Accurate>;
    return qx_real<T,A>( fmin( TX(a), TX(b) ) );
  }


  //
  // f maximum
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto const fmax ( qx_real<T,Aa> const& a, qx_real<T,Ab> const &b )
  -> std::enable_if_t< A!=Algorithm::Quasi, qx_real<T,A> > {
    using TX = qx_real<T,A>;
    int i=0; for(i;i<4-1;i++) {
      if ( a.x[i] != b.x[i] ) break;
    }
    if ( a.x[i] >= b.x[i] ) {
      return TX( a );
    } else {
      return TX( b );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto const max ( qx_real<T,Aa> const& a, qx_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, qx_real<T,A> > {
  //
  // Quasi
  //
    using TX = qx_real<T,Algorithm::Accurate>;
    return qx_real<T,A>( fmax( TX(a), TX(b) ) );
  }


  //
  // PRNG
  //
  template < typename T, Algorithm A >
  inline qx_real<T,A> const qx_real<T,A>::rand () {
    using TX = qx_real<T,A>;
    auto constexpr f = fp<T>::one / (1<<16) / (1<<15);
    auto g = f;
    auto r = TX::zero();
    if ( std::is_same< T, double >::value ) {
      auto bits = TX::L * 53;
      for(int i=0; i<bits; i+=31 ) {
        auto b_ = std::rand();
        auto b = T( b_ );
        auto c = b * g;
             r = r + TX( c );
             g = g * f;
      }
    }
    if ( std::is_same< T, float >::value ) {
      auto bits = TX::L * 24;
      for(int i=0; i<bits; i+=31 ) {
        auto b_ = std::rand();
        auto b = T( b_ & 0x7fff0000 );
        auto c = b * g;
             r = r + TX( c );
             b = T( b_ & 0x0000ffff );
             c = b * g;
             r = r + TX( c );
             g = g * f;
      }

    }
    return r;
  }


  //
  //
  template < typename T, Algorithm Aa >
  inline std::ostream& operator<< ( std::ostream& stream, qx_real<T,Aa> const& a ) {
    stream << std::to_string(a.x[0]) << " "
           << std::to_string(a.x[1]) << " "
           << std::to_string(a.x[2]) << " "
	   << std::to_string(a.x[3]);
    return stream;
  }

}
}


#endif

