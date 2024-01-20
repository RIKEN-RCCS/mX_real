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
        line = line + ', Algorithm A, IF_T_fp<T>'
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
            print( 'INLINE auto const operator_{func}_body '.format( func=func ) + arg_list( Ta, Tb ) )
            #print( 'INLINE auto operator_{func}_body '.format( func=func ) + arg_list( Ta, Tb ) )

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
            print( caller_head( '    QxW::{}'.format(func), Ta,Tb,Tc,1, 'a','b','c' ) )
            print( '  } else {' )
            print( caller_head( '    QxW::{}'.format(func), Ta,Tb,Tc,0, 'a','b','c' ) )
            print( '    if ( A{a} != Algorithm::Quasi ) {{ Normalize( c ); }}'.format( a=A ) )
            print( '  }' )
            print( '  return c;' )
            print( '}' )

# ************************************************************************************************
#
# ************************************************************************************************
def gen_exception( Tc, description, func, op, commutable ) :

    print( 'template < typename TXa, typename TXb, IF_T_mX2<TXa,TXb> >' )
    print( 'INLINE auto const operator_{func}_exception ( TXa const& a, TXb const& b, bool & flag )'.format( func=func ) )
    print( '-> return_TX2< TXa, TXb, {L} > {{'.format( L=Tc ) )
    print( '  Algorithm constexpr A = commonAlgorithm< TXa::base_A, TXb::base_A >::algorithm;' )
    print( '  auto sa = a.quick_Normalized();' )
    print( '  auto sb = b.quick_Normalized();' )
    print( '  return {m}X_real::operator_{func}_exception<A> ( sa, sb, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    print( 'template < typename TXa, typename T, IF_T_mX<TXa>, IF_T_fp<T> >' )
    print( 'INLINE auto const operator_{func}_exception ( TXa const& a, T const& b, bool & flag )'.format( func=func ) )
    print( '-> return_TX< TXa, T, {L} > {{'.format( L=Tc ) )
    print( '  Algorithm constexpr A = TXa::base_A;' )
    print( '  auto sa = a.quick_Normalized();' )
    print( '  return {m}X_real::operator_{func}_exception<A> ( sa, b, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    if commutable == 1 :
        return

    print( 'template < typename TXb, typename T, IF_T_mX<TXb>, IF_T_fp<T> >' )
    print( 'INLINE auto const operator_{func}_exception ( T const& a, TXb const& b, bool & flag )'.format( func=func ) )
    print( '-> return_TX< TXb, T, {L} > {{'.format( L=Tc ) )
    print( '  Algorithm constexpr A = TXb::base_A;' )
    print( '  auto sb = b.quick_Normalized();' )
    print( '  return {m}X_real::operator_{func}_exception<A> ( a, sb, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

def gen_exception_add( Tc, description, func, op, commutable ) :

    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    ARG = ','.join( [ 'c' for i in range( Tc ) ] )

    print( 'template < Algorithm A, typename T, IF_T_fp<T> >' )
    print( 'INLINE auto const operator_add_exception ( T const& a, T const& b, bool & flag ) {' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  flag = true; {' )
    print( '    if ( fp<T>::isnan( a ) || fp<T>::isnan( b ) ) {' )
    print( '      auto c = fp<T>::copysign( fp<T>::copysign( fp<T>::nan, a ), b );' )
    print( '      return TX{ '+ARG+' };' )
    print( '    }' )
    print( '    if ( fp<T>::isinf( a ) && fp<T>::isinf( b ) ) {' )
    print( '      auto sign = std::signbit( a ) ^ std::signbit( b );' )
    print( '      if ( sign ) {' )
    print( '        auto c = fp<T>::nan;' )
    print( '        return TX{ '+ARG+' };' )
    print( '      } else {' )
    print( '        auto c = fp<T>::copysign( fp<T>::inf, a );' )
    print( '        return TX{ '+ARG+' };' )
    print( '      }' )
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

    print( 'template < Algorithm A, typename T, IF_T_fp<T> >' )
    print( 'INLINE auto const operator_mul_exception ( T const& a, T const& b, bool & flag ) {' )
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

    print( 'template < Algorithm A, typename T, IF_T_fp<T> >' )
    print( 'INLINE auto const operator_div_exception ( T const& a, T const& b, bool & flag ) {' )
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

    print( 'template < typename TXa, typename TXb, IF_T_mX2<TXa,TXb> >' )
    #print( 'INLINE auto const operator_{func} ( TXa const& a, TXb const& b )'.format( func=func ) )
    print( 'INLINE auto operator_{func} ( TXa const& a, TXb const& b )'.format( func=func ) )
    print( '-> cond_return_TX2<(TXa::L>=TXb::L),TXa,TXb,{L}> {{'.format( L=Tc ) )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  bool flag;' )
    print( '  auto e = {m}X_real::operator_{func}_exception ( a, b, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '  if ( flag ) { return e; }' )
    print( '#endif' )
    print( '  return {m}X_real::operator_{func}_body ( a, b );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    print( 'template < typename TXa, typename TXb, IF_T_mX2<TXa,TXb> >' )
    #print( 'INLINE auto const operator_{func} ( TXa const& a, TXb const& b )'.format( func=func ) )
    print( 'INLINE auto operator_{func} ( TXa const& a, TXb const& b )'.format( func=func ) )
    print( '-> cond_return_TX2<(TXa::L<TXb::L),TXa,TXb,{L}> {{'.format( L=Tc ) )
    if commutable == 1 :
        print( '  return {m}X_real::operator_{func} ( b, a );'.format( m=mX_type(Tc), func=func ) )
    else :
        print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
        print( '  bool flag;' )
        print( '  auto e = {m}X_real::operator_{func}_exception ( a, b, flag );'.format( m=mX_type(Tc), func=func ) )
        print( '  if ( flag ) { return e; }' )
        print( '#endif' )
        print( '  return {m}X_real::operator_{func}_body ( a, b );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    print( 'template < typename TXa, typename T, IF_T_fp<T>, IF_T_mX<TXa> >' )
    #print( 'INLINE auto const operator_{func} ( TXa const& a, T const& b )'.format( func=func ) )
    print( 'INLINE auto operator_{func} ( TXa const& a, T const& b )'.format( func=func ) )
    print( '-> return_TX<TXa,T,{L}> {{'.format( L=Tc ) )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  bool flag;' )
    print( '  auto e = {m}X_real::operator_{func}_exception ( a, b, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '  if ( flag ) { return e; }' )
    print( '#endif' )
    print( '  return {m}X_real::operator_{func}_body ( a, b );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    print( 'template < typename T, typename TXb, IF_T_fp<T>, IF_T_mX<TXb> >' )
    #print( 'INLINE auto const operator_{func} ( T const& a, TXb const& b )'.format( func=func ) )
    print( 'INLINE auto operator_{func} ( T const& a, TXb const& b )'.format( func=func ) )
    print( '-> return_TX<TXb,T,{L}> {{'.format( L=Tc ) )
    if commutable == 1 :
        print( '  return {m}X_real::operator_{func} ( b, a );'.format( m=mX_type(Tc), func=func ) )
    else :
        print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
        print( '  bool flag;' )
        print( '  auto e = {m}X_real::operator_{func}_exception ( a, b, flag );'.format( m=mX_type(Tc), func=func ) )
        print( '  if ( flag ) { return e; }' )
        print( '#endif' )
        print( '  return {m}X_real::operator_{func}_body ( a, b );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    print( 'template < Algorithm A=Algorithm::Accurate, typename T, IF_T_fp<T> >' )
    #print( 'INLINE auto const operator_{func} ( T const& a, T const& b ) {{'.format( func=func ) )
    print( 'INLINE auto operator_{func} ( T const& a, T const& b ) {{'.format( func=func ) )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  bool flag;' )
    print( '  auto e = {m}X_real::operator_{func}_exception<A> ( a, b, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '  if ( flag ) { return e; }' )
    print( '#endif' )
    print( '  return {m}X_real::operator_{func}_body<T,A> ( a, b );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

def gen_op_operator( Tc, description, func, op, commutable ) :

    print( 'template < typename TXa, typename TXb, IF_T_mX2<TXa,TXb> >' )
    #print( 'INLINE auto const operator{op} ( TXa const& a, TXb const& b )'.format( op=op ) )
    print( 'INLINE auto operator{op} ( TXa const& a, TXb const& b )'.format( op=op ) )
    print( '-> cond_return_TX2<(TXa::L=={L}||TXb::L=={L}),TXa,TXb,{L}> {{'.format( L=Tc ) )
    print( '  return {m}X_real::operator_{func} ( a, b );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    print( 'template < typename T, Algorithm A, typename Ts, IF_T_scalar<Ts> >' )
    #print( 'INLINE auto const operator{op} ( {m}X_real::{m}x_real<T,A> const& a, Ts const& b ) {{'.format( op=op, m=mX_type(Tc) ) )
    print( 'INLINE auto operator{op} ( {m}X_real::{m}x_real<T,A> const& a, Ts const& b ) {{'.format( op=op, m=mX_type(Tc) ) )
    print( '  auto const b_ = T(b);' )
    print( '  return {m}X_real::operator_{func} ( a, b_ );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    print( 'template < typename T, Algorithm A, typename Ts, IF_T_scalar<Ts> >' )
    #print( 'INLINE auto const operator{op} ( Ts const& a, {m}X_real::{m}x_real<T,A> const& b ) {{'.format( op=op, m=mX_type(Tc) ) )
    print( 'INLINE auto operator{op} ( Ts const& a, {m}X_real::{m}x_real<T,A> const& b ) {{'.format( op=op, m=mX_type(Tc) ) )
    if commutable == 1 :
        print( '  return b {op} a;'.format( op=op ) )
    else :
        print( '  auto const a_ = T(a);' )
        print( '  return {m}X_real::operator_{func} ( a_, b );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

def gen_op_operator_sub( Tc, description, func, op, commutable ) :

    print( 'template < typename TXa, typename TXb, IF_T_mX2<TXa,TXb> >' )
    #print( 'INLINE auto const operator- ( TXa const& a, TXb const& b )' )
    print( 'INLINE auto operator- ( TXa const& a, TXb const& b )' )
    print( '-> cond_return_TX2<(TXa::L=={L}||TXb::L=={L}),TXa,TXb,{L}> {{'.format( L=Tc ) )
    print( '  return a + (-b);' )
    print( '}' )

    print( 'template < typename T, Algorithm A, typename Ts, IF_T_scalar<Ts> >' )
    #print( 'INLINE auto const operator- ( {m}X_real::{m}x_real<T,A> const& a, Ts const& b ) {{'.format( m=mX_type(Tc) ) )
    print( 'INLINE auto operator- ( {m}X_real::{m}x_real<T,A> const& a, Ts const& b ) {{'.format( m=mX_type(Tc) ) )
    print( '  return a + (-b);' )
    print( '}' )

    print( 'template < Algorithm A=Algorithm::Accurate, typename T, typename Ts, IF_T_scalar<Ts> >' )
    #print( 'INLINE auto const operator- ( Ts const &a, {m}X_real::{m}x_real<T,A> const& b ) {{'.format( m=mX_type(Tc) ) )
    print( 'INLINE auto operator- ( Ts const &a, {m}X_real::{m}x_real<T,A> const& b ) {{'.format( m=mX_type(Tc) ) )
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
    print( 'template < typename TXa, IF_T_mX<TXa> >' )
    print( 'INLINE auto const operator_abs_body ( TXa const& a ) {' )
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
    print( 'template < typename TXa, IF_T_mX<TXa> >' )
    print( 'INLINE auto const operator_abs_exception ( TXa const& a, bool & flag ) {' )
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
    print( 'template < typename TXa, IF_T_mX<TXa> >' )
    print( 'INLINE auto const operator_abs ( TXa const& a ) {' )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  bool flag;' )
    print( '  auto e = {m}X_real::operator_abs_exception ( a, flag );'.format( m=mX_type(Tc) ) )
    print( '  if ( flag ) { return e; }' )
    print( '#endif' )
    print( '  return {m}X_real::operator_abs_body ( a );'.format( m=mX_type(Tc) ) )
    print( '}' )
    print( 'template < Algorithm A=Algorithm::Accurate, typename T, IF_T_fp<T> >' )
    print( 'INLINE auto const operator_abs ( T const& a ) {' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  return TX{ std::abs( a ) };' )
    print( '}' )
    print( '//' )
    print( 'template < typename TXa, IF_T_mX<TXa> >' )
    print( 'INLINE auto const abs ( TXa const& a ) {' )
    print( '  return {m}X_real::operator_abs ( a );'.format( m=mX_type(Tc) ) )
    print( '}' )
    print( 'template < Algorithm A=Algorithm::Accurate, typename T, IF_T_fp<T> >' )
    print( 'INLINE auto const abs ( T const& a ) {' )
    print( '  return {m}X_real::operator_abs<A> ( a );'.format( m=mX_type(Tc) ) )
    print( '}' )
    print( '//' )


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

