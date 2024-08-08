#ifndef	TX_REAL_H
#define	TX_REAL_H

#include "mX_real.hpp"


namespace mX_real {
  namespace tX_real {

    template < typename T, Algorithm A=Algorithm::Accurate >
    struct tx_real {

      static_assert( fp<T>::value,
                     "The base type must be a pre-defined type such as float or double." );

      //
      //
      //
      using base_T = T;
      static Algorithm constexpr base_A = A;

      //
      // {DX,TX,QX}_REAL are shortcut type only within tx_real
      //
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
      //
    public:
      static int constexpr L = 3;
      T x[L];

      //
      //
      //
      template < Algorithm _A_ >
      using type_with_Algorithm = TX_REAL<_A_>;
      //
      using typeAccurate   = type_with_Algorithm<Algorithm::Accurate>;
      using typeSloppy   = type_with_Algorithm<Algorithm::Sloppy>;
      using typeQuasi   = type_with_Algorithm<Algorithm::Quasi>;
      //
      using accurateType   = type_with_Algorithm<accurateAlgorithm<A>::algorithm>;
      using inaccurateType = type_with_Algorithm<inaccurateAlgorithm<A>::algorithm>;
      using narrowerType   = typename std::conditional_t< 3>=4, tX_real::tx_real<T,A>, dX_real::dx_real<T,A> >;
      using widerType      = typename std::conditional_t< 3==2, tX_real::tx_real<T,A>, qX_real::qx_real<T,A> >;


      //
      // A special member function utilized in constructors
      //
      //
      INLINE T constexpr quick_Normalized () const NOEXCEPT {
        T s = x[0];
        if ( A == Algorithm::Quasi ) for(int i=1; i<L; i++) { s += x[i]; }
        return s;
      }
      INLINE TX_REAL<> constexpr element_rotate () const NOEXCEPT {
        T y[L]; for(int i=0; i<L; i++) { y[i] = x[i]; }
        for(int i=0; i<L-1; i++) {
          if ( ! fp<T>::is_zero( y[0] ) ) { return TX_REAL<>( y ); }
          T t = y[0]; for(int j=0; j<L-1; j++) { y[j] = y[j+1]; } y[L-1] = t;
        }
        return TX_REAL<>( y );
      }


      //
      // Simple constructors
      //
      //
      INLINE tx_real() NOEXCEPT {
        for(int i=0; i<L; i++) { x[i] = fp<T>::zero(); }
      }
      INLINE tx_real( T const& x0, T const& x1, T const& x2 ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
        auto t = x0 + x1 + x2;
        if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
          for(int i=0; i<L; i++) { x[i] = t; }
        } else
#endif
          { x[0] = x0; x[1] = x1; x[2] = x2; }
      }
      INLINE tx_real( T const &h ) NOEXCEPT {
        { x[0] = h; for(int i=1; i<L; i++) { x[i] = fp<T>::zero(); } }
      }
      template < typename Ts, T_scalar(Ts), T_neq_Ts(T,Ts) >
      INLINE tx_real( Ts const &h ) NOEXCEPT {
        { x[0] = T(h); for(int i=1; i<L; i++) { x[i] = fp<T>::zero(); } }
      }
      INLINE tx_real( T const *d ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
        auto t = d[0]; for(int i=1; i<L; i++) { t += d[1]; }
        if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
          for(int i=0; i<L; i++) { x[i] = t; }
        } else
#endif
          for(int i=0; i<L; i++) { x[i] = d[i]; }
      }


      //
      // Copy constructor with (or without) any Algorithm switch
      //
      //
      template < template < typename __T__, Algorithm __A__ > class _TX_, Algorithm _A_, T_mX(_TX_<T,_A_>) >
      INLINE tx_real( _TX_<T,_A_> const& h ) NOEXCEPT {
        using TX = _TX_<T,_A_>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
        auto const t = h.quick_Normalized();
        if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
          for(int i=0; i<L; i++) { x[i] = t; }
        } else
#endif
          {
            // rewritten in the copya and normalization process
            int constexpr LL = TX::L;
            bool constexpr need_normalize =
              ( A != Algorithm::Quasi && _A_ == Algorithm::Quasi );
            if ( L < LL ) {
              if ( _A_ == Algorithm::Quasi ) {
                auto s = h;
                mX_real::Normalize<1>( s );
                mX_real::copy_with_rounding( x, s.x, L, LL );
              } else {
                mX_real::copy_with_rounding( x, h.x, L, LL );
              }
              if ( need_normalize ) { mX_real::Normalize<0>( *this ); }
            } else {
              mX_real::copy_with_rounding( x, h.x, L, LL );
              if ( need_normalize ) { mX_real::Normalize<1>( *this ); }
            }
          }
      }


      //
      // Copy-assignment operator
      //
      //
      INLINE TX_REAL<> constexpr &operator=( T const& h )& NOEXCEPT {
        x[0] = h; for(int i=1; i<L; i++) { x[i] = fp<T>::zero(); }
        return *this;
      }
      template < typename Ts, T_scalar(Ts), T_neq_Ts(T,Ts) >
      INLINE TX_REAL<> constexpr &operator=( Ts const& h )& NOEXCEPT {
        x[0] = T(h); for(int i=1; i<L; i++) { x[i] = fp<T>::zero(); }
        return *this;
      }
      template < template < typename __T__, Algorithm __A__ > class _TX_, Algorithm _A_, T_mX(_TX_<T,_A_>) >
      INLINE TX_REAL<> constexpr &operator=( _TX_<T,_A_> const& h )& NOEXCEPT {
        if ( std::is_same< TX_REAL<A>, _TX_<T,_A_> >::value &&
             this == (TX_REAL<>*)(&h) ) { return *this; }
#if MX_REAL_USE_INF_NAN_EXCEPTION
        auto const t = h.quick_Normalized();
        if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
          for(int i=0; i<L; i++) { x[i] = t; }
        } else
#endif
          {
            // rewritten in the copya and normalization process
            int constexpr LL = _TX_<T,_A_>::L;
            bool constexpr need_normalize = 
              ( A != Algorithm::Quasi && _A_ == Algorithm::Quasi );
            if ( L < LL ) {
              if ( _A_ == Algorithm::Quasi ) {
                auto s = h;
                mX_real::Normalize<1>( s );
                mX_real::copy_with_rounding( x, s.x, L, LL );
              } else {
                mX_real::copy_with_rounding( x, h.x, L, LL );
              }
              if ( need_normalize ) { mX_real::Normalize<0>( *this ); }
            } else {
              mX_real::copy_with_rounding( x, h.x, L, LL );
              if ( need_normalize ) { mX_real::Normalize<1>( *this ); }
            }
          }
        return *this;
      }


      //
      // Casting to other datatypes
      //
      //
      explicit
      INLINE operator T() const NOEXCEPT {
        if ( A == Algorithm::Quasi ) {
          return this->quick_Normalized();
        } else {
          return x[0];
        }
      }
      template < typename Ts, T_fp(Ts), T_neq_Ts(T,Ts) >
      explicit
      INLINE operator Ts() const NOEXCEPT {
        if ( A == Algorithm::Quasi ) {
          return Ts(this->quick_Normalized());
        } else {
          return Ts(x[0]);
        }
      }
      template < template < typename __T__, Algorithm __A__ > class _TX_, Algorithm _A_, T_mX(_TX_<T,_A_>) >
      explicit
      INLINE operator _TX_<T,_A_>() const NOEXCEPT {
        using TX = _TX_<T,_A_>;
        TX c;
        int constexpr LL = TX::L;
#if MX_REAL_USE_INF_NAN_EXCEPTION
        auto const t = this->quick_Normalized();
        if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
          for(int i=0; i<LL; i++) { c.x[i] = t; }
        } else
