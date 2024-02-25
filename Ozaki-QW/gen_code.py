def init () :

    header = '''#ifndef QXW_H
#define QXW_H


//
// C++-fied version 2023.12.26 (refered to Ozaki's code)
//
// This C++ code was automatically generated
// referering to the idea by Katsuhisa Ozaki (Shibaura Institute of Technology)
// Debug is not sufficiently done, so please use it carefully and
//
// by Toshiyuki Imamura (RIKEN, R-CCS)
//


#include <stdio.h>
#include <math.h>


namespace QxW {

// ------------------------
// Constants
// ------------------------

template < typename T > struct fp_const {};
template <> struct fp_const<float> {
  static INLINE auto constexpr zero() NOEXCEPT { return 0.0f; }
  static INLINE auto constexpr one()  NOEXCEPT { return 1.0f; }
  static INLINE auto constexpr two()  NOEXCEPT { return 2.0f; }
  static INLINE auto constexpr half() NOEXCEPT { return 0.5f; }

  static uint32_t constexpr SBIT = 0x80000000;
  static uint32_t constexpr MASK = 0x7f800000;
  static uint32_t constexpr RINF = 0x7f000000;
  static uint32_t constexpr XONE = 0x0b800000;
  static uint32_t constexpr EONE = 0x00800000;
  static uint32_t constexpr FRAC = 0x007fffff;

  static INLINE auto constexpr fp2uint( float const a ) NOEXCEPT {
    union { float a; uint32_t e; } x = { .a = a };
    return x.e;
  }
  static INLINE auto constexpr uint2fp( uint32_t const e ) NOEXCEPT {
    union { float a; uint32_t e; } x = { .e = e };
    return x.a;
  }

  template < bool doubled = false >
  static INLINE auto constexpr hbit( float const a ) NOEXCEPT {
    auto e = fp2uint( a );
    auto s = e & SBIT;
    e &= MASK;
    if ( doubled ) e += EONE;
    e |= s;
    return uint2fp( e );
  }
  static INLINE auto constexpr hbit2( float const a ) NOEXCEPT {
    return hbit<true>( a );
  }
  static INLINE auto constexpr ulp( float const a ) NOEXCEPT {
    if ( a == zero() ) return a;
    auto e = fp2uint( a );
    auto s = e & SBIT;
    e &= MASK;
    e -= XONE;
    if ( e & SBIT ) { e = 0; } // underflow
    e = e | s; // allow -0
    return uint2fp( e );
  }
  template < bool inverse = false >
  static INLINE auto constexpr exponent( float const a ) NOEXCEPT {
    if ( a == zero() ) return one();
    auto e = fp2uint( a );
    e &= MASK;
    if ( e == MASK ) return one();
    if ( inverse ) e = RINF - e;
    return uint2fp( e );
  }
  static INLINE auto constexpr exponenti( float const a ) NOEXCEPT {
    return exponent<true>( a );
  }
};
template <> struct fp_const<double> {
  static INLINE auto constexpr zero() NOEXCEPT { return 0.0; }
  static INLINE auto constexpr one()  NOEXCEPT { return 1.0; }
  static INLINE auto constexpr two()  NOEXCEPT { return 2.0; }
  static INLINE auto constexpr half() NOEXCEPT { return 0.5; }

  static uint64_t constexpr SBIT = 0x8000000000000000;
  static uint64_t constexpr MASK = 0x7ff0000000000000;
  static uint64_t constexpr RINF = 0x7fe0000000000000;
  static uint64_t constexpr XONE = 0x0340000000000000;
  static uint64_t constexpr EONE = 0x0010000000000000;
  static uint64_t constexpr FRAC = 0x000fffffffffffff;

  static INLINE auto constexpr fp2uint( double const a ) NOEXCEPT {
    union { double a; uint64_t e; } x = { .a = a };
    return x.e;
  }
  static INLINE auto constexpr uint2fp( uint64_t const e ) NOEXCEPT {
    union { double a; uint64_t e; } x = { .e = e };
    return x.a;
  }

  template < bool doubled = false >
  static INLINE auto constexpr hbit( double const a ) NOEXCEPT {
    auto e = fp2uint( a );
    auto s = e & SBIT;
    e &= MASK;
    if ( doubled ) e += EONE;
    e |= s;
    return uint2fp( e );
  }
  static INLINE auto constexpr hbit2( double const a ) NOEXCEPT {
    return hbit<true>( a );
  }
  static INLINE auto constexpr ulp( double const a ) NOEXCEPT {
    if ( a == zero() ) return a;
    auto e = fp2uint( a );
    auto s = e & SBIT;
    e &= MASK;
    e -= XONE;
    if ( e & SBIT ) { e = 0; } // underflow
    e = e | s; // allow -0
    return uint2fp( e );
  }
  template < bool inverse = false >
  static INLINE auto constexpr exponent( double const a ) NOEXCEPT {
    if ( a == zero() ) return one();
    auto e = fp2uint( a );
    e &= MASK;
    if ( e == MASK ) return one();
    if ( inverse ) e = RINF - e;
    return uint2fp( e );
  }
  static INLINE auto constexpr exponenti( double const a ) NOEXCEPT {
    return exponent<true>( a );
  }
};


// ------------------------
// Basic EFT Part
// ------------------------

template < typename T > INLINE void constexpr
TwoSum ( T const a, T const b, T &x, T &y ) NOEXCEPT
{
  T z;
  x = a + b;
  z = x - a;
  y = (a - (x - z)) + (b - z);
}

template < typename T > INLINE void constexpr
FastTwoSum ( T const a, T const b, T &x, T &y ) NOEXCEPT
{
  x = a + b;
  y = (a - x) + b;
}

template < typename T > INLINE void constexpr
TwoProductFMA ( T const a, T const b, T &x, T &y ) NOEXCEPT
{
  x = a * b;
  y = std::fma(a, b, -x);
}
'''
    print( header )

def mX_suffics ( ACC, width ) :
    if ACC > 0 :
        return [ '_', '_SW', '_DW', '_TW', '_QW' ][width]
    else :
        return [ '_', '_SW', '_PA', '_QTW', '_QQW' ][width]

def fname ( Func, NA, NB, NC, ACC ) :
    print( '{}{}{}{}'.format(
        Func, mX_suffics( ACC, NA ), mX_suffics( ACC, NB ) if NB > 0 else '', mX_suffics( ACC, NC ) ), end='' )

def def_head ( Func, NA, NB, NC, ACC, va, vb, vc ) :
    if NB > 0 :
        print( '// {}: {}-{}-{}'.format( Func, NA, NB, NC ) )
    else :
        print( '// {}: {}-{}'.format( Func, NA, NC ) )
    print( 'template < typename T > INLINE void constexpr' )
    fname( Func, NA, NB, NC, ACC )
    va_list = ','.join( [ ' T const {}{}'.format( va, i ) for i in range( NA ) ] )
    vb_list = ','.join( [ ' T const {}{}'.format( vb, i ) for i in range( NB ) ] )
    vc_list = ','.join( [ ' T &{}{}'.format( vc, i ) for i in range( NC ) ] )
    if NB > 0 :
        print( '({},{},{} ) NOEXCEPT'.format( va_list, vb_list, vc_list ) )
    else :
        print( '({},{} ) NOEXCEPT'.format( va_list, vc_list ) )

def caller_head ( Func, NA, NB, NC, ACC, va, vb, vc ) :
    fname( Func, NA, NB, NC, ACC )
    va_list = ','.join( [ ' {}'.format( va[i] ) for i in range( NA ) ] )
    vb_list = ','.join( [ ' {}'.format( vb[i] ) for i in range( NB ) ] )
    vc_list = ','.join( [ ' {}'.format( vc[i] ) for i in range( NC ) ] )
    if NB > 0 :
        print( '({},{},{} );'.format( va_list, vb_list, vc_list ) )
    else :
        print( '({},{} );'.format( va_list, vc_list ) )

def insert ( line, pos, element ) :
    L = len( line )
    if pos < L :
        for i in reversed( range( pos, L ) ) :
            line[i+1] = line[i]
    else :
        for i in range( L, pos ) :
            line[i] = '!'
    line[pos] = element

def delete ( line, pos ) :
    L = len( line )
    if pos < L :
        del line[pos]
    return line

def def_list ( line ) :
    if '!' in line :
        return {}
    a_list = line.split()
    if a_list[0] == 'FMA' or a_list[0] == 'TWO' or a_list[0] == 'QQQ' :
        del a_list[0]
        del a_list[0]
        del a_list[0]
        return a_list
    if a_list[0] == 'ZER' :
        del a_list[0]
        return a_list
    if a_list[0] == 'LET' :
        del a_list[0]
        del a_list[1]
        return a_list
    if a_list[0] == 'ADD' or a_list[0] == 'MUL' :
        del a_list[0]
        del a_list[1]
        del a_list[1]
        return a_list
    if a_list[0] == 'MAD' :
        del a_list[0]
        del a_list[1]
        del a_list[1]
        del a_list[1]
        return a_list
    if a_list[0] == 'SUM' :
        b_list = {}
        b_list[0] = a_list[2]
        return b_list
    if a_list[0] == 'IN' :
        b_list = {}
        b_list[0] = a_list[1]
        return b_list
    return {}

