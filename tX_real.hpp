#ifndef	TX_REAL_H
#define	TX_REAL_H

#include "mX_real.hpp"


namespace mX_real {
namespace tX_real {

  template < typename T, Algorithm A=Algorithm::Accurate >
  struct tx_real {

    static_assert( fp<T>::value,
                   "The base type must be out of the defined types, such as float or double." );

    //
    //
    using base_T = T;
    static Algorithm constexpr base_A = A;
    //
    static int constexpr L = 3;
    T x[L];

    //
    // Simple constructor
    //
    inline tx_real() {
      x[0] = x[1] = x[2] = fp<T>::zero;
    }
    inline tx_real( T const& x0, T const& x1, T const& x2 ) {
      x[0] = x0; x[1] = x1; x[2] = x2;
    }
    inline tx_real( int const &h ) {
      x[0] = T(h); x[1] = x[2] = fp<T>::zero;
    }
    inline tx_real( T const *d ) {
      x[0] = d[0]; x[1] = d[1]; x[2] = d[2];
    }

    //
    //
    inline tx_real( T const &h ) {
      x[0] = h; x[1] = x[2] = fp<T>::zero;
    }
    template < Algorithm _A_ >
    inline tx_real( dX_real::dx_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = fp<T>::zero;
      } else {
        twoSum( h.x[0], h.x[1], x[0], x[1] );
        quickSum( x[0], x[1], x[0], x[1] );
	x[2] = fp<T>::zero;
      }
    }
    template < Algorithm _A_ >
    inline tx_real( tx_real<T,_A_> const &h ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2];
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
      } else {
        vecSum<-3>( x );
        VSEB_Sloppy<3,3>( x );
      }
    }
    template < Algorithm _A_ >
    inline tx_real( qX_real::qx_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2];
      } else {
        T f[4] = { h.x[0], h.x[1], h.x[2], h.x[3] };
        vecSum<-4>( f );
        VSEB_Sloppy<3,4>( f );
	x[0] = f[0]; x[1] = f[1]; x[2] = f[2];
      }
    }

    //
    //
    inline tx_real<T,A> &operator=( int const &h ) {
      x[0] = T(h); x[1] = x[2] = fp<T>::zero;
      return *this;
    }
    inline tx_real<T,A> &operator=( T const &h ) {
      x[0] = h; x[1] = x[2] = fp<T>::zero;
      return *this;
    }
    template < Algorithm _A_ >
    inline tx_real<T,A> &operator=( dX_real::dx_real<T,_A_> const &h ) {
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
    inline tx_real<T,A> &operator=( tx_real<T,_A_> const &h ) {
      if ( A == _A_ && this == (tx_real<T,A>*)(&h) ) { return *this; }
      x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2];
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
      } else {
        vecSum<-3>( x );
        VSEB_Sloppy<3,3>( x );
      }
      return *this;
    }
    template < Algorithm _A_ >
    inline tx_real<T,A> &operator=( qX_real::qx_real<T,_A_> const &h ) {
      if ( A == Algorithm::Quasi || _A_ != Algorithm::Quasi ) {
        x[0] = h.x[0]; x[1] = h.x[1]; x[2] = h.x[2];
      } else {
        T f[4] = { h.x[0], h.x[1], h.x[2], h.x[3] };
        vecSum<-4>( f );
        VSEB_Sloppy<3,4>( f );
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
    inline operator dX_real::dx_real<T,_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[3] = { x[0], x[1], x[2] };
        vecSum<-3>( f );
        VSEB_Sloppy<2,3>( f );
	return dX_real::dx_real<T,_A_>( f );
      } else {
        return dX_real::dx_real<T,_A_>( x );
      }
    }
    template < Algorithm _A_ >
    inline operator tx_real<T,_A_>() const {
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[3] = { x[0], x[1], x[2] };
        vecSum<-3>( f );
        VSEB_Sloppy<3,3>( f );
        return tx_real<T,_A_>( f );
      } else {
        return tx_real<T,_A_>( x );
      }
    }
    template < Algorithm _A_ >
    inline operator qX_real::qx_real<T,_A_>() const {
      auto c = qX_real::qx_real<T,_A_>( x[0], x[1], x[2], fp<T>::zero );
      if ( A == Algorithm::Quasi && _A_ != Algorithm::Quasi ) {
        T f[3] = { x[0], x[1], x[2] };
        vecSum<-3>( c.x );
        VSEB_Sloppy<3,3>( c.x );
        return qX_real::qx_real<T,_A_>( f[0], f[1], f[2], fp<T>::zero );
      } else {
        return qX_real::qx_real<T,_A_>( x[0], x[1], x[2], fp<T>::zero );
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
    static inline tx_real<T,A> constexpr zero () { return tx_real<T,A>( fp<T>::zero ); }
    static inline tx_real<T,A> constexpr one  () { return tx_real<T,A>( fp<T>::one ); }
    static inline tx_real<T,A> constexpr two  () { return tx_real<T,A>( fp<T>::two ); }
    static inline tx_real<T,A> constexpr half () { return tx_real<T,A>( fp<T>::half ); }

    static inline tx_real<T,A> constexpr epsilon () {
      T c = fp<T>::epsilon * fp<T>::half; c = (c * c * c) * 2; return tx_real<T,A>( c ); }

    static inline tx_real<T,A> constexpr nan  () { T c = fp<T>::nan; return tx_real<T,A>( c,c,c ); }
    static inline tx_real<T,A> constexpr inf  () { T c = fp<T>::inf; return tx_real<T,A>( c,c,c ); }

    static inline tx_real<T,A> constexpr denorm_min  () {
      T c0 = std::numeric_limits<T>::denorm_min();
      return tx_real<T,A>( c0 );
    }
    static inline tx_real<T,A> constexpr min  () {
      T c0 = (std::numeric_limits<T>::min() * 4) * fp<T>::epsiloni * fp<T>::epsiloni;
      return tx_real<T,A>( c0 );
    }
    static inline tx_real<T,A> constexpr max  () {
      T c0 = std::numeric_limits<T>::max();
      T c1 = c0 * fp<T>::epsilon * fp<T>::half;
      T c2 = c1 * fp<T>::epsilon * fp<T>::half;
      return tx_real<T,A>( c0, c1, c2 );
    }

    static inline tx_real<T,A> const rand ();

  };


  //
  // Alias names
  //
  template < typename T >
  using tX_real_accurate = tx_real<T,Algorithm::Accurate>;
  template < typename T >
  using tX_real_sloppy   = tx_real<T,Algorithm::Sloppy>;
  template < typename T >
  using tX_real_quasi    = tx_real<T,Algorithm::Quasi>;


  //
  // Comparation
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator== ( tx_real<T,Aa> const& a, tx_real<T,Ab> const& b ) {
    if ( A == Algorithm::Quasi ) {
      return b == tx_real<T,Algorithm::Accurate>( a );
    } else {
      return a.x[0] == b.x[0] && a.x[1] == b.x[1] && a.x[2] == b.x[2];
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline bool operator!= ( tx_real<T,Aa> const& a, tx_real<T,Ab> const& b ) {
    return !(a == b);
  }


  //
  // unitary operators
  //
  template < typename T, Algorithm Aa >
  inline auto operator+ ( tx_real<T,Aa> const& a ) {
    using TX = tx_real<T,Aa>;
    TX r = a;
    return r;
  }
  template < typename T, Algorithm Aa >
  inline auto operator- ( tx_real<T,Aa> const& a ) {
    using TX = tx_real<T,Aa>;
    return TX( -a.x[0], -a.x[1], -a.x[2] );
  }


  //
  // Addition
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_add ( tx_real<T,Aa> const& a, tx_real<T,Ab> const& b ) {
    using TX = tx_real<T,A>;
    if ( A != Algorithm::Quasi ) {
    //
    //  Accurate or Sloppy
    // 
    //  TW-add TWSum algorithm
    //  N. Fabiano, J-M Muller, and J. Picot
    //  IEEE Trans. Computers, Vol 68, Issue 11, 2019
    // 
      T f[6];
      vecMerge<3,3>( f, a.x, b.x );
      vecSum_Sloppy<-6>( f );
      VSEB_Sloppy<3,6>( f );
      return TX( f );
    } else {
    //
    // Quasi by Ozaki
    //
      T c[3], e[3];
      twoSum( a.x[0], b.x[0], c[0], e[0] );
      twoSum( a.x[1], b.x[1], c[1], e[1] );
      twoSum( c[1], e[0], c[1], e[2] );
      c[2] = a.x[2] + b.x[2] + e[1] + e[2];
      return TX( c );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_add ( dX_real::dx_real<T,Aa> const& a, tx_real<T,Ab> const& b ) {
    using TX = tx_real<T,A>;
    if ( A != Algorithm::Quasi ) {
    //
    //  Accurate or Sloppy
    // 
      T f[5];
      vecMerge<2,3>( f, a.x, b.x );
      vecSum_Sloppy<-5>( f );
      VSEB_Sloppy<3,5>( f );
      return TX( f );
    } else {
    //
    // Quasi by Ozaki
    //
      T c[3], e[3];
      twoSum( a.x[0], b.x[0], c[0], e[0] );
      twoSum( a.x[1], b.x[1], c[1], e[1] );
      twoSum( c[1], e[0], c[1], e[2] );
      c[2] = b.x[2] + e[1] + e[2];
      return TX( c );
    }
  }
  template < typename T, Algorithm Ab >
  inline auto operator_add ( T const& a, tx_real<T,Ab> const& b ) {
    using TX = tx_real<T,Ab>;
    if ( Ab != Algorithm::Quasi ) {
    //
    //  Accurate or Sloppy
    // 
      T f[4];
      vecMerge<1,3>( f, &a, b.x );
      vecSum_Sloppy<-4>( f );
      VSEB_Sloppy<3,4>( f );
      return TX( f );
    } else {
    //
    // Quasi by Ozaki
    //
      T c[3], e[3];
      twoSum( a.x[0], b.x[0], c[0], e[0] );
      twoSum( b.x[1], e[0], c[1], e[2] );
      c[2] = b.x[2] + e[2];
      return TX( c );
    }
  }
  template < typename T, Algorithm A >
  inline auto operator_add ( T const& a, T const& b ) 
  -> std::enable_if_t< fp<T>::value, tX_real::tx_real<T,A> > {
    using TX = tx_real<T,A>;
    T c[2];
    twoSum( a, b, c[0], c[1] );
    if ( A != Algorithm::Quasi ) { Normalize<2>( c[0], c[1] ); }
    return TX( c[0], c[1], fp<T>::zero );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator+ ( tx_real<T,Aa> const& a, tx_real<T,Ab> const& b ) {
    return tX_real::operator_add( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator+ ( dX_real::dx_real<T,Aa> const& a, tx_real<T,Ab> const& b ) {
    return tX_real::operator_add( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator+ ( tx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return b + a;
  }
  template < typename Ts, typename T, Algorithm Ab >
  inline auto operator+ ( Ts const& a, tx_real<T,Ab> const& b )
  -> std::enable_if_t< std::is_arithmetic<Ts>::value, tX_real::tx_real<T,Ab> > {
    return tX_real::operator_add( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa >
  inline auto operator+ ( tx_real<T,Aa> const& a, Ts const& b ) {
    return b + a;
  }


  //
  // Substruction
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator- ( tx_real<T,Aa> const& a, tx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator- ( dX_real::dx_real<T,Aa> const& a, tx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator- ( tx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename Ts, typename T, Algorithm Ab >
  inline auto operator- ( Ts const& a, tx_real<T,Ab> const& b ) {
    return a + (-b);
  }
  template < typename Ts, typename T, Algorithm Aa >
  inline auto operator- ( tx_real<T,Aa> const& a, Ts const& b ) {
    return a + (-b);
  }


  //
  // Multiplication
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_mul ( tx_real<T,Aa> const& a, tx_real<T,Ab> const& b )
  -> std::enable_if_t< A!=Algorithm::Quasi, tx_real<T,A> > {
  // 
  // Accurate
  // 
  //  TW-mul 3Prod_{3,3}^{fast/acc} algorithm
  //  N. Fabiano, J-M Muller, and J. Picot
  //  IEEE Trans. Computers, Vol 68, Issue 11, 2019
  //
    using TX = tx_real<T,A>;
    T p00, q00;
    T p01, q01;
    T p10, q10;
    twoProdFMA( a.x[0], b.x[0], p00, q00 );
    twoProdFMA( a.x[0], b.x[1], p01, q01 );
    twoProdFMA( a.x[1], b.x[0], p10, q10 );

    T b_[3] = { q00, p01, p10 };
    vecSum<-3>( b_ ); // e, e^2, e^2

    T c   = std::fma( a.x[1], b.x[1], b_[2]);
    T z31 = std::fma( a.x[0], b.x[2], q10 );
    T z32 = std::fma( a.x[2], b.x[0], q01 );

    if ( A == Algorithm::Sloppy ) {
      T z3  = z31 + z32;
      T s3  = c + z3;
      T e[4] = { p00, b_[0], b_[1], s3 };
      vecSum_Sloppy<-4>( e );
      VSEB_Sloppy<2,3>( &e[1] );
      return TX( e );
    } else {
      T z3  = z31 + z32;
      T e[5] = { p00, b_[0], b_[1], c, z3 };
      vecSum<-5>( e );
      VSEB_Sloppy<2,4>( &e[1] );
      return TX( e );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_mul ( tx_real<T,Aa> const& a, tx_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, tx_real<T,A> > {
  //
  // Quasi by Ozaki
  //
    using TX = tx_real<T,A>;
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
  inline auto operator_mul ( dX_real::dx_real<T,Aa> const& a, tx_real<T,Ab> const& b )
  -> std::enable_if_t< A!=Algorithm::Quasi, tx_real<T,A> > {
  // 
  // Accurate
  // Sloppy
  //
    using TX = tx_real<T,A>;
    T p00, q00;
    T p01, q01;
    T p10, q10;
    twoProdFMA( a.x[0], b.x[0], p00, q00 );
    twoProdFMA( a.x[0], b.x[1], p01, q01 );
    twoProdFMA( a.x[1], b.x[0], p10, q10 );

    T b_[3] = { q00, p01, p10 };
    vecSum<-3>( b_ ); // e, e^2, e^2

    T c   = std::fma( a.x[1], b.x[1], b_[2]);
    T z31 = std::fma( a.x[0], b.x[2], q10 );
    T z32 = q01;

    if ( A == Algorithm::Sloppy ) {
      T z3  = z31 + z32;
      T s3  = c + z3;
      T e[4] = { p00, b_[0], b_[1], s3 };
      vecSum_Sloppy<-4>( e );
      VSEB_Sloppy<2,3>( &e[1] );
      return TX( e );
    } else {
      T z3  = z31 + z32;
      T e[5] = { p00, b_[0], b_[1], c, z3 };
      vecSum<-5>( e );
      VSEB_Sloppy<2,4>( &e[1] );
      return TX( e );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_mul ( dX_real::dx_real<T,Aa> const& a, tx_real<T,Ab> const& b )
  -> std::enable_if_t< A==Algorithm::Quasi, tx_real<T,A> > {
  //
  // Quasi by Ozaki
  //
    using TX = tx_real<T,A>;
    T e1, e2, e3, e4, e5;
    T t2, t3;
    TX c;
    twoProdFMA( a.x[0], b.x[0], c.x[0], e1 );
    twoProdFMA( a.x[0], b.x[1], t2, e2 );
    twoProdFMA( a.x[1], b.x[0], t3, e3 );
    twoSum( t2, t3, c.x[1], e4 );
    twoSum( c.x[1], e1, c.x[1], e5 );
    c.x[2] = e2 + std::fma( a.x[2], b.x[0], e3 )
           + std::fma( a.x[1], b.x[1], e4 )
           + e5;
    return c;
  }
  template < typename T, Algorithm Ab >
  inline auto operator_mul ( T const& a, tx_real<T,Ab> const& b )
  -> std::enable_if_t< Ab!=Algorithm::Quasi, tx_real<T,Ab> > {
  //
  // Sloppy
  // Accurate
  // 
  //  based-on TW-mul 3Prod_{3,3}^{fast/acc} algorithm
  //  N. Fabiano, J-M Muller, and J. Picot
  //  IEEE Trans. Computers, Vol 68, Issue 11, 2019
  //
    using TX = tx_real<T,Ab>;
    T p00, q00;
    T p01, q01;
    twoProdFMA( a, b.x[0], p00, q00 );
    twoProdFMA( a, b.x[1], p01, q01 );

    T b_[2] = { q00, p01 };
    twoSum( b_[0], b_[1], b_[0], b_[1] );

    T s3 = std::fma( a, b.x[2], q01 );

    T e[4] = { p00, b_[0], b_[1], s3 };
    if ( Ab == Algorithm::Sloppy ) {
      vecSum_Sloppy<-4>( e );
    } else {
      vecSum<-4>( e );
    }
    VSEB_Sloppy<2,3>( &e[1] );

    return TX( e );
  }
  template < typename T, Algorithm Ab >
  inline auto operator_mul ( T const& a, tx_real<T,Ab> const& b )
  -> std::enable_if_t< Ab==Algorithm::Quasi, tx_real<T,Ab> > {
  // 
  // Quasi by Ozaki
  // 
    using TX = tx_real<T,Ab>;
    T e1, e2, e4, e5, t2;
    T c[3];
    twoProdFMA( a, b.x[0], c[0], e1 );
    twoProdFMA( a, b.x[1], t2, e2 );
    twoSum( t2, e1, c[1], e5 );
    c[2] = e2 + std::fma( a, b.x[2], e5 );
    return TX( c );
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_mul ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) 
  -> std::enable_if_t< A!=Algorithm::Quasi, tx_real<T,A> > {
    using TX = tx_real<T,A>;
    T p00, q00;
    T p01, q01;
    T p10, q10;
    twoProdFMA( a.x[0], b.x[0], p00, q00 );
    twoProdFMA( a.x[0], b.x[1], p01, q01 );
    twoProdFMA( a.x[1], b.x[0], p10, q10 );

    T b_[3] = { q00, p01, p10 };
    vecSum<-3>( b_ ); // e, e^2, e^2

    T c   = std::fma( a.x[1], b.x[1], b_[2]);
    T z31 = q10;
    T z32 = q01;

    if ( A == Algorithm::Sloppy ) {
      T z3  = z31 + z32;
      T s3  = c + z3;
      T e[4] = { p00, b_[0], b_[1], s3 };
      vecSum_Sloppy<-4>( e );
      VSEB_Sloppy<2,3>( &e[1] );
      return TX( e );
    } else {
      T z3  = z31 + z32;
      T e[5] = { p00, b_[0], b_[1], c, z3 };
      vecSum<-5>( e );
      VSEB_Sloppy<2,4>( &e[1] );
      return TX( e );
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_mul ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) 
  -> std::enable_if_t< A==Algorithm::Quasi, tx_real<T,A> > {
  //
  // Quasi by Ozaki
  //
    using TX = tx_real<T,A>;
    T e1, e2, e3, e4, e5;
    T t2, t3;
    TX c;
    twoProdFMA( a.x[0], b.x[0], c.x[0], e1 );
    twoProdFMA( a.x[0], b.x[1], t2, e2 );
    twoProdFMA( a.x[1], b.x[0], t3, e3 );
    twoSum( t2, t3, c.x[1], e4 );
    twoSum( c.x[1], e1, c.x[1], e5 );
    c.x[2] = e2
           + std::fma( a.x[1], b.x[1], e3 )
           + e4 + e5;
    return c;
  }
  template < typename T, Algorithm A >
  inline auto operator_mul ( T const& a, T const& b )
  -> std::enable_if_t< fp<T>::value, tx_real<T,A> > {
    T c[2];
    twoProdFMA( a, b, c[0], c[1] );
    return tx_real<T,A>( c[0], c[1], fp<T>::zero );
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator* ( tx_real<T,Aa> const& a, tx_real<T,Ab> const& b ) {
    return tX_real::operator_mul( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator* ( dX_real::dx_real<T,Aa> const& a, tx_real<T,Ab> const& b ) {
    return tX_real::operator_mul( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator* ( tx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return b * a;
  }
  template < typename Ts, typename T, Algorithm Ab >
  inline auto operator* ( Ts const& a, tx_real<T,Ab> const& b )
  -> std::enable_if_t< std::is_arithmetic<Ts>::value, tx_real<T,Ab> > {
    return tX_real::operator_mul( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa >
  inline auto operator* ( tx_real<T,Aa> const& a, Ts const& b ) {
    return b * a;
  }


  //
  // Division
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto operator_div ( tx_real<T,Aa> const& a, tx_real<T,Ab> const& b ) {
    using TX = tx_real<T,A>;
    if ( A != Algorithm::Quasi ) {
      {
        if ( b.x[0] == fp<T>::zero ) { return TX::inf(); }
        if ( b.x[0] == -fp<T>::zero ) { return -TX::inf(); }
      }
      //
      // Accurate or Sloppy
      //
      //  TW-div 3Div algorithm
      //  N. Fabiano, J-M Muller, and J. Picot
      //  IEEE Trans. Computers, Vol 68, Issue 11, 2019
      //
      T _2u    = fp<T>::epsilon;
      T one_2u = fp<T>::one + _2u;
      T alpha  = one_2u / b.x[0];
      T h11    = std::fma( alpha, b.x[0], -one_2u );
      T h1     = std::fma( alpha, b.x[1], h11 );
      //
      T b01, b11;
      twoProdFMA( alpha, fp<T>::one - _2u, b01, b11 );
      T b12 = std::fma( alpha, -h1, b11 );
      TX b_inv;
      quickSum( b01, b12, b_inv.x[0], b_inv.x[1] );
      //
      return (a * b_inv) * (TX::two() - (b * b_inv));
    } else {
      auto s = b.x[0] + b.x[1];
      {
        if ( s + b.x[2] == fp<T>::zero ) {
          s += b.x[2]; auto c = std::copysign( fp<T>::inf, s ); return TX( c,c,c );
        }
        if ( b.x[0] == fp<T>::zero ) {
          return tX_real::operator_div( a, tx_real<T,A>( b.x[1], b.x[2], b.x[0] ) );
        }
      }
      //
      // Quasi by Ozaki
      //
      auto c = dX_real::dx_real<T,A>( a ) / dX_real::dx_real<T,A>( b );
      auto t = a - c * b;
      auto r = TX( c );
      r.x[2] = (t.x[0] + t.x[1] + t.x[2]) / s;
      return r;
    }
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline tx_real<T,A> operator_div ( dX_real::dx_real<T,Aa> const& a, tx_real<T,Ab> const& b ) {
    using TX = tx_real<T,A>;
    return TX(a) / b;
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline tx_real<T,A> operator_div ( tx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = tx_real<T,A>;
    return a / TX( b );
  }
  template < typename T, Algorithm Ab >
  inline tx_real<T,Ab> operator_div ( T const& a, tx_real<T,Ab> const& b ) {
    using TX = tx_real<T,Ab>;
    return TX(a) / b;
  }
  template < typename T, Algorithm Aa >
  inline tx_real<T,Aa> operator_div ( tx_real<T,Aa> const& a, T const& b ) {
    using TX = tx_real<T,Aa>;
    return a / TX(b);
  }
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline tx_real<T,A> operator_div ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    using TX = tx_real<T,A>;
    return TX(a) / TX(b);
  }
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator/ ( tx_real<T,Aa> const& a, tx_real<T,Ab> const& b ) {
    return tX_real::operator_div( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  inline auto operator/ ( dX_real::dx_real<T,Aa> const& a, tx_real<T,Ab> const& b ) {
    return tX_real::operator_div( a, b );
  }
  template < typename T, Algorithm Aa, Algorithm Ab  >
  inline auto operator/ ( tx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
    return tX_real::operator_div( a, b );
  }
  template < typename Ts, typename T, Algorithm Ab >
  inline auto operator/ ( Ts const& a, tx_real<T,Ab> const& b )
  -> std::enable_if_t< std::is_arithmetic<Ts>::value, dX_real::dx_real<T,Ab> > {
    return tX_real::operator_div( T(a), b );
  }
  template < typename Ts, typename T, Algorithm Aa >
  inline auto operator/ ( tx_real<T,Aa> const& a, Ts const& b ) 
  -> std::enable_if_t< std::is_arithmetic<Ts>::value, dX_real::dx_real<T,Aa> > {
    return tX_real::operator_div( a, T(b) );
  }


  //
  //
  template < typename T, Algorithm Aa >
  inline bool isnan ( tx_real<T,Aa> const& a ) {
    auto s = a.x[0];
    if ( Aa == Algorithm::Quasi ) { s += a.x[1] + a.x[2]; }
    return std::isnan( s );
  }
  template < typename T, Algorithm Aa >
  inline bool signbit ( tx_real<T,Aa> const& a ) {
    auto s = a.x[0];
    if ( Aa == Algorithm::Quasi ) { s += a.x[1] + a.x[2]; }
    return std::signbit( s );
  }


  //
  // Absolute value
  //
  template < typename T, Algorithm Aa >
  inline tx_real<T,Aa> abs ( tx_real<T,Aa> const& a ) {
    auto s = a.x[0];
    if ( Aa == Algorithm::Quasi ) { s += a.x[1] + a.x[2]; }
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
  inline auto sqrt ( tx_real<T,Aa> const& a ) {
    using TX = tx_real<T,Aa>;
    {
      auto s = a.x[0];
      if ( Aa == Algorithm::Quasi ) { s += a.x[1] + a.x[2]; }
      if ( s == fp<T>::zero ) { return a; }
      if ( s < fp<T>::zero ) { return TX::nan(); }
      if ( Aa == Algorithm::Quasi ) {
        if ( a.x[0] == fp<T>::zero ) { return tX_real::sqrt( tx_real<T,Aa>( a.x[1], a.x[2], a.x[0] ) ); }
      }
    }
    if ( Aa != Algorithm::Quasi ) {
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
      // r = 1/approx.sqrt(a0)[=as] = e/approx.sqrt(a0')
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

    } else {
    //
    // Quasi by Ozaki
    //
      auto r = dX_real::sqrt( dX_real::dx_real<T,Aa>( a ) );
      auto t = a - tX_real::operator_mul( r, r );
      auto c = TX( r );
      c.x[2] = (t.x[0] + t.x[1] + t.x[2]) / (2*(c.x[0] + c.x[1]));
      return c;
    }
  }
  template < typename T, Algorithm Aa >
  inline tx_real<T,Aa> const sqrt ( dX_real::dx_real<T,Aa> const& a ) {
    return tX_real::sqrt( tx_real<T,Aa>( a ) );
  }
  template < typename T, Algorithm Aa=Algorithm::Accurate >
  inline auto const sqrt ( T const& a )
  -> std::enable_if_t< fp<T>::value, tx_real<T,Aa> > {
    return tX_real::sqrt( tx_real<T,Aa>( a ) );
  }


  //
  // f minimum
  //
  template < typename T, Algorithm Aa, Algorithm Ab, Algorithm A=commonAlgorithm<Aa,Ab>::algorithm >
  inline auto fmin ( tx_real<T,Aa> const& a, tx_real<T,Ab> const &b ) {
    using TX = tx_real<T,A>;
    if ( A == Algorithm::Quasi ) {
      using T_ = tx_real<T,Algorithm::Accurate>;
      return TX( tX_real::fmin( T_(a), T_(b) ) );
    } else {
      int i=0; for(i;i<3-1;i++) {
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
  inline auto fmax ( tx_real<T,Aa> const& a, tx_real<T,Ab> const &b ) {
    using TX = tx_real<T,A>;
    if ( A == Algorithm::Quasi ) {
      using T_ = tx_real<T,Algorithm::Accurate>;
      return TX( tX_real::fmax( T_(a), T_(b) ) );
    } else {
      int i=0; for(i;i<3-1;i++) {
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
  inline auto const rand ()
  -> std::enable_if_t< std::is_same<T,double>::value, tx_real<T,A> > {
    using TX = tx_real<T,A>;
    auto constexpr f = fp<T>::one / (1<<16) / (1<<15);
    auto g = f;
    auto r = TX::zero();
    auto bits = TX::L * 53;
    for(int i=0; i<bits; i+=31 ) {
      auto b_ = std::rand();
      auto b = T (b_ );
      auto c = b * g;
           r = r + TX( c );
           g = g * f;
    }
    return r;
  }
  template < typename T, Algorithm A >
  inline auto const rand () 
  -> std::enable_if_t< std::is_same<T,float>::value, tx_real<T,A> > {
    using TX = tx_real<T,A>;
    auto constexpr f = fp<T>::one / (1<<16) / (1<<15);
    auto g = f;
    auto r = TX::zero();
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
    return r;
  }
  template < typename T, Algorithm A >
  inline tx_real<T,A> const tx_real<T,A>::rand () {
    return tX_real::rand<T,A>();
  }


  //
  //
  template < typename T, Algorithm Aa >
  inline std::ostream& operator<< ( std::ostream& stream, tx_real<T,Aa> const& a ) {
    stream << std::to_string(a.x[0]) << " "
           << std::to_string(a.x[1]) << " "
           << std::to_string(a.x[2]);
    return stream;
  }

}
}


#endif

