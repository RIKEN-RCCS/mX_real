#ifndef	QX_REAL_H
#define	QX_REAL_H

#include "mX_real.hpp"


namespace mX_real {

  template < typename T, Algorithm A=Algorithm::Accurate >
  struct qX_real {

    static_assert( fp<T>::value,
                   "The base type must be out of the defined types, such as float or double." );

    //
    static bool constexpr __is_mX_real__ = true;
    //
    static int constexpr L = 4;
    using base   = qX_real<T,Algorithm::Accurate>;
    //
    using this_T = qX_real<T,A>;
    using base_T = T;
    static Algorithm constexpr base_A = A;
    //
    T x[L];

    //
    inline qX_real() {
      x[0] = x[1] = x[2] = x[3] = fp<T>::zero;
    }
    inline qX_real( T const& x0, T const& x1, T const& x2, T const& x3 ) {
      x[0] = x0; x[1] = x1; x[2] = x2; x[3] = x3;
    }
    inline qX_real(int const &h) {
      x[0] = T(h); x[1] = x[2] = x[3] = fp<T>::zero;
    }
    inline qX_real(T const *d) {
      x[0] = d[0]; x[1] = d[1]; x[2] = d[2]; x[3] = d[3];
    }

    //
    //
    inline qX_real( T const &h ) {
      x[0] = h; x[1] = x[2] = x[3] = fp<T>::zero;
    }
    template < Algorithm _A_ >
    inline qX_real( dX_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = x[3] = fp<T>::zero;
      } else {
        twoSum( h.x[0], h.x[1], x[0], x[1] );
        quickSum( x[0], x[1], x[0], x[1] );
       	x[2] = x[3] = fp<T>::zero;
      }
    }
    template < Algorithm _A_ >
    inline qX_real( tX_real<T,_A_> const &h ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2]; x[3] = fp<T>::zero;
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
      } else {
	vecSum<3>( x );
	VSEB<3,3>( x );
      }
    }
    template < Algorithm _A_ >
    inline qX_real( qX_real<T,_A_> const &h ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2]; x[3] = h.x[3];
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
      } else {
	vecSum<4>( x );
	VSEB<4,4>( x );
      }
    }

    //
    //
    inline qX_real<T,A> &operator=( int const &h ) {
      x[0] = T(h); x[1] = x[2] = x[3] = fp<T>::zero;
      return *this;
    }
    inline qX_real<T,A> &operator=( T const &h ) {
      x[0] = h; x[1] = x[2] = x[3] = fp<T>::zero;
      return *this;
    }
    template < Algorithm _A_ >
    inline qX_real<T,A> &operator=( dX_real<T,_A_> const &h ) {
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
    inline qX_real<T,A> &operator=( tX_real<T,_A_> const &h ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2]; x[3] = fp<T>::zero;
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
      } else {
        vecSum<3>( x );
        VSEB<3,3>( x );
      }
      return *this;
    }
    template < Algorithm _A_ >
    inline qX_real<T,A> &operator=( qX_real<T,_A_> const &h ) {
      if ( A == _A_ && this == (qX_real<T,A>*)(&h) ) { return *this; }
      x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2]; x[3] = h.x[3];
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
      } else {
        vecSum<4>( x );
        VSEB<4,4>( x );
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
    inline operator dX_real<T,_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[4] = { x[0], x[1], x[2], x[3] };
	vecSum<4>( f );
	VSEB<2,4>( f );
	return dX_real<T,_A_>( f );
      } else {
	return dX_real<T,_A_>( x );
      }
    }
    template < Algorithm _A_ >
    inline operator tX_real<T,_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[4] = { x[0], x[1], x[2], x[3] };
	vecSum<4>( f );
	VSEB<3,4>( f );
	return tX_real<T,_A_>( f );
      } else {
	return tX_real<T,_A_>( x );
      }
    }
    template < Algorithm _A_ >
    inline operator qX_real<T,_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[4] = { x[0], x[1], x[2], x[3] };
	vecSum<4>( f );
	VSEB<4,4>( f );
	return qX_real<T,_A_>( f );
      } else {
	return qX_real<T,_A_>( x );
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
    static inline qX_real<T,A> constexpr zero () { return qX_real<T,A>( fp<T>::zero ); }
    static inline qX_real<T,A> constexpr one  () { return qX_real<T,A>( fp<T>::one ); }
    static inline qX_real<T,A> constexpr two  () { return qX_real<T,A>( fp<T>::two ); }
    static inline qX_real<T,A> constexpr half () { return qX_real<T,A>( fp<T>::half ); }

    static inline qX_real<T,A> constexpr epsilon () {
	    T c = fp<T>::epsilon * fp<T>::half; c = (c * c); c = (c * c) * 2; return qX_real<T,A>( c ); }

    static inline qX_real<T,A> constexpr nan  () { T c = fp<T>::nan; return qX_real<T,A>( c,c,c,c ); }
    static inline qX_real<T,A> constexpr inf  () { T c = fp<T>::inf; return qX_real<T,A>( c,c,c,c ); }

    static inline qX_real<T,A> constexpr denorm_min  () {
      T c0 = std::numeric_limits<T>::denorm_min();
      return qX_real<T,A>( c0 );
    }
    static inline qX_real<T,A> constexpr min  () {
      T c0 = (std::numeric_limits<T>::min() * 8) * fp<T>::epsiloni * fp<T>::epsiloni * fp<T>::epsiloni;
      return qX_real<T,A>( c0 );
    }
    static inline qX_real<T,A> constexpr max  () {
      T c0 = std::numeric_limits<T>::max();
      T c1 = c0 * fp<T>::epsilon * fp<T>::half;
      T c2 = c1 * fp<T>::epsilon * fp<T>::half;
      T c3 = c2 * fp<T>::epsilon * fp<T>::half;
      return qX_real<T,A>( c0, c1, c2, c3 );
    }

  };

  // alias names
  template < typename T >
  using qX_real_accurate = qX_real<T,Algorithm::Accurate>;
  template < typename T >
  using qX_real_sloppy   = qX_real<T,Algorithm::Sloppy>;
  template < typename T >
  using qX_real_quasi   = qX_real<T,Algorithm::Quasi>;


  //
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator== ( qX_real<T,Aa> const& a, qX_real<T,Ab> const& b )
  -> std::enable_if_t< A!=Algorithm::Quasi,bool > {
    return a.x[0] == b.x[0] && a.x[1] == b.x[1] && a.x[2] == b.x[2] && a.x[3] == b.x[3];
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator== ( qX_real<T,Aa> const& a, qX_real<T,Ab> const& b )
  -> std::enable_if_t< Aa==Algorithm::Quasi,bool > {
    return (b == qX_real<T,Algorithm::Accurate>( a ) );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline bool operator!= ( qX_real<T,Aa> const& a, qX_real<T,Ab> const& b ) {
    return !(a == b);
  }


  //
  //
  template < typename T, Algorithm Aa >
  inline qX_real<T,Aa> operator+ ( qX_real<T,Aa> const& a ) {
    using TX = qX_real<T,Aa>;
    TX r = a;
    return r;
  }
  template < typename T, Algorithm Aa >
  inline qX_real<T,Aa> operator- ( qX_real<T,Aa> const& a ) {
    using TX = qX_real<T,Aa>;
    return TX( -a.x[0], -a.x[1], -a.x[2], -a.x[3] );
  }


  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator+ ( qX_real<T,Aa> const& a, qX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Sloppy, qX_real<T,A> > {
  //
  // Sloppy
  //
    using TX = qX_real<T,A>;
    T s[4], t[4];
#if 1
    // lower-accuracy then the QD-library
    { T v[4];
      for(int i=0;i<3;i++) s[i] = a.x[i] + b.x[i];
      for(int i=0;i<3;i++) v[i] = s[i]   - a.x[i];
      for(int i=0;i<3;i++) t[i] = s[i]   - v[i];
      for(int i=0;i<3;i++) t[i] = a.x[i] - t[i];
      for(int i=0;i<3;i++) v[i] = b.x[i] - v[i];
      for(int i=0;i<3;i++) t[i] = t[i]   + v[i];
      //  s0 t0
      //     s1 t1
      //        s2 t2
    }
    twoSum( s[1], t[0], s[1], t[0] );
      //  s0    t0
      //     s1 t1
      //        s2 t2
    twoSum( s[2], t[0], s[2], t[0] );
    twoSum( s[2], t[1], s[2], t[1] );
      //  s0       t0
      //     s1    t1
      //        s2 t2
    s[3] = (a.x[3] + b.x[3]) + t[0] + t[1] + t[2];

    Normalize<4>( s[0], s[1], s[2], s[3] );
#else
    // compatible to the QD-library
    { T v[4];
      for(int i=0;i<4;i++) s[i] = a.x[i] + b.x[i];
      for(int i=0;i<4;i++) v[i] = s[i]   - a.x[i];
      for(int i=0;i<4;i++) t[i] = s[i]   - v[i];
      for(int i=0;i<4;i++) t[i] = a.x[i] - t[i];
      for(int i=0;i<4;i++) v[i] = b.x[i] - v[i];
      for(int i=0;i<4;i++) t[i] = t[i]   + v[i];
    }
    twoSum( s[1], t[0], s[1], t[0] );
    threeSum( s[2], t[0], t[1] );
    threeSum2( s[3], t[0], t[2] );
    t[0] = t[0] + t[1] + t[3];
    Normalize<4>( s[0], s[1], s[2], s[3], t[0] );
#endif
    return TX( s );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator+ ( qX_real<T,Aa> const& a, qX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Accurate, qX_real<T,A> > {
#if 1
    using TX = qX_real<T,A>;

    // vmerge
    T f[8];
    vecMerge<4,4>( f, a.x, b.x );

    // vecsum 7 twoSum
    vecSum<8>( f );

    // VSEB at least 4 quickSum
    VSEB<4,8>( f );
    return TX( f );
#else
  //
  // Possibly more Accurate
  //
    using TX = qX_real<T,A>;
    T s[4], t[4];
    { T v[4];
      for(int i=0;i<4;i++) s[i] = a.x[i] + b.x[i];
      for(int i=0;i<4;i++) v[i] = s[i]   - a.x[i];
      for(int i=0;i<4;i++) t[i] = s[i]   - v[i];
      for(int i=0;i<4;i++) t[i] = a.x[i] - t[i];
      for(int i=0;i<4;i++) v[i] = b.x[i] - v[i];
      for(int i=0;i<4;i++) t[i] = t[i]   + v[i];
      //  s0 t0
      //     s1 t1
      //        s2 t2
      //           s3 t3
    }
    twoSum( s[1], t[0], s[1], t[0] );
      //  s0    t0
      //     s1 t1
      //        s2 t2
      //           s3 t3
    threeSum( s[2], t[0], t[1] );
      //  s0
      //     s1    t0 t1
      //        s2 t2
      //           s3 t3
    threeSum2( s[3], t[0], t[2] );
      //  s0
      //     s1       t1
      //        s2    t0
      //           s3 t3
    t[0] = t[0] + t[1] + t[3];

    Normalize<4>( s[0], s[1], s[2], s[3], t[0] );
    return TX( s );
#endif
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator+ ( qX_real<T,Aa> const& a, qX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, qX_real<T,A> > {
  //
  // Quasi by Ozaki
  //
    using TX = qX_real<T,A>; 
    T c[3], e[6];
    twoSum( a.x[0], b.x[0], c[0], e[0] );
    twoSum( a.x[1], b.x[1], c[1], e[1] );
    twoSum( c[1], e[0], c[1], e[2] );
    twoSum( a.x[2], b.x[2], c[2], e[3] );
    twoSum( c[2], e[1], c[2], e[4] );
    twoSum( c[2], e[2], c[2], e[5] );
    c[3] = a.x[3] + b.x[3] + e[3] + e[4] + e[5];
    return TX( c );
  }


  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline qX_real<T,A> operator- ( qX_real<T,Aa> const& a, qX_real<T,Ab> const& b ) {
    return (a + (-b));
  }
  template < typename T, Algorithm Ab >
  inline qX_real<T,Ab> operator- ( T const& a, qX_real<T,Ab> const& b ) {
    return (a + (-b));
  }
  template < typename T, Algorithm Aa >
  inline qX_real<T,Aa> operator- ( qX_real<T,Aa> const& a, T const& b ) {
    return (a + (-b));
  }


  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator* ( qX_real<T,Aa> const& a, qX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Sloppy, qX_real<T,A> > {
  //
  // ported from qd_real::sloppy_mul in the QD library
  //
    using TX = qX_real<T,A>;

    T p0, p1, p2, p3, p4, p5;
    T q0, q1, q2, q3, q4, q5;
    T t0, t1;
    T s0, s1, s2;

    twoProdFMA( a.x[0], b.x[0], p0, q0 );

    twoProdFMA( a.x[0], b.x[1], p1, q1 );
    twoProdFMA( a.x[1], b.x[0], p2, q2 );

    twoProdFMA( a.x[0], b.x[2], p3, q3 );
    twoProdFMA( a.x[1], b.x[1], p4, q4 );
    twoProdFMA( a.x[2], b.x[0], p5, q5 );

    /* Start Accumulation */
    threeSum( p1, p2, q0 );

    /* Six-Three Sum  of p2, q1, q2, p3, p4, p5. */
    threeSum( p2, q1, q2 );
    threeSum( p3, p4, p5 );
    /* compute (s0, s1, s2) = (p2, q1, q2) + (p3, p4, p5). */
    twoSum( p2, p3, s0, t0 );
    twoSum( q1, p4, s1, t1 );
    s2 = q2 + p5;
    twoSum( s1, t0, s1, t0 );
    s2 += (t0 + t1);

    /* O(eps^3) order terms */
    s1 += a.x[0]*b.x[3] + a.x[1]*b.x[2] + a.x[2]*b.x[1] + a.x[3]*b.x[0] + q0 + q3 + q4 + q5;
    Normalize<4>( p0, p1, s0, s1, s2 );
    return TX( p0, p1, s0, s1 );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator* ( qX_real<T,Aa> const& a, qX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Accurate, qX_real<T,A> > {
  //
  // ported from qd_real::accurate_mul in the QD library
  //
    using TX = qX_real<T,A>;

    T p0, p1, p2, p3, p4, p5;
    T q0, q1, q2, q3, q4, q5;
    T p6, p7, p8, p9;
    T q6, q7, q8, q9;
    T r0, r1;
    T t0, t1;
    T s0, s1, s2;

    twoProdFMA( a.x[0], b.x[0], p0, q0 );

    twoProdFMA( a.x[0], b.x[1], p1, q1 );
    twoProdFMA( a.x[1], b.x[0], p2, q2 );

    twoProdFMA( a.x[0], b.x[2], p3, q3 );
    twoProdFMA( a.x[1], b.x[1], p4, q4 );
    twoProdFMA( a.x[2], b.x[0], p5, q5 );

    /* Start Accumulation */
    threeSum( p1, p2, q0 );

    /* Six-Three Sum  of p2, q1, q2, p3, p4, p5. */
    threeSum( p2, q1, q2 );
    threeSum( p3, p4, p5 );
    /* compute (s0, s1, s2) = (p2, q1, q2) + (p3, p4, p5). */
    twoSum( p2, p3, s0, t0 );
    twoSum( q1, p4, s1, t1 );
    s2 = q2 + p5;
    twoSum( s1, t0, s1, t0 );
    s2 += (t0 + t1);

    /* O(eps^3) order terms */
    twoProdFMA( a.x[0], b.x[3], p6, q6 );
    twoProdFMA( a.x[1], b.x[2], p7, q7 );
    twoProdFMA( a.x[2], b.x[1], p8, q8 );
    twoProdFMA( a.x[3], b.x[0], p9, q9 );

    /* Nine-Two-Sum of q0, s1, q3, q4, q5, p6, p7, p8, p9. */
    twoSum( q0, q3, q0, q3 );
    twoSum( q4, q5, q4, q5 );
    twoSum( p6, p7, p6, p7 );
    twoSum( p8, p9, p8, p9 );
    /* Compute (t0, t1) = (q0, q3) + (q4, q5). */
    twoSum( q0, q4, t0, t1 );
    t1 += (q3 + q5);
    /* Compute (r0, r1) = (p6, p7) + (p8, p9). */
    twoSum( p6, p8, r0, r1 );
    r1 += (p7 + p9);
    /* Compute (q3, q4) = (t0, t1) + (r0, r1). */
    twoSum( t0, r0, q3, q4 );
    q4 += (t1 + r1);
    /* Compute (t0, t1) = (q3, q4) + s1. */
    twoSum( q3, s1, t0, t1 );
    t1 += q4;

    /* O(eps^4) terms -- Nine-One-Sum */
    t1 = std::fma( a.x[1], b.x[3],
         std::fma( a.x[2], b.x[2],
         std::fma( a.x[3], b.x[1], t1 ) ) )
          + q6 + q7 + q8 + q9 + s2;

    Normalize<4>( p0, p1, s0, t0, t1 );
    return TX( p0, p1, s0, t0 );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator* ( qX_real<T,Aa> const& a, qX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, qX_real<T,A> > {
  //
  // Quasi by Ozaki
  //
    using TX = qX_real<T,A>;

    T c[4], e[14], t[6];
    
    twoProdFMA( a.x[0], b.x[0], c[0], e[0] );
    twoProdFMA( a.x[0], b.x[1], t[1], e[1] );
    twoProdFMA( a.x[1], b.x[0], t[2], e[2] );

    twoSum( t[1],t[2],c[1], e[3] );
    twoSum( c[1],e[0],c[1], e[4] );

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
  inline qX_real<T,A> operator* ( tX_real<T,Aa> const& a, qX_real<T,Ab> const& b ) {
    using TX = qX_real<T,Aa>;
    return TX( a ) * b;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline qX_real<T,A> operator* ( qX_real<T,Aa> const& a, tX_real<T,Ab> const& b ) {
    return b * a;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline qX_real<T,A> operator* ( dX_real<T,Aa> const& a, qX_real<T,Ab> const& b ) {
    using TX = qX_real<T,Aa>;
    return TX( a ) * b;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline qX_real<T,A> operator* ( qX_real<T,Aa> const& a, dX_real<T,Ab> const& b ) {
    return b * a;
  }
  template < typename T, Algorithm Ab >
  inline qX_real<T,Ab> operator* ( T const& a, qX_real<T,Ab> const& b ) {
    using TX = qX_real<T,Ab>;
    return (TX(a) * b);
  }
  template < typename T, Algorithm Aa >
  inline qX_real<T,Aa> operator* ( qX_real<T,Aa> const& a, T const& b ) {
    return (b * a);
  }
  template < typename T, Algorithm Ab >
  inline qX_real<T,Ab> operator* ( int const& a, qX_real<T,Ab> const& b ) {
    return (T(a) * b);
  }
  template < typename T, Algorithm Aa >
  inline qX_real<T,Aa> operator* ( qX_real<T,Aa> const& a, int const& b ) {
    return (b * a);
  }


  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator/ ( qX_real<T,Aa> const& a, qX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Sloppy, qX_real<T,A> > {
  //
  // Sloppy in QD by Bailey and Hida
  //
    using TX = qX_real<T,A>;
    if ( b.x[0] == fp<T>::inf ) { return TX::zero(); }
    if ( b.x[0] == -fp<T>::inf ) { return -TX::zero(); }
    if ( b.x[0] == fp<T>::zero ) { auto c = std::copysign( fp<T>::inf, b.x[0] ); return TX( c,c,c,c ); }
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
      //
    Normalize<4>( q0, q1, q2, q3 );
    return TX( q0, q1, q2, q3 );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator/ ( qX_real<T,Aa> const& a, qX_real<T,Ab> const& b)
  -> std::enable_if_t< A==Algorithm::Accurate, qX_real<T,A> > {
  //
  // Accurate in QD by Bailey and Hida
  //
    using TX = qX_real<T,A>;
    if ( b.x[0] == fp<T>::inf ) { return TX::zero(); }
    if ( b.x[0] == -fp<T>::inf ) { return -TX::zero(); }
    if ( b.x[0] == fp<T>::zero ) { auto c = std::copysign( fp<T>::inf, b.x[0] ); return TX( c,c,c,c ); }
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
    auto q4 = r.x[0] / b.x[0];
      //
    Normalize<4>( q0, q1, q2, q3, q4 );
    return TX( q0, q1, q2, q3 );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator/ ( qX_real<T,Aa> const& a, qX_real<T,Ab> const& b)
  -> std::enable_if_t< A==Algorithm::Quasi, qX_real<T,A> > {
  //
  // Quasi by Ozaki
  //
    using TX = qX_real<T,A>;
    if ( b.x[0] == fp<T>::inf ) { return TX::zero(); }
    if ( b.x[0] == -fp<T>::inf ) { return -TX::zero(); }
    auto s = b.x[0] + b.x[1] + b.x[2];
    if ( s + b.x[3] == fp<T>::zero ) {
      s = s + b.x[3]; auto c = std::copysign( fp<T>::inf, s ); return TX( c,c,c,c ); }
    if ( b.x[0] == fp<T>::zero ) { return a / qX_real<T,A>( b.x[1], b.x[2], b.x[3], b.x[0] ); }
    auto c = qX_real<T,A>( tX_real<T,A>( a ) / tX_real<T,A>( b ) );
    auto t = a - c * b;
    c.x[3] = (t.x[0] + t.x[1] + t.x[2] + t.x[3]) / s;
    return c;
  }
  template < typename T, Algorithm Ab >
  inline qX_real<T,Ab> operator/ ( T const& a, qX_real<T,Ab> const& b ) {
    using TX = qX_real<T,Ab>;
    return (TX(a) / b);
  }
  template < typename T, Algorithm Aa >
  inline qX_real<T,Aa> operator/ ( qX_real<T,Aa> const& a, T const& b ) {
    using TX = qX_real<T,Aa>;
    return (a / TX(b));
  }


  //
  //
  template < typename T, Algorithm Aa >
  inline bool isnan ( qX_real<T,Aa> const& a ) {
    auto s = a.x[0];
    if ( Aa == Algorithm::Quasi ) { s += a.x[1] + a.x[2] + a.x[3]; }
    return std::isnan( s );
  }
  template < typename T, Algorithm Aa >
  inline bool signbit ( qX_real<T,Aa> const& a ) {
    auto s = a.x[0];
    if ( Aa == Algorithm::Quasi ) { s += a.x[1] + a.x[2] + a.x[3]; }
    return std::signbit( s );
  }


  //
  //
  template < typename T, Algorithm Aa >
  inline qX_real<T,Aa> abs ( qX_real<T,Aa> const& a ) {
    using TX = qX_real<T,Aa>;
    if ( a.x[0] == fp<T>::inf ) { return TX::inf(); }
    auto s = a.x[0];
    if ( Aa == Algorithm::Quasi ) { s += a.x[1] + a.x[2] + a.x[3]; }
    if ( s >= fp<T>::zero ) {
      return a;
    } else {
      return -a;
    }
  }


  template < typename T, Algorithm Aa >
  inline auto sqrt ( qX_real<T,Aa> const& a )
  -> std::enable_if_t< Aa!=Algorithm::Quasi, qX_real<T,Aa> > {
    using TX = qX_real<T,Aa>;
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
  inline auto sqrt ( qX_real<T,Aa> const& a )
  -> std::enable_if_t< Aa==Algorithm::Quasi, qX_real<T,Aa> > {
  //
  // Quasi by Ozaki
  //
    using TX = qX_real<T,Aa>;
    auto s = a.x[0] + a.x[1] + a.x[2] + a.x[3];
    if ( s == fp<T>::zero ) { return a; }
    if ( s < fp<T>::zero ) { return TX::nan(); }
    if ( a.x[0] == fp<T>::zero ) { return sqrt( qX_real<T,Aa>( a.x[1], a.x[2], a.x[3], a.x[0] ) ); };

    auto c = TX( sqrt( tX_real<T,Aa>( a ) ) );
    auto t = a - c * c;
    c.x[3] = (t.x[0] + t.x[1] + t.x[2] + t.x[3]) / (2*(c.x[0] + c.x[1] + c.x[2]));
    return c;
  }


  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto fmin ( qX_real<T,Aa> const& a, qX_real<T,Ab> const &b )
  -> std::enable_if_t< A!=Algorithm::Quasi, qX_real<T,A> > {
    using TX = qX_real<T,A>;
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
  inline auto fmin ( qX_real<T,Aa> const& a, qX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, qX_real<T,A> > {
  //
  // Quasi
  //
    using TX = qX_real<T,Algorithm::Accurate>;
    return qX_real<T,A>( fmin( TX(a), TX(b) ) );
  }


  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto fmax ( qX_real<T,Aa> const& a, qX_real<T,Ab> const &b )
  -> std::enable_if_t< A!=Algorithm::Quasi, qX_real<T,A> > {
    using TX = qX_real<T,A>;
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
  inline auto max ( qX_real<T,Aa> const& a, qX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, qX_real<T,A> > {
  //
  // Quasi
  //
    using TX = qX_real<T,Algorithm::Accurate>;
    return qX_real<T,A>( fmax( TX(a), TX(b) ) );
  }


  //
  //
  template < typename T, Algorithm Aa >
  inline std::ostream& operator<< ( std::ostream& stream, qX_real<T,Aa> const& a ) {
    stream << std::to_string(a.x[0]) << " "
           << std::to_string(a.x[1]) << " "
           << std::to_string(a.x[2]) << " "
	   << std::to_string(a.x[3]);
    return stream;
  }

}


#endif

