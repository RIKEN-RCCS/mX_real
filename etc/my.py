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
        line = line + ', Algorithm A, T_fp(T)'
    line = line + ' >'
    return line

def arg_list( Ta, Tb ) :
    line= '('
    if Ta == 1 :
        line = line + ' T & a'
    else :
        line = line + ' {m}X_real::{m}x_real<T,Aa> & a'.format( m=mX_type(Ta) )
    if Tb == 1 :
        line = line + ', T const& b'
    else :
        line = line + ', {m}X_real::{m}x_real<T,Ab> const& b'.format( m=mX_type(Tb) )
    line = line + ' ) {'
    return line

def gen_op_ow_body( Tc, description, func, op, commutable ) :
    Ta = Tc
    for Tb in reversed( range( 1, 4+1 ) ) :

       print( template( Ta, Tb ) )
       print( 'INLINE auto constexpr operator_{func}_ow_body '.format( func=func ) + arg_list( Ta, Tb ) )

       A = commonA( Ta, Tb )
       if Ta > 1 and Tb > 1 :
           print( '  Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;' )
       print( '  if ( A{a} == Algorithm::Accurate ) {{'.format( a=A ) )
       print( caller_head( '    QxW::{}'.format(func), Ta,Tb,Tc,1, 'a','b','a' ) )
       print( '  } else {' )
       print( caller_head( '    QxW::{}'.format(func), Ta,Tb,Tc,0, 'a','b','a' ) )
       print( '    if ( A{a} != Algorithm::Quasi ) {{ Normalize( a ); }}'.format( a=A ) )
       print( '  }' )
       print( '  return a;' )
       print( '}' )

def gen_op_ow_stub( Tc, description, func, op, commutable ) :

    print( 'template < typename T, Algorithm Aa, template < typename _Tb_, Algorithm _Ab_ > class TXb, Algorithm Ab, T_mX(TXb<T,Ab>), A_owAble(Aa,Ab) >' )
    print( 'INLINE auto constexpr operator_{func}_ow ( {m}X_real::{m}x_real<T,Aa> & a, TXb<T,Ab> const& b ) {{'.format( func=func, m=mX_type(Tc) ) )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  bool flag = false;' )
    print( '  auto e = {m}X_real::operator_{func}_exception ( a, b, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '  if ( flag ) { return e; }' )
    print( '#endif' )
    print( '  return {m}X_real::operator_{func}_ow_body ( a, b );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    print( 'template < typename T, Algorithm A >' )
    print( 'INLINE auto constexpr operator_{func}_ow ( {m}X_real::{m}x_real<T,A> & a, T const& b ) {{'.format( func=func, m=mX_type(Tc) ) )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  bool flag = false;' )
    print( '  auto e = {m}X_real::operator_{func}_exception ( a, b, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '  if ( flag ) { return e; }' )
    print( '#endif' )
    print( '  return {m}X_real::operator_{func}_ow_body ( a, b );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

def gen_op_ow_operator( Tc, description, func, op, commutable ) :

    print( 'template < typename T, Algorithm Aa, template < typename _Tb_, Algorithm _Ab_ > class TXb, Algorithm Ab, T_mX(TXb<T,Ab>), A_owAble(Aa,Ab) >' )
    print( 'INLINE auto constexpr operator{op}= ( {m}X_real::{m}x_real<T,Aa> & a, TXb<T,Ab> const& b ) {{'.format( m=mX_type(Tc), op=op ) )
    print( '  return {m}X_real::operator_{func}_ow ( a, b );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    print( 'template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >' )
    print( 'INLINE auto constexpr operator{op}= ( {m}X_real::{m}x_real<T,A> & a, Ts const& b ) {{'.format( m=mX_type(Tc), op=op ) )
    print( '  return {m}X_real::operator_{func}_ow ( a, T(b) );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

def gen_op_ow_operator_sub( Tc, description, func, op, commutable ) :

    print( 'template < typename T, Algorithm Aa, Algorithm Ab, template < typename _Tb_, Algorithm _Ab_ > class TXb, T_mX(TXb<T,Ab>), A_owAble(Aa,Ab) >' )
    print( 'INLINE auto constexpr operator-= ( {m}X_real::{m}x_real<T,Aa> & a, TXb<T,Ab> const& b ) {{'.format( m=mX_type(Tc) ) )
    print( '  return a += (-b);' )
    print( '}' )
    print( 'template < typename T, Algorithm A, typename Ts, T_scalar(Ts) >' )
    print( 'INLINE auto constexpr operator-= ( {m}X_real::{m}x_real<T,A> & a, Ts const& b ) {{'.format( m=mX_type(Tc) ) )
    print( '  return a += (-b);' )
    print( '}' )

def gen_add ( Tc ) :

    description = 'Addition + Overwrite'
    func = 'add'
    op = '+'
    commutable = 1

    print( '//' )
    gen_op_ow_body( Tc, description, func, op, commutable )
    print( '//' )
    gen_op_ow_stub( Tc, description, func, op, commutable )
    print( '//' )
    gen_op_ow_operator( Tc, description, func, op, commutable )
    print( '//' )
    print( '' )

def gen_sub ( Tc ) :

    description = 'Subtraction + Overwrite'
    func = 'sub'
    op = '-'
    commutable = 0

    print( '//' )
    gen_op_ow_operator_sub( Tc, description, func, op, commutable ) 
    print( '//' )
    print( '' )


def gen_mul ( Tc ) :

    description = 'Multiplication + Overwrite'
    func = 'mul'
    op = '*'
    commutable = 1

    print( '//' )
    gen_op_ow_body( Tc, description, func, op, commutable )
    print( '//' )
    gen_op_ow_stub( Tc, description, func, op, commutable )
    print( '//' )
    gen_op_ow_operator( Tc, description, func, op, commutable )
    print( '//' )
    print( '' )

def gen_div ( Tc ) :

    description = 'Division + Overwrite'
    func = 'div'
    op = '/'
    commutable = 0

    print( '//' )
    gen_op_ow_body( Tc, description, func, op, commutable )
    print( '//' )
    gen_op_ow_stub( Tc, description, func, op, commutable )
    print( '//' )
    gen_op_ow_operator( Tc, description, func, op, commutable )
    print( '//' )
    print( '' )


if __name__ == '__main__' :

    args = sys.argv
    if len(args) != 3 :
        exit(1)

    Tc = int(args[1])
    if Tc < 2 or Tc > 4 :
        exit(0)

    K = int(args[2])

    if K == 0 :
        gen_add ( Tc )
    if K == 1 :
        gen_sub ( Tc )
    if K == 2 :
        gen_mul ( Tc )
    if K == 3 :
        gen_div ( Tc )

