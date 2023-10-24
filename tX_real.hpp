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
    using base = tX_real<T,Algorithm::Accurate>;
    using base_T = T;
    Algorithm base_A = A;
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
    static inline tX_real<T,A> constexpr zero ( void ) { return tX_real<T,A>( fp<T>::zero ); }
    static inline tX_real<T,A> constexpr one  ( void ) { return tX_real<T,A>( fp<T>::one ); }
    static inline tX_real<T,A> constexpr two  ( void ) { return tX_real<T,A>( fp<T>::two ); }
    static inline tX_real<T,A> constexpr half ( void ) { return tX_real<T,A>( fp<T>::half ); }
    static inline tX_real<T,A> constexpr epsilon ( void ) {
	    T c = fp<T>::epsilon * fp<T>::half; c = (c * c * c) * 2; return tX_real<T,A>( c ); }
    static inline tX_real<T,A> constexpr nan  ( void ) { T c = fp<T>::nan; return tX_real<T,A>( c,c,c ); }
    static inline tX_real<T,A> constexpr inf  ( void ) { T c = fp<T>::inf; return tX_real<T,A>( c,c,c ); }

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
  template < typename T, Algorithm A >
  inline auto debug_print ( std::string message, tX_real<T,A> const &x, bool const flag = false )
  -> std::enable_if_t< std::is_same< T, float >::value, void > {
    uint32_t * d = (uint32_t *)(&x.x[0]);
    long double xx = (long double)x.x[2]
                   + (long double)x.x[1]
                   + (long double)x.x[0];
    std::cout << message;
    printf(" %cTF : %08x %08x %08x %26.19Le\n",
		    toString(A)[0], d[0], d[1], d[2], xx);
    if ( flag ) {
    std::cout << "  : " << std::bitset<32>(d[0])
            << "\n    " << std::bitset<32>(d[1])
	    << "\n    " << std::bitset<32>(d[2]);
    printf(" %26.19Le\n", xx);
    }
  }
  template < typename T, Algorithm A >
  inline auto debug_print ( std::string message, tX_real<T,A> const &x, bool const flag = false )
  -> std::enable_if_t< std::is_same< T, double >::value, void > {
    uint64_t * d = (uint64_t *)(&x.x[0]);
    long double xx = (long double)x.x[2]
                   + (long double)x.x[1]
                   + (long double)x.x[0];
    std::cout << message;
    printf(" %cTD : %016lx %016lx %016lx %26.19Le\n",
		    toString(A)[0], d[0], d[1], d[2], xx);
    if ( flag ) {
    std::cout << "  : " << std::bitset<64>(d[0])
            << "\n    " << std::bitset<64>(d[1])
	    << "\n    " << std::bitset<64>(d[2]);
    printf(" %26.19Le\n", xx);
    }
  }


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


    if ( e[0] == nan<T>() )  { printf("0> %le %le %le\n", e[0], e[1], e[2] ); }
    if ( e[0] == -nan<T>() ) { printf("1> %le %le %le\n", e[0], e[1], e[2] ); }
    if ( e[1] == nan<T>() )  { printf("2> %le %le %le\n", e[0], e[1], e[2] ); }
    if ( e[1] == -nan<T>() ) { printf("3> %le %le %le\n", e[0], e[1], e[2] ); }
    if ( e[2] == nan<T>() )  { printf("4> %le %le %le\n", e[0], e[1], e[2] ); }
    if ( e[2] == -nan<T>() ) { printf("5> %le %le %le\n", e[0], e[1], e[2] ); }


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
    if ( b.x[0] == zero<T>() ) { return inf<TX>(); }
    if ( b.x[0] == -zero<T>() ) { return -inf<TX>(); }

    T _2u    = epsilon<T>();
    T one_2u = one<T>() + _2u;
    T alpha  = one_2u / b.x[0];
    T h11    = std::fma( alpha, b.x[0], -one_2u );
    T h1     = std::fma( alpha, b.x[1], h11 );

    T b01, b11;
    twoProdFMA( alpha, one<T>() - _2u, b01, b11 );
    T b12 = std::fma( alpha, -h1, b11 );
    TX b_inv;
    quickSum( b01, b12, b_inv.x[0], b_inv.x[1] );

    return (a * b_inv) * (two<TX>() - (b * b_inv));
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator/ ( tX_real<T,Aa> const& a, tX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, tX_real<T,A> > {
  //
  // Quasi by Ozaki
  //
    using TX = tX_real<T,A>;
    if ( b.x[0] == inf<T>() ) { return zero<TX>(); }
    if ( b.x[0] == -inf<T>() ) { return -zero<TX>(); }
    auto s = b.x[0] + b.x[1];
    if ( s + b.x[2] == zero<T>() ) {
      s = s + b.x[2]; auto c = std::copysign( inf<TX>(), s ); return TX( c,c,c ); }
    if ( b.x[0] == zero<T>() ) { return a / tX_real<T,A>( b.x[1], b.x[2], b.x[0] ); }
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
  inline tX_real<T,Aa> abs ( tX_real<T,Aa> const& a ) {
    auto s = a.x[0];
    if ( Aa == Algorithm::Quasi ) { s += a.x[1] + a.x[2]; }
    if ( s >= zero<T>() ) {
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
      if ( s == zero<T>() ) { return a; }
      if ( s < zero<T>() ) { return nan<TX>(); }
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
    auto ex2 = ex * half<T>();

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
    r = (3*half<TX>() - ax * (r * r)) * r;
    r = (3*half<TX>() - ax * (r * r)) * r;

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
      if ( s == zero<T>() ) { return a; }
      if ( s < zero<T>() ) { return nan<TX>(); }
    }
    if ( a.x[0] == zero<T>() ) { return sqrt( tX_real<T,Aa>( a.x[1], a.x[2], a.x[0] ) ); }

    auto c = TX( sqrt( dX_real<T,Aa>( a ) ) );
    auto t = c * c - a;
    c.x[2] = - (t.x[0] + t.x[1] + t.x[2]) / (2*(c.x[0] + c.x[1]));
    return c;
  }


  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto min ( tX_real<T,Aa> const& a, tX_real<T,Ab> const &b )
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
  inline auto min ( tX_real<T,Aa> const& a, tX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, tX_real<T,A> > {
  //
  // Quasi
  //
    using TX = tX_real<T,Algorithm::Accurate>;
    return tX_real<T,A>( min( TX(a), TX(b) ) );
  }


  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto max ( tX_real<T,Aa> const& a, tX_real<T,Ab> const &b )
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
  inline auto max ( tX_real<T,Aa> const& a, tX_real<T,Ab> const &b )
  -> std::enable_if_t< A==Algorithm::Quasi, tX_real<T,A> > {
  //
  // Quasi
  //
    using TX = tX_real<T,Algorithm::Accurate>;
    return tX_real<T,A>( max( TX(a), TX(b) ) );
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

namespace std {
  template < typename T, mX_real::Algorithm A >
  class numeric_limits<mX_real::tX_real<T,A>> {
    using TX = mX_real::tX_real<T,A>;
  public:
    inline static TX epsilon() { return TX::epsilon(); }
    inline static TX infinity() { return TX::inf(); }
    inline static TX quiet_NaN() { return TX::nan(); }

    static int const digits       = 53*3;
  };
}


#endif

