#ifndef QXW_FP_H
#define QXW_FP_H


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
    static uint32_t constexpr FONE = 0x00000001;
    static uint32_t constexpr SBIT = FONE << (32-1); // 0x80000000;
    static uint32_t constexpr EONE = FONE << (23);   // 0x00800000;
    static uint32_t constexpr MASK = SBIT - EONE;    // 0x7f800000;
    static uint32_t constexpr FRAC = EONE - FONE;    // 0x007fffff;
    static uint32_t constexpr RINF = MASK - EONE;    // 0x7f000000;
    static uint32_t constexpr XONE = EONE * 23;      // 0x0b800000;

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
      // returns the exponent part with the hidden top bit number component
      // but if a is zero returns one.
      //
      if ( a == zero() ) return one();
      auto e = fp2uint( a );
      e &= MASK;
      if ( e == MASK ) return one();
      if ( inverse ) e = RINF - e;
      return uint2fp( e );
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
    static uint64_t constexpr FONE = 0x0000000000000001;
    static uint64_t constexpr SBIT = FONE << (64-1); // 0x8000000000000000;
    static uint64_t constexpr EONE = FONE << (52);   // 0x0010000000000000;
    static uint64_t constexpr MASK = SBIT - EONE;    // 0x7ff0000000000000;
    static uint64_t constexpr FRAC = EONE - FONE;    // 0x000fffffffffffff;
    static uint64_t constexpr RINF = MASK - EONE;    // 0x7fe0000000000000;
    static uint64_t constexpr XONE = EONE * 52;      // 0x0340000000000000;

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
      auto frac = fp2uint( a ) & FRAC;
      return ( frac == 0 );
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
      // returns the exponent part with the hidden top bit number component
      // but if a is zero returns one.
      //
      if ( a == zero() ) return one();
      auto e = fp2uint( a );
      e &= MASK;
      if ( e == MASK ) return one();
      if ( inverse ) e = RINF - e;
      return uint2fp( e );
    }
    static INLINE auto constexpr exponenti( double const a ) NOEXCEPT {
      //
      // returns the inverse of exponent()
      //
      return exponent<true>( a );
    }
  };

}
#endif

