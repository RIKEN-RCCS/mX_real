#ifndef	TX_REAL_H
#define	TX_REAL_H

#include "mX_real.hpp"


namespace mX_real {

  template < typename T, Algorithm A=Algorithm::Accurate >
  struct tX_real {

    static_assert( fp<T>::value,
                   "The base type must be out of the defined types, such as float or double." );

    //
    static bool constexpr __is_mX_real__ = true;
    //
    static int constexpr L = 3;
    using base   = tX_real<T,Algorithm::Accurate>;
    //
    using this_T = tX_real<T,A>;
    using base_T = T;
    static Algorithm constexpr base_A = A;
    //
    T x[L];

    //
    //
    inline tX_real() {
      x[0] = x[1] = x[2] = fp<T>::zero;
    }
    inline tX_real( T const& x0, T const& x1, T const& x2 ) {
      x[0] = x0; x[1] = x1; x[2] = x2;
    }
    inline tX_real( int const &h ) {
      x[0] = T(h); x[1] = x[2] = fp<T>::zero;
    }
    inline tX_real( T const *d ) {
      x[0] = d[0]; x[1] = d[1]; x[2] = d[2];
    }

    //
    //
    inline tX_real( T const &h ) {
      x[0] = h; x[1] = x[2] = fp<T>::zero;
    }
    template < Algorithm _A_ >
    inline tX_real( dX_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = fp<T>::zero;
      } else {
        twoSum( h.x[0], h.x[1], x[0], x[1] );
        quickSum( x[0], x[1], x[0], x[1] );
	x[2] = fp<T>::zero;
      }
    }
    template < Algorithm _A_ >
    inline tX_real( tX_real<T,_A_> const &h ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2];
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
      } else {
        vecSum<3>( x );
        VSEB<3,3>( x );
      }
    }
    template < Algorithm _A_ >
    inline tX_real( qX_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2];
      } else {
        T f[4] = { h.x[0], h.x[1], h.x[2], h.x[3] };
        vecSum<4>( f );
        VSEB<3,4>( f );
	x[0] = f[0]; x[1] = f[1]; x[2] = f[2];
      }
    }

    //
    //
    inline tX_real<T,A> &operator=( int const &h ) {
      x[0] = T(h); x[1] = x[2] = fp<T>::zero;
      return *this;
    }
    inline tX_real<T,A> &operator=( T const &h ) {
      x[0] = h; x[1] = x[2] = fp<T>::zero;
      return *this;
    }
    template < Algorithm _A_ >
    inline tX_real<T,A> &operator=( dX_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = fp<T>::zero;
      } else {
        twoSum( h.x[0], h.x[1], x[0], x[1] );
        quickSum( x[0], x[1], x[0], x[1] );
       	x[2] = fp<T>::zero;
      }
      return *this;
    }
    template < Algorithm _A_ >
    inline tX_real<T,A> &operator=( tX_real<T,_A_> const &h ) {
      if ( A == _A_ && this == (tX_real<T,A>*)(&h) ) { return *this; }
      x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2];
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
      } else {
        vecSum<3>( x );
        VSEB<3,3>( x );
      }
      return *this;
    }
    template < Algorithm _A_ >
    inline tX_real<T,A> &operator=( qX_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2];
      } else {
        T f[4] = { h.x[0], h.x[1], h.x[2], h.x[3] };
        vecSum<4>( f );
        VSEB<3,4>( f );
        x[0] = f[0]; x[1] = f[1]; x[2] = f[2];
      }
      return *this;
    }

    //
    //
    inline operator T() const {
      if ( A == Algorithm::Quasi ) {
        return x[0] + x[1] + x[2];
      } else {
        return x[0];
      }
    }
    template < Algorithm _A_ >
    inline operator dX_real<T,_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[3] = { x[0], x[1], x[2] };
        vecSum<3>( f );
        VSEB<2,3>( f );
	return dX_real<T,_A_>( f );
      } else {
        return dX_real<T,_A_>( x );
      }
    }
    template < Algorithm _A_ >
    inline operator tX_real<T,_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[3] = { x[0], x[1], x[2] };
        vecSum<3>( f );
        VSEB<3,3>( f );
        return tX_real<T,_A_>( f );
      } else {
        return tX_real<T,_A_>( x );
      }
    }
    template < Algorithm _A_ >
    inline operator qX_real<T,_A_>() const {
      auto c = qX_real<T,_A_>( x[0], x[1], x[2], fp<T>::zero );
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[3] = { x[0], x[1], x[2] };
        vecSum<3>( c.x );
        VSEB<3,3>( c.x );
        return qX_real<T,_A_>( f[0], f[1], f[2], fp<T>::zero );
      } else {
        return qX_real<T,_A_>( x[0], x[1], x[2], fp<T>::zero );
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
    static inline tX_real<T,A> constexpr zero () { return tX_real<T,A>( fp<T>::zero ); }
    static inline tX_real<T,A> constexpr one  () { return tX_real<T,A>( fp<T>::one ); }
    static inline tX_real<T,A> constexpr two  () { return tX_real<T,A>( fp<T>::two ); }
    static inline tX_real<T,A> constexpr half () { return tX_real<T,A>( fp<T>::half ); }

    static inline tX_real<T,A> constexpr epsilon () {
	    T c = fp<T>::epsilon * fp<T>::half; c = (c * c * c) * 2; return tX_real<T,A>( c ); }

    static inline tX_real<T,A> constexpr nan  () { T c = fp<T>::nan; return tX_real<T,A>( c,c,c ); }
    static inline tX_real<T,A> constexpr inf  () { T c = fp<T>::inf; return tX_real<T,A>( c,c,c ); }

    static inline tX_real<T,A> constexpr denorm_min  () {
      T c0 = std::numeric_limits<T>::denorm_min();
      return tX_real<T,A>( c0 );
    }
    static inline tX_real<T,A> constexpr min  () {
      T c0 = (std::numeric_limits<T>::min() * 4) * fp<T>::epsiloni * fp<T>::epsiloni;
      return tX_real<T,A>( c0 );
    }
    static inline tX_real<T,A> constexpr max  () {
      T c0 = std::numeric_limits<T>::max();
      T c1 = c0 * fp<T>::epsilon * fp<T>::half;
      T c2 = c1 * fp<T>::epsilon * fp<T>::half;
      return tX_real<T,A>( c0, c1, c2 );
    }

  };

  // alias names
  template < typename T >
  using tX_real_accurate = tX_real<T,Algorithm::Accurate>;
  template < typename T >
  using tX_real_sloppy   = tX_real<T,Algorithm::Sloppy>;
  template < typename T >
  using tX_real_quasi    = tX_real<T,Algorithm::Quasi>;


  //
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator== ( tX_real<T,Aa> const& a, tX_real<T,Ab> const& b )
  -> std::enable_if_t< A!=Algorithm::Quasi,bool > {
    return a.x[0] == b.x[0] && a.x[1] == b.x[1] && a.x[2] == b.x[2];
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator== ( tX_real<T,Aa> const& a, tX_real<T,Ab> const& b )
  -> std::enable_if_t< Aa==Algorithm::Quasi,bool > {
    return (b == tX_real<T,Algorithm::Accurate>( a ) );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline bool operator!= ( tX_real<T,Aa> const& a, tX_real<T,Ab> const& b ) {
    return !(a == b);
  }


  //
  //
  template < typename T, Algorithm Aa >
  inline tX_real<T,Aa> operator+ ( tX_real<T,Aa> const& a ) {
    using TX = tX_real<T,Aa>;
    TX r = a;
    return r;
  }
  template < typename T, Algorithm Aa >
  inline tX_real<T,Aa> operator- ( tX_real<T,Aa> const& a ) {
    using TX = tX_real<T,Aa>;
    return TX( -a.x[0], -a.x[1], -a.x[2] );
  }


  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator+ ( tX_real<T,Aa> const& a, tX_real<T,Ab> const& b )
  -> std::enable_if_t< A!=Algorithm::Quasi, tX_real<T,A> > {
  //
  //  Accurate or Sloppy
  // 
  //  TW-add TWSum algorithm
  //  N. Fabiano, J-M Muller, and J. Picot
  //  IEEE Trans. Computers, Vol 68, Issue 11, 2019
  // 
    using TX = tX_real<T,A>;

    // vmerge
    T f[6];
    vecMerge<3,3>( f, a.x, b.x );

    // vecsum 5 twoSum
    vecSum<6>( f );

    // VSEB at least 3 quickSum
    VSEB<3,6>( f );
    return TX( f );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator+ ( tX_real<T,Aa> const& a, tX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, tX_real<T,A> > {
  //
  // Quasi by Ozaki
  //
    using TX = tX_real<T,A>;
    T c[3], e[3];
    twoSum( a.x[0], b.x[0], c[0], e[0] );
    twoSum( a.x[1], b.x[1], c[1], e[1] );
    twoSum( c[1], e[0], c[1], e[2] );
    c[2] = a.x[2] + b.x[2] + e[1] + e[2];
    return TX( c );
  }


  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline tX_real<T,A> operator- ( tX_real<T,Aa> const& a, tX_real<T,Ab> const& b ) {
    return (a + (-b));
  }
  template < typename T, Algorithm Ab >
  inline tX_real<T,Ab> operator- ( T const& a, tX_real<T,Ab> const& b ) {
    return (a + (-b));
  }
  template < typename T, Algorithm Aa >
  inline tX_real<T,Aa> operator- ( tX_real<T,Aa> const& a, T const& b ) {
    return (a + (-b));
  }

#if 1
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator* ( tX_real<T,Aa> const& a, tX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Sloppy, tX_real<T,A> > {
  //
  // Sloppy
  // 
  //  TW-mul 3Prod_{3,3}^{fast} algorithm
  //  N. Fabiano, J-M Muller, and J. Picot
  //  IEEE Trans. Computers, Vol 68, Issue 11, 2019
  //
    using TX = tX_real<T,A>;
    T p00, q00;
    T p01, q01;
    T p10, q10;
    twoProdFMA( a.x[0], b.x[0], p00, q00 );
    twoProdFMA( a.x[0], b.x[1], p01, q01 );
    twoProdFMA( a.x[1], b.x[0], p10, q10 );

    T b_[3] = { q00, p01, p10 };
    vecSum<3>( b_ );

    T c   = std::fma( a.x[1], b.x[1], b_[2]);
    T z31 = std::fma( a.x[0], b.x[2], q10 );
    T z32 = std::fma( a.x[2], b.x[0], q01 );

    T z3  = z31 + z32;
    T s3  = c + z3;

    T e[4] = { p00, b_[0], b_[1], s3 };
    vecSum<4>( e );

    VSEB<2,3>( &e[1] );

    return TX( e );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator* ( tX_real<T,Aa> const& a, tX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Accurate, tX_real<T,A> > {
#else
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator* ( tX_real<T,Aa> const& a, tX_real<T,Ab> const& b )
  -> std::enable_if_t< A!=Algorithm::Quasi, tX_real<T,A> > {
#endif
  // 
  // Accurate
  // 
  //  TW-mul 3Prod_{3,3}^{acc} algorithm
  //  N. Fabiano, J-M Muller, and J. Picot
  //  IEEE Trans. Computers, Vol 68, Issue 11, 2019
  //
    using TX = tX_real<T,A>;
    T p00, q00;
    T p01, q01;
    T p10, q10;
    twoProdFMA( a.x[0], b.x[0], p00, q00 );
    twoProdFMA( a.x[0], b.x[1], p01, q01 );
    twoProdFMA( a.x[1], b.x[0], p10, q10 );

    T b_[3] = { q00, p01, p10 };
    vecSum<3>( b_ );

    T c   = std::fma( a.x[1], b.x[1], b_[2]);
    T z31 = std::fma( a.x[0], b.x[2], q10 );
    T z32 = std::fma( a.x[2], b.x[0], q01 );

    T z3  = z31 + z32;

    T e[5] = { p00, b_[0], b_[1], c, z3 };
    vecSum<5>( e );

    VSEB<2,4>( &e[1] );

    return TX( e );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator* ( tX_real<T,Aa> const& a, tX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, tX_real<T,A> > {
  //
  // Quasi by Ozaki
  //
    using TX = tX_real<T,A>;
    T e1, e2, e3, e4, e5;
    T t2, t3;
    TX c;
    twoProdFMA( a.x[0], b.x[0], c.x[0], e1 );
    twoProdFMA( a.x[0], b.x[1], t2, e2 );
    twoProdFMA( a.x[1], b.x[0], t3, e3 );
    twoSum( t2, t3, c.x[1], e4 );
    twoSum( c.x[1], e1, c.x[1], e5 );
    c.x[2] = std::fma( a.x[2], b.x[0], e2 )
           + std::fma( a.x[1], b.x[1], e3 )
           + std::fma( a.x[0], b.x[2], e4 ) + e5;
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline tX_real<T,A> operator* ( dX_real<T,Aa> const& a, tX_real<T,Ab> const& b ) {
    using TX = tX_real<T,Aa>;
    return TX( a ) * b;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline tX_real<T,A> operator* ( tX_real<T,Aa> const& a, dX_real<T,Ab> const& b ) {
    return b * a;
  }
  template < typename T, Algorithm Ab >
  inline auto operator* ( T const& a, tX_real<T,Ab> const& b )
  -> std::enable_if_t< Ab==Algorithm::Sloppy, tX_real<T,Ab> > {
  //
  // Sloppy
  // 
  //  based-on TW-mul 3Prod_{3,3}^{fast} algorithm
  //  N. Fabiano, J-M Muller, and J. Picot
  //  IEEE Trans. Computers, Vol 68, Issue 11, 2019
  //
    using TX = tX_real<T,Ab>;
    T p00, q00;
    T p01, q01;
    twoProdFMA( a, b.x[0], p00, q00 );
    twoProdFMA( a, b.x[1], p01, q01 );

    T b_[2] = { q00, p01 };
    twoSum( b_[0], b_[1], b_[0], b_[1] );

    T s3 = std::fma( a, b.x[2], q01 );

    T e[4] = { p00, b_[0], b_[1], s3 };
    vecSum<4>( e );

    VSEB<2,3>( &e[1] );

    return TX( e );
  }
  template < typename T, Algorithm Ab >
  inline auto operator* ( T const& a, tX_real<T,Ab> const& b )
  -> std::enable_if_t< Ab==Algorithm::Accurate, tX_real<T,Ab> > {
  // 
  // Accurate
  // 
  //  based-on TW-mul 3Prod_{3,3}^{acc} algorithm
  //  N. Fabiano, J-M Muller, and J. Picot
  //  IEEE Trans. Computers, Vol 68, Issue 11, 2019
  // 
    using TX = tX_real<T,Ab>;
    T p00, q00;
    T p01, q01;
    twoProdFMA( a, b.x[0], p00, q00 );
    twoProdFMA( a, b.x[1], p01, q01 );

    T b_[2] = { q00, p01 };
    twoSum( b_[0], b_[1], b_[0], b_[1] );

    T z3 = std::fma( a, b.x[2], q01 );

    T e[4] = { p00, b_[0], b_[1], z3 };
    vecSum<4>( e );

    VSEB<2,3>( &e[1] );

    return TX( e );
  }
  template < typename T, Algorithm Ab >
  inline auto operator* ( T const& a, tX_real<T,Ab> const& b )
  -> std::enable_if_t< Ab==Algorithm::Quasi, tX_real<T,Ab> > {
  // 
  // Quasi by Ozaki
  // 
    using TX = tX_real<T,Ab>;
    T e1, e2, e4, e5, t2;
    T c[3];
    twoProdFMA( a, b.x[0], c[0], e1 );
    twoProdFMA( a, b.x[1], t2, e2 );
    twoSum( t2, e1, c[1], e5 );
    c[2] = e2 + std::fma( a, b.x[2], e5 );
    return TX( c );
  }
  template < typename T, Algorithm Aa >
  inline tX_real<T,Aa> operator* ( tX_real<T,Aa> const& a, T const& b ) {
    return (b * a);
  }
  template < typename T, Algorithm Ab >
  inline tX_real<T,Ab> operator* ( int const& a, tX_real<T,Ab> const& b ) {
    return (T(a) * b);
  }
  template < typename T, Algorithm Aa >
  inline tX_real<T,Aa> operator* ( tX_real<T,Aa> const& a, int const& b ) {
    return (b * a);
  }


  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator/ ( tX_real<T,Aa> const& a, tX_real<T,Ab> const& b )
  -> std::enable_if_t< A!=Algorithm::Quasi, tX_real<T,A> > {
  //
  // Accurate or Sloppy
  //
  //  TW-div 3Div algorithm
  //  N. Fabiano, J-M Muller, and J. Picot
  //  IEEE Trans. Computers, Vol 68, Issue 11, 2019
  //
    using TX = tX_real<T,A>;
    if ( b.x[0] == fp<T>::zero ) { return TX::inf(); }
    if ( b.x[0] == -fp<T>::zero ) { return -TX::inf(); }

    T _2u    = fp<T>::eps();
    T one_2u = fp<T>::one + _2u;
    T alpha  = one_2u / b.x[0];
    T h11    = std::fma( alpha, b.x[0], -one_2u );
    T h1     = std::fma( alpha, b.x[1], h11 );

    T b01, b11;
    twoProdFMA( alpha, fp<T>::one - _2u, b01, b11 );
    T b12 = std::fma( alpha, -h1, b11 );
    TX b_inv;
    quickSum( b01, b12, b_inv.x[0], b_inv.x[1] );

    return (a * b_inv) * (TX::two() - (b * b_inv));
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator/ ( tX_real<T,Aa> const& a, tX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, tX_real<T,A> > {
  //
  // Quasi by Ozaki
  //
    using TX = tX_real<T,A>;
    if ( b.x[0] == fp<T>::inf ) { return TX::zero(); }
    if ( b.x[0] == -fp<T>::inf ) { return -TX::zero(); }
    auto s = b.x[0] + b.x[1];
    if ( s + b.x[2] == fp<T>::zero ) {
      s = s + b.x[2]; auto c = std::copysign( fp<T>::inf, s ); return TX( c,c,c ); }
    if ( b.x[0] == fp<T>::zero ) { return a / tX_real<T,A>( b.x[1], b.x[2], b.x[0] ); }
    auto c = TX ( dX_real<T,A> ( a )  / dX_real<T,A> ( b ) );
    auto t = a - c * b;
    c.x[2] = (t.x[0] + t.x[1] + t.x[2]) / s;
    return c;
  }
  template < typename T, Algorithm Ab >
  inline tX_real<T,Ab> operator/ ( T const& a, tX_real<T,Ab> const& b ) {
    using TX = tX_real<T,Ab>;
    return (TX(a) / b);
  }
  template < typename T, Algorithm Aa >
  inline tX_real<T,Aa> operator/ ( tX_real<T,Aa> const& a, T const& b ) {
    using TX = tX_real<T,Aa>;
    return (a / TX(b));
  }


  //
  //
  template < typename T, Algorithm Aa >
  inline bool isnan ( tX_real<T,Aa> const& a ) {
    auto s = a.x[0];
    if ( Aa == Algorithm::Quasi ) { s += a.x[1] + a.x[2]; }
    return std::isnan( s );
  }
  template < typename T, Algorithm Aa >
  inline bool signbit ( tX_real<T,Aa> const& a ) {
    auto s = a.x[0];
    if ( Aa == Algorithm::Quasi ) { s += a.x[1] + a.x[2]; }
    return std::signbit( s );
  }


  //
  //
  template < typename T, Algorithm Aa >
  inline tX_real<T,Aa> abs ( tX_real<T,Aa> const& a ) {
    auto s = a.x[0];
    if ( Aa == Algorithm::Quasi ) { s += a.x[1] + a.x[2]; }
    if ( s >= fp<T>::zero ) {
      return a;
    } else {
      return -a;
    }
  }


  template < typename T, Algorithm Aa >
  inline auto sqrt ( tX_real<T,Aa> const& a )
  -> std::enable_if_t< Aa!=Algorithm::Quasi, tX_real<T,Aa> > {
    using TX = tX_real<T,Aa>;
    {
      auto s = a.x[0];
      if ( s == fp<T>::zero ) { return a; }
      if ( s < fp<T>::zero ) { return TX::nan(); }
    }

    //
    // this is another version, in which
    // qd_real division of div_accurate in the QD-library uses.
    //

    //
    // scaling
    //   sqrt(a) = a*(1/sqrt(a)) = a * r
    //   1/sqrt(a) => 1/sqrt(a*ex*ex) = 1/sqrt(a')
    //
    auto as = std::sqrt( a.x[0] );
    auto e  = fp<T>::exponent( as );
    auto ex = fp<T>::exponenti( as );
    auto ex2 = ex * fp<T>::half;

    //
    // merged with multiplying a' and 1/2
    // => (a'/2) = (a*ex)*(ex/2) = (a*ex)*ex2
    //
    auto ax = TX( (a.x[0]*ex)*ex2,
                  (a.x[1]*ex)*ex2,
                  (a.x[2]*ex)*ex2 );
    //
    // r = 1/approx.sqrt(a0) = e/approx.sqrt(a0')
    //
    auto r  = TX( e / as );

    //
    // two iterations are enough
    //
    r = (3*TX::half() - ax * (r * r)) * r;
    r = (3*TX::half() - ax * (r * r)) * r;

    //
    // scaling back
    //   sqrt(a) = sqrt(a')/ex = sqrt(a')*e
    //   sqrt(a') = a'/sqrt(a') = (2*(a'/2))*(1/sqrt(a')) = 2*(a'/2) * r
    // Therefore sqrt(a) = 2*(a'/2)*r*e = ax*r*(2*e)
    //
    r = ax * r;
    e = 2 * e;
    r.x[0] *= e;
    r.x[1] *= e;
    r.x[2] *= e;

    return r;
  }
  template < typename T, Algorithm Aa >
  inline auto sqrt ( tX_real<T,Aa> const& a )
  -> std::enable_if_t< Aa==Algorithm::Quasi, tX_real<T,Aa> > {
  //
  // Quasi by Ozaki
  //
    using TX = tX_real<T,Aa>;
    {
      auto s = a.x[0] + a.x[1] + a.x[2];
      if ( s == fp<T>::zero ) { return a; }
      if ( s < fp<T>::zero ) { return TX::nan(); }
    }
    if ( a.x[0] == fp<T>::zero ) { return sqrt( tX_real<T,Aa>( a.x[1], a.x[2], a.x[0] ) ); }

    auto c = TX( sqrt( dX_real<T,Aa>( a ) ) );
    auto t = a - c * c;
    c.x[2] = (t.x[0] + t.x[1] + t.x[2]) / (2*(c.x[0] + c.x[1]));
    return c;
  }


  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto fmin ( tX_real<T,Aa> const& a, tX_real<T,Ab> const &b )
  -> std::enable_if_t< A!=Algorithm::Quasi, tX_real<T,A> > {
    using TX = tX_real<T,A>;
    int i=0; for(i;i<3-1;i++) {
      if ( a.x[i] != b.x[i] ) break;
    }
    if ( a.x[i] <= b.x[i] ) {
      return TX( a );
    } else {
      return TX( b );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto fmin ( tX_real<T,Aa> const& a, tX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, tX_real<T,A> > {
  //
  // Quasi
  //
    using TX = tX_real<T,Algorithm::Accurate>;
    return tX_real<T,A>( fmin( TX(a), TX(b) ) );
  }


  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto fmax ( tX_real<T,Aa> const& a, tX_real<T,Ab> const &b )
  -> std::enable_if_t< A!=Algorithm::Quasi, tX_real<T,A> > {
    using TX = tX_real<T,A>;
    int i=0; for(i;i<3-1;i++) {
      if ( a.x[i] != b.x[i] ) break;
    }
    if ( a.x[i] >= b.x[i] ) {
      return TX( a );
    } else {
      return TX( b );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto fmax ( tX_real<T,Aa> const& a, tX_real<T,Ab> const &b )
  -> std::enable_if_t< A==Algorithm::Quasi, tX_real<T,A> > {
  //
  // Quasi
  //
    using TX = tX_real<T,Algorithm::Accurate>;
    return tX_real<T,A>( fmax( TX(a), TX(b) ) );
  }


  //
  //
  template < typename T, Algorithm Aa >
  inline std::ostream& operator<< ( std::ostream& stream, tX_real<T,Aa> const& a ) {
    stream << std::to_string(a.x[0]) << " "
           << std::to_string(a.x[1]) << " "
           << std::to_string(a.x[2]);
    return stream;
  }

}


#endif