def use_list ( line ) :
    if '!' in line :
        return {}
    a_list = line.split()
    if a_list[0] == 'FMA' or a_list[0] == 'TWO' or a_list[0] == 'QQQ' :
        del a_list[0]
        del a_list[2]
        del a_list[2]
        return a_list
    if a_list[0] == 'LET' or a_list[0] == 'ADD' or a_list[0] == 'MUL' or a_list[0] == 'MAD' :
        del a_list[0]
        del a_list[0]
        return a_list
    if a_list[0] == 'MAD' :
        del a_list[0]
        del a_list[0]
        return a_list
    if a_list[0] == 'SUM' :
        del a_list[0]
        del a_list[0]
        del a_list[0]
        return a_list
    if a_list[0] == 'OUT' or a_list[0] == 'OUTZ' :
        b_list = {}
        b_list[0] = a_list[1]
        return b_list
    return {}


def normalize ( line ) :
    linex = {}
    nx = 0
    for i in range( len( line ) ) :
        if '!' in line[i] :
            continue
        a_list = line[i].split()
        if a_list[0] == 'LET' :
            if a_list[1] == a_list[2] :
                continue
        if a_list[0] == 'SUM' :
            if int( a_list[1] ) == 0 :
                continue
            if int( a_list[1] ) == 1 :
                if a_list[2] == a_list[3] :
                    continue
                line[i] = 'LET {} {}'.format( a_list[2], a_list[3] )
            if int( a_list[1] ) == 2 :
                line[i] = 'ADD {} {} {}'.format( a_list[2], a_list[3], a_list[4] )
        linex[nx] = line[i]
        nx = nx + 1
    line = {}
    line = linex
    return line


def concat ( list, element ) :
    return '{} {}'.format( list, element )


def INIT_propagate( line, NA, NB, NC ) :

    for i in range( 4 ) :
        insert( line, 0, 'ZER c{}'.format( i ) )
    for i in range( NB, 4 ) :
        insert( line, 0, 'ZER b{}'.format( i ) )
    for i in range( NA, 4 ) :
        insert( line, 0, 'ZER a{}'.format( i ) )

    for i in range( NB ) :
        insert( line, 0, 'IN b{}'.format( i ) )
    for i in range( NA ) :
        insert( line, 0, 'IN a{}'.format( i ) )
    for i in range( NC ) :
        insert( line, len(line), 'OUT c{}'.format( i ) )

    return line


def ZERO_propagate( line ) :

    #print( 'REMOVE ZERO chain' )
    while True :
        line = normalize( line )
        #print( line )

        zero = ''
        for j in range( len(line) ) :
            if '!' in line[j] :
                continue
            a_list = line[j].split()
            if a_list[0] == 'ZER' :
                zero = a_list[1]
                line[j] = '!'
                k = j
                break
        if zero == '' :
            break

        #print( 'ZERO={}'.format( zero ) )
        #print( line )
            
        # trace and fill zero
        j = k
        while True :
            j = j + 1
            if j >= len( line ) :
                break

            if '!' in line[j] :
                continue

            #print( '{}: {}'.format(j,line[j]) )
            line_pre = line[j]
            a_list = line[j].split()

            if a_list[0] == 'IN' :
                continue
            if a_list[0] == 'OUT' or a_list[0] == 'OUTZ':
                if a_list[1] == zero :
                    line[j] = 'OUTZ {}'.format( a_list[1] )
                continue

            if a_list[0] == 'ZER' :
                if a_list[1] == zero :
                    break
                continue

            if a_list[0] == 'LET' :
                if a_list[2] == zero :
                    line[j] = 'ZER {}'.format( a_list[1] )
                    #print( 'LET {} => {}'.format( line_pre, line[j] ) )
                if a_list[1] == zero :
                    #print( "LET: next ZERO" )
                    break
                continue

            if a_list[0] == 'FMA' :
                if a_list[1] == zero or a_list[2] == zero :
                    line[j] = 'ZER {}'.format( a_list[3] )
                    insert( line, j+1, 'ZER {}'.format( a_list[4] ) )
                    #print( 'FMA {} => {} / {}'.format( line_pre, line[j], line[j+1] ) )
                    j = j + 1
                if a_list[3] == zero or a_list[4] == zero :
                    #print( "FMA: next ZERO" )
                    break
                continue

            if a_list[0] == 'TWO' or a_list[0] == 'QQQ' :

                if a_list[1] == zero and a_list[2] == zero :
                    line[j] = 'ZER {}'.format( a_list[3] )
                    insert( line, j+1, 'ZER {}'.format( a_list[4] ) )
                    #print( 'TWO/QQQ type1 {} => {} / {}'.format( line_pre, line[j], line[j+1] ) )
                    j = j + 1
                if a_list[1] == zero and a_list[2] != zero :
                    line[j] = 'LET {} {}'.format( a_list[3], a_list[2] )
                    insert( line, j+1, 'ZER {}'.format( a_list[4] ) )
                    #print( 'TWO/QQQ type2 {} => {} / {}'.format( line_pre, line[j], line[j+1] ) )
                    j = j + 1
                if a_list[2] == zero and a_list[1] != zero :
                    line[j] = 'ZER {}'.format( a_list[4] )
                    if a_list[3] != a_list[1] :
                        insert( line, j, 'LET {} {}'.format( a_list[3], a_list[1] ) )
                        #print( 'TWO/QQQ type3 {} => {} / {}'.format( line_pre, line[j], line[j+1] ) )
                        j = j + 1
                    #else :
                    #    print( 'TWO/QQQ type4 {} => {}'.format( line_pre, line[j] ) )
                if a_list[3] == zero or a_list[4] == zero :
                    #print( "TWO: next ZERO" )
                    break
                continue

            if a_list[0] == 'ADD' :
                if a_list[2] == zero and a_list[3] == zero :
                    line[j] = 'ZER {}'.format( a_list[1] )
                    #print( 'ADD1 {} => {}'.format( line_pre, line[j] ) )
                if a_list[2] == zero and a_list[3] != zero :
                    line[j] = 'LET {} {}'.format( a_list[1], a_list[3] )
                    #print( 'ADD2 {} => {}'.format( line_pre, line[j] ) )
                if a_list[3] == zero and a_list[2] != zero :
                    line[j] = 'LET {} {}'.format( a_list[1], a_list[2] )
                    #print( 'ADD3 {} => {}'.format( line_pre, line[j] ) )
                if a_list[1] == zero :
                    #print( "ADD: next ZERO" )
                    break
                continue

            if a_list[0] == 'MUL' :
                if a_list[2] == zero or a_list[3] == zero :
                    line[j] = 'ZER {}'.format( a_list[1] )
                    #print( 'MUL {} => {}'.format( line_pre, line[j] ) )
                if a_list[1] == zero :
                    #print( "MUL: next ZERO" )
                    break
                continue

            if a_list[0] == 'MAD' :
                if ( a_list[2] == zero or a_list[3] == zero ) and (a_list[4] != zero ) :
                    line[j] = 'LET {} {}'.format( a_list[1], a_list[4] )
                    #print( 'MAD1 {} => {}'.format( line_pre, line[j] ) )
                if a_list[4] == zero :
                    line[j] = 'MUL {} {} {}'.format( a_list[1], a_list[2], a_list[3] )
                    #print( 'MAD2 {} => {}'.format( line_pre, line[j] ) )
                if a_list[1] == zero :
                    #print( "MAD: next ZERO" )
                    break
                continue

            if a_list[0] == 'SUM' :
                va = ''
                na = 0
                for k in range( 3, len(a_list) ) :
                    if a_list[k] != zero :
                        va = concat( va, a_list[k] )
                        na = na + 1
                if na < int(a_list[1]) :
                    if na > 1 :
                        line[j] = 'SUM {} {} {}'.format( na, a_list[2], va )
                    if na == 1 :
                        line[j] = 'LET {} {}'.format( a_list[2], va )
                    if na == 0 :
                        line[j] = '!'
                    #print( 'SUM {} => {}'.format( line_pre, line[j] ) )
                if a_list[2] == zero :
                    #print( "SUM: next ZERO" )
                    break
                continue

    #print( line )
    return line

def is_effective( param, line, idx ) :

    done_ref = False
    for i in range( idx, len( line ) ) :
        if not ( param in line[i] ) :
            continue

        c_list = use_list( line[i] )
        for j in range( len( c_list ) ) :
            if c_list[j] == param :
                done_ref = True
                break
        if done_ref :
            # A is used
            break

        done_def = False
        c_list = def_list( line[i] )
        for j in range( len( c_list ) ) :
            if c_list[j] == param :
                done_def = True
                break
        if done_def :
            # A is not used before re-definition
            break

    return done_ref


