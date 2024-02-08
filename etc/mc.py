import sys

def mX_type ( width ) :
    return [ '_', 's', 'd', 't', 'q' ][width]

def gen_comparison_op( description, op, not_op, func, Tc ) :

    print( '    //' )
    print( '    // {description}'.format( description=description ) )
    print( '    //' )
    print( '    //' )

    print( '    template < typename T, Algorithm A >' )
    print( '    INLINE auto constexpr operator_{func} ( {m}X_real::{m}x_real<T,A> const& a, {m}X_real::{m}x_real<T,A> const& b ) {{'.format( m=mX_type(Tc), func=func ) )
    print( '      using TX = {m}X_real::{m}x_real<T,A>;'.format( m=mX_type(Tc), func=func ) )
    print( '      int i=0; for(i=0; i<TX::L-1; i++) {' )
    print( '        if ( a.x[i] != b.x[i] ) {{ return a.x[i] {op} b.x[i]; }}'.format( op=op ) )
    print( '      }} return a.x[i] {op} b.x[i];'.format( op=op ) )
    print( '    }' )
    print( '    template < typename T, Algorithm Aa, Algorithm Ab >' )
    print( '    INLINE auto constexpr operator{op} ( {m}X_real::{m}x_real<T,Aa> const& a, {m}X_real::{m}x_real<T,Ab> const& b ) {{'.format( m=mX_type(Tc), op=op ) )
    print( '      Algorithm constexpr A=commonAlgorithm<Aa,Ab>::algorithm;' )
    print( '      if ( A == Algorithm::Quasi ) {' )
    print( '        using TT = {m}X_real::{m}x_real<T,Algorithm::Accurate>;'.format( m=mX_type(Tc) ) )
    print( '        return {m}X_real::operator_{func}( TT{{ a }}, TT{{ b }} );'.format( func=func, m=mX_type(Tc) ) )
    print( '      } else {' )
    print( '        return {m}X_real::operator_{func}( a, b );'.format( func=func, m=mX_type(Tc) ) )
    print( '      }' )
    print( '    }' )
    print( '    template < typename T, Algorithm Aa, Algorithm Ab >' )
    print( '    INLINE auto constexpr operator{not_op} ( {m}X_real::{m}x_real<T,Aa> const& a, {m}X_real::{m}x_real<T,Ab> const& b ) {{'.format( not_op=not_op, m=mX_type(Tc) ) )
    print( '      return !(a {op} b);'.format( op=op ) )
    print( '    }' )
    print( '' )

def gen_comp( Tc ) :

    description='Comparison, equivalence (==)'
    op='=='
    not_op='!='
    func='eq'
    #Tc=3
    gen_comparison_op( description, op, not_op, func, Tc )

    description='Comparison, greater than (>)'
    op='>'
    not_op='<='
    func='gt'
    #Tc=3
    gen_comparison_op( description, op, not_op, func, Tc )

    description='Comparison, less than (<)'
    op='<'
    not_op='>='
    func='lt'
    #Tc=3
    gen_comparison_op( description, op, not_op, func, Tc )

if __name__ == '__main__' :

    args = sys.argv
    if len(args) != 2 :
        exit( 1 )

    Tc = int(args[1])
    if Tc < 2 or Tc > 5 :
        exit( 0 )

    gen_comp( Tc )

