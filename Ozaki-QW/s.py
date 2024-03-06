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
  static inline auto constexpr zero() { return 0.0f; }
  static inline auto constexpr one()  { return 1.0f; }
  static inline auto constexpr nhalf() { return 0.5f; }

  static uint32_t constexpr MASK = 0x7f800000;
  static uint32_t constexpr RINF = 0x7f000000;

  template < bool inverse = false >
  static inline auto exponent( float const a ) {
    if ( a == zero() ) return one();
    uint32_t e = *(uint32_t *)&a;
    e &= MASK;
    if ( e == MASK ) return one();
    if ( inverse ) e = RINF - e;
    return *(float *)&e;
  }
  static inline auto exponenti( float const a ) {
    return exponent<true>( a );
  }
};
template <> struct fp_const<double> {
  static inline auto constexpr zero() { return 0.0; }
  static inline auto constexpr one()  { return 1.0; }
  static inline auto constexpr nhalf() { return 0.5; }

  static uint64_t constexpr MASK = 0x7ff0000000000000;
  static uint64_t constexpr RINF = 0x7fe0000000000000;

  template < bool inverse = false >
  static inline auto exponent( double const a ) {
    if ( a == zero() ) return one();
    uint64_t e = *(uint64_t *)&a;
    e &= MASK;
    if ( e == MASK ) return one();
    if ( inverse ) e = RINF - e;
    return *(double *)&e;
  }
  static inline auto exponenti( double const a ) {
    return exponent<true>( a );
  }
};


// ------------------------
// Basic Part
// ------------------------

template < typename T > INLINE void
TwoSum ( T const a, T const b, T &x, T &y )
{
  T z;
  x = a + b;
  z = x - a;
  y = (a - (x - z)) + (b - z);
}

template < typename T > INLINE void
FastTwoSum ( T const a, T const b, T &x, T &y )
{
  x = a + b;
  y = (a - x) + b;
}

template < typename T > INLINE void
TwoProductFMA ( T const a, T const b, T &x, T &y )
{
  x = a * b;
  y = std::fma(a, b, -x);
}
'''
    print( header )

def final () :

    header = '''

#include "sqrt.hpp"

}

#endif
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
        print( ' ({},{},{} ) NOEXCEPT'.format( va_list, vb_list, vc_list ) )
    else :
        print( ' ({},{} ) NOEXCEPT'.format( va_list, vc_list ) )