def LET_propagate( line ) :

    #print( 'REMOVE LET chain' )
    #print( line )
    itr = 0
    while True :
        line = normalize( line )
        #print( line )

        stable = True
        for i in range( len( line ) ) :
            if '!' in line[i] :
                continue
            a_list = line[i].split()
            if a_list[0] != 'LET' :
                continue
            if a_list[1] == a_list[2] :
                line[i] = '!'
                continue


            #print( 'LET ::: [{}] => [{}]'.format( a_list[1], a_list[2] ) )
            #print( line )


            done_use = False
            done_def = False
            for j in range( i+1, len( line ) ) :
                if '!' in line[j] :
                    continue
                b_list = use_list( line[j] )
                for k in range( len(b_list) ) :
                    if a_list[1] in b_list[k] :
                        done_use = True
                if done_use :
                    break
                b_list = def_list( line[j] )
                for k in range( len(b_list) ) :
                    if a_list[1] in b_list[k] :
                        done_def = True
                if done_def :
                    break
            if ( not done_use ) or ( done_def ) :
                #if ( not done_use ) :
                #    print( 'var {} is not used'.format( a_list[1] ) )
                #if ( done_def ) :
                #    print( 'var {} is defined later'.format( a_list[1] ) )
                if 'c' in a_list[1] :
                    stable = True
                else :
                    stable = False
                    #print( 'LET is commented out {}'.format( line[i] ) )
                    line[i] = '!'
                continue


            last_def = -1
            for j in range( i-1 ) :
                if '!' in line[j] :
                    continue
                b_list = line[j].split()
                for k in range( 1, len(b_list) ) :
                    if a_list[1] in b_list[k] :
                        last_def = i



            done = False
            for j in range( i+1, len( line ) ) :
                if '!' in line[j] :
                    continue
                b_list = line[j].split()
                #print( 'check replacement :: {} --> {}'.format( line[i], line[j] ) )

                if b_list[0] == 'IN' :
                    continue
                if b_list[0] == 'OUT' or b_list[0] == 'OUTZ' :
                    continue

                if b_list[0] == 'LET' :
                    if b_list[1] == b_list[2] :
                        line[j] = '!'
                        continue

                    # LET A B
                    # LET X Y
                    if b_list[1] != a_list[1] and b_list[1] != a_list[2] and b_list[2] != a_list[1] and b_list[2] != a_list[2] :
                        continue

                    # LET A B --> !
                    # LET A Y --> LET A Y
                    if b_list[1] == a_list[1] :
                        done = True
                        break

                    # LET A B --> !
                    # LET B A --> LET A B
                    if b_list[1] == a_list[2] and b_list[2] == a_list[1] :
                        line[j] = 'LET {} {}'.format( a_list[1], a_list[2] )
                        #print( '>>>>{}'.format( line[j] ) )
                        done = True
                        break

                    # LET A B --> !
                    # LET X A --> LET X B
                    #         --> LET A X
                    if b_list[1] != a_list[1] and b_list[2] == a_list[1] :
                        line[j] = 'LET {} {}'.format( b_list[1], a_list[2] )

                        if is_effective( a_list[1], line, j+1 ) :
                            insert( line, j+1, 'LET {} {}'.format( a_list[1], b_list[1] ) )
                            #print( '>>>>{}'.format( line[j] ) )
                            #print( '>>>>{}'.format( line[j+1] ) )
                        done = True
                        break

                    # LET A B --> !
                    # LET B Y --> ( LET A B )  or '!'
                    #         --> LET B Y
                    if b_list[1] == a_list[2] :

                        if is_effective( a_list[1], line, j+1 ) :
                            insert( line, j, 'LET {} {}'.format( a_list[1], a_list[2] ) )
                            #print( '>>>>{}'.format( line[j] ) )
                            #print( '>>>>{}'.format( line[j+1] ) )
                        done = True
                        break

                    continue

                if b_list[0] == 'TWO' or b_list[0] == 'QQQ' or b_list[0] == 'FMA' :

                    OW_dest = ( b_list[3] == a_list[1] or b_list[4] == a_list[1] )
                    OW_src  = ( b_list[3] == a_list[2] or b_list[4] == a_list[2] )
                    tobeTerminated = OW_dest or OW_src

                    if b_list[1] == a_list[1] or b_list[2] == a_list[1] :
                        if b_list[1] == a_list[1] :
                            b_list[1] = a_list[2]
                        if b_list[2] == a_list[1] :
                            b_list[2] = a_list[2]
                        line[j] = '{} {} {} {} {}'.format( b_list[0], b_list[1], b_list[2], b_list[3], b_list[4] )
                        done = True

                    if OW_dest :
                        line[i] = '!'

                    #if done :
                    #    print( '>>>>{}'.format( line[j] ) )

                    if tobeTerminated :
                        done = False
                        break

                    continue

                if b_list[0] == 'MAD' :

                    OW_dest = ( b_list[1] == a_list[1] )
                    OW_src  = ( b_list[1] == a_list[2] )
                    tobeTerminated = OW_dest or OW_src

                    if b_list[2] == a_list[1] or b_list[3] == a_list[1] or b_list[4] == a_list[1] :
                        if b_list[2] == a_list[1] :
                            b_list[2] = a_list[2]
                        if b_list[3] == a_list[1] :
                            b_list[3] = a_list[2]
                        if b_list[4] == a_list[1] :
                            b_list[4] = a_list[2]
                        line[j] = '{} {} {} {} {}'.format( b_list[0], b_list[1], b_list[2], b_list[3], b_list[4] )
                        done = True

                    if OW_dest :
                        line[i] = '!'

                    #if done :
                    #    print( '>>>>{}'.format( line[j] ) )

                    if tobeTerminated :
                        done = False
                        break

                    continue

                if b_list[0] == 'ADD' or b_list[0] == 'MUL' :

                    OW_dest = ( b_list[1] == a_list[1] )
                    OW_src  = ( b_list[1] == a_list[2] )
                    tobeTerminated = OW_dest or OW_src

                    if b_list[2] == a_list[1] or b_list[3] == a_list[1] :
                        if b_list[2] == a_list[1] :
                            b_list[2] = a_list[2]
                        if b_list[3] == a_list[1] :
                            b_list[3] = a_list[2]
                        line[j] = '{} {} {} {}'.format( b_list[0], b_list[1], b_list[2], b_list[3] )
                        done = True

                    if OW_dest :
                        line[i] = '!'

                    #if done :
                    #    print( '>>>>{}'.format( line[j] ) )

                    if tobeTerminated :
                        done = False
                        break

                    continue

                if b_list[0] == 'SUM' :

                    OW_dest = ( b_list[2] == a_list[1] )
                    OW_src  = ( b_list[2] == a_list[2] )
                    tobeTerminated = OW_dest or OW_src

                    L = len( b_list )
                    va = ''
                    na = 0
                    for k in range( 3, L ) :
                        if b_list[k] == a_list[1] :
                            b_list[k] = a_list[2]
                        va = concat( va, b_list[k] )
                        na = na +1

                    if na > 2 :
                        line[j] = 'SUM {} {} {}'.format( na, b_list[2], va )
                        done = True
                    if na == 2 :
                        line[j] = 'ADD {} {}'.format( b_list[2], va )
                        done = True
                    if na == 1 :
                        line[j] = 'LET {} {}'.format( b_list[2], va )
                        done = True

                    if OW_dest :
                        line[i] = '!'
                        
                    #if done :
                    #    print( '>>>>{}'.format( line[j] ) )

                    if tobeTerminated :
                        done = False
                        break

                    continue

            if done :
                stable = False
                #print( 'LET is commented out {}'.format( line[i] ) )
                line[i] = '!'


        #print( line )

        for i in range( len( line )-1 ) :
            if '!' in line[i] :
                continue
            a_list = line[i].split()
            if a_list[0] == 'MUL' and 'half' in a_list[3] :
                if i+1 >= len(line) :
                    continue
                b_list = line[i+1].split()
                if b_list[0] == 'ADD' and a_list[1] == b_list[2] and b_list[2] == b_list[3] :
                    #print( 'HALF is removed / {} / {}'.format( line[i], line[i+1] ) )
                    stable = False
                    line[i] = '!'
                    line[i+1] = 'LET {} {}'.format( b_list[1], a_list[2] )
                    if a_list[1] in line[i+1] :
                        continue
                    if a_list[2] in line[i+1] :
                        continue
                    if i+2 >= len(line) :
                        continue
                    b_list = line[i+2].split()
                if b_list[0] == 'ADD' and a_list[1] == b_list[2] and b_list[2] == b_list[3] :
                    #print( 'HALF is removed / {} / {}'.format( line[i], line[i+1] ) )
                    stable = False
                    line[i] = '!'
                    line[i+2] = 'LET {} {}'.format( b_list[1], a_list[2] )

        line = Cregs_backpropagete ( line )

        itr = itr + 1
        if stable and itr > 2 :
            break

    #print( line )
    return line


def Cregs_backpropagete ( line ) :

    for i in reversed( range( len(line) ) ) :
        if '!' in line[i] :
            continue
        a_list = line[i].split()
        if a_list[0] != 'LET' :
            continue
        if not ( 'c' in a_list[1] ) :
            continue
        if not ( 'e' in a_list[2] ) :
            continue

        c_reg = a_list[1]
        e_reg = a_list[2]

        for j in reversed( range( i ) ) :
            if '!' in line[j] :
                continue

            b_list = line[j].split()
            line[j] = ''
            for k in range( len(b_list) ) :
                if b_list[k] == e_reg :
                    b_list[k] = c_reg
                line[j] = concat( line[j], b_list[k] )

        line[i] = '!'

    return line


