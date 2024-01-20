//
// Addition
//
//
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto const operator_add_body ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
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
INLINE auto const operator_add_body ( qX_real::qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
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
INLINE auto const operator_add_body ( qX_real::qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
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
INLINE auto const operator_add_body ( qX_real::qx_real<T,Aa> const& a, T const& b ) {
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
INLINE auto const operator_add_body ( tX_real::tx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
  return operator_add_body ( b, a );
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto const operator_add_body ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
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
INLINE auto const operator_add_body ( tX_real::tx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
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
INLINE auto const operator_add_body ( tX_real::tx_real<T,Aa> const& a, T const& b ) {
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
INLINE auto const operator_add_body ( dX_real::dx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
  return operator_add_body ( b, a );
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto const operator_add_body ( dX_real::dx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
  return operator_add_body ( b, a );
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto const operator_add_body ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
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
INLINE auto const operator_add_body ( dX_real::dx_real<T,Aa> const& a, T const& b ) {
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
INLINE auto const operator_add_body ( T const& a, qX_real::qx_real<T,Ab> const& b ) {
  return operator_add_body ( b, a );
}
template < typename T, Algorithm Ab >
INLINE auto const operator_add_body ( T const& a, tX_real::tx_real<T,Ab> const& b ) {
  return operator_add_body ( b, a );
}
template < typename T, Algorithm Ab >
INLINE auto const operator_add_body ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
  return operator_add_body ( b, a );
}
template < typename T, Algorithm A, IF_T_fp<T> >
INLINE auto const operator_add_body ( T const& a, T const& b ) {
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
template < Algorithm A, typename T, IF_T_fp<T> >
INLINE auto const operator_add_exception ( T const& a, T const& b, bool & flag ) {
  using TX = tX_real::tx_real<T,A>;
  flag = true; {
    if ( fp<T>::isnan( a ) || fp<T>::isnan( b ) ) {
      auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );
      return TX{ c,c,c };
    }
    if ( fp<T>::isinf( a ) && fp<T>::isinf( b ) ) {
      auto sign = std::signbit( a ) ^ std::signbit( b );
      if ( sign ) {
        auto c = fp<T>::nan;
        return TX{ c,c,c };
      } else {
        auto c = fp<T>::copysign( fp<T>::inf, a );
        return TX{ c,c,c };
      }
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
template < typename TXa, typename TXb, IF_T_mX2<TXa,TXb> >
INLINE auto const operator_add_exception ( TXa const& a, TXb const& b, bool & flag )
-> return_TX2< TXa, TXb, 3 > {
  Algorithm constexpr A = commonAlgorithm< TXa::base_A, TXb::base_A >::algorithm;
  auto sa = a.quick_Normalized();
  auto sb = b.quick_Normalized();
  return tX_real::operator_add_exception<A> ( sa, sb, flag );
}
template < typename TXa, typename T, IF_T_mX<TXa>, IF_T_fp<T> >
INLINE auto const operator_add_exception ( TXa const& a, T const& b, bool & flag )
-> return_TX< TXa, T, 3 > {
  Algorithm constexpr A = TXa::base_A;
  auto sa = a.quick_Normalized();
  return tX_real::operator_add_exception<A> ( sa, b, flag );
}
#endif
//
template < typename TXa, typename TXb, IF_T_mX2<TXa,TXb> >
INLINE auto operator_add ( TXa const& a, TXb const& b )
-> cond_return_TX2<(TXa::L>=TXb::L),TXa,TXb,3> {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = tX_real::operator_add_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_add_body ( a, b );
}
template < typename TXa, typename TXb, IF_T_mX2<TXa,TXb> >
INLINE auto operator_add ( TXa const& a, TXb const& b )
-> cond_return_TX2<(TXa::L<TXb::L),TXa,TXb,3> {
  return tX_real::operator_add ( b, a );
}
template < typename TXa, typename T, IF_T_fp<T>, IF_T_mX<TXa> >
INLINE auto operator_add ( TXa const& a, T const& b )
-> return_TX<TXa,T,3> {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = tX_real::operator_add_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_add_body ( a, b );
}
template < typename T, typename TXb, IF_T_fp<T>, IF_T_mX<TXb> >
INLINE auto operator_add ( T const& a, TXb const& b )
-> return_TX<TXb,T,3> {
  return tX_real::operator_add ( b, a );
}
template < Algorithm A=Algorithm::Accurate, typename T, IF_T_fp<T> >
INLINE auto operator_add ( T const& a, T const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = tX_real::operator_add_exception<A> ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_add_body<T,A> ( a, b );
}
//
template < typename TXa, typename TXb, IF_T_mX2<TXa,TXb> >
INLINE auto operator+ ( TXa const& a, TXb const& b )
-> cond_return_TX2<(TXa::L==3||TXb::L==3),TXa,TXb,3> {
  return tX_real::operator_add ( a, b );
}
template < typename T, Algorithm A, typename Ts, IF_T_scalar<Ts> >
INLINE auto operator+ ( tX_real::tx_real<T,A> const& a, Ts const& b ) {
  auto const b_ = T(b);
  return tX_real::operator_add ( a, b_ );
}
template < typename T, Algorithm A, typename Ts, IF_T_scalar<Ts> >
INLINE auto operator+ ( Ts const& a, tX_real::tx_real<T,A> const& b ) {
  return b + a;
}
//

//
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto operator_add_ow_body ( tX_real::tx_real<T,Aa> & a, qX_real::qx_real<T,Ab> const& b ) {
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
INLINE auto operator_add_ow_body ( tX_real::tx_real<T,Aa> & a, tX_real::tx_real<T,Ab> const& b ) {
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
INLINE auto operator_add_ow_body ( tX_real::tx_real<T,Aa> & a, dX_real::dx_real<T,Ab> const& b ) {
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
INLINE auto operator_add_ow_body ( tX_real::tx_real<T,Aa> & a, T const& b ) {
  if ( Aa == Algorithm::Accurate ) {
    QxW::add_TW_SW_TW ( _TX_(a), _SX_(b), _TX_(a) );
  } else {
    QxW::add_QTW_SW_QTW ( _TX_(a), _SX_(b), _TX_(a) );
    if ( Aa != Algorithm::Quasi ) { Normalize( a ); }
  }
  return a;
}
//
template < typename Ta, Algorithm Aa, typename TXb, IF_T_mX<TXb> >
INLINE auto operator_add_ow ( tX_real::tx_real<Ta,Aa> & a, TXb const& b )
-> return_TXA<TXb,Ta,Aa,3> {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = tX_real::operator_add_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_add_ow_body ( a, b );
}
template < typename T, Algorithm A >
INLINE auto operator_add_ow ( tX_real::tx_real<T,A> & a, T const& b )
-> tX_real::tx_real<T,A> {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = tX_real::operator_add_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_add_ow_body ( a, b );
}
//
template < typename Ta, Algorithm Aa, typename TXb, IF_T_mX<TXb> >
INLINE auto operator+= ( tX_real::tx_real<Ta,Aa> & a, TXb const& b )
-> return_TXA<TXb,Ta,Aa,3> {
  return tX_real::operator_add_ow ( a, b );
}
template < typename T, Algorithm A, typename Ts, IF_T_scalar<Ts> >
INLINE auto operator+= ( tX_real::tx_real<T,A> & a, Ts const& b ) {
  auto const b_ = T(b);
  return tX_real::operator_add_ow ( a, b_ );
}
//

//
// Substraction
//
//
template < typename TXa, typename TXb, IF_T_mX2<TXa,TXb> >
INLINE auto operator- ( TXa const& a, TXb const& b )
-> cond_return_TX2<(TXa::L==3||TXb::L==3),TXa,TXb,3> {
  return a + (-b);
}
template < typename T, Algorithm A, typename Ts, IF_T_scalar<Ts> >
INLINE auto operator- ( tX_real::tx_real<T,A> const& a, Ts const& b ) {
  return a + (-b);
}
template < Algorithm A=Algorithm::Accurate, typename T, typename Ts, IF_T_scalar<Ts> >
INLINE auto operator- ( Ts const &a, tX_real::tx_real<T,A> const& b ) {
  return a + (-b);
}
//

//
template < typename Ta, Algorithm Aa, typename TXb, IF_T_mX<TXb> >
INLINE auto operator-= ( tX_real::tx_real<Ta,Aa> & a, TXb const& b )
-> return_TX<TXb,Ta,3> {
  return a += (-b);
}
template < typename T, Algorithm A, typename Ts, IF_T_scalar<Ts> >
INLINE auto operator-= ( tX_real::tx_real<T,A> & a, Ts const& b ) {
  auto const b_ = T(b);
  return a += (-b_);
}
//
//
// Multiplication
//
//
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto const operator_mul_body ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = tX_real::tx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::mul_QW_QW_TW ( _QX_(a), _QX_(b), _TX_(c) );
  } else {
    QxW::mul_QQW_QQW_QTW ( _QX_(a), _QX_(b), _TX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto const operator_mul_body ( qX_real::qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
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
INLINE auto const operator_mul_body ( qX_real::qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
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
INLINE auto const operator_mul_body ( qX_real::qx_real<T,Aa> const& a, T const& b ) {
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
INLINE auto const operator_mul_body ( tX_real::tx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
  return operator_mul_body ( b, a );
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto const operator_mul_body ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = tX_real::tx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::mul_TW_TW_TW ( _TX_(a), _TX_(b), _TX_(c) );
  } else {
    QxW::mul_QTW_QTW_QTW ( _TX_(a), _TX_(b), _TX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto const operator_mul_body ( tX_real::tx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
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
INLINE auto const operator_mul_body ( tX_real::tx_real<T,Aa> const& a, T const& b ) {
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
INLINE auto const operator_mul_body ( dX_real::dx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
  return operator_mul_body ( b, a );
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto const operator_mul_body ( dX_real::dx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
  return operator_mul_body ( b, a );
}
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto const operator_mul_body ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;
  using TX = tX_real::tx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::mul_DW_DW_TW ( _DX_(a), _DX_(b), _TX_(c) );
  } else {
    QxW::mul_PA_PA_QTW ( _DX_(a), _DX_(b), _TX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
template < typename T, Algorithm Aa >
INLINE auto const operator_mul_body ( dX_real::dx_real<T,Aa> const& a, T const& b ) {
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
INLINE auto const operator_mul_body ( T const& a, qX_real::qx_real<T,Ab> const& b ) {
  return operator_mul_body ( b, a );
}
template < typename T, Algorithm Ab >
INLINE auto const operator_mul_body ( T const& a, tX_real::tx_real<T,Ab> const& b ) {
  return operator_mul_body ( b, a );
}
template < typename T, Algorithm Ab >
INLINE auto const operator_mul_body ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
  return operator_mul_body ( b, a );
}
template < typename T, Algorithm A, IF_T_fp<T> >
INLINE auto const operator_mul_body ( T const& a, T const& b ) {
  using TX = tX_real::tx_real<T,A>;
  TX c;
  if ( A == Algorithm::Accurate ) {
    QxW::mul_SW_SW_TW ( _SX_(a), _SX_(b), _TX_(c) );
  } else {
    QxW::mul_SW_SW_QTW ( _SX_(a), _SX_(b), _TX_(c) );
    if ( A != Algorithm::Quasi ) { Normalize( c ); }
  }
  return c;
}
//
#if MX_REAL_USE_INF_NAN_EXCEPTION
template < Algorithm A, typename T, IF_T_fp<T> >
INLINE auto const operator_mul_exception ( T const& a, T const& b, bool & flag ) {
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
template < typename TXa, typename TXb, IF_T_mX2<TXa,TXb> >
INLINE auto const operator_mul_exception ( TXa const& a, TXb const& b, bool & flag )
-> return_TX2< TXa, TXb, 3 > {
  Algorithm constexpr A = commonAlgorithm< TXa::base_A, TXb::base_A >::algorithm;
  auto sa = a.quick_Normalized();
  auto sb = b.quick_Normalized();
  return tX_real::operator_mul_exception<A> ( sa, sb, flag );
}
template < typename TXa, typename T, IF_T_mX<TXa>, IF_T_fp<T> >
INLINE auto const operator_mul_exception ( TXa const& a, T const& b, bool & flag )
-> return_TX< TXa, T, 3 > {
  Algorithm constexpr A = TXa::base_A;
  auto sa = a.quick_Normalized();
  return tX_real::operator_mul_exception<A> ( sa, b, flag );
}
#endif
//
template < typename TXa, typename TXb, IF_T_mX2<TXa,TXb> >
INLINE auto operator_mul ( TXa const& a, TXb const& b )
-> cond_return_TX2<(TXa::L>=TXb::L),TXa,TXb,3> {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = tX_real::operator_mul_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_mul_body ( a, b );
}
template < typename TXa, typename TXb, IF_T_mX2<TXa,TXb> >
INLINE auto operator_mul ( TXa const& a, TXb const& b )
-> cond_return_TX2<(TXa::L<TXb::L),TXa,TXb,3> {
  return tX_real::operator_mul ( b, a );
}
template < typename TXa, typename T, IF_T_fp<T>, IF_T_mX<TXa> >
INLINE auto operator_mul ( TXa const& a, T const& b )
-> return_TX<TXa,T,3> {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = tX_real::operator_mul_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_mul_body ( a, b );
}
template < typename T, typename TXb, IF_T_fp<T>, IF_T_mX<TXb> >
INLINE auto operator_mul ( T const& a, TXb const& b )
-> return_TX<TXb,T,3> {
  return tX_real::operator_mul ( b, a );
}
template < Algorithm A=Algorithm::Accurate, typename T, IF_T_fp<T> >
INLINE auto operator_mul ( T const& a, T const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = tX_real::operator_mul_exception<A> ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_mul_body<T,A> ( a, b );
}
//
template < typename TXa, typename TXb, IF_T_mX2<TXa,TXb> >
INLINE auto operator* ( TXa const& a, TXb const& b )
-> cond_return_TX2<(TXa::L==3||TXb::L==3),TXa,TXb,3> {
  return tX_real::operator_mul ( a, b );
}
template < typename T, Algorithm A, typename Ts, IF_T_scalar<Ts> >
INLINE auto operator* ( tX_real::tx_real<T,A> const& a, Ts const& b ) {
  auto const b_ = T(b);
  return tX_real::operator_mul ( a, b_ );
}
template < typename T, Algorithm A, typename Ts, IF_T_scalar<Ts> >
INLINE auto operator* ( Ts const& a, tX_real::tx_real<T,A> const& b ) {
  return b * a;
}
//

//
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto operator_mul_ow_body ( tX_real::tx_real<T,Aa> & a, qX_real::qx_real<T,Ab> const& b ) {
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
INLINE auto operator_mul_ow_body ( tX_real::tx_real<T,Aa> & a, tX_real::tx_real<T,Ab> const& b ) {
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
INLINE auto operator_mul_ow_body ( tX_real::tx_real<T,Aa> & a, dX_real::dx_real<T,Ab> const& b ) {
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
INLINE auto operator_mul_ow_body ( tX_real::tx_real<T,Aa> & a, T const& b ) {
  if ( Aa == Algorithm::Accurate ) {
    QxW::mul_TW_SW_TW ( _TX_(a), _SX_(b), _TX_(a) );
  } else {
    QxW::mul_QTW_SW_QTW ( _TX_(a), _SX_(b), _TX_(a) );
    if ( Aa != Algorithm::Quasi ) { Normalize( a ); }
  }
  return a;
}
//
template < typename Ta, Algorithm Aa, typename TXb, IF_T_mX<TXb> >
INLINE auto operator_mul_ow ( tX_real::tx_real<Ta,Aa> & a, TXb const& b )
-> return_TXA<TXb,Ta,Aa,3> {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = tX_real::operator_mul_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_mul_ow_body ( a, b );
}
template < typename T, Algorithm A >
INLINE auto operator_mul_ow ( tX_real::tx_real<T,A> & a, T const& b )
-> tX_real::tx_real<T,A> {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = tX_real::operator_mul_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_mul_ow_body ( a, b );
}
//
template < typename Ta, Algorithm Aa, typename TXb, IF_T_mX<TXb> >
INLINE auto operator*= ( tX_real::tx_real<Ta,Aa> & a, TXb const& b )
-> return_TXA<TXb,Ta,Aa,3> {
  return tX_real::operator_mul_ow ( a, b );
}
template < typename T, Algorithm A, typename Ts, IF_T_scalar<Ts> >
INLINE auto operator*= ( tX_real::tx_real<T,A> & a, Ts const& b ) {
  auto const b_ = T(b);
  return tX_real::operator_mul_ow ( a, b_ );
}
//

//
// Division
//
//
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto const operator_div_body ( qX_real::qx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
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
INLINE auto const operator_div_body ( qX_real::qx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
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
INLINE auto const operator_div_body ( qX_real::qx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
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
INLINE auto const operator_div_body ( qX_real::qx_real<T,Aa> const& a, T const& b ) {
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
INLINE auto const operator_div_body ( tX_real::tx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
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
INLINE auto const operator_div_body ( tX_real::tx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
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
INLINE auto const operator_div_body ( tX_real::tx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
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
INLINE auto const operator_div_body ( tX_real::tx_real<T,Aa> const& a, T const& b ) {
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
INLINE auto const operator_div_body ( dX_real::dx_real<T,Aa> const& a, qX_real::qx_real<T,Ab> const& b ) {
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
INLINE auto const operator_div_body ( dX_real::dx_real<T,Aa> const& a, tX_real::tx_real<T,Ab> const& b ) {
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
INLINE auto const operator_div_body ( dX_real::dx_real<T,Aa> const& a, dX_real::dx_real<T,Ab> const& b ) {
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
INLINE auto const operator_div_body ( dX_real::dx_real<T,Aa> const& a, T const& b ) {
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
INLINE auto const operator_div_body ( T const& a, qX_real::qx_real<T,Ab> const& b ) {
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
INLINE auto const operator_div_body ( T const& a, tX_real::tx_real<T,Ab> const& b ) {
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
INLINE auto const operator_div_body ( T const& a, dX_real::dx_real<T,Ab> const& b ) {
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
template < typename T, Algorithm A, IF_T_fp<T> >
INLINE auto const operator_div_body ( T const& a, T const& b ) {
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
template < Algorithm A, typename T, IF_T_fp<T> >
INLINE auto const operator_div_exception ( T const& a, T const& b, bool & flag ) {
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
template < typename TXa, typename TXb, IF_T_mX2<TXa,TXb> >
INLINE auto const operator_div_exception ( TXa const& a, TXb const& b, bool & flag )
-> return_TX2< TXa, TXb, 3 > {
  Algorithm constexpr A = commonAlgorithm< TXa::base_A, TXb::base_A >::algorithm;
  auto sa = a.quick_Normalized();
  auto sb = b.quick_Normalized();
  return tX_real::operator_div_exception<A> ( sa, sb, flag );
}
template < typename TXa, typename T, IF_T_mX<TXa>, IF_T_fp<T> >
INLINE auto const operator_div_exception ( TXa const& a, T const& b, bool & flag )
-> return_TX< TXa, T, 3 > {
  Algorithm constexpr A = TXa::base_A;
  auto sa = a.quick_Normalized();
  return tX_real::operator_div_exception<A> ( sa, b, flag );
}
template < typename TXb, typename T, IF_T_mX<TXb>, IF_T_fp<T> >
INLINE auto const operator_div_exception ( T const& a, TXb const& b, bool & flag )
-> return_TX< TXb, T, 3 > {
  Algorithm constexpr A = TXb::base_A;
  auto sb = b.quick_Normalized();
  return tX_real::operator_div_exception<A> ( a, sb, flag );
}
#endif
//
template < typename TXa, typename TXb, IF_T_mX2<TXa,TXb> >
INLINE auto operator_div ( TXa const& a, TXb const& b )
-> cond_return_TX2<(TXa::L>=TXb::L),TXa,TXb,3> {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = tX_real::operator_div_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_div_body ( a, b );
}
template < typename TXa, typename TXb, IF_T_mX2<TXa,TXb> >
INLINE auto operator_div ( TXa const& a, TXb const& b )
-> cond_return_TX2<(TXa::L<TXb::L),TXa,TXb,3> {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = tX_real::operator_div_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_div_body ( a, b );
}
template < typename TXa, typename T, IF_T_fp<T>, IF_T_mX<TXa> >
INLINE auto operator_div ( TXa const& a, T const& b )
-> return_TX<TXa,T,3> {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = tX_real::operator_div_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_div_body ( a, b );
}
template < typename T, typename TXb, IF_T_fp<T>, IF_T_mX<TXb> >
INLINE auto operator_div ( T const& a, TXb const& b )
-> return_TX<TXb,T,3> {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = tX_real::operator_div_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_div_body ( a, b );
}
template < Algorithm A=Algorithm::Accurate, typename T, IF_T_fp<T> >
INLINE auto operator_div ( T const& a, T const& b ) {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = tX_real::operator_div_exception<A> ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_div_body<T,A> ( a, b );
}
//
template < typename TXa, typename TXb, IF_T_mX2<TXa,TXb> >
INLINE auto operator/ ( TXa const& a, TXb const& b )
-> cond_return_TX2<(TXa::L==3||TXb::L==3),TXa,TXb,3> {
  return tX_real::operator_div ( a, b );
}
template < typename T, Algorithm A, typename Ts, IF_T_scalar<Ts> >
INLINE auto operator/ ( tX_real::tx_real<T,A> const& a, Ts const& b ) {
  auto const b_ = T(b);
  return tX_real::operator_div ( a, b_ );
}
template < typename T, Algorithm A, typename Ts, IF_T_scalar<Ts> >
INLINE auto operator/ ( Ts const& a, tX_real::tx_real<T,A> const& b ) {
  auto const a_ = T(a);
  return tX_real::operator_div ( a_, b );
}
//

//
template < typename T, Algorithm Aa, Algorithm Ab >
INLINE auto operator_div_ow_body ( tX_real::tx_real<T,Aa> & a, qX_real::qx_real<T,Ab> const& b ) {
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
INLINE auto operator_div_ow_body ( tX_real::tx_real<T,Aa> & a, tX_real::tx_real<T,Ab> const& b ) {
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
INLINE auto operator_div_ow_body ( tX_real::tx_real<T,Aa> & a, dX_real::dx_real<T,Ab> const& b ) {
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
INLINE auto operator_div_ow_body ( tX_real::tx_real<T,Aa> & a, T const& b ) {
  if ( Aa == Algorithm::Accurate ) {
    QxW::div_TW_SW_TW ( _TX_(a), _SX_(b), _TX_(a) );
  } else {
    QxW::div_QTW_SW_QTW ( _TX_(a), _SX_(b), _TX_(a) );
    if ( Aa != Algorithm::Quasi ) { Normalize( a ); }
  }
  return a;
}
//
template < typename Ta, Algorithm Aa, typename TXb, IF_T_mX<TXb> >
INLINE auto operator_div_ow ( tX_real::tx_real<Ta,Aa> & a, TXb const& b )
-> return_TXA<TXb,Ta,Aa,3> {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = tX_real::operator_div_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_div_ow_body ( a, b );
}
template < typename T, Algorithm A >
INLINE auto operator_div_ow ( tX_real::tx_real<T,A> & a, T const& b )
-> tX_real::tx_real<T,A> {
#if MX_REAL_USE_INF_NAN_EXCEPTION
  bool flag;
  auto e = tX_real::operator_div_exception ( a, b, flag );
  if ( flag ) { return e; }
#endif
  return tX_real::operator_div_ow_body ( a, b );
}
//
template < typename Ta, Algorithm Aa, typename TXb, IF_T_mX<TXb> >
INLINE auto operator/= ( tX_real::tx_real<Ta,Aa> & a, TXb const& b )
-> return_TXA<TXb,Ta,Aa,3> {
  return tX_real::operator_div_ow ( a, b );
}
template < typename T, Algorithm A, typename Ts, IF_T_scalar<Ts> >
INLINE auto operator/= ( tX_real::tx_real<T,A> & a, Ts const& b ) {
  auto const b_ = T(b);
  return tX_real::operator_div_ow ( a, b_ );
}
//

