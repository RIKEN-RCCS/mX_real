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
    //
    using base_T = T;
    static Algorithm constexpr base_A = A;
    //
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
    static inline dX_real<T,A> constexpr zero () { return dX_real<T,A>( fp<T>::zero ); }
    static inline dX_real<T,A> constexpr one  () { return dX_real<T,A>( fp<T>::one );  }
    static inline dX_real<T,A> constexpr two  () { return dX_real<T,A>( fp<T>::two );  }
    static inline dX_real<T,A> constexpr half () { return dX_real<T,A>( fp<T>::half ); }

    static inline dX_real<T,A> constexpr epsilon () {
      T c = fp<T>::epsilon * fp<T>::half; c = (c * c) * 2; return dX_real<T,A>( c ); }

    static inline dX_real<T,A> constexpr nan  () { T c = fp<T>::nan; return dX_real<T,A>( c,c ); }
    static inline dX_real<T,A> constexpr inf  () { T c = fp<T>::inf; return dX_real<T,A>( c,c ); }

    static inline dX_real<T,A> constexpr denorm_min  () {
      T c0 = std::numeric_limits<T>::denorm_min();
      return dX_real<T,A>( c0 );
    }
    static inline dX_real<T,A> constexpr min  () {
      T c0 = (std::numeric_limits<T>::min() * 2) * fp<T>::epsiloni;
      return dX_real<T,A>( c0 );
    }
    static inline dX_real<T,A> constexpr max  () {
      T c0 = std::numeric_limits<T>::max();
      T c1 = c0 * fp<T>::epsilon * fp<T>::half;
      return dX_real<T,A>( c0, c1 );
    }


    //
    // friend functions
    //
    static inline dX_real<T,A> const rand ();
    static inline dX_real<T,A> const sqrt ( T const& x );

  };


  //
  // Alias names
  //
  template < typename T >
  using dX_real_accurate = dX_real<T,Algorithm::Accurate>;
  template < typename T >
  using dX_real_sloppy   = dX_real<T,Algorithm::Sloppy>;
  template < typename T >
  using dX_real_quasi    = dX_real<T,Algorithm::Quasi>;


  //
  // Comparation
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator== ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< A!=Algorithm::Quasi,bool > {
    return a.x[0] == b.x[0] && a.x[1] == b.x[1];
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator== ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< Aa==Algorithm::Quasi,bool > {
    return b == dX_real<T,Algorithm::Accurate>( a );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline bool operator!= ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b ) {
    return !(a == b);
  }


  //
  // Unitary operators
  //
  template < typename T, Algorithm Aa >
  inline auto operator+ ( dX_real<T,Aa> const& a ) {
    using TX = dX_real<T,Aa>;
    TX  r = a;
    return r;
  }
  template < typename T, Algorithm Aa >
  inline auto operator- ( dX_real<T,Aa> const& a ) {
    using TX = dX_real<T,Aa>;
    return TX( -a.x[0], -a.x[1] );
  }


  //
  // Addition
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline dX_real<T,A> operator_add_dX ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b ) {
    using TX = dX_real<T,A>;
    T p0, q0;
    twoSum( a.x[0], b.x[0], p0, q0 );
    if ( A == Algorithm::Accurate ) {
      T p1, q1;
      twoSum( a.x[1], b.x[1], p1, q1 );
      q0 = q0 + p1;
      Normalize<2>( p0, q0 );
      q0 = q0 + q1;
    } else {
      q0 = (a.x[1] + b.x[1]) + q0;
    }
    if ( A != Algorithm::Quasi ) Normalize<2>( p0, q0 );
    return TX( p0, q0 );
  }
  template < typename T, Algorithm Ab >
  inline dX_real<T,Ab> operator_add_dX ( T const& a, dX_real<T,Ab> const& b ) {
  //
  // Sloppy in QD by Bailey and Hida
  // Accurate
  // Quasi/PW by Ozaki
  //
    using TX = dX_real<T,Ab>;
    T p0, q0;
    twoSum( a, b.x[0], p0, q0 );
    q0 = b.x[1] + q0;
    if ( Ab != Algorithm::Quasi ) Normalize<2>( p0, q0 );
    return TX( p0, q0 );
  }
  template < typename T, Algorithm A >
  inline dX_real<T,A> operator_add_dX ( T const& a, T const& b ) {
    using TX = dX_real<T,A>;
    T p0, q0;
    twoSum( a, b, p0, q0 );
    if ( A != Algorithm::Quasi ) { quickSum( p0, q0, p0, q0 ); }
    return TX( p0, q0 );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator+ ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b ) {
    return operator_add_dX( a, b );
  }
  template < typename Ts, typename T, Algorithm Ab >
  inline auto operator+ ( Ts const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< std::is_arithmetic<Ts>::value, dX_real<T,Ab> > {
    return operator_add_dX( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa >
  inline auto operator+ ( dX_real<T,Aa> const& a, Ts const& b ) {
    return b + a;
  }


  //
  // Substruction
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator- ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename Ts, typename T, Algorithm Ab >
  inline auto operator- ( Ts const& a, dX_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename Ts, typename T, Algorithm Aa >
  inline auto operator- ( dX_real<T,Aa> const& a, Ts const& b ) {
    return a + (-b);
  }


  //
  // Multiplication
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline dX_real<T,A> operator_mul_dX ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b ) {
  //
  // Sloppy in QD by Bailey and Hida
  // Accurate in QD by Bailey and Hida
  // Quasi by Ozaki
  //
    using TX = dX_real<T,A>;
    T p0, q0;
    // O(1)
    twoProdFMA( a.x[0], b.x[0], p0, q0 );
    // O(eps)
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
  inline dX_real<T,Ab> operator_mul_dX ( T const& a, dX_real<T,Ab> const& b ) {
    using TX = dX_real<T,Ab>;
    T p0, q0;
    // O(1)
    twoProdFMA( a, b.x[0], p0, q0 );
    // O(eps)
    if ( Ab == Algorithm::Quasi ) {
      q0 = std::fma( a, b.x[1], q0 );
    } else {
    // full-compatible to the QD-library
      q0 = q0 + a * b.x[1];
    }
    if ( Ab != Algorithm::Quasi ) Normalize<2>( p0, q0 );
    return TX( p0, q0 );
  }
  template < typename T, Algorithm A >
  inline dX_real<T,A> operator_mul_dX ( T const& a, T const& b ) {
    using TX = dX_real<T,A>;
    T p0, q0;
    twoProdFMA( a, b, p0, q0 );
    return TX( p0, q0 );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator* ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b ) {
    return operator_mul_dX( a, b );
  }
  template < typename Ts, typename T, Algorithm Ab >
  inline auto operator* ( Ts const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< std::is_arithmetic<Ts>::value, dX_real<T,Ab> > {
    return operator_mul_dX( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa >
  inline auto operator* ( dX_real<T,Aa> const& a, Ts const& b ) {
    return b * a;
  }


  //
  // Division
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_div_dX ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Sloppy, dX_real<T,A> > {
  //
  // Sloppy in QD by Bailey and Hida
  //
    using TX = dX_real<T,A>;
    {
      if ( b.x[0] == fp<T>::inf ) { return TX::zero(); }
      if ( b.x[0] == -fp<T>::inf ) { return -TX::zero(); }
      if ( b.x[0] == fp<T>::zero ) { auto c = std::copysign( fp<T>::inf, b.x[0] ); return TX( c,c ); }
    }
    auto q0 = a.x[0] / b.x[0];
    auto r = b * q0;
    T s0, s1;
    twoSum( a.x[0], -r.x[0], s0, s1 );
    s1 -= r.x[1];
    s1 += a.x[1];
    auto q1 = (s0 + s1) / b.x[0];
    quickSum( q0, q1, r.x[0], r.x[1] );
    return r;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_div_dX ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Accurate, dX_real<T,A> > {
  //
  // Accurate in QD by Bailey and Hida
  //
    using TX = dX_real<T,A>;
    {
      if ( b.x[0] == fp<T>::inf ) { return TX::zero(); }
        if ( b.x[0] == -fp<T>::inf ) { return -TX::zero(); }
    if ( b.x[0] == fp<T>::zero ) { auto c = std::copysign( fp<T>::inf, b.x[0] ); return TX( c,c ); }
    }
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
  inline auto operator_div_dX ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, dX_real<T,A> > {
  //
  // Quasi/PW by Ozaki
  //
    using TX = dX_real<T,A>;
    {
      if ( b.x[0] == fp<T>::inf ) { return TX::zero(); }
      if ( b.x[0] == -fp<T>::inf ) { return -TX::zero(); }
    }
    auto s = b.x[0] + b.x[1];
    {
      if ( s == fp<T>::zero ) { auto c = std::copysign( fp<T>::inf, s ); return TX( c,c ); }
      if ( b.x[0] == fp<T>::zero ) { return a / dX_real<T,A>( b.x[1], b.x[0] ); }
    }
    auto ch = a.x[0] / b.x[0];
    auto p  = std::fma( -b.x[0], ch, a.x[0] ) + a.x[1];
    auto cl = std::fma( ch, -b.x[1], p ) / b.x[0];
    return TX( ch,cl );
  }
  template < typename T, Algorithm Ab >
  inline dX_real<T,Ab> operator_div_dX ( T const& a, dX_real<T,Ab> const& b ) {
    using TX = dX_real<T,Ab>;
    return ( TX(a) / b );
  }
  template < typename T, Algorithm Aa >
  inline dX_real<T,Aa> operator_div_dX ( dX_real<T,Aa> const& a, T const& b ) {
    using TX = dX_real<T,Aa>;
    return ( a / TX(b) );
  }
  template < typename T, Algorithm A >
  inline dX_real<T,A> operator_div_dX ( T const& a, T const& b ) {
    using TX = dX_real<T,A>;
    {
      if ( b == fp<T>::inf ) { return TX::zero(); }
      if ( b == -fp<T>::inf ) { return -TX::zero(); }
      if ( b == fp<T>::zero ) { auto c = std::copysign( fp<T>::inf, b ); return TX( c,c ); }
    }
    T c[2];
    c[0] = a / b;
    c[1] = std::fma( -b, c[0], a );
    if ( A != Algorithm::Quasi ) { Normalize<2>( c[0], c[1] ); }
    return TX( c );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator/ ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b ) {
    return operator_div_dX( a, b );
  }
  template < typename Ts, typename T, Algorithm Ab >
  inline auto operator/ ( Ts const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< std::is_arithmetic<Ts>::value, dX_real<T,Ab> > {
    return operator_div_dX( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa >
  inline auto operator/ ( dX_real<T,Aa> const& a, Ts const& b )
  -> std::enable_if_t< std::is_arithmetic<Ts>::value, dX_real<T,Aa> > {
    return operator_div_dX( a, T(b) );
  }


  //
  //
  template < typename T, Algorithm Aa >
  inline bool const isnan ( dX_real<T,Aa> const& a ) {
    auto s = a.x[0];
    if ( Aa == Algorithm::Quasi ) { s += a.x[1]; }
    return std::isnan( s );
  }
  template < typename T, Algorithm Aa >
  inline bool const signbit ( dX_real<T,Aa> const& a ) {
    auto s = a.x[0];
    if ( Aa == Algorithm::Quasi ) { s += a.x[1]; }
    return std::signbit( s );
  }


  //
  // Absolute value
  //
  template < typename T, Algorithm Aa >
  inline dX_real<T,Aa> const abs ( dX_real<T,Aa> const& a ) {
    auto s = a.x[0];
    if ( Aa == Algorithm::Quasi ) { s += a.x[1]; }
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
  inline auto const sqrt ( dX_real<T,Aa> const& a )
  -> std::enable_if_t< Aa!=Algorithm::Quasi, dX_real<T,Aa> > {
  //
  // Sloppy or Accurate
  // Karp's trick
  //
    using TX = dX_real<T,Aa>;
    {
      auto s = a.x[0];
      if ( s == fp<T>::zero ) { return a; }
      if ( s < fp<T>::zero ) { return TX::nan(); }
    }

    // care of the {under|over}flow
    auto as = std::sqrt( a.x[0] );
    auto e  = fp<T>::exponent( as );
    auto ex = fp<T>::exponenti( as );
    auto exx = ex * ex; // if a.x[0] is regular, normal_min() < {ex, exx} < max()
    auto ax = TX( a.x[0]*exx, a.x[1]*exx ); // 1<= |ax| < 2

    auto x  = e / as;
    auto r0 = ax.x[0] * x;
    TX err;
    twoProdFMA( r0, r0, err.x[0], err.x[1] );
    err = (ax - err) * x;
    r0 = r0 * e;
    auto r1 = (err.x[0] * fp<T>::half) * e;

    return TX( r0, r1 );
  }
  template < typename T, Algorithm Aa >
  inline auto const sqrt ( dX_real<T,Aa> const& a )
  -> std::enable_if_t< Aa==Algorithm::Quasi, dX_real<T,Aa> > {
  //
  // Quasi/PW by Ozaki
  //
    using TX = dX_real<T,Aa>;
    {
      auto s = a.x[0] + a.x[1];
      if ( s == fp<T>::zero ) { return a; }
      if ( s < fp<T>::zero ) { return TX::nan(); }
      if ( a.x[0] == fp<T>::zero ) { return sqrt( dX_real<T,Aa>( a.x[1], a.x[0] ) ); }
    }

    auto ch = std::sqrt( a.x[0] );
    auto cl = ( std::fma( -ch, ch, a.x[0] ) + a.x[1] ) / (2*ch);
    return TX( ch, cl );
  }
  template < typename T, Algorithm Aa >
  inline dX_real<T,Aa> const dX_real<T,Aa>::sqrt ( T const& a ) {
    return sqrt( dX_real<T,Aa>( a ) );
  }


  //
  // f minimum
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto const fmin ( dX_real<T,Aa> const& a, dX_real<T,Ab> const &b )
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
  inline auto const fmin ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, dX_real<T,A> > {
  //
  // Quasi
  //
    using TX = dX_real<T,Algorithm::Accurate>;
    return dX_real<T,A>( fmin( TX(a), TX(b) ) );
  }


  //
  // f maximum
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto const fmax ( dX_real<T,Aa> const& a, dX_real<T,Ab> const &b )
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
  inline auto const fmax ( dX_real<T,Aa> const& a, dX_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, dX_real<T,A> > {
  //
  // Quasi
  //
    using TX = dX_real<T,Algorithm::Accurate>;
    return dX_real<T,A>( fmax( TX(a), TX(b) ) );
  }


  //
  // PRNG
  //
  template < typename T, Algorithm A >
  inline dX_real<T,A> const dX_real<T,A>::rand () {
    using TX = dX_real<T,A>;
    auto constexpr f = fp<T>::one / (1<<16) / (1<<15);
    auto g = f;
    auto r = TX::zero();
    if ( std::is_same<T,double>::value ) {
      auto bits = TX::L * 53;
      for(int i=0; i<bits; i+=31 ) {
        auto b_ = std::rand();
        auto b = T(b_);
        auto c = b * g;
             r = r + TX( c );
             g = g * f;
      }
    }
    if ( std::is_same<T,float>::value ) {
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
  inline std::ostream& operator<< ( std::ostream& stream, dX_real<T,Aa> const& a ) {
    stream << std::to_string(a.x[0])
    << " " << std::to_string(a.x[1]);
    return stream;
  }


}


#endif