def Remove_uneffective( line ) :

    while True :

        stable = True
        for i in range( len( line ) ) :
            if '!' in line[i] :
                continue
            a_list = line[i].split()

            if a_list[0] == 'LET' :
                if a_list[1] == a_list[2] :
                    linr[i] = '!'
                    continue

            if a_list[0] == 'TWO' or a_list[0] == 'QQQ' :
                if a_list[1] == a_list[2] :
                    line[i] = 'ADD {} {} {}'.format( a_list[3], a_list[1], a_list[1] )
                    insert( line, i+1, 'ZER {}'.format( a_list[4] ) )
                    stable = False
                    break

            if a_list[0] == 'LET' or a_list[0] == 'ADD' or a_list[0] == 'MUL' or a_list[0] == 'MAD' :
                if not is_effective( a_list[1], line, i+1 ) :
                    line[i] = '!'
                continue

            if a_list[0] == 'SUM' :
                if not is_effective( a_list[2], line, i+1 ) :
                    line[i] = '!'
                continue

            if a_list[0] == 'TWO' :
                if '!' in line[i+1] :
                    continue
                if line[i] == line[i+1] :
                    line[i+1] = '!'
                    continue

                b_list = line[i+1].split()
                if b_list[0] == 'QQQ' and b_list[1] == a_list[1] and b_list[2] == a_list[2] and b_list[3] == a_list[3] and b_list[4] == a_list[4] :
                    line[i+1] = '!'
                continue

            if a_list[0] == 'QQQ' :
                if line[i] == line[i+1] :
                    line[i+1] = '!'
                continue

        if stable :
            break

    return line


def regs_rename( line ) :

    NumRegs = 0
    for i in range( len(line) ) :
        if '!' in line[i] :
            continue
        a_list = line[i].split()
        if len(a_list) < 2 :
            continue
        if not ( 'e' in line[i] ) :
            continue

        for j in range( 1, len(a_list) ) :
            if not ( 'e' in a_list[j] ) :
                continue

            t_reg = 't{}'.format( NumRegs )

            flag = 0
            for k in range( i, len(line) ) :
                if '!' in line[k] :
                    continue
                if not ( 'e' in line[k] ) :
                    continue

                b_list = line[k].split()
                line[k] = ''
                for l in range( len(b_list) ) :
                    if b_list[l] == a_list[j] :
                        b_list[l] = t_reg
                        flag = 1
                    line[k] = concat( line[k], b_list[l] )
            if flag == 1 :
                NumRegs = NumRegs + 1

    return ( line, NumRegs )


def print_code( Func, NA, NB, NC, ACC, line, NumRegs ) :

    def_head( Func, NA, NB, NC, ACC, 'a', 'b', 'c' )

    print( '{' )

    trick = ''
    if NumRegs > 0 :
        print( '  T', end='' )
        for i in range( NumRegs ) :
            print( '{} t{}'.format( trick, i ), end='' )
            trick = ','
        print( ';' )

    for i in range( len(line) ) :
        if '!' in line[i] :
            continue

        a_list = line[i].split()

        if a_list[0] == 'IN' :
            continue
        if a_list[0] == 'OUT' :
            continue

        if a_list[0] == 'LET' :
            print( '  {} = {};'.format( a_list[1], a_list[2] ) )

        if a_list[0] == 'TWO' :
            print( '  TwoSum( {}, {}, {}, {} );'.format( a_list[1], a_list[2], a_list[3], a_list[4] ) )

        if a_list[0] == 'FMA' :
            print( '  TwoProductFMA( {}, {}, {}, {} );'.format( a_list[1], a_list[2], a_list[3], a_list[4] ) )

        if a_list[0] == 'QQQ' :
            print( '  FastTwoSum( {}, {}, {}, {} );'.format( a_list[1], a_list[2], a_list[3], a_list[4] ) )

        if a_list[0] == 'MAD' :
            print( '  {} = std::fma ( {}, {}, {} );'.format( a_list[1], a_list[2], a_list[3], a_list[4] ) )

        if a_list[0] == 'ADD' :
            print( '  {} = {} + {};'.format( a_list[1], a_list[2], a_list[3] ) )

        if a_list[0] == 'SUB' :
            print( '  {} = {} - {};'.format( a_list[1], a_list[2], a_list[3] ) )

        if a_list[0] == 'MUL' :
            print( '  {} = {} * {};'.format( a_list[1], a_list[2], a_list[3] ) )

        if a_list[0] == 'DIV' :
            print( '  {} = {} / {};'.format( a_list[1], a_list[2], a_list[3] ) )

        if a_list[0] == 'SQRT' :
            print( '  {} = std::sqrt( {} );'.format( a_list[1], a_list[2] ) )

        if a_list[0] == 'ZER' or a_list[0] == 'OUTZ' :
            print( '  {} = fp_const<T>::zero();'.format( a_list[1] ) )

        if a_list[0] == 'DEF' :
            num_regs = int( a_list[1] )
            print( '  T', end='' )
            trick = ''
            for j in range( num_regs ) :
                print( '{} {}'.format( trick, a_list[2+j] ), end='' )
                trick = ','
            print( ';' )

        if a_list[0] == 'SUM' :
            num_regs = int( a_list[1] )
            print( '  {}'.format( a_list[2] ), end='' )
            trick = '='
            for j in range( num_regs ) :
                print( ' {} {}'.format( trick, a_list[3+j] ), end='' )
                trick = '+'
            print( ';' )

        if a_list[0] == 'add' or a_list[0] == 'sub' or a_list[0] == 'mul' or a_list[0] == 'div' :
            va = {}
            vb = {}
            vc = {}
            Na = int( a_list[1] )
            Nb = int( a_list[2] )
            Nc = int( a_list[3] )
            Acc = int( a_list[4] )
            for j in range( Na ) :
                va[j] = a_list[5+j]
                #print( 'VA={}'.format( va ) )
            for j in range( Nb ) :
                vb[j] = a_list[5+Na+j]
                #print( 'VB={}'.format( vb ) )
            for j in range( Nc ) :
                vc[j] = a_list[5+Na+Nb+j]
                #print( 'VC={}'.format( vc ) )

            print( '  ', end='' )
            caller_head( a_list[0], Na, Nb, Nc, Acc, va, vb, vc )
        
        if a_list[0] == 'sqr' or a_list[0] == 'sqrt' :
             va = {}
             vc = {}
             Na = int( a_list[1] )
             Nc = int( a_list[2] )
             Acc = int( a_list[3] )
             for j in range( Na ) :
                 va[j] = a_list[4+j]
                 #print( 'VA={}'.format( va ) )
             for j in range( Nc ) :
                 vc[j] = a_list[4+Na+j]
                 #print( 'VC={}'.format( vc ) )

             print( '  ', end='' )
             caller_head( a_list[0], Na, 0, Nc, Acc, va, va, vc )


    print( '}\n' )


