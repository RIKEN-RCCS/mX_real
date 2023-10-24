#ifndef	DX_REAL_H
#define	DX_REAL_H

#include "mX_real.hpp"

namespace mX_real {

  template < typename T, Algorithm A=Algorithm::Accurate >
  struct dX_real {

    static_assert( fp<T>::value,
                   "The base type must be out of the defined types, such as float or double." );

    //
    static bool constexpr __is_mX_real__ = true;
    //
    static int constexpr L = 2;
    using base = dX_real<T,Algorithm::Accurate>;
    using base_T = T;
    Algorithm base_A = A;
    T x[L];

    //
    // Simple constructor
    //
    inline dX_real() {
      x[0] = x[1] = fp<T>::zero;
    }
    inline dX_real( T const& x0, T const& x1 ) {
      x[0] = x0; x[1] = x1;
    }
    inline dX_real( int const &h ) {
      x[0] = T(h); x[1] = fp<T>::zero;
    }
    inline dX_real( T const *d ) {
      x[0] = d[0]; x[1] = d[1];
    }

    //
    // Copy constructor with (or without) any Algorithm switch
    // auto zero_accurate = df_Real( 0 ); // constructor
    // auto zero_sloppy = df_Real_sloppy( zero_accurate ); // copy constructor
    //
    inline dX_real( T const &h ) { // constructor
      x[0] = h; x[1] = fp<T>::zero;
    }
    template < Algorithm _A_ >
    inline dX_real( dX_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1];
      } else {
        twoSum( h.x[0], h.x[1], x[0], x[1] );
        quickSum( x[0], x[1], x[0], x[1] );
      }
    }
    template < Algorithm _A_ >
    inline dX_real<T,A>( tX_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1];
      } else {
        T f[3] = { h.x[0], h.x[1], h.x[2] };
        vecSum<3>( f );
        VSEB<2,3>( f );
	x[0] = f[0]; x[1] = f[1];
      }
    }
    template < Algorithm _A_ >
    inline dX_real<T,A>( qX_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1];
      } else {
        T f[4] = { h.x[0], h.x[1], h.x[2], h.x[3] };
        vecSum<4>( f );
        VSEB<2,4>( f );
	x[0] = f[0]; x[1] = f[1];
      }
    }

    //
    // Copy-assignment operator
    // auto one = df_Real( 1 ); // a constructor
    // one = df_Real_sloppy( one ); // LHS is a copy-assignment operator
    //
    inline dX_real<T,A> operator=( int const h ) {
      x[0] = T(h); x[1] = fp<T>::zero;
      return *this;
    }
    inline dX_real<T,A> &operator=( int const &h ) {
      x[0] = T(h); x[1] = fp<T>::zero;
      return *this;
    }
    inline dX_real<T,A> &operator=( T const &h ) {
      x[0] = h; x[1] = fp<T>::zero;
      return *this;
    }
    template < Algorithm _A_ >
    inline dX_real<T,A> &operator=( dX_real<T,_A_> const &h ) {
      if ( A == _A_ && this == (dX_real<T,A>*)(&h) ) { return *this; }
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1];
      } else {
        twoSum( h.x[0], h.x[1], x[0], x[1] );
        quickSum( x[0], x[1], x[0], x[1] );
      }
      return *this;
    }
    template < Algorithm _A_ >
    inline dX_real<T,A> &operator=( tX_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1];
      } else {
        T f[3] = { h.x[0], h.x[1], h.x[2] };
        vecSum<3>( f );
        VSEB<2,3>( f );
	x[0] = f[0]; x[1] = f[1];
      }
      return *this;
    }
    template < Algorithm _A_ >
    inline dX_real<T,A> &operator=( qX_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1];
      } else {
        T f[4] = { h.x[0], h.x[1], h.x[2], h.x[3] };
        vecSum<4>( f );
        VSEB<2,4>( f );
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
    inline operator dX_real<T,_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[2];
        twoSum( x[0], x[1], f[0], f[1] );
        quickSum( f[0], f[1], f[0], f[1] );
        return dX_real<T,_A_>( f );
      } else {
        return dX_real<T,_A_>( x );
      }
    }
    template < Algorithm _A_ >
    inline operator tX_real<T,_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[2];
        twoSum( x[0], x[1], f[0], f[1] );
        quickSum( f[0], f[1], f[0], f[1] );
        return tX_real<T,_A_>( f[0], f[1], fp<T>::zero );
      } else {
        return tX_real<T,_A_>( x[0], x[1], fp<T>::zero );
      }
    }
    template < Algorithm _A_ >
    inline operator qX_real<T,_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[2];
        twoSum( x[0], x[1], f[0], f[1] );
        quickSum( f[0], f[1], f[0], f[1] );
        return qX_real<T,_A_>( f[0], f[1], fp<T>::zero, fp<T>::zero );
      } else {
        return qX_real<T,_A_>( x[0], x[1], fp<T>::zero, fp<T>::zero );
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
    static inline dX_real<T,A> constexpr zero ( void ) { return dX_real<T,A>( fp<T>::zero ); }
    static inline dX_real<T,A> constexpr one  ( void ) { return dX_real<T,A>( fp<T>::one );  }
    static inline dX_real<T,A> constexpr two  ( void ) { return dX_real<T,A>( fp<T>::two );  }
    static inline dX_real<T,A> constexpr half ( void ) { return dX_real<T,A>( fp<T>::half ); }
    static inline dX_real<T,A> constexpr epsilon ( void ) {
	    T c = fp<T>::epsilon * fp<T>::half; c = (c * c) * 2; return dX_real<T,A>( c ); }
    static inline dX_real<T,A> constexpr nan  ( void ) { T c = fp<T>::nan; return dX_real<T,A>( c,c ); }
    static inline dX_real<T,A> constexpr inf  ( void ) { T c = fp<T>::inf; return dX_real<T,A>( c,c ); }

  };


  // alias names
  template < typename T >
  using dX_real_accurate = dX_real<T,Algorithm::Accurate>;
  template < typename T >
  using dX_real_sloppy   = dX_real<T,Algorithm::Sloppy>;
  template < typename T >
  using dX_real_quasi    = dX_real<T,Algorithm::Quasi>;


  //
  //
  template < typename T, Algorithm A >
  inline auto debug_print ( std::string message, dX_real<T,A> const &x, bool const flag = false )
  -> std::enable_if_t< std::is_same< T, float >::value, void > {
    uint32_t * d = (uint32_t *)(&x.x[0]);
    long double xx =
          (long double)x.x[1] +
          (long double)x.x[0];
    std::cout << message;
    printf(" %cDF : %08x %08x %26.19Le\n",
		    toString(A)[0], d[0], d[1], xx);
    if ( flag ) {
    std::cout << "  : " << std::bitset<32>(d[0])
            << "\n    " << std::bitset<32>(d[1]);
    printf(" %26.19Le\n", xx);
    }
  }
  template < typename T, Algorithm A >
  inline auto debug_print ( std::string message, dX_real<T,A> const &x, bool const flag = false )
  -> std::enable_if_t< std::is_same< T, double >::value, void > {
    uint64_t * d = (uint64_t *)(&x.x[0]);
    long double xx =
          (long double)x.x[1] +
          (long double)x.x[0];
    std::cout << message;
    printf(" %cDD : %016lx %016lx %26.19Le\n",
		    toString(A)[0], d[0], d[1], xx);
    if ( flag ) {
    std::cout << "  : " << std::bitset<64>(d[0])
            << "\n    " << std::bitset<64>(d[1]);
    printf(" %26.19Le\n", xx);
    }
  }


  //
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator== ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< A!=Algorithm::Quasi,bool > {
    return a.x[0] == b.x[0] && a.x[1] == b.x[1];
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator== ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< Aa==Algorithm::Quasi,bool > {
    return (b == dX_real<T,Algorithm::Accurate>( a ) );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline bool operator!= ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b ) {
    return !(a == b);
  }


  //
  //
  template < typename T, Algorithm Aa >
  inline dX_real<T,Aa> operator+ ( dX_real<T,Aa> const& a ) {
    using TX = dX_real<T,Aa>;
    TX  r = a;
    return r;
  }
  template < typename T, Algorithm Aa >
  inline dX_real<T,Aa> operator- ( dX_real<T,Aa> const& a ) {
    using TX = dX_real<T,Aa>;
    return TX( -a.x[0], -a.x[1] );
  }


  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator+ ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< A!=Algorithm::Accurate, dX_real<T,A> > {
  //
  // Sloppy in QD by Bailey and Hida
  // Quasi/PW by Ozaki
  //
    using TX = dX_real<T,A>;
    T p0, q0;
    twoSum( a.x[0], b.x[0], p0, q0 );
    q0 = (a.x[1] + b.x[1]) + q0;
    if ( A != Algorithm::Quasi ) Normalize<2>( p0, q0 );
    return TX( p0, q0 );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator+ ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Accurate, dX_real<T,A> > {
  //
  // Accurate (=IEEE/accurate) in QD by Bailey and Hida
  //
    using TX = dX_real<T,A>;
    T p0, p1, q0, q1;
    twoSum( a.x[0], b.x[0], p0, q0 );
    twoSum( a.x[1], b.x[1], p1, q1 );
    q0 = q0 + p1;
    Normalize<2>( p0, q0 );
    q0 = q0 + q1;
    Normalize<2>( p0, q0 );
    return TX( p0, q0 );
  }
  template < typename T, Algorithm Ab >
  inline dX_real<T,Ab> operator+ ( T const& a, dX_real<T,Ab> const& b ) {
  //
  // Sloppy in QD by Bailey and Hida
  // Quasi/PW by Ozaki
  //
    using TX = dX_real<T,Ab>;
    T p0, q0;
    twoSum( a, b.x[0], p0, q0 );
    q0 = q0 + b.x[1];
    if ( Ab != Algorithm::Quasi ) Normalize<2>( p0, q0 );
    return TX( p0, q0 );
  }
  template < typename T, Algorithm Aa >
  inline dX_real<T,Aa> operator+ ( dX_real<T,Aa> const& a, T const& b ) {
    return (b + a);
  }


  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline dX_real<T,A> operator- ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b ) {
    return (a + (-b));
  }
  template < typename T, Algorithm Ab >
  inline dX_real<T,Ab> operator- ( T const& a, dX_real<T,Ab> const& b ) {
    return (a + (-b));
  }
  template < typename T, Algorithm Aa >
  inline dX_real<T,Aa> operator- ( dX_real<T,Aa> const& a, T const& b ) {
    return (a + (-b));
  }


  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline dX_real<T,A> operator* ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b ) {
  //
  // Sloppy in QD by Bailey and Hida
  // Accurate in QD by Bailey and Hida
  // Quasi by Ozaki
  //
    using TX = dX_real<T,A>;
    T p0, q0;
    twoProdFMA( a.x[0], b.x[0], p0, q0 );
    if ( A == Algorithm::Quasi ) {
      q0 = std::fma( a.x[0], b.x[1], std::fma( a.x[1], b.x[0], q0 ) );
    } else {
    // full-compatible to the QD-library
      q0 = q0 + (a.x[0] * b.x[1] + a.x[1] * b.x[0]);
    }
    if ( A != Algorithm::Quasi ) Normalize<2>( p0, q0 );
    return TX( p0, q0 );
  }
  template < typename T, Algorithm Ab >
  inline dX_real<T,Ab> operator* ( T const& a, dX_real<T,Ab> const& b ) {
    using TX = dX_real<T,Ab>;
    T p0, q0;
    twoProdFMA( a, b.x[0], p0, q0 );
    if ( Ab == Algorithm::Quasi ) {
      q0 = std::fma( a, b.x[1], q0 );
    } else {
    // full-compatible to the QD-library
      q0 = q0 + a * b.x[1];
    }
    if ( Ab != Algorithm::Quasi ) Normalize<2>( p0, q0 );
    return TX( p0, q0 );
  }
  template < typename T, Algorithm Aa >
  inline dX_real<T,Aa> operator* ( dX_real<T,Aa> const& a, T const& b ) {
    return (b * a);
  }
  template < typename T, Algorithm Ab >
  inline dX_real<T,Ab> operator* ( int const& a, dX_real<T,Ab> const& b ) {
    return (T(a) * b);
  }
  template < typename T, Algorithm Aa >
  inline dX_real<T,Aa> operator* ( dX_real<T,Aa> const& a, int const& b ) {
    return (b * a);
  }


  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator/ ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Sloppy, dX_real<T,A> > {
  //
  // Sloppy in QD by Bailey and Hida
  //
    using TX = dX_real<T,A>;
    if ( b.x[0] == inf<T>() ) { return zero<TX>(); }
    if ( b.x[0] == -inf<T>() ) { return -zero<TX>(); }
    if ( b.x[0] == zero<T>() ) { auto c = std::copysign( inf<T>(), b.x[0] ); return TX( c,c ); }
    auto q0 = a.x[0] / b.x[0];
    auto r = TX( b * q0 );
    T s0, s1;
    twoSum( a.x[0], -r.x[0], s0, s1 );
    s1 -= r.x[1];
    s1 += a.x[1];
    auto q1 = (s0 + s1) / b.x[0];
    quickSum( q0, q1, r.x[0], r.x[1] );
    return r;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator/ ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Accurate, dX_real<T,A> > {
  //
  // Accurate in QD by Bailey and Hida
  //
    using TX = dX_real<T,A>;
    if ( b.x[0] == inf<T>() ) { return zero<TX>(); }
    if ( b.x[0] == -inf<T>() ) { return -zero<TX>(); }
    if ( b.x[0] == zero<T>() ) { auto c = std::copysign( inf<T>(), b.x[0] ); return TX( c,c ); }
    auto q0 = a.x[0] / b.x[0];
    auto r = a - b * q0;
    auto q1 = r.x[0] / b.x[0];
    r = r - b * q1;
    auto q2 = r.x[0] / b.x[0];
    quickSum( q0, q1, r.x[0], r.x[1] );
    r = r + q2;
    return r;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator/ ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, dX_real<T,A> > {
  //
  // Quasi/PW by Ozaki
  //
    using TX = dX_real<T,A>;
    if ( b.x[0] == inf<T>() ) { return zero<TX>(); }
    if ( b.x[0] == -inf<T>() ) { return -zero<TX>(); }
    auto s = b.x[0] + b.x[1];
    if ( s == zero<T>() ) { auto c = std::copysign( inf<T>(), s ); return TX( c,c ); }
    if ( b.x[0] == zero<T>() ) { return a / dX_real<T,A>( b.x[1], b.x[0] ); }
    auto ch = a.x[0] / b.x[0];
    auto p  = std::fma( -b.x[0], ch, a.x[0] ) + a.x[1];
    auto cl = std::fma( ch, -b.x[1], p ) / b.x[0];
    return TX( ch,cl );
  }
  template < typename T, Algorithm Ab >
  inline dX_real<T,Ab> operator/ ( T const& a, dX_real<T,Ab> const& b ) {
    using TX = dX_real<T,Ab>;
    return ( TX(a) / b );
  }
  template < typename T, Algorithm Aa >
  inline dX_real<T,Aa> operator/ ( dX_real<T,Aa> const& a, T const& b ) {
    using TX = dX_real<T,Aa>;
    return ( a / TX(b) );
  }


  //
  //
  template < typename T, Algorithm Aa >
  inline dX_real<T,Aa> abs ( dX_real<T,Aa> const& a ) {
    auto s = a.x[0];
    if ( Aa == Algorithm::Quasi ) { s += a.x[1]; }
    if ( s >= zero<T>() ) {
      return a;
    } else {
      return -a;
    }
  }

  template < typename T, Algorithm Aa >
  inline auto sqrt ( dX_real<T,Aa> const& a )
  -> std::enable_if_t< Aa!=Algorithm::Quasi, dX_real<T,Aa> > {
  //
  // Sloppy or Accurate
  // Karp's trick
  //
    using TX = dX_real<T,Aa>;
    {
      auto s = a.x[0];
      if ( s == zero<T>() ) { return a; }
      if ( s < zero<T>() ) { return nan<TX>(); }
    }

    auto as = std::sqrt( a.x[0] );
    auto e  = fp<T>::exponent( as );
    auto ex = fp<T>::exponenti( as );
    auto ax = TX( (a.x[0]*ex)*ex, (a.x[1]*ex)*ex );

    auto x  = e / as;
    T r0 = ax.x[0] * x;
    TX err;
    twoProdFMA( r0, r0, err.x[0], err.x[1] );
    err = (ax - err) * x;

    return TX( r0*e, (err.x[0] * half<T>())*e );
  }
  template < typename T, Algorithm Aa >
  inline auto sqrt ( dX_real<T,Aa> const& a )
  -> std::enable_if_t< Aa==Algorithm::Quasi, dX_real<T,Aa> > {
  //
  // Quasi/PW by Ozaki
  //
    using TX = dX_real<T,Aa>;
    {
      auto s = a.x[0] + a.x[1];
      if ( s == zero<T>() ) { return a; }
      if ( s < zero<T>() ) { return nan<TX>(); }
      if ( a.x[0] == zero<T>() ) { return sqrt( dX_real<T,Aa>( a.x[1], a.x[0] ) ); }
    }

    auto ch = std::sqrt( a.x[0] );
    auto cl = ( std::fma( -ch, ch, a.x[0] ) + a.x[1] ) / (2*ch);
    return TX( ch, cl );
  }

  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto min ( dX_real<T,Aa> const& a, dX_real<T,Ab> const &b )
  -> std::enable_if_t< A!=Algorithm::Quasi, dX_real<T,A> > {
  //
  // Sloppy or Accurate
  //
    using TX = dX_real<T,A>;
    int i=0; for(i;i<2-1;i++) {
      if ( a.x[i] != b.x[i] ) break;
    }
    if ( a.x[i] <= b.x[i] ) {
      return TX( a );
    } else {
      return TX( b );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto min ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, dX_real<T,A> > {
  //
  // Quasi
  //
    using TX = dX_real<T,Algorithm::Accurate>;
    return dX_real<T,A>( min( TX(a), TX(b) ) );
  }

  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto max ( dX_real<T,Aa> const& a, dX_real<T,Ab> const &b )
  -> std::enable_if_t< A!=Algorithm::Quasi, dX_real<T,A> > {
  //
  // Sloppy or Accurate
  //
    using TX = dX_real<T,A>;
    int i=0; for(i;i<2-1;i++) {
      if ( a.x[i] != b.x[i] ) break;
    }
    if ( a.x[i] >= b.x[i] ) {
      return TX( a );
    } else {
      return TX( b );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto max ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, dX_real<T,A> > {
  //
  // Quasi
  //
    using TX = dX_real<T,Algorithm::Accurate>;
    return dX_real<T,A>( max( TX(a), TX(b) ) );
  }


  //
  //
  template < typename T, Algorithm Aa >
  inline std::ostream& operator<< ( std::ostream& stream, dX_real<T,Aa> const& a ) {
    stream << std::to_string(a.x[0])
    << " " << std::to_string(a.x[1]);
    return stream;
  }

}

namespace std {
  template < typename T, mX_real::Algorithm A >
  class numeric_limits<mX_real::dX_real<T,A>> {
    using TX = mX_real::dX_real<T,A>;
  public:
    inline static TX epsilon() { return TX::epsilon(); }
    inline static TX infinity() { return TX::inf(); }
    inline static TX quiet_NaN() { return TX::nan(); }

    static int const digits       = 53*2;
  };
}


#endif

