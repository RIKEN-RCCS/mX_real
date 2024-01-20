//
// Absolute value
//
//
template < typename TXa, IF_T_mX<TXa> >
INLINE auto const operator_abs_body ( TXa const& a ) {
  using T = typename TXa::base_T;
  using TX = qX_real::qx_real<T,TXa::base_A>;
  return TX{ is_negative( a ) ? -a : a };
}
//
#if MX_REAL_USE_INF_NAN_EXCEPTION
template < Algorithm A, typename T, IF_T_fp<T> >
INLINE auto const operator_abs_exception ( T const& a, bool & flag ) {
  using TX = qX_real::qx_real<T,A>;
  flag = true; {
    if ( fp<T>::isnan( a ) ) { TX::nan(); }
    if ( fp<T>::isinf( a ) ) { TX::inf(); }
  }
  //
  flag = false; return TX::zero();
}
template < typename TXa, IF_T_mX<TXa> >
INLINE auto const operator_abs_exception ( TXa const& a, bool & flag )
-> return_TX2< TXa, TXa, 4 > {
  Algorithm constexpr A = TXa::base_A;
  auto sa = a.quick_Normalized();
  return qX_real::operator_abs_exception<A> ( sa, flag );
}
#endif
//
template < typename TXa, IF_T_mX<TXa> >
INLINE auto const operator_abs ( TXa const& a ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = qX_real::operator_abs_exception ( a, flag );
  if ( flag ) { return e; }
#endif
  return qX_real::operator_abs_body ( a );
}
template < Algorithm A=Algorithm::Accurate, typename T, IF_T_fp<T> >
INLINE auto const operator_abs ( T const& a ) {
  using TX = qX_real::qx_real<T,A>;
  return TX{ std::abs( a ) };
}
//
template < typename TXa, IF_T_mX<TXa> >
INLINE auto const abs ( TXa const& a ) {
  return qX_real::operator_abs ( a );
}
template < Algorithm A=Algorithm::Accurate, typename T, IF_T_fp<T> >
INLINE auto const abs ( T const& a ) {
  return qX_real::operator_abs<A> ( a );
}