def gen_add( NA, NB, NC, ACC ) :

    if NA < 1 or NB < 1 or NC < 1 :
        return
    if NA*NB*NC*ACC == 1 :
        return

    line = {}
    NumRegs = 0


    if NC == 1 :
        line[0] = '!'
        if ACC > 0 :
            line[1] = 'TWO a0 b0 c0 e0'
        else :
            line[1] = 'ADD c0 a0 b0'
            
        line[2] = 'ADD e1 a1 b1'
        line[3] = 'ADD e2 a2 b2'
        line[4] = 'ADD e3 a3 b3'
        line[5] = 'SUM 3 e1 e1 e2 e3'
        line[6] = 'ADD c0 c0 e1'
            
        if ACC > 0 :
            line[7] = 'ADD c0 c0 e0'

    if NC == 2 :
        line[0] = '!'
        line[1] = 'TWO a0 b0 c0 c1' # (c0)(c1)

        if ACC > 0 :
            line[2] = 'TWO a1 b1 e0 e1' # (c0)(c1 e0)(e1)
            line[3] = 'TWO c1 e0 c1 e0' # (c0)(c1)(e0 e1)
            line[4] = 'ADD c1 c1 e0' # (c0)(c1)(e1)
            line[5] = 'ADD e2 a2 b2' # (c0)(c1)(e1 e2)
            line[6] = 'ADD e3 a3 b3' # (c0)(c1)(e1 e2 e3)
            line[7] = 'ADD c1 c1 e2' # (c0)(c1)(e1 e3)
            line[8] = 'ADD c1 c1 e3' # (c0)(c1)(e1)
            line[9] = 'QQQ c0 c1 c0 c1' if NA*NB > 1 else '!'
            line[10] = 'ADD c1 c1 e1' # (c0)(c1)
            line[11] = 'QQQ c0 c1 c0 c1' if NA > 1 and NB > 1 else '!'
        else :
            line[2] = 'ADD e0 a1 b1'
            line[3] = 'ADD e1 a2 b2'
            line[4] = 'ADD e2 a3 b3'
            line[5] = 'SUM 3 e0 e0 e1 e2'
            line[6] = 'ADD c1 c1 e0'

        if ACC > 0 :
            line[12] = 'QQQ c0 c1 c0 c1'

    if NC == 3 :
        line[0] = '!'
        line[1] = 'TWO a0 b0 c0 c1' # (c0)(c1)
        line[2] = 'TWO a1 b1 e0 c2' # (c0)(c1 e0)(c2)
        line[3] = 'TWO c1 e0 c1 e0' # (c0)(c1)(c2 e0)

        if ACC > 0 :
            line[4] = 'TWO a2 b2 e1 e2' # (c0)(c1)(c2 e0 e1)(e2)
            line[5] = 'TWO c2 e0 c2 e0' # (c0)(c1)(c2 e1)(e0 e2)
            line[6] = 'TWO c2 e1 c2 e1' # (c0)(c1)(c2)(e0 e1 e2)
            line[7] = 'QQQ c1 c2 c1 c2'
            line[8] = 'ADD e3 a3 b3' # (c0)(c1)(c2)(e0 e1 e2 e3)
            line[9] = 'ADD c2 c2 e3' # (c0)(c1)(c2)(e0 e1 e2)
            line[10] = 'QQQ c1 c2 c1 c2' if NA*NB > 2 else '!'
            line[11] = 'SUM 3 e0 e0 e1 e2' # (c0)(c1)(c2)(e0)
            line[12] = 'ADD c2 c2 e0' # (c0)(c1)(c2)
            line[13] = 'QQQ c1 c2 c1 c2' if NA*NB > 2 else '!'
        else :
            line[4] = 'ADD e1 a2 b2' # (c0)(c1)(c2 e0 e1)
            line[5] = 'ADD e2 a3 b3' # (c0)(c1)(c2 e0 e1 e2)
            line[6] = 'SUM 4 c2 c2 e0 e1 e2' # (c0)(c1)(c2)

        if ACC > 0 :
            line[14] = 'QQQ c0 c1 c0 c1'
        if NA*NB > 2 and ACC > 0 :
            line[15] = 'QQQ c1 c2 c1 c2'
            line[16] = 'QQQ c0 c1 c0 c1'

    if NC == 4 :
        line[0] = '!'
        line[1] = 'TWO a0 b0 c0 c1' # (c0)(c1)
        line[2] = 'TWO a1 b1 e0 c2' # (c0)(c1 e0)(c2)
        line[3] = 'TWO a2 b2 e1 c3' # (c0)(c1 e0)(c2 e1)(c3)
        line[4] = 'TWO c1 e0 c1 e0' # (c0)(c1)(c2 e0 e1)(c3)
        line[5] = 'TWO c2 e0 c2 e0' # (c0)(c1)(c2 e1)(c3 e0)
        line[6] = 'TWO c2 e1 c2 e1' # (c0)(c1)(c2)(c3 e0 e1)

        if ACC > 0 :
            line[7] = 'TWO a3 b3 e2 e3' # (c0)(c1)(c2)(c3 e0 e1 e2)(e3)
            line[8] = 'TWO c3 e0 c3 e0' # (c0)(c1)(c2)(c3 e1 e2)(e0 e3)
            line[9] = 'TWO c3 e1 c3 e1' # (c0)(c1)(c2)(c3 e2)(e0 e1 e3)
            line[10] = 'TWO c3 e2 c3 e2' # (c0)(c1)(c2)(c3)(e0 e1 e2 e3)
            line[11] = 'QQQ c2 c3 c2 c3'
            line[12] = 'SUM 4 e0 e0 e1 e2 e3' # (c0)(c1)(c2)(c3)(e0)
            line[13] = 'ADD c3 c3 e0' # (c0)(c1)(c2)(c3)
        else :
            line[7] = 'ADD e2 a3 b3' # (c0)(c1)(c2)(c3 e0 e1 e2)
            line[8] = 'SUM 4 c3 c3 e0 e1 e2' # (c0)(c1)(c2)(c3)

        if ACC > 0 :
            line[14] = 'QQQ c0 c1 c0 c1'
        if NA > 1 and NB > 1 and ACC > 0 :
            line[15] = 'QQQ c1 c2 c1 c2'
            line[16] = 'QQQ c2 c3 c2 c3'
            line[17] = 'QQQ c0 c1 c0 c1'
            line[18] = 'QQQ c1 c2 c1 c2'
            line[19] = 'QQQ c0 c1 c0 c1'


    line = INIT_propagate( line, NA, NB, NC )
    line = ZERO_propagate( line )
    line = LET_propagate( line )
    line = Remove_uneffective( line )
    ( line, NumRegs ) = regs_rename( line )
    print_code( 'add', NA, NB, NC, ACC, line, NumRegs )


