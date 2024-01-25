import sys

def mX_suffics ( ACC, width ) :
    if ACC > 0 :
        return [ '_', '_SW', '_DW', '_TW', '_QW' ][width]
    else :
        return [ '_', '_SW', '_PA', '_QTW', '_QQW' ][width]

def mX_type ( width ) :
    return [ '_', 's', 'd', 't', 'q' ][width]
def mX_Type ( width ) :
    return [ '_', 'S', 'D', 'T', 'Q' ][width]

def mX_arg ( width, var ) :
    return [ '_', '_SX_({})', '_DX_({})', '_TX_({})', '_QX_({})' ][width].format(var)

def commonA ( Ta, Tb ) :
    A = ''
    if Ta > 1 and Tb == 1 :
        A = 'a'
    if Tb > 1 and Ta == 1 :
        A = 'b'
    return A

def fname ( Func, NA, NC, ACC ) :
    return '{}{}{}'.format(
        Func, mX_suffics( ACC, NA ), mX_suffics( ACC, NC ) )

def caller_head ( Func, NA, NC, ACC, va, vc ) :
    line = fname( Func, NA, NC, ACC )
    line = line + ' ( {}, {} );'.format( mX_arg(NA,va), mX_arg(NC,vc) )
    return line

def template( Ta ) :
    line = 'template < typename T'
    if Ta > 1 :
        line = line + ', Algorithm A'
    if Ta == 1 :
        line = line + ', Algorithm A, IF_T_fp<T>'
    line = line + ' >'
    return line

def arg_list( Ta ) :
    line= '('
    if Ta == 1 :
        line = line + ' T const& a'
    else :
        line = line + ' {m}X_real::{m}x_real<T,A> const& a'.format( m=mX_type(Ta) )
    line = line + ' ) {'
    return line


# ************************************************************************************************
#
# ************************************************************************************************
def gen_op_body( Tc, description, func, op, commutable ) :
    for Ta in reversed( range( 1, 4+1 ) ) :
            #if Ta > Tc or Tb > Tc :
            #    continue

            print( template( Ta ) )
            print( 'INLINE auto const operator_{}_body '.format( func ) + arg_list( Ta ) )

            line = '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) )
            print( line )

            print( '  TX c;' )
            print( '  if ( A == Algorithm::Accurate ) {' )
            print( caller_head( '    QxW::{}'.format(func), Ta,Tc,1, 'a','c' ) )
            print( '  } else {' )
            print( caller_head( '    QxW::{}'.format(func), Ta,Tc,0, 'a','c' ) )
            print( '    if ( A != Algorithm::Quasi ) { Normalize( c ); }' )
            print( '  }' )
            print( '  return c;' )
            print( '}' )