def caller_head ( Func, NA, NB, NC, ACC, va, vb, vc ) :
    fname( Func, NA, NB, NC, ACC )
    va_list = ','.join( [ ' {}'.format( va[i] ) for i in range( NA ) ] )
    vb_list = ','.join( [ ' {}'.format( vb[i] ) for i in range( NB ) ] )
    vc_list = ','.join( [ ' {}'.format( vc[i] ) for i in range( NC ) ] )
    if NB > 0 :
        print( ' ({},{},{} );'.format( va_list, vb_list, vc_list ) )
    else :
        print( ' ({},{} );'.format( va_list, vc_list ) )

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

            line_pre = line[j]
            a_list = line[j].split()

            if a_list[0] == 'IN' :
                continue
            if a_list[0] == 'OUT' or a_list[0] == 'OUTZ':
                if a_list[1] == zero :
                    line[j] = 'OUTZ {}'.format( a_list[1] )
                continue

            if a_list[0] == 'LET' :
                if a_list[2] == zero :
                    line[j] = 'ZER {}'.format( a_list[1] )
                    #print( '{} => {}'.format( line_pre, line[j] ) )
                if a_list[1] == zero :
                    break
                continue

            if a_list[0] == 'FMA' :
                if a_list[1] == zero or a_list[2] == zero :
                    line[j] = 'ZER {}'.format( a_list[3] )
                    insert( line, j+1, 'ZER {}'.format( a_list[4] ) )
                    #print( '{} => {} / {}'.format( line_pre, line[j], line[j+1] ) )
                if a_list[3] == zero or a_list[4] == zero :
                    break
                continue

            if a_list[0] == 'TWO' or a_list[0] == 'QQQ' :

                if a_list[1] == zero and a_list[2] == zero :
                    line[j] = 'ZER {}'.format( a_list[3] )
                    insert( line, j+1, 'ZER {}'.format( a_list[4] ) )
                    #print( '{} => {} / {}'.format( line_pre, line[j], line[j+1] ) )
                if a_list[1] == zero and a_list[2] != zero :
                    line[j] = 'LET {} {}'.format( a_list[3], a_list[2] )
                    insert( line, j+1, 'ZER {}'.format( a_list[4] ) )
                    #print( '{} => {} / {}'.format( line_pre, line[j], line[j+1] ) )
                if a_list[2] == zero and a_list[1] != zero :
                    line[j] = 'LET {} {}'.format( a_list[3], a_list[1] )
                    insert( line, j+1, 'ZER {}'.format( a_list[4] ) )
                    #print( '{} => {} / {}'.format( line_pre, line[j], line[j+1] ) )
                if a_list[3] == zero or a_list[4] == zero :
                    break
                continue

            if a_list[0] == 'ADD' :
                if a_list[2] == zero and a_list[3] == zero :
                    line[j] = 'ZER {}'.format( a_list[1] )
                    #print( '{} => {}'.format( line_pre, line[j] ) )
                if a_list[2] == zero and a_list[3] != zero :
                    line[j] = 'LET {} {}'.format( a_list[1], a_list[3] )
                    #print( '{} => {}'.format( line_pre, line[j] ) )
                if a_list[3] == zero and a_list[2] != zero :
                    line[j] = 'LET {} {}'.format( a_list[1], a_list[2] )
                    #print( '{} => {}'.format( line_pre, line[j] ) )
                if a_list[1] == zero :
                    break
                continue

            if a_list[0] == 'MUL' :
                if a_list[2] == zero or a_list[3] == zero :
                    line[j] = 'ZER {}'.format( a_list[1] )
                    #print( '{} => {}'.format( line_pre, line[j] ) )
                if a_list[1] == zero :
                    break
                continue

            if a_list[0] == 'MAD' :
                if ( a_list[2] == zero or a_list[3] == zero ) and (a_list[4] != zero ) :
                    line[j] = 'LET {} {}'.format( a_list[1], a_list[4] )
                    #print( '{} => {}'.format( line_pre, line[j] ) )
                if a_list[4] == zero :
                    line[j] = 'MUL {} {} {}'.format( a_list[1], a_list[2], a_list[3] )
                    #print( '{} => {}'.format( line_pre, line[j] ) )
                if a_list[1] == zero :
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
                        #print( '{} => {}'.format( line_pre, line[j] ) )
                if a_list[2] == zero :
                    break
                continue

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


        for i in range( len( line )-1 ) :
            if '!' in line[i] :
                continue
            a_list = line[i].split()
            if a_list[0] == 'MUL' and 'nhalf' in a_list[3] :
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
            NumRegs = NumRegs + 1

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
                    line[k] = concat( line[k], b_list[l] )

    return ( line, NumRegs )


def print_code( Func, NA, NB, NC, ACC, line, NumRegs ) :

    def_head( Func, NA, NB, NC, ACC, 'a', 'b', 'c' )

    print( '{' )

    trick = ''
    if NumRegs > 0 :
        print( '  T ' + ', '.join( 't{}'.format( i ) ) + ';' )

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
            print( '  T ' + ', '.join( a_list[2+0:2*num_regs] ) + ';' )

        if a_list[0] == 'SUM' :
            num_regs = int( a_list[1] )
            print( '  {} = '.format( a_list[2] ) + ' + '.join( a_list[3+0:3+num_regs] ) + ';' )

        if a_list[0] == 'add' or a_list[0] == 'sub' or a_list[0] == 'mul' or a_list[0] == 'div' :
            Na = int( a_list[1] )
            Nb = int( a_list[2] )
            Nc = int( a_list[3] )
            Acc = int( a_list[4] )
            va_list = a_list[5+0:5+Na]
            vb_list = a_list[5+Na:5+Na+Nb]
            vc_list = a_list[5+Na+Nb:5+Na+Nb+Nc]

            print( '  ', end='' )
            caller_head( a_list[0], Na, Nb, Nc, Acc, va_list, vb_list, vc_list )

        if a_list[0] == 'sqr' or a_list[0] == 'sqrt' :
            Na = int( a_list[1] )
            Nc = int( a_list[2] )
            Acc = int( a_list[3] )
            va_list = a_list[4+0:4+Na]
            vc_list = a_list[4+Na:4+Na+Nc]

            print( '  ', end='' )
            caller_head( a_list[0], Na, 0, Nc, Acc, va_list, va_list, vc_list )

    print( '}\n' )