def gen_mul( NA, NB, NC, ACC ) :

    if NA < 1 or NB < 0 or NC < 1 :
        return
    if NA*NB*NC*ACC == 1 :
        return

    line = {}
    NumRegs = 0


    if NC == 1 :
        line[0] = '!'
        if ACC == 0 :
            line[1] = 'SUM 4 e0 a0 a1 a2 a3'
            line[2] = 'SUM 4 e1 b0 b1 b2 b3'
            line[3] = 'MUL c0 e0 e1'
        else :
            line[1] = 'MUL c0 a0 b0'
            line[2] = 'SUM 3 e0 a1 a2 a3'
            line[3] = 'SUM 3 e1 b1 b2 b3'
            line[4] = 'MAD c0 a0 e1 c0'
            line[5] = 'MAD c0 b0 e0 c0'
            line[6] = 'MAD c0 e0 e1 c0'

    if NC == 2 :
        line[0] = '!'
        line[1] = 'FMA a0 b0 c0 c1' # (c1)

        if ACC == 0 :
            line[2] = 'SUM 3 e0 a1 a2 a3'
            line[3] = 'SUM 3 e1 b1 b2 b3'
            line[4] = 'MAD c1 a0 e1 c1' # (c1)
            line[5] = 'MAD c1 b0 e0 c1' # (c1)
        else :
            line[2] = 'FMA a0 b1 e2 e3' # (c1,e2)(e3)
            line[3] = 'FMA a1 b0 e4 e5' # (c1,e2,e4)(e3,e5)
            line[4] = 'TWO e2 e4 e2 e4' # (c1,e2)(e4,e3,e5)
            line[5] = 'TWO c1 e2 c1 e2' # (c1)(e2,e4,e3,e5)
            line[6] = 'QQQ c0 c1 c0 c1' if NA*NB > 1 else '!'

            line[7] = 'ADD e3 e3 e5'  # (c1)(e2,e4,e3)
            line[8] = 'SUM 3 e2 e2 e3 e4' # (c1)(e2)

            line[9] = 'ADD e0 a2 a3'
            line[10] = 'ADD e1 b2 b3'
            line[11] = 'MAD e2 a1 b1 e2' # (c1)(e2)
            if NA == NB :
                line[12] = 'MUL e2 e2 fp_const<T>::half()' # (c1)(e2)
                line[13] = 'MAD e3 a0 e1 e2' # (c1)(e2,e3)
                line[14] = 'MAD e4 b0 e0 e2' # (c1)(e3,e4)
                line[15] = 'ADD e3 e3 e4' # (c1)(e3)
            else :
                line[12] = 'MAD e2 a0 e1 e2' # (c1)(e2)
                line[13] = 'MAD e3 b0 e0 e2' # (c1)(e3)
                line[14] = '!' # (c1)(e3)
                line[15] = '!' # (c1)(e3)

            line[16] = 'ADD c1 c1 e3' # (c1)
            line[17] = 'QQQ c0 c1 c0 c1' if NA*NB > 1 else '!'

    if NC == 3 :
        line[0] = '!'
        line[1] = 'FMA a0 b0 c0 c1' # (c1)
        line[2] = 'FMA a0 b1 c2 e2' # (c1,c2)(e2)
        line[3] = 'FMA a1 b0 e3 e4' # (c1,c2,e3)(e2,e4)
        line[4] = 'TWO c1 c2 c1 c2' # (c1,e3)(c2,e2,e4)
        line[5] = 'TWO c1 e3 c1 e3' # (c1)(c2,e3,e2,e4)

        if ACC == 0 :
            line[6] = 'SUM 4 c2 c2 e3 e2 e4' # (c1)(c2)
            line[7] = 'SUM 3 e0 a3 a2'
            line[8] = 'SUM 3 e1 b3 b2'
            line[9] = 'MAD c2 a0 e1 c2' # (c1)(c2)
            line[10] = 'MAD c2 a1 b1 c2' # (c1)(c2)
            line[11] = 'MAD c2 e0 b0 c2' # (c1)(c2)
        else :
            line[6] = 'QQQ c0 c1 c0 c1' if NA*NB > 1 else '!'

            line[7] = 'FMA a0 b2 e5 e6' # (c1)(c2,e3,e2,e4,e5)(e6)
            line[8] = 'FMA a2 b0 e7 e8' # (c1)(c2,e3,e2,e4,e5,e7)(e6,e8)
            line[9] = 'FMA a1 b1 e9 e10' # (c1)(c2,e3,e2,e4,e5,e7,e9)(e6,e8,e10)
            line[10] = 'TWO c2 e3 c2 e3' # (c1)(c2,e2,e4,e5,e7,e9)(e3,e6,e8,e10)
            line[11] = 'TWO c2 e2 c2 e2' # (c1)(c2,e4,e5,e7,e9)(e2,e3,e6,e8,e10)
            line[12] = 'TWO c2 e4 c2 e4' # (c1)(c2,e5,e7,e9)(e4,e2,e3,e6,e8,e10)
            line[13] = 'TWO c2 e5 c2 e5' # (c1)(c2,e7,e9)(e5,e4,e2,e3,e6,e8,e10)
            line[14] = 'TWO c2 e7 c2 e7' # (c1)(c2,e9)(e7,e5,e4,e2,e3,e6,e8,e10)
            line[15] = 'TWO c2 e9 c2 e9' # (c1)(c2)(e9,e7,e5,e4,e2,e3,e6,e8,e10)

            line[16] = 'SUM 9 e2 e2 e3 e4 e5 e6 e7 e8 e9 e10'  # (c1)(c2)(e2)

            if NA == NB :
                line[17] = 'MUL e2 e2 fp_const<T>::half()' # (c1)(c2)(e2)
                line[18] = 'MAD e3 a0 b3 e2' # (c1)(c2)(e3)
                line[19] = 'MAD e4 a3 b0 e2' # (c1)(c2)(e4,e3)
                line[20] = 'MAD e3 a1 b2 e3' # (c1)(c2)(e4,e3)
                line[21] = 'MAD e4 a2 b1 e4' # (c1)(c2)(e4,e3)
                line[22] = 'ADD e2 e3 e4' # (c1)(c2)(e2)
            else :
                line[17] = 'MAD e2 a0 b3 e2' # (c1)(c2)(e2)
                line[18] = 'MAD e2 a3 b0 e2' # (c1)(c2)(e2)
                line[19] = 'MAD e2 a1 b2 e2' # (c1)(c2)(e2)
                line[20] = 'MAD e2 a2 b1 e2' # (c1)(c2)(e2)
                line[21] = '!'
                line[22] = '!'

            line[23] = 'ADD c2 c2 e2' # (c1)(c2)

            line[24] = 'QQQ c0 c1 c0 c1' if NA*NB > 1 else '!'
            line[25] = 'QQQ c1 c2 c1 c2' if NA*NB > 1 else '!'
            line[26] = 'QQQ c0 c1 c0 c1' if NA*NB > 1 else '!'


    if NC == 4 :
        line[0] = '!'
        line[1] = 'FMA a0 b0 c0 c1' # (c1)

        line[2] = 'FMA a0 b1 c2 c3' # (c1,c2)(c3)
        line[3] = 'FMA a1 b0 e3 e4' # (c1,c2,e3)(c3,e4)
        line[4] = 'TWO c1 c2 c1 c2' # (c1,e3)(c2,c3,e4)
        line[5] = 'TWO c1 e3 c1 e3' # (c1)(c2,e3,c3,e4)

        line[6] = 'QQQ c0 c1 c0 c1' if NA*NB > 1 and ACC > 0 else '!'

        line[7] = 'FMA a0 b2 e5 e6' # (c1)(c2,e3,c3,e4,e5)(e6)
        line[8] = 'FMA a1 b1 e7 e8' # (c1)(c2,e3,c3,e4,e5,e7)(e6,e8)
        line[9] = 'FMA a2 b0 e9 e10' # (c1)(c2,e3,c3,e4,e5,e7,e9)(e6,e8,e10)

        line[10] = 'TWO c2 e3 c2 e3' # (c1)(c2,c3,e4,e5,e7,e9)(e3,e6,e8,e10)
        line[11] = 'TWO c2 c3 c2 c3' # (c1)(c2,e4,e5,e7,e9)(c3,e3,e6,e8,e10)
        line[12] = 'TWO c2 e4 c2 e4' # (c1)(c2,e5,e7,e9)(e4,c3,e3,e6,e8,e10)
        line[13] = 'TWO c2 e5 c2 e5' # (c1)(c2,e7,e9)(e5,e4,c3,e3,e6,e8,e10)
        line[14] = 'TWO c2 e7 c2 e7' # (c1)(c2,e9)(e7,e5,e4,c3,e3,e6,e8,e10)
        line[15] = 'TWO c2 e9 c2 e9' # (c1)(c2)(e9,e7,e5,e4,c3,e3,e6,e8,e10)

        if ACC == 0 :
            line[16] = 'SUM 9 c3 c3 e3 e4 e5 e6 e7 e8 e9 e10' # (c1)(c2)(c3)
            line[17] = 'MAD c3 a0 b3 c3' # (c1)(c2)(c3)
            line[18] = 'MAD c3 a1 b2 c3' # (c1)(c2)(c3)
            line[19] = 'MAD c3 a2 b1 c3' # (c1)(c2)(c3)
            line[20] = 'MAD c3 a3 b0 c3' # (c1)(c2)(c3)
        else :
            line[16] = 'QQQ c1 c2 c1 c2' if NA*NB > 1 else '!'

            line[17] = 'TWO c3 e9 c3 e9' # (c1)(c2)(c3,e7,e5,e4,e3,e6,e8,e10)(e9)
            line[18] = 'TWO c3 e7 c3 e7' # (c1)(c2)(c3,e5,e4,e3,e6,e8,e10)(e9,e7)
            line[19] = 'TWO c3 e5 c3 e5' # (c1)(c2)(c3,e4,e3,e6,e8,e10)(e9,e7,e5)
            line[20] = 'TWO c3 e4 c3 e4' # (c1)(c2)(c3,e3,e6,e8,e10)(e9,e7,e5,e4)
            line[21] = 'TWO c3 e3 c3 e3' # (c1)(c2)(c3,e6,e8,e10)(e9,e7,e5,e4,e3)
            line[22] = 'TWO c3 e6 c3 e6' # (c1)(c2)(c3,e8,e10)(e9,e7,e5,e4,e3,e6)
            line[23] = 'TWO c3 e8 c3 e8' # (c1)(c2)(c3,e10)(e8,e9,e7,e5,e4,e3,e6)
            line[24] = 'TWO c3 e10 c3 e10' # (c1)(c2)(c3)(e10,e8,e9,e7,e5,e4,e3,e6)
            line[25] = 'SUM 8 e3 e3 e4 e5 e6 e7 e8 e9 e10' # (c1)(c2)(c3)(e3)

            line[26] = 'FMA a0 b3 e4 e5' # (c1)(c2)(c3,e4)(e3,e5)
            line[27] = 'FMA a1 b2 e6 e7' # (c1)(c2)(c3,e4,e6)(e3,e5,e7)
            line[28] = 'FMA a2 b1 e8 e9' # (c1)(c2)(c3,e4,e6,e8)(e3,e5,e7,e9)
            line[29] = 'FMA a3 b0 e10 e11' # (c1)(c2)(c3,e4,e6,e8,e10)(e3,e5,e7,e9,e11)

            line[30] = 'TWO e4 e10 e4 e10' # (c1)(c2)(c3,e4,e6,e8)(e10,e3,e5,e7,e9,e11)
            line[31] = 'TWO e6 e8 e6 e8' # (c1)(c2)(c3,e4,e6)(e8,e10,e3,e5,e7,e9,e11)
            line[32] = 'TWO e4 e6 e4 e6' # (c1)(c2)(e4,c3)(e6,e8,e10,e3,e5,e7,e9,e11)
            line[33] = 'TWO c3 e4 c3 e4' # (c1)(c2)(c3)(e4,e6,e8,e10,e3,e5,e7,e9,e11)

            if NA == NB :
                line[34] = 'ADD e5 e5 e11' # (c1)(c2)(c3)(e4,e6,e8,e10,e3,e5,e7)
                line[35] = 'ADD e7 e7 e9'
                line[36] = 'SUM 7 e2 e3 e4 e5 e6 e7 e8 e10' # (c1)(c2)(c3)(e2)
            else :
                line[34] = 'SUM 9 e2 e3 e4 e5 e6 e7 e8 e9 e10 e11' # (c1)(c2)(c3)(e2)
                line[35] = '!'
                line[36] = '!'

            line[37] = 'QQQ c3 e2 c3 e2' if NA*NB > 4 else '!' # (c1)(c2)(c3)(e2)
            line[38] = 'QQQ c2 c3 c2 c3' if NA*NB > 4 else '!'

            if NA == NB :
                line[39] = 'MUL e3 e2 fp_const<T>::half()' # (c1)(c2)(c3)(e3)
                line[40] = 'MAD e0 a1 b3 e3' # (c1)(c2)(c3)(e0,e3)
                line[41] = 'MAD e1 a3 b1 e3' # (c1)(c2)(c3)(e0,e1,e3)
                line[42] = 'ADD e3 e0 e1' # (c1)(c2)(c3)(e3)
                line[43] = 'MAD e3 a2 b2 e3' # (c1)(c2)(c3)(e3)
            else :
                line[39] = 'MAD e3 a1 b3 e2' # (c1)(c2)(c3)(e3)
                line[40] = 'MAD e3 a2 b2 e3' # (c1)(c2)(c3)(e3)
                line[41] = 'MAD e3 a3 b1 e3' # (c1)(c2)(c3)(e3)
                line[42] = '!'
                line[43] = '!'
            line[44] = 'ADD c3 c3 e3' # (c1)(c2)(c3)

            line[45] = 'QQQ c0 c1 c0 c1'
            line[46] = 'QQQ c1 c2 c1 c2'
            line[47] = 'QQQ c2 c3 c2 c3'
            line[48] = 'QQQ c0 c1 c0 c1'
            line[49] = 'QQQ c1 c2 c1 c2'
            line[50] = 'QQQ c0 c1 c0 c1'


    line = INIT_propagate( line, NA, NB, NC )
    line = ZERO_propagate( line )
    line = LET_propagate( line )
    line = Remove_uneffective( line )
    ( line, NumRegs ) = regs_rename( line )
    print_code( 'mul', NA, NB, NC, ACC, line, NumRegs )


