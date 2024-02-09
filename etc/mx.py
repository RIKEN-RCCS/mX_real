import sys

def mX_suffics ( ACC, width ) :
    if ACC > 0 :
        return [ '_', '_SW', '_DW', '_TW', '_QW' ][width]
    else :
        return [ '_', '_SW', '_PA', '_QTW', '_QQW' ][width]

def mX_type ( width ) :
    return [ '_', 's', 'd', 't', 'q' ][width]

def mX_arg ( width, var ) :
    return [ '_', '_SX_({})', '_DX_({})', '_TX_({})', '_QX_({})' ][width].format(var)

def commonA ( Ta, Tb ) :
    A = ''
    if Ta > 1 and Tb == 1 :
        A = 'a'
    if Tb > 1 and Ta == 1 :
        A = 'b'
    return A

def fname ( Func, NA, NB, NC, ACC ) :
    return '{}{}{}{}'.format(
        Func, mX_suffics( ACC, NA ), mX_suffics( ACC, NB ) if NB > 0 else '', mX_suffics( ACC, NC ) )

def caller_head ( Func, NA, NB, NC, ACC, va, vb, vc ) :
    line = fname( Func, NA, NB, NC, ACC )
    if NB > 0 :
        line = line + ' ( {}, {}, {} );'.format( mX_arg(NA,va), mX_arg(NB,vb), mX_arg(NC,vc) )
    else :
        line = line + ' ( {}, {} );'.format( mX_arg(NA,va), mX_arg(NC,vc) )
    return line

def template( Ta, Tb ) :
    line = 'template < typename T'
    if Ta > 1 :
        line = line + ', Algorithm Aa'
    if Tb > 1 :
        line = line + ', Algorithm Ab'
    if Ta == 1 and Tb == 1 :
        line = line + ', Algorithm A'
        line = line + ', T_fp(T)'
    line = line + ' >'
    return line

def arg_list( Ta, Tb ) :
    line= '('
    if Ta == 1 :
        line = line + ' T const& a'
    else :
        line = line + ' {m}X_real::{m}x_real<T,Aa> const& a'.format( m=mX_type(Ta) )
    if Tb == 1 :
        line = line + ', T const& b'
    else :
        line = line + ', {m}X_real::{m}x_real<T,Ab> const& b'.format( m=mX_type(Tb) )
    line = line + ' ) {'
    return line


# ************************************************************************************************
#
# ************************************************************************************************
def gen_op_body( Tc, description, func, op, commutable ) :
    for Ta in reversed( range( 1, 4+1 ) ) :
        for Tb in reversed( range( 1, 4+1 ) ) :
            #if Ta > Tc or Tb > Tc :
            #    continue

            print( template( Ta, Tb ) )
            print( 'INLINE auto constexpr operator_{func}_body '.format( func=func ) + arg_list( Ta, Tb ) )

            if ( Tb > Ta ) and ( commutable == 1 ) :
                print( '  return operator_{func}_body ( b, a );'.format( func=func ) )
                print( '}' )
                continue

            A = commonA( Ta, Tb )
            if Ta > 1 and Tb > 1 :
                print( '  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;' )
            line = '  using TX = {m}X_real::{m}x_real<T,A{a}>;'.format( m=mX_type(Tc), a=A )
            print( line )

            print( '  TX c;' )
            print( '  if ( A{a} == Algorithm::Accurate ) {{'.format( a=A ) )
            if func == 'mul' and Ta == Tb :
                print( '  if ( (void *)(&a) == (void *)(&b) ) {' )
                print( caller_head( '    QxW::{}'.format('sqr'), Ta,-1,Tc,1, 'a','b','c' ) )
                print( '  } else {' )
            print( caller_head( '    QxW::{}'.format(func), Ta,Tb,Tc,1, 'a','b','c' ) )
            if func == 'mul' and Ta == Tb :
                print( '  }' )
            print( '  } else {' )
            if func == 'mul' and Ta == Tb :
                print( '  if ( (void *)(&a) == (void *)(&b) ) {' )
                print( caller_head( '    QxW::{}'.format('sqr'), Ta,-1,Tc,0, 'a','b','c' ) )
                print( '  } else {' )
            print( caller_head( '    QxW::{}'.format(func), Ta,Tb,Tc,0, 'a','b','c' ) )
            if func == 'mul' and Ta == Tb :
                print( '  }' )
            print( '    if ( A{a} != Algorithm::Quasi ) {{ Normalize( c ); }}'.format( a=A ) )
            print( '  }' )
            print( '  return c;' )
            print( '}' )