#endif
          {
            // rewritten in the copya and normalization process
            bool constexpr need_normalize = 
              ( _A_ != Algorithm::Quasi && A == Algorithm::Quasi );
            if ( LL < L ) {
              if ( A == Algorithm::Quasi ) {
                auto s = *this;
                mX_real::Normalize<1>( s );
                mX_real::copy_with_rounding( c.x, s.x, LL, L );
              } else {
                mX_real::copy_with_rounding( c.x, x, LL, L );
              }
              if ( need_normalize ) { mX_real::Normalize<0>( c ); }
            } else {
              mX_real::copy_with_rounding( c.x, x, LL, L );
              if ( need_normalize ) { mX_real::Normalize<1>( c ); }
            }
          }
        return c;
      }


      //
      // unneccessary operators are invalidaded
      //
      //
      auto operator! () = delete;
      auto operator~ () = delete;
      auto operator++ () = delete;
      auto operator-- () = delete;
      template < typename _T_ > auto operator&&  ( _T_ a ) = delete;
      template < typename _T_ > auto operator||  ( _T_ a ) = delete;
      template < typename _T_ > auto operator&=  ( _T_ a ) = delete;
      template < typename _T_ > auto operator^=  ( _T_ a ) = delete;
      template < typename _T_ > auto operator<<=  ( _T_ a ) = delete;
      template < typename _T_ > auto operator>>=  ( _T_ a ) = delete;
      template < typename _T_ > auto operator,  ( _T_ a ) = delete;
      template < typename _T_ > auto operator[]  ( _T_ a ) = delete;
      auto operator() (...) = delete;


      //
      // static constexpr member functions or constatns
      // contents are always in Accurate format
      // but they follow in the Algorithm argument 'A'
      //
      //
      static INLINE TX_REAL<> constexpr zero () NOEXCEPT { return TX_REAL<>{ fp<T>::zero() }; }
      static INLINE TX_REAL<> constexpr one () NOEXCEPT { return TX_REAL<>{ fp<T>::one() }; }
      static INLINE TX_REAL<> constexpr two () NOEXCEPT { return TX_REAL<>{ fp<T>::two() }; }
      static INLINE TX_REAL<> constexpr nhalf () NOEXCEPT { return TX_REAL<>{ fp<T>::nhalf() }; }
      static INLINE TX_REAL<> constexpr nan () NOEXCEPT { T c = fp<T>::nan(); return TX_REAL<>{ c, c, c }; }
      static INLINE TX_REAL<> constexpr inf () NOEXCEPT { T c = fp<T>::inf(); return TX_REAL<>{ c, c, c }; }
      static INLINE TX_REAL<> constexpr epsilon () NOEXCEPT {
        T c = fp<T>::epsilon() * fp<T>::nhalf(); c = (c * c* c) * 2;
        return TX_REAL<>{ c };
      }
      static INLINE TX_REAL<> constexpr denorm_min  () NOEXCEPT {
        T c = fp<T>::denorm_min();
        return TX_REAL<>{ c };
      }
      static INLINE TX_REAL<> constexpr min  () NOEXCEPT {
        T c = fp<T>::min();
        for(int i=1; i<L; i++) c = c * ( fp<T>::epsiloni() * 2 );
        return TX_REAL<>{ c };
      }
      static INLINE TX_REAL<> constexpr safe_min  () NOEXCEPT {
        T c = fp<T>::min();
        for(int i=1; i<L; i++) c = c * ( fp<T>::epsiloni() * 2 );
        c = std::sqrt( c );
        return TX_REAL<>{ c };
      }
      static INLINE TX_REAL<> constexpr critical_min  () NOEXCEPT {
        T c = fp<T>::denorm_min();
        for(int i=1; i<L; i++) c = c * ( fp<T>::epsiloni() * 2 );
        c = std::sqrt( c );
        return TX_REAL<>{ c };
      }
      static INLINE TX_REAL<> constexpr max  () NOEXCEPT {
        T c[L];
        c[0] = fp<T>::max();
        for(int i=1; i<L; i++) c[i] = c[i-1] * ( fp<T>::epsilon() * fp<T>::nhalf() );
        return TX_REAL<>{ c };
      }


      //
      // static member funtions
      // definition is below outside of the struct definition block
      //
      static INLINE TX_REAL<> constexpr sqrt ( TX_REAL<> const& a ) NOEXCEPT;
      static INLINE TX_REAL<> constexpr abs ( TX_REAL<> const& a ) NOEXCEPT;
      static INLINE TX_REAL<> constexpr fabs ( TX_REAL<> const& a ) NOEXCEPT {
        return TX_REAL<>::abs(a);
      }
      static INLINE TX_REAL<> rand () NOEXCEPT;
      static INLINE bool constexpr is_positive ( TX_REAL<> const& a ) NOEXCEPT;
      static INLINE bool constexpr is_negative ( TX_REAL<> const& a ) NOEXCEPT;
      static INLINE bool constexpr isnan ( TX_REAL<> const& a ) NOEXCEPT;
      static INLINE bool constexpr is_zero ( TX_REAL<> const& a ) NOEXCEPT;
      static INLINE bool constexpr signbit ( TX_REAL<> const& a ) NOEXCEPT;
      static INLINE bool constexpr isinf ( TX_REAL<> const& a ) NOEXCEPT;
      //


      //
      // member functions (aka methods)
      //
      //
      INLINE void constexpr Normalize () NOEXCEPT { mX_real::Normalize( *this ); }
      //
      INLINE bool constexpr is_positive () const NOEXCEPT { return TX_REAL<>::is_positive( *this ); }
      INLINE bool constexpr is_negative () const NOEXCEPT { return TX_REAL<>::is_negative( *this ); }
      INLINE bool constexpr isnan () const NOEXCEPT { return TX_REAL<>::isnan( *this ); }
      INLINE bool constexpr is_zero () const NOEXCEPT { return TX_REAL<>::is_zero( *this ); }
      INLINE bool constexpr signbit () const NOEXCEPT { return TX_REAL<>::signbit( *this ); }
      INLINE bool constexpr isinf () const NOEXCEPT { return TX_REAL<>::isinf( *this ); }
      //


      //  Friend
      template < typename _T_, Algorithm _A_ >
      friend std::ostream& operator<< ( std::ostream& stream, tX_real::tx_real<_T_,_A_> const& a );

    };


    // 
    // get a new type tX_real<T,A> from TX(={m}X_real<T,A>)
    //
    //
    template < typename TX, T_mX(TX) >
    struct mX_real_impl {
      static Algorithm constexpr A = TX::base_A;
      using T    = typename TX::base_T;
      using type = typename tX_real::tx_real<T,A>;
    };
    template < typename TX >
    using mX_real = typename mX_real_impl<TX>::type;
    //
    //
    //
    template < typename TX, Algorithm A, T_mX(TX) >
    struct type_with_Algorithm_impl {
      using type = typename tX_real::tx_real<typename TX::base_T,A>;
    };
    template < typename TX, Algorithm A >
    using type_with_Algorithm = typename type_with_Algorithm_impl<TX,A>::type;
    //
    template < typename TX >
    using type_Accurate = type_with_Algorithm_impl<TX, Algorithm::Accurate>;
    template < typename TX >
    using type_Sloppy   = type_with_Algorithm_impl<TX, Algorithm::Sloppy>;
    template < typename TX >
    using type_Quasi    = type_with_Algorithm_impl<TX, Algorithm::Quasi>;


    //
    // Alias names
    //
    //
    template < typename T >
    using tX_real_accurate = tX_real::tx_real<T,Algorithm::Accurate>;
    template < typename T >
    using tX_real_sloppy   = tX_real::tx_real<T,Algorithm::Sloppy>;
    template < typename T >
    using tX_real_quasi    = tX_real::tx_real<T,Algorithm::Quasi>;


    //
    // Bitwise or FP-bit-specific ops
    //
    //
    template < typename T, Algorithm Aa >
    INLINE auto constexpr signbit ( tX_real::tx_real<T,Aa> const& a ) NOEXCEPT {
      return fp<T>::signbit( a.quick_Normalized() );
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr isnan ( tX_real::tx_real<T,Aa> const& a ) NOEXCEPT {
      return fp<T>::isnan( a.quick_Normalized() );
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr isinf ( tX_real::tx_real<T,Aa> const& a ) NOEXCEPT {
      return fp<T>::isinf( a.quick_Normalized() );
    }
    template < typename T, Algorithm Aa >
    INLINE bool constexpr is_negative ( tX_real::tx_real<T,Aa> const& a ) NOEXCEPT {
      return a.quick_Normalized() < fp<T>::zero();
    }
    template < typename T, Algorithm Aa >
    INLINE bool constexpr is_positive ( tX_real::tx_real<T,Aa> const& a ) NOEXCEPT {
      return a.quick_Normalized() > fp<T>::zero();
    }
    template < typename T, Algorithm Aa >
    INLINE bool constexpr is_zero ( tX_real::tx_real<T,Aa> const& a ) NOEXCEPT {
      return a.quick_Normalized() == fp<T>::zero();
    }
    //
    template < typename T, Algorithm A >
    INLINE bool constexpr tX_real::tx_real<T,A>::isinf ( tX_real::tx_real<T,A> const& a ) NOEXCEPT {
      return tX_real::isinf ( a );
    }
    template < typename T, Algorithm A >
    INLINE bool constexpr tX_real::tx_real<T,A>::isnan ( tX_real::tx_real<T,A> const& a ) NOEXCEPT {
      return tX_real::isnan ( a );
    }
    template < typename T, Algorithm A >
    INLINE bool constexpr tX_real::tx_real<T,A>::signbit ( tX_real::tx_real<T,A> const& a ) NOEXCEPT {
      return tX_real::signbit ( a );
    }
    template < typename T, Algorithm A >
    INLINE bool constexpr tX_real::tx_real<T,A>::is_zero ( tX_real::tx_real<T,A> const& a ) NOEXCEPT {
      return tX_real::is_zero ( a );
    }
    template < typename T, Algorithm A >
    INLINE bool constexpr tX_real::tx_real<T,A>::is_positive ( tX_real::tx_real<T,A> const& a ) NOEXCEPT {
      return tX_real::is_positive ( a );
    }
    template < typename T, Algorithm A >
    INLINE bool constexpr tX_real::tx_real<T,A>::is_negative ( tX_real::tx_real<T,A> const& a ) NOEXCEPT {
      return tX_real::is_negative ( a );
    }
    //

    //
    // Comparison, equivalence (==)
    //
    //
    template < typename T, Algorithm A >
    INLINE auto constexpr operator_eq ( tX_real::tx_real<T,A> const& a, tX_real::tx_real<T,A> const& b ) NOEXCEPT {
      using TX = tX_real::tx_real<T,A>;
      int i=0; for(i=0; i<TX::L-1; i++) {
        if ( a.x[i] != b.x[i] ) { return a.x[i] == b.x[i]; }
      } return a.x[i] == b.x[i];
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator== ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
      Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
      if ( A == Algorithm::Quasi ) {
        using TT = tX_real::tx_real<T,Algorithm::Accurate>;
        return tX_real::operator_eq( TT{ a }, TT{ b } );
      } else {
        return tX_real::operator_eq( a, b );
      }
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator!= ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
      return !(a == b);
    }

    //
    // Comparison, greater than (>)
    //
    //
    template < typename T, Algorithm A >
    INLINE auto constexpr operator_gt ( tX_real::tx_real<T,A> const& a, tX_real::tx_real<T,A> const& b ) NOEXCEPT {
      using TX = tX_real::tx_real<T,A>;
      int i=0; for(i=0; i<TX::L-1; i++) {
        if ( a.x[i] != b.x[i] ) { return a.x[i] > b.x[i]; }
      } return a.x[i] > b.x[i];
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator> ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
      Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
      if ( A == Algorithm::Quasi ) {
        using TT = tX_real::tx_real<T,Algorithm::Accurate>;
        return tX_real::operator_gt( TT{ a }, TT{ b } );
      } else {
        return tX_real::operator_gt( a, b );
      }
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator<= ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
      return !(a > b);
    }

    //
    // Comparison, less than (<)
    //
    //
    template < typename T, Algorithm A >
    INLINE auto constexpr operator_lt ( tX_real::tx_real<T,A> const& a, tX_real::tx_real<T,A> const& b ) NOEXCEPT {
      using TX = tX_real::tx_real<T,A>;
      int i=0; for(i=0; i<TX::L-1; i++) {
        if ( a.x[i] != b.x[i] ) { return a.x[i] < b.x[i]; }
      } return a.x[i] < b.x[i];
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator< ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
      Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
      if ( A == Algorithm::Quasi ) {
        using TT = tX_real::tx_real<T,Algorithm::Accurate>;
        return tX_real::operator_lt( TT{ a }, TT{ b } );
      } else {
        return tX_real::operator_lt( a, b );
      }
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator>= ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
      return !(a < b);
    }

    //
    // Unitary operators
    //
    //
    template < typename T, Algorithm Aa >
    INLINE auto constexpr reversed_sign ( tX_real::tx_real<T,Aa> const& a ) NOEXCEPT {
      return tX_real::tx_real<T,Aa>( -a.x[0],-a.x[1],-a.x[2] );
    }
    //
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator+ ( tX_real::tx_real<T,Aa> const& a ) NOEXCEPT {
      return a;
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator- ( tX_real::tx_real<T,Aa> const& a ) NOEXCEPT {
      return tX_real::reversed_sign( a );
    }
    //

    //
    // Addition
    //
    //
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) NOEXCEPT {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = tX_real::tx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::add_QW_QW_TW ( _QX_(a), _QX_(b), _TX_(c) );
      } else {
        QxW::add_QQW_QQW_QTW ( _QX_(a), _QX_(b), _TX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( qX_real::qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = tX_real::tx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::add_QW_TW_TW ( _QX_(a), _TX_(b), _TX_(c) );
      } else {
        QxW::add_QQW_QTW_QTW ( _QX_(a), _TX_(b), _TX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( qX_real::qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) NOEXCEPT {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = tX_real::tx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::add_QW_DW_TW ( _QX_(a), _DX_(b), _TX_(c) );
      } else {
        QxW::add_QQW_PA_QTW ( _QX_(a), _DX_(b), _TX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator_add_body ( qX_real::qx_real<T,Aa> const& a, T const& b ) NOEXCEPT {
      using TX = tX_real::tx_real<T,Aa>;
      TX c;
      if ( Aa == Algorithm::Accurate ) {
        QxW::add_QW_SW_TW ( _QX_(a), _SX_(b), _TX_(c) );
      } else {
        QxW::add_QQW_SW_QTW ( _QX_(a), _SX_(b), _TX_(c) );
        if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( tX_real::tx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) NOEXCEPT {
      return operator_add_body ( b, a );
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = tX_real::tx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::add_TW_TW_TW ( _TX_(a), _TX_(b), _TX_(c) );
      } else {
        QxW::add_QTW_QTW_QTW ( _TX_(a), _TX_(b), _TX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( tX_real::tx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) NOEXCEPT {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = tX_real::tx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::add_TW_DW_TW ( _TX_(a), _DX_(b), _TX_(c) );
      } else {
        QxW::add_QTW_PA_QTW ( _TX_(a), _DX_(b), _TX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator_add_body ( tX_real::tx_real<T,Aa> const& a, T const& b ) NOEXCEPT {
      using TX = tX_real::tx_real<T,Aa>;
      TX c;
      if ( Aa == Algorithm::Accurate ) {
        QxW::add_TW_SW_TW ( _TX_(a), _SX_(b), _TX_(c) );
      } else {
        QxW::add_QTW_SW_QTW ( _TX_(a), _SX_(b), _TX_(c) );
        if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( dX_real::dx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) NOEXCEPT {
      return operator_add_body ( b, a );
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( dX_real::dx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
      return operator_add_body ( b, a );
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) NOEXCEPT {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = tX_real::tx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::add_DW_DW_TW ( _DX_(a), _DX_(b), _TX_(c) );
      } else {
        QxW::add_PA_PA_QTW ( _DX_(a), _DX_(b), _TX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator_add_body ( dX_real::dx_real<T,Aa> const& a, T const& b ) NOEXCEPT {
      using TX = tX_real::tx_real<T,Aa>;
      TX c;
      if ( Aa == Algorithm::Accurate ) {
        QxW::add_DW_SW_TW ( _DX_(a), _SX_(b), _TX_(c) );
      } else {
        QxW::add_PA_SW_QTW ( _DX_(a), _SX_(b), _TX_(c) );
        if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( T const& a, qX_real::qx_real<T,Ab> const& b ) NOEXCEPT {
      return operator_add_body ( b, a );
    }
    template < typename T, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( T const& a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
      return operator_add_body ( b, a );
    }
    template < typename T, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( T const& a, dX_real::dx_real<T,Ab> const& b ) NOEXCEPT {
      return operator_add_body ( b, a );
    }
    template < typename T, Algorithm A, T_fp(T) >
    INLINE auto constexpr operator_add_body ( T const& a, T const& b ) NOEXCEPT {
      using TX = tX_real::tx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::add_SW_SW_TW ( _SX_(a), _SX_(b), _TX_(c) );
      } else {
        QxW::add_SW_SW_QTW ( _SX_(a), _SX_(b), _TX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    //
#if MX_REAL_USE_INF_NAN_EXCEPTION
    template < Algorithm A, typename T, T_fp(T) >
    INLINE auto constexpr operator_add_exception ( T const& a, T const& b, bool & flag ) NOEXCEPT {
      using TX = tX_real::tx_real<T,A>;
      flag = true; {
        if ( fp<T>::isnan( a ) || fp<T>::isnan( b ) ) {
          auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );
          return TX{ c,c,c };
        }
        if ( fp<T>::isinf( a ) && fp<T>::isinf( b ) ) {
          auto sign = std::signbit( a ) ^ std::signbit( b );
          auto c = sign ? fp<T>::nan : fp<T>::copysign( fp<T>::inf, a );
          return TX{ c,c,c };
        }
        if ( fp<T>::isinf( a ) ) {
          auto c = fp<T>::copysign( fp<T>::inf, a );
          return TX{ c,c,c };
        }
        if ( fp<T>::isinf( b ) ) {
          auto c = fp<T>::copysign( fp<T>::inf, b );
          return TX{ c,c,c };
        }
      }
      //
      flag = false; return TX::zero();
    }
    template < typename TXa, typename TXb, T_mX2(TXa,TXb) ) >
  INLINE auto constexpr operator_add_exception ( TXa const& a, TXb const& b, bool & flag ) NOEXCEPT {
    Algorithm constexpr A = commonAlgorithm< TXa::base_A, TXb::base_A >::algorithm;
    auto sa = a.quick_Normalized();
    auto sb = b.quick_Normalized();
    return tX_real::operator_add_exception<A> ( sa, sb, flag );
  }
  template < typename TXa, typename T, T_mX_fp(TXa,T) >
  INLINE auto constexpr operator_add_exception ( TXa const& a, T const& b, bool & flag ) NOEXCEPT {
    Algorithm constexpr A = TXa::base_A;
    auto sa = a.quick_Normalized();
    return tX_real::operator_add_exception<A> ( sa, b, flag );
  }
#endif
  //
  template < typename TXa, typename TXb, T_mX2(TXa,TXb), T_assert( TXa::L >= TXb::L ) >
  INLINE auto constexpr operator_add ( TXa const& a, TXb const& b ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    bool flag = false;
    auto e = tX_real::operator_add_exception ( a, b, flag );
    if ( flag ) { return e; }
#endif
    return tX_real::operator_add_body ( a, b );
  }
  template < typename TXa, typename TXb, T_mX2(TXa,TXb), T_assert( TXa::L < TXb::L ) >
  INLINE auto constexpr operator_add ( TXa const& a, TXb const& b ) NOEXCEPT {
    return tX_real::operator_add ( b, a );
  }
    template < typename TXa, typename T, T_mX_fp(TXa,T) >
    INLINE auto constexpr operator_add ( TXa const& a, T const& b ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = tX_real::operator_add_exception ( a, b, flag );
      if ( flag ) { return e; }
#endif
      return tX_real::operator_add_body ( a, b );
    }
  template < typename T, typename TXb, T_mX_fp(TXb,T) >
  INLINE auto constexpr operator_add ( T const& a, TXb const& b ) NOEXCEPT {
    return tX_real::operator_add ( b, a );
  }
  template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >
  INLINE auto constexpr operator_add ( T const& a, T const& b ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    bool flag = false;
    auto e = tX_real::operator_add_exception<A> ( a, b, flag );
    if ( flag ) { return e; }
#endif
    return tX_real::operator_add_body<T,A> ( a, b );
  }
  //
  template < typename TXa, typename TXb, T_mX2(TXa,TXb), T_assert( std::max( TXa::L, TXb::L ) == 3 ) >
  INLINE auto constexpr operator+ ( TXa const& a, TXb const& b ) NOEXCEPT {
    return tX_real::operator_add ( a, b );
  }
  template < typename T, Algorithm A >
  INLINE auto constexpr operator+ ( tX_real::tx_real<T,A> const& a, T const& b ) NOEXCEPT {
    return tX_real::operator_add ( a, b );
  }
  template < typename T, Algorithm A, typename Ts, T_scalar(Ts), T_neq_Ts(T,Ts) >
  INLINE auto constexpr operator+ ( tX_real::tx_real<T,A> const& a, Ts const& b ) NOEXCEPT {
    return tX_real::operator_add ( a, T(b) );
  }
  template < typename T, Algorithm A >
  INLINE auto constexpr operator+ ( T const& a, tX_real::tx_real<T,A> const& b ) NOEXCEPT {
    return b + a;
  }
  template < typename T, Algorithm A, typename Ts, T_scalar(Ts), T_neq_Ts(T,Ts) >
  INLINE auto constexpr operator+ ( Ts const& a, tX_real::tx_real<T,A> const& b ) NOEXCEPT {
    return b + a;
  }
  //

  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto constexpr operator_add_ow_body ( tX_real::tx_real<T,Aa> & a, qX_real::qx_real<T,Ab> const& b ) NOEXCEPT {
    Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
    if ( A == Algorithm::Accurate ) {
      QxW::add_TW_QW_TW ( _TX_(a), _QX_(b), _TX_(a) );
    } else {
      QxW::add_QTW_QQW_QTW ( _TX_(a), _QX_(b), _TX_(a) );
      if ( A != Algorithm::Quasi ) { Normalize( a ); }
    }
    return a;
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto constexpr operator_add_ow_body ( tX_real::tx_real<T,Aa> & a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
    Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
    if ( A == Algorithm::Accurate ) {
      QxW::add_TW_TW_TW ( _TX_(a), _TX_(b), _TX_(a) );
    } else {
      QxW::add_QTW_QTW_QTW ( _TX_(a), _TX_(b), _TX_(a) );
      if ( A != Algorithm::Quasi ) { Normalize( a ); }
    }
    return a;
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto constexpr operator_add_ow_body ( tX_real::tx_real<T,Aa> & a, dX_real::dx_real<T,Ab> const& b ) NOEXCEPT {
    Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
    if ( A == Algorithm::Accurate ) {
      QxW::add_TW_DW_TW ( _TX_(a), _DX_(b), _TX_(a) );
    } else {
      QxW::add_QTW_PA_QTW ( _TX_(a), _DX_(b), _TX_(a) );
      if ( A != Algorithm::Quasi ) { Normalize( a ); }
    }
    return a;
  }
  template < typename T, Algorithm Aa >
  INLINE auto constexpr operator_add_ow_body ( tX_real::tx_real<T,Aa> & a, T const& b ) NOEXCEPT {
    if ( Aa == Algorithm::Accurate ) {
      QxW::add_TW_SW_TW ( _TX_(a), _SX_(b), _TX_(a) );
    } else {
      QxW::add_QTW_SW_QTW ( _TX_(a), _SX_(b), _TX_(a) );
      if ( Aa != Algorithm::Quasi ) { Normalize( a ); }
    }
    return a;
  }
  //
  template < typename T, Algorithm Aa, template < typename _Tb_, Algorithm _Ab_ > class TXb, Algorithm Ab, T_mX(TXb<T,Ab>), A_owAble(Aa,Ab) >
  INLINE auto constexpr operator_add_ow ( tX_real::tx_real<T,Aa> & a, TXb<T,Ab> const& b ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    bool flag = false;
    auto e = tX_real::operator_add_exception ( a, b, flag );
    if ( flag ) { return e; }
#endif
    return tX_real::operator_add_ow_body ( a, b );
  }
  template < typename T, Algorithm A >
  INLINE auto constexpr operator_add_ow ( tX_real::tx_real<T,A> & a, T const& b ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    bool flag = false;
    auto e = tX_real::operator_add_exception ( a, b, flag );
    if ( flag ) { return e; }
#endif
    return tX_real::operator_add_ow_body ( a, b );
  }
  //
  template < typename T, Algorithm Aa, template < typename _Tb_, Algorithm _Ab_ > class TXb, Algorithm Ab, T_mX(TXb<T,Ab>), A_owAble(Aa,Ab) >
  INLINE auto constexpr operator+= ( tX_real::tx_real<T,Aa> & a, TXb<T,Ab> const& b ) NOEXCEPT {
    return tX_real::operator_add_ow ( a, b );
  }
  template < typename T, Algorithm A >
  INLINE auto constexpr operator+= ( tX_real::tx_real<T,A> & a, T const& b ) NOEXCEPT {
    return tX_real::operator_add_ow ( a, b );
  }
  template < typename T, Algorithm A, typename Ts, T_scalar(Ts), T_neq_Ts(T,Ts) >
  INLINE auto constexpr operator+= ( tX_real::tx_real<T,A> & a, Ts const& b ) NOEXCEPT {
    return tX_real::operator_add_ow ( a, T(b) );
  }
  //

  //
  // Substraction
  //
  //
  template < typename TXa, typename TXb, T_mX2(TXa,TXb), T_assert( std::max( TXa::L, TXb::L ) ==3 ) >
  INLINE auto constexpr operator- ( TXa const& a, TXb const& b ) NOEXCEPT {
    return a + (-b);
  }
  template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >
  INLINE auto constexpr operator- ( tX_real::tx_real<T,A> const& a, Ts const& b ) NOEXCEPT {
    return a + (-b);
  }
  template < Algorithm A=Algorithm::Accurate, typename T, typename Ts, T_scalar(Ts) >
  INLINE auto constexpr operator- ( Ts const &a, tX_real::tx_real<T,A> const& b ) NOEXCEPT {
    return a + (-b);
  }
  //

  //
  template < typename T, Algorithm Aa, Algorithm Ab, template < typename _Tb_, Algorithm _Ab_ > class TXb, T_mX(TXb<T,Ab>), A_owAble(Aa,Ab) >
  INLINE auto constexpr operator-= ( tX_real::tx_real<T,Aa> & a, TXb<T,Ab> const& b ) NOEXCEPT {
    return a += (-b);
  }
  template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >
  INLINE auto constexpr operator-= ( tX_real::tx_real<T,A> & a, Ts const& b ) NOEXCEPT {
    return a += (-b);
  }
  //

  //
  // Multiplication
  //
  //
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto constexpr operator_mul_body ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) NOEXCEPT {
    Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
    using TX = tX_real::tx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      if ( (void *)(&a) == (void *)(&b) ) {
        QxW::sqr_QW_TW ( _QX_(a), _TX_(c) );
      } else {
        QxW::mul_QW_QW_TW ( _QX_(a), _QX_(b), _TX_(c) );
      }
    } else {
      if ( (void *)(&a) == (void *)(&b) ) {
        QxW::sqr_QQW_QTW ( _QX_(a), _TX_(c) );
      } else {
        QxW::mul_QQW_QQW_QTW ( _QX_(a), _QX_(b), _TX_(c) );
      }
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto constexpr operator_mul_body ( qX_real::qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
    Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
    using TX = tX_real::tx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::mul_QW_TW_TW ( _QX_(a), _TX_(b), _TX_(c) );
    } else {
      QxW::mul_QQW_QTW_QTW ( _QX_(a), _TX_(b), _TX_(c) );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto constexpr operator_mul_body ( qX_real::qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) NOEXCEPT {
    Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
    using TX = tX_real::tx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::mul_QW_DW_TW ( _QX_(a), _DX_(b), _TX_(c) );
    } else {
      QxW::mul_QQW_PA_QTW ( _QX_(a), _DX_(b), _TX_(c) );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa >
  INLINE auto constexpr operator_mul_body ( qX_real::qx_real<T,Aa> const& a, T const& b ) NOEXCEPT {
    using TX = tX_real::tx_real<T,Aa>;
    TX c;
    if ( Aa == Algorithm::Accurate ) {
      QxW::mul_QW_SW_TW ( _QX_(a), _SX_(b), _TX_(c) );
    } else {
      QxW::mul_QQW_SW_QTW ( _QX_(a), _SX_(b), _TX_(c) );
      if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto constexpr operator_mul_body ( tX_real::tx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) NOEXCEPT {
    return operator_mul_body ( b, a );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto constexpr operator_mul_body ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
    Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
    using TX = tX_real::tx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      if ( (void *)(&a) == (void *)(&b) ) {
        QxW::sqr_TW_TW ( _TX_(a), _TX_(c) );
      } else {
        QxW::mul_TW_TW_TW ( _TX_(a), _TX_(b), _TX_(c) );
      }
    } else {
      if ( (void *)(&a) == (void *)(&b) ) {
        QxW::sqr_QTW_QTW ( _TX_(a), _TX_(c) );
      } else {
        QxW::mul_QTW_QTW_QTW ( _TX_(a), _TX_(b), _TX_(c) );
      }
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto constexpr operator_mul_body ( tX_real::tx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) NOEXCEPT {
    Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
    using TX = tX_real::tx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      QxW::mul_TW_DW_TW ( _TX_(a), _DX_(b), _TX_(c) );
    } else {
      QxW::mul_QTW_PA_QTW ( _TX_(a), _DX_(b), _TX_(c) );
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa >
  INLINE auto constexpr operator_mul_body ( tX_real::tx_real<T,Aa> const& a, T const& b ) NOEXCEPT {
    using TX = tX_real::tx_real<T,Aa>;
    TX c;
    if ( Aa == Algorithm::Accurate ) {
      QxW::mul_TW_SW_TW ( _TX_(a), _SX_(b), _TX_(c) );
    } else {
      QxW::mul_QTW_SW_QTW ( _TX_(a), _SX_(b), _TX_(c) );
      if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto constexpr operator_mul_body ( dX_real::dx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) NOEXCEPT {
    return operator_mul_body ( b, a );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto constexpr operator_mul_body ( dX_real::dx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
    return operator_mul_body ( b, a );
  }
  template < typename T, Algorithm Aa, Algorithm Ab >
  INLINE auto constexpr operator_mul_body ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) NOEXCEPT {
    Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
    using TX = tX_real::tx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      if ( (void *)(&a) == (void *)(&b) ) {
        QxW::sqr_DW_TW ( _DX_(a), _TX_(c) );
      } else {
        QxW::mul_DW_DW_TW ( _DX_(a), _DX_(b), _TX_(c) );
      }
    } else {
      if ( (void *)(&a) == (void *)(&b) ) {
        QxW::sqr_PA_QTW ( _DX_(a), _TX_(c) );
      } else {
        QxW::mul_PA_PA_QTW ( _DX_(a), _DX_(b), _TX_(c) );
      }
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Aa >
  INLINE auto constexpr operator_mul_body ( dX_real::dx_real<T,Aa> const& a, T const& b ) NOEXCEPT {
    using TX = tX_real::tx_real<T,Aa>;
    TX c;
    if ( Aa == Algorithm::Accurate ) {
      QxW::mul_DW_SW_TW ( _DX_(a), _SX_(b), _TX_(c) );
    } else {
      QxW::mul_PA_SW_QTW ( _DX_(a), _SX_(b), _TX_(c) );
      if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  template < typename T, Algorithm Ab >
  INLINE auto constexpr operator_mul_body ( T const& a, qX_real::qx_real<T,Ab> const& b ) NOEXCEPT {
    return operator_mul_body ( b, a );
  }
  template < typename T, Algorithm Ab >
  INLINE auto constexpr operator_mul_body ( T const& a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
    return operator_mul_body ( b, a );
  }
  template < typename T, Algorithm Ab >
  INLINE auto constexpr operator_mul_body ( T const& a, dX_real::dx_real<T,Ab> const& b ) NOEXCEPT {
    return operator_mul_body ( b, a );
  }
  template < typename T, Algorithm A, T_fp(T) >
  INLINE auto constexpr operator_mul_body ( T const& a, T const& b ) NOEXCEPT {
    using TX = tX_real::tx_real<T,A>;
    TX c;
    if ( A == Algorithm::Accurate ) {
      if ( (void *)(&a) == (void *)(&b) ) {
        QxW::sqr_SW_TW ( _SX_(a), _TX_(c) );
      } else {
        QxW::mul_SW_SW_TW ( _SX_(a), _SX_(b), _TX_(c) );
      }
    } else {
      if ( (void *)(&a) == (void *)(&b) ) {
        QxW::sqr_SW_QTW ( _SX_(a), _TX_(c) );
      } else {
        QxW::mul_SW_SW_QTW ( _SX_(a), _SX_(b), _TX_(c) );
      }
      if ( A != Algorithm::Quasi ) { Normalize( c ); }
    }
    return c;
  }
  //
#if MX_REAL_USE_INF_NAN_EXCEPTION
  template < Algorithm A, typename T, T_fp(T) >
  INLINE auto constexpr operator_mul_exception ( T const& a, T const& b, bool & flag ) NOEXCEPT {
    using TX = tX_real::tx_real<T,A>;
    flag = true; {
      if ( fp<T>::isnan( a ) || fp<T>::isnan( b ) ) {
        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );
        return TX{ c,c,c };
      }
      if ( fp<T>::isinf( a ) && fp<T>::isinf( b ) ) {
        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::inf, a ), b );
        return TX{ c,c,c };
      }
      if ( fp<T>::isinf( a ) ) {
        if ( fp<T>::is_zero( b ) ) {
          auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );
          return TX{ c,c,c };
        } else {
          auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::inf, a ), b );
          return TX{ c,c,c };
        }
      }
      if ( fp<T>::isinf( b ) ) {
        if ( fp<T>::is_zero( a ) ) {
          auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );
          return TX{ c,c,c };
        } else {
          auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::inf, a ), b );
          return TX{ c,c,c };
        }
      }
      if ( fp<T>::is_zero( a ) || fp<T>::is_zero( b ) ) {
        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::zero, a ), b );
        return TX{ c,c,c };
      }
    }
    //
    flag = false; return TX::zero();
  }
  template < typename TXa, typename TXb, T_mX2(TXa,TXb) ) >
  INLINE auto constexpr operator_mul_exception ( TXa const& a, TXb const& b, bool & flag ) NOEXCEPT {
    Algorithm constexpr A = commonAlgorithm< TXa::base_A, TXb::base_A >::algorithm;
    auto sa = a.quick_Normalized();
    auto sb = b.quick_Normalized();
    return tX_real::operator_mul_exception<A> ( sa, sb, flag );
  }
template < typename TXa, typename T, T_mX_fp(TXa,T) >
INLINE auto constexpr operator_mul_exception ( TXa const& a, T const& b, bool & flag ) NOEXCEPT {
  Algorithm constexpr A = TXa::base_A;
  auto sa = a.quick_Normalized();
  return tX_real::operator_mul_exception<A> ( sa, b, flag );
}
#endif
//
template < typename TXa, typename TXb, T_mX2(TXa,TXb), T_assert( TXa::L >= TXb::L ) >
INLINE auto constexpr operator_mul ( TXa const& a, TXb const& b ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag = false;
  auto e = tX_real::operator_mul_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_mul_body ( a, b );
}
template < typename TXa, typename TXb, T_mX2(TXa,TXb), T_assert( TXa::L < TXb::L ) >
INLINE auto constexpr operator_mul ( TXa const& a, TXb const& b ) NOEXCEPT {
  return tX_real::operator_mul ( b, a );
}
  template < typename TXa, typename T, T_mX_fp(TXa,T) >
  INLINE auto constexpr operator_mul ( TXa const& a, T const& b ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
    bool flag = false;
    auto e = tX_real::operator_mul_exception ( a, b, flag );
    if ( flag ) { return e; }
#endif
    return tX_real::operator_mul_body ( a, b );
  }
template < typename T, typename TXb, T_mX_fp(TXb,T) >
INLINE auto constexpr operator_mul ( T const& a, TXb const& b ) NOEXCEPT {
  return tX_real::operator_mul ( b, a );
}
template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >
INLINE auto constexpr operator_mul ( T const& a, T const& b ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag = false;
  auto e = tX_real::operator_mul_exception<A> ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_mul_body<T,A> ( a, b );
}
//
template < typename T, Algorithm A >
INLINE auto constexpr operator_mul_pow2 ( T const& a, tX_real::tx_real<T,A> const& b ) NOEXCEPT {
  auto sa = a;
  auto c = b;
  for(int i=0; i<3; i++) {
    c.x[i] = sa * b.x[i];
  }
  return c;
}
template < typename T, Algorithm A, typename Ts, T_scalar(Ts), T_neq_Ts(T,Ts) >
INLINE auto constexpr operator_mul_pow2 ( Ts const& a, tX_real::tx_real<T,A> const& b ) NOEXCEPT {
  auto sa = T(a);
  auto c = b;
  for(int i=0; i<3; i++) {
    c.x[i] = sa * b.x[i];
  }
  return c;
}
template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >
INLINE auto constexpr operator_mul_pow2 ( tX_real::tx_real<T,A> const& a, Ts const& b ) NOEXCEPT {
  return tX_real::operator_mul_pow2( b, a );
}
//
template < typename TXa, typename TXb, T_mX2(TXa,TXb), T_assert( std::max( TXa::L, TXb::L ) == 3 ) >
INLINE auto constexpr operator* ( TXa const& a, TXb const& b ) NOEXCEPT {
  return tX_real::operator_mul ( a, b );
}
template < typename T, Algorithm A >
INLINE auto constexpr operator* ( tX_real::tx_real<T,A> const& a, T const& b ) NOEXCEPT {
#if MX_REAL_OPTIMIZE_PROD_BY_POW2
  if ( ( QxW::fp_const<T>::fp2uint( b ) & QxW::fp_const<T>::FRAC ) == 0 ) {
    return tX_real::operator_mul_pow2 ( a, b );
  } else
#endif
    {
      return tX_real::operator_mul ( a, b );
    }
}
template < typename T, Algorithm A, typename Ts, T_scalar(Ts), T_neq_Ts(T,Ts) >
INLINE auto constexpr operator* ( tX_real::tx_real<T,A> const& a, Ts const& b ) NOEXCEPT {
#if MX_REAL_OPTIMIZE_PROD_BY_POW2
  if ( ( QxW::fp_const<T>::fp2uint( T(b) ) & QxW::fp_const<T>::FRAC ) == 0 ) {
    return tX_real::operator_mul_pow2 ( a, b );
  } else
#endif
    {
      return tX_real::operator_mul ( a, T(b) );
    }
}
template < typename T, Algorithm A >
INLINE auto constexpr operator* ( T const& a, tX_real::tx_real<T,A> const& b ) NOEXCEPT {
  return b * a;
}
template < typename T, Algorithm A, typename Ts, T_scalar(Ts), T_neq_Ts(T,Ts) >
INLINE auto constexpr operator* ( Ts const& a, tX_real::tx_real<T,A> const& b ) NOEXCEPT {
  return b * a;
}
//

//
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_mul_ow_body ( tX_real::tx_real<T,Aa> & a, qX_real::qx_real<T,Ab> const& b ) NOEXCEPT {
  Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
  if ( A == Algorithm::Accurate ) {
    QxW::mul_TW_QW_TW ( _TX_(a), _QX_(b), _TX_(a) );
  } else {
    QxW::mul_QTW_QQW_QTW ( _TX_(a), _QX_(b), _TX_(a) );
    if ( A != Algorithm::Quasi ) { Normalize( a ); }
  }
  return a;
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_mul_ow_body ( tX_real::tx_real<T,Aa> & a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
  Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
  if ( A == Algorithm::Accurate ) {
    QxW::mul_TW_TW_TW ( _TX_(a), _TX_(b), _TX_(a) );
  } else {
    QxW::mul_QTW_QTW_QTW ( _TX_(a), _TX_(b), _TX_(a) );
    if ( A != Algorithm::Quasi ) { Normalize( a ); }
  }
  return a;
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_mul_ow_body ( tX_real::tx_real<T,Aa> & a, dX_real::dx_real<T,Ab> const& b ) NOEXCEPT {
  Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
  if ( A == Algorithm::Accurate ) {
    QxW::mul_TW_DW_TW ( _TX_(a), _DX_(b), _TX_(a) );
  } else {
    QxW::mul_QTW_PA_QTW ( _TX_(a), _DX_(b), _TX_(a) );
    if ( A != Algorithm::Quasi ) { Normalize( a ); }
  }
  return a;
}
template < typename T, Algorithm Aa >
INLINE auto constexpr operator_mul_ow_body ( tX_real::tx_real<T,Aa> & a, T const& b ) NOEXCEPT {
  if ( Aa == Algorithm::Accurate ) {
    QxW::mul_TW_SW_TW ( _TX_(a), _SX_(b), _TX_(a) );
  } else {
    QxW::mul_QTW_SW_QTW ( _TX_(a), _SX_(b), _TX_(a) );
    if ( Aa != Algorithm::Quasi ) { Normalize( a ); }
  }
  return a;
}
//
template < typename T, Algorithm Aa, template < typename _Tb_, Algorithm _Ab_ > class TXb, Algorithm Ab, T_mX(TXb<T,Ab>), A_owAble(Aa,Ab) >
INLINE auto constexpr operator_mul_ow ( tX_real::tx_real<T,Aa> & a, TXb<T,Ab> const& b ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag = false;
  auto e = tX_real::operator_mul_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_mul_ow_body ( a, b );
}
template < typename T, Algorithm A >
INLINE auto constexpr operator_mul_ow ( tX_real::tx_real<T,A> & a, T const& b ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag = false;
  auto e = tX_real::operator_mul_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_mul_ow_body ( a, b );
}
//
template < typename T, Algorithm A >
INLINE auto constexpr operator_mul_ow_pow2 ( tX_real::tx_real<T,A> & a, T const& b ) NOEXCEPT {
  for(int i=0; i<3; i++) {
    a.x[i] *= b;
  }
  return a;
}
template < typename T, Algorithm A, typename Ts, T_scalar(Ts), T_neq_Ts(T,Ts) >
INLINE auto constexpr operator_mul_ow_pow2 ( tX_real::tx_real<T,A> & a, Ts const& b ) NOEXCEPT {
  auto sb = T(b);
  for(int i=0; i<3; i++) {
    a.x[i] *= sb;
  }
  return a;
}
//
template < typename T, Algorithm Aa, template < typename _Tb_, Algorithm _Ab_ > class TXb, Algorithm Ab, T_mX(TXb<T,Ab>), A_owAble(Aa,Ab) >
INLINE auto constexpr operator*= ( tX_real::tx_real<T,Aa> & a, TXb<T,Ab> const& b ) NOEXCEPT {
  return tX_real::operator_mul_ow ( a, b );
}
template < typename T, Algorithm A >
INLINE auto constexpr operator*= ( tX_real::tx_real<T,A> & a, T const& b ) NOEXCEPT {
#if MX_REAL_OPTIMIZE_PROD_BY_POW2
  if ( ( QxW::fp_const<T>::fp2uint( b ) & QxW::fp_const<T>::FRAC ) == 0 ) {
    return tX_real::operator_mul_ow_pow2 ( a, b );
  } else
#endif
    {
      return tX_real::operator_mul_ow ( a, b );
    }
}
template < typename T, Algorithm A, typename Ts, T_scalar(Ts), T_neq_Ts(T,Ts) >
INLINE auto constexpr operator*= ( tX_real::tx_real<T,A> & a, Ts const& b ) NOEXCEPT {
#if MX_REAL_OPTIMIZE_PROD_BY_POW2
  if ( ( QxW::fp_const<T>::fp2uint( T(b) ) & QxW::fp_const<T>::FRAC ) == 0 ) {
    return tX_real::operator_mul_ow_pow2 ( a, b );
  } else
#endif
    {
      return tX_real::operator_mul_ow ( a, T(b) );
    }
}
//

//
// Division
//
//
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_div_body ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) NOEXCEPT {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = tX_real::tx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::div_QW_QW_TW ( _QX_(a), _QX_(b), _TX_(c) );
  } else {
    QxW::div_QQW_QQW_QTW ( _QX_(a), _QX_(b), _TX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_div_body ( qX_real::qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = tX_real::tx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::div_QW_TW_TW ( _QX_(a), _TX_(b), _TX_(c) );
  } else {
    QxW::div_QQW_QTW_QTW ( _QX_(a), _TX_(b), _TX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_div_body ( qX_real::qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) NOEXCEPT {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = tX_real::tx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::div_QW_DW_TW ( _QX_(a), _DX_(b), _TX_(c) );
  } else {
    QxW::div_QQW_PA_QTW ( _QX_(a), _DX_(b), _TX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm Aa >
INLINE auto constexpr operator_div_body ( qX_real::qx_real<T,Aa> const& a, T const& b ) NOEXCEPT {
  using TX = tX_real::tx_real<T,Aa>;
  TX c;
  if ( Aa == Algorithm::Accurate ) {
    QxW::div_QW_SW_TW ( _QX_(a), _SX_(b), _TX_(c) );
  } else {
    QxW::div_QQW_SW_QTW ( _QX_(a), _SX_(b), _TX_(c) );
    if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_div_body ( tX_real::tx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) NOEXCEPT {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = tX_real::tx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::div_TW_QW_TW ( _TX_(a), _QX_(b), _TX_(c) );
  } else {
    QxW::div_QTW_QQW_QTW ( _TX_(a), _QX_(b), _TX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_div_body ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = tX_real::tx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::div_TW_TW_TW ( _TX_(a), _TX_(b), _TX_(c) );
  } else {
    QxW::div_QTW_QTW_QTW ( _TX_(a), _TX_(b), _TX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_div_body ( tX_real::tx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) NOEXCEPT {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = tX_real::tx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::div_TW_DW_TW ( _TX_(a), _DX_(b), _TX_(c) );
  } else {
    QxW::div_QTW_PA_QTW ( _TX_(a), _DX_(b), _TX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm Aa >
INLINE auto constexpr operator_div_body ( tX_real::tx_real<T,Aa> const& a, T const& b ) NOEXCEPT {
  using TX = tX_real::tx_real<T,Aa>;
  TX c;
  if ( Aa == Algorithm::Accurate ) {
    QxW::div_TW_SW_TW ( _TX_(a), _SX_(b), _TX_(c) );
  } else {
    QxW::div_QTW_SW_QTW ( _TX_(a), _SX_(b), _TX_(c) );
    if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_div_body ( dX_real::dx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) NOEXCEPT {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = tX_real::tx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::div_DW_QW_TW ( _DX_(a), _QX_(b), _TX_(c) );
  } else {
    QxW::div_PA_QQW_QTW ( _DX_(a), _QX_(b), _TX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_div_body ( dX_real::dx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = tX_real::tx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::div_DW_TW_TW ( _DX_(a), _TX_(b), _TX_(c) );
  } else {
    QxW::div_PA_QTW_QTW ( _DX_(a), _TX_(b), _TX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_div_body ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) NOEXCEPT {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = tX_real::tx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::div_DW_DW_TW ( _DX_(a), _DX_(b), _TX_(c) );
  } else {
    QxW::div_PA_PA_QTW ( _DX_(a), _DX_(b), _TX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm Aa >
INLINE auto constexpr operator_div_body ( dX_real::dx_real<T,Aa> const& a, T const& b ) NOEXCEPT {
  using TX = tX_real::tx_real<T,Aa>;
  TX c;
  if ( Aa == Algorithm::Accurate ) {
    QxW::div_DW_SW_TW ( _DX_(a), _SX_(b), _TX_(c) );
  } else {
    QxW::div_PA_SW_QTW ( _DX_(a), _SX_(b), _TX_(c) );
    if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm Ab >
INLINE auto constexpr operator_div_body ( T const& a, qX_real::qx_real<T,Ab> const& b ) NOEXCEPT {
  using TX = tX_real::tx_real<T,Ab>;
  TX c;
  if ( Ab == Algorithm::Accurate ) {
    QxW::div_SW_QW_TW ( _SX_(a), _QX_(b), _TX_(c) );
  } else {
    QxW::div_SW_QQW_QTW ( _SX_(a), _QX_(b), _TX_(c) );
    if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm Ab >
INLINE auto constexpr operator_div_body ( T const& a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
  using TX = tX_real::tx_real<T,Ab>;
  TX c;
  if ( Ab == Algorithm::Accurate ) {
    QxW::div_SW_TW_TW ( _SX_(a), _TX_(b), _TX_(c) );
  } else {
    QxW::div_SW_QTW_QTW ( _SX_(a), _TX_(b), _TX_(c) );
    if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm Ab >
INLINE auto constexpr operator_div_body ( T const& a, dX_real::dx_real<T,Ab> const& b ) NOEXCEPT {
  using TX = tX_real::tx_real<T,Ab>;
  TX c;
  if ( Ab == Algorithm::Accurate ) {
    QxW::div_SW_DW_TW ( _SX_(a), _DX_(b), _TX_(c) );
  } else {
    QxW::div_SW_PA_QTW ( _SX_(a), _DX_(b), _TX_(c) );
    if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm A, T_fp(T) >
INLINE auto constexpr operator_div_body ( T const& a, T const& b ) NOEXCEPT {
  using TX = tX_real::tx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::div_SW_SW_TW ( _SX_(a), _SX_(b), _TX_(c) );
  } else {
    QxW::div_SW_SW_QTW ( _SX_(a), _SX_(b), _TX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
//
#if MX_REAL_USE_INF_NAN_EXCEPTION
template < Algorithm A, typename T, T_fp(T) >
INLINE auto constexpr operator_div_exception ( T const& a, T const& b, bool & flag ) NOEXCEPT {
  using TX = tX_real::tx_real<T,A>;
  flag = true; {
    if ( fp<T>::isnan( a ) || fp<T>::isnan( b ) ) {
      auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );
      return TX{ c,c,c };
    }
    if ( fp<T>::isinf( a ) && fp<T>::isinf( b ) ) {
      auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );
      return TX{ c,c,c };
    }
    if ( fp<T>::isinf( a ) ) {
      auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::inf, a ), b );
      return TX{ c,c,c };
    }
    if ( fp<T>::isinf( b ) ) {
      auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::zero, a ), b );
      return TX{ c,c,c };
    }
    if ( fp<T>::is_zero( b ) ) {
      if ( fp<T>::is_zero( a ) ) {
        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );
        return TX{ c,c,c };
      } else {
        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::inf, a ), b );
        return TX{ c,c,c };
      }
    }
  }
  //
  flag = false; return TX::zero();
}
template < typename TXa, typename TXb, T_mX2(TXa,TXb) ) >
INLINE auto constexpr operator_div_exception ( TXa const& a, TXb const& b, bool & flag ) NOEXCEPT {
  Algorithm constexpr A = commonAlgorithm< TXa::base_A, TXb::base_A >::algorithm;
  auto sa = a.quick_Normalized();
  auto sb = b.quick_Normalized();
  return tX_real::operator_div_exception<A> ( sa, sb, flag );
}
template < typename TXa, typename T, T_mX_fp(TXa,T) >
INLINE auto constexpr operator_div_exception ( TXa const& a, T const& b, bool & flag ) NOEXCEPT {
  Algorithm constexpr A = TXa::base_A;
  auto sa = a.quick_Normalized();
  return tX_real::operator_div_exception<A> ( sa, b, flag );
}
template < typename T, typename TXb, T_mX_fp(TXb,T) >
INLINE auto constexpr operator_div_exception ( T const& a, TXb const& b, bool & flag ) NOEXCEPT {
  Algorithm constexpr A = TXb::base_A;
  auto sb = b.quick_Normalized();
  return tX_real::operator_div_exception<A> ( a, sb, flag );
}
#endif
//
template < typename TXa, typename TXb, T_mX2(TXa,TXb) >
INLINE auto constexpr operator_div ( TXa const& a, TXb const& b ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag = false;
  auto e = tX_real::operator_div_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_div_body ( a, b );
}
template < typename TXa, typename T, T_mX_fp(TXa,T) >
INLINE auto constexpr operator_div ( TXa const& a, T const& b ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag = false;
  auto e = tX_real::operator_div_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_div_body ( a, b );
}
template < typename T, typename TXb, T_mX_fp(TXb,T) >
INLINE auto constexpr operator_div ( T const& a, TXb const& b ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag = false;
  auto e = tX_real::operator_div_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_div_body ( a, b );
}
template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >
INLINE auto constexpr operator_div ( T const& a, T const& b ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag = false;
  auto e = tX_real::operator_div_exception<A> ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_div_body<T,A> ( a, b );
}
//
template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >
INLINE auto constexpr operator_div_pow2 ( tX_real::tx_real<T,A> const& a, Ts const& b ) NOEXCEPT {
  auto sb = fp<T>::one() / b;
  auto c = a;
  for(int i=0; i<3; i++) {
    c.x[i] = sb * a.x[i];
  }
  return c;
}
//
template < typename TXa, typename TXb, T_mX2(TXa,TXb), T_assert( std::max( TXa::L, TXb::L ) == 3 ) >
INLINE auto constexpr operator/ ( TXa const& a, TXb const& b ) NOEXCEPT {
  return tX_real::operator_div ( a, b );
}
template < typename T, Algorithm A >
INLINE auto constexpr operator/ ( tX_real::tx_real<T,A> const& a, T const& b ) NOEXCEPT {
#if MX_REAL_OPTIMIZE_PROD_BY_POW2
  if ( ( QxW::fp_const<T>::fp2uint( b ) & QxW::fp_const<T>::FRAC ) == 0 ) {
    return tX_real::operator_div_pow2 ( a, b );
  } else
#endif
    {
      return tX_real::operator_div ( a, b );
    }
}
template < typename T, Algorithm A, typename Ts, T_scalar(Ts), T_neq_Ts(T,Ts) >
INLINE auto constexpr operator/ ( tX_real::tx_real<T,A> const& a, Ts const& b ) NOEXCEPT {
#if MX_REAL_OPTIMIZE_PROD_BY_POW2
  if ( ( QxW::fp_const<T>::fp2uint( T(b) ) & QxW::fp_const<T>::FRAC ) == 0 ) {
    return tX_real::operator_div_pow2 ( a, b );
  } else
#endif
    {
      return tX_real::operator_div ( a, T(b) );
    }
}
template < typename T, Algorithm A >
INLINE auto constexpr operator/ ( T const& a, tX_real::tx_real<T,A> const& b ) NOEXCEPT {
  return tX_real::operator_div ( a, b );
}
template < typename T, Algorithm A, typename Ts, T_scalar(Ts), T_neq_Ts(T,Ts) >
INLINE auto constexpr operator/ ( Ts const& a, tX_real::tx_real<T,A> const& b ) NOEXCEPT {
  return tX_real::operator_div ( T(a), b );
}
//

//
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_div_ow_body ( tX_real::tx_real<T,Aa> & a, qX_real::qx_real<T,Ab> const& b ) NOEXCEPT {
  Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
  if ( A == Algorithm::Accurate ) {
    QxW::div_TW_QW_TW ( _TX_(a), _QX_(b), _TX_(a) );
  } else {
    QxW::div_QTW_QQW_QTW ( _TX_(a), _QX_(b), _TX_(a) );
    if ( A != Algorithm::Quasi ) { Normalize( a ); }
  }
  return a;
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_div_ow_body ( tX_real::tx_real<T,Aa> & a, tX_real::tx_real<T,Ab> const& b ) NOEXCEPT {
  Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
  if ( A == Algorithm::Accurate ) {
    QxW::div_TW_TW_TW ( _TX_(a), _TX_(b), _TX_(a) );
  } else {
    QxW::div_QTW_QTW_QTW ( _TX_(a), _TX_(b), _TX_(a) );
    if ( A != Algorithm::Quasi ) { Normalize( a ); }
  }
  return a;
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_div_ow_body ( tX_real::tx_real<T,Aa> & a, dX_real::dx_real<T,Ab> const& b ) NOEXCEPT {
  Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
  if ( A == Algorithm::Accurate ) {
    QxW::div_TW_DW_TW ( _TX_(a), _DX_(b), _TX_(a) );
  } else {
    QxW::div_QTW_PA_QTW ( _TX_(a), _DX_(b), _TX_(a) );
    if ( A != Algorithm::Quasi ) { Normalize( a ); }
  }
  return a;
}
template < typename T, Algorithm Aa >
INLINE auto constexpr operator_div_ow_body ( tX_real::tx_real<T,Aa> & a, T const& b ) NOEXCEPT {
  if ( Aa == Algorithm::Accurate ) {
    QxW::div_TW_SW_TW ( _TX_(a), _SX_(b), _TX_(a) );
  } else {
    QxW::div_QTW_SW_QTW ( _TX_(a), _SX_(b), _TX_(a) );
    if ( Aa != Algorithm::Quasi ) { Normalize( a ); }
  }
  return a;
}
//
template < typename T, Algorithm Aa, template < typename _Tb_, Algorithm _Ab_ > class TXb, Algorithm Ab, T_mX(TXb<T,Ab>), A_owAble(Aa,Ab) >
INLINE auto constexpr operator_div_ow ( tX_real::tx_real<T,Aa> & a, TXb<T,Ab> const& b ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag = false;
  auto e = tX_real::operator_div_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_div_ow_body ( a, b );
}
template < typename T, Algorithm A >
INLINE auto constexpr operator_div_ow ( tX_real::tx_real<T,A> & a, T const& b ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag = false;
  auto e = tX_real::operator_div_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_div_ow_body ( a, b );
}
//
template < typename T, Algorithm A, typename Ts, T_scalar(Ts), T_neq_Ts(T,Ts) >
INLINE auto constexpr operator_div_ow_pow2 ( tX_real::tx_real<T,A> & a, Ts const& b ) NOEXCEPT {
  auto sb = fp<T>::one() / T(b);
  for(int i=0; i<3; i++) {
    a.x[i] *= sb;
  }
  return a;
}
template < typename T, Algorithm A >
INLINE auto constexpr operator_div_ow_pow2 ( tX_real::tx_real<T,A> & a, T const& b ) NOEXCEPT {
  for(int i=0; i<3; i++) {
    a.x[i] /= b;
  }
  return a;
}
//
template < typename T, Algorithm Aa, template < typename _Tb_, Algorithm _Ab_ > class TXb, Algorithm Ab, T_mX(TXb<T,Ab>), A_owAble(Aa,Ab) >
INLINE auto constexpr operator/= ( tX_real::tx_real<T,Aa> & a, TXb<T,Ab> const& b ) NOEXCEPT {
  return tX_real::operator_div_ow ( a, b );
}
template < typename T, Algorithm A >
INLINE auto constexpr operator/= ( tX_real::tx_real<T,A> & a, T const& b ) NOEXCEPT {
#if MX_REAL_OPTIMIZE_PROD_BY_POW2
  if ( ( QxW::fp_const<T>::fp2uint( b ) & QxW::fp_const<T>::FRAC ) == 0 ) {
    return tX_real::operator_div_ow_pow2 ( a, b );
  } else
#endif
    {
      return tX_real::operator_div_ow ( a, b );
    }
}
template < typename T, Algorithm A, typename Ts, T_scalar(Ts), T_neq_Ts(T,Ts) >
INLINE auto constexpr operator/= ( tX_real::tx_real<T,A> & a, Ts const& b ) NOEXCEPT {
#if MX_REAL_OPTIMIZE_PROD_BY_POW2
  if ( ( QxW::fp_const<T>::fp2uint( T(b) ) & QxW::fp_const<T>::FRAC ) == 0 ) {
    return tX_real::operator_div_ow_pow2 ( a, b );
  } else
#endif
    {
      return tX_real::operator_div_ow ( a, T(b) );
    }
}
//

//
// Absolute value
//
//
template < typename TXa, T_mX(TXa) >
INLINE auto constexpr operator_abs_body ( TXa const& a ) NOEXCEPT {
  return tX_real::mX_real<TXa>{ is_negative( a ) ? -a : a };
}
//
#if MX_REAL_USE_INF_NAN_EXCEPTION
template < Algorithm A, typename T, T_fp(T) >
INLINE auto constexpr operator_abs_exception ( T const& a, bool & flag ) NOEXCEPT {
  using TX = tX_real::tx_real<T,A>;
  flag = true; {
    if ( fp<T>::isnan( a ) ) { TX::nan(); }
    if ( fp<T>::isinf( a ) ) { TX::inf(); }
  }
  //
  flag = false; return TX::zero();
}
template < typename TXa, T_mX(TXa) >
INLINE auto constexpr operator_abs_exception ( TXa const& a, bool & flag ) NOEXCEPT {
  Algorithm constexpr A = TXa::base_A;
  auto sa = a.quick_Normalized();
  return tX_real::operator_abs_exception<A> ( sa, flag );
}
#endif
//
template < typename TXa, T_mX(TXa) >
INLINE auto constexpr operator_abs ( TXa const& a ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag = false;
  auto e = tX_real::operator_abs_exception ( a, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_abs_body ( a );
}
template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >
INLINE auto constexpr operator_abs ( T const& a ) NOEXCEPT {
  using TX = tX_real::tx_real<T,A>;
  return TX{ std::abs( a ) };
}
//
template < typename TXa, T_mX(TXa) >
INLINE auto constexpr abs ( TXa const& a ) NOEXCEPT {
  return tX_real::operator_abs ( a );
}
template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >
INLINE auto constexpr abs ( T const& a ) NOEXCEPT {
  return tX_real::operator_abs<A> ( a );
}
template < typename TXa, T_mX(TXa) >
INLINE auto constexpr fabs ( TXa const& a ) NOEXCEPT {
  return tX_real::operator_abs ( a );
}
template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >
INLINE auto constexpr fabs ( T const& a ) NOEXCEPT {
  return tX_real::operator_abs<A> ( a );
}
//
template < typename T, Algorithm A >
INLINE tX_real::tx_real<T,A> constexpr tX_real::tx_real<T,A>::abs ( tX_real::tx_real<T,A> const& a ) NOEXCEPT {
  return tX_real::operator_abs ( a );
}
//

//
// Square root
//
//
template < typename T, Algorithm A >
INLINE auto constexpr operator_sqrt_body ( qX_real::qx_real<T,A> const& a ) NOEXCEPT {
  using TX = tX_real::tx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::sqrt_QW_TW ( _QX_(a), _TX_(c) );
  } else {
    QxW::sqrt_QQW_QTW ( _QX_(a), _TX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm A >
INLINE auto constexpr operator_sqrt_body ( tX_real::tx_real<T,A> const& a ) NOEXCEPT {
  using TX = tX_real::tx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::sqrt_TW_TW ( _TX_(a), _TX_(c) );
  } else {
    QxW::sqrt_QTW_QTW ( _TX_(a), _TX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm A >
INLINE auto constexpr operator_sqrt_body ( dX_real::dx_real<T,A> const& a ) NOEXCEPT {
  using TX = tX_real::tx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::sqrt_DW_TW ( _DX_(a), _TX_(c) );
  } else {
    QxW::sqrt_PA_QTW ( _DX_(a), _TX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < Algorithm A, typename T, T_fp(T) >
INLINE auto constexpr operator_sqrt_body ( T const& a ) NOEXCEPT {
  using TX = tX_real::tx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::sqrt_SW_TW ( _SX_(a), _TX_(c) );
  } else {
    QxW::sqrt_SW_QTW ( _SX_(a), _TX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
//
#if MX_REAL_USE_INF_NAN_EXCEPTION
template < Algorithm A, typename T, T_fp(T) >
INLINE auto constexpr operator_sqrt_exception ( T const& a, bool & flag ) NOEXCEPT {
  using TX = tX_real::tx_real<T,A>;
  flag = true; {
    if ( fp<T>::isnan( a ) ) { auto c=a; return TX{ c,c,c }; }
    if ( fp<T>::is_negative( a ) ) { return TX::nan(); }
    if ( fp<T>::isinf( a ) ) { return TX::inf(); }
    if ( fp<T>::is_zero( a ) ) { auto c=a; return TX{ c,c,c }; }
  }
  //
  flag = false; return TX::zero();
}
template < typename TXa, T_mX(TXa) >
INLINE auto constexpr operator_sqrt_exception ( TXa const& a, bool & flag ) NOEXCEPT {
  Algorithm constexpr A = TXa::base_A;
  auto sa = a.quick_Normalized();
  return tX_real::operator_sqrt_exception<A> ( sa, flag );
}
#endif
//
template < typename TXa, T_mX(TXa) >
INLINE auto constexpr operator_sqrt ( TXa const& a ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag = false;
  auto e = tX_real::operator_sqrt_exception ( a, flag );
  if ( flag ) { return e; }
#else
  if ( a.is_zero() ) { return tX_real::mX_real<TXa>{ a }; }
#endif
  Algorithm constexpr A = TXa::base_A;
  if ( A != Algorithm::Quasi ) {
    return tX_real::operator_sqrt_body ( a );
  } else {
    return tX_real::operator_sqrt_body ( a.element_rotate() );
  }
}
template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >
INLINE auto constexpr operator_sqrt ( T const& a ) NOEXCEPT {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag = false;
  auto e = tX_real::operator_sqrt_exception<A> ( a, flag );
  if ( flag ) { return e; }
#else
  if ( fp<T>::is_zero( a ) ) { return tX_real::tx_real<T,A>{ a }; }
#endif
  return tX_real::operator_sqrt_body<A> ( a );
}
//
template < typename TXa, T_mX(TXa) >
INLINE auto constexpr sqrt ( TXa const& a ) NOEXCEPT {
  return tX_real::operator_sqrt ( a );
}
template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >
INLINE auto constexpr sqrt ( T const& a ) NOEXCEPT {
  return tX_real::operator_sqrt<A> ( a );
}
//
template < typename T, Algorithm A >
INLINE tX_real::tx_real<T,A> constexpr tX_real::tx_real<T,A>::sqrt ( tX_real::tx_real<T,A> const& a ) NOEXCEPT {
  return tX_real::operator_sqrt ( a );
}
//

//
// f minimum
//
//
template < typename T, Algorithm Aa, Algorithm Ab, A_noQuasi(Aa), A_noQuasi(Ab) >
INLINE auto constexpr operator_fmin_body ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const &b ) NOEXCEPT {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = tX_real::tx_real<T,A>;
  int i=0; for(i=0;i<TX::L-1;i++) { if ( a.x[i] != b.x[i] ) break; }
  return TX{ ( a.x[i] <= b.x[i] ) ? a : b };
}
//
#if MX_REAL_USE_INF_NAN_EXCEPTION
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_fmin_exception ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const &b, bool & flag ) NOEXCEPT {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = tX_real::tx_real<T,A>;
  flag = true; {
    if ( isnan( a ) || isnan( b ) ) { return TX::nan(); }
    if ( isinf( a ) && isinf( b ) ) {
      return ( is_positive( a ) && is_positive( b ) ) ? a : -TX::inf();
    }
    if ( isinf( a ) ) { return is_negative( a ) ? a : b; }
    if ( isinf( b ) ) { return is_negative( b ) ? b : a; }
  }
  //
  flag = false; return TX::zero();
}
#endif
//
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_fmin ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const &b ) NOEXCEPT {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = tX_real::tx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag = false;
  auto e = tX_real::operator_fmin_exception( a, b, flag );
  if ( flag ) { return e; }
#endif
  if ( A == Algorithm::Quasi ) {
    using TT = typename  TX::type_Accurate;
    return TX{ tX_real::operator_fmin_body( TT{ a }, TT{ b } ) };
  } else {
    return tX_real::operator_fmin_body( a, b );
  }
}
//
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr fmin ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const &b ) NOEXCEPT {
  return tX_real::operator_fmin( a, b );
}
//

//
// f maximum
//
//
template < typename T, Algorithm Aa, Algorithm Ab, A_noQuasi(Aa), A_noQuasi(Ab) >
INLINE auto constexpr operator_fmax_body ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const &b ) NOEXCEPT {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = tX_real::tx_real<T,A>;
  int i=0; for(i=0;i<TX::L-1;i++) { if ( a.x[i] != b.x[i] ) break; }
  return TX{ ( a.x[i] >= b.x[i] ) ? a : b };
}
//
#if MX_REAL_USE_INF_NAN_EXCEPTION
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_fmax_exception ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const &b, bool & flag ) NOEXCEPT {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = tX_real::tx_real<T,A>;
  flag = true; {
    if ( isnan( a ) || isnan( b ) ) { return TX::nan(); }
    if ( isinf( a ) && isinf( b ) ) {
      return ( is_negative( a ) && is_negative( b ) ) ? a : TX::inf();
    }
    if ( isinf( a ) ) { return is_positive( a ) ? a : b; }
    if ( isinf( b ) ) { return is_positive( b ) ? b : a; }
  }
  //
  flag = false; return TX::zero();
}
#endif
//
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr operator_fmax ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const &b ) NOEXCEPT {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = tX_real::tx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag = false;
  auto e = tX_real::operator_fmax_exception( a, b, flag );
  if ( flag ) { return e; }
#endif
  if ( A == Algorithm::Quasi ) {
    using TT = typename TX::type_Accurate;
    return TX{ tX_real::operator_fmax_body( TT{ a }, TT{ b } ) };
  } else {
    return tX_real::operator_fmax_body( a, b );
  }
}
//
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto constexpr fmax ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const &b ) NOEXCEPT {
  return tX_real::operator_fmax( a, b );
}
//

//
// PRNG
//
//
template < typename T, Algorithm A, T_fp(T) >
INLINE auto rand () NOEXCEPT {
  using TX = tX_real::tx_real<T,A>;
  auto const f = fp<T>::nhalf() / (1 << 30);
  auto g = f;
  auto r = TX::zero();
  auto const bits = std::numeric_limits<TX>::digits;
  auto const rand_bits = std::numeric_limits<int>::digits - 1;
  for(int i=0; i<bits; i+=rand_bits ) {
    auto b_ = std::rand() & 0x7fffffff;
    T b, c;
    if ( std::is_same<T,float>::value ) {
      b = T( ( b_ & 0x7fff0000 ) >> 16 ) * (1 << 16);
    } else {
      b = T( b_ );
    }
    if ( std::is_same<T,float>::value ) {
      c = b * g;
      r = r + TX{ c };
      b = T( b_ & 0x0000ffff );
    }
    c = b * g;
    r = r + TX{ c };
    g = g * f;
  }
  return r;
}
template < typename T, Algorithm A >
INLINE tX_real::tx_real<T,A> tX_real::tx_real<T,A>::rand () NOEXCEPT {
  return tX_real::rand<T,A>();
}
//


//
//
//
template < typename T, Algorithm Aa >
std::ostream& operator<< ( std::ostream& stream, tX_real::tx_real<T,Aa> const& a ) {
  int constexpr LL = tX_real::tx_real<T,Aa>::L;
#if USE_MPREAL
  mpfr::mpreal t = mpfr::mpreal( a.x[0] );
  for(int i=1; i<LL; i++) { t += mpfr::mpreal( a.x[i] ); }
  stream << t;
#else
  for(int i=0; i<LL; i++) { stream << a.x[i] << ":"; }
#endif
  return stream;
}

}
}


#endif