def gen_sqr( NA, NC, ACC ) :

    if NA < 1 or NC < 1 :
        return
    if NA*NC*ACC == 1 :
        return

    line = {}
    NumRegs = 0


    if NC == 1 :
        line[0] = '!'
        if ACC == 0 :
            line[1] = 'SUM 4 e0 a0 a1 a2 a3'
            line[2] = 'MUL c0 e0 e0'
        else :
            line[1] = 'MUL c0 a0 a0'
            line[2] = 'ADD e0 a0 a0'
            line[3] = 'SUM 3 e1 a1 a2 a3'
            line[4] = 'MAD c0 e0 e1 c0'


    if NC == 2 :
        line[0] = '!'
        line[1] = 'FMA a0 a0 c0 c1' # (c1)

        line[2] = 'ADD e0 a0 a0' # if NA > 1 else 'ZER e0'
        if ACC == 0 :
            line[3] = 'SUM 3 e1 a1 a2 a3'
            line[4] = 'MAD c1 e0 e1 c1' 
        else:
            line[3] = 'FMA e0 a1 e2 e3' # (c1,e2)(e3)
            line[4] = 'TWO c1 e2 c1 e2' # (c1)(e2,e3)
            line[5] = 'QQQ c0 c1 c0 c1'

            line[6] = 'ADD e2 e2 e3' # (c1)(e2)
            line[7] = 'MAD e2 a1 a1 e2' # (c1)(e2)
            line[8] = 'MAD e2 a2 e0 e2' # (c1)(e2)

            line[9] = 'ADD c1 c1 e2' # (c1)
            line[10] = 'QQQ c0 c1 c0 c1'


    if NC == 3 :
        line[0] = '!'
        line[1] = 'FMA a0 a0 c0 c1' # (c1)

        line[2] = 'ADD e0 a0 a0' # if NA > 1 else 'ZER e0'
        line[3] = 'FMA e0 a1 c2 e1' # (c1,c2)(e1)
        line[4] = 'TWO c1 c2 c1 c2' # (c1)(c2,e1)

        if ACC == 0 :
            line[5] = 'ADD c2 c2 e1' # (c1)(c2)
            line[6] = 'ADD e2 a2 a3' # (c1)(c2,e1)
            line[7] = 'MAD c2 e0 e2 c2' # (c1)(c2,e1)
            line[8] = 'MAD c2 a1 a1 c2' # (c1)(c2,e1)
        else :
            line[5] = 'QQQ c0 c1 c0 c1' if NA > 1 else '!'
            line[6] = 'FMA e0 a2 e3 e4' # (c1)(c2,e1,e3)(e4)
            line[7] = 'FMA a1 a1 e5 e6' # (c1)(c2,e1,e3,e5)(e4,e6)
            line[8] = 'TWO c2 e1 c2 e1' # (c1)(c2,e3,e5)(e1,e4,e6)
            line[9] = 'TWO c2 e3 c2 e3' # (c1)(c2,e5)(e3,e1,e4,e6)
            line[10] = 'TWO c2 e5 c2 e5' # (c1)(c2)(e5,e3,e1,e4,e6)
            line[11] = 'QQQ c1 c2 c1 c2'

            line[12] = 'SUM 5 e1 e1 e3 e4 e5 e6' # (c1)(c2)(e1)
            line[13] = 'MAD e1 e0 a3 e1'
            line[14] = 'ADD e2 a2 a2'
            line[15] = 'MAD e1 e2 a1 e1'

            line[16] = 'ADD c2 c2 e1'

            line[17] = 'QQQ c0 c1 c0 c1'
            line[18] = 'QQQ c1 c2 c1 c2'
            line[19] = 'QQQ c0 c1 c0 c1'


    if NC == 4 :
        line[0] = '!'
        line[1] = 'FMA a0 a0 c0 c1' # (c1)

        line[2] = 'ADD e0 a0 a0' # if NA > 1 else 'ZER e0'
        line[3] = 'FMA e0 a1 c2 c3' # (c1,c2)(c3)
        line[4] = 'TWO c1 c2 c1 c2' # (c1)(c2,c3)
        line[5] = 'QQQ c0 c1 c0 c1' if NA > 1 and ACC > 0 else '!'

        line[6] = 'TWO c2 c3 c2 c3' # (c1)(c2)(c3)
        line[7] = 'QQQ c1 c2 c1 c2' if NA > 1 and ACC > 0 else '!'

        line[8] = 'FMA e0 a2 e2 e3' # (c1)(c2,e2)(c3,e3)
        line[9] = 'QQQ c2 c3 c2 c3' if NA > 1 and ACC > 0 else '!'

        line[10] = 'FMA a1 a1 e4 e5' # (c1)(c2,e2,e4)(c3,e3,e5)
        line[11] = 'TWO c2 e2 c2 e2' # (c1)(c2,e4)(c3,e2,e3,e5)
        line[12] = 'TWO c2 e4 c2 e4' # (c1)(c2)(c3,e2,e3,e4,e5)
        line[13] = 'QQQ c2 c3 c2 c3' if NA > 1 and ACC > 0 else '!'

        line[14] = 'ADD e1 a1 a1' # if NA > 2 else 'ZER e1' # (c1)(c2)(c3,e2,e3,e4,e5)
        if ACC == 0 :
            line[15] = 'SUM 5 c3 c3 e2 e3 e4 e5' # (c1)(c2)(c3)
            line[16] = 'MAD c3 e0 a3 c3' # (c1)(c2)(c3)
            line[17] = 'MAD c3 e1 a2 c3' # (c1)(c2)(c3)
        else :
            line[15] = 'FMA e1 a2 e7 e8' # (c1)(c2)(c3,e2,e3,e4.e5,e7)(e8)
            line[16] = 'FMA a3 e0 e9 e10' # (c1)(c2)(c3,e2,e3,e4.e5,e7,e9)(e8,e10)
            line[17] = 'TWO e7 e9 e7 e9' # (c1)(c2)(c3,e2,e3.e4,e5,e7)(e8,e9,e10)
            line[18] = 'TWO c3 e2 c3 e2' # (c1)(c2)(c3,e3,e4,e5,e7)(e2,e8,e9,e10)
            line[19] = 'TWO c3 e3 c3 e3' # (c1)(c2)(c3,e4,e5,e7)(e2,e3,e8,e9,e10)
            line[20] = 'TWO c3 e4 c3 e4' # (c1)(c2)(c3,e5,e7)(e2,e3,e4,e8,e9,e10)
            line[21] = 'TWO c3 e7 c3 e7' # (c1)(c2)(c3,e5)(e2,e3,e4,e7,e8,e9,e10)
            line[22] = 'TWO c3 e5 c3 e5' # (c1)(c2)(c3)(e2,e3,e4,e5,e7,e8,e9,e10)
            line[23] = 'QQQ c2 c3 c2 c3'

            line[24] = 'SUM 8 e2 e2 e3 e4 e5 e7 e8 e9 e10' # (c1)(c2)(c3)(e1)
            line[25] = 'MAD e2 a3 e1 e2'
            line[26] = 'MAD e2 a2 a2 e2'

            line[27] = 'ADD c3 c3 e2'

            line[28] = 'QQQ c0 c1 c0 c1'
            line[29] = 'QQQ c1 c2 c1 c2'
            line[30] = 'QQQ c2 c3 c2 c3'
            line[31] = 'QQQ c0 c1 c0 c1'
            line[32] = 'QQQ c1 c2 c1 c2'
            line[33] = 'QQQ c0 c1 c0 c1'


    line = INIT_propagate( line, NA, NB, NC )
    line = ZERO_propagate( line )
    line = LET_propagate( line )
    line = Remove_uneffective( line )
    ( line, NumRegs ) = regs_rename( line )
    print_code( 'sqr', NA, 0, NC, ACC, line, NumRegs )


def gen_sub( NA, NB, NC, ACC ) :

    if NA < 1 or NB < 1 or NC < 1 :
        return
    if NA*NB*NC == 1 :
        if ACC > 0 :
            return

    line = {}
    
    va_list = ' '.join( [ 'a{}'.format( i ) for i in range( NA ) ] )
    vb_list = ' '.join( [ '-b{}'.format( i ) for i in range( NB ) ] )
    vc_list = ' '.join( [ 'c{}'.format( i ) for i in range( NC ) ] )

    line[0] = 'add {} {} {} {} {} {} {}'.format( NA, NB, NC, ACC, va_list, vb_list, vc_list )
    print_code( 'sub', NA, NB, NC, ACC, line, 0 )