def gen_sqrt( NA, NC, ACC ) :

    if NA < 1 or NC < 1 :
        return

    if NC==1 :
        if ACC == 0 :
            def_head( 'sqrt', NA, 0, NC, ACC, 'a', 'b', 'c' )
            print( '{' )
            va_list = ' + '.join( [ 'a{}'.format( i ) for i in range( NA ) ] )
            print( '  c0 = std::sqrt( {} );'.format( va_list ) )
            print( '}\n' )
        if ACC == 1 and NA > 1 :
            def_head( 'sqrt', NA, 0, NC, ACC, 'a', 'b', 'c' )
            print( '{' )

            print( '  T const as = std::sqrt( a0 );' )
            print( '  T const e  = fp_const<T>::exponent ( as );' )
            print( '  T const ex = fp_const<T>::exponenti( as );' )
            [ print( '  T const b{} = ( a{} * ex ) * ex;'.format( i, i ) ) for i in range( NA ) ]
            print( '' )
            print( '  T const x  = e / as;' )
            print( '  T const ax = b0 * x;' )
            print( '  T r0, r1;' )
            print( '' )
            print( '  TwoProductFMA( ax, ax, r0, r1 );' )
            if NA == 1 :
                print( '  sub_SW_DW_SW( b0, r0, r1, c0 );' )
            if NA == 2 :
                print( '  sub_DW_DW_SW( b0, b1, r0, r1, c0 );' )
            if NA == 3 :
                print( '  sub_TW_DW_SW( b0, b1, b2, r0, r1, c0 );' )
            if NA == 4 :
                print( '  sub_DW_DW_SW( b0, b1, b2, b3, r0, r1, c0 );' )
            print( '' )
            print( '  c0 = ax + c0 * ( x * fp_const<T>::nhalf() );' )
            print( '  c0 = c0 * e;' )
            print( '}\n' )
        return

    if NC==2 :
        if ACC == 0 :
            def_head( 'sqrt', NA, 0, NC, ACC, 'a', 'b', 'c' )
            print( '{' )
            va_list = ''.join( [ ' + a{}'.format( i ) for i in range( 1, NA ) ] )
            print( '  c0 = std::sqrt( a0 );' )
            print( '  c1 = ( std::fma( -c0, c0, a0 ){} ) / (c0 + c0);'.format( va_list ) )
            print( '}\n' )
            return
        if False and ACC == 1 :
            def_head( 'sqrt', NA, 0, NC, ACC, 'a', 'b', 'c' )
            print( '{' )

            # sqrt(a) = sqrt( a * ex*ex / (ex*ex) ) = sqrt(a') / ex = sqrt(a') * e
            # 1/sqrt(a') = e/sqrt(a)

            if NA == 2 :
                print( '  sqrt_PA_PA( a0, a1, c0, c1 );' )
                print( '  T const as = c0 + c1;' )
            else :
                print( '  T const as = std::sqrt( a0 );' )
            print( '  T const e  = fp_const<T>::exponent ( as );' )
            print( '  T const ex = fp_const<T>::exponenti( as );' )
            [ print( '  T const b{} = ( a{} * ex ) * ex;'.format( i, i ) ) for i in range( NA ) ]
            print( '' )
            print( '  T const x  = e / as;' )
            print( '  T const ax = b0 * x;' )
            print( '  T r0, r1;' )
            print( '' )
            print( '  TwoProductFMA( ax, ax, r0, r1 );' )
            if NA == 1 :
                print( '  sub_SW_DW_SW( b0, r0, r1, c0 );' )
            if NA == 2 :
                print( '  sub_DW_DW_SW( b0, b1, r0, r1, c0 );' )
            if NA == 3 :
                print( '  sub_TW_DW_SW( b0, b1, b2, r0, r1, c0 );' )
            if NA == 4 :
                print( '  sub_QW_DW_SW( b0, b1, b2, b3, r0, r1, c0 );' )
            print( '  TwoSum( ax, c0 * ( x * fp_const<T>::nhalf() ), c0, c1 );' )
            print( '\n  c0 = c0 * e;' )
            print( '  c1 = c1 * e;' )

            print( '}\n' )
            return

    line = {}
    e_list = {}

    LineCount = len( line )
    RegCounter = 40
    if ACC == 0 :
        if NA > NC :

            va_list = ' '.join( [ 'a{}'.format( i ) for i in range( NC-1 ) ] ) + ' ' \
                    + '+'.join( [ 'a{}'.format( i ) for i in range( NC-1, NA ) ] )
            vc_list = ' '.join( [ 'c{}'.format( i ) for i in range( NC ) ] )

            line[LineCount] = 'sqrt {} {} {} {} {}'.format( NC, NC, ACC, va_list, vc_list )
            LineCount = LineCount + 1

        else :
            NCC = NC - 1
            
            va_list = ' '.join( [ 'a{}'.format( i ) for i in range( NA ) ] )
            vc_list = ' '.join( [ 'c{}'.format( i ) for i in range( NCC ) ] )
            vt_list = ' '.join( [ 't{}'.format( i ) for i in range( NC ) ] )

            line[LineCount] = 'DEF {} {}'.format( NC, vt_list )
            LineCount = LineCount + 1
            line[LineCount] = 'sqrt {} {} {} {} {}'.format( NA, NCC, ACC, va_list, vc_list )
            LineCount = LineCount + 1
            line[LineCount] = 'sqr {} {} {} {} {}'.format( NCC, NC, ACC, vc_list, vt_list )
            LineCount = LineCount + 1
            line[LineCount] = 'sub {} {} {} {} {} {} {}'.format( NA, NC, NC, ACC, va_list, vt_list, vt_list )
            LineCount = LineCount + 1
            line[LineCount] = 'DEF 2 tn td'
            LineCount = LineCount + 1
            line[LineCount] = 'SUM {} tn {}'.format( NC, vt_list )
            LineCount = LineCount + 1
            line[LineCount] = 'SUM {} td {}'.format( NCC, vc_list )
            LineCount = LineCount + 1
            line[LineCount] = 'DIV c{} tn td'.format( NC-1 )
            LineCount = LineCount + 1
            line[LineCount] = 'MUL c{} c{} fp_const<T>::nhalf()'.format( NC-1, NC-1 )

    else :

        def_head( 'sqrt', NA, 0, NC, ACC, 'a', 'b', 'c' )
        print( '{' )
        print( '  T const as = std::sqrt( a0 );' )
        print( '  T const e  = fp_const<T>::exponent( as );' )
        print( '  T const e2 = e * fp_const<T>::two();' )
        print( '  T const ex = fp_const<T>::exponenti( as );' )
        print( '  T const ex2 = ex * fp_const<T>::nhalf();' )

        NX = max(NA, NC)

        [ print( '  T const ax{} = (a{} * ex) * ex2;'.format( i, i ) ) for i in range( NA ) ]
        print( '  T const h0 = 3*fp_const<T>::nhalf();' )

        print( '  T ' + ', '.join( [ 't{}'.format( i ) for i in range( NX ) ] ) + ';' )
        print( '  T ' + ', '.join( [ 'r{}'.format( i ) for i in range( NX ) ] ) + ';' )
        print( '  r0 = e / as;' )

        vax = [ 'ax{}'.format(i) for i in range( NX ) ]
        vr  = [ 'r{}'.format(i) for i in range( NX ) ]
        vh  = [ 'h{}'.format(i) for i in range( NX ) ]
        vt  = [ 't{}'.format(i) for i in range( NX ) ]

        for itr in range( NC-1 ) :
            print( '' )
            if itr == 0 :
                caller_head ( '  sqr', 1, 0, 2, ACC, vr, vr, vt )
                caller_head ( '  mul', NA, 2, NX, ACC, vax, vt, vt )
            else :
                caller_head ( '  sqr', NX, 0, NX, ACC, vr, vr, vt )
                caller_head ( '  mul', NA, NX, NX, ACC, vax, vt, vt )
            caller_head ( '  sub', 1, NX, NX, ACC, vh, vt, vt )
            if itr == 0 :
                caller_head ( '  mul', NX, 1, NC, ACC, vt, vr, vr )
            elif itr == NC-2 :
                caller_head ( '  mul', NX, NX, NC, ACC, vt, vr, vr )
            else :
                caller_head ( '  mul', NX, NX, NX, ACC, vt, vr, vr )

        print( '' )
        caller_head ( '  mul', NA, NC, NC, ACC, vax, vr, vr )
        [ print( '  c{} = r{} * e2;'.format( i, i ) ) for i in range( NC ) ]
        print( '}\n' )

        return

    print_code( 'sqrt', NA, 0, NC, ACC, line, 0 )


            
        
if __name__ == '__main__' :

    print (
'''
// ------------------------
// Square Root Operations
// ------------------------
''' )
    for ACC in range( 2 ) :

        for NA in range(1,4+1) :
                for NC in range(1,4+1) :
                    gen_sqrt( NA, NC, ACC )

    print( '}' )
    print( '' )
    print( '' )
    print( '#endif' )