# ************************************************************************************************
#
# ************************************************************************************************
def gen_exception( Tc, description, func, op, commutable ) :

    print( 'template < typename TXa, IF_T_mX<TXa> >' )
    print( 'INLINE auto const operator_{func}_exception ( TXa const& a, bool & flag ) {{'.format( func=func ) )
    print( '  Algorithm constexpr A = TXa::base_A;' )
    print( '  auto sa = a.quick_Normalized();' )
    print( '  return {m}X_real::operator_{func}_exception<A> ( sa, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

def gen_exception_abs( Tc, description, func, op, commutable ) :

    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    ARG = ','.join( [ 'c' for i in range( Tc ) ] )

    print( 'template < Algorithm A, typename T, IF_T_fp<T> >' )
    print( 'INLINE auto const operator_abs_exception ( T const& a, bool & flag ) {' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  flag = true; {' )
    print( '    if ( fp<T>::isnan( a ) ) { TX::nan(); }' )
    print( '    if ( fp<T>::isinf( a ) ) { TX::inf(); }' )
    print( '  }' )
    print( '  //' )
    print( '  flag = false; return TX::zero();' )
    print( '}' )
    gen_exception( Tc, description, func, op, commutable )
    print( '#endif' )

def gen_exception_sqrt( Tc, description, func, op, commutable ) :

    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    ARG = ','.join( [ 'c' for i in range( Tc ) ] )

    print( 'template < Algorithm A, typename T, IF_T_fp<T> >' )
    print( 'INLINE auto const operator_sqrt_exception ( T const& a, bool & flag ) {' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  flag = true; {' )
    print( '    if ( fp<T>::isnan( a ) ) { auto c=a; return TX{ '+ARG+' }; }' )
    print( '    if ( fp<T>::is_negative( a ) ) { return TX::nan(); }' )
    print( '    if ( fp<T>::isinf( a ) ) { return TX::inf(); }' )
    print( '    if ( fp<T>::is_zero( a ) ) { auto c=a; return TX{ '+ARG+' }; }' )
    print( '  }' )
    print( '  //' )
    print( '  flag = false; return TX::zero();' )
    print( '}' )
    gen_exception( Tc, description, func, op, commutable )
    print( '#endif' )


# ************************************************************************************************
#
# ************************************************************************************************
def gen_op_stub_sqrt( Tc, description, func, op, commutable ) :

    print( 'template < typename TXa, IF_T_mX<TXa> >' )
    print( 'INLINE auto const operator_{func} ( TXa const& a ) {{'.format( func=func ) )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  bool flag;' )
    print( '  auto e = {m}X_real::operator_{func}_exception ( a, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '  if ( flag ) { return e; }' )
    print( '#else' )
    print( '  if ( a.is_zero() ) {{ return {m}X_real::mX_real<TXa>{{ a }}; }}'.format( m=mX_type(Tc) ) )
    print( '#endif' )
    print( '  Algorithm constexpr A = TXa::base_A;' )
    print( '  if ( A != Algorithm::Quasi ) {' )
    print( '    return {m}X_real::operator_{func}_body ( a );'.format( m=mX_type(Tc), func=func ) )
    print( '  } else {' )
    print( '    return {m}X_real::operator_{func}_body ( a.element_rotate() );'.format( m=mX_type(Tc), func=func ) )
    print( '  }' )
    print( '}' )

    print( 'template < Algorithm A=Algorithm::Accurate, typename T, IF_T_fp<T> >' )
    print( 'INLINE auto const operator_{func} ( T const& a ) {{'.format( func=func ) )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  bool flag;' )
    print( '  auto e = {m}X_real::operator_{func}_exception<A> ( a, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '  if ( flag ) { return e; }' )
    print( '#else' )
    print( '  if ( fp<T>::is_zero( a ) ) {{ return {m}X_real::{m}x_real<T,A>{{ a }}; }}'.format( m=mX_type(Tc) ) )
    print( '#endif' )
    print( '  return {m}X_real::operator_{func}_body<T,A> ( a );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )


# ************************************************************************************************
#
# ************************************************************************************************
def gen_op_operator( Tc, description, func, op, commutable ) :

    print( 'template < typename TXa, IF_T_mX<TXa> >' )
    print( 'INLINE auto const {op} ( TXa const& a ) {{'.format( op=op ) )
    print( '  return {m}X_real::operator_{func} ( a );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    print( 'template < Algorithm A=Algorithm::Accurate, typename T, IF_T_fp<T> >' )
    print( 'INLINE auto const {op} ( T const& a ) {{'.format( op=op ) )
    print( '  return {m}X_real::operator_{func}<A> ( a );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )


# ************************************************************************************************
#
# ************************************************************************************************
def gen_op_member( Tc, description, func, op, commutable ) :

    print( 'template < typename T, Algorithm A >' )
    print( 'INLINE {m}X_real::{m}x_real<T,A> constexpr {m}X_real::{m}x_real<T,A>::{op} ( {m}X_real::{m}x_real<T,A> const& a ) {{'.format( m=mX_type(Tc), op=op ) )
    print( '  return {m}X_real::operator_{func} ( a );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )


# ************************************************************************************************
#
# ************************************************************************************************
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
    print( '  return {m}X_real::mX_real<TXa>{{ is_negative( a ) ? -a : a }};'.format( m=mX_type(Tc) ) )
    print( '}' )
    print( '//' )
    gen_exception_abs( Tc, description, func, op, commutable )
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
    gen_op_member( Tc, description, func, op, commutable )
    print( '//' )
    print( '' )

def gen_sqrt ( Tc ) :

    description = 'Square root'
    func = 'sqrt'
    op = 'sqrt'
    commutable = 0

    print( '//' )
    print( '// {}'.format( description) )
    print( '//' )

    print( '//' )
    gen_op_body( Tc, description, func, op, commutable )
    print( '//' )
    gen_exception_sqrt( Tc, description, func, op, commutable )
    print( '//' )
    gen_op_stub_sqrt( Tc, description, func, op, commutable )
    print( '//' )
    gen_op_operator( Tc, description, func, op, commutable )
    print( '//' )
    gen_op_member( Tc, description, func, op, commutable )
    print( '//' )
    print( '' )

def gen_fmin ( Tc ) :

    description = 'f minimum'
    func = 'fmin'
    op = 'fmin'
    commutable = 0

    print( '//' )
    print( '// {}'.format( description) )
    print( '//' )

    print( '//' )
    print( 'template < typename T, Algorithm Aa, Algorithm Ab, IF_A2_noQuasi<Aa,Ab> >' )
    print( 'INLINE auto const operator_fmin_body ( {m}X_real::{m}x_real<T,Aa> const& a, {m}X_real::{m}x_real<T,Ab> const &b ) {{'.format( m=mX_type(Tc) ) )
    print( '  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  int i; for(i=0;i<TX::L-1;i++) { if ( a.x[i] != b.x[i] ) break; }' )
    print( '  return TX{ ( a.x[i] <= b.x[i] ) ? a : b };' )
    print( '}' )
    print( '//' )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( 'template < typename T, Algorithm Aa, Algorithm Ab >' )
    print( 'INLINE auto const operator_fmin_exception ( {m}X_real::{m}x_real<T,Aa> const& a, {m}X_real::{m}x_real<T,Ab> const &b, bool & flag ) {{'.format( m=mX_type(Tc) ) )
    print( '  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  flag = true; {' )
    print( '    if ( isnan( a ) || isnan( b ) ) { return TX::nan(); }' )
    print( '    if ( isinf( a ) && isinf( b ) ) {' )
    print( '      return ( is_positive( a ) && is_positive( b ) ) ? a : -TX::inf();' )
    print( '    }' )
    print( '    if ( isinf( a ) ) { return is_negative( a ) ? a : b; }' )
    print( '    if ( isinf( b ) ) { return is_negative( b ) ? b : a; }' )
    print( '  }' )
    print( '  //' )
    print( '  flag = false; return TX::zero();' )
    print( '}' )
    print( '#endif' )
    print( '//' )
    print( 'template < typename T, Algorithm Aa, Algorithm Ab >' )
    print( 'INLINE auto const operator_fmin ( {m}X_real::{m}x_real<T,Aa> const& a, {m}X_real::{m}x_real<T,Ab> const &b ) {{'.format( m=mX_type(Tc) ) )
    print( '  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  bool flag;' )
    print( '  auto e = {m}X_real::operator_fmin_exception( a, b, flag );'.format( m=mX_type(Tc) ) )
    print( '  if ( flag ) { return e; }' )
    print( '#endif' )
    print( '  if ( A == Algorithm::Quasi ) {' )
    print( '    using TT = typename  TX::type_Accurate;' )
    print( '    return TX{{ {m}X_real::operator_fmin_body( TT{{ a }}, TT{{ b }} ) }};'.format( m=mX_type(Tc) ) )
    print( '  } else {' )
    print( '    return {m}X_real::operator_fmin_body( a, b );'.format( m=mX_type(Tc) ) )
    print( '  }' )
    print( '}' )
    print( '//' )
    print( 'template < typename T, Algorithm Aa, Algorithm Ab >' )
    print( 'INLINE auto const fmin ( {m}X_real::{m}x_real<T,Aa> const& a, {m}X_real::{m}x_real<T,Ab> const &b ) {{'.format( m=mX_type(Tc) ) )
    print( '  return {m}X_real::operator_fmin( a, b );'.format( m=mX_type(Tc) ) )
    print( '}' )
    print( '//' )
    print( '' )

def gen_fmax ( Tc ) :

    description = 'f maximum'
    func = 'fmax'
    op = 'fmax'
    commutable = 0

    print( '//' )
    print( '// {}'.format( description) )
    print( '//' )

    print( '//' )
    print( 'template < typename T, Algorithm Aa, Algorithm Ab, IF_A2_noQuasi<Aa,Ab> >' )
    print( 'INLINE auto const operator_fmax_body ( {m}X_real::{m}x_real<T,Aa> const& a, {m}X_real::{m}x_real<T,Ab> const &b ) {{'.format( m=mX_type(Tc) ) )
    print( '  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  int i; for(i=0;i<TX::L-1;i++) { if ( a.x[i] != b.x[i] ) break; }' )
    print( '  return TX{ ( a.x[i] >= b.x[i] ) ? a : b };' )
    print( '}' )
    print( '//' )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( 'template < typename T, Algorithm Aa, Algorithm Ab >' )
    print( 'INLINE auto const operator_fmax_exception ( {m}X_real::{m}x_real<T,Aa> const& a, {m}X_real::{m}x_real<T,Ab> const &b, bool & flag ) {{'.format( m=mX_type(Tc) ) )
    print( '  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  flag = true; {' )
    print( '    if ( isnan( a ) || isnan( b ) ) { return TX::nan(); }' )
    print( '    if ( isinf( a ) && isinf( b ) ) {' )
    print( '      return ( is_negative( a ) && is_negative( b ) ) ? a : TX::inf();' )
    print( '    }' )
    print( '    if ( isinf( a ) ) { return is_positive( a ) ? a : b; }' )
    print( '    if ( isinf( b ) ) { return is_positive( b ) ? b : a; }' )
    print( '  }' )
    print( '  //' )
    print( '  flag = false; return TX::zero();' )
    print( '}' )
    print( '#endif' )
    print( '//' )
    print( 'template < typename T, Algorithm Aa, Algorithm Ab >' )
    print( 'INLINE auto const operator_fmax ( {m}X_real::{m}x_real<T,Aa> const& a, {m}X_real::{m}x_real<T,Ab> const &b ) {{'.format( m=mX_type(Tc) ) )
    print( '  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  bool flag;' )
    print( '  auto e = {m}X_real::operator_fmax_exception( a, b, flag );'.format( m=mX_type(Tc) ) )
    print( '  if ( flag ) { return e; }' )
    print( '#endif' )
    print( '  if ( A == Algorithm::Quasi ) {' )
    print( '    using TT = typename TX::type_Accurate;' )
    print( '    return TX{{ {m}X_real::operator_fmax_body( TT{{ a }}, TT{{ b }} ) }};'.format( m=mX_type(Tc) ) )
    print( '  } else {' )
    print( '    return {m}X_real::operator_fmax_body( a, b );'.format( m=mX_type(Tc) ) )
    print( '  }' )
    print( '}' )
    print( '//' )
    print( 'template < typename T, Algorithm Aa, Algorithm Ab >' )
    print( 'INLINE auto const fmax ( {m}X_real::{m}x_real<T,Aa> const& a, {m}X_real::{m}x_real<T,Ab> const &b ) {{'.format( m=mX_type(Tc) ) )
    print( '  return {m}X_real::operator_fmax( a, b );'.format( m=mX_type(Tc) ) )
    print( '}' )
    print( '//' )
    print( '' )

def gen_prng ( Tc ) :

    description = 'PRNG'
    func = 'prng'
    op = 'prng'
    commutable = 0

    print( '//' )
    print( '// {}'.format( description) )
    print( '//' )

    print( '//' )
    print( 'template < typename T, Algorithm A >' )
    print( 'INLINE auto rand () -> if_T_double<T,{m}X_real::{m}x_real<T,A>> {{'.format( m=mX_type(Tc) ) )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  auto constexpr f = fp<T>::half / (1 << 30);' )
    print( '  auto g = f;' )
    print( '  auto r = TX::zero();' )
    print( '  auto constexpr bits = std::numeric_limits<TX>::digits;' )
    print( '  auto constexpr rand_bits = std::numeric_limits<int>::digits - 1;' )
    print( '  for(int i=0; i<bits; i+=rand_bits ) {' )
    print( '    auto b_ = fp<T>::rand();' )
    print( '    auto b = T (b_ );' )
    print( '    auto c = b * g;' )
    print( '         r = r + TX{ c };' )
    print( '         g = g * f;' )
    print( '  }' )
    print( '  return r;' )
    print( '}' )
    print( 'template < typename T, Algorithm A >' )
    print( 'INLINE auto rand () -> if_T_float<T,{m}X_real::{m}x_real<T,A>> {{'.format( m=mX_type(Tc) ) )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  auto constexpr f = fp<T>::half / (1 << 30);' )
    print( '  auto g = f;' )
    print( '  auto r = TX::zero();' )
    print( '  auto constexpr bits = std::numeric_limits<TX>::digits;' )
    print( '  auto constexpr rand_bits = std::numeric_limits<int>::digits - 1;' )
    print( '  for(int i=0; i<bits; i+=rand_bits ) {' )
    print( '    auto b_ = fp<T>::rand();' )
    print( '    auto b = T( ( b_ & 0x7fff0000 ) >> 16 ) * (1 << 16);' )
    print( '    auto c = b * g;' )
    print( '         r = r + TX{ c };' )
    print( '         b = T( b_ & 0x0000ffff );' )
    print( '         c = b * g;' )
    print( '         r = r + TX{ c };' )
    print( '         g = g * f;' )
    print( '  }' )
    print( '  return r;' )
    print( '}' )
    print( '//' )
    print( 'template < typename T, Algorithm A >' )
    print( 'INLINE {m}X_real::{m}x_real<T,A> constexpr {m}X_real::{m}x_real<T,A>::rand () {{'.format( m=mX_type(Tc) ) )
    print( '  return {m}X_real::rand<T,A>();'.format( m=mX_type(Tc) ) )
    print( '}' )
    print( '//' )

def gen_bit ( Tc ) :

    print( '//' )
    print( '// Bitwise or FP-bit-specific ops' )
    print( '//' )
    print( '//' )
    for func in { 'isinf', 'isnan', 'signbit' } :
        print( '  template < typename T, Algorithm Aa >' )
        print( '  INLINE auto const {func} ( {m}X_real::{m}x_real<T,Aa> const& a ) {{'.format( m=mX_type(Tc), func=func ) )
        print( '    return fp<T>::{func}( a.quick_Normalized() );'.format( func=func ) )
        print( '  }' )
    #print( 'template < typename T, Algorithm Aa >' )
    #print( '  INLINE auto const isinf ( {m}X_real::{m}x_real<T,Aa> const& a ) {{'.format( m=mX_type(Tc) ) )
    #print( '    return fp<T>::isinf( a.quick_Normalized() );' )
    #print( '  }' )
    #print( '  template < typename T, Algorithm Aa >' )
    #print( '  INLINE auto const isnan ( {m}X_real::{m}x_real<T,Aa> const& a ) {{'.format( m=mX_type(Tc) ) )
    #print( '    return fp<T>::isnan( a.quick_Normalized() );' )
    #print( '  }' )
    #print( '  template < typename T, Algorithm Aa >' )
    #print( '  INLINE auto const signbit ( {m}X_real::{m}x_real<T,Aa> const& a ) {{'.format( m=mX_type(Tc) ) )
    #print( '    return fp<T>::signbit( a.quick_Normalized() );' )
    #print( '  }' )
    for item in { 'is_zero ==', 'is_positive >', 'is_negative <' } :
        a_list = item.split()
        func = a_list[0]
        op = a_list[1]
        print( '  template < typename T, Algorithm Aa >' )
        print( '  INLINE bool constexpr {func} ( {m}X_real::{m}x_real<T,Aa> const& a ) {{'.format( m=mX_type(Tc), func=func ) )
        print( '    return a.quick_Normalized() {op} fp<T>::zero;'.format( op=op ) )
        print( '  }' )
    #print( '  template < typename T, Algorithm Aa >' )
    #print( '  INLINE bool constexpr is_zero ( {m}X_real::{m}x_real<T,Aa> const& a ) {{'.format( m=mX_type(Tc) ) )
    #print( '    return a.quick_Normalized() == fp<T>::zero;' )
    #print( '  }' )
    #print( '  template < typename T, Algorithm Aa >' )
    #print( '  INLINE bool constexpr is_positive ( {m}X_real::{m}x_real<T,Aa> const& a ) {{'.format( m=mX_type(Tc) ) )
    #print( '    return a.quick_Normalized() > fp<T>::zero;' )
    #print( '  }' )
    #print( '  template < typename T, Algorithm Aa >' )
    #print( '  INLINE bool constexpr is_negative ( {m}X_real::{m}x_real<T,Aa> const& a ) {{'.format( m=mX_type(Tc) ) )
    #print( '    return a.quick_Normalized() < fp<T>::zero;' )
    #print( '  }' )

    print( '//' )
    for func in [ 'isinf', 'isnan', 'signbit', 'is_zero', 'is_positive', 'is_negative' ] :
        print( ' template < typename T, Algorithm A >' )
        print( ' INLINE bool constexpr {m}X_real::{m}x_real<T,A>::{func} ( {m}X_real::{m}x_real<T,A> const& a ) {{'.format( m=mX_type(Tc), func=func ) )
        print( ' return {m}X_real::{func} ( a );'.format( m=mX_type(Tc), func=func ) )
        print( ' }' )
    print( '//' )
    print( '' )


def gen_members ( Tc ) :

    print( '//' )
    print( '// static member funtions' )
    print( '// definition is below outside of the struct definition block' )
    print( '//' )
    for func in { 'abs', 'sqrt' } :
        print( 'static INLINE {m}X_REAL<> constexpr {func} ( {m}X_REAL<> const& a );'.format( m=mX_Type(Tc), func=func ) )
    #print( 'static INLINE {m}X_REAL<> constexpr abs ( {m}X_REAL<> const& a );'.format( m=mX_Type(Tc) ) )
    #print( 'static INLINE {m}X_REAL<> constexpr sqrt ( {m}X_REAL<> const& a );'.format( m=mX_Type(Tc) ) )
    print( 'static INLINE {m}X_REAL<> constexpr rand ();'.format( m=mX_Type(Tc) ) )
    for func in { 'signbit', 'isinf', 'isnan', 'is_zero', 'is_positive', 'is_negative' } :
        print( 'static INLINE bool constexpr {func} ( {m}X_REAL<> const& a );'.format( m=mX_Type(Tc), func=func ) )
    #print( 'static INLINE bool      constexpr signbit ( {m}X_REAL<> const& a );'.format( m=mX_Type(Tc) ) )
    #print( 'static INLINE bool      constexpr isinf ( {m}X_REAL<> const& a );'.format( m=mX_Type(Tc) ) )
    #print( 'static INLINE bool      constexpr isnan ( {m}X_REAL<> const& a );'.format( m=mX_Type(Tc) ) )
    #print( 'static INLINE bool      constexpr is_zero ( {m}X_REAL<> const& a );'.format( m=mX_Type(Tc) ) )
    #print( 'static INLINE bool      constexpr is_positive ( {m}X_REAL<> const& a );'.format( m=mX_Type(Tc) ) )
    #print( 'static INLINE bool      constexpr is_negative ( {m}X_REAL<> const& a );'.format( m=mX_Type(Tc) ) )
    print( '//' )
    print( '' )
    print( '' )
    print( '//' )
    print( '// member functions (aka methods)' )
    print( '//' )
    print( '//' )
    print( 'INLINE void constexpr Normalize () { mX_real::Normalize( *this ); }' )
    print( '//' )
    for func in { 'signbit', 'isinf', 'isnan', 'is_zero', 'is_positive', 'is_negative' } :
        print( 'INLINE bool constexpr {func} () const {{ return {m}X_REAL<>::{func}( *this ); }}'.format( m=mX_Type(Tc), func=func ) )
    #print( 'INLINE bool constexpr signbit ()     const {{ return {m}X_REAL<>::signbit( *this ); }}'.format( m=mX_Type(Tc) ) )
    #print( 'INLINE bool constexpr isinf ()       const {{ return {m}X_REAL<>::isinf( *this ); }}'.format( m=mX_Type(Tc) ) )
    #print( 'INLINE bool constexpr isnan ()       const {{ return {m}X_REAL<>::isnan( *this ); }}'.format( m=mX_Type(Tc) ) )
    #print( 'INLINE bool constexpr is_zero ()     const {{ return {m}X_REAL<>::is_zero( *this ); }}'.format( m=mX_Type(Tc) ) )
    #print( 'INLINE bool constexpr is_positive () const {{ return {m}X_REAL<>::is_positive( *this ); }}'.format( m=mX_Type(Tc) ) )
    #print( 'INLINE bool constexpr is_negative () const {{ return {m}X_REAL<>::is_negative( *this ); }}'.format( m=mX_Type(Tc) ) )
    print( '//' )
    print( '' )


if __name__ == '__main__' :

    args = sys.argv
    if len(args) != 3 :
        exit( 1 )

    Tc = int(args[1])
    if Tc < 2 or Tc > 4 :
        exit( 0 )

    K = int(args[2])

    if K == 4 :
        gen_abs( Tc )
    if K == 5 :
        gen_sqrt( Tc )
    if K == 6 :
        gen_fmin( Tc )
    if K == 7 :
        gen_fmax( Tc )
    if K == 8 :
        gen_prng( Tc )
    if K == 9 :
        gen_bit ( Tc ) 
    if K == 10 :
        gen_members ( Tc ) 


