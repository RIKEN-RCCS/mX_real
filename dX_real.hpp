#ifndef	DX_REAL_H
#define	DX_REAL_H

#include "mX_real.hpp"


namespace mX_real {
  namespace dX_real {

    template < typename T, Algorithm A=Algorithm::Accurate >
    struct dx_real {

      static_assert( fp<T>::value,
                     "The base type must be out of the defined types, such as float or double." );

      //
      //
      //
      using base_T = T;
      static Algorithm constexpr base_A = A;

      //
      // {DX,TX,QX}_REAL are shortcut type only within dx_real
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
      static int constexpr L = 2;
      T x[L];

      //
      //
      //
      template < Algorithm _A_ >
      using type_with_Algorithm = DX_REAL<_A_>;
      //
      using type_Accurate   = type_with_Algorithm<Algorithm::Accurate>;
      using type_Sloppy   = type_with_Algorithm<Algorithm::Sloppy>;
      using type_Quasi   = type_with_Algorithm<Algorithm::Quasi>;
      //
      using accurate_type   = type_with_Algorithm<accurateAlgorithm<A>::algorithm>;
      using inaccurate_type = type_with_Algorithm<inaccurateAlgorithm<A>::algorithm>;


      //
      // A special member function utilized in constructors
      //
      //
      INLINE T constexpr quick_Normalized () const {
        T s = x[0];
        if ( A == Algorithm::Quasi ) for(int i=1; i<L; i++) { s += x[i]; }
        return s;
      }
      INLINE DX_REAL<> constexpr element_rotate () const {
        T y[L]; for(int i=0; i<L; i++) { y[i] = x[i]; }
        for(int i=0; i<L-1; i++) {
          if ( ! fp<T>::is_zero( y[0] ) ) { return DX_REAL<>( y ); }
          T t = y[0]; for(int j=0; j<L-1; j++) { y[j] = y[j+1]; } y[L-1] = t;
        }
        return DX_REAL<>( y );
      }


      //
      // Simple constructors
      //
      //
      INLINE dx_real() {
        for(int i=0; i<L; i++) { x[i] = fp<T>::zero; }
      }
      INLINE dx_real( T const& x0, T const& x1 ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
        auto t = x0 + x1;
        if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
          for(int i=0; i<L; i++) { x[i] = t; }
        } else
#endif
          { x[0] = x0; x[1] = x1; }
      }
      template < typename Ts, T_scalar(Ts) >
      INLINE dx_real( Ts const &h ) {
        { x[0] = T(h); for(int i=1; i<L; i++) { x[i] = fp<T>::zero; } }
      }
      INLINE dx_real( T const *d ) {
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
      INLINE dx_real( _TX_<T,_A_> const& h ) {
        using TX = _TX_<T,_A_>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
        auto const t = h.quick_Normalized();
        if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
          for(int i=0; i<L; i++) { x[i] = t; }
        } else
#endif
          {
            int constexpr LL = TX::L;
            if ( ( L < LL || A != Algorithm::Quasi ) && _A_ == Algorithm::Quasi ) {
              auto s = h;
              mX_real::Normalize<1>( s );
              copy_with_rounding( x, s.x, L, LL );
            } else {
              copy_with_rounding( x, h.x, L, LL );
            }
            if ( L < LL ) {
              mX_real::Normalize<0>( *this );
            }
          }
      }


      //
      // Copy-assignment operator
      //
      //
      template < typename Ts, T_scalar(Ts) >
      INLINE DX_REAL<> constexpr &operator=( Ts const& h )& {
        x[0] = T(h); for(int i=1; i<L; i++) { x[i] = fp<T>::zero; }
        return *this;
      }
      template < template < typename __T__, Algorithm __A__ > class _TX_, Algorithm _A_, T_mX(_TX_<T,_A_>) >
      INLINE DX_REAL<> constexpr &operator=( _TX_<T,_A_> const& h )& {
        if ( std::is_same< DX_REAL<A>, _TX_<T,_A_> >::value &&
             this == (DX_REAL<>*)(&h) ) { return *this; }
#if MX_REAL_USE_INF_NAN_EXCEPTION
        auto const t = h.quick_Normalized();
        if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
          for(int i=0; i<L; i++) { x[i] = t; }
        } else
#endif
          {
            int constexpr LL = DX_REAL<>::L;
            if ( ( L < LL || A != Algorithm::Quasi ) && _A_ == Algorithm::Quasi ) {
              auto s = h;
              mX_real::Normalize<1>( s );
              copy_with_rounding( x, s.x, L, LL );
            } else {
              copy_with_rounding( x, h.x, L, LL );
            }
            if ( L < LL ) {
              mX_real::Normalize<0>( *this );
            }
          }
        return *this;
      }


      //
      // Casting to other datatypes
      //
      //
      template < typename Ts, T_fp(Ts) >
      explicit
      INLINE operator Ts() const noexcept {
        if ( A == Algorithm::Quasi ) {
          return Ts(this->quick_Normalized());
        } else {
          return Ts(x[0]);
        }
      }
      template < template < typename __T__, Algorithm __A__ > class _TX_, Algorithm _A_, T_mX(_TX_<T,_A_>) >
      explicit
      INLINE operator _TX_<T,_A_>() const noexcept {
        using TX = _TX_<T,_A_>;
        TX c;
        int constexpr LL = TX::L;
#if MX_REAL_USE_INF_NAN_EXCEPTION
        auto const t = this->quick_Normalized();
        if ( fp<T>::isinf( t ) || fp<T>::isnan( t ) ) {
          for(int i=0; i<LL; i++) { c.x[i] = t; }
        } else
#endif
          if ( ( LL < L || _A_ != Algorithm::Quasi ) && A == Algorithm::Quasi ) {
            auto s = *this;
            mX_real::Normalize<1>( s );
            copy_with_rounding( c.x, s.x, LL, L );
          } else {
            copy_with_rounding( c.x, x, LL, L );
          }
        if ( LL < L ) {
          mX_real::Normalize<0>( c );
        }
        return c;
      }


      //
      // unneccessary operators are invalidaded
      //
      //
      auto const operator! () = delete;
      auto const operator~ () = delete;
      auto const operator++ () = delete;
      auto const operator-- () = delete;
      template < typename _T_ > auto const operator&&  ( _T_ a ) = delete;
      template < typename _T_ > auto const operator||  ( _T_ a ) = delete;
      template < typename _T_ > auto const operator&=  ( _T_ a ) = delete;
      template < typename _T_ > auto const operator^=  ( _T_ a ) = delete;
      template < typename _T_ > auto const operator<<=  ( _T_ a ) = delete;
      template < typename _T_ > auto const operator>>=  ( _T_ a ) = delete;
      template < typename _T_ > auto const operator,  ( _T_ a ) = delete;
      template < typename _T_ > auto const operator[]  ( _T_ a ) = delete;
      auto const operator() (...) = delete;


      //
      // static constexpr member functions or constatns
      // contents are always in Accurate format
      // but they follow in the Algorithm argument 'A'
      //
      //
      static INLINE DX_REAL<> constexpr zero () { return DX_REAL<>{ fp<T>::zero }; }
      static INLINE DX_REAL<> constexpr one () { return DX_REAL<>{ fp<T>::one }; }
      static INLINE DX_REAL<> constexpr two () { return DX_REAL<>{ fp<T>::two }; }
      static INLINE DX_REAL<> constexpr half () { return DX_REAL<>{ fp<T>::half }; }
      static INLINE DX_REAL<> constexpr nan () { T c = fp<T>::nan; return DX_REAL<>{ c, c }; }
      static INLINE DX_REAL<> constexpr inf () { T c = fp<T>::inf; return DX_REAL<>{ c, c }; }
      static INLINE DX_REAL<> constexpr epsilon () {
        T c = fp<T>::epsilon * fp<T>::half; c = (c * c) * 2;
        return DX_REAL<>{ c };
      }
      static INLINE DX_REAL<> constexpr denorm_min  () {
        T c = fp<T>::denorm_min;
        return DX_REAL<>{ c };
      }
      static INLINE DX_REAL<> constexpr min  () {
        T c = fp<T>::min;
        for(int i=1; i<L; i++) c = c * ( fp<T>::epsiloni * 2 );
        return DX_REAL<>{ c };
      }
      static INLINE DX_REAL<> constexpr safe_min  () {
        T c = fp<T>::min;
        for(int i=1; i<L; i++) c = c * ( fp<T>::epsiloni * 2 );
        c = std::sqrt( c );
        return DX_REAL<>{ c };
      }
      static INLINE DX_REAL<> constexpr critical_min  () {
        T c = fp<T>::denorm_min;
        for(int i=1; i<L; i++) c = c * ( fp<T>::epsiloni * 2 );
        c = std::sqrt( c );
        return DX_REAL<>{ c };
      }
      static INLINE DX_REAL<> constexpr max  () {
        T c[L];
        c[0] = fp<T>::max;
        for(int i=1; i<L; i++) c[i] = c[i-1] * ( fp<T>::epsilon * fp<T>::half );
        return DX_REAL<>{ c };
      }