# ************************************************************************************************
#
# ************************************************************************************************
def gen_exception( Tc, description, func, op, commutable ) :

    print( 'template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb), T_assert( std::is_same<typename TXa::base_T,typename TXb::base_T>::value ) >' )
    print( 'INLINE auto constexpr operator_{func}_exception ( TXa const& a, TXb const& b, bool & flag ) {{'.format( func=func ) )
    print( '  Algorithm constexpr A = commonAlgorithm< TXa::base_A, TXb::base_A >::algorithm;' )
    print( '  auto sa = a.quick_Normalized();' )
    print( '  auto sb = b.quick_Normalized();' )
    print( '  return {m}X_real::operator_{func}_exception<A> ( sa, sb, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    print( 'template < typename TXa, typename T, T_mX(TXa), T_fp(T), T_assert( std::is_same<typename TXa::base_T,T>::value ) >' )
    print( 'INLINE auto constexpr operator_{func}_exception ( TXa const& a, T const& b, bool & flag ) {{'.format( func=func ) )
    print( '  Algorithm constexpr A = TXa::base_A;' )
    print( '  auto sa = a.quick_Normalized();' )
    print( '  return {m}X_real::operator_{func}_exception<A> ( sa, b, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    if commutable == 1 :
        return

    print( 'template < typename T, typename TXb, T_fp(T), T_mX(TXb), T_assert( std::is_same<T,typename TXb::base_T>::value ) >' )
    print( 'INLINE auto constexpr operator_{func}_exception ( T const& a, TXb const& b, bool & flag ) {{'.format( func=func ) )
    print( '  Algorithm constexpr A = TXb::base_A;' )
    print( '  auto sb = b.quick_Normalized();' )
    print( '  return {m}X_real::operator_{func}_exception<A> ( a, sb, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

def gen_exception_add( Tc, description, func, op, commutable ) :

    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    ARG = ','.join( [ 'c' for i in range( Tc ) ] )

    print( 'template < Algorithm A, typename T, T_fp(T) >' )
    print( 'INLINE auto constexpr operator_add_exception ( T const& a, T const& b, bool & flag ) {' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  flag = true; {' )
    print( '    if ( fp<T>::isnan( a ) || fp<T>::isnan( b ) ) {' )
    print( '      auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );' )
    print( '      return TX{ '+ARG+' };' )
    print( '    }' )
    print( '    if ( fp<T>::isinf( a ) && fp<T>::isinf( b ) ) {' )
    print( '      auto sign = std::signbit( a ) ^ std::signbit( b );' )
    print( '      auto c = sign ? fp<T>::nan : fp<T>::copysign( fp<T>::inf, a );' )
    print( '      return TX{ '+ARG+' };' )
    print( '    }' )
    print( '    if ( fp<T>::isinf( a ) ) {' )
    print( '      auto c = fp<T>::copysign( fp<T>::inf, a );' )
    print( '      return TX{ '+ARG+' };' )
    print( '    }' )
    print( '    if ( fp<T>::isinf( b ) ) {' )
    print( '      auto c = fp<T>::copysign( fp<T>::inf, b );' )
    print( '      return TX{ '+ARG+' };' )
    print( '    }' )
    print( '  }' )
    print( '  //' )
    print( '  flag = false; return TX::zero();' )
    print( '}' )

    gen_exception( Tc, description, func, op, commutable )
    print( '#endif' )

def gen_exception_mul( Tc, description, func, op, commutable ) :

    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    ARG = ','.join( [ 'c' for i in range( Tc ) ] )

    print( 'template < Algorithm A, typename T, T_fp(T) >' )
    print( 'INLINE auto constexpr operator_mul_exception ( T const& a, T const& b, bool & flag ) {' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  flag = true; {' )
    print( '    if ( fp<T>::isnan( a ) || fp<T>::isnan( b ) ) {' )
    print( '      auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );' )
    print( '      return TX{ '+ARG+' };' )
    print( '    }' )
    print( '    if ( fp<T>::isinf( a ) && fp<T>::isinf( b ) ) {' )
    print( '      auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::inf, a ), b );' )
    print( '      return TX{ '+ARG+' };' )
    print( '    }' )
    print( '    if ( fp<T>::isinf( a ) ) {' )
    print( '      if ( fp<T>::is_zero( b ) ) {' )
    print( '        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );' )
    print( '        return TX{ '+ARG+' };' )
    print( '      } else {' )
    print( '        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::inf, a ), b );' )
    print( '        return TX{ '+ARG+' };' )
    print( '      }' )
    print( '    }' )
    print( '    if ( fp<T>::isinf( b ) ) {' )
    print( '      if ( fp<T>::is_zero( a ) ) {' )
    print( '        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );' )
    print( '        return TX{ '+ARG+' };' )
    print( '      } else {' )
    print( '        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::inf, a ), b );' )
    print( '        return TX{ '+ARG+' };' )
    print( '      }' )
    print( '    }' )
    print( '    if ( fp<T>::is_zero( a ) || fp<T>::is_zero( b ) ) {' )
    print( '      auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::zero, a ), b );' )
    print( '      return TX{ '+ARG+' };' )
    print( '    }' )
    print( '  }' )
    print( '  //' )
    print( '  flag = false; return TX::zero();' )
    print( '}' )

    gen_exception( Tc, description, func, op, commutable )
    print( '#endif' )

def gen_exception_div( Tc, description, func, op, commutable ) :

    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    ARG = ','.join( [ 'c' for i in range( Tc ) ] )

    print( 'template < Algorithm A, typename T, T_fp(T) >' )
    print( 'INLINE auto constexpr operator_div_exception ( T const& a, T const& b, bool & flag ) {' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  flag = true; {' )
    print( '    if ( fp<T>::isnan( a ) || fp<T>::isnan( b ) ) {' )
    print( '      auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );' )
    print( '      return TX{ '+ARG+' };' )
    print( '    }' )
    print( '    if ( fp<T>::isinf( a ) && fp<T>::isinf( b ) ) {' )
    print( '      auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );' )
    print( '      return TX{ '+ARG+' };' )
    print( '    }' )
    print( '    if ( fp<T>::isinf( a ) ) {' )
    print( '      auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::inf, a ), b );' )
    print( '      return TX{ '+ARG+' };' )
    print( '    }' )
    print( '    if ( fp<T>::isinf( b ) ) {' )
    print( '      auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::zero, a ), b );' )
    print( '      return TX{ '+ARG+' };' )
    print( '    }' )
    print( '    if ( fp<T>::is_zero( b ) ) {' )
    print( '      if ( fp<T>::is_zero( a ) ) {' )
    print( '        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );' )
    print( '        return TX{ '+ARG+' };' )
    print( '      } else {' )
    print( '        auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::inf, a ), b );' )
    print( '        return TX{ '+ARG+' };' )
    print( '      }' )
    print( '    }' )
    print( '  }' )
    print( '  //' )
    print( '  flag = false; return TX::zero();' )
    print( '}' )

    gen_exception( Tc, description, func, op, commutable )
    print( '#endif' )

# ************************************************************************************************
#
# ************************************************************************************************
def gen_op_stub( Tc, description, func, op, commutable ) :

    print( 'template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb)',end='' )
    if commutable == 1 :
        print( ', T_assert( TXa::L >= TXb::L ) >' )
    else :
        print( ' >' )
    print( 'INLINE auto constexpr operator_{func} ( TXa const& a, TXb const& b ) {{'.format( func=func ) )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  bool flag = false;' )
    print( '  auto e = {m}X_real::operator_{func}_exception ( a, b, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '  if ( flag ) { return e; }' )
    print( '#endif' )
    print( '  return {m}X_real::operator_{func}_body ( a, b );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    if commutable == 1 :
        print( 'template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb), T_assert( std::is_same<typename TXa::base_T,typename TXb::base_T>::value && TXa::L < TXb::L ) >' )
        print( 'INLINE auto constexpr operator_{func} ( TXa const& a, TXb const& b ) {{'.format( func=func ) )
        print( '  return {m}X_real::operator_{func} ( b, a );'.format( m=mX_type(Tc), func=func ) )
        print( '}' )

    print( 'template < typename TXa, typename T, T_mX(TXa), T_fp(T), T_assert( std::is_same<typename TXa::base_T,T>::value ) >' )
    print( 'INLINE auto constexpr operator_{func} ( TXa const& a, T const& b ) {{'.format( func=func ) )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  bool flag = false;' )
    print( '  auto e = {m}X_real::operator_{func}_exception ( a, b, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '  if ( flag ) { return e; }' )
    print( '#endif' )
    print( '  return {m}X_real::operator_{func}_body ( a, b );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    print( 'template < typename T, typename TXb, T_fp(T), T_mX(TXb), T_assert( std::is_same<T,typename TXb::base_T>::value ) >' )
    print( 'INLINE auto constexpr operator_{func} ( T const& a, TXb const& b ) {{'.format( func=func ) )
    if commutable == 1 :
        print( '  return {m}X_real::operator_{func} ( b, a );'.format( m=mX_type(Tc), func=func ) )
    else :
        print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
        print( '  bool flag = false;' )
        print( '  auto e = {m}X_real::operator_{func}_exception ( a, b, flag );'.format( m=mX_type(Tc), func=func ) )
        print( '  if ( flag ) { return e; }' )
        print( '#endif' )
        print( '  return {m}X_real::operator_{func}_body ( a, b );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    print( 'template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >' )
    print( 'INLINE auto constexpr operator_{func} ( T const& a, T const& b ) {{'.format( func=func ) )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  bool flag = false;' )
    print( '  auto e = {m}X_real::operator_{func}_exception<A> ( a, b, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '  if ( flag ) { return e; }' )
    print( '#endif' )
    print( '  return {m}X_real::operator_{func}_body<T,A> ( a, b );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

def gen_op_operator( Tc, description, func, op, commutable ) :

    print( 'template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb), T_assert( std::is_same<typename TXa::base_T,typename TXb::base_T>::value && std::max( TXa::L, TXb::L ) == {L} ) >'.format( L=Tc ) )
    print( 'INLINE auto constexpr operator{op} ( TXa const& a, TXb const& b ) {{'.format( op=op ) )
    print( '  return {m}X_real::operator_{func} ( a, b );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    print( 'template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >' )
    print( 'INLINE auto constexpr operator{op} ( {m}X_real::{m}x_real<T,A> const& a, Ts const& b ) {{'.format( op=op, m=mX_type(Tc) ) )
    print( '  return {m}X_real::operator_{func} ( a, T(b) );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    print( 'template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >' )
    print( 'INLINE auto constexpr operator{op} ( Ts const& a, {m}X_real::{m}x_real<T,A> const& b ) {{'.format( op=op, m=mX_type(Tc) ) )
    if commutable == 1 :
        print( '  return b {op} a;'.format( op=op ) )
    else :
        print( '  return {m}X_real::operator_{func} ( T(a), b );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

def gen_op_operator_sub( Tc, description, func, op, commutable ) :

    print( 'template < typename TXa, typename TXb, T_mX(TXa), T_mX(TXb), T_assert( std::is_same<typename TXa::base_T,typename TXb::base_T>::value && std::max( TXa::L, TXb::L ) =={L} ) >'.format( L=Tc ) )
    print( 'INLINE auto constexpr operator- ( TXa const& a, TXb const& b ) {' )
    print( '  return a + (-b);' )
    print( '}' )

    print( 'template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >' )
    print( 'INLINE auto constexpr operator- ( {m}X_real::{m}x_real<T,A> const& a, Ts const& b ) {{'.format( m=mX_type(Tc) ) )
    print( '  return a + (-b);' )
    print( '}' )

    print( 'template < Algorithm A=Algorithm::Accurate, typename T, typename Ts, T_scalar(Ts) >' )
    print( 'INLINE auto constexpr operator- ( Ts const &a, {m}X_real::{m}x_real<T,A> const& b ) {{'.format( m=mX_type(Tc) ) )
    print( '  return a + (-b);' )
    print( '}' )

def gen_add ( Tc ) :

    description = 'Addition'
    func = 'add'
    op = '+'
    commutable = 1

    print( '//' )
    print( '// {}'.format( description) )
    print( '//' )

    print( '//' )
    gen_op_body( Tc, description, func, op, commutable )
    print( '//' )
    gen_exception_add( Tc, description, func, op, commutable )
    print( '//' )
    gen_op_stub( Tc, description, func, op, commutable )
    print( '//' )
    gen_op_operator( Tc, description, func, op, commutable )
    print( '//' )
    print( '' )

def gen_sub ( Tc ) :

    description = 'Substraction'
    func = 'sub'
    op = '-'
    commutable = 0

    print( '//' )
    print( '// {}'.format( description) )
    print( '//' )

    print( '//' )
    gen_op_operator_sub( Tc, description, func, op, commutable )
    print( '//' )
    print( '' )

def gen_mul ( Tc ) :

    description = 'Multiplication'
    func = 'mul'
    op = '*'
    commutable = 1

    print( '//' )
    print( '// {}'.format( description) )
    print( '//' )

    print( '//' )
    gen_op_body( Tc, description, func, op, commutable )
    print( '//' )
    gen_exception_mul( Tc, description, func, op, commutable )
    print( '//' )
    gen_op_stub( Tc, description, func, op, commutable )
    print( '//' )
    gen_op_operator( Tc, description, func, op, commutable )
    print( '//' )
    print( '' )

def gen_div ( Tc ) :

    description = 'Division'
    func = 'div'
    op = '/'
    commutable = 0

    print( '//' )
    print( '// {}'.format( description) )
    print( '//' )

    print( '//' )
    gen_op_body( Tc, description, func, op, commutable )
    print( '//' )
    gen_exception_div( Tc, description, func, op, commutable )
    print( '//' )
    gen_op_stub( Tc, description, func, op, commutable )
    print( '//' )
    gen_op_operator( Tc, description, func, op, commutable )
    print( '//' )
    print( '' )

def gen_abs ( Tc ) :

    description = 'Absolute value'
    func = 'abs'
    op = 'abs'
    commutable = 0

    print( '//' )
    print( '// {}'.format( description) )
    print( '//' )

    print( '//' )
    print( 'template < typename TXa, T_mX(TXa) >' )
    print( 'INLINE auto constexpr operator_abs_body ( TXa const& a ) {' )
    print( '  using T = typename TXa::base_T;' )
    print( '  using TX = {m}X_real::{m}x_real<T,TXa::base_A>;'.format( m=mX_type(Tc) ) )
    print( '  if ( is_negative( a ) ) {' )
    print( '    return TX{ -a };' )
    print( '  } else {' )
    print( '    return TX{  a };' )
    print( '  }' )
    print( '}' )
    print( '//' )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( 'template < typename TXa, T_mX(TXa) >' )
    print( 'INLINE auto constexpr operator_abs_exception ( TXa const& a, bool & flag ) {' )
    print( '  using T = typename TXa::base_T;' )
    print( '  using TX = {m}X_real::{m}x_real<T,TXa::base_A>;'.format( m=mX_type(Tc) ) )
    print( '  flag = true; {' )
    print( '    if ( isnan( a ) ) { return TX::nan(); }' )
    print( '    if ( isinf( a ) ) { return TX::inf(); }' )
    print( '  }' )
    print( '  //' )
    print( '  flag = false; return TX::zero();' )
    print( '}' )
    print( '#endif' )
    print( '//' )
    print( 'template < typename TXa, T_mX(TXa) >' )
    print( 'INLINE auto constexpr operator_abs ( TXa const& a ) {' )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  bool flag;' )
    print( '  auto e = {m}X_real::operator_abs_exception ( a, flag );'.format( m=mX_type(Tc) ) )
    print( '  if ( flag ) { return e; }' )
    print( '#endif' )
    print( '  return {m}X_real::operator_abs_body ( a );'.format( m=mX_type(Tc) ) )
    print( '}' )
    print( 'template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >' )
    print( 'INLINE auto constexpr operator_abs ( T const& a ) {' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  return TX{ std::abs( a ) };' )
    print( '}' )
    print( '//' )
    print( 'template < typename TXa, T_mX(TXa) >' )
    print( 'INLINE auto constexpr abs ( TXa const& a ) {' )
    print( '  return {m}X_real::operator_abs ( a );'.format( m=mX_type(Tc) ) )
    print( '}' )
    print( 'template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >' )
    print( 'INLINE auto constexpr abs ( T const& a ) {' )
    print( '  return {m}X_real::operator_abs<A> ( a );'.format( m=mX_type(Tc) ) )
    print( '}' )
    print( '//' )
    print( '' )

def gen_uni( Tc ) :

    print( '//' )
    print( '// Unitary operators' )
    print( '//' )
    print( '//' )
    print( 'template < typename T, Algorithm Aa >' )
    print( 'INLINE auto constexpr reversed_sign ( {m}X_real::{m}x_real<T,Aa> const& a ) {{'.format( m=mX_type(Tc) ) )
    print( '  return {m}X_real::{m}x_real<T,Aa>( '.format( m=mX_type(Tc) ) +
            ','.join( [ '-a.x[{i}]'.format( i=i ) for i in range( Tc ) ] )+' );' )
    print( '}' )
    print( '//' )
    print( 'template < typename T, Algorithm Aa >' )
    print( 'INLINE auto constexpr operator+ ( {m}X_real::{m}x_real<T,Aa> const& a ) {{'.format( m=mX_type(Tc) ) )
    print( '  return a;' )
    print( '}' )
    print( 'template < typename T, Algorithm Aa >' )
    print( 'INLINE auto constexpr operator- ( {m}X_real::{m}x_real<T,Aa> const& a ) {{'.format( m=mX_type(Tc) ) )
    print( '  return {m}X_real::reversed_sign( a );'.format( m=mX_type(Tc) ) )
    print( '}' )
    print( '//' )
    print( '' )


if __name__ == '__main__' :

    args = sys.argv
    if len(args) != 3 :
        exit( 1 )

    #Tc = 4
    Tc = int(args[1])
    if Tc < 2 or Tc > 5 :
        exit( 0 )

    K = int(args[2])

    if K == 0 :
        gen_add( Tc )
    if K == 1 :
        gen_sub( Tc )
    if K == 2 :
        gen_mul( Tc )
    if K == 3 :
        gen_div( Tc )
    if K == 4 :
        gen_abs( Tc )
    if K == 5 :
        gen_uni( Tc )