//
// Square root
//
//
template < typename T, Algorithm A >
INLINE auto const operator_sqrt_body ( qX_real::qx_real<T,A> const& a ) {
  using TX = qX_real::qx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::sqrt_QW_QW ( _QX_(a), _QX_(c) );
  } else {
    QxW::sqrt_QQW_QQW ( _QX_(a), _QX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm A >
INLINE auto const operator_sqrt_body ( tX_real::tx_real<T,A> const& a ) {
  using TX = qX_real::qx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::sqrt_TW_QW ( _TX_(a), _QX_(c) );
  } else {
    QxW::sqrt_QTW_QQW ( _TX_(a), _QX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm A >
INLINE auto const operator_sqrt_body ( dX_real::dx_real<T,A> const& a ) {
  using TX = qX_real::qx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::sqrt_DW_QW ( _DX_(a), _QX_(c) );
  } else {
    QxW::sqrt_PA_QQW ( _DX_(a), _QX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm A, IF_T_fp<T> >
INLINE auto const operator_sqrt_body ( T const& a ) {
  using TX = qX_real::qx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::sqrt_SW_QW ( _SX_(a), _QX_(c) );
  } else {
    QxW::sqrt_SW_QQW ( _SX_(a), _QX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
//
#if MX_REAL_USE_INF_NAN_EXCEPTION
template < Algorithm A, typename T, IF_T_fp<T> >
INLINE auto const operator_sqrt_exception ( T const& a, bool & flag ) {
  using TX = qX_real::qx_real<T,A>;
  flag = true; {
    if ( fp<T>::isnan( a ) ) { auto c=a; return TX{ c,c,c,c }; }
    if ( fp<T>::is_negative( a ) ) { return TX::nan(); }
    if ( fp<T>::isinf( a ) ) { return TX::inf(); }
    if ( fp<T>::is_zero( a ) ) { auto c=a; return TX{ c,c,c,c }; }
  }
  //
  flag = false; return TX::zero();
}
template < typename TXa, IF_T_mX<TXa> >
INLINE auto const operator_sqrt_exception ( TXa const& a, bool & flag )
-> return_TX2< TXa, TXa, 4 > {
  Algorithm constexpr A = TXa::base_A;
  auto sa = a.quick_Normalized();
  return qX_real::operator_sqrt_exception<A> ( sa, flag );
}
#endif
//
template < typename TXa, IF_T_mX<TXa> >
INLINE auto const operator_sqrt ( TXa const& a ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = qX_real::operator_sqrt_exception ( a, flag );
  if ( flag ) { return e; }
#else
  using T = typename TXa::base_T;
  using TX = typename qX_real::qx_real<T,TXa::base_A>;
  if ( fp<T>::is_zero( a.quick_Normalized() ) ) { return TX{ a }; }
#endif
  Algorithm constexpr A = TXa::base_A;
  if ( A != Algorithm::Quasi ) {
    return qX_real::operator_sqrt_body ( a );
  } else {
    return qX_real::operator_sqrt_body ( a.element_rotate() );
  }
}
template < Algorithm A=Algorithm::Accurate, typename T, IF_T_fp<T> >
INLINE auto const operator_sqrt ( T const& a ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = qX_real::operator_sqrt_exception<A> ( a, flag );
  if ( flag ) { return e; }
#else
  using TX = qX_real::qx_real<T,A>;
  if ( fp<T>::is_zero( a ) ) { return TX{ a }; }
#endif
  return qX_real::operator_sqrt_body<T,A> ( a );
}
//
template < typename TXa, IF_T_mX<TXa> >
INLINE auto const sqrt ( TXa const& a ) {
  return qX_real::operator_sqrt ( a );
}
template < Algorithm A=Algorithm::Accurate, typename T, IF_T_fp<T> >
INLINE auto const sqrt ( T const& a ) {
  return qX_real::operator_sqrt<A> ( a );
}
//

//
// f minimum
//
//
template < typename T, Algorithm Aa, Algorithm Ab, IF_A2_noQuasi<Aa,Ab> >
INLINE auto const operator_fmin_body ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const &b ) {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = qX_real::qx_real<T,A>;
  int i; for(i=0;i<TX::L-1;i++) { if ( a.x[i] != b.x[i] ) break; }
  return TX{ ( a.x[i] <= b.x[i] ) ? a : b };
}
//
#if MX_REAL_USE_INF_NAN_EXCEPTION
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto const operator_fmin_exception ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const &b, bool & flag ) {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = qX_real::qx_real<T,A>;
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
INLINE auto const operator_fmin ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const &b ) {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = qX_real::qx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = qX_real::operator_fmin_exception( a, b, flag );
  if ( flag ) { return e; }
#endif
  if ( A == Algorithm::Quasi ) {
    using TT = qX_real::qx_real<T,Algorithm::Accurate>;
    return TX{ qX_real::operator_fmin_body( TT{ a }, TT{ b } ) };
  } else {
    return qX_real::operator_fmin_body( a, b );
  }
}
//
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto const fmin ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const &b ) {
  return qX_real::operator_fmin( a, b );
}
//

//
// f maximum
//
//
template < typename T, Algorithm Aa, Algorithm Ab, IF_A2_noQuasi<Aa,Ab> >
INLINE auto const operator_fmax_body ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const &b ) {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = qX_real::qx_real<T,A>;
  int i; for(i=0;i<TX::L-1;i++) { if ( a.x[i] != b.x[i] ) break; }
  return TX{ ( a.x[i] >= b.x[i] ) ? a : b };
}
//
#if MX_REAL_USE_INF_NAN_EXCEPTION
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto const operator_fmax_exception ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const &b, bool & flag ) {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = qX_real::qx_real<T,A>;
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
INLINE auto const operator_fmax ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const &b ) {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = qX_real::qx_real<T,A>;
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = qX_real::operator_fmax_exception( a, b, flag );
  if ( flag ) { return e; }
#endif
  if ( A == Algorithm::Quasi ) {
    using TT = qX_real::qx_real<T,Algorithm::Accurate>;
    return TX{ qX_real::operator_fmax_body( TT{ a }, TT{ b } ) };
  } else {
    return qX_real::operator_fmax_body( a, b );
  }
}
//
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto const fmax ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const &b ) {
  return qX_real::operator_fmax( a, b );
}
//

//
// PRNG
//
//
template < typename T, Algorithm A >
INLINE auto rand () -> if_T_double<T,qX_real::qx_real<T,A>> {
  using TX = qX_real::qx_real<T,A>;
  auto constexpr f = fp<T>::half / (1 << 30);
  auto g = f;
  auto r = TX::zero();
  auto constexpr bits = std::numeric_limits<TX>::digits;
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
  auto constexpr bits = std::numeric_limits<TX>::digits;
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
//
template < typename T, Algorithm A >
INLINE qX_real::qx_real<T,A> constexpr qX_real::qx_real<T,A>::rand () {
  return qX_real::rand<T,A>();
}
//