      //
      // static member funtions
      // definition is below outside of the struct definition block
      //
      static INLINE DX_REAL<> constexpr abs ( DX_REAL<> const& a );
      static INLINE DX_REAL<> constexpr sqrt ( DX_REAL<> const& a );
      static INLINE DX_REAL<> constexpr rand ();
      static INLINE bool constexpr is_positive ( DX_REAL<> const& a );
      static INLINE bool constexpr signbit ( DX_REAL<> const& a );
      static INLINE bool constexpr is_zero ( DX_REAL<> const& a );
      static INLINE bool constexpr isnan ( DX_REAL<> const& a );
      static INLINE bool constexpr isinf ( DX_REAL<> const& a );
      static INLINE bool constexpr is_negative ( DX_REAL<> const& a );
      //


      //
      // member functions (aka methods)
      //
      //
      INLINE void constexpr Normalize () { mX_real::Normalize( *this ); }
      //
      INLINE bool constexpr is_positive () const { return DX_REAL<>::is_positive( *this ); }
      INLINE bool constexpr signbit () const { return DX_REAL<>::signbit( *this ); }
      INLINE bool constexpr is_zero () const { return DX_REAL<>::is_zero( *this ); }
      INLINE bool constexpr isnan () const { return DX_REAL<>::isnan( *this ); }
      INLINE bool constexpr isinf () const { return DX_REAL<>::isinf( *this ); }
      INLINE bool constexpr is_negative () const { return DX_REAL<>::is_negative( *this ); }
      //


