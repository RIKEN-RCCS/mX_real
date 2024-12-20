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
    if Ta == 1 :
        line = 'template < Algorithm A, typename T, T_fp(T) >'
    else :
        line = 'template < typename T, Algorithm A >'
    return line

def arg_list( Ta ) :
    line= '('
    if Ta == 1 :
        line = line + ' T const& a'
    else :
        line = line + ' {m}X_real::{m}x_real<T,A> const& a'.format( m=mX_type(Ta) )
    line = line + ' ) NOEXCEPT {'
    return line


# ************************************************************************************************
#
# ************************************************************************************************
def gen_op_body( Tc, description, func, op, commutable ) :
    for Ta in reversed( range( 1, 4+1 ) ) :
            #if Ta > Tc or Tb > Tc :
            #    continue

            print( template( Ta ) )
            print( 'INLINE auto constexpr operator_{}_body '.format( func ) + arg_list( Ta ) )

            line = '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) )
            print( line )

            print( '  TX c;' )
            print( '  if ( A <= Algorithm::WeakAccurate ) {' )
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

    print( 'template < typename TXa, T_mX(TXa) >' )
    print( 'INLINE auto constexpr operator_{func}_exception ( TXa const& a, bool & flag ) NOEXCEPT {{'.format( func=func ) )
    print( '  Algorithm constexpr A = TXa::base_A;' )
    print( '  auto sa = a.quick_Normalized();' )
    print( '  return {m}X_real::operator_{func}_exception<A> ( sa, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

def gen_exception_abs( Tc, description, func, op, commutable ) :

    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    ARG = ','.join( [ 'c' for i in range( Tc ) ] )

    print( 'template < Algorithm A, typename T, T_fp(T) >' )
    print( 'INLINE auto constexpr operator_abs_exception ( T const& a, bool & flag ) NOEXCEPT {' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  {' )
    print( '    if ( fp<T>::isnan( a ) ) { flag = true; return TX::nan(); }' )
    print( '    if ( fp<T>::isinf( a ) ) { flag = true; return TX::inf(); }' )
    print( '  }' )
    print( '  //' )
    print( '  flag = false; return TX::zero();' )
    print( '}' )
    gen_exception( Tc, description, func, op, commutable )
    print( '#endif' )

def gen_exception_sqrt( Tc, description, func, op, commutable ) :

    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    ARG = ','.join( [ 'c' for i in range( Tc ) ] )

    print( 'template < Algorithm A, typename T, T_fp(T) >' )
    print( 'INLINE auto constexpr operator_sqrt_exception ( T const& a, bool & flag ) NOEXCEPT {' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  {' )
    print( '    if ( fp<T>::isnan( a ) ) { flag = true; auto c=a; return TX{ '+ARG+' }; }' )
    print( '    if ( fp<T>::is_negative( a ) ) { flag = true; return TX::nan(); }' )
    print( '    if ( fp<T>::isinf( a ) ) { flag = true; return TX::inf(); }' )
    print( '    if ( fp<T>::is_zero( a ) ) { flag = true; auto c=a; return TX{ '+ARG+' }; }' )
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

    print( 'template < typename TXa, T_mX(TXa) >' )
    print( 'INLINE auto constexpr operator_{func} ( TXa const& a ) NOEXCEPT {{'.format( func=func ) )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  auto flag = false;' )
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

    print( 'template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >' )
    print( 'INLINE auto constexpr operator_{func} ( T const& a ) NOEXCEPT {{'.format( func=func ) )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  auto flag = false;' )
    print( '  auto e = {m}X_real::operator_{func}_exception<A> ( a, flag );'.format( m=mX_type(Tc), func=func ) )
    print( '  if ( flag ) { return e; }' )
    print( '#else' )
    print( '  if ( fp<T>::is_zero( a ) ) {{ return {m}X_real::{m}x_real<T,A>{{ a }}; }}'.format( m=mX_type(Tc) ) )
    print( '#endif' )
    print( '  return {m}X_real::operator_{func}_body<A> ( a );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )


# ************************************************************************************************
#
# ************************************************************************************************
def gen_op_operator( Tc, description, func, op, commutable ) :

    print( 'template < typename TXa, T_mX(TXa) >' )
    print( 'INLINE auto constexpr {op} ( TXa const& a ) NOEXCEPT {{'.format( op=op ) )
    print( '  return {m}X_real::operator_{func} ( a );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )

    print( 'template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >' )
    print( 'INLINE auto constexpr {op} ( T const& a ) NOEXCEPT {{'.format( op=op ) )
    print( '  return {m}X_real::operator_{func}<A> ( a );'.format( m=mX_type(Tc), func=func ) )
    print( '}' )


# ************************************************************************************************
#
# ************************************************************************************************
def gen_op_member( Tc, description, func, op, commutable, flag ) :

    for Tcx in { 2,3,4 } :
        if ( flag == True ) or ( Tcx == Tc ) :
            print( 'template < typename T, Algorithm A >' )
            print( 'INLINE {m}X_real::{m}x_real<T,A> constexpr {m}X_real::{m}x_real<T,A>::{op} ( {mm}X_real::{mm}x_real<T,A> const& a ) NOEXCEPT {{'.format( m=mX_type(Tc), mm=mX_type(Tcx), op=op ) )
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
    print( 'template < typename TXa, T_mX(TXa) >' )
    print( 'INLINE auto constexpr operator_abs_body ( TXa const& a ) NOEXCEPT {' )
    print( '  return {m}X_real::mX_real<TXa>{{ is_negative( a ) ? -a : a }};'.format( m=mX_type(Tc) ) )
    print( '}' )
    print( '//' )
    gen_exception_abs( Tc, description, func, op, commutable )
    print( '//' )
    print( 'template < typename TXa, T_mX(TXa) >' )
    print( 'INLINE auto constexpr operator_abs ( TXa const& a ) NOEXCEPT {' )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  auto flag = false;' )
    print( '  auto e = {m}X_real::operator_abs_exception ( a, flag );'.format( m=mX_type(Tc) ) )
    print( '  if ( flag ) { return e; }' )
    print( '#endif' )
    print( '  return {m}X_real::operator_abs_body ( a );'.format( m=mX_type(Tc) ) )
    print( '}' )
    print( 'template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >' )
    print( 'INLINE auto constexpr operator_abs ( T const& a ) NOEXCEPT {' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  return TX{ std::abs( a ) };' )
    print( '}' )
    print( '//' )
    print( 'template < typename TXa, T_mX(TXa) >' )
    print( 'INLINE auto constexpr abs ( TXa const& a ) NOEXCEPT {' )
    print( '  return {m}X_real::operator_abs ( a );'.format( m=mX_type(Tc) ) )
    print( '}' )
    print( 'template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >' )
    print( 'INLINE auto constexpr abs ( T const& a ) NOEXCEPT {' )
    print( '  return {m}X_real::operator_abs<A> ( a );'.format( m=mX_type(Tc) ) )
    print( '}' )
    print( 'template < typename TXa, T_mX(TXa) >' )
    print( 'INLINE auto constexpr fabs ( TXa const& a ) NOEXCEPT {' )
    print( '  return {m}X_real::operator_abs ( a );'.format( m=mX_type(Tc) ) )
    print( '}' )
    print( 'template < Algorithm A=Algorithm::Accurate, typename T, T_fp(T) >' )
    print( 'INLINE auto constexpr fabs ( T const& a ) NOEXCEPT {' )
    print( '  return {m}X_real::operator_abs<A> ( a );'.format( m=mX_type(Tc) ) )
    print( '}' )
    print( '//' )
    gen_op_member( Tc, description, func, op, commutable, True )
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
    gen_op_member( Tc, description, func, op, commutable, True )
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
    print( 'template < typename T, Algorithm Aa, Algorithm Ab, A_noQuasi(Aa), A_noQuasi(Ab) >' )
    print( 'INLINE auto constexpr operator_fmin_body ( {m}X_real::{m}x_real<T,Aa> const& a, {m}X_real::{m}x_real<T,Ab> const &b ) NOEXCEPT {{'.format( m=mX_type(Tc) ) )
    print( '  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  auto i=0; for(i=0;i<TX::L-1;i++) { if ( a.x[i] != b.x[i] ) break; }' )
    print( '  return TX{ ( a.x[i] <= b.x[i] ) ? a : b };' )
    print( '}' )
    print( '//' )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( 'template < typename T, Algorithm Aa, Algorithm Ab >' )
    print( 'INLINE auto constexpr operator_fmin_exception ( {m}X_real::{m}x_real<T,Aa> const& a, {m}X_real::{m}x_real<T,Ab> const &b, bool & flag ) NOEXCEPT {{'.format( m=mX_type(Tc) ) )
    print( '  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  {' )
    print( '    if ( isnan( a ) || isnan( b ) ) { flag = true; return TX::nan(); }' )
    print( '    if ( isinf( a ) && isinf( b ) ) {' )
    print( '      flag = true; return ( is_positive( a ) && is_positive( b ) ) ? a : -TX::inf();' )
    print( '    }' )
    print( '    if ( isinf( a ) ) { flag = true; return is_negative( a ) ? a : b; }' )
    print( '    if ( isinf( b ) ) { flag = true; return is_negative( b ) ? b : a; }' )
    print( '  }' )
    print( '  //' )
    print( '  flag = false; return TX::zero();' )
    print( '}' )
    print( '#endif' )
    print( '//' )
    print( 'template < typename T, Algorithm Aa, Algorithm Ab >' )
    print( 'INLINE auto constexpr operator_fmin ( {m}X_real::{m}x_real<T,Aa> const& a, {m}X_real::{m}x_real<T,Ab> const &b ) NOEXCEPT {{'.format( m=mX_type(Tc) ) )
    print( '  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  auto flag = false;' )
    print( '  auto e = {m}X_real::operator_fmin_exception( a, b, flag );'.format( m=mX_type(Tc) ) )
    print( '  if ( flag ) { return e; }' )
    print( '#endif' )
    print( '  if ( A == Algorithm::Quasi ) {' )
    print( '    using TT = typename TX::type_Accurate;' )
    print( '    return TX{{ {m}X_real::operator_fmin_body( TT{{ a }}, TT{{ b }} ) }};'.format( m=mX_type(Tc) ) )
    print( '  } else {' )
    print( '    return {m}X_real::operator_fmin_body( a, b );'.format( m=mX_type(Tc) ) )
    print( '  }' )
    print( '}' )
    print( '//' )
    print( 'template < typename T, Algorithm Aa, Algorithm Ab >' )
    print( 'INLINE auto constexpr fmin ( {m}X_real::{m}x_real<T,Aa> const& a, {m}X_real::{m}x_real<T,Ab> const &b ) NOEXCEPT {{'.format( m=mX_type(Tc) ) )
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
    print( 'template < typename T, Algorithm Aa, Algorithm Ab, A_noQuasi(Aa), A_noQuasi(Ab) >' )
    print( 'INLINE auto constexpr operator_fmax_body ( {m}X_real::{m}x_real<T,Aa> const& a, {m}X_real::{m}x_real<T,Ab> const &b ) NOEXCEPT {{'.format( m=mX_type(Tc) ) )
    print( '  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  auto i=0; for(i=0;i<TX::L-1;i++) { if ( a.x[i] != b.x[i] ) break; }' )
    print( '  return TX{ ( a.x[i] >= b.x[i] ) ? a : b };' )
    print( '}' )
    print( '//' )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( 'template < typename T, Algorithm Aa, Algorithm Ab >' )
    print( 'INLINE auto constexpr operator_fmax_exception ( {m}X_real::{m}x_real<T,Aa> const& a, {m}X_real::{m}x_real<T,Ab> const &b, bool & flag ) NOEXCEPT {{'.format( m=mX_type(Tc) ) )
    print( '  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  {' )
    print( '    if ( isnan( a ) || isnan( b ) ) { flag = true; return TX::nan(); }' )
    print( '    if ( isinf( a ) && isinf( b ) ) {' )
    print( '      flag = true; return ( is_negative( a ) && is_negative( b ) ) ? a : TX::inf();' )
    print( '    }' )
    print( '    if ( isinf( a ) ) { flag = true; return is_positive( a ) ? a : b; }' )
    print( '    if ( isinf( b ) ) { flag = true; return is_positive( b ) ? b : a; }' )
    print( '  }' )
    print( '  //' )
    print( '  flag = false; return TX::zero();' )
    print( '}' )
    print( '#endif' )
    print( '//' )
    print( 'template < typename T, Algorithm Aa, Algorithm Ab >' )
    print( 'INLINE auto constexpr operator_fmax ( {m}X_real::{m}x_real<T,Aa> const& a, {m}X_real::{m}x_real<T,Ab> const &b ) NOEXCEPT {{'.format( m=mX_type(Tc) ) )
    print( '  Algorithm constexpr A = commonAlgorithm<Aa,Ab>::algorithm;' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '#if MX_REAL_USE_INF_NAN_EXCEPTION' )
    print( '  auto flag = false;' )
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
    print( 'INLINE auto constexpr fmax ( {m}X_real::{m}x_real<T,Aa> const& a, {m}X_real::{m}x_real<T,Ab> const &b ) NOEXCEPT {{'.format( m=mX_type(Tc) ) )
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
    print( 'template < typename T, Algorithm A, T_fp(T) >' )
    print( 'INLINE auto rand () NOEXCEPT {' )
    print( '  using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc) ) )
    print( '  auto const f = fp<T>::nhalf() / (1 << 30);' )
    print( '  auto g = f;' )
    print( '  auto r = TX::zero();' )
    print( '  auto const bits = std::numeric_limits<TX>::digits;' )
    print( '  auto const rand_bits = std::numeric_limits<int>::digits - 1;' )
    print( '  auto const b_for_sign = std::rand();' )
    print( '  for(auto i=0; i<bits; i+=rand_bits ) {' )
    print( '    auto b_ = std::rand() & 0x7fffffff;' )
    print( '    T b, c;' )
    print( '    if ( std::is_same<T,float>::value ) {' )
    print( '         b = T( ( b_ & 0x7fff0000 ) >> 16 ) * (1 << 16);' )
    print( '    } else {' )
    print( '         b = T( b_ );' )
    print( '    }' )
    print( '    if ( std::is_same<T,float>::value ) {' )
    print( '         c = b * g;' )
    print( '         r = r + TX{ c };' )
    print( '         b = T( b_ & 0x0000ffff );' )
    print( '    }' )
    print( '         c = b * g;' )
    print( '         r = r + TX{ c };' )
    print( '         g = g * f;' )
    print( '  }' )
    print( '  {' )
    print( '    auto b_ = b_for_sign;' )
    print( '    for(auto i=1; i<TX::L; i++ ) {' )
    print( '      auto s = T( ( 0x1 & b_ ) ? 1 : -1 );' )
    print( '      r.x[i] *= s;' )
    print( '      b_ >>= 1;' )
    print( '    }' )
    print( '  }' )
    print( '  return r;' )
    print( '}' )
    print( 'template < typename T, Algorithm A >' )
    print( 'INLINE {m}X_real::{m}x_real<T,A> {m}X_real::{m}x_real<T,A>::rand () NOEXCEPT {{'.format( m=mX_type(Tc) ) )
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
        print( '  INLINE auto constexpr {func} ( {m}X_real::{m}x_real<T,Aa> const& a ) NOEXCEPT {{'.format( m=mX_type(Tc), func=func ) )
        print( '    return fp<T>::{func}( a.quick_Normalized() );'.format( func=func ) )
        print( '  }' )
    for item in { 'is_zero ==', 'is_positive >', 'is_negative <' } :
        a_list = item.split()
        func = a_list[0]
        op = a_list[1]
        print( '  template < typename T, Algorithm Aa >' )
        print( '  INLINE bool constexpr {func} ( {m}X_real::{m}x_real<T,Aa> const& a ) NOEXCEPT {{'.format( m=mX_type(Tc), func=func ) )
        print( '    return a.quick_Normalized() {op} fp<T>::zero();'.format( op=op ) )
        print( '  }' )
    print( '//' )
    for func in [ 'isinf', 'isnan', 'signbit', 'is_zero', 'is_positive', 'is_negative' ] :
        print( ' template < typename T, Algorithm A >' )
        print( ' INLINE bool constexpr {m}X_real::{m}x_real<T,A>::{func} ( {m}X_real::{m}x_real<T,A> const& a ) NOEXCEPT {{'.format( m=mX_type(Tc), func=func ) )
        print( ' return {m}X_real::{func} ( a );'.format( m=mX_type(Tc), func=func ) )
        print( ' }' )
    print( '//' )
    print( '' )


def gen_members ( Tc ) :

    print( '//' )
    print( '// static member funtions' )
    print( '// their definitions are below outside of the struct definition block' )
    print( '//' )
    for func in { 'abs', 'sqrt' } :
        for Tcx in { 2,3,4 } :
            print( 'static INLINE {m}X_REAL<> constexpr {func} ( {mm}X_REAL<> const& a ) NOEXCEPT;'.format( m=mX_Type(Tc), mm=mX_Type(Tcx), func=func ) )

    for Tcx in { 2,3,4 } :
        print( 'static INLINE {m}X_REAL<> constexpr {func} ( {mm}X_REAL<> const& a ) NOEXCEPT {{'.format( m=mX_Type(Tc), mm=mX_Type(Tcx), func='fabs' ) )
        print( '  return {m}X_REAL<>::abs(a);'.format( m=mX_Type(Tc) ) )
        print( '}' )

    print( 'static INLINE {m}X_REAL<> rand () NOEXCEPT;'.format( m=mX_Type(Tc) ) )
    for func in { 'signbit', 'isinf', 'isnan', 'is_zero', 'is_positive', 'is_negative' } :
        print( 'static INLINE bool constexpr {func} ( {m}X_REAL<> const& a ) NOEXCEPT;'.format( m=mX_Type(Tc), func=func ) )
    print( '//' )
    print( '' )
    print( '' )
    print( '//' )
    print( '// member functions (aka methods)' )
    print( '//' )
    print( '//' )
    print( 'INLINE void constexpr Normalize () NOEXCEPT { mX_real::Normalize( *this ); }' )
    print( '//' )
    for func in { 'signbit', 'isinf', 'isnan', 'is_zero', 'is_positive', 'is_negative' } :
        print( 'INLINE bool constexpr {func} () const NOEXCEPT {{ return {m}X_REAL<>::{func}( *this ); }}'.format( m=mX_Type(Tc), func=func ) )
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