def gen_div( NA, NB, NC, ACC ) :

    if NA < 1 or NB < 1 or NC < 1 :
        return
    if NA*NB*NC==1 :
        if ACC == 0 :
            def_head( 'div', NA, NB, NC, ACC, 'a', 'b', 'c' )
            print( '{' )
            print( '  c0 = a0 / b0;' )
            print( '}\n' )
        return
    if NA*NB==1 and NC>1 :
        def_head( 'div', NA, NB, NC, ACC, 'a', 'b', 'c' )
        print( '{' )
        for i in range( NC ) :
            if i == 0 :
                print( '  T r = a0;' )
            else :
                print( '  r = std::fma( -b0, c{}, r );'.format( i-1 ) )
            print( '  c{} = r / b0;'.format( i, i ) )
        print( '}\n' )
        return
    if NA==2 and NB==2 and NC==2 :
        def_head( 'div', NA, NB, NC, ACC, 'a', 'b', 'c' )
        print( '{' )
        # print( '  c0 = a0 / b0;' )
        print( '  c0 = a0 / (b0 + b1);' )
        print( '  c1 = (std::fma(-b0, c0, a0 ) + std::fma(-b1, c0, a1 )) / (b0 + b1);' )
        print( '}\n' )
        return

    line = {}
    e_list = {}

    LineCount = len( line )
    RegCounter = 40
    if ACC == 0 :
        if NC == 1 :
            line[LineCount] = 'DEF 2 e{} e{}'.format( RegCounter, RegCounter+1 )
            LineCount = LineCount + 1
            va_list = ' '.join( [ 'a{}'.format( i ) for i in range( NA ) ] )
            vb_list = ' '.join( [ 'b{}'.format( i ) for i in range( NB ) ] )
            line[LineCount] = 'SUM {} e{} {}'.format( NA, RegCounter, va_list )
            LineCount = LineCount + 1
            line[LineCount] = 'SUM {} e{} {}'.format( NB, RegCounter+1, vb_list )
            LineCount = LineCount + 1
            line[LineCount] = 'DIV c0 e{} e{}'.format( RegCounter, RegCounter+1 )
            LineCount = LineCount + 1
        else :
            NCC = NC - 1
            NAA = min( NA, NCC )
            NBB = min( NB, NCC )
            
            vt_list = ''
            line[LineCount] = 'DEF {}'.format( NC )
            for i in range( NC ) :
                line[LineCount] = '{} t{}'.format( line[LineCount], i )
                vt_list = '{} t{}'.format( vt_list, i )
            LineCount = LineCount + 1

            if ( NA >= NC and NB > NC ) or ( NA > NC and NB >= NC ):
                
                if NA > NC :
                    line[LineCount] = 'DEF 1 e{}'.format( RegCounter )
                    LineCount = LineCount + 1
                    e_list[0] = RegCounter
                    RegCounter = RegCounter + 1
                    vv = ' '.join( [ 'a{}'.format(i) for i in range( NC-1, NA ) ] )
                    line[LineCount] = 'SUM {} e{} {}'.format( NA-NC+1, e_list[0], vv )
                    LineCount = LineCount + 1
                if NB > NC :
                    line[LineCount] = 'DEF 1 e{}'.format( RegCounter )
                    LineCount = LineCount + 1
                    e_list[1] = RegCounter
                    RegCounter = RegCounter + 1
                    vv = ' '.join( [ 'b{}'.format(i) for i in range( NC-1, NB ) ] )
                    line[LineCount] = 'SUM {} e{} {}'.format( NB-NC+1, e_list[1], vv )
                    LineCount = LineCount + 1

                va_list = ' '.join( [ 'a{}'.format( i ) for i in range( NC-1 ) ] )
                vb_list = ' '.join( [ 'b{}'.format( i ) for i in range( NC-1 ) ] )
                vc_list = ' '.join( [ 'c{}'.format( i ) for i in range( NC ) ] )
                if NA > NC :
                    va_list = va_list + ' ' + 'e{}'.format( e_list[0] );
                else :
                    va_list = va_list + ' ' + 'a{}'.format( NC-1 );
                if NB > NC :
                    vb_list = vb_list + ' ' + 'e{}'.format( e_list[1] );
                else :
                    vb_list = vb_list + ' ' + 'b{}'.format( NC-1 );

                line[LineCount] = 'div {} {} {} {} {} {} {}'.format( min(NA, NC), min(NB,NC), NC, ACC, va_list, vb_list, vc_list )
                LineCount = LineCount + 1

            else :

                if NA > NCC :
                    line[LineCount] = 'DEF 1 e{}'.format( RegCounter )
                    LineCount = LineCount + 1
                    e_list[0] = RegCounter
                    RegCounter = RegCounter + 1
                    va_list = ' '.join( [ 'a{}'.format( i ) for i in range( NCC-1, NA ) ] )
                    line[LineCount] = 'SUM {} e{} {}'.format( NA-NCC+1, e_list[0], va_list )
                    LineCount = LineCount + 1

                if NB > NCC :
                    line[LineCount] = 'DEF 1 e{}'.format( RegCounter )
                    LineCount = LineCount + 1
                    e_list[1] = RegCounter
                    RegCounter = RegCounter + 1
                    vb_list = ' '.join( [ 'b{}'.format( i ) for i in range( NCC-1, NB ) ] )
                    line[LineCount] = 'SUM {} e{} {}'.format( NB-NCC+1, e_list[1], vb_list )
                    LineCount = LineCount + 1

                vA_list = ' '.join( [ 'a{}'.format( i ) for i in range( NA ) ] )
                vB_list = ' '.join( [ 'b{}'.format( i ) for i in range( NB ) ] )
                va_list = ' '.join( [ 'a{}'.format( i ) for i in range( NCC-1 if NCC < NA else NA ) ] ) \
                    + ( ' e{}'.format( e_list[0] ) if NA > NCC else '' )
                vb_list = ' '.join( [ 'b{}'.format( i ) for i in range( NCC-1 if NCC < NB else NB ) ] ) \
                    + ( ' e{}'.format( e_list[1] ) if NB > NCC else '' )
                vc_list = ' '.join( [ 'c{}'.format( i ) for i in range( NC-1 ) ] )

                vk_list = 'b0 b1 b2 b3'

                line[LineCount] = 'div {} {} {} {} {} {} {}'.format( NAA, NBB, NCC, ACC, va_list, vb_list, vc_list )
                LineCount = LineCount + 1
                line[LineCount] = 'mul {} {} {} {} {} {} {}'.format( NCC, NB, NC, ACC, vc_list, vB_list, vt_list )
                LineCount = LineCount + 1
                line[LineCount] = 'sub {} {} {} {} {} {} {}'.format( NA, NC, NC, ACC, vA_list, vt_list, vt_list )
                LineCount = LineCount + 1
                line[LineCount] = 'DEF 2 tn td'
                LineCount = LineCount + 1
                line[LineCount] = 'SUM {} tn {}'.format( NC, vt_list )
                LineCount = LineCount + 1
                #line[LineCount] = 'SUM {} td {}'.format( max(1,NB-1), vk_list )
                line[LineCount] = 'SUM {} td {}'.format( NB, vk_list )
                LineCount = LineCount + 1
                line[LineCount] = 'DIV c{} tn td'.format( NC-1 )

    else :

        NUM_T = max(NA,NB,NC)
        NUM_R = max(NA,NB,NC)

        line[LineCount] = 'DEF {}'.format( NUM_T )
        for i in range( NUM_T ) :
            line[LineCount] = '{} t{}'.format( line[LineCount], i )
        LineCount = LineCount + 1

        line[LineCount] = 'DEF {}'.format( NUM_R )
        for i in range( NUM_R ) :
            line[LineCount] = '{} r{}'.format( line[LineCount], i )
        LineCount = LineCount + 1

        va = ''
        for i in range( NA ) :
            va = '{} a{}'.format( va, i )
        vb = ''
        for i in range( NB ) :
            vb = '{} b{}'.format( vb, i )
        vr = ''
        for i in range( NUM_R ) :
            vr = '{} r{}'.format( vr, i )
        vt = ''
        for i in range( NUM_T ) :
            vt = '{} t{}'.format( vt, i )

        for i in range( NC ) :
            line[LineCount] = 'DIV c{} {}0 b0'.format( i, 'a' if i==0 else 'r' )
            LineCount = LineCount + 1
            line[LineCount] = 'mul {} {} {} {} {} c{} {}'.format( NB, 1, NUM_T, ACC, vb, i, vt )
            LineCount = LineCount + 1
            line[LineCount] = 'sub {} {} {} {} {} {} {}'.format( NA if i==0 else NUM_R, NUM_T, 1 if i==NC-1 else NUM_R, ACC, va if i==0 else vr, vt, 'r0' if i==NC-1 else vr )
            LineCount = LineCount + 1

        line[LineCount] = 'DIV r0 r0 b0'
        LineCount = LineCount + 1
        line[LineCount] = 'ADD c{} c{} r0'.format( NC-1, NC-1 )
        LineCount = LineCount + 1

        for i in reversed( range( 1, NC ) ) :
            line[LineCount] = 'QQQ c{} c{} c{} c{}'.format( i-1,i, i-1,i )
            LineCount = LineCount + 1

    print_code( 'div', NA, NB, NC, ACC, line, 0 )


            
        
if __name__ == '__main__' :

    init()

    for ACC in range( 2 ) :

        if  ACC == 0 :
             print(
'''
// ------------------------
// PA/Quasi Arithmetic Part
// ------------------------
''' )
        else :
             print(
'''
// ------------------------
// Accurate Arithmetic Part
// ------------------------
''' )

        for NA in range(1,4+1) :
            for NB in range(1,4+1) :
                for NC in range(1,4+1) :
                    #continue
                    gen_add( NA, NB, NC, ACC )

        for NA in range(1,4+1) :
            for NB in range(1,4+1) :
                for NC in range(1,4+1) :
                    #continue
                    gen_sub( NA, NB, NC, ACC )

        for NA in range(1,4+1) :
            for NB in range(1,4+1) :
                for NC in range(1,4+1) :
                    #continue
                    gen_mul( NA, NB, NC, ACC )

        for NA in range(1,4+1) :
            for NB in range(1,4+1) :
                for NC in range(1,4+1) :
                    #continue
                    gen_div( NA, NB, NC, ACC )

        for NA in range(1,4+1) :
                for NC in range(1,4+1) :
                    #continue
                    gen_sqr( NA, NC, ACC )


