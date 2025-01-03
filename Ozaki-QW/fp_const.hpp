#pragma once

//
// C++-ified version 2023.12.26 (refered to Ozaki's code)
//
// This C++ code was automatically generated
// referering to the idea by Katsuhisa Ozaki (Shibaura Institute of Technology)
// Debug is not sufficiently done, so please use it carefully and
//
// by Toshiyuki Imamura (RIKEN, R-CCS)
//


#include <stdio.h>
#include <math.h>
#include <cstdint>


namespace QxW {

  // ------------------------
  // Constants
  // ------------------------

  //
  template < typename T > struct fp_const { };
  //
  template <> struct fp_const<float> {
    static INLINE auto constexpr zero() NOEXCEPT { return 0.0f; }
    static INLINE auto constexpr one()  NOEXCEPT { return 1.0f; }
    static INLINE auto constexpr two()  NOEXCEPT { return 2.0f; }
    static INLINE auto constexpr nhalf() NOEXCEPT { return 0.5f; }
    static INLINE auto constexpr threehalves() NOEXCEPT { return 1.5f; }

    //
    // significant parameters based on IEE754
    //
    static uint32_t constexpr SOFW = sizeof(float)*8;
    static uint32_t constexpr SOFF = std::numeric_limits<float>::digits - 1;
    static uint32_t constexpr SOFE = SOFW - SOFF - 1;
    static uint32_t constexpr BONE = 0x00000001;
    static uint32_t constexpr SBIT = BONE << (SOFW-1); // 0x80000000;
    static uint32_t constexpr EONE = BONE << SOFF;   // 0x00800000;
    static uint32_t constexpr MASK = SBIT - EONE;    // 0x7f800000;
    static uint32_t constexpr FRAC = EONE - BONE;    // 0x007fffff;
    static uint32_t constexpr RINF = MASK - EONE;    // 0x7f000000;
    static uint32_t constexpr XONE = EONE * SOFF;    // 0x0b800000;

    static uint32_t constexpr FONE = EONE * ((1<<SOFE)-1); // 0x3f800000;
    static uint32_t constexpr PWRP = FONE + XONE;    // 0x4b000000;
    static uint32_t constexpr PWRN = FONE - XONE;    // 0x34000000;

    static INLINE auto constexpr fp2uint( float const a ) NOEXCEPT {
      union { float a; uint32_t e; } x = { .a = a };
      return x.e;
    }
    static INLINE auto constexpr uint2fp( uint32_t const e ) NOEXCEPT {
      union { float a; uint32_t e; } x = { .e = e };
      return x.a;
    }

    static INLINE auto constexpr is_pow2( float const a ) NOEXCEPT {
      //
      // 'a' is either power of 2 or 0 or INF
      //
      if ( a == zero() ) return true;

      auto x = std::fabs(a);
      // check if denormal floating point number
      auto is_small = ( x <= uint2fp( EONE ) );
      x *= ( is_small ? uint2fp( PWRP ) : one() );

      auto frac = fp2uint( a ) & FRAC;
      return ( frac == 0 );
    }

    static INLINE auto constexpr ulp( float const a ) NOEXCEPT {
      //
      // returns the unit bit of the last place with the exponent factor of 'a'
      // but if a is zero returns zero.
      //
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
      //
      // returns the exponent part considering the denormalized case
      // but if a is zero, INF, or NaN, it returns one.
      //
      if ( a == zero() ) return one();

      auto x = std::fabs(a);
      // check if denormal floating point number
      auto is_small = ( x <= uint2fp( EONE ) );
      x *= ( is_small ? uint2fp( PWRP ) : one() );
      auto e = fp2uint( x ) & MASK;

      // check if INF or NaN
      if ( e == MASK ) return one();
      if ( inverse ) e = RINF - e;

      auto r = uint2fp( e );
      r *= ( is_small ? uint2fp( inverse ? PWRP : PWRN ) : one() );
      return r;
    }
    static INLINE auto constexpr exponenti( float const a ) NOEXCEPT {
      //
      // returns the inverse of exponent()
      //
      return exponent<true>( a );
    }
  };
  template <> struct fp_const<double> {
    static INLINE auto constexpr zero() NOEXCEPT { return 0.0; }
    static INLINE auto constexpr one()  NOEXCEPT { return 1.0; }
    static INLINE auto constexpr two()  NOEXCEPT { return 2.0; }
    static INLINE auto constexpr nhalf() NOEXCEPT { return 0.5; }
    static INLINE auto constexpr threehalves() NOEXCEPT { return 1.5; }

    //
    // significant parameters based on IEE754
    //
    static uint32_t constexpr SOFW = sizeof(double)*8;
    static uint64_t constexpr SOFF = std::numeric_limits<double>::digits - 1;
    static uint64_t constexpr SOFE = SOFW - SOFF - 1;
    static uint64_t constexpr BONE = 0x0000000000000001;
    static uint64_t constexpr SBIT = BONE << (SOFW-1); // 0x8000000000000000;
    static uint64_t constexpr EONE = BONE << SOFF;   // 0x0010000000000000;
    static uint64_t constexpr MASK = SBIT - EONE;    // 0x7ff0000000000000;
    static uint64_t constexpr FRAC = EONE - BONE;    // 0x000fffffffffffff;
    static uint64_t constexpr RINF = MASK - EONE;    // 0x7fe0000000000000;
    static uint64_t constexpr XONE = EONE * SOFF;    // 0x0340000000000000;
    static uint64_t constexpr FONE = EONE * ((1<<SOFE)-1); // 0x3ff0000000000000;
    static uint64_t constexpr PWRP = FONE + XONE;    // 0x4330000000000000;
    static uint64_t constexpr PWRN = FONE - XONE;    // 0x3cb0000000000000;

    static INLINE auto constexpr fp2uint( double const a ) NOEXCEPT {
      union { double a; uint64_t e; } x = { .a = a };
      return x.e;
    }
    static INLINE auto constexpr uint2fp( uint64_t const e ) NOEXCEPT {
      union { double a; uint64_t e; } x = { .e = e };
      return x.a;
    }

    static INLINE auto constexpr is_pow2( double const a ) NOEXCEPT {
      //
      // 'a' is either power of 2 or 0 or INF
      //
#if 1
      auto x = fp2uint( a ) & ( MASK | FRAC );
      if ( ( x & (x - 1) ) == 0 ) return true;
      auto z = x & MASK;
      return ( z == x ) || ( z == MASK );
#else
      if ( a == zero() ) return true;

      auto x = std::fabs(a);
      // check if denormal floating point number
      auto is_small = ( x <= uint2fp( EONE ) );
      x *= ( is_small ? uint2fp( PWRP ) : one() );

      auto frac = fp2uint( x ) & FRAC;
      return ( frac == 0 );
#endif
    }

    static INLINE auto constexpr ulp( double const a ) NOEXCEPT {
      //
      // returns the unit bit of the last place with the exponent factor of 'a'
      // but if a is zero returns zero.
      //
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
      //
      // returns the exponent part considering the denormalized case
      // but if a is zero, INF, or NaN, it returns one.
      //
      if ( a == zero() ) return one();

      auto x = std::fabs(a);
      // check if denormal floating point number
      auto is_small = ( x <= uint2fp( EONE ) );
      x *= ( is_small ? uint2fp( PWRP ) : one() );
      auto e = fp2uint( x ) & MASK;

      // check if INF or NaN
      if ( e == MASK ) return one();
      if ( inverse ) e = RINF - e;

      auto r = uint2fp( e );
      r *= ( is_small ? uint2fp( inverse ? PWRP : PWRN ) : one() );
      return r;
    }
    static INLINE auto constexpr exponenti( double const a ) NOEXCEPT {
      //
      // returns the inverse of exponent()
      //
      return exponent<true>( a );
    }
  };

}