      //  Friend
      template < typename _T_, Algorithm _A_ >
      friend std::ostream& operator<< ( std::ostream& stream, dX_real::dx_real<_T_,_A_> const& a );

    };


    // 
    // get a new type dX_real<T,A> from TX(={m}X_real<T,A>)
    //
    //
    template < typename TX, T_mX(TX) >
    struct mX_real_impl {
      static Algorithm constexpr A = TX::base_A;
      using T    = typename TX::base_T;
      using type = typename dX_real::dx_real<T,A>;
    };
    template < typename TX >
    using mX_real = typename mX_real_impl<TX>::type;


    //
    // Alias names
    //
    //
    template < typename T >
    using dX_real_accurate = dX_real::dx_real<T,Algorithm::Accurate>;
    template < typename T >
    using dX_real_sloppy   = dX_real::dx_real<T,Algorithm::Sloppy>;
    template < typename T >
    using dX_real_quasi    = dX_real::dx_real<T,Algorithm::Quasi>;


    //
    // Bitwise or FP-bit-specific ops
    //
    //
    template < typename T, Algorithm Aa >
    INLINE auto constexpr isnan ( dX_real::dx_real<T,Aa> const& a ) {
      return fp<T>::isnan( a.quick_Normalized() );
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr signbit ( dX_real::dx_real<T,Aa> const& a ) {
      return fp<T>::signbit( a.quick_Normalized() );
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr isinf ( dX_real::dx_real<T,Aa> const& a ) {
      return fp<T>::isinf( a.quick_Normalized() );
    }
    template < typename T, Algorithm Aa >
    INLINE bool constexpr is_positive ( dX_real::dx_real<T,Aa> const& a ) {
      return a.quick_Normalized() > fp<T>::zero;
    }
    template < typename T, Algorithm Aa >
    INLINE bool constexpr is_zero ( dX_real::dx_real<T,Aa> const& a ) {
      return a.quick_Normalized() == fp<T>::zero;
    }
    template < typename T, Algorithm Aa >
    INLINE bool constexpr is_negative ( dX_real::dx_real<T,Aa> const& a ) {
      return a.quick_Normalized() < fp<T>::zero;
    }
    //
    template < typename T, Algorithm A >
    INLINE bool constexpr dX_real::dx_real<T,A>::isinf ( dX_real::dx_real<T,A> const& a ) {
      return dX_real::isinf ( a );
    }
    template < typename T, Algorithm A >
    INLINE bool constexpr dX_real::dx_real<T,A>::isnan ( dX_real::dx_real<T,A> const& a ) {
      return dX_real::isnan ( a );
    }
    template < typename T, Algorithm A >
    INLINE bool constexpr dX_real::dx_real<T,A>::signbit ( dX_real::dx_real<T,A> const& a ) {
      return dX_real::signbit ( a );
    }
    template < typename T, Algorithm A >
    INLINE bool constexpr dX_real::dx_real<T,A>::is_zero ( dX_real::dx_real<T,A> const& a ) {
      return dX_real::is_zero ( a );
    }
    template < typename T, Algorithm A >
    INLINE bool constexpr dX_real::dx_real<T,A>::is_positive ( dX_real::dx_real<T,A> const& a ) {
      return dX_real::is_positive ( a );
    }
    template < typename T, Algorithm A >
    INLINE bool constexpr dX_real::dx_real<T,A>::is_negative ( dX_real::dx_real<T,A> const& a ) {
      return dX_real::is_negative ( a );
    }
    //

    //
    // Comparison, equivalence (==)
    //
    //
    template < typename T, Algorithm A >
    INLINE auto constexpr operator_eq ( dX_real::dx_real<T,A> const& a, dX_real::dx_real<T,A> const& b ) {
      using TX = dX_real::dx_real<T,A>;
      int i=0; for(i=0; i<TX::L-1; i++) {
        if ( a.x[i] != b.x[i] ) { return a.x[i] == b.x[i]; }
      } return a.x[i] == b.x[i];
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator== ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
      Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
      if ( A == Algorithm::Quasi ) {
        using TT = dX_real::dx_real<T,Algorithm::Accurate>;
        return dX_real::operator_eq( TT{ a }, TT{ b } );
      } else {
        return dX_real::operator_eq( a, b );
      }
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator!= ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
      return !(a == b);
    }

    //
    // Comparison, greater than (>)
    //
    //
    template < typename T, Algorithm A >
    INLINE auto constexpr operator_gt ( dX_real::dx_real<T,A> const& a, dX_real::dx_real<T,A> const& b ) {
      using TX = dX_real::dx_real<T,A>;
      int i=0; for(i=0; i<TX::L-1; i++) {
        if ( a.x[i] != b.x[i] ) { return a.x[i] > b.x[i]; }
      } return a.x[i] > b.x[i];
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator> ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
      Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
      if ( A == Algorithm::Quasi ) {
        using TT = dX_real::dx_real<T,Algorithm::Accurate>;
        return dX_real::operator_gt( TT{ a }, TT{ b } );
      } else {
        return dX_real::operator_gt( a, b );
      }
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator<= ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
      return !(a > b);
    }

    //
    // Comparison, less than (<)
    //
    //
    template < typename T, Algorithm A >
    INLINE auto constexpr operator_lt ( dX_real::dx_real<T,A> const& a, dX_real::dx_real<T,A> const& b ) {
      using TX = dX_real::dx_real<T,A>;
      int i=0; for(i=0; i<TX::L-1; i++) {
        if ( a.x[i] != b.x[i] ) { return a.x[i] < b.x[i]; }
      } return a.x[i] < b.x[i];
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator< ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
      Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
      if ( A == Algorithm::Quasi ) {
        using TT = dX_real::dx_real<T,Algorithm::Accurate>;
        return dX_real::operator_lt( TT{ a }, TT{ b } );
      } else {
        return dX_real::operator_lt( a, b );
      }
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator>= ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
      return !(a < b);
    }

    //
    // Unitary operators
    //
    //
    template < typename T, Algorithm Aa >
    INLINE auto constexpr reversed_sign ( dX_real::dx_real<T,Aa> const& a ) {
      return dX_real::dx_real<T,Aa>( -a.x[0],-a.x[1] );
    }
    //
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator+ ( dX_real::dx_real<T,Aa> const& a ) {
      return a;
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator- ( dX_real::dx_real<T,Aa> const& a ) {
      return dX_real::reversed_sign( a );
    }
    //

    //
    // Addition
    //
    //
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::add_QW_QW_DW ( _QX_(a), _QX_(b), _DX_(c) );
      } else {
        QxW::add_QQW_QQW_PA ( _QX_(a), _QX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( qX_real::qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::add_QW_TW_DW ( _QX_(a), _TX_(b), _DX_(c) );
      } else {
        QxW::add_QQW_QTW_PA ( _QX_(a), _TX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( qX_real::qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::add_QW_DW_DW ( _QX_(a), _DX_(b), _DX_(c) );
      } else {
        QxW::add_QQW_PA_PA ( _QX_(a), _DX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator_add_body ( qX_real::qx_real<T,Aa> const& a, T const& b ) {
      using TX = dX_real::dx_real<T,Aa>;
      TX c;
      if ( Aa == Algorithm::Accurate ) {
        QxW::add_QW_SW_DW ( _QX_(a), _SX_(b), _DX_(c) );
      } else {
        QxW::add_QQW_SW_PA ( _QX_(a), _SX_(b), _DX_(c) );
        if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( tX_real::tx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
      return operator_add_body ( b, a );
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::add_TW_TW_DW ( _TX_(a), _TX_(b), _DX_(c) );
      } else {
        QxW::add_QTW_QTW_PA ( _TX_(a), _TX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( tX_real::tx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::add_TW_DW_DW ( _TX_(a), _DX_(b), _DX_(c) );
      } else {
        QxW::add_QTW_PA_PA ( _TX_(a), _DX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator_add_body ( tX_real::tx_real<T,Aa> const& a, T const& b ) {
      using TX = dX_real::dx_real<T,Aa>;
      TX c;
      if ( Aa == Algorithm::Accurate ) {
        QxW::add_TW_SW_DW ( _TX_(a), _SX_(b), _DX_(c) );
      } else {
        QxW::add_QTW_SW_PA ( _TX_(a), _SX_(b), _DX_(c) );
        if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( dX_real::dx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
      return operator_add_body ( b, a );
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( dX_real::dx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
      return operator_add_body ( b, a );
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::add_DW_DW_DW ( _DX_(a), _DX_(b), _DX_(c) );
      } else {
        QxW::add_PA_PA_PA ( _DX_(a), _DX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator_add_body ( dX_real::dx_real<T,Aa> const& a, T const& b ) {
      using TX = dX_real::dx_real<T,Aa>;
      TX c;
      if ( Aa == Algorithm::Accurate ) {
        QxW::add_DW_SW_DW ( _DX_(a), _SX_(b), _DX_(c) );
      } else {
        QxW::add_PA_SW_PA ( _DX_(a), _SX_(b), _DX_(c) );
        if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( T const& a, qX_real::qx_real<T,Ab> const& b ) {
      return operator_add_body ( b, a );
    }
    template < typename T, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( T const& a, tX_real::tx_real<T,Ab> const& b ) {
      return operator_add_body ( b, a );
    }
    template < typename T, Algorithm Ab >
    INLINE auto constexpr operator_add_body ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
      return operator_add_body ( b, a );
    }
    template < typename T, Algorithm A, T_fp(T) >
    INLINE auto constexpr operator_add_body ( T const& a, T const& b ) {
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::add_SW_SW_DW ( _SX_(a), _SX_(b), _DX_(c) );
      } else {
        QxW::add_SW_SW_PA ( _SX_(a), _SX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    //
#if MX_REAL_USE_INF_NAN_EXCEPTION
    template < Algorithm A, typename T, T_fp(T) >
    INLINE auto constexpr operator_add_exception ( T const& a, T const& b, bool & flag ) {
      using TX = dX_real::dx_real<T,A>;
      flag = true; {
        if ( fp<T>::isnan( a ) || fp<T>::isnan( b ) ) {
          auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );
          return TX{ c,c };
        }
        if ( fp<T>::isinf( a ) && fp<T>::isinf( b ) ) {
          auto sign = std::signbit( a ) ^ std::signbit( b );
          auto c = sign ? fp<T>::nan : fp<T>::copysign( fp<T>::inf, a );
          return TX{ c,c };
        }
        if ( fp<T>::isinf( a ) ) {
          auto c = fp<T>::copysign( fp<T>::inf, a );
          return TX{ c,c };
        }
        if ( fp<T>::isinf( b ) ) {
          auto c = fp<T>::copysign( fp<T>::inf, b );
          return TX{ c,c };
        }
      }
      //
      flag = false; return TX::zero();
    }
    template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb), T_assert( std::is_same<typename TXa::base_T,typename TXb::base_T>::value ) >
    INLINE auto constexpr operator_add_exception ( TXa const& a, TXb const& b, bool & flag ) {
      Algorithm constexpr A = commonAlgorithm< TXa::base_A, TXb::base_A >::algorithm;
      auto sa = a.quick_Normalized();
      auto sb = b.quick_Normalized();
      return dX_real::operator_add_exception<A> ( sa, sb, flag );
    }
    template < typename TXa, typename T, T_mX(TXa), T_fp(T), T_assert( std::is_same<typename TXa::base_T,T>::value ) >
    INLINE auto constexpr operator_add_exception ( TXa const& a, T const& b, bool & flag ) {
      Algorithm constexpr A = TXa::base_A;
      auto sa = a.quick_Normalized();
      return dX_real::operator_add_exception<A> ( sa, b, flag );
    }
#endif
    //
    template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb), T_assert( TXa::L >= TXb::L ) >
    INLINE auto constexpr operator_add ( TXa const& a, TXb const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_add_exception ( a, b, flag );
      if ( flag ) { return e; }
#endif
      return dX_real::operator_add_body ( a, b );
    }
    template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb), T_assert( std::is_same<typename TXa::base_T,typename TXb::base_T>::value && TXa::L < TXb::L ) >
    INLINE auto constexpr operator_add ( TXa const& a, TXb const& b ) {
      return dX_real::operator_add ( b, a );
    }
      template < typename TXa, typename T, T_mX(TXa), T_fp(T), T_assert( std::is_same<typename TXa::base_T,T>::value ) >
      INLINE auto constexpr operator_add ( TXa const& a, T const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
        bool flag = false;
        auto e = dX_real::operator_add_exception ( a, b, flag );
        if ( flag ) { return e; }
#endif
        return dX_real::operator_add_body ( a, b );
      }
    template < typename T, typename TXb, T_fp(T), T_mX(TXb), T_assert( std::is_same<T,typename TXb::base_T>::value ) >
    INLINE auto constexpr operator_add ( T const& a, TXb const& b ) {
      return dX_real::operator_add ( b, a );
    }
    template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >
    INLINE auto constexpr operator_add ( T const& a, T const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_add_exception<A> ( a, b, flag );
      if ( flag ) { return e; }
#endif
      return dX_real::operator_add_body<T,A> ( a, b );
    }
    //
    template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb), T_assert( std::is_same<typename TXa::base_T,typename TXb::base_T>::value && std::max( TXa::L, TXb::L ) == 2 ) >
    INLINE auto constexpr operator+ ( TXa const& a, TXb const& b ) {
      return dX_real::operator_add ( a, b );
    }
    template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >
    INLINE auto constexpr operator+ ( dX_real::dx_real<T,A> const& a, Ts const& b ) {
      return dX_real::operator_add ( a, T(b) );
    }
    template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >
    INLINE auto constexpr operator+ ( Ts const& a, dX_real::dx_real<T,A> const& b ) {
      return b + a;
    }
    //

    //
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_ow_body ( dX_real::dx_real<T,Aa> & a, qX_real::qx_real<T,Ab> const& b ) {
      Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
      if ( A == Algorithm::Accurate ) {
        QxW::add_DW_QW_DW ( _DX_(a), _QX_(b), _DX_(a) );
      } else {
        QxW::add_PA_QQW_PA ( _DX_(a), _QX_(b), _DX_(a) );
        if ( A != Algorithm::Quasi ) { Normalize( a ); }
      }
      return a;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_ow_body ( dX_real::dx_real<T,Aa> & a, tX_real::tx_real<T,Ab> const& b ) {
      Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
      if ( A == Algorithm::Accurate ) {
        QxW::add_DW_TW_DW ( _DX_(a), _TX_(b), _DX_(a) );
      } else {
        QxW::add_PA_QTW_PA ( _DX_(a), _TX_(b), _DX_(a) );
        if ( A != Algorithm::Quasi ) { Normalize( a ); }
      }
      return a;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_add_ow_body ( dX_real::dx_real<T,Aa> & a, dX_real::dx_real<T,Ab> const& b ) {
      Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
      if ( A == Algorithm::Accurate ) {
        QxW::add_DW_DW_DW ( _DX_(a), _DX_(b), _DX_(a) );
      } else {
        QxW::add_PA_PA_PA ( _DX_(a), _DX_(b), _DX_(a) );
        if ( A != Algorithm::Quasi ) { Normalize( a ); }
      }
      return a;
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator_add_ow_body ( dX_real::dx_real<T,Aa> & a, T const& b ) {
      if ( Aa == Algorithm::Accurate ) {
        QxW::add_DW_SW_DW ( _DX_(a), _SX_(b), _DX_(a) );
      } else {
        QxW::add_PA_SW_PA ( _DX_(a), _SX_(b), _DX_(a) );
        if ( Aa != Algorithm::Quasi ) { Normalize( a ); }
      }
      return a;
    }
    //
    template < typename T, Algorithm Aa, template < typename _Tb_, Algorithm _Ab_ > class TXb, Algorithm Ab, T_mX(TXb<T,Ab>), A_owAble(Aa,Ab) >
    INLINE auto constexpr operator_add_ow ( dX_real::dx_real<T,Aa> & a, TXb<T,Ab> const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_add_exception ( a, b, flag );
      if ( flag ) { return e; }
#endif
      return dX_real::operator_add_ow_body ( a, b );
    }
    template < typename T, Algorithm A >
    INLINE auto constexpr operator_add_ow ( dX_real::dx_real<T,A> & a, T const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_add_exception ( a, b, flag );
      if ( flag ) { return e; }
#endif
      return dX_real::operator_add_ow_body ( a, b );
    }
    //
    template < typename T, Algorithm Aa, template < typename _Tb_, Algorithm _Ab_ > class TXb, Algorithm Ab, T_mX(TXb<T,Ab>), A_owAble(Aa,Ab) >
    INLINE auto constexpr operator+= ( dX_real::dx_real<T,Aa> & a, TXb<T,Ab> const& b ) {
      return dX_real::operator_add_ow ( a, b );
    }
    template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >
    INLINE auto constexpr operator+= ( dX_real::dx_real<T,A> & a, Ts const& b ) {
      return dX_real::operator_add_ow ( a, T(b) );
    }
    //

    //
    // Substraction
    //
    //
    template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb), T_assert( std::is_same<typename TXa::base_T,typename TXb::base_T>::value && std::max( TXa::L, TXb::L ) ==2 ) >
    INLINE auto constexpr operator- ( TXa const& a, TXb const& b ) {
      return a + (-b);
    }
    template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >
    INLINE auto constexpr operator- ( dX_real::dx_real<T,A> const& a, Ts const& b ) {
      return a + (-b);
    }
    template < Algorithm A=Algorithm::Accurate, typename T, typename Ts, T_scalar(Ts) >
    INLINE auto constexpr operator- ( Ts const &a, dX_real::dx_real<T,A> const& b ) {
      return a + (-b);
    }
    //

    //
    template < typename T, Algorithm Aa, Algorithm Ab, template < typename _Tb_, Algorithm _Ab_ > class TXb, T_mX(TXb<T,Ab>), A_owAble(Aa,Ab) >
    INLINE auto constexpr operator-= ( dX_real::dx_real<T,Aa> & a, TXb<T,Ab> const& b ) {
      return a += (-b);
    }
    template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >
    INLINE auto constexpr operator-= ( dX_real::dx_real<T,A> & a, Ts const& b ) {
      return a += (-b);
    }
    //

    //
    // Multiplication
    //
    //
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_mul_body ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        if ( (void *)(&a) == (void *)(&b) ) {
          QxW::sqr_QW_DW ( _QX_(a), _DX_(c) );
        } else {
          QxW::mul_QW_QW_DW ( _QX_(a), _QX_(b), _DX_(c) );
        }
      } else {
        if ( (void *)(&a) == (void *)(&b) ) {
          QxW::sqr_QQW_PA ( _QX_(a), _DX_(c) );
        } else {
          QxW::mul_QQW_QQW_PA ( _QX_(a), _QX_(b), _DX_(c) );
        }
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_mul_body ( qX_real::qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::mul_QW_TW_DW ( _QX_(a), _TX_(b), _DX_(c) );
      } else {
        QxW::mul_QQW_QTW_PA ( _QX_(a), _TX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_mul_body ( qX_real::qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::mul_QW_DW_DW ( _QX_(a), _DX_(b), _DX_(c) );
      } else {
        QxW::mul_QQW_PA_PA ( _QX_(a), _DX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator_mul_body ( qX_real::qx_real<T,Aa> const& a, T const& b ) {
      using TX = dX_real::dx_real<T,Aa>;
      TX c;
      if ( Aa == Algorithm::Accurate ) {
        QxW::mul_QW_SW_DW ( _QX_(a), _SX_(b), _DX_(c) );
      } else {
        QxW::mul_QQW_SW_PA ( _QX_(a), _SX_(b), _DX_(c) );
        if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_mul_body ( tX_real::tx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
      return operator_mul_body ( b, a );
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_mul_body ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        if ( (void *)(&a) == (void *)(&b) ) {
          QxW::sqr_TW_DW ( _TX_(a), _DX_(c) );
        } else {
          QxW::mul_TW_TW_DW ( _TX_(a), _TX_(b), _DX_(c) );
        }
      } else {
        if ( (void *)(&a) == (void *)(&b) ) {
          QxW::sqr_QTW_PA ( _TX_(a), _DX_(c) );
        } else {
          QxW::mul_QTW_QTW_PA ( _TX_(a), _TX_(b), _DX_(c) );
        }
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_mul_body ( tX_real::tx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::mul_TW_DW_DW ( _TX_(a), _DX_(b), _DX_(c) );
      } else {
        QxW::mul_QTW_PA_PA ( _TX_(a), _DX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator_mul_body ( tX_real::tx_real<T,Aa> const& a, T const& b ) {
      using TX = dX_real::dx_real<T,Aa>;
      TX c;
      if ( Aa == Algorithm::Accurate ) {
        QxW::mul_TW_SW_DW ( _TX_(a), _SX_(b), _DX_(c) );
      } else {
        QxW::mul_QTW_SW_PA ( _TX_(a), _SX_(b), _DX_(c) );
        if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_mul_body ( dX_real::dx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
      return operator_mul_body ( b, a );
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_mul_body ( dX_real::dx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
      return operator_mul_body ( b, a );
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_mul_body ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        if ( (void *)(&a) == (void *)(&b) ) {
          QxW::sqr_DW_DW ( _DX_(a), _DX_(c) );
        } else {
          QxW::mul_DW_DW_DW ( _DX_(a), _DX_(b), _DX_(c) );
        }
      } else {
        if ( (void *)(&a) == (void *)(&b) ) {
          QxW::sqr_PA_PA ( _DX_(a), _DX_(c) );
        } else {
          QxW::mul_PA_PA_PA ( _DX_(a), _DX_(b), _DX_(c) );
        }
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator_mul_body ( dX_real::dx_real<T,Aa> const& a, T const& b ) {
      using TX = dX_real::dx_real<T,Aa>;
      TX c;
      if ( Aa == Algorithm::Accurate ) {
        QxW::mul_DW_SW_DW ( _DX_(a), _SX_(b), _DX_(c) );
      } else {
        QxW::mul_PA_SW_PA ( _DX_(a), _SX_(b), _DX_(c) );
        if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Ab >
    INLINE auto constexpr operator_mul_body ( T const& a, qX_real::qx_real<T,Ab> const& b ) {
      return operator_mul_body ( b, a );
    }
    template < typename T, Algorithm Ab >
    INLINE auto constexpr operator_mul_body ( T const& a, tX_real::tx_real<T,Ab> const& b ) {
      return operator_mul_body ( b, a );
    }
    template < typename T, Algorithm Ab >
    INLINE auto constexpr operator_mul_body ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
      return operator_mul_body ( b, a );
    }
    template < typename T, Algorithm A, T_fp(T) >
    INLINE auto constexpr operator_mul_body ( T const& a, T const& b ) {
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        if ( (void *)(&a) == (void *)(&b) ) {
          QxW::sqr_SW_DW ( _SX_(a), _DX_(c) );
        } else {
          QxW::mul_SW_SW_DW ( _SX_(a), _SX_(b), _DX_(c) );
        }
      } else {
        if ( (void *)(&a) == (void *)(&b) ) {
          QxW::sqr_SW_PA ( _SX_(a), _DX_(c) );
        } else {
          QxW::mul_SW_SW_PA ( _SX_(a), _SX_(b), _DX_(c) );
        }
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    //
#if MX_REAL_USE_INF_NAN_EXCEPTION
    template < Algorithm A, typename T, T_fp(T) >
    INLINE auto constexpr operator_mul_exception ( T const& a, T const& b, bool & flag ) {
      using TX = dX_real::dx_real<T,A>;
      flag = true; {
        if ( fp<T>::isnan( a ) || fp<T>::isnan( b ) ) {
          auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );
          return TX{ c,c };
        }
        if ( fp<T>::isinf( a ) && fp<T>::isinf( b ) ) {
          auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::inf, a ), b );
          return TX{ c,c };
        }
        if ( fp<T>::isinf( a ) ) {
          if ( fp<T>::is_zero( b ) ) {
            auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );
            return TX{ c,c };
          } else {
            auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::inf, a ), b );
            return TX{ c,c };
          }
        }
        if ( fp<T>::isinf( b ) ) {
          if ( fp<T>::is_zero( a ) ) {
            auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );
            return TX{ c,c };
          } else {
            auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::inf, a ), b );
            return TX{ c,c };
          }
        }
        if ( fp<T>::is_zero( a ) || fp<T>::is_zero( b ) ) {
          auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::zero, a ), b );
          return TX{ c,c };
        }
      }
      //
      flag = false; return TX::zero();
    }
    template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb), T_assert( std::is_same<typename TXa::base_T,typename TXb::base_T>::value ) >
    INLINE auto constexpr operator_mul_exception ( TXa const& a, TXb const& b, bool & flag ) {
      Algorithm constexpr A = commonAlgorithm< TXa::base_A, TXb::base_A >::algorithm;
      auto sa = a.quick_Normalized();
      auto sb = b.quick_Normalized();
      return dX_real::operator_mul_exception<A> ( sa, sb, flag );
    }
    template < typename TXa, typename T, T_mX(TXa), T_fp(T), T_assert( std::is_same<typename TXa::base_T,T>::value ) >
    INLINE auto constexpr operator_mul_exception ( TXa const& a, T const& b, bool & flag ) {
      Algorithm constexpr A = TXa::base_A;
      auto sa = a.quick_Normalized();
      return dX_real::operator_mul_exception<A> ( sa, b, flag );
    }
#endif
    //
    template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb), T_assert( TXa::L >= TXb::L ) >
    INLINE auto constexpr operator_mul ( TXa const& a, TXb const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_mul_exception ( a, b, flag );
      if ( flag ) { return e; }
#endif
      return dX_real::operator_mul_body ( a, b );
    }
    template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb), T_assert( std::is_same<typename TXa::base_T,typename TXb::base_T>::value && TXa::L < TXb::L ) >
    INLINE auto constexpr operator_mul ( TXa const& a, TXb const& b ) {
      return dX_real::operator_mul ( b, a );
    }
      template < typename TXa, typename T, T_mX(TXa), T_fp(T), T_assert( std::is_same<typename TXa::base_T,T>::value ) >
      INLINE auto constexpr operator_mul ( TXa const& a, T const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
        bool flag = false;
        auto e = dX_real::operator_mul_exception ( a, b, flag );
        if ( flag ) { return e; }
#endif
        return dX_real::operator_mul_body ( a, b );
      }
    template < typename T, typename TXb, T_fp(T), T_mX(TXb), T_assert( std::is_same<T,typename TXb::base_T>::value ) >
    INLINE auto constexpr operator_mul ( T const& a, TXb const& b ) {
      return dX_real::operator_mul ( b, a );
    }
    template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >
    INLINE auto constexpr operator_mul ( T const& a, T const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_mul_exception<A> ( a, b, flag );
      if ( flag ) { return e; }
#endif
      return dX_real::operator_mul_body<T,A> ( a, b );
    }
    //
    template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb), T_assert( std::is_same<typename TXa::base_T,typename TXb::base_T>::value && std::max( TXa::L, TXb::L ) == 2 ) >
    INLINE auto constexpr operator* ( TXa const& a, TXb const& b ) {
      return dX_real::operator_mul ( a, b );
    }
    template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >
    INLINE auto constexpr operator* ( dX_real::dx_real<T,A> const& a, Ts const& b ) {
      return dX_real::operator_mul ( a, T(b) );
    }
    template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >
    INLINE auto constexpr operator* ( Ts const& a, dX_real::dx_real<T,A> const& b ) {
      return b * a;
    }
    //

    //
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_mul_ow_body ( dX_real::dx_real<T,Aa> & a, qX_real::qx_real<T,Ab> const& b ) {
      Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
      if ( A == Algorithm::Accurate ) {
        QxW::mul_DW_QW_DW ( _DX_(a), _QX_(b), _DX_(a) );
      } else {
        QxW::mul_PA_QQW_PA ( _DX_(a), _QX_(b), _DX_(a) );
        if ( A != Algorithm::Quasi ) { Normalize( a ); }
      }
      return a;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_mul_ow_body ( dX_real::dx_real<T,Aa> & a, tX_real::tx_real<T,Ab> const& b ) {
      Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
      if ( A == Algorithm::Accurate ) {
        QxW::mul_DW_TW_DW ( _DX_(a), _TX_(b), _DX_(a) );
      } else {
        QxW::mul_PA_QTW_PA ( _DX_(a), _TX_(b), _DX_(a) );
        if ( A != Algorithm::Quasi ) { Normalize( a ); }
      }
      return a;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_mul_ow_body ( dX_real::dx_real<T,Aa> & a, dX_real::dx_real<T,Ab> const& b ) {
      Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
      if ( A == Algorithm::Accurate ) {
        QxW::mul_DW_DW_DW ( _DX_(a), _DX_(b), _DX_(a) );
      } else {
        QxW::mul_PA_PA_PA ( _DX_(a), _DX_(b), _DX_(a) );
        if ( A != Algorithm::Quasi ) { Normalize( a ); }
      }
      return a;
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator_mul_ow_body ( dX_real::dx_real<T,Aa> & a, T const& b ) {
      if ( Aa == Algorithm::Accurate ) {
        QxW::mul_DW_SW_DW ( _DX_(a), _SX_(b), _DX_(a) );
      } else {
        QxW::mul_PA_SW_PA ( _DX_(a), _SX_(b), _DX_(a) );
        if ( Aa != Algorithm::Quasi ) { Normalize( a ); }
      }
      return a;
    }
    //
    template < typename T, Algorithm Aa, template < typename _Tb_, Algorithm _Ab_ > class TXb, Algorithm Ab, T_mX(TXb<T,Ab>), A_owAble(Aa,Ab) >
    INLINE auto constexpr operator_mul_ow ( dX_real::dx_real<T,Aa> & a, TXb<T,Ab> const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_mul_exception ( a, b, flag );
      if ( flag ) { return e; }
#endif
      return dX_real::operator_mul_ow_body ( a, b );
    }
    template < typename T, Algorithm A >
    INLINE auto constexpr operator_mul_ow ( dX_real::dx_real<T,A> & a, T const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_mul_exception ( a, b, flag );
      if ( flag ) { return e; }
#endif
      return dX_real::operator_mul_ow_body ( a, b );
    }
    //
    template < typename T, Algorithm Aa, template < typename _Tb_, Algorithm _Ab_ > class TXb, Algorithm Ab, T_mX(TXb<T,Ab>), A_owAble(Aa,Ab) >
    INLINE auto constexpr operator*= ( dX_real::dx_real<T,Aa> & a, TXb<T,Ab> const& b ) {
      return dX_real::operator_mul_ow ( a, b );
    }
    template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >
    INLINE auto constexpr operator*= ( dX_real::dx_real<T,A> & a, Ts const& b ) {
      return dX_real::operator_mul_ow ( a, T(b) );
    }
    //

    //
    // Division
    //
    //
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_div_body ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::div_QW_QW_DW ( _QX_(a), _QX_(b), _DX_(c) );
      } else {
        QxW::div_QQW_QQW_PA ( _QX_(a), _QX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_div_body ( qX_real::qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::div_QW_TW_DW ( _QX_(a), _TX_(b), _DX_(c) );
      } else {
        QxW::div_QQW_QTW_PA ( _QX_(a), _TX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_div_body ( qX_real::qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::div_QW_DW_DW ( _QX_(a), _DX_(b), _DX_(c) );
      } else {
        QxW::div_QQW_PA_PA ( _QX_(a), _DX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator_div_body ( qX_real::qx_real<T,Aa> const& a, T const& b ) {
      using TX = dX_real::dx_real<T,Aa>;
      TX c;
      if ( Aa == Algorithm::Accurate ) {
        QxW::div_QW_SW_DW ( _QX_(a), _SX_(b), _DX_(c) );
      } else {
        QxW::div_QQW_SW_PA ( _QX_(a), _SX_(b), _DX_(c) );
        if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_div_body ( tX_real::tx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::div_TW_QW_DW ( _TX_(a), _QX_(b), _DX_(c) );
      } else {
        QxW::div_QTW_QQW_PA ( _TX_(a), _QX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_div_body ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::div_TW_TW_DW ( _TX_(a), _TX_(b), _DX_(c) );
      } else {
        QxW::div_QTW_QTW_PA ( _TX_(a), _TX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_div_body ( tX_real::tx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::div_TW_DW_DW ( _TX_(a), _DX_(b), _DX_(c) );
      } else {
        QxW::div_QTW_PA_PA ( _TX_(a), _DX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator_div_body ( tX_real::tx_real<T,Aa> const& a, T const& b ) {
      using TX = dX_real::dx_real<T,Aa>;
      TX c;
      if ( Aa == Algorithm::Accurate ) {
        QxW::div_TW_SW_DW ( _TX_(a), _SX_(b), _DX_(c) );
      } else {
        QxW::div_QTW_SW_PA ( _TX_(a), _SX_(b), _DX_(c) );
        if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_div_body ( dX_real::dx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::div_DW_QW_DW ( _DX_(a), _QX_(b), _DX_(c) );
      } else {
        QxW::div_PA_QQW_PA ( _DX_(a), _QX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_div_body ( dX_real::dx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::div_DW_TW_DW ( _DX_(a), _TX_(b), _DX_(c) );
      } else {
        QxW::div_PA_QTW_PA ( _DX_(a), _TX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_div_body ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::div_DW_DW_DW ( _DX_(a), _DX_(b), _DX_(c) );
      } else {
        QxW::div_PA_PA_PA ( _DX_(a), _DX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator_div_body ( dX_real::dx_real<T,Aa> const& a, T const& b ) {
      using TX = dX_real::dx_real<T,Aa>;
      TX c;
      if ( Aa == Algorithm::Accurate ) {
        QxW::div_DW_SW_DW ( _DX_(a), _SX_(b), _DX_(c) );
      } else {
        QxW::div_PA_SW_PA ( _DX_(a), _SX_(b), _DX_(c) );
        if ( Aa != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Ab >
    INLINE auto constexpr operator_div_body ( T const& a, qX_real::qx_real<T,Ab> const& b ) {
      using TX = dX_real::dx_real<T,Ab>;
      TX c;
      if ( Ab == Algorithm::Accurate ) {
        QxW::div_SW_QW_DW ( _SX_(a), _QX_(b), _DX_(c) );
      } else {
        QxW::div_SW_QQW_PA ( _SX_(a), _QX_(b), _DX_(c) );
        if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Ab >
    INLINE auto constexpr operator_div_body ( T const& a, tX_real::tx_real<T,Ab> const& b ) {
      using TX = dX_real::dx_real<T,Ab>;
      TX c;
      if ( Ab == Algorithm::Accurate ) {
        QxW::div_SW_TW_DW ( _SX_(a), _TX_(b), _DX_(c) );
      } else {
        QxW::div_SW_QTW_PA ( _SX_(a), _TX_(b), _DX_(c) );
        if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm Ab >
    INLINE auto constexpr operator_div_body ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
      using TX = dX_real::dx_real<T,Ab>;
      TX c;
      if ( Ab == Algorithm::Accurate ) {
        QxW::div_SW_DW_DW ( _SX_(a), _DX_(b), _DX_(c) );
      } else {
        QxW::div_SW_PA_PA ( _SX_(a), _DX_(b), _DX_(c) );
        if ( Ab != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm A, T_fp(T) >
    INLINE auto constexpr operator_div_body ( T const& a, T const& b ) {
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::div_SW_SW_DW ( _SX_(a), _SX_(b), _DX_(c) );
      } else {
        QxW::div_SW_SW_PA ( _SX_(a), _SX_(b), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    //
#if MX_REAL_USE_INF_NAN_EXCEPTION
    template < Algorithm A, typename T, T_fp(T) >
    INLINE auto constexpr operator_div_exception ( T const& a, T const& b, bool & flag ) {
      using TX = dX_real::dx_real<T,A>;
      flag = true; {
        if ( fp<T>::isnan( a ) || fp<T>::isnan( b ) ) {
          auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );
          return TX{ c,c };
        }
        if ( fp<T>::isinf( a ) && fp<T>::isinf( b ) ) {
          auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );
          return TX{ c,c };
        }
        if ( fp<T>::isinf( a ) ) {
          auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::inf, a ), b );
          return TX{ c,c };
        }
        if ( fp<T>::isinf( b ) ) {
          auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::zero, a ), b );
          return TX{ c,c };
        }
        if ( fp<T>::is_zero( b ) ) {
          if ( fp<T>::is_zero( a ) ) {
            auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );
            return TX{ c,c };
          } else {
            auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::inf, a ), b );
            return TX{ c,c };
          }
        }
      }
      //
      flag = false; return TX::zero();
    }
    template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb), T_assert( std::is_same<typename TXa::base_T,typename TXb::base_T>::value ) >
    INLINE auto constexpr operator_div_exception ( TXa const& a, TXb const& b, bool & flag ) {
      Algorithm constexpr A = commonAlgorithm< TXa::base_A, TXb::base_A >::algorithm;
      auto sa = a.quick_Normalized();
      auto sb = b.quick_Normalized();
      return dX_real::operator_div_exception<A> ( sa, sb, flag );
    }
    template < typename TXa, typename T, T_mX(TXa), T_fp(T), T_assert( std::is_same<typename TXa::base_T,T>::value ) >
    INLINE auto constexpr operator_div_exception ( TXa const& a, T const& b, bool & flag ) {
      Algorithm constexpr A = TXa::base_A;
      auto sa = a.quick_Normalized();
      return dX_real::operator_div_exception<A> ( sa, b, flag );
    }
    template < typename T, typename TXb, T_fp(T), T_mX(TXb), T_assert( std::is_same<T,typename TXb::base_T>::value ) >
    INLINE auto constexpr operator_div_exception ( T const& a, TXb const& b, bool & flag ) {
      Algorithm constexpr A = TXb::base_A;
      auto sb = b.quick_Normalized();
      return dX_real::operator_div_exception<A> ( a, sb, flag );
    }
#endif
    //
    template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb) >
    INLINE auto constexpr operator_div ( TXa const& a, TXb const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_div_exception ( a, b, flag );
      if ( flag ) { return e; }
#endif
      return dX_real::operator_div_body ( a, b );
    }
    template < typename TXa, typename T, T_mX(TXa), T_fp(T), T_assert( std::is_same<typename TXa::base_T,T>::value ) >
    INLINE auto constexpr operator_div ( TXa const& a, T const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_div_exception ( a, b, flag );
      if ( flag ) { return e; }
#endif
      return dX_real::operator_div_body ( a, b );
    }
    template < typename T, typename TXb, T_fp(T), T_mX(TXb), T_assert( std::is_same<T,typename TXb::base_T>::value ) >
    INLINE auto constexpr operator_div ( T const& a, TXb const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_div_exception ( a, b, flag );
      if ( flag ) { return e; }
#endif
      return dX_real::operator_div_body ( a, b );
    }
    template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >
    INLINE auto constexpr operator_div ( T const& a, T const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_div_exception<A> ( a, b, flag );
      if ( flag ) { return e; }
#endif
      return dX_real::operator_div_body<T,A> ( a, b );
    }
    //
    template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb), T_assert( std::is_same<typename TXa::base_T,typename TXb::base_T>::value && std::max( TXa::L, TXb::L ) == 2 ) >
    INLINE auto constexpr operator/ ( TXa const& a, TXb const& b ) {
      return dX_real::operator_div ( a, b );
    }
    template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >
    INLINE auto constexpr operator/ ( dX_real::dx_real<T,A> const& a, Ts const& b ) {
      return dX_real::operator_div ( a, T(b) );
    }
    template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >
    INLINE auto constexpr operator/ ( Ts const& a, dX_real::dx_real<T,A> const& b ) {
      return dX_real::operator_div ( T(a), b );
    }
    //

    //
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_div_ow_body ( dX_real::dx_real<T,Aa> & a, qX_real::qx_real<T,Ab> const& b ) {
      Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
      if ( A == Algorithm::Accurate ) {
        QxW::div_DW_QW_DW ( _DX_(a), _QX_(b), _DX_(a) );
      } else {
        QxW::div_PA_QQW_PA ( _DX_(a), _QX_(b), _DX_(a) );
        if ( A != Algorithm::Quasi ) { Normalize( a ); }
      }
      return a;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_div_ow_body ( dX_real::dx_real<T,Aa> & a, tX_real::tx_real<T,Ab> const& b ) {
      Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
      if ( A == Algorithm::Accurate ) {
        QxW::div_DW_TW_DW ( _DX_(a), _TX_(b), _DX_(a) );
      } else {
        QxW::div_PA_QTW_PA ( _DX_(a), _TX_(b), _DX_(a) );
        if ( A != Algorithm::Quasi ) { Normalize( a ); }
      }
      return a;
    }
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_div_ow_body ( dX_real::dx_real<T,Aa> & a, dX_real::dx_real<T,Ab> const& b ) {
      Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;
      if ( A == Algorithm::Accurate ) {
        QxW::div_DW_DW_DW ( _DX_(a), _DX_(b), _DX_(a) );
      } else {
        QxW::div_PA_PA_PA ( _DX_(a), _DX_(b), _DX_(a) );
        if ( A != Algorithm::Quasi ) { Normalize( a ); }
      }
      return a;
    }
    template < typename T, Algorithm Aa >
    INLINE auto constexpr operator_div_ow_body ( dX_real::dx_real<T,Aa> & a, T const& b ) {
      if ( Aa == Algorithm::Accurate ) {
        QxW::div_DW_SW_DW ( _DX_(a), _SX_(b), _DX_(a) );
      } else {
        QxW::div_PA_SW_PA ( _DX_(a), _SX_(b), _DX_(a) );
        if ( Aa != Algorithm::Quasi ) { Normalize( a ); }
      }
      return a;
    }
    //
    template < typename T, Algorithm Aa, template < typename _Tb_, Algorithm _Ab_ > class TXb, Algorithm Ab, T_mX(TXb<T,Ab>), A_owAble(Aa,Ab) >
    INLINE auto constexpr operator_div_ow ( dX_real::dx_real<T,Aa> & a, TXb<T,Ab> const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_div_exception ( a, b, flag );
      if ( flag ) { return e; }
#endif
      return dX_real::operator_div_ow_body ( a, b );
    }
    template < typename T, Algorithm A >
    INLINE auto constexpr operator_div_ow ( dX_real::dx_real<T,A> & a, T const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_div_exception ( a, b, flag );
      if ( flag ) { return e; }
#endif
      return dX_real::operator_div_ow_body ( a, b );
    }
    //
    template < typename T, Algorithm Aa, template < typename _Tb_, Algorithm _Ab_ > class TXb, Algorithm Ab, T_mX(TXb<T,Ab>), A_owAble(Aa,Ab) >
    INLINE auto constexpr operator/= ( dX_real::dx_real<T,Aa> & a, TXb<T,Ab> const& b ) {
      return dX_real::operator_div_ow ( a, b );
    }
    template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >
    INLINE auto constexpr operator/= ( dX_real::dx_real<T,A> & a, Ts const& b ) {
      return dX_real::operator_div_ow ( a, T(b) );
    }
    //

    //
    // Absolute value
    //
    //
    template < typename TXa, T_mX(TXa) >
    INLINE auto constexpr operator_abs_body ( TXa const& a ) {
      return dX_real::mX_real<TXa>{ is_negative( a ) ? -a : a };
    }
    //
#if MX_REAL_USE_INF_NAN_EXCEPTION
    template < Algorithm A, typename T, T_fp(T) >
    INLINE auto constexpr operator_abs_exception ( T const& a, bool & flag ) {
      using TX = dX_real::dx_real<T,A>;
      flag = true; {
        if ( fp<T>::isnan( a ) ) { TX::nan(); }
        if ( fp<T>::isinf( a ) ) { TX::inf(); }
      }
      //
      flag = false; return TX::zero();
    }
    template < typename TXa, T_mX(TXa) >
    INLINE auto constexpr operator_abs_exception ( TXa const& a, bool & flag ) {
      Algorithm constexpr A = TXa::base_A;
      auto sa = a.quick_Normalized();
      return dX_real::operator_abs_exception<A> ( sa, flag );
    }
#endif
    //
    template < typename TXa, T_mX(TXa) >
    INLINE auto constexpr operator_abs ( TXa const& a ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_abs_exception ( a, flag );
      if ( flag ) { return e; }
#endif
      return dX_real::operator_abs_body ( a );
    }
    template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >
    INLINE auto constexpr operator_abs ( T const& a ) {
      using TX = dX_real::dx_real<T,A>;
      return TX{ std::abs( a ) };
    }
    //
    template < typename TXa, T_mX(TXa) >
    INLINE auto constexpr abs ( TXa const& a ) {
      return dX_real::operator_abs ( a );
    }
    template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >
    INLINE auto constexpr abs ( T const& a ) {
      return dX_real::operator_abs<A> ( a );
    }
    //
    template < typename T, Algorithm A >
    INLINE dX_real::dx_real<T,A> constexpr dX_real::dx_real<T,A>::abs ( dX_real::dx_real<T,A> const& a ) {
      return dX_real::operator_abs ( a );
    }
    //

    //
    // Square root
    //
    //
    template < typename T, Algorithm A >
    INLINE auto constexpr operator_sqrt_body ( qX_real::qx_real<T,A> const& a ) {
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::sqrt_QW_DW ( _QX_(a), _DX_(c) );
      } else {
        QxW::sqrt_QQW_PA ( _QX_(a), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm A >
    INLINE auto constexpr operator_sqrt_body ( tX_real::tx_real<T,A> const& a ) {
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::sqrt_TW_DW ( _TX_(a), _DX_(c) );
      } else {
        QxW::sqrt_QTW_PA ( _TX_(a), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < typename T, Algorithm A >
    INLINE auto constexpr operator_sqrt_body ( dX_real::dx_real<T,A> const& a ) {
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::sqrt_DW_DW ( _DX_(a), _DX_(c) );
      } else {
        QxW::sqrt_PA_PA ( _DX_(a), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    template < Algorithm A, typename T, T_fp(T) >
    INLINE auto constexpr operator_sqrt_body ( T const& a ) {
      using TX = dX_real::dx_real<T,A>;
      TX c;
      if ( A == Algorithm::Accurate ) {
        QxW::sqrt_SW_DW ( _SX_(a), _DX_(c) );
      } else {
        QxW::sqrt_SW_PA ( _SX_(a), _DX_(c) );
        if ( A != Algorithm::Quasi ) { Normalize( c ); }
      }
      return c;
    }
    //
#if MX_REAL_USE_INF_NAN_EXCEPTION
    template < Algorithm A, typename T, T_fp(T) >
    INLINE auto constexpr operator_sqrt_exception ( T const& a, bool & flag ) {
      using TX = dX_real::dx_real<T,A>;
      flag = true; {
        if ( fp<T>::isnan( a ) ) { auto c=a; return TX{ c,c }; }
        if ( fp<T>::is_negative( a ) ) { return TX::nan(); }
        if ( fp<T>::isinf( a ) ) { return TX::inf(); }
        if ( fp<T>::is_zero( a ) ) { auto c=a; return TX{ c,c }; }
      }
      //
      flag = false; return TX::zero();
    }
    template < typename TXa, T_mX(TXa) >
    INLINE auto constexpr operator_sqrt_exception ( TXa const& a, bool & flag ) {
      Algorithm constexpr A = TXa::base_A;
      auto sa = a.quick_Normalized();
      return dX_real::operator_sqrt_exception<A> ( sa, flag );
    }
#endif
    //
    template < typename TXa, T_mX(TXa) >
    INLINE auto constexpr operator_sqrt ( TXa const& a ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_sqrt_exception ( a, flag );
      if ( flag ) { return e; }
#else
      if ( a.is_zero() ) { return dX_real::mX_real<TXa>{ a }; }
#endif
      Algorithm constexpr A = TXa::base_A;
      if ( A != Algorithm::Quasi ) {
        return dX_real::operator_sqrt_body ( a );
      } else {
        return dX_real::operator_sqrt_body ( a.element_rotate() );
      }
    }
    template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >
    INLINE auto constexpr operator_sqrt ( T const& a ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_sqrt_exception<A> ( a, flag );
      if ( flag ) { return e; }
#else
      if ( fp<T>::is_zero( a ) ) { return dX_real::dx_real<T,A>{ a }; }
#endif
      return dX_real::operator_sqrt_body<A> ( a );
    }
    //
    template < typename TXa, T_mX(TXa) >
    INLINE auto constexpr sqrt ( TXa const& a ) {
      return dX_real::operator_sqrt ( a );
    }
    template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >
    INLINE auto constexpr sqrt ( T const& a ) {
      return dX_real::operator_sqrt<A> ( a );
    }
    //
    template < typename T, Algorithm A >
    INLINE dX_real::dx_real<T,A> constexpr dX_real::dx_real<T,A>::sqrt ( dX_real::dx_real<T,A> const& a ) {
      return dX_real::operator_sqrt ( a );
    }
    //

    //
    // f minimum
    //
    //
    template < typename T, Algorithm Aa, Algorithm Ab, A_noQuasi(Aa), A_noQuasi(Ab) >
    INLINE auto constexpr operator_fmin_body ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const &b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      int i=0; for(i=0;i<TX::L-1;i++) { if ( a.x[i] != b.x[i] ) break; }
      return TX{ ( a.x[i] <= b.x[i] ) ? a : b };
    }
    //
#if MX_REAL_USE_INF_NAN_EXCEPTION
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_fmin_exception ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const &b, bool & flag ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
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
    INLINE auto constexpr operator_fmin ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const &b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_fmin_exception( a, b, flag );
      if ( flag ) { return e; }
#endif
      if ( A == Algorithm::Quasi ) {
        using TT = typename  TX::type_Accurate;
        return TX{ dX_real::operator_fmin_body( TT{ a }, TT{ b } ) };
      } else {
        return dX_real::operator_fmin_body( a, b );
      }
    }
    //
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr fmin ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const &b ) {
      return dX_real::operator_fmin( a, b );
    }
    //

    //
    // f maximum
    //
    //
    template < typename T, Algorithm Aa, Algorithm Ab, A_noQuasi(Aa), A_noQuasi(Ab) >
    INLINE auto constexpr operator_fmax_body ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const &b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
      int i=0; for(i=0;i<TX::L-1;i++) { if ( a.x[i] != b.x[i] ) break; }
      return TX{ ( a.x[i] >= b.x[i] ) ? a : b };
    }
    //
#if MX_REAL_USE_INF_NAN_EXCEPTION
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr operator_fmax_exception ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const &b, bool & flag ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
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
    INLINE auto constexpr operator_fmax ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const &b ) {
      Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
      using TX = dX_real::dx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
      bool flag = false;
      auto e = dX_real::operator_fmax_exception( a, b, flag );
      if ( flag ) { return e; }
#endif
      if ( A == Algorithm::Quasi ) {
        using TT = typename TX::type_Accurate;
        return TX{ dX_real::operator_fmax_body( TT{ a }, TT{ b } ) };
      } else {
        return dX_real::operator_fmax_body( a, b );
      }
    }
    //
    template < typename T, Algorithm Aa, Algorithm Ab >
    INLINE auto constexpr fmax ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const &b ) {
      return dX_real::operator_fmax( a, b );
    }
    //

    //
    // PRNG
    //
    //
    template < typename T, Algorithm A, T_fp(T) >
    INLINE auto const rand () {
      using TX = dX_real::dx_real<T,A>;
      auto constexpr f = fp<T>::half / (1 << 30);
      auto g = f;
      auto r = TX::zero();
      auto constexpr bits = std::numeric_limits<TX>::digits;
      auto constexpr rand_bits = std::numeric_limits<int>::digits - 1;
      for(int i=0; i<bits; i+=rand_bits ) {
        auto b_ = fp<T>::rand() & 0x7fffffff;
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
    INLINE dX_real::dx_real<T,A> constexpr dX_real::dx_real<T,A>::rand () {
      return dX_real::rand<T,A>();
    }
    //


    //
    //
    //
    template < typename T, Algorithm Aa >
    std::ostream& operator<< ( std::ostream& stream, dX_real::dx_real<T,Aa> const& a ) {
      int constexpr LL = dX_real::dx_real<T,Aa>::L;
#if USE_MPREAL
      mpfr::mpreal t = double( a.x[0] );
      for(int i=1; i<LL; i++) { t += double( a.x[i] ); }
      stream << t;
#else
      for(int i=0; i<LL; i++) { stream << a.x[i] << ":"; }
#endif
      return stream;
    }

  }
}


#endif

