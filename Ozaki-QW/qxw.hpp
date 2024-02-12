#ifndef QXW_H
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
    static inline auto constexpr two()  { return 2.0f; }
    static inline auto constexpr half() { return 0.5f; }

    static uint32_t constexpr SBIT = 0x80000000;
    static uint32_t constexpr MASK = 0x7f800000;
    static uint32_t constexpr RINF = 0x7f000000;
    static uint32_t constexpr XONE = 0x0b800000;
    static uint32_t constexpr EONE = 0x00800000;

    static inline auto constexpr fp2uint( float const a ) {
      union { float a; uint32_t e; } x = { .a = a };
      return x.e;
    }
    static inline auto constexpr uint2fp( uint32_t const e ) {
      union { float a; uint32_t e; } x = { .e = e };
      return x.a;
    }

    template < bool doubled = false >
    static INLINE auto constexpr hbit( float const a ) {
      auto e = fp2uint( a );
      auto s = e & SBIT;
      e &= MASK;
      if ( doubled ) e += EONE;
      e |= s;
      return uint2fp( e );
    }
    static INLINE auto constexpr hbit2( float const a ) {
      return hbit<true>( a );
    }
    static INLINE auto constexpr ulp( float const a ) {
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
    static inline auto constexpr exponent( float const a ) {
      if ( a == zero() ) return one();
      auto e = fp2uint( a );
      e &= MASK;
      if ( e == MASK ) return one();
      if ( inverse ) e = RINF - e;
      return uint2fp( e );
    }
    static inline auto constexpr exponenti( float const a ) {
      return exponent<true>( a );
    }
  };
  template <> struct fp_const<double> {
    static inline auto constexpr zero() { return 0.0; }
    static inline auto constexpr one()  { return 1.0; }
    static inline auto constexpr two()  { return 2.0; }
    static inline auto constexpr half() { return 0.5; }

    static uint64_t constexpr SBIT = 0x8000000000000000;
    static uint64_t constexpr MASK = 0x7ff0000000000000;
    static uint64_t constexpr RINF = 0x7fe0000000000000;
    static uint64_t constexpr XONE = 0x0340000000000000;
    static uint64_t constexpr EONE = 0x0010000000000000;

    static inline auto constexpr fp2uint( double const a ) {
      union { double a; uint64_t e; } x = { .a = a };
      return x.e;
    }
    static inline auto constexpr uint2fp( uint64_t const e ) {
      union { double a; uint64_t e; } x = { .e = e };
      return x.a;
    }

    template < bool doubled = false >
    static INLINE auto constexpr hbit( double const a ) {
      auto e = fp2uint( a );
      auto s = e & SBIT;
      e &= MASK;
      if ( doubled ) e += EONE;
      e |= s;
      return uint2fp( e );
    }
    static INLINE auto constexpr hbit2( double const a ) {
      return hbit<true>( a );
    }
    static INLINE auto constexpr ulp( double const a ) {
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
    static inline auto constexpr exponent( double const a ) {
      if ( a == zero() ) return one();
      auto e = fp2uint( a );
      e &= MASK;
      if ( e == MASK ) return one();
      if ( inverse ) e = RINF - e;
      return uint2fp( e );
    }
    static inline auto constexpr exponenti( double const a ) {
      return exponent<true>( a );
    }
  };


  // ------------------------
  // Basic EFT Part
  // ------------------------

  template < typename T > __always_inline void constexpr
  TwoSum ( T const a, T const b, T &x, T &y )
  {
    T z;
    x = a + b;
    z = x - a;
    y = (a - (x - z)) + (b - z);
  }

  template < typename T > __always_inline void constexpr
  FastTwoSum ( T const a, T const b, T &x, T &y )
  {
    x = a + b;
    y = (a - x) + b;
  }

  template < typename T > __always_inline void constexpr
  TwoProductFMA ( T const a, T const b, T &x, T &y )
  {
    x = a * b;
    y = std::fma(a, b, -x);
  }


  // ------------------------
  // PA/Quasi Arithmetic Part
  // ------------------------

  // add: 1-1-1
  template < typename T > __always_inline void constexpr
  add_SW_SW_SW( T const a0, T const b0, T &c0 )
  {
    c0 = a0 + b0;
  }

  // add: 1-1-2
  template < typename T > __always_inline void constexpr
  add_SW_SW_PA( T const a0, T const b0, T &c0, T &c1 )
  {
    TwoSum( a0, b0, c0, c1 );
  }

  // add: 1-1-3
  template < typename T > __always_inline void constexpr
  add_SW_SW_QTW( T const a0, T const b0, T &c0, T &c1, T &c2 )
  {
    TwoSum( a0, b0, c0, c1 );
    c2 = fp_const<T>::zero();
  }

  // add: 1-1-4
  template < typename T > __always_inline void constexpr
  add_SW_SW_QQW( T const a0, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    TwoSum( a0, b0, c0, c1 );
    c2 = fp_const<T>::zero();
    c3 = fp_const<T>::zero();
  }

  // add: 1-2-1
  template < typename T > __always_inline void constexpr
  add_SW_PA_SW( T const a0, T const b0, T const b1, T &c0 )
  {
    c0 = a0 + b0;
    c0 = c0 + b1;
  }

  // add: 1-2-2
  template < typename T > __always_inline void constexpr
  add_SW_PA_PA( T const a0, T const b0, T const b1, T &c0, T &c1 )
  {
    TwoSum( a0, b0, c0, c1 );
    c1 = c1 + b1;
  }

  // add: 1-2-3
  template < typename T > __always_inline void constexpr
  add_SW_PA_QTW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, c2 );
  }

  // add: 1-2-4
  template < typename T > __always_inline void constexpr
  add_SW_PA_QQW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, c2 );
    c3 = fp_const<T>::zero();
  }

  // add: 1-3-1
  template < typename T > __always_inline void constexpr
  add_SW_QTW_SW( T const a0, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0;
    c0 = a0 + b0;
    t0 = b1 + b2;
    c0 = c0 + t0;
  }

  // add: 1-3-2
  template < typename T > __always_inline void constexpr
  add_SW_QTW_PA( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    t0 = b1 + b2;
    c1 = c1 + t0;
  }

  // add: 1-3-3
  template < typename T > __always_inline void constexpr
  add_SW_QTW_QTW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, t0 );
    c2 = t0 + b2;
  }

  // add: 1-3-4
  template < typename T > __always_inline void constexpr
  add_SW_QTW_QQW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, t0 );
    TwoSum( t0, b2, c2, c3 );
  }

  // add: 1-4-1
  template < typename T > __always_inline void constexpr
  add_SW_QQW_SW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0;
    c0 = a0 + b0;
    t0 = b1 + b2 + b3;
    c0 = c0 + t0;
  }

  // add: 1-4-2
  template < typename T > __always_inline void constexpr
  add_SW_QQW_PA( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    t0 = b1 + b2 + b3;
    c1 = c1 + t0;
  }

  // add: 1-4-3
  template < typename T > __always_inline void constexpr
  add_SW_QQW_QTW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, t0 );
    c2 = t0 + b2 + b3;
  }

  // add: 1-4-4
  template < typename T > __always_inline void constexpr
  add_SW_QQW_QQW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, t0 );
    TwoSum( t0, b2, c2, t1 );
    c3 = t1 + b3;
  }

  // add: 2-1-1
  template < typename T > __always_inline void constexpr
  add_PA_SW_SW( T const a0, T const a1, T const b0, T &c0 )
  {
    c0 = a0 + b0;
    c0 = c0 + a1;
  }

  // add: 2-1-2
  template < typename T > __always_inline void constexpr
  add_PA_SW_PA( T const a0, T const a1, T const b0, T &c0, T &c1 )
  {
    TwoSum( a0, b0, c0, c1 );
    c1 = c1 + a1;
  }

  // add: 2-1-3
  template < typename T > __always_inline void constexpr
  add_PA_SW_QTW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2 )
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, c2 );
  }

  // add: 2-1-4
  template < typename T > __always_inline void constexpr
  add_PA_SW_QQW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, c2 );
    c3 = fp_const<T>::zero();
  }

  // add: 2-2-1
  template < typename T > __always_inline void constexpr
  add_PA_PA_SW( T const a0, T const a1, T const b0, T const b1, T &c0 )
  {
    T t0;
    c0 = a0 + b0;
    t0 = a1 + b1;
    c0 = c0 + t0;
  }

  // add: 2-2-2
  template < typename T > __always_inline void constexpr
  add_PA_PA_PA( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + b1;
    c1 = c1 + t0;
  }

  // add: 2-2-3
  template < typename T > __always_inline void constexpr
  add_PA_PA_QTW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    c2 = c2 + t0;
  }

  // add: 2-2-4
  template < typename T > __always_inline void constexpr
  add_PA_PA_QQW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, c3, c2 );
    TwoSum( c1, c3, c1, c3 );
    TwoSum( c2, c3, c2, c3 );
  }

  // add: 2-3-1
  template < typename T > __always_inline void constexpr
  add_PA_QTW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0;
    c0 = a0 + b0;
    t0 = a1 + b1;
    t0 = t0 + b2;
    c0 = c0 + t0;
  }

  // add: 2-3-2
  template < typename T > __always_inline void constexpr
  add_PA_QTW_PA( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + b1;
    t0 = t0 + b2;
    c1 = c1 + t0;
  }

  // add: 2-3-3
  template < typename T > __always_inline void constexpr
  add_PA_QTW_QTW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    c2 = c2 + t0 + b2;
  }

  // add: 2-3-4
  template < typename T > __always_inline void constexpr
  add_PA_QTW_QQW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, b2, c2, t1 );
    c3 = t0 + t1;
  }

  // add: 2-4-1
  template < typename T > __always_inline void constexpr
  add_PA_QQW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0;
    c0 = a0 + b0;
    t0 = a1 + b1;
    t0 = t0 + b2 + b3;
    c0 = c0 + t0;
  }

  // add: 2-4-2
  template < typename T > __always_inline void constexpr
  add_PA_QQW_PA( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + b1;
    t0 = t0 + b2 + b3;
    c1 = c1 + t0;
  }

  // add: 2-4-3
  template < typename T > __always_inline void constexpr
  add_PA_QQW_QTW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    c2 = c2 + t0 + b2 + b3;
  }

  // add: 2-4-4
  template < typename T > __always_inline void constexpr
  add_PA_QQW_QQW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, b2, c2, t1 );
    c3 = t0 + t1 + b3;
  }

  // add: 3-1-1
  template < typename T > __always_inline void constexpr
  add_QTW_SW_SW( T const a0, T const a1, T const a2, T const b0, T &c0 )
  {
    T t0;
    c0 = a0 + b0;
    t0 = a1 + a2;
    c0 = c0 + t0;
  }

  // add: 3-1-2
  template < typename T > __always_inline void constexpr
  add_QTW_SW_PA( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + a2;
    c1 = c1 + t0;
  }

  // add: 3-1-3
  template < typename T > __always_inline void constexpr
  add_QTW_SW_QTW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, t0 );
    c2 = t0 + a2;
  }

  // add: 3-1-4
  template < typename T > __always_inline void constexpr
  add_QTW_SW_QQW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, t0 );
    TwoSum( t0, a2, c2, c3 );
  }

  // add: 3-2-1
  template < typename T > __always_inline void constexpr
  add_QTW_PA_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0 )
  {
    T t0;
    c0 = a0 + b0;
    t0 = a1 + b1;
    t0 = t0 + a2;
    c0 = c0 + t0;
  }

  // add: 3-2-2
  template < typename T > __always_inline void constexpr
  add_QTW_PA_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + b1;
    t0 = t0 + a2;
    c1 = c1 + t0;
  }

  // add: 3-2-3
  template < typename T > __always_inline void constexpr
  add_QTW_PA_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    c2 = c2 + t0 + a2;
  }

  // add: 3-2-4
  template < typename T > __always_inline void constexpr
  add_QTW_PA_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, a2, c2, t1 );
    c3 = t0 + t1;
  }

  // add: 3-3-1
  template < typename T > __always_inline void constexpr
  add_QTW_QTW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0, t1;
    c0 = a0 + b0;
    t0 = a1 + b1;
    t1 = a2 + b2;
    t0 = t0 + t1;
    c0 = c0 + t0;
  }

  // add: 3-3-2
  template < typename T > __always_inline void constexpr
  add_QTW_QTW_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + b1;
    t1 = a2 + b2;
    t0 = t0 + t1;
    c1 = c1 + t0;
  }

  // add: 3-3-3
  template < typename T > __always_inline void constexpr
  add_QTW_QTW_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    t1 = a2 + b2;
    c2 = c2 + t0 + t1;
  }

  // add: 3-3-4
  template < typename T > __always_inline void constexpr
  add_QTW_QTW_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( a2, b2, t1, c3 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    c3 = c3 + t0 + t1;
  }

  // add: 3-4-1
  template < typename T > __always_inline void constexpr
  add_QTW_QQW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0, t1;
    c0 = a0 + b0;
    t0 = a1 + b1;
    t1 = a2 + b2;
    t0 = t0 + t1 + b3;
    c0 = c0 + t0;
  }

  // add: 3-4-2
  template < typename T > __always_inline void constexpr
  add_QTW_QQW_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + b1;
    t1 = a2 + b2;
    t0 = t0 + t1 + b3;
    c1 = c1 + t0;
  }

  // add: 3-4-3
  template < typename T > __always_inline void constexpr
  add_QTW_QQW_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    t1 = a2 + b2;
    c2 = c2 + t0 + t1 + b3;
  }

  // add: 3-4-4
  template < typename T > __always_inline void constexpr
  add_QTW_QQW_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( a2, b2, t1, c3 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    c3 = c3 + t0 + t1 + b3;
  }

  // add: 4-1-1
  template < typename T > __always_inline void constexpr
  add_QQW_SW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0 )
  {
    T t0;
    c0 = a0 + b0;
    t0 = a1 + a2 + a3;
    c0 = c0 + t0;
  }

  // add: 4-1-2
  template < typename T > __always_inline void constexpr
  add_QQW_SW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + a2 + a3;
    c1 = c1 + t0;
  }

  // add: 4-1-3
  template < typename T > __always_inline void constexpr
  add_QQW_SW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, t0 );
    c2 = t0 + a2 + a3;
  }

  // add: 4-1-4
  template < typename T > __always_inline void constexpr
  add_QQW_SW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, t0 );
    TwoSum( t0, a2, c2, t1 );
    c3 = t1 + a3;
  }

  // add: 4-2-1
  template < typename T > __always_inline void constexpr
  add_QQW_PA_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0 )
  {
    T t0;
    c0 = a0 + b0;
    t0 = a1 + b1;
    t0 = t0 + a2 + a3;
    c0 = c0 + t0;
  }

  // add: 4-2-2
  template < typename T > __always_inline void constexpr
  add_QQW_PA_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + b1;
    t0 = t0 + a2 + a3;
    c1 = c1 + t0;
  }

  // add: 4-2-3
  template < typename T > __always_inline void constexpr
  add_QQW_PA_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    c2 = c2 + t0 + a2 + a3;
  }

  // add: 4-2-4
  template < typename T > __always_inline void constexpr
  add_QQW_PA_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, a2, c2, t1 );
    c3 = t0 + t1 + a3;
  }

  // add: 4-3-1
  template < typename T > __always_inline void constexpr
  add_QQW_QTW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0, t1;
    c0 = a0 + b0;
    t0 = a1 + b1;
    t1 = a2 + b2;
    t0 = t0 + t1 + a3;
    c0 = c0 + t0;
  }

  // add: 4-3-2
  template < typename T > __always_inline void constexpr
  add_QQW_QTW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + b1;
    t1 = a2 + b2;
    t0 = t0 + t1 + a3;
    c1 = c1 + t0;
  }

  // add: 4-3-3
  template < typename T > __always_inline void constexpr
  add_QQW_QTW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    t1 = a2 + b2;
    c2 = c2 + t0 + t1 + a3;
  }

  // add: 4-3-4
  template < typename T > __always_inline void constexpr
  add_QQW_QTW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( a2, b2, t1, c3 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    c3 = c3 + t0 + t1 + a3;
  }

  // add: 4-4-1
  template < typename T > __always_inline void constexpr
  add_QQW_QQW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0, t1, t2;
    c0 = a0 + b0;
    t0 = a1 + b1;
    t1 = a2 + b2;
    t2 = a3 + b3;
    t0 = t0 + t1 + t2;
    c0 = c0 + t0;
  }

  // add: 4-4-2
  template < typename T > __always_inline void constexpr
  add_QQW_QQW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + b1;
    t1 = a2 + b2;
    t2 = a3 + b3;
    t0 = t0 + t1 + t2;
    c1 = c1 + t0;
  }

  // add: 4-4-3
  template < typename T > __always_inline void constexpr
  add_QQW_QQW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    t1 = a2 + b2;
    t2 = a3 + b3;
    c2 = c2 + t0 + t1 + t2;
  }

  // add: 4-4-4
  template < typename T > __always_inline void constexpr
  add_QQW_QQW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( a2, b2, t1, c3 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    t2 = a3 + b3;
    c3 = c3 + t0 + t1 + t2;
  }

  // sub: 1-1-1
  template < typename T > __always_inline void constexpr
  sub_SW_SW_SW( T const a0, T const b0, T &c0 )
  {
    add_SW_SW_SW( a0, -b0, c0 );
  }

  // sub: 1-1-2
  template < typename T > __always_inline void constexpr
  sub_SW_SW_PA( T const a0, T const b0, T &c0, T &c1 )
  {
    add_SW_SW_PA( a0, -b0, c0, c1 );
  }

  // sub: 1-1-3
  template < typename T > __always_inline void constexpr
  sub_SW_SW_QTW( T const a0, T const b0, T &c0, T &c1, T &c2 )
  {
    add_SW_SW_QTW( a0, -b0, c0, c1, c2 );
  }

  // sub: 1-1-4
  template < typename T > __always_inline void constexpr
  sub_SW_SW_QQW( T const a0, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    add_SW_SW_QQW( a0, -b0, c0, c1, c2, c3 );
  }

  // sub: 1-2-1
  template < typename T > __always_inline void constexpr
  sub_SW_PA_SW( T const a0, T const b0, T const b1, T &c0 )
  {
    add_SW_PA_SW( a0, -b0, -b1, c0 );
  }

  // sub: 1-2-2
  template < typename T > __always_inline void constexpr
  sub_SW_PA_PA( T const a0, T const b0, T const b1, T &c0, T &c1 )
  {
    add_SW_PA_PA( a0, -b0, -b1, c0, c1 );
  }

  // sub: 1-2-3
  template < typename T > __always_inline void constexpr
  sub_SW_PA_QTW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    add_SW_PA_QTW( a0, -b0, -b1, c0, c1, c2 );
  }

  // sub: 1-2-4
  template < typename T > __always_inline void constexpr
  sub_SW_PA_QQW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    add_SW_PA_QQW( a0, -b0, -b1, c0, c1, c2, c3 );
  }

  // sub: 1-3-1
  template < typename T > __always_inline void constexpr
  sub_SW_QTW_SW( T const a0, T const b0, T const b1, T const b2, T &c0 )
  {
    add_SW_QTW_SW( a0, -b0, -b1, -b2, c0 );
  }

  // sub: 1-3-2
  template < typename T > __always_inline void constexpr
  sub_SW_QTW_PA( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    add_SW_QTW_PA( a0, -b0, -b1, -b2, c0, c1 );
  }

  // sub: 1-3-3
  template < typename T > __always_inline void constexpr
  sub_SW_QTW_QTW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    add_SW_QTW_QTW( a0, -b0, -b1, -b2, c0, c1, c2 );
  }

  // sub: 1-3-4
  template < typename T > __always_inline void constexpr
  sub_SW_QTW_QQW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    add_SW_QTW_QQW( a0, -b0, -b1, -b2, c0, c1, c2, c3 );
  }

  // sub: 1-4-1
  template < typename T > __always_inline void constexpr
  sub_SW_QQW_SW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    add_SW_QQW_SW( a0, -b0, -b1, -b2, -b3, c0 );
  }

  // sub: 1-4-2
  template < typename T > __always_inline void constexpr
  sub_SW_QQW_PA( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    add_SW_QQW_PA( a0, -b0, -b1, -b2, -b3, c0, c1 );
  }

  // sub: 1-4-3
  template < typename T > __always_inline void constexpr
  sub_SW_QQW_QTW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    add_SW_QQW_QTW( a0, -b0, -b1, -b2, -b3, c0, c1, c2 );
  }

  // sub: 1-4-4
  template < typename T > __always_inline void constexpr
  sub_SW_QQW_QQW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    add_SW_QQW_QQW( a0, -b0, -b1, -b2, -b3, c0, c1, c2, c3 );
  }

  // sub: 2-1-1
  template < typename T > __always_inline void constexpr
  sub_PA_SW_SW( T const a0, T const a1, T const b0, T &c0 )
  {
    add_PA_SW_SW( a0, a1, -b0, c0 );
  }

  // sub: 2-1-2
  template < typename T > __always_inline void constexpr
  sub_PA_SW_PA( T const a0, T const a1, T const b0, T &c0, T &c1 )
  {
    add_PA_SW_PA( a0, a1, -b0, c0, c1 );
  }

  // sub: 2-1-3
  template < typename T > __always_inline void constexpr
  sub_PA_SW_QTW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2 )
  {
    add_PA_SW_QTW( a0, a1, -b0, c0, c1, c2 );
  }

  // sub: 2-1-4
  template < typename T > __always_inline void constexpr
  sub_PA_SW_QQW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    add_PA_SW_QQW( a0, a1, -b0, c0, c1, c2, c3 );
  }

  // sub: 2-2-1
  template < typename T > __always_inline void constexpr
  sub_PA_PA_SW( T const a0, T const a1, T const b0, T const b1, T &c0 )
  {
    add_PA_PA_SW( a0, a1, -b0, -b1, c0 );
  }

  // sub: 2-2-2
  template < typename T > __always_inline void constexpr
  sub_PA_PA_PA( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1 )
  {
    add_PA_PA_PA( a0, a1, -b0, -b1, c0, c1 );
  }

  // sub: 2-2-3
  template < typename T > __always_inline void constexpr
  sub_PA_PA_QTW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    add_PA_PA_QTW( a0, a1, -b0, -b1, c0, c1, c2 );
  }

  // sub: 2-2-4
  template < typename T > __always_inline void constexpr
  sub_PA_PA_QQW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    add_PA_PA_QQW( a0, a1, -b0, -b1, c0, c1, c2, c3 );
  }

  // sub: 2-3-1
  template < typename T > __always_inline void constexpr
  sub_PA_QTW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0 )
  {
    add_PA_QTW_SW( a0, a1, -b0, -b1, -b2, c0 );
  }

  // sub: 2-3-2
  template < typename T > __always_inline void constexpr
  sub_PA_QTW_PA( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    add_PA_QTW_PA( a0, a1, -b0, -b1, -b2, c0, c1 );
  }

  // sub: 2-3-3
  template < typename T > __always_inline void constexpr
  sub_PA_QTW_QTW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    add_PA_QTW_QTW( a0, a1, -b0, -b1, -b2, c0, c1, c2 );
  }

  // sub: 2-3-4
  template < typename T > __always_inline void constexpr
  sub_PA_QTW_QQW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    add_PA_QTW_QQW( a0, a1, -b0, -b1, -b2, c0, c1, c2, c3 );
  }

  // sub: 2-4-1
  template < typename T > __always_inline void constexpr
  sub_PA_QQW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    add_PA_QQW_SW( a0, a1, -b0, -b1, -b2, -b3, c0 );
  }

  // sub: 2-4-2
  template < typename T > __always_inline void constexpr
  sub_PA_QQW_PA( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    add_PA_QQW_PA( a0, a1, -b0, -b1, -b2, -b3, c0, c1 );
  }

  // sub: 2-4-3
  template < typename T > __always_inline void constexpr
  sub_PA_QQW_QTW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    add_PA_QQW_QTW( a0, a1, -b0, -b1, -b2, -b3, c0, c1, c2 );
  }

  // sub: 2-4-4
  template < typename T > __always_inline void constexpr
  sub_PA_QQW_QQW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    add_PA_QQW_QQW( a0, a1, -b0, -b1, -b2, -b3, c0, c1, c2, c3 );
  }

  // sub: 3-1-1
  template < typename T > __always_inline void constexpr
  sub_QTW_SW_SW( T const a0, T const a1, T const a2, T const b0, T &c0 )
  {
    add_QTW_SW_SW( a0, a1, a2, -b0, c0 );
  }

  // sub: 3-1-2
  template < typename T > __always_inline void constexpr
  sub_QTW_SW_PA( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1 )
  {
    add_QTW_SW_PA( a0, a1, a2, -b0, c0, c1 );
  }

  // sub: 3-1-3
  template < typename T > __always_inline void constexpr
  sub_QTW_SW_QTW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2 )
  {
    add_QTW_SW_QTW( a0, a1, a2, -b0, c0, c1, c2 );
  }

  // sub: 3-1-4
  template < typename T > __always_inline void constexpr
  sub_QTW_SW_QQW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    add_QTW_SW_QQW( a0, a1, a2, -b0, c0, c1, c2, c3 );
  }

  // sub: 3-2-1
  template < typename T > __always_inline void constexpr
  sub_QTW_PA_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0 )
  {
    add_QTW_PA_SW( a0, a1, a2, -b0, -b1, c0 );
  }

  // sub: 3-2-2
  template < typename T > __always_inline void constexpr
  sub_QTW_PA_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1 )
  {
    add_QTW_PA_PA( a0, a1, a2, -b0, -b1, c0, c1 );
  }

  // sub: 3-2-3
  template < typename T > __always_inline void constexpr
  sub_QTW_PA_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    add_QTW_PA_QTW( a0, a1, a2, -b0, -b1, c0, c1, c2 );
  }

  // sub: 3-2-4
  template < typename T > __always_inline void constexpr
  sub_QTW_PA_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    add_QTW_PA_QQW( a0, a1, a2, -b0, -b1, c0, c1, c2, c3 );
  }

  // sub: 3-3-1
  template < typename T > __always_inline void constexpr
  sub_QTW_QTW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0 )
  {
    add_QTW_QTW_SW( a0, a1, a2, -b0, -b1, -b2, c0 );
  }

  // sub: 3-3-2
  template < typename T > __always_inline void constexpr
  sub_QTW_QTW_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    add_QTW_QTW_PA( a0, a1, a2, -b0, -b1, -b2, c0, c1 );
  }

  // sub: 3-3-3
  template < typename T > __always_inline void constexpr
  sub_QTW_QTW_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    add_QTW_QTW_QTW( a0, a1, a2, -b0, -b1, -b2, c0, c1, c2 );
  }

  // sub: 3-3-4
  template < typename T > __always_inline void constexpr
  sub_QTW_QTW_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    add_QTW_QTW_QQW( a0, a1, a2, -b0, -b1, -b2, c0, c1, c2, c3 );
  }

  // sub: 3-4-1
  template < typename T > __always_inline void constexpr
  sub_QTW_QQW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    add_QTW_QQW_SW( a0, a1, a2, -b0, -b1, -b2, -b3, c0 );
  }

  // sub: 3-4-2
  template < typename T > __always_inline void constexpr
  sub_QTW_QQW_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    add_QTW_QQW_PA( a0, a1, a2, -b0, -b1, -b2, -b3, c0, c1 );
  }

  // sub: 3-4-3
  template < typename T > __always_inline void constexpr
  sub_QTW_QQW_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    add_QTW_QQW_QTW( a0, a1, a2, -b0, -b1, -b2, -b3, c0, c1, c2 );
  }

  // sub: 3-4-4
  template < typename T > __always_inline void constexpr
  sub_QTW_QQW_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    add_QTW_QQW_QQW( a0, a1, a2, -b0, -b1, -b2, -b3, c0, c1, c2, c3 );
  }

  // sub: 4-1-1
  template < typename T > __always_inline void constexpr
  sub_QQW_SW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0 )
  {
    add_QQW_SW_SW( a0, a1, a2, a3, -b0, c0 );
  }

  // sub: 4-1-2
  template < typename T > __always_inline void constexpr
  sub_QQW_SW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1 )
  {
    add_QQW_SW_PA( a0, a1, a2, a3, -b0, c0, c1 );
  }

  // sub: 4-1-3
  template < typename T > __always_inline void constexpr
  sub_QQW_SW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2 )
  {
    add_QQW_SW_QTW( a0, a1, a2, a3, -b0, c0, c1, c2 );
  }

  // sub: 4-1-4
  template < typename T > __always_inline void constexpr
  sub_QQW_SW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    add_QQW_SW_QQW( a0, a1, a2, a3, -b0, c0, c1, c2, c3 );
  }

  // sub: 4-2-1
  template < typename T > __always_inline void constexpr
  sub_QQW_PA_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0 )
  {
    add_QQW_PA_SW( a0, a1, a2, a3, -b0, -b1, c0 );
  }

  // sub: 4-2-2
  template < typename T > __always_inline void constexpr
  sub_QQW_PA_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1 )
  {
    add_QQW_PA_PA( a0, a1, a2, a3, -b0, -b1, c0, c1 );
  }

  // sub: 4-2-3
  template < typename T > __always_inline void constexpr
  sub_QQW_PA_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    add_QQW_PA_QTW( a0, a1, a2, a3, -b0, -b1, c0, c1, c2 );
  }

  // sub: 4-2-4
  template < typename T > __always_inline void constexpr
  sub_QQW_PA_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    add_QQW_PA_QQW( a0, a1, a2, a3, -b0, -b1, c0, c1, c2, c3 );
  }

  // sub: 4-3-1
  template < typename T > __always_inline void constexpr
  sub_QQW_QTW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0 )
  {
    add_QQW_QTW_SW( a0, a1, a2, a3, -b0, -b1, -b2, c0 );
  }

  // sub: 4-3-2
  template < typename T > __always_inline void constexpr
  sub_QQW_QTW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    add_QQW_QTW_PA( a0, a1, a2, a3, -b0, -b1, -b2, c0, c1 );
  }

  // sub: 4-3-3
  template < typename T > __always_inline void constexpr
  sub_QQW_QTW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    add_QQW_QTW_QTW( a0, a1, a2, a3, -b0, -b1, -b2, c0, c1, c2 );
  }

  // sub: 4-3-4
  template < typename T > __always_inline void constexpr
  sub_QQW_QTW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    add_QQW_QTW_QQW( a0, a1, a2, a3, -b0, -b1, -b2, c0, c1, c2, c3 );
  }

  // sub: 4-4-1
  template < typename T > __always_inline void constexpr
  sub_QQW_QQW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    add_QQW_QQW_SW( a0, a1, a2, a3, -b0, -b1, -b2, -b3, c0 );
  }

  // sub: 4-4-2
  template < typename T > __always_inline void constexpr
  sub_QQW_QQW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    add_QQW_QQW_PA( a0, a1, a2, a3, -b0, -b1, -b2, -b3, c0, c1 );
  }

  // sub: 4-4-3
  template < typename T > __always_inline void constexpr
  sub_QQW_QQW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    add_QQW_QQW_QTW( a0, a1, a2, a3, -b0, -b1, -b2, -b3, c0, c1, c2 );
  }

  // sub: 4-4-4
  template < typename T > __always_inline void constexpr
  sub_QQW_QQW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    add_QQW_QQW_QQW( a0, a1, a2, a3, -b0, -b1, -b2, -b3, c0, c1, c2, c3 );
  }

  // mul: 1-1-1
  template < typename T > __always_inline void constexpr
  mul_SW_SW_SW( T const a0, T const b0, T &c0 )
  {
    c0 = a0 * b0;
  }

  // mul: 1-1-2
  template < typename T > __always_inline void constexpr
  mul_SW_SW_PA( T const a0, T const b0, T &c0, T &c1 )
  {
    TwoProductFMA( a0, b0, c0, c1 );
  }

  // mul: 1-1-3
  template < typename T > __always_inline void constexpr
  mul_SW_SW_QTW( T const a0, T const b0, T &c0, T &c1, T &c2 )
  {
    TwoProductFMA( a0, b0, c0, c1 );
    c2 = fp_const<T>::zero();
  }

  // mul: 1-1-4
  template < typename T > __always_inline void constexpr
  mul_SW_SW_QQW( T const a0, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    TwoProductFMA( a0, b0, c0, c1 );
    c2 = fp_const<T>::zero();
    c3 = fp_const<T>::zero();
  }

  // mul: 1-2-1
  template < typename T > __always_inline void constexpr
  mul_SW_PA_SW( T const a0, T const b0, T const b1, T &c0 )
  {
    T t0;
    t0 = b0 + b1;
    c0 = a0 * t0;
  }

  // mul: 1-2-2
  template < typename T > __always_inline void constexpr
  mul_SW_PA_PA( T const a0, T const b0, T const b1, T &c0, T &c1 )
  {
    TwoProductFMA( a0, b0, c0, c1 );
    c1 = std::fma ( a0, b1, c1 );
  }

  // mul: 1-2-3
  template < typename T > __always_inline void constexpr
  mul_SW_PA_QTW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    c2 = c2 + t0;
  }

  // mul: 1-2-4
  template < typename T > __always_inline void constexpr
  mul_SW_PA_QQW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c2, c3, c2, c3 );
  }

  // mul: 1-3-1
  template < typename T > __always_inline void constexpr
  mul_SW_QTW_SW( T const a0, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0;
    t0 = b0 + b1 + b2;
    c0 = a0 * t0;
  }

  // mul: 1-3-2
  template < typename T > __always_inline void constexpr
  mul_SW_QTW_PA( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = b1 + b2;
    c1 = std::fma ( a0, t0, c1 );
  }

  // mul: 1-3-3
  template < typename T > __always_inline void constexpr
  mul_SW_QTW_QTW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    c2 = c2 + t0;
    c2 = std::fma ( a0, b2, c2 );
  }

  // mul: 1-3-4
  template < typename T > __always_inline void constexpr
  mul_SW_QTW_QQW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoSum( c1, c2, c1, c2 );
    TwoProductFMA( a0, b2, t0, t1 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t0, c2, t0 );
    c3 = c3 + t0 + t1;
  }

  // mul: 1-4-1
  template < typename T > __always_inline void constexpr
  mul_SW_QQW_SW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0;
    t0 = b0 + b1 + b2 + b3;
    c0 = a0 * t0;
  }

  // mul: 1-4-2
  template < typename T > __always_inline void constexpr
  mul_SW_QQW_PA( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = b1 + b2 + b3;
    c1 = std::fma ( a0, t0, c1 );
  }

  // mul: 1-4-3
  template < typename T > __always_inline void constexpr
  mul_SW_QQW_QTW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    c2 = c2 + t0;
    t1 = b3 + b2;
    c2 = std::fma ( a0, t1, c2 );
  }

  // mul: 1-4-4
  template < typename T > __always_inline void constexpr
  mul_SW_QQW_QQW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoSum( c1, c2, c1, c2 );
    TwoProductFMA( a0, b2, t0, t1 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t0, c2, t0 );
    c3 = c3 + t0 + t1;
    c3 = std::fma ( a0, b3, c3 );
  }

  // mul: 2-1-1
  template < typename T > __always_inline void constexpr
  mul_PA_SW_SW( T const a0, T const a1, T const b0, T &c0 )
  {
    T t0;
    t0 = a0 + a1;
    c0 = t0 * b0;
  }

  // mul: 2-1-2
  template < typename T > __always_inline void constexpr
  mul_PA_SW_PA( T const a0, T const a1, T const b0, T &c0, T &c1 )
  {
    TwoProductFMA( a0, b0, c0, c1 );
    c1 = std::fma ( b0, a1, c1 );
  }

  // mul: 2-1-3
  template < typename T > __always_inline void constexpr
  mul_PA_SW_QTW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2 )
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    c2 = t0 + t1;
  }

  // mul: 2-1-4
  template < typename T > __always_inline void constexpr
  mul_PA_SW_QQW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, t0, c3 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( t0, c3, c2, c3 );
  }

  // mul: 2-2-1
  template < typename T > __always_inline void constexpr
  mul_PA_PA_SW( T const a0, T const a1, T const b0, T const b1, T &c0 )
  {
    T t0, t1;
    t0 = a0 + a1;
    t1 = b0 + b1;
    c0 = t0 * t1;
  }

  // mul: 2-2-2
  template < typename T > __always_inline void constexpr
  mul_PA_PA_PA( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1 )
  {
    TwoProductFMA( a0, b0, c0, c1 );
    c1 = std::fma ( a0, b1, c1 );
    c1 = std::fma ( b0, a1, c1 );
  }

  // mul: 2-2-3
  template < typename T > __always_inline void constexpr
  mul_PA_PA_QTW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    c2 = c2 + t1 + t0 + t2;
    c2 = std::fma ( a1, b1, c2 );
  }

  // mul: 2-2-4
  template < typename T > __always_inline void constexpr
  mul_PA_PA_QQW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoProductFMA( a1, b1, t2, t3 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    c3 = c3 + t0 + t1 + t2 + t3;
  }

  // mul: 2-3-1
  template < typename T > __always_inline void constexpr
  mul_PA_QTW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0, t1;
    t0 = a0 + a1;
    t1 = b0 + b1 + b2;
    c0 = t0 * t1;
  }

  // mul: 2-3-2
  template < typename T > __always_inline void constexpr
  mul_PA_QTW_PA( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = b1 + b2;
    c1 = std::fma ( a0, t0, c1 );
    c1 = std::fma ( b0, a1, c1 );
  }

  // mul: 2-3-3
  template < typename T > __always_inline void constexpr
  mul_PA_QTW_QTW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    c2 = c2 + t1 + t0 + t2;
    c2 = std::fma ( a0, b2, c2 );
    c2 = std::fma ( a1, b1, c2 );
  }

  // mul: 2-3-4
  template < typename T > __always_inline void constexpr
  mul_PA_QTW_QQW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoProductFMA( a0, b2, t2, t3 );
    TwoProductFMA( a1, b1, t4, t5 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t4, c2, t4 );
    c3 = c3 + t0 + t1 + t2 + t3 + t4 + t5;
    c3 = std::fma ( a1, b2, c3 );
  }

  // mul: 2-4-1
  template < typename T > __always_inline void constexpr
  mul_PA_QQW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0, t1;
    t0 = a0 + a1;
    t1 = b0 + b1 + b2 + b3;
    c0 = t0 * t1;
  }

  // mul: 2-4-2
  template < typename T > __always_inline void constexpr
  mul_PA_QQW_PA( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = b1 + b2 + b3;
    c1 = std::fma ( a0, t0, c1 );
    c1 = std::fma ( b0, a1, c1 );
  }

  // mul: 2-4-3
  template < typename T > __always_inline void constexpr
  mul_PA_QQW_QTW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    c2 = c2 + t1 + t0 + t2;
    t3 = b3 + b2;
    c2 = std::fma ( a0, t3, c2 );
    c2 = std::fma ( a1, b1, c2 );
  }

  // mul: 2-4-4
  template < typename T > __always_inline void constexpr
  mul_PA_QQW_QQW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoProductFMA( a0, b2, t2, t3 );
    TwoProductFMA( a1, b1, t4, t5 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t4, c2, t4 );
    c3 = c3 + t0 + t1 + t2 + t3 + t4 + t5;
    c3 = std::fma ( a0, b3, c3 );
    c3 = std::fma ( a1, b2, c3 );
  }

  // mul: 3-1-1
  template < typename T > __always_inline void constexpr
  mul_QTW_SW_SW( T const a0, T const a1, T const a2, T const b0, T &c0 )
  {
    T t0;
    t0 = a0 + a1 + a2;
    c0 = t0 * b0;
  }

  // mul: 3-1-2
  template < typename T > __always_inline void constexpr
  mul_QTW_SW_PA( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1 )
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = a1 + a2;
    c1 = std::fma ( b0, t0, c1 );
  }

  // mul: 3-1-3
  template < typename T > __always_inline void constexpr
  mul_QTW_SW_QTW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2 )
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    c2 = t0 + t1;
    c2 = std::fma ( a2, b0, c2 );
  }

  // mul: 3-1-4
  template < typename T > __always_inline void constexpr
  mul_QTW_SW_QQW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    TwoProductFMA( a2, b0, t2, t3 );
    TwoSum( t0, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    c3 = t1 + t2 + t3;
  }

  // mul: 3-2-1
  template < typename T > __always_inline void constexpr
  mul_QTW_PA_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0 )
  {
    T t0, t1;
    t0 = a0 + a1 + a2;
    t1 = b0 + b1;
    c0 = t0 * t1;
  }

  // mul: 3-2-2
  template < typename T > __always_inline void constexpr
  mul_QTW_PA_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = a1 + a2;
    c1 = std::fma ( a0, b1, c1 );
    c1 = std::fma ( b0, t0, c1 );
  }

  // mul: 3-2-3
  template < typename T > __always_inline void constexpr
  mul_QTW_PA_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    c2 = c2 + t1 + t0 + t2;
    c2 = std::fma ( a1, b1, c2 );
    c2 = std::fma ( a2, b0, c2 );
  }

  // mul: 3-2-4
  template < typename T > __always_inline void constexpr
  mul_QTW_PA_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoProductFMA( a1, b1, t2, t3 );
    TwoProductFMA( a2, b0, t4, t5 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t4, c2, t4 );
    c3 = c3 + t0 + t1 + t2 + t3 + t4 + t5;
    c3 = std::fma ( a2, b1, c3 );
  }

  // mul: 3-3-1
  template < typename T > __always_inline void constexpr
  mul_QTW_QTW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0, t1;
    t0 = a0 + a1 + a2;
    t1 = b0 + b1 + b2;
    c0 = t0 * t1;
  }

  // mul: 3-3-2
  template < typename T > __always_inline void constexpr
  mul_QTW_QTW_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = a1 + a2;
    t1 = b1 + b2;
    c1 = std::fma ( a0, t1, c1 );
    c1 = std::fma ( b0, t0, c1 );
  }

  // mul: 3-3-3
  template < typename T > __always_inline void constexpr
  mul_QTW_QTW_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    c2 = c2 + t1 + t0 + t2;
    c2 = std::fma ( a0, b2, c2 );
    c2 = std::fma ( a1, b1, c2 );
    c2 = std::fma ( a2, b0, c2 );
  }

  // mul: 3-3-4
  template < typename T > __always_inline void constexpr
  mul_QTW_QTW_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5, t6, t7;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoProductFMA( a0, b2, t2, t3 );
    TwoProductFMA( a1, b1, t4, t5 );
    TwoProductFMA( a2, b0, t6, t7 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t4, c2, t4 );
    TwoSum( c2, t6, c2, t6 );
    c3 = c3 + t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7;
    c3 = std::fma ( a1, b2, c3 );
    c3 = std::fma ( a2, b1, c3 );
  }

  // mul: 3-4-1
  template < typename T > __always_inline void constexpr
  mul_QTW_QQW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0, t1;
    t0 = a0 + a1 + a2;
    t1 = b0 + b1 + b2 + b3;
    c0 = t0 * t1;
  }

  // mul: 3-4-2
  template < typename T > __always_inline void constexpr
  mul_QTW_QQW_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = a1 + a2;
    t1 = b1 + b2 + b3;
    c1 = std::fma ( a0, t1, c1 );
    c1 = std::fma ( b0, t0, c1 );
  }

  // mul: 3-4-3
  template < typename T > __always_inline void constexpr
  mul_QTW_QQW_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    c2 = c2 + t1 + t0 + t2;
    t3 = b3 + b2;
    c2 = std::fma ( a0, t3, c2 );
    c2 = std::fma ( a1, b1, c2 );
    c2 = std::fma ( a2, b0, c2 );
  }

  // mul: 3-4-4
  template < typename T > __always_inline void constexpr
  mul_QTW_QQW_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5, t6, t7;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoProductFMA( a0, b2, t2, t3 );
    TwoProductFMA( a1, b1, t4, t5 );
    TwoProductFMA( a2, b0, t6, t7 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t4, c2, t4 );
    TwoSum( c2, t6, c2, t6 );
    c3 = c3 + t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7;
    c3 = std::fma ( a0, b3, c3 );
    c3 = std::fma ( a1, b2, c3 );
    c3 = std::fma ( a2, b1, c3 );
  }

  // mul: 4-1-1
  template < typename T > __always_inline void constexpr
  mul_QQW_SW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0 )
  {
    T t0;
    t0 = a0 + a1 + a2 + a3;
    c0 = t0 * b0;
  }

  // mul: 4-1-2
  template < typename T > __always_inline void constexpr
  mul_QQW_SW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1 )
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = a1 + a2 + a3;
    c1 = std::fma ( b0, t0, c1 );
  }

  // mul: 4-1-3
  template < typename T > __always_inline void constexpr
  mul_QQW_SW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    c2 = t0 + t1;
    t2 = a3 + a2;
    c2 = std::fma ( t2, b0, c2 );
  }

  // mul: 4-1-4
  template < typename T > __always_inline void constexpr
  mul_QQW_SW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    TwoProductFMA( a2, b0, t2, t3 );
    TwoSum( t0, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    c3 = t1 + t2 + t3;
    c3 = std::fma ( a3, b0, c3 );
  }

  // mul: 4-2-1
  template < typename T > __always_inline void constexpr
  mul_QQW_PA_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0 )
  {
    T t0, t1;
    t0 = a0 + a1 + a2 + a3;
    t1 = b0 + b1;
    c0 = t0 * t1;
  }

  // mul: 4-2-2
  template < typename T > __always_inline void constexpr
  mul_QQW_PA_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = a1 + a2 + a3;
    c1 = std::fma ( a0, b1, c1 );
    c1 = std::fma ( b0, t0, c1 );
  }

  // mul: 4-2-3
  template < typename T > __always_inline void constexpr
  mul_QQW_PA_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    c2 = c2 + t1 + t0 + t2;
    t3 = a3 + a2;
    c2 = std::fma ( a1, b1, c2 );
    c2 = std::fma ( t3, b0, c2 );
  }

  // mul: 4-2-4
  template < typename T > __always_inline void constexpr
  mul_QQW_PA_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoProductFMA( a1, b1, t2, t3 );
    TwoProductFMA( a2, b0, t4, t5 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t4, c2, t4 );
    c3 = c3 + t0 + t1 + t2 + t3 + t4 + t5;
    c3 = std::fma ( a2, b1, c3 );
    c3 = std::fma ( a3, b0, c3 );
  }

  // mul: 4-3-1
  template < typename T > __always_inline void constexpr
  mul_QQW_QTW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0, t1;
    t0 = a0 + a1 + a2 + a3;
    t1 = b0 + b1 + b2;
    c0 = t0 * t1;
  }

  // mul: 4-3-2
  template < typename T > __always_inline void constexpr
  mul_QQW_QTW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = a1 + a2 + a3;
    t1 = b1 + b2;
    c1 = std::fma ( a0, t1, c1 );
    c1 = std::fma ( b0, t0, c1 );
  }

  // mul: 4-3-3
  template < typename T > __always_inline void constexpr
  mul_QQW_QTW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    c2 = c2 + t1 + t0 + t2;
    t3 = a3 + a2;
    c2 = std::fma ( a0, b2, c2 );
    c2 = std::fma ( a1, b1, c2 );
    c2 = std::fma ( t3, b0, c2 );
  }

  // mul: 4-3-4
  template < typename T > __always_inline void constexpr
  mul_QQW_QTW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5, t6, t7;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoProductFMA( a0, b2, t2, t3 );
    TwoProductFMA( a1, b1, t4, t5 );
    TwoProductFMA( a2, b0, t6, t7 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t4, c2, t4 );
    TwoSum( c2, t6, c2, t6 );
    c3 = c3 + t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7;
    c3 = std::fma ( a1, b2, c3 );
    c3 = std::fma ( a2, b1, c3 );
    c3 = std::fma ( a3, b0, c3 );
  }

  // mul: 4-4-1
  template < typename T > __always_inline void constexpr
  mul_QQW_QQW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0, t1;
    t0 = a0 + a1 + a2 + a3;
    t1 = b0 + b1 + b2 + b3;
    c0 = t0 * t1;
  }

  // mul: 4-4-2
  template < typename T > __always_inline void constexpr
  mul_QQW_QQW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = a1 + a2 + a3;
    t1 = b1 + b2 + b3;
    c1 = std::fma ( a0, t1, c1 );
    c1 = std::fma ( b0, t0, c1 );
  }

  // mul: 4-4-3
  template < typename T > __always_inline void constexpr
  mul_QQW_QQW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3, t4;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    c2 = c2 + t1 + t0 + t2;
    t3 = a3 + a2;
    t4 = b3 + b2;
    c2 = std::fma ( a0, t4, c2 );
    c2 = std::fma ( a1, b1, c2 );
    c2 = std::fma ( t3, b0, c2 );
  }

  // mul: 4-4-4
  template < typename T > __always_inline void constexpr
  mul_QQW_QQW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5, t6, t7;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoProductFMA( a0, b2, t2, t3 );
    TwoProductFMA( a1, b1, t4, t5 );
    TwoProductFMA( a2, b0, t6, t7 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t4, c2, t4 );
    TwoSum( c2, t6, c2, t6 );
    c3 = c3 + t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7;
    c3 = std::fma ( a0, b3, c3 );
    c3 = std::fma ( a1, b2, c3 );
    c3 = std::fma ( a2, b1, c3 );
    c3 = std::fma ( a3, b0, c3 );
  }

  // div: 1-1-1
  template < typename T > __always_inline void constexpr
  div_SW_SW_SW( T const a0, T const b0, T &c0 )
  {
    c0 = a0 / b0;
  }

  // div: 1-1-2
  template < typename T > __always_inline void constexpr
  div_SW_SW_PA( T const a0, T const b0, T &c0, T &c1 )
  {
    T r = a0;
    c0 = r / b0;
    r = std::fma( -b0, c0, r );
    c1 = r / b0;
  }

  // div: 1-1-3
  template < typename T > __always_inline void constexpr
  div_SW_SW_QTW( T const a0, T const b0, T &c0, T &c1, T &c2 )
  {
    T r = a0;
    c0 = r / b0;
    r = std::fma( -b0, c0, r );
    c1 = r / b0;
    r = std::fma( -b0, c1, r );
    c2 = r / b0;
  }

  // div: 1-1-4
  template < typename T > __always_inline void constexpr
  div_SW_SW_QQW( T const a0, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    T r = a0;
    c0 = r / b0;
    r = std::fma( -b0, c0, r );
    c1 = r / b0;
    r = std::fma( -b0, c1, r );
    c2 = r / b0;
    r = std::fma( -b0, c2, r );
    c3 = r / b0;
  }

  // div: 1-2-1
  template < typename T > __always_inline void constexpr
  div_SW_PA_SW( T const a0, T const b0, T const b1, T &c0 )
  {
    T e40, e41;
    e40 = a0;
    e41 = b0 + b1;
    c0 = e40 / e41;
  }

  // div: 1-2-2
  template < typename T > __always_inline void constexpr
  div_SW_PA_PA( T const a0, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0, t1;
    T e40;
    e40 = b0 + b1;
    div_SW_SW_SW( a0, e40, c0 );
    mul_SW_PA_PA( c0, b0, b1, t0, t1 );
    sub_SW_PA_PA( a0, t0, t1, t0, t1 );
    T tn, td;
    tn = t0 + t1;
    td = b0;
    c1 = tn / td;
  }

  // div: 1-2-3
  template < typename T > __always_inline void constexpr
  div_SW_PA_QTW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    div_SW_PA_PA( a0, b0, b1, c0, c1 );
    mul_PA_PA_QTW( c0, c1, b0, b1, t0, t1, t2 );
    sub_SW_QTW_QTW( a0, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0;
    c2 = tn / td;
  }

  // div: 1-2-4
  template < typename T > __always_inline void constexpr
  div_SW_PA_QQW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    div_SW_PA_QTW( a0, b0, b1, c0, c1, c2 );
    mul_QTW_PA_QQW( c0, c1, c2, b0, b1, t0, t1, t2, t3 );
    sub_SW_QQW_QQW( a0, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0;
    c3 = tn / td;
  }

  // div: 1-3-1
  template < typename T > __always_inline void constexpr
  div_SW_QTW_SW( T const a0, T const b0, T const b1, T const b2, T &c0 )
  {
    T e40, e41;
    e40 = a0;
    e41 = b0 + b1 + b2;
    c0 = e40 / e41;
  }

  // div: 1-3-2
  template < typename T > __always_inline void constexpr
  div_SW_QTW_PA( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1;
    T e40;
    e40 = b0 + b1 + b2;
    div_SW_SW_SW( a0, e40, c0 );
    mul_SW_QTW_PA( c0, b0, b1, b2, t0, t1 );
    sub_SW_PA_PA( a0, t0, t1, t0, t1 );
    T tn, td;
    tn = t0 + t1;
    td = b0 + b1;
    c1 = tn / td;
  }

  // div: 1-3-3
  template < typename T > __always_inline void constexpr
  div_SW_QTW_QTW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T e40;
    e40 = b1 + b2;
    div_SW_PA_PA( a0, b0, e40, c0, c1 );
    mul_PA_QTW_QTW( c0, c1, b0, b1, b2, t0, t1, t2 );
    sub_SW_QTW_QTW( a0, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0 + b1;
    c2 = tn / td;
  }

  // div: 1-3-4
  template < typename T > __always_inline void constexpr
  div_SW_QTW_QQW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    div_SW_QTW_QTW( a0, b0, b1, b2, c0, c1, c2 );
    mul_QTW_QTW_QQW( c0, c1, c2, b0, b1, b2, t0, t1, t2, t3 );
    sub_SW_QQW_QQW( a0, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1;
    c3 = tn / td;
  }

  // div: 1-4-1
  template < typename T > __always_inline void constexpr
  div_SW_QQW_SW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T e40, e41;
    e40 = a0;
    e41 = b0 + b1 + b2 + b3;
    c0 = e40 / e41;
  }

  // div: 1-4-2
  template < typename T > __always_inline void constexpr
  div_SW_QQW_PA( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1;
    T e40;
    e40 = b0 + b1 + b2 + b3;
    div_SW_SW_SW( a0, e40, c0 );
    mul_SW_QQW_PA( c0, b0, b1, b2, b3, t0, t1 );
    sub_SW_PA_PA( a0, t0, t1, t0, t1 );
    T tn, td;
    tn = t0 + t1;
    td = b0 + b1 + b2;
    c1 = tn / td;
  }

  // div: 1-4-3
  template < typename T > __always_inline void constexpr
  div_SW_QQW_QTW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T e40;
    e40 = b1 + b2 + b3;
    div_SW_PA_PA( a0, b0, e40, c0, c1 );
    mul_PA_QQW_QTW( c0, c1, b0, b1, b2, b3, t0, t1, t2 );
    sub_SW_QTW_QTW( a0, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0 + b1 + b2;
    c2 = tn / td;
  }

  // div: 1-4-4
  template < typename T > __always_inline void constexpr
  div_SW_QQW_QQW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T e40;
    e40 = b2 + b3;
    div_SW_QTW_QTW( a0, b0, b1, e40, c0, c1, c2 );
    mul_QTW_QQW_QQW( c0, c1, c2, b0, b1, b2, b3, t0, t1, t2, t3 );
    sub_SW_QQW_QQW( a0, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1 + b2;
    c3 = tn / td;
  }

  // div: 2-1-1
  template < typename T > __always_inline void constexpr
  div_PA_SW_SW( T const a0, T const a1, T const b0, T &c0 )
  {
    T e40, e41;
    e40 = a0 + a1;
    e41 = b0;
    c0 = e40 / e41;
  }

  // div: 2-1-2
  template < typename T > __always_inline void constexpr
  div_PA_SW_PA( T const a0, T const a1, T const b0, T &c0, T &c1 )
  {
    T t0, t1;
    T e40;
    e40 = a0 + a1;
    div_SW_SW_SW( e40, b0, c0 );
    mul_SW_SW_PA( c0, b0, t0, t1 );
    sub_PA_PA_PA( a0, a1, t0, t1, t0, t1 );
    T tn, td;
    tn = t0 + t1;
    td = b0;
    c1 = tn / td;
  }

  // div: 2-1-3
  template < typename T > __always_inline void constexpr
  div_PA_SW_QTW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    div_PA_SW_PA( a0, a1, b0, c0, c1 );
    mul_PA_SW_QTW( c0, c1, b0, t0, t1, t2 );
    sub_PA_QTW_QTW( a0, a1, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0;
    c2 = tn / td;
  }

  // div: 2-1-4
  template < typename T > __always_inline void constexpr
  div_PA_SW_QQW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    div_PA_SW_QTW( a0, a1, b0, c0, c1, c2 );
    mul_QTW_SW_QQW( c0, c1, c2, b0, t0, t1, t2, t3 );
    sub_PA_QQW_QQW( a0, a1, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0;
    c3 = tn / td;
  }

  // div: 2-2-1
  template < typename T > __always_inline void constexpr
  div_PA_PA_SW( T const a0, T const a1, T const b0, T const b1, T &c0 )
  {
    T e40, e41;
    e40 = a0 + a1;
    e41 = b0 + b1;
    c0 = e40 / e41;
  }

  // div: 2-2-2
  template < typename T > __always_inline void constexpr
  div_PA_PA_PA( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1 )
  {
    c0 = a0 / (b0 + b1);
    c1 = (std::fma(-b0, c0, a0 ) + std::fma(-b1, c0, a1 )) / (b0 + b1);
  }

  // div: 2-2-3
  template < typename T > __always_inline void constexpr
  div_PA_PA_QTW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    div_PA_PA_PA( a0, a1, b0, b1, c0, c1 );
    mul_PA_PA_QTW( c0, c1, b0, b1, t0, t1, t2 );
    sub_PA_QTW_QTW( a0, a1, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0;
    c2 = tn / td;
  }

  // div: 2-2-4
  template < typename T > __always_inline void constexpr
  div_PA_PA_QQW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    div_PA_PA_QTW( a0, a1, b0, b1, c0, c1, c2 );
    mul_QTW_PA_QQW( c0, c1, c2, b0, b1, t0, t1, t2, t3 );
    sub_PA_QQW_QQW( a0, a1, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0;
    c3 = tn / td;
  }

  // div: 2-3-1
  template < typename T > __always_inline void constexpr
  div_PA_QTW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0 )
  {
    T e40, e41;
    e40 = a0 + a1;
    e41 = b0 + b1 + b2;
    c0 = e40 / e41;
  }

  // div: 2-3-2
  template < typename T > __always_inline void constexpr
  div_PA_QTW_PA( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1;
    T e40;
    e40 = a0 + a1;
    T e41;
    e41 = b0 + b1 + b2;
    div_SW_SW_SW( e40, e41, c0 );
    mul_SW_QTW_PA( c0, b0, b1, b2, t0, t1 );
    sub_PA_PA_PA( a0, a1, t0, t1, t0, t1 );
    T tn, td;
    tn = t0 + t1;
    td = b0 + b1;
    c1 = tn / td;
  }

  // div: 2-3-3
  template < typename T > __always_inline void constexpr
  div_PA_QTW_QTW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T e40;
    e40 = b1 + b2;
    div_PA_PA_PA( a0, a1, b0, e40, c0, c1 );
    mul_PA_QTW_QTW( c0, c1, b0, b1, b2, t0, t1, t2 );
    sub_PA_QTW_QTW( a0, a1, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0 + b1;
    c2 = tn / td;
  }

  // div: 2-3-4
  template < typename T > __always_inline void constexpr
  div_PA_QTW_QQW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    div_PA_QTW_QTW( a0, a1, b0, b1, b2, c0, c1, c2 );
    mul_QTW_QTW_QQW( c0, c1, c2, b0, b1, b2, t0, t1, t2, t3 );
    sub_PA_QQW_QQW( a0, a1, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1;
    c3 = tn / td;
  }

  // div: 2-4-1
  template < typename T > __always_inline void constexpr
  div_PA_QQW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T e40, e41;
    e40 = a0 + a1;
    e41 = b0 + b1 + b2 + b3;
    c0 = e40 / e41;
  }

  // div: 2-4-2
  template < typename T > __always_inline void constexpr
  div_PA_QQW_PA( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1;
    T e40;
    e40 = a0 + a1;
    T e41;
    e41 = b0 + b1 + b2 + b3;
    div_SW_SW_SW( e40, e41, c0 );
    mul_SW_QQW_PA( c0, b0, b1, b2, b3, t0, t1 );
    sub_PA_PA_PA( a0, a1, t0, t1, t0, t1 );
    T tn, td;
    tn = t0 + t1;
    td = b0 + b1 + b2;
    c1 = tn / td;
  }

  // div: 2-4-3
  template < typename T > __always_inline void constexpr
  div_PA_QQW_QTW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T e40;
    e40 = b1 + b2 + b3;
    div_PA_PA_PA( a0, a1, b0, e40, c0, c1 );
    mul_PA_QQW_QTW( c0, c1, b0, b1, b2, b3, t0, t1, t2 );
    sub_PA_QTW_QTW( a0, a1, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0 + b1 + b2;
    c2 = tn / td;
  }

  // div: 2-4-4
  template < typename T > __always_inline void constexpr
  div_PA_QQW_QQW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T e40;
    e40 = b2 + b3;
    div_PA_QTW_QTW( a0, a1, b0, b1, e40, c0, c1, c2 );
    mul_QTW_QQW_QQW( c0, c1, c2, b0, b1, b2, b3, t0, t1, t2, t3 );
    sub_PA_QQW_QQW( a0, a1, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1 + b2;
    c3 = tn / td;
  }

  // div: 3-1-1
  template < typename T > __always_inline void constexpr
  div_QTW_SW_SW( T const a0, T const a1, T const a2, T const b0, T &c0 )
  {
    T e40, e41;
    e40 = a0 + a1 + a2;
    e41 = b0;
    c0 = e40 / e41;
  }

  // div: 3-1-2
  template < typename T > __always_inline void constexpr
  div_QTW_SW_PA( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1 )
  {
    T t0, t1;
    T e40;
    e40 = a0 + a1 + a2;
    div_SW_SW_SW( e40, b0, c0 );
    mul_SW_SW_PA( c0, b0, t0, t1 );
    sub_QTW_PA_PA( a0, a1, a2, t0, t1, t0, t1 );
    T tn, td;
    tn = t0 + t1;
    td = b0;
    c1 = tn / td;
  }

  // div: 3-1-3
  template < typename T > __always_inline void constexpr
  div_QTW_SW_QTW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T e40;
    e40 = a1 + a2;
    div_PA_SW_PA( a0, e40, b0, c0, c1 );
    mul_PA_SW_QTW( c0, c1, b0, t0, t1, t2 );
    sub_QTW_QTW_QTW( a0, a1, a2, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0;
    c2 = tn / td;
  }

  // div: 3-1-4
  template < typename T > __always_inline void constexpr
  div_QTW_SW_QQW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    div_QTW_SW_QTW( a0, a1, a2, b0, c0, c1, c2 );
    mul_QTW_SW_QQW( c0, c1, c2, b0, t0, t1, t2, t3 );
    sub_QTW_QQW_QQW( a0, a1, a2, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0;
    c3 = tn / td;
  }

  // div: 3-2-1
  template < typename T > __always_inline void constexpr
  div_QTW_PA_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0 )
  {
    T e40, e41;
    e40 = a0 + a1 + a2;
    e41 = b0 + b1;
    c0 = e40 / e41;
  }

  // div: 3-2-2
  template < typename T > __always_inline void constexpr
  div_QTW_PA_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0, t1;
    T e40;
    e40 = a0 + a1 + a2;
    T e41;
    e41 = b0 + b1;
    div_SW_SW_SW( e40, e41, c0 );
    mul_SW_PA_PA( c0, b0, b1, t0, t1 );
    sub_QTW_PA_PA( a0, a1, a2, t0, t1, t0, t1 );
    T tn, td;
    tn = t0 + t1;
    td = b0;
    c1 = tn / td;
  }

  // div: 3-2-3
  template < typename T > __always_inline void constexpr
  div_QTW_PA_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T e40;
    e40 = a1 + a2;
    div_PA_PA_PA( a0, e40, b0, b1, c0, c1 );
    mul_PA_PA_QTW( c0, c1, b0, b1, t0, t1, t2 );
    sub_QTW_QTW_QTW( a0, a1, a2, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0;
    c2 = tn / td;
  }

  // div: 3-2-4
  template < typename T > __always_inline void constexpr
  div_QTW_PA_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    div_QTW_PA_QTW( a0, a1, a2, b0, b1, c0, c1, c2 );
    mul_QTW_PA_QQW( c0, c1, c2, b0, b1, t0, t1, t2, t3 );
    sub_QTW_QQW_QQW( a0, a1, a2, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0;
    c3 = tn / td;
  }

  // div: 3-3-1
  template < typename T > __always_inline void constexpr
  div_QTW_QTW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0 )
  {
    T e40, e41;
    e40 = a0 + a1 + a2;
    e41 = b0 + b1 + b2;
    c0 = e40 / e41;
  }

  // div: 3-3-2
  template < typename T > __always_inline void constexpr
  div_QTW_QTW_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1;
    T e40;
    e40 = a0 + a1 + a2;
    T e41;
    e41 = b0 + b1 + b2;
    div_SW_SW_SW( e40, e41, c0 );
    mul_SW_QTW_PA( c0, b0, b1, b2, t0, t1 );
    sub_QTW_PA_PA( a0, a1, a2, t0, t1, t0, t1 );
    T tn, td;
    tn = t0 + t1;
    td = b0 + b1;
    c1 = tn / td;
  }

  // div: 3-3-3
  template < typename T > __always_inline void constexpr
  div_QTW_QTW_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T e40;
    e40 = a1 + a2;
    T e41;
    e41 = b1 + b2;
    div_PA_PA_PA( a0, e40, b0, e41, c0, c1 );
    mul_PA_QTW_QTW( c0, c1, b0, b1, b2, t0, t1, t2 );
    sub_QTW_QTW_QTW( a0, a1, a2, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0 + b1;
    c2 = tn / td;
  }

  // div: 3-3-4
  template < typename T > __always_inline void constexpr
  div_QTW_QTW_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    div_QTW_QTW_QTW( a0, a1, a2, b0, b1, b2, c0, c1, c2 );
    mul_QTW_QTW_QQW( c0, c1, c2, b0, b1, b2, t0, t1, t2, t3 );
    sub_QTW_QQW_QQW( a0, a1, a2, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1;
    c3 = tn / td;
  }

  // div: 3-4-1
  template < typename T > __always_inline void constexpr
  div_QTW_QQW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T e40, e41;
    e40 = a0 + a1 + a2;
    e41 = b0 + b1 + b2 + b3;
    c0 = e40 / e41;
  }

  // div: 3-4-2
  template < typename T > __always_inline void constexpr
  div_QTW_QQW_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1;
    T e40;
    e40 = a0 + a1 + a2;
    T e41;
    e41 = b0 + b1 + b2 + b3;
    div_SW_SW_SW( e40, e41, c0 );
    mul_SW_QQW_PA( c0, b0, b1, b2, b3, t0, t1 );
    sub_QTW_PA_PA( a0, a1, a2, t0, t1, t0, t1 );
    T tn, td;
    tn = t0 + t1;
    td = b0 + b1 + b2;
    c1 = tn / td;
  }

  // div: 3-4-3
  template < typename T > __always_inline void constexpr
  div_QTW_QQW_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T e40;
    e40 = a1 + a2;
    T e41;
    e41 = b1 + b2 + b3;
    div_PA_PA_PA( a0, e40, b0, e41, c0, c1 );
    mul_PA_QQW_QTW( c0, c1, b0, b1, b2, b3, t0, t1, t2 );
    sub_QTW_QTW_QTW( a0, a1, a2, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0 + b1 + b2;
    c2 = tn / td;
  }

  // div: 3-4-4
  template < typename T > __always_inline void constexpr
  div_QTW_QQW_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T e40;
    e40 = b2 + b3;
    div_QTW_QTW_QTW( a0, a1, a2, b0, b1, e40, c0, c1, c2 );
    mul_QTW_QQW_QQW( c0, c1, c2, b0, b1, b2, b3, t0, t1, t2, t3 );
    sub_QTW_QQW_QQW( a0, a1, a2, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1 + b2;
    c3 = tn / td;
  }

  // div: 4-1-1
  template < typename T > __always_inline void constexpr
  div_QQW_SW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0 )
  {
    T e40, e41;
    e40 = a0 + a1 + a2 + a3;
    e41 = b0;
    c0 = e40 / e41;
  }

  // div: 4-1-2
  template < typename T > __always_inline void constexpr
  div_QQW_SW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1 )
  {
    T t0, t1;
    T e40;
    e40 = a0 + a1 + a2 + a3;
    div_SW_SW_SW( e40, b0, c0 );
    mul_SW_SW_PA( c0, b0, t0, t1 );
    sub_QQW_PA_PA( a0, a1, a2, a3, t0, t1, t0, t1 );
    T tn, td;
    tn = t0 + t1;
    td = b0;
    c1 = tn / td;
  }

  // div: 4-1-3
  template < typename T > __always_inline void constexpr
  div_QQW_SW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T e40;
    e40 = a1 + a2 + a3;
    div_PA_SW_PA( a0, e40, b0, c0, c1 );
    mul_PA_SW_QTW( c0, c1, b0, t0, t1, t2 );
    sub_QQW_QTW_QTW( a0, a1, a2, a3, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0;
    c2 = tn / td;
  }

  // div: 4-1-4
  template < typename T > __always_inline void constexpr
  div_QQW_SW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T e40;
    e40 = a2 + a3;
    div_QTW_SW_QTW( a0, a1, e40, b0, c0, c1, c2 );
    mul_QTW_SW_QQW( c0, c1, c2, b0, t0, t1, t2, t3 );
    sub_QQW_QQW_QQW( a0, a1, a2, a3, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0;
    c3 = tn / td;
  }

  // div: 4-2-1
  template < typename T > __always_inline void constexpr
  div_QQW_PA_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0 )
  {
    T e40, e41;
    e40 = a0 + a1 + a2 + a3;
    e41 = b0 + b1;
    c0 = e40 / e41;
  }

  // div: 4-2-2
  template < typename T > __always_inline void constexpr
  div_QQW_PA_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0, t1;
    T e40;
    e40 = a0 + a1 + a2 + a3;
    T e41;
    e41 = b0 + b1;
    div_SW_SW_SW( e40, e41, c0 );
    mul_SW_PA_PA( c0, b0, b1, t0, t1 );
    sub_QQW_PA_PA( a0, a1, a2, a3, t0, t1, t0, t1 );
    T tn, td;
    tn = t0 + t1;
    td = b0;
    c1 = tn / td;
  }

  // div: 4-2-3
  template < typename T > __always_inline void constexpr
  div_QQW_PA_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T e40;
    e40 = a1 + a2 + a3;
    div_PA_PA_PA( a0, e40, b0, b1, c0, c1 );
    mul_PA_PA_QTW( c0, c1, b0, b1, t0, t1, t2 );
    sub_QQW_QTW_QTW( a0, a1, a2, a3, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0;
    c2 = tn / td;
  }

  // div: 4-2-4
  template < typename T > __always_inline void constexpr
  div_QQW_PA_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T e40;
    e40 = a2 + a3;
    div_QTW_PA_QTW( a0, a1, e40, b0, b1, c0, c1, c2 );
    mul_QTW_PA_QQW( c0, c1, c2, b0, b1, t0, t1, t2, t3 );
    sub_QQW_QQW_QQW( a0, a1, a2, a3, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0;
    c3 = tn / td;
  }

  // div: 4-3-1
  template < typename T > __always_inline void constexpr
  div_QQW_QTW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0 )
  {
    T e40, e41;
    e40 = a0 + a1 + a2 + a3;
    e41 = b0 + b1 + b2;
    c0 = e40 / e41;
  }

  // div: 4-3-2
  template < typename T > __always_inline void constexpr
  div_QQW_QTW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1;
    T e40;
    e40 = a0 + a1 + a2 + a3;
    T e41;
    e41 = b0 + b1 + b2;
    div_SW_SW_SW( e40, e41, c0 );
    mul_SW_QTW_PA( c0, b0, b1, b2, t0, t1 );
    sub_QQW_PA_PA( a0, a1, a2, a3, t0, t1, t0, t1 );
    T tn, td;
    tn = t0 + t1;
    td = b0 + b1;
    c1 = tn / td;
  }

  // div: 4-3-3
  template < typename T > __always_inline void constexpr
  div_QQW_QTW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T e40;
    e40 = a1 + a2 + a3;
    T e41;
    e41 = b1 + b2;
    div_PA_PA_PA( a0, e40, b0, e41, c0, c1 );
    mul_PA_QTW_QTW( c0, c1, b0, b1, b2, t0, t1, t2 );
    sub_QQW_QTW_QTW( a0, a1, a2, a3, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0 + b1;
    c2 = tn / td;
  }

  // div: 4-3-4
  template < typename T > __always_inline void constexpr
  div_QQW_QTW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T e40;
    e40 = a2 + a3;
    div_QTW_QTW_QTW( a0, a1, e40, b0, b1, b2, c0, c1, c2 );
    mul_QTW_QTW_QQW( c0, c1, c2, b0, b1, b2, t0, t1, t2, t3 );
    sub_QQW_QQW_QQW( a0, a1, a2, a3, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1;
    c3 = tn / td;
  }

  // div: 4-4-1
  template < typename T > __always_inline void constexpr
  div_QQW_QQW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T e40, e41;
    e40 = a0 + a1 + a2 + a3;
    e41 = b0 + b1 + b2 + b3;
    c0 = e40 / e41;
  }

  // div: 4-4-2
  template < typename T > __always_inline void constexpr
  div_QQW_QQW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1;
    T e40;
    e40 = a0 + a1 + a2 + a3;
    T e41;
    e41 = b0 + b1 + b2 + b3;
    div_SW_SW_SW( e40, e41, c0 );
    mul_SW_QQW_PA( c0, b0, b1, b2, b3, t0, t1 );
    sub_QQW_PA_PA( a0, a1, a2, a3, t0, t1, t0, t1 );
    T tn, td;
    tn = t0 + t1;
    td = b0 + b1 + b2;
    c1 = tn / td;
  }

  // div: 4-4-3
  template < typename T > __always_inline void constexpr
  div_QQW_QQW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T e40;
    e40 = a1 + a2 + a3;
    T e41;
    e41 = b1 + b2 + b3;
    div_PA_PA_PA( a0, e40, b0, e41, c0, c1 );
    mul_PA_QQW_QTW( c0, c1, b0, b1, b2, b3, t0, t1, t2 );
    sub_QQW_QTW_QTW( a0, a1, a2, a3, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0 + b1 + b2;
    c2 = tn / td;
  }

  // div: 4-4-4
  template < typename T > __always_inline void constexpr
  div_QQW_QQW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T e40;
    e40 = a2 + a3;
    T e41;
    e41 = b2 + b3;
    div_QTW_QTW_QTW( a0, a1, e40, b0, b1, e41, c0, c1, c2 );
    mul_QTW_QQW_QQW( c0, c1, c2, b0, b1, b2, b3, t0, t1, t2, t3 );
    sub_QQW_QQW_QQW( a0, a1, a2, a3, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1 + b2;
    c3 = tn / td;
  }

  // sqr: 1-1
  template < typename T > __always_inline void constexpr
  sqr_SW_SW( T const a0, T &c0 )
  {
    c0 = a0 * a0;
  }

  // sqr: 1-2
  template < typename T > __always_inline void constexpr
  sqr_SW_PA( T const a0, T &c0, T &c1 )
  {
    TwoProductFMA( a0, a0, c0, c1 );
  }

  // sqr: 1-3
  template < typename T > __always_inline void constexpr
  sqr_SW_QTW( T const a0, T &c0, T &c1, T &c2 )
  {
    TwoProductFMA( a0, a0, c0, c1 );
    c2 = fp_const<T>::zero();
  }

  // sqr: 1-4
  template < typename T > __always_inline void constexpr
  sqr_SW_QQW( T const a0, T &c0, T &c1, T &c2, T &c3 )
  {
    TwoProductFMA( a0, a0, c0, c1 );
    c2 = fp_const<T>::zero();
    c3 = fp_const<T>::zero();
  }

  // sqr: 2-1
  template < typename T > __always_inline void constexpr
  sqr_PA_SW( T const a0, T const a1, T &c0 )
  {
    T t0;
    t0 = a0 + a1;
    c0 = t0 * t0;
  }

  // sqr: 2-2
  template < typename T > __always_inline void constexpr
  sqr_PA_PA( T const a0, T const a1, T &c0, T &c1 )
  {
    T t0;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    c1 = std::fma ( t0, a1, c1 );
  }

  // sqr: 2-3
  template < typename T > __always_inline void constexpr
  sqr_PA_QTW( T const a0, T const a1, T &c0, T &c1, T &c2 )
  {
    T t0, t1;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    TwoProductFMA( t0, a1, c2, t1 );
    TwoSum( c1, c2, c1, c2 );
    c2 = c2 + t1;
    c2 = std::fma ( a1, a1, c2 );
  }

  // sqr: 2-4
  template < typename T > __always_inline void constexpr
  sqr_PA_QQW( T const a0, T const a1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    TwoProductFMA( t0, a1, c2, c3 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c2, c3, c2, c3 );
    TwoProductFMA( a1, a1, t1, t2 );
    TwoSum( c2, t1, c2, t1 );
    c3 = c3 + t1 + t2;
  }

  // sqr: 3-1
  template < typename T > __always_inline void constexpr
  sqr_QTW_SW( T const a0, T const a1, T const a2, T &c0 )
  {
    T t0;
    t0 = a0 + a1 + a2;
    c0 = t0 * t0;
  }

  // sqr: 3-2
  template < typename T > __always_inline void constexpr
  sqr_QTW_PA( T const a0, T const a1, T const a2, T &c0, T &c1 )
  {
    T t0, t1;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    t1 = a1 + a2;
    c1 = std::fma ( t0, t1, c1 );
  }

  // sqr: 3-3
  template < typename T > __always_inline void constexpr
  sqr_QTW_QTW( T const a0, T const a1, T const a2, T &c0, T &c1, T &c2 )
  {
    T t0, t1;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    TwoProductFMA( t0, a1, c2, t1 );
    TwoSum( c1, c2, c1, c2 );
    c2 = c2 + t1;
    c2 = std::fma ( t0, a2, c2 );
    c2 = std::fma ( a1, a1, c2 );
  }

  // sqr: 3-4
  template < typename T > __always_inline void constexpr
  sqr_QTW_QQW( T const a0, T const a1, T const a2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    TwoProductFMA( t0, a1, c2, c3 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c2, c3, c2, c3 );
    TwoProductFMA( t0, a2, t1, t2 );
    TwoProductFMA( a1, a1, t3, t4 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t3, c2, t3 );
    t5 = a1 + a1;
    c3 = c3 + t1 + t2 + t3 + t4;
    c3 = std::fma ( t5, a2, c3 );
  }

  // sqr: 4-1
  template < typename T > __always_inline void constexpr
  sqr_QQW_SW( T const a0, T const a1, T const a2, T const a3, T &c0 )
  {
    T t0;
    t0 = a0 + a1 + a2 + a3;
    c0 = t0 * t0;
  }

  // sqr: 4-2
  template < typename T > __always_inline void constexpr
  sqr_QQW_PA( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1 )
  {
    T t0, t1;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    t1 = a1 + a2 + a3;
    c1 = std::fma ( t0, t1, c1 );
  }

  // sqr: 4-3
  template < typename T > __always_inline void constexpr
  sqr_QQW_QTW( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    TwoProductFMA( t0, a1, c2, t1 );
    TwoSum( c1, c2, c1, c2 );
    c2 = c2 + t1;
    t2 = a2 + a3;
    c2 = std::fma ( t0, t2, c2 );
    c2 = std::fma ( a1, a1, c2 );
  }

  // sqr: 4-4
  template < typename T > __always_inline void constexpr
  sqr_QQW_QQW( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    TwoProductFMA( t0, a1, c2, c3 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c2, c3, c2, c3 );
    TwoProductFMA( t0, a2, t1, t2 );
    TwoProductFMA( a1, a1, t3, t4 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t3, c2, t3 );
    t5 = a1 + a1;
    c3 = c3 + t1 + t2 + t3 + t4;
    c3 = std::fma ( t0, a3, c3 );
    c3 = std::fma ( t5, a2, c3 );
  }


  // ------------------------
  // Accurate Arithmetic Part
  // ------------------------

  // add: 1-1-2
  template < typename T > __always_inline void constexpr
  add_SW_SW_DW( T const a0, T const b0, T &c0, T &c1 )
  {
    TwoSum( a0, b0, c0, c1 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 1-1-3
  template < typename T > __always_inline void constexpr
  add_SW_SW_TW( T const a0, T const b0, T &c0, T &c1, T &c2 )
  {
    TwoSum( a0, b0, c0, c1 );
    FastTwoSum( c0, c1, c0, c1 );
    c2 = fp_const<T>::zero();
  }

  // add: 1-1-4
  template < typename T > __always_inline void constexpr
  add_SW_SW_QW( T const a0, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    TwoSum( a0, b0, c0, c1 );
    FastTwoSum( c0, c1, c0, c1 );
    c2 = fp_const<T>::zero();
    c3 = fp_const<T>::zero();
  }

  // add: 1-2-1
  template < typename T > __always_inline void constexpr
  add_SW_DW_SW( T const a0, T const b0, T const b1, T &c0 )
  {
    T t0;
    TwoSum( a0, b0, c0, t0 );
    c0 = c0 + b1;
    c0 = c0 + t0;
  }

  // add: 1-2-2
  template < typename T > __always_inline void constexpr
  add_SW_DW_DW( T const a0, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, t0 );
    c1 = c1 + t0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 1-2-3
  template < typename T > __always_inline void constexpr
  add_SW_DW_TW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, t0 );
    FastTwoSum( c1, t0, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 1-2-4
  template < typename T > __always_inline void constexpr
  add_SW_DW_QW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    c3 = fp_const<T>::zero();
  }

  // add: 1-3-1
  template < typename T > __always_inline void constexpr
  add_SW_TW_SW( T const a0, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, t0 );
    t1 = b1 + b2;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 1-3-2
  template < typename T > __always_inline void constexpr
  add_SW_TW_DW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, t0 );
    c1 = c1 + t0;
    c1 = c1 + b2;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 1-3-3
  template < typename T > __always_inline void constexpr
  add_SW_TW_TW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, t0 );
    TwoSum( t0, b2, c2, t1 );
    FastTwoSum( c1, c2, c1, c2 );
    c2 = c2 + t1;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 1-3-4
  template < typename T > __always_inline void constexpr
  add_SW_TW_QW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, t0 );
    TwoSum( t0, b2, c2, t1 );
    FastTwoSum( c2, t1, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 1-4-1
  template < typename T > __always_inline void constexpr
  add_SW_QW_SW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, t0 );
    t1 = b1 + b2 + b3;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 1-4-2
  template < typename T > __always_inline void constexpr
  add_SW_QW_DW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, t0 );
    c1 = c1 + t0;
    c1 = c1 + b2;
    c1 = c1 + b3;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 1-4-3
  template < typename T > __always_inline void constexpr
  add_SW_QW_TW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, t0 );
    TwoSum( t0, b2, c2, t1 );
    FastTwoSum( c1, c2, c1, c2 );
    c2 = c2 + b3;
    FastTwoSum( c1, c2, c1, c2 );
    c2 = c2 + t1;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 1-4-4
  template < typename T > __always_inline void constexpr
  add_SW_QW_QW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, t0 );
    TwoSum( t0, b2, c2, t1 );
    TwoSum( t1, b3, c3, t2 );
    FastTwoSum( c2, c3, c2, c3 );
    c3 = c3 + t2;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-1-1
  template < typename T > __always_inline void constexpr
  add_DW_SW_SW( T const a0, T const a1, T const b0, T &c0 )
  {
    T t0;
    TwoSum( a0, b0, c0, t0 );
    c0 = c0 + a1;
    c0 = c0 + t0;
  }

  // add: 2-1-2
  template < typename T > __always_inline void constexpr
  add_DW_SW_DW( T const a0, T const a1, T const b0, T &c0, T &c1 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, t0 );
    c1 = c1 + t0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-1-3
  template < typename T > __always_inline void constexpr
  add_DW_SW_TW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, t0 );
    FastTwoSum( c1, t0, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-1-4
  template < typename T > __always_inline void constexpr
  add_DW_SW_QW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    c3 = fp_const<T>::zero();
  }

  // add: 2-2-1
  template < typename T > __always_inline void constexpr
  add_DW_DW_SW( T const a0, T const a1, T const b0, T const b1, T &c0 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + b1;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 2-2-2
  template < typename T > __always_inline void constexpr
  add_DW_DW_DW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    c1 = c1 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-2-3
  template < typename T > __always_inline void constexpr
  add_DW_DW_TW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    FastTwoSum( c1, c2, c1, c2 );
    c2 = c2 + t0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-2-4
  template < typename T > __always_inline void constexpr
  add_DW_DW_QW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    FastTwoSum( c2, t0, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-3-1
  template < typename T > __always_inline void constexpr
  add_DW_TW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + b1;
    t1 = t1 + b2;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 2-3-2
  template < typename T > __always_inline void constexpr
  add_DW_TW_DW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    c1 = c1 + t0;
    c1 = c1 + b2;
    FastTwoSum( c0, c1, c0, c1 );
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-3-3
  template < typename T > __always_inline void constexpr
  add_DW_TW_TW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, b2, c2, t1 );
    FastTwoSum( c1, c2, c1, c2 );
    t0 = t0 + t1;
    c2 = c2 + t0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-3-4
  template < typename T > __always_inline void constexpr
  add_DW_TW_QW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, b2, c2, t1 );
    TwoSum( t0, t1, c3, t1 );
    FastTwoSum( c2, c3, c2, c3 );
    c3 = c3 + t1;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-4-1
  template < typename T > __always_inline void constexpr
  add_DW_QW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + b1;
    t1 = t1 + b2 + b3;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 2-4-2
  template < typename T > __always_inline void constexpr
  add_DW_QW_DW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    c1 = c1 + t0;
    c1 = c1 + b2;
    c1 = c1 + b3;
    FastTwoSum( c0, c1, c0, c1 );
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-4-3
  template < typename T > __always_inline void constexpr
  add_DW_QW_TW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, b2, c2, t1 );
    FastTwoSum( c1, c2, c1, c2 );
    c2 = c2 + b3;
    FastTwoSum( c1, c2, c1, c2 );
    t0 = t0 + t1;
    c2 = c2 + t0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-4-4
  template < typename T > __always_inline void constexpr
  add_DW_QW_QW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, b2, c2, t1 );
    TwoSum( t0, t1, c3, t1 );
    TwoSum( c3, b3, c3, t2 );
    FastTwoSum( c2, c3, c2, c3 );
    t0 = t1 + t2;
    c3 = c3 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-1-1
  template < typename T > __always_inline void constexpr
  add_TW_SW_SW( T const a0, T const a1, T const a2, T const b0, T &c0 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + a2;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 3-1-2
  template < typename T > __always_inline void constexpr
  add_TW_SW_DW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, t0 );
    c1 = c1 + t0;
    c1 = c1 + a2;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-1-3
  template < typename T > __always_inline void constexpr
  add_TW_SW_TW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, t0 );
    TwoSum( t0, a2, c2, t1 );
    FastTwoSum( c1, c2, c1, c2 );
    c2 = c2 + t1;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-1-4
  template < typename T > __always_inline void constexpr
  add_TW_SW_QW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, t0 );
    TwoSum( t0, a2, c2, t1 );
    FastTwoSum( c2, t1, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-2-1
  template < typename T > __always_inline void constexpr
  add_TW_DW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + b1;
    t1 = t1 + a2;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 3-2-2
  template < typename T > __always_inline void constexpr
  add_TW_DW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    c1 = c1 + t0;
    c1 = c1 + a2;
    FastTwoSum( c0, c1, c0, c1 );
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-2-3
  template < typename T > __always_inline void constexpr
  add_TW_DW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, a2, c2, t1 );
    FastTwoSum( c1, c2, c1, c2 );
    t0 = t0 + t1;
    c2 = c2 + t0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-2-4
  template < typename T > __always_inline void constexpr
  add_TW_DW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, a2, c2, t1 );
    TwoSum( t0, t1, c3, t1 );
    FastTwoSum( c2, c3, c2, c3 );
    c3 = c3 + t1;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-3-1
  template < typename T > __always_inline void constexpr
  add_TW_TW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + b1;
    t2 = a2 + b2;
    t1 = t1 + t2;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 3-3-2
  template < typename T > __always_inline void constexpr
  add_TW_TW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    c1 = c1 + t0;
    t2 = a2 + b2;
    c1 = c1 + t2;
    FastTwoSum( c0, c1, c0, c1 );
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-3-3
  template < typename T > __always_inline void constexpr
  add_TW_TW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( a2, b2, t1, t2 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    FastTwoSum( c1, c2, c1, c2 );
    t0 = t0 + t1 + t2;
    c2 = c2 + t0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-3-4
  template < typename T > __always_inline void constexpr
  add_TW_TW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( a2, b2, t1, c3 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t1, c3, t1 );
    FastTwoSum( c2, c3, c2, c3 );
    t0 = t0 + t1;
    c3 = c3 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-4-1
  template < typename T > __always_inline void constexpr
  add_TW_QW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + b1;
    t2 = a2 + b2;
    t1 = t1 + t2 + b3;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 3-4-2
  template < typename T > __always_inline void constexpr
  add_TW_QW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    c1 = c1 + t0;
    t2 = a2 + b2;
    c1 = c1 + t2;
    c1 = c1 + b3;
    FastTwoSum( c0, c1, c0, c1 );
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-4-3
  template < typename T > __always_inline void constexpr
  add_TW_QW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( a2, b2, t1, t2 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    FastTwoSum( c1, c2, c1, c2 );
    c2 = c2 + b3;
    FastTwoSum( c1, c2, c1, c2 );
    t0 = t0 + t1 + t2;
    c2 = c2 + t0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-4-4
  template < typename T > __always_inline void constexpr
  add_TW_QW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( a2, b2, t1, c3 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t1, c3, t1 );
    TwoSum( c3, b3, c3, t2 );
    FastTwoSum( c2, c3, c2, c3 );
    t0 = t0 + t1 + t2;
    c3 = c3 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 4-1-1
  template < typename T > __always_inline void constexpr
  add_QW_SW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + a2 + a3;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 4-1-2
  template < typename T > __always_inline void constexpr
  add_QW_SW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1 )
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, t0 );
    c1 = c1 + t0;
    c1 = c1 + a2;
    c1 = c1 + a3;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 4-1-3
  template < typename T > __always_inline void constexpr
  add_QW_SW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, t0 );
    TwoSum( t0, a2, c2, t1 );
    FastTwoSum( c1, c2, c1, c2 );
    c2 = c2 + a3;
    FastTwoSum( c1, c2, c1, c2 );
    c2 = c2 + t1;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 4-1-4
  template < typename T > __always_inline void constexpr
  add_QW_SW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, t0 );
    TwoSum( t0, a2, c2, t1 );
    TwoSum( t1, a3, c3, t2 );
    FastTwoSum( c2, c3, c2, c3 );
    c3 = c3 + t2;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 4-2-1
  template < typename T > __always_inline void constexpr
  add_QW_DW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + b1;
    t1 = t1 + a2 + a3;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 4-2-2
  template < typename T > __always_inline void constexpr
  add_QW_DW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    c1 = c1 + t0;
    c1 = c1 + a2;
    c1 = c1 + a3;
    FastTwoSum( c0, c1, c0, c1 );
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 4-2-3
  template < typename T > __always_inline void constexpr
  add_QW_DW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, a2, c2, t1 );
    FastTwoSum( c1, c2, c1, c2 );
    c2 = c2 + a3;
    FastTwoSum( c1, c2, c1, c2 );
    t0 = t0 + t1;
    c2 = c2 + t0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 4-2-4
  template < typename T > __always_inline void constexpr
  add_QW_DW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, a2, c2, t1 );
    TwoSum( t0, t1, c3, t1 );
    TwoSum( c3, a3, c3, t2 );
    FastTwoSum( c2, c3, c2, c3 );
    t0 = t1 + t2;
    c3 = c3 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 4-3-1
  template < typename T > __always_inline void constexpr
  add_QW_TW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + b1;
    t2 = a2 + b2;
    t1 = t1 + t2 + a3;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 4-3-2
  template < typename T > __always_inline void constexpr
  add_QW_TW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    c1 = c1 + t0;
    t2 = a2 + b2;
    c1 = c1 + t2;
    c1 = c1 + a3;
    FastTwoSum( c0, c1, c0, c1 );
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 4-3-3
  template < typename T > __always_inline void constexpr
  add_QW_TW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( a2, b2, t1, t2 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    FastTwoSum( c1, c2, c1, c2 );
    c2 = c2 + a3;
    FastTwoSum( c1, c2, c1, c2 );
    t0 = t0 + t1 + t2;
    c2 = c2 + t0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 4-3-4
  template < typename T > __always_inline void constexpr
  add_QW_TW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( a2, b2, t1, c3 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t1, c3, t1 );
    TwoSum( c3, a3, c3, t2 );
    FastTwoSum( c2, c3, c2, c3 );
    t0 = t0 + t1 + t2;
    c3 = c3 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 4-4-1
  template < typename T > __always_inline void constexpr
  add_QW_QW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0, t1, t2, t3;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + b1;
    t2 = a2 + b2;
    t3 = a3 + b3;
    t1 = t1 + t2 + t3;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 4-4-2
  template < typename T > __always_inline void constexpr
  add_QW_QW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1, t2, t3;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    c1 = c1 + t0;
    t2 = a2 + b2;
    t3 = a3 + b3;
    c1 = c1 + t2;
    c1 = c1 + t3;
    FastTwoSum( c0, c1, c0, c1 );
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 4-4-3
  template < typename T > __always_inline void constexpr
  add_QW_QW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( a2, b2, t1, t2 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    FastTwoSum( c1, c2, c1, c2 );
    t3 = a3 + b3;
    c2 = c2 + t3;
    FastTwoSum( c1, c2, c1, c2 );
    t0 = t0 + t1 + t2;
    c2 = c2 + t0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 4-4-4
  template < typename T > __always_inline void constexpr
  add_QW_QW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( a2, b2, t1, c3 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( a3, b3, t2, t3 );
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t1, c3, t1 );
    TwoSum( c3, t2, c3, t2 );
    FastTwoSum( c2, c3, c2, c3 );
    t0 = t0 + t1 + t2 + t3;
    c3 = c3 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // sub: 1-1-2
  template < typename T > __always_inline void constexpr
  sub_SW_SW_DW( T const a0, T const b0, T &c0, T &c1 )
  {
    add_SW_SW_DW( a0, -b0, c0, c1 );
  }

  // sub: 1-1-3
  template < typename T > __always_inline void constexpr
  sub_SW_SW_TW( T const a0, T const b0, T &c0, T &c1, T &c2 )
  {
    add_SW_SW_TW( a0, -b0, c0, c1, c2 );
  }

  // sub: 1-1-4
  template < typename T > __always_inline void constexpr
  sub_SW_SW_QW( T const a0, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    add_SW_SW_QW( a0, -b0, c0, c1, c2, c3 );
  }

  // sub: 1-2-1
  template < typename T > __always_inline void constexpr
  sub_SW_DW_SW( T const a0, T const b0, T const b1, T &c0 )
  {
    add_SW_DW_SW( a0, -b0, -b1, c0 );
  }

  // sub: 1-2-2
  template < typename T > __always_inline void constexpr
  sub_SW_DW_DW( T const a0, T const b0, T const b1, T &c0, T &c1 )
  {
    add_SW_DW_DW( a0, -b0, -b1, c0, c1 );
  }

  // sub: 1-2-3
  template < typename T > __always_inline void constexpr
  sub_SW_DW_TW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    add_SW_DW_TW( a0, -b0, -b1, c0, c1, c2 );
  }

  // sub: 1-2-4
  template < typename T > __always_inline void constexpr
  sub_SW_DW_QW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    add_SW_DW_QW( a0, -b0, -b1, c0, c1, c2, c3 );
  }

  // sub: 1-3-1
  template < typename T > __always_inline void constexpr
  sub_SW_TW_SW( T const a0, T const b0, T const b1, T const b2, T &c0 )
  {
    add_SW_TW_SW( a0, -b0, -b1, -b2, c0 );
  }

  // sub: 1-3-2
  template < typename T > __always_inline void constexpr
  sub_SW_TW_DW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    add_SW_TW_DW( a0, -b0, -b1, -b2, c0, c1 );
  }

  // sub: 1-3-3
  template < typename T > __always_inline void constexpr
  sub_SW_TW_TW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    add_SW_TW_TW( a0, -b0, -b1, -b2, c0, c1, c2 );
  }

  // sub: 1-3-4
  template < typename T > __always_inline void constexpr
  sub_SW_TW_QW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    add_SW_TW_QW( a0, -b0, -b1, -b2, c0, c1, c2, c3 );
  }

  // sub: 1-4-1
  template < typename T > __always_inline void constexpr
  sub_SW_QW_SW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    add_SW_QW_SW( a0, -b0, -b1, -b2, -b3, c0 );
  }

  // sub: 1-4-2
  template < typename T > __always_inline void constexpr
  sub_SW_QW_DW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    add_SW_QW_DW( a0, -b0, -b1, -b2, -b3, c0, c1 );
  }

  // sub: 1-4-3
  template < typename T > __always_inline void constexpr
  sub_SW_QW_TW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    add_SW_QW_TW( a0, -b0, -b1, -b2, -b3, c0, c1, c2 );
  }

  // sub: 1-4-4
  template < typename T > __always_inline void constexpr
  sub_SW_QW_QW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    add_SW_QW_QW( a0, -b0, -b1, -b2, -b3, c0, c1, c2, c3 );
  }

  // sub: 2-1-1
  template < typename T > __always_inline void constexpr
  sub_DW_SW_SW( T const a0, T const a1, T const b0, T &c0 )
  {
    add_DW_SW_SW( a0, a1, -b0, c0 );
  }

  // sub: 2-1-2
  template < typename T > __always_inline void constexpr
  sub_DW_SW_DW( T const a0, T const a1, T const b0, T &c0, T &c1 )
  {
    add_DW_SW_DW( a0, a1, -b0, c0, c1 );
  }

  // sub: 2-1-3
  template < typename T > __always_inline void constexpr
  sub_DW_SW_TW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2 )
  {
    add_DW_SW_TW( a0, a1, -b0, c0, c1, c2 );
  }

  // sub: 2-1-4
  template < typename T > __always_inline void constexpr
  sub_DW_SW_QW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    add_DW_SW_QW( a0, a1, -b0, c0, c1, c2, c3 );
  }

  // sub: 2-2-1
  template < typename T > __always_inline void constexpr
  sub_DW_DW_SW( T const a0, T const a1, T const b0, T const b1, T &c0 )
  {
    add_DW_DW_SW( a0, a1, -b0, -b1, c0 );
  }

  // sub: 2-2-2
  template < typename T > __always_inline void constexpr
  sub_DW_DW_DW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1 )
  {
    add_DW_DW_DW( a0, a1, -b0, -b1, c0, c1 );
  }

  // sub: 2-2-3
  template < typename T > __always_inline void constexpr
  sub_DW_DW_TW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    add_DW_DW_TW( a0, a1, -b0, -b1, c0, c1, c2 );
  }

  // sub: 2-2-4
  template < typename T > __always_inline void constexpr
  sub_DW_DW_QW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    add_DW_DW_QW( a0, a1, -b0, -b1, c0, c1, c2, c3 );
  }

  // sub: 2-3-1
  template < typename T > __always_inline void constexpr
  sub_DW_TW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0 )
  {
    add_DW_TW_SW( a0, a1, -b0, -b1, -b2, c0 );
  }

  // sub: 2-3-2
  template < typename T > __always_inline void constexpr
  sub_DW_TW_DW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    add_DW_TW_DW( a0, a1, -b0, -b1, -b2, c0, c1 );
  }

  // sub: 2-3-3
  template < typename T > __always_inline void constexpr
  sub_DW_TW_TW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    add_DW_TW_TW( a0, a1, -b0, -b1, -b2, c0, c1, c2 );
  }

  // sub: 2-3-4
  template < typename T > __always_inline void constexpr
  sub_DW_TW_QW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    add_DW_TW_QW( a0, a1, -b0, -b1, -b2, c0, c1, c2, c3 );
  }

  // sub: 2-4-1
  template < typename T > __always_inline void constexpr
  sub_DW_QW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    add_DW_QW_SW( a0, a1, -b0, -b1, -b2, -b3, c0 );
  }

  // sub: 2-4-2
  template < typename T > __always_inline void constexpr
  sub_DW_QW_DW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    add_DW_QW_DW( a0, a1, -b0, -b1, -b2, -b3, c0, c1 );
  }

  // sub: 2-4-3
  template < typename T > __always_inline void constexpr
  sub_DW_QW_TW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    add_DW_QW_TW( a0, a1, -b0, -b1, -b2, -b3, c0, c1, c2 );
  }

  // sub: 2-4-4
  template < typename T > __always_inline void constexpr
  sub_DW_QW_QW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    add_DW_QW_QW( a0, a1, -b0, -b1, -b2, -b3, c0, c1, c2, c3 );
  }

  // sub: 3-1-1
  template < typename T > __always_inline void constexpr
  sub_TW_SW_SW( T const a0, T const a1, T const a2, T const b0, T &c0 )
  {
    add_TW_SW_SW( a0, a1, a2, -b0, c0 );
  }

  // sub: 3-1-2
  template < typename T > __always_inline void constexpr
  sub_TW_SW_DW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1 )
  {
    add_TW_SW_DW( a0, a1, a2, -b0, c0, c1 );
  }

  // sub: 3-1-3
  template < typename T > __always_inline void constexpr
  sub_TW_SW_TW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2 )
  {
    add_TW_SW_TW( a0, a1, a2, -b0, c0, c1, c2 );
  }

  // sub: 3-1-4
  template < typename T > __always_inline void constexpr
  sub_TW_SW_QW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    add_TW_SW_QW( a0, a1, a2, -b0, c0, c1, c2, c3 );
  }

  // sub: 3-2-1
  template < typename T > __always_inline void constexpr
  sub_TW_DW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0 )
  {
    add_TW_DW_SW( a0, a1, a2, -b0, -b1, c0 );
  }

  // sub: 3-2-2
  template < typename T > __always_inline void constexpr
  sub_TW_DW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1 )
  {
    add_TW_DW_DW( a0, a1, a2, -b0, -b1, c0, c1 );
  }

  // sub: 3-2-3
  template < typename T > __always_inline void constexpr
  sub_TW_DW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    add_TW_DW_TW( a0, a1, a2, -b0, -b1, c0, c1, c2 );
  }

  // sub: 3-2-4
  template < typename T > __always_inline void constexpr
  sub_TW_DW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    add_TW_DW_QW( a0, a1, a2, -b0, -b1, c0, c1, c2, c3 );
  }

  // sub: 3-3-1
  template < typename T > __always_inline void constexpr
  sub_TW_TW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0 )
  {
    add_TW_TW_SW( a0, a1, a2, -b0, -b1, -b2, c0 );
  }

  // sub: 3-3-2
  template < typename T > __always_inline void constexpr
  sub_TW_TW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    add_TW_TW_DW( a0, a1, a2, -b0, -b1, -b2, c0, c1 );
  }

  // sub: 3-3-3
  template < typename T > __always_inline void constexpr
  sub_TW_TW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    add_TW_TW_TW( a0, a1, a2, -b0, -b1, -b2, c0, c1, c2 );
  }

  // sub: 3-3-4
  template < typename T > __always_inline void constexpr
  sub_TW_TW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    add_TW_TW_QW( a0, a1, a2, -b0, -b1, -b2, c0, c1, c2, c3 );
  }

  // sub: 3-4-1
  template < typename T > __always_inline void constexpr
  sub_TW_QW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    add_TW_QW_SW( a0, a1, a2, -b0, -b1, -b2, -b3, c0 );
  }

  // sub: 3-4-2
  template < typename T > __always_inline void constexpr
  sub_TW_QW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    add_TW_QW_DW( a0, a1, a2, -b0, -b1, -b2, -b3, c0, c1 );
  }

  // sub: 3-4-3
  template < typename T > __always_inline void constexpr
  sub_TW_QW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    add_TW_QW_TW( a0, a1, a2, -b0, -b1, -b2, -b3, c0, c1, c2 );
  }

  // sub: 3-4-4
  template < typename T > __always_inline void constexpr
  sub_TW_QW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    add_TW_QW_QW( a0, a1, a2, -b0, -b1, -b2, -b3, c0, c1, c2, c3 );
  }

  // sub: 4-1-1
  template < typename T > __always_inline void constexpr
  sub_QW_SW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0 )
  {
    add_QW_SW_SW( a0, a1, a2, a3, -b0, c0 );
  }

  // sub: 4-1-2
  template < typename T > __always_inline void constexpr
  sub_QW_SW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1 )
  {
    add_QW_SW_DW( a0, a1, a2, a3, -b0, c0, c1 );
  }

  // sub: 4-1-3
  template < typename T > __always_inline void constexpr
  sub_QW_SW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2 )
  {
    add_QW_SW_TW( a0, a1, a2, a3, -b0, c0, c1, c2 );
  }

  // sub: 4-1-4
  template < typename T > __always_inline void constexpr
  sub_QW_SW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    add_QW_SW_QW( a0, a1, a2, a3, -b0, c0, c1, c2, c3 );
  }

  // sub: 4-2-1
  template < typename T > __always_inline void constexpr
  sub_QW_DW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0 )
  {
    add_QW_DW_SW( a0, a1, a2, a3, -b0, -b1, c0 );
  }

  // sub: 4-2-2
  template < typename T > __always_inline void constexpr
  sub_QW_DW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1 )
  {
    add_QW_DW_DW( a0, a1, a2, a3, -b0, -b1, c0, c1 );
  }

  // sub: 4-2-3
  template < typename T > __always_inline void constexpr
  sub_QW_DW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    add_QW_DW_TW( a0, a1, a2, a3, -b0, -b1, c0, c1, c2 );
  }

  // sub: 4-2-4
  template < typename T > __always_inline void constexpr
  sub_QW_DW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    add_QW_DW_QW( a0, a1, a2, a3, -b0, -b1, c0, c1, c2, c3 );
  }

  // sub: 4-3-1
  template < typename T > __always_inline void constexpr
  sub_QW_TW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0 )
  {
    add_QW_TW_SW( a0, a1, a2, a3, -b0, -b1, -b2, c0 );
  }

  // sub: 4-3-2
  template < typename T > __always_inline void constexpr
  sub_QW_TW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    add_QW_TW_DW( a0, a1, a2, a3, -b0, -b1, -b2, c0, c1 );
  }

  // sub: 4-3-3
  template < typename T > __always_inline void constexpr
  sub_QW_TW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    add_QW_TW_TW( a0, a1, a2, a3, -b0, -b1, -b2, c0, c1, c2 );
  }

  // sub: 4-3-4
  template < typename T > __always_inline void constexpr
  sub_QW_TW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    add_QW_TW_QW( a0, a1, a2, a3, -b0, -b1, -b2, c0, c1, c2, c3 );
  }

  // sub: 4-4-1
  template < typename T > __always_inline void constexpr
  sub_QW_QW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    add_QW_QW_SW( a0, a1, a2, a3, -b0, -b1, -b2, -b3, c0 );
  }

  // sub: 4-4-2
  template < typename T > __always_inline void constexpr
  sub_QW_QW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    add_QW_QW_DW( a0, a1, a2, a3, -b0, -b1, -b2, -b3, c0, c1 );
  }

  // sub: 4-4-3
  template < typename T > __always_inline void constexpr
  sub_QW_QW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    add_QW_QW_TW( a0, a1, a2, a3, -b0, -b1, -b2, -b3, c0, c1, c2 );
  }

  // sub: 4-4-4
  template < typename T > __always_inline void constexpr
  sub_QW_QW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    add_QW_QW_QW( a0, a1, a2, a3, -b0, -b1, -b2, -b3, c0, c1, c2, c3 );
  }

  // mul: 1-1-2
  template < typename T > __always_inline void constexpr
  mul_SW_SW_DW( T const a0, T const b0, T &c0, T &c1 )
  {
    TwoProductFMA( a0, b0, c0, c1 );
  }

  // mul: 1-1-3
  template < typename T > __always_inline void constexpr
  mul_SW_SW_TW( T const a0, T const b0, T &c0, T &c1, T &c2 )
  {
    TwoProductFMA( a0, b0, c0, c1 );
    c2 = fp_const<T>::zero();
  }

  // mul: 1-1-4
  template < typename T > __always_inline void constexpr
  mul_SW_SW_QW( T const a0, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    TwoProductFMA( a0, b0, c0, c1 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c0, c1, c0, c1 );
    c2 = fp_const<T>::zero();
    c3 = fp_const<T>::zero();
  }

  // mul: 1-2-1
  template < typename T > __always_inline void constexpr
  mul_SW_DW_SW( T const a0, T const b0, T const b1, T &c0 )
  {
    c0 = a0 * b0;
    c0 = std::fma ( a0, b1, c0 );
  }

  // mul: 1-2-2
  template < typename T > __always_inline void constexpr
  mul_SW_DW_DW( T const a0, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    t0 = t0 + t1;
    c1 = c1 + t0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 1-2-3
  template < typename T > __always_inline void constexpr
  mul_SW_DW_TW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoSum( c2, t0, c2, t0 );
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 1-2-4
  template < typename T > __always_inline void constexpr
  mul_SW_DW_QW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 1-3-1
  template < typename T > __always_inline void constexpr
  mul_SW_TW_SW( T const a0, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0;
    c0 = a0 * b0;
    t0 = b1 + b2;
    c0 = std::fma ( a0, t0, c0 );
  }

  // mul: 1-3-2
  template < typename T > __always_inline void constexpr
  mul_SW_TW_DW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    t0 = t0 + t1;
    t0 = std::fma ( a0, b2, t0 );
    c1 = c1 + t0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 1-3-3
  template < typename T > __always_inline void constexpr
  mul_SW_TW_TW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a0, b2, t1, t2 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    t0 = t0 + t1 + t2;
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 1-3-4
  template < typename T > __always_inline void constexpr
  mul_SW_TW_QW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a0, b2, t0, t1 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t0, c2, t0 );
    FastTwoSum( c1, c2, c1, c2 );
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t1, c3, t1 );
    t2 = t0 + t1;
    c3 = c3 + t2;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 1-4-1
  template < typename T > __always_inline void constexpr
  mul_SW_QW_SW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0;
    c0 = a0 * b0;
    t0 = b1 + b2 + b3;
    c0 = std::fma ( a0, t0, c0 );
  }

  // mul: 1-4-2
  template < typename T > __always_inline void constexpr
  mul_SW_QW_DW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    t0 = t0 + t1;
    t2 = b2 + b3;
    t0 = std::fma ( a0, t2, t0 );
    c1 = c1 + t0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 1-4-3
  template < typename T > __always_inline void constexpr
  mul_SW_QW_TW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a0, b2, t1, t2 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    t0 = t0 + t1 + t2;
    t0 = std::fma ( a0, b3, t0 );
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 1-4-4
  template < typename T > __always_inline void constexpr
  mul_SW_QW_QW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a0, b2, t0, t1 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t0, c2, t0 );
    FastTwoSum( c1, c2, c1, c2 );
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t1, c3, t1 );
    t2 = t0 + t1;
    TwoProductFMA( a0, b3, t3, t0 );
    TwoSum( c3, t3, c3, t3 );
    t4 = t2 + t3 + t0;
    c3 = c3 + t4;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 2-1-1
  template < typename T > __always_inline void constexpr
  mul_DW_SW_SW( T const a0, T const a1, T const b0, T &c0 )
  {
    c0 = a0 * b0;
    c0 = std::fma ( b0, a1, c0 );
  }

  // mul: 2-1-2
  template < typename T > __always_inline void constexpr
  mul_DW_SW_DW( T const a0, T const a1, T const b0, T &c0, T &c1 )
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, t0, c1, t2 );
    FastTwoSum( c0, c1, c0, c1 );
    t2 = t2 + t1;
    c1 = c1 + t2;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 2-1-3
  template < typename T > __always_inline void constexpr
  mul_DW_SW_TW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2 )
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoSum( t0, t1, c2, t1 );
    c2 = c2 + t1;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 2-1-4
  template < typename T > __always_inline void constexpr
  mul_DW_SW_QW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoSum( t0, t1, c2, t1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, t1, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 2-2-1
  template < typename T > __always_inline void constexpr
  mul_DW_DW_SW( T const a0, T const a1, T const b0, T const b1, T &c0 )
  {
    c0 = a0 * b0;
    c0 = std::fma ( a0, b1, c0 );
    c0 = std::fma ( b0, a1, c0 );
    c0 = std::fma ( a1, b1, c0 );
  }

  // mul: 2-2-2
  template < typename T > __always_inline void constexpr
  mul_DW_DW_DW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, t0, t1 );
    TwoProductFMA( a1, b0, t2, t3 );
    TwoSum( t0, t2, t0, t2 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    t1 = t1 + t3;
    t0 = t0 + t1 + t2;
    t0 = std::fma ( a1, b1, t0 );
    c1 = c1 + t0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 2-2-3
  template < typename T > __always_inline void constexpr
  mul_DW_DW_TW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3, t4;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a1, b1, t3, t4 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t3, c2, t3 );
    t0 = t0 + t1 + t2 + t3 + t4;
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 2-2-4
  template < typename T > __always_inline void constexpr
  mul_DW_DW_QW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a1, b1, t2, t3 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    FastTwoSum( c1, c2, c1, c2 );
    TwoSum( c3, t2, c3, t2 );
    TwoSum( c3, t1, c3, t1 );
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t3, c3, t3 );
    t0 = t0 + t1 + t2 + t3;
    c3 = c3 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 2-3-1
  template < typename T > __always_inline void constexpr
  mul_DW_TW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0;
    c0 = a0 * b0;
    t0 = b1 + b2;
    c0 = std::fma ( a0, t0, c0 );
    c0 = std::fma ( b0, a1, c0 );
    c0 = std::fma ( a1, t0, c0 );
  }

  // mul: 2-3-2
  template < typename T > __always_inline void constexpr
  mul_DW_TW_DW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, t0, t1 );
    TwoProductFMA( a1, b0, t2, t3 );
    TwoSum( t0, t2, t0, t2 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    t1 = t1 + t3;
    t0 = t0 + t1 + t2;
    t0 = std::fma ( a1, b1, t0 );
    t0 = std::fma ( a0, b2, t0 );
    c1 = c1 + t0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 2-3-3
  template < typename T > __always_inline void constexpr
  mul_DW_TW_TW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3, t4, t5, t6;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a0, b2, t3, t4 );
    TwoProductFMA( a1, b1, t5, t6 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t3, c2, t3 );
    TwoSum( c2, t5, c2, t5 );
    t0 = t0 + t1 + t2 + t3 + t4 + t5 + t6;
    t0 = std::fma ( a1, b2, t0 );
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 2-3-4
  template < typename T > __always_inline void constexpr
  mul_DW_TW_QW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5, t6;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a0, b2, t2, t3 );
    TwoProductFMA( a1, b1, t4, t5 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t4, c2, t4 );
    FastTwoSum( c1, c2, c1, c2 );
    TwoSum( c3, t4, c3, t4 );
    TwoSum( c3, t2, c3, t2 );
    TwoSum( c3, t1, c3, t1 );
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t3, c3, t3 );
    TwoSum( c3, t5, c3, t5 );
    t0 = t0 + t1 + t2 + t3 + t4 + t5;
    TwoProductFMA( a1, b2, t3, t4 );
    TwoSum( c3, t3, c3, t1 );
    t6 = t0 + t1 + t4;
    FastTwoSum( c3, t6, c3, t6 );
    FastTwoSum( c2, c3, c2, c3 );
    c3 = c3 + t6;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 2-4-1
  template < typename T > __always_inline void constexpr
  mul_DW_QW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0;
    c0 = a0 * b0;
    t0 = b1 + b2 + b3;
    c0 = std::fma ( a0, t0, c0 );
    c0 = std::fma ( b0, a1, c0 );
    c0 = std::fma ( a1, t0, c0 );
  }

  // mul: 2-4-2
  template < typename T > __always_inline void constexpr
  mul_DW_QW_DW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1, t2, t3, t4;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, t0, t1 );
    TwoProductFMA( a1, b0, t2, t3 );
    TwoSum( t0, t2, t0, t2 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    t1 = t1 + t3;
    t0 = t0 + t1 + t2;
    t4 = b2 + b3;
    t0 = std::fma ( a1, b1, t0 );
    t0 = std::fma ( a0, t4, t0 );
    c1 = c1 + t0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 2-4-3
  template < typename T > __always_inline void constexpr
  mul_DW_QW_TW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3, t4, t5, t6;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a0, b2, t3, t4 );
    TwoProductFMA( a1, b1, t5, t6 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t3, c2, t3 );
    TwoSum( c2, t5, c2, t5 );
    t0 = t0 + t1 + t2 + t3 + t4 + t5 + t6;
    t0 = std::fma ( a0, b3, t0 );
    t0 = std::fma ( a1, b2, t0 );
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 2-4-4
  template < typename T > __always_inline void constexpr
  mul_DW_QW_QW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5, t6;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a0, b2, t2, t3 );
    TwoProductFMA( a1, b1, t4, t5 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t4, c2, t4 );
    FastTwoSum( c1, c2, c1, c2 );
    TwoSum( c3, t4, c3, t4 );
    TwoSum( c3, t2, c3, t2 );
    TwoSum( c3, t1, c3, t1 );
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t3, c3, t3 );
    TwoSum( c3, t5, c3, t5 );
    t0 = t0 + t1 + t2 + t3 + t4 + t5;
    TwoProductFMA( a0, b3, t1, t2 );
    TwoProductFMA( a1, b2, t3, t4 );
    TwoSum( t1, t3, t1, t3 );
    TwoSum( c3, t1, c3, t1 );
    t6 = t0 + t1 + t2 + t3 + t4;
    FastTwoSum( c3, t6, c3, t6 );
    FastTwoSum( c2, c3, c2, c3 );
    t0 = std::fma ( a1, b3, t6 );
    c3 = c3 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 3-1-1
  template < typename T > __always_inline void constexpr
  mul_TW_SW_SW( T const a0, T const a1, T const a2, T const b0, T &c0 )
  {
    T t0;
    c0 = a0 * b0;
    t0 = a1 + a2;
    c0 = std::fma ( b0, t0, c0 );
  }

  // mul: 3-1-2
  template < typename T > __always_inline void constexpr
  mul_TW_SW_DW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1 )
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, t0, c1, t2 );
    FastTwoSum( c0, c1, c0, c1 );
    t2 = t2 + t1;
    t3 = std::fma ( b0, a2, t2 );
    c1 = c1 + t3;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 3-1-3
  template < typename T > __always_inline void constexpr
  mul_TW_SW_TW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3, t4;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a2, b0, t2, t3 );
    TwoSum( t0, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    t4 = t1 + t2 + t3;
    c2 = c2 + t4;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 3-1-4
  template < typename T > __always_inline void constexpr
  mul_TW_SW_QW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a2, b0, t2, t3 );
    TwoSum( t0, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    FastTwoSum( c1, c2, c1, c2 );
    TwoSum( t2, t1, c3, t1 );
    TwoSum( c3, t3, c3, t3 );
    t0 = t1 + t3;
    c3 = c3 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 3-2-1
  template < typename T > __always_inline void constexpr
  mul_TW_DW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0 )
  {
    T t0;
    c0 = a0 * b0;
    t0 = a1 + a2;
    c0 = std::fma ( a0, b1, c0 );
    c0 = std::fma ( b0, t0, c0 );
    c0 = std::fma ( t0, b1, c0 );
  }

  // mul: 3-2-2
  template < typename T > __always_inline void constexpr
  mul_TW_DW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, t0, t1 );
    TwoProductFMA( a1, b0, t2, t3 );
    TwoSum( t0, t2, t0, t2 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    t1 = t1 + t3;
    t0 = t0 + t1 + t2;
    t0 = std::fma ( a1, b1, t0 );
    t1 = std::fma ( b0, a2, t0 );
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 3-2-3
  template < typename T > __always_inline void constexpr
  mul_TW_DW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3, t4, t5, t6;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a2, b0, t3, t4 );
    TwoProductFMA( a1, b1, t5, t6 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t3, c2, t3 );
    TwoSum( c2, t5, c2, t5 );
    t0 = t0 + t1 + t2 + t3 + t4 + t5 + t6;
    t0 = std::fma ( a2, b1, t0 );
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 3-2-4
  template < typename T > __always_inline void constexpr
  mul_TW_DW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5, t6;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a1, b1, t2, t3 );
    TwoProductFMA( a2, b0, t4, t5 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t4, c2, t4 );
    FastTwoSum( c1, c2, c1, c2 );
    TwoSum( c3, t4, c3, t4 );
    TwoSum( c3, t2, c3, t2 );
    TwoSum( c3, t1, c3, t1 );
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t3, c3, t3 );
    TwoSum( c3, t5, c3, t5 );
    t0 = t0 + t1 + t2 + t3 + t4 + t5;
    TwoProductFMA( a2, b1, t3, t4 );
    TwoSum( c3, t3, c3, t1 );
    t6 = t0 + t1 + t4;
    FastTwoSum( c3, t6, c3, t6 );
    FastTwoSum( c2, c3, c2, c3 );
    c3 = c3 + t6;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 3-3-1
  template < typename T > __always_inline void constexpr
  mul_TW_TW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0, t1;
    c0 = a0 * b0;
    t0 = a1 + a2;
    t1 = b1 + b2;
    c0 = std::fma ( a0, t1, c0 );
    c0 = std::fma ( b0, t0, c0 );
    c0 = std::fma ( t0, t1, c0 );
  }

  // mul: 3-3-2
  template < typename T > __always_inline void constexpr
  mul_TW_TW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, t0, t1 );
    TwoProductFMA( a1, b0, t2, t3 );
    TwoSum( t0, t2, t0, t2 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    t1 = t1 + t3;
    t0 = t0 + t1 + t2;
    t0 = std::fma ( a1, b1, t0 );
    t0 = t0 * fp_const<T>::half();
    t1 = std::fma ( a0, b2, t0 );
    t2 = std::fma ( b0, a2, t0 );
    t1 = t1 + t2;
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 3-3-3
  template < typename T > __always_inline void constexpr
  mul_TW_TW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3, t4, t5, t6, t7, t8;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a0, b2, t3, t4 );
    TwoProductFMA( a2, b0, t5, t6 );
    TwoProductFMA( a1, b1, t7, t8 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t3, c2, t3 );
    TwoSum( c2, t5, c2, t5 );
    TwoSum( c2, t7, c2, t7 );
    t0 = t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8;
    t0 = t0 * fp_const<T>::half();
    t1 = std::fma ( a1, b2, t0 );
    t2 = std::fma ( a2, b1, t0 );
    t0 = t1 + t2;
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 3-3-4
  template < typename T > __always_inline void constexpr
  mul_TW_TW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5, t6, t7, t8;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a0, b2, t2, t3 );
    TwoProductFMA( a1, b1, t4, t5 );
    TwoProductFMA( a2, b0, t6, t7 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t4, c2, t4 );
    TwoSum( c2, t6, c2, t6 );
    FastTwoSum( c1, c2, c1, c2 );
    TwoSum( c3, t6, c3, t6 );
    TwoSum( c3, t4, c3, t4 );
    TwoSum( c3, t2, c3, t2 );
    TwoSum( c3, t1, c3, t1 );
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t3, c3, t3 );
    TwoSum( c3, t5, c3, t5 );
    TwoSum( c3, t7, c3, t7 );
    t0 = t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7;
    TwoProductFMA( a1, b2, t3, t4 );
    TwoProductFMA( a2, b1, t5, t6 );
    TwoSum( t3, t5, t3, t5 );
    TwoSum( c3, t3, c3, t1 );
    t4 = t4 + t6;
    t8 = t0 + t1 + t4 + t5;
    FastTwoSum( c3, t8, c3, t8 );
    FastTwoSum( c2, c3, c2, c3 );
    t0 = std::fma ( a2, b2, t8 );
    c3 = c3 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 3-4-1
  template < typename T > __always_inline void constexpr
  mul_TW_QW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0, t1;
    c0 = a0 * b0;
    t0 = a1 + a2;
    t1 = b1 + b2 + b3;
    c0 = std::fma ( a0, t1, c0 );
    c0 = std::fma ( b0, t0, c0 );
    c0 = std::fma ( t0, t1, c0 );
  }

  // mul: 3-4-2
  template < typename T > __always_inline void constexpr
  mul_TW_QW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1, t2, t3, t4;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, t0, t1 );
    TwoProductFMA( a1, b0, t2, t3 );
    TwoSum( t0, t2, t0, t2 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    t1 = t1 + t3;
    t0 = t0 + t1 + t2;
    t4 = b2 + b3;
    t0 = std::fma ( a1, b1, t0 );
    t0 = std::fma ( a0, t4, t0 );
    t1 = std::fma ( b0, a2, t0 );
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 3-4-3
  template < typename T > __always_inline void constexpr
  mul_TW_QW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3, t4, t5, t6, t7, t8;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a0, b2, t3, t4 );
    TwoProductFMA( a2, b0, t5, t6 );
    TwoProductFMA( a1, b1, t7, t8 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t3, c2, t3 );
    TwoSum( c2, t5, c2, t5 );
    TwoSum( c2, t7, c2, t7 );
    t0 = t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8;
    t0 = std::fma ( a0, b3, t0 );
    t0 = std::fma ( a1, b2, t0 );
    t0 = std::fma ( a2, b1, t0 );
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 3-4-4
  template < typename T > __always_inline void constexpr
  mul_TW_QW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5, t6, t7, t8;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a0, b2, t2, t3 );
    TwoProductFMA( a1, b1, t4, t5 );
    TwoProductFMA( a2, b0, t6, t7 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t4, c2, t4 );
    TwoSum( c2, t6, c2, t6 );
    FastTwoSum( c1, c2, c1, c2 );
    TwoSum( c3, t6, c3, t6 );
    TwoSum( c3, t4, c3, t4 );
    TwoSum( c3, t2, c3, t2 );
    TwoSum( c3, t1, c3, t1 );
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t3, c3, t3 );
    TwoSum( c3, t5, c3, t5 );
    TwoSum( c3, t7, c3, t7 );
    t0 = t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7;
    TwoProductFMA( a0, b3, t1, t2 );
    TwoProductFMA( a1, b2, t3, t4 );
    TwoProductFMA( a2, b1, t5, t6 );
    TwoSum( t3, t5, t3, t5 );
    TwoSum( t1, t3, t1, t3 );
    TwoSum( c3, t1, c3, t1 );
    t8 = t0 + t1 + t2 + t3 + t4 + t5 + t6;
    FastTwoSum( c3, t8, c3, t8 );
    FastTwoSum( c2, c3, c2, c3 );
    t0 = std::fma ( a1, b3, t8 );
    t0 = std::fma ( a2, b2, t0 );
    c3 = c3 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 4-1-1
  template < typename T > __always_inline void constexpr
  mul_QW_SW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0 )
  {
    T t0;
    c0 = a0 * b0;
    t0 = a1 + a2 + a3;
    c0 = std::fma ( b0, t0, c0 );
  }

  // mul: 4-1-2
  template < typename T > __always_inline void constexpr
  mul_QW_SW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1 )
  {
    T t0, t1, t2, t3, t4;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, t0, c1, t2 );
    FastTwoSum( c0, c1, c0, c1 );
    t2 = t2 + t1;
    t3 = a2 + a3;
    t4 = std::fma ( b0, t3, t2 );
    c1 = c1 + t4;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 4-1-3
  template < typename T > __always_inline void constexpr
  mul_QW_SW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3, t4;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a2, b0, t2, t3 );
    TwoSum( t0, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    t4 = t1 + t2 + t3;
    t4 = std::fma ( a3, b0, t4 );
    c2 = c2 + t4;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 4-1-4
  template < typename T > __always_inline void constexpr
  mul_QW_SW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a2, b0, t2, t3 );
    TwoSum( t0, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    FastTwoSum( c1, c2, c1, c2 );
    TwoSum( t2, t1, c3, t1 );
    TwoSum( c3, t3, c3, t3 );
    t0 = t1 + t3;
    TwoProductFMA( a3, b0, t3, t4 );
    TwoSum( c3, t3, c3, t1 );
    t5 = t0 + t1 + t4;
    c3 = c3 + t5;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 4-2-1
  template < typename T > __always_inline void constexpr
  mul_QW_DW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0 )
  {
    T t0;
    c0 = a0 * b0;
    t0 = a1 + a2 + a3;
    c0 = std::fma ( a0, b1, c0 );
    c0 = std::fma ( b0, t0, c0 );
    c0 = std::fma ( t0, b1, c0 );
  }

  // mul: 4-2-2
  template < typename T > __always_inline void constexpr
  mul_QW_DW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0, t1, t2, t3, t4;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, t0, t1 );
    TwoProductFMA( a1, b0, t2, t3 );
    TwoSum( t0, t2, t0, t2 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    t1 = t1 + t3;
    t0 = t0 + t1 + t2;
    t4 = a2 + a3;
    t0 = std::fma ( a1, b1, t0 );
    t1 = std::fma ( b0, t4, t0 );
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 4-2-3
  template < typename T > __always_inline void constexpr
  mul_QW_DW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3, t4, t5, t6;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a2, b0, t3, t4 );
    TwoProductFMA( a1, b1, t5, t6 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t3, c2, t3 );
    TwoSum( c2, t5, c2, t5 );
    t0 = t0 + t1 + t2 + t3 + t4 + t5 + t6;
    t0 = std::fma ( a3, b0, t0 );
    t0 = std::fma ( a2, b1, t0 );
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 4-2-4
  template < typename T > __always_inline void constexpr
  mul_QW_DW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5, t6, t7, t8;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a1, b1, t2, t3 );
    TwoProductFMA( a2, b0, t4, t5 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t4, c2, t4 );
    FastTwoSum( c1, c2, c1, c2 );
    TwoSum( c3, t4, c3, t4 );
    TwoSum( c3, t2, c3, t2 );
    TwoSum( c3, t1, c3, t1 );
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t3, c3, t3 );
    TwoSum( c3, t5, c3, t5 );
    t0 = t0 + t1 + t2 + t3 + t4 + t5;
    TwoProductFMA( a2, b1, t3, t4 );
    TwoProductFMA( a3, b0, t5, t6 );
    TwoSum( t5, t3, t1, t7 );
    TwoSum( c3, t1, c3, t1 );
    t8 = t0 + t1 + t7 + t4 + t6;
    FastTwoSum( c3, t8, c3, t8 );
    FastTwoSum( c2, c3, c2, c3 );
    t0 = std::fma ( a3, b1, t8 );
    c3 = c3 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 4-3-1
  template < typename T > __always_inline void constexpr
  mul_QW_TW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0, t1;
    c0 = a0 * b0;
    t0 = a1 + a2 + a3;
    t1 = b1 + b2;
    c0 = std::fma ( a0, t1, c0 );
    c0 = std::fma ( b0, t0, c0 );
    c0 = std::fma ( t0, t1, c0 );
  }

  // mul: 4-3-2
  template < typename T > __always_inline void constexpr
  mul_QW_TW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1, t2, t3, t4;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, t0, t1 );
    TwoProductFMA( a1, b0, t2, t3 );
    TwoSum( t0, t2, t0, t2 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    t1 = t1 + t3;
    t0 = t0 + t1 + t2;
    t4 = a2 + a3;
    t0 = std::fma ( a1, b1, t0 );
    t0 = std::fma ( a0, b2, t0 );
    t1 = std::fma ( b0, t4, t0 );
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 4-3-3
  template < typename T > __always_inline void constexpr
  mul_QW_TW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3, t4, t5, t6, t7, t8;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a0, b2, t3, t4 );
    TwoProductFMA( a2, b0, t5, t6 );
    TwoProductFMA( a1, b1, t7, t8 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t3, c2, t3 );
    TwoSum( c2, t5, c2, t5 );
    TwoSum( c2, t7, c2, t7 );
    t0 = t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8;
    t0 = std::fma ( a3, b0, t0 );
    t0 = std::fma ( a1, b2, t0 );
    t0 = std::fma ( a2, b1, t0 );
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 4-3-4
  template < typename T > __always_inline void constexpr
  mul_QW_TW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5, t6, t7, t8, t9;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a0, b2, t2, t3 );
    TwoProductFMA( a1, b1, t4, t5 );
    TwoProductFMA( a2, b0, t6, t7 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t4, c2, t4 );
    TwoSum( c2, t6, c2, t6 );
    FastTwoSum( c1, c2, c1, c2 );
    TwoSum( c3, t6, c3, t6 );
    TwoSum( c3, t4, c3, t4 );
    TwoSum( c3, t2, c3, t2 );
    TwoSum( c3, t1, c3, t1 );
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t3, c3, t3 );
    TwoSum( c3, t5, c3, t5 );
    TwoSum( c3, t7, c3, t7 );
    t0 = t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7;
    TwoProductFMA( a1, b2, t3, t4 );
    TwoProductFMA( a2, b1, t5, t6 );
    TwoProductFMA( a3, b0, t7, t8 );
    TwoSum( t3, t5, t3, t5 );
    TwoSum( t7, t3, t1, t3 );
    TwoSum( c3, t1, c3, t1 );
    t9 = t0 + t1 + t3 + t4 + t5 + t6 + t8;
    FastTwoSum( c3, t9, c3, t9 );
    FastTwoSum( c2, c3, c2, c3 );
    t0 = std::fma ( a2, b2, t9 );
    t0 = std::fma ( a3, b1, t0 );
    c3 = c3 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 4-4-1
  template < typename T > __always_inline void constexpr
  mul_QW_QW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0, t1;
    c0 = a0 * b0;
    t0 = a1 + a2 + a3;
    t1 = b1 + b2 + b3;
    c0 = std::fma ( a0, t1, c0 );
    c0 = std::fma ( b0, t0, c0 );
    c0 = std::fma ( t0, t1, c0 );
  }

  // mul: 4-4-2
  template < typename T > __always_inline void constexpr
  mul_QW_QW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1, t2, t3, t4, t5;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, t0, t1 );
    TwoProductFMA( a1, b0, t2, t3 );
    TwoSum( t0, t2, t0, t2 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    t1 = t1 + t3;
    t0 = t0 + t1 + t2;
    t4 = a2 + a3;
    t5 = b2 + b3;
    t0 = std::fma ( a1, b1, t0 );
    t0 = t0 * fp_const<T>::half();
    t1 = std::fma ( a0, t5, t0 );
    t2 = std::fma ( b0, t4, t0 );
    t1 = t1 + t2;
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 4-4-3
  template < typename T > __always_inline void constexpr
  mul_QW_QW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3, t4, t5, t6, t7, t8;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a0, b2, t3, t4 );
    TwoProductFMA( a2, b0, t5, t6 );
    TwoProductFMA( a1, b1, t7, t8 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t3, c2, t3 );
    TwoSum( c2, t5, c2, t5 );
    TwoSum( c2, t7, c2, t7 );
    t0 = t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8;
    t0 = t0 * fp_const<T>::half();
    t1 = std::fma ( a0, b3, t0 );
    t2 = std::fma ( a3, b0, t0 );
    t1 = std::fma ( a1, b2, t1 );
    t2 = std::fma ( a2, b1, t2 );
    t0 = t1 + t2;
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 4-4-4
  template < typename T > __always_inline void constexpr
  mul_QW_QW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t0, c1, t0 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a0, b2, t2, t3 );
    TwoProductFMA( a1, b1, t4, t5 );
    TwoProductFMA( a2, b0, t6, t7 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t4, c2, t4 );
    TwoSum( c2, t6, c2, t6 );
    FastTwoSum( c1, c2, c1, c2 );
    TwoSum( c3, t6, c3, t6 );
    TwoSum( c3, t4, c3, t4 );
    TwoSum( c3, t2, c3, t2 );
    TwoSum( c3, t1, c3, t1 );
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t3, c3, t3 );
    TwoSum( c3, t5, c3, t5 );
    TwoSum( c3, t7, c3, t7 );
    t0 = t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7;
    TwoProductFMA( a0, b3, t1, t2 );
    TwoProductFMA( a1, b2, t3, t4 );
    TwoProductFMA( a2, b1, t5, t6 );
    TwoProductFMA( a3, b0, t7, t8 );
    TwoSum( t1, t7, t1, t7 );
    TwoSum( t3, t5, t3, t5 );
    TwoSum( t1, t3, t1, t3 );
    TwoSum( c3, t1, c3, t1 );
    t2 = t2 + t8;
    t4 = t4 + t6;
    t9 = t0 + t1 + t2 + t3 + t4 + t5 + t7;
    FastTwoSum( c3, t9, c3, t9 );
    FastTwoSum( c2, c3, c2, c3 );
    t0 = t9 * fp_const<T>::half();
    t10 = std::fma ( a1, b3, t0 );
    t11 = std::fma ( a3, b1, t0 );
    t0 = t10 + t11;
    t0 = std::fma ( a2, b2, t0 );
    c3 = c3 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 1-1-2
  template < typename T > __always_inline void constexpr
  div_SW_SW_DW( T const a0, T const b0, T &c0, T &c1 )
  {
    T r = a0;
    c0 = r / b0;
    r = std::fma( -b0, c0, r );
    c1 = r / b0;
  }

  // div: 1-1-3
  template < typename T > __always_inline void constexpr
  div_SW_SW_TW( T const a0, T const b0, T &c0, T &c1, T &c2 )
  {
    T r = a0;
    c0 = r / b0;
    r = std::fma( -b0, c0, r );
    c1 = r / b0;
    r = std::fma( -b0, c1, r );
    c2 = r / b0;
  }

  // div: 1-1-4
  template < typename T > __always_inline void constexpr
  div_SW_SW_QW( T const a0, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    T r = a0;
    c0 = r / b0;
    r = std::fma( -b0, c0, r );
    c1 = r / b0;
    r = std::fma( -b0, c1, r );
    c2 = r / b0;
    r = std::fma( -b0, c2, r );
    c3 = r / b0;
  }

  // div: 1-2-1
  template < typename T > __always_inline void constexpr
  div_SW_DW_SW( T const a0, T const b0, T const b1, T &c0 )
  {
    T t0, t1;
    T r0, r1;
    c0 = a0 / b0;
    mul_DW_SW_DW( b0, b1, c0, t0, t1 );
    sub_SW_DW_DW( a0, t0, t1, r0, r1 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 1-2-2
  template < typename T > __always_inline void constexpr
  div_SW_DW_DW( T const a0, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0, t1;
    T r0, r1;
    c0 = a0 / b0;
    mul_DW_SW_DW( b0, b1, c0, t0, t1 );
    sub_SW_DW_DW( a0, t0, t1, r0, r1 );
    c1 = r0 / b0;
    mul_DW_SW_DW( b0, b1, c1, t0, t1 );
    sub_DW_DW_DW( r0, r1, t0, t1, r0, r1 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 1-2-3
  template < typename T > __always_inline void constexpr
  div_SW_DW_TW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_DW_SW_TW( b0, b1, c0, t0, t1, t2 );
    sub_SW_TW_TW( a0, t0, t1, t2, r0, r1, r2 );
    c1 = r0 / b0;
    mul_DW_SW_TW( b0, b1, c1, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    c2 = r0 / b0;
    mul_DW_SW_TW( b0, b1, c2, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 1-2-4
  template < typename T > __always_inline void constexpr
  div_SW_DW_QW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_DW_SW_QW( b0, b1, c0, t0, t1, t2, t3 );
    sub_SW_QW_QW( a0, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_DW_SW_QW( b0, b1, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_DW_SW_QW( b0, b1, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c3 = r0 / b0;
    mul_DW_SW_QW( b0, b1, c3, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 1-3-1
  template < typename T > __always_inline void constexpr
  div_SW_TW_SW( T const a0, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c0, t0, t1, t2 );
    sub_SW_TW_TW( a0, t0, t1, t2, r0, r1, r2 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 1-3-2
  template < typename T > __always_inline void constexpr
  div_SW_TW_DW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c0, t0, t1, t2 );
    sub_SW_TW_TW( a0, t0, t1, t2, r0, r1, r2 );
    c1 = r0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c1, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 1-3-3
  template < typename T > __always_inline void constexpr
  div_SW_TW_TW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c0, t0, t1, t2 );
    sub_SW_TW_TW( a0, t0, t1, t2, r0, r1, r2 );
    c1 = r0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c1, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    c2 = r0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c2, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 1-3-4
  template < typename T > __always_inline void constexpr
  div_SW_TW_QW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c0, t0, t1, t2, t3 );
    sub_SW_QW_QW( a0, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c3 = r0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c3, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 1-4-1
  template < typename T > __always_inline void constexpr
  div_SW_QW_SW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_SW_QW_QW( a0, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 1-4-2
  template < typename T > __always_inline void constexpr
  div_SW_QW_DW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_SW_QW_QW( a0, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 1-4-3
  template < typename T > __always_inline void constexpr
  div_SW_QW_TW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_SW_QW_QW( a0, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 1-4-4
  template < typename T > __always_inline void constexpr
  div_SW_QW_QW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_SW_QW_QW( a0, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c3 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c3, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-1-1
  template < typename T > __always_inline void constexpr
  div_DW_SW_SW( T const a0, T const a1, T const b0, T &c0 )
  {
    T t0, t1;
    T r0, r1;
    c0 = a0 / b0;
    mul_SW_SW_DW( b0, c0, t0, t1 );
    sub_DW_DW_DW( a0, a1, t0, t1, r0, r1 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 2-1-2
  template < typename T > __always_inline void constexpr
  div_DW_SW_DW( T const a0, T const a1, T const b0, T &c0, T &c1 )
  {
    T t0, t1;
    T r0, r1;
    c0 = a0 / b0;
    mul_SW_SW_DW( b0, c0, t0, t1 );
    sub_DW_DW_DW( a0, a1, t0, t1, r0, r1 );
    c1 = r0 / b0;
    mul_SW_SW_DW( b0, c1, t0, t1 );
    sub_DW_DW_DW( r0, r1, t0, t1, r0, r1 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-1-3
  template < typename T > __always_inline void constexpr
  div_DW_SW_TW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_SW_SW_TW( b0, c0, t0, t1, t2 );
    sub_DW_TW_TW( a0, a1, t0, t1, t2, r0, r1, r2 );
    c1 = r0 / b0;
    mul_SW_SW_TW( b0, c1, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    c2 = r0 / b0;
    mul_SW_SW_TW( b0, c2, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-1-4
  template < typename T > __always_inline void constexpr
  div_DW_SW_QW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_SW_SW_QW( b0, c0, t0, t1, t2, t3 );
    sub_DW_QW_QW( a0, a1, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_SW_SW_QW( b0, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_SW_SW_QW( b0, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c3 = r0 / b0;
    mul_SW_SW_QW( b0, c3, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-2-1
  template < typename T > __always_inline void constexpr
  div_DW_DW_SW( T const a0, T const a1, T const b0, T const b1, T &c0 )
  {
    T t0, t1;
    T r0, r1;
    c0 = a0 / b0;
    mul_DW_SW_DW( b0, b1, c0, t0, t1 );
    sub_DW_DW_DW( a0, a1, t0, t1, r0, r1 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 2-2-2
  template < typename T > __always_inline void constexpr
  div_DW_DW_DW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1 )
  {
    c0 = a0 / (b0 + b1);
    c1 = (std::fma(-b0, c0, a0 ) + std::fma(-b1, c0, a1 )) / (b0 + b1);
  }

  // div: 2-2-3
  template < typename T > __always_inline void constexpr
  div_DW_DW_TW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_DW_SW_TW( b0, b1, c0, t0, t1, t2 );
    sub_DW_TW_TW( a0, a1, t0, t1, t2, r0, r1, r2 );
    c1 = r0 / b0;
    mul_DW_SW_TW( b0, b1, c1, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    c2 = r0 / b0;
    mul_DW_SW_TW( b0, b1, c2, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-2-4
  template < typename T > __always_inline void constexpr
  div_DW_DW_QW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_DW_SW_QW( b0, b1, c0, t0, t1, t2, t3 );
    sub_DW_QW_QW( a0, a1, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_DW_SW_QW( b0, b1, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_DW_SW_QW( b0, b1, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c3 = r0 / b0;
    mul_DW_SW_QW( b0, b1, c3, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-3-1
  template < typename T > __always_inline void constexpr
  div_DW_TW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c0, t0, t1, t2 );
    sub_DW_TW_TW( a0, a1, t0, t1, t2, r0, r1, r2 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 2-3-2
  template < typename T > __always_inline void constexpr
  div_DW_TW_DW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c0, t0, t1, t2 );
    sub_DW_TW_TW( a0, a1, t0, t1, t2, r0, r1, r2 );
    c1 = r0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c1, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-3-3
  template < typename T > __always_inline void constexpr
  div_DW_TW_TW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c0, t0, t1, t2 );
    sub_DW_TW_TW( a0, a1, t0, t1, t2, r0, r1, r2 );
    c1 = r0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c1, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    c2 = r0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c2, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-3-4
  template < typename T > __always_inline void constexpr
  div_DW_TW_QW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c0, t0, t1, t2, t3 );
    sub_DW_QW_QW( a0, a1, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c3 = r0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c3, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-4-1
  template < typename T > __always_inline void constexpr
  div_DW_QW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_DW_QW_QW( a0, a1, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 2-4-2
  template < typename T > __always_inline void constexpr
  div_DW_QW_DW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_DW_QW_QW( a0, a1, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-4-3
  template < typename T > __always_inline void constexpr
  div_DW_QW_TW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_DW_QW_QW( a0, a1, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-4-4
  template < typename T > __always_inline void constexpr
  div_DW_QW_QW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_DW_QW_QW( a0, a1, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c3 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c3, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-1-1
  template < typename T > __always_inline void constexpr
  div_TW_SW_SW( T const a0, T const a1, T const a2, T const b0, T &c0 )
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_SW_SW_TW( b0, c0, t0, t1, t2 );
    sub_TW_TW_TW( a0, a1, a2, t0, t1, t2, r0, r1, r2 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 3-1-2
  template < typename T > __always_inline void constexpr
  div_TW_SW_DW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1 )
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_SW_SW_TW( b0, c0, t0, t1, t2 );
    sub_TW_TW_TW( a0, a1, a2, t0, t1, t2, r0, r1, r2 );
    c1 = r0 / b0;
    mul_SW_SW_TW( b0, c1, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-1-3
  template < typename T > __always_inline void constexpr
  div_TW_SW_TW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_SW_SW_TW( b0, c0, t0, t1, t2 );
    sub_TW_TW_TW( a0, a1, a2, t0, t1, t2, r0, r1, r2 );
    c1 = r0 / b0;
    mul_SW_SW_TW( b0, c1, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    c2 = r0 / b0;
    mul_SW_SW_TW( b0, c2, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-1-4
  template < typename T > __always_inline void constexpr
  div_TW_SW_QW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_SW_SW_QW( b0, c0, t0, t1, t2, t3 );
    sub_TW_QW_QW( a0, a1, a2, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_SW_SW_QW( b0, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_SW_SW_QW( b0, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c3 = r0 / b0;
    mul_SW_SW_QW( b0, c3, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-2-1
  template < typename T > __always_inline void constexpr
  div_TW_DW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0 )
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_DW_SW_TW( b0, b1, c0, t0, t1, t2 );
    sub_TW_TW_TW( a0, a1, a2, t0, t1, t2, r0, r1, r2 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 3-2-2
  template < typename T > __always_inline void constexpr
  div_TW_DW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_DW_SW_TW( b0, b1, c0, t0, t1, t2 );
    sub_TW_TW_TW( a0, a1, a2, t0, t1, t2, r0, r1, r2 );
    c1 = r0 / b0;
    mul_DW_SW_TW( b0, b1, c1, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-2-3
  template < typename T > __always_inline void constexpr
  div_TW_DW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_DW_SW_TW( b0, b1, c0, t0, t1, t2 );
    sub_TW_TW_TW( a0, a1, a2, t0, t1, t2, r0, r1, r2 );
    c1 = r0 / b0;
    mul_DW_SW_TW( b0, b1, c1, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    c2 = r0 / b0;
    mul_DW_SW_TW( b0, b1, c2, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-2-4
  template < typename T > __always_inline void constexpr
  div_TW_DW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_DW_SW_QW( b0, b1, c0, t0, t1, t2, t3 );
    sub_TW_QW_QW( a0, a1, a2, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_DW_SW_QW( b0, b1, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_DW_SW_QW( b0, b1, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c3 = r0 / b0;
    mul_DW_SW_QW( b0, b1, c3, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-3-1
  template < typename T > __always_inline void constexpr
  div_TW_TW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c0, t0, t1, t2 );
    sub_TW_TW_TW( a0, a1, a2, t0, t1, t2, r0, r1, r2 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 3-3-2
  template < typename T > __always_inline void constexpr
  div_TW_TW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c0, t0, t1, t2 );
    sub_TW_TW_TW( a0, a1, a2, t0, t1, t2, r0, r1, r2 );
    c1 = r0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c1, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-3-3
  template < typename T > __always_inline void constexpr
  div_TW_TW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c0, t0, t1, t2 );
    sub_TW_TW_TW( a0, a1, a2, t0, t1, t2, r0, r1, r2 );
    c1 = r0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c1, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    c2 = r0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c2, t0, t1, t2 );
    sub_TW_TW_TW( r0, r1, r2, t0, t1, t2, r0, r1, r2 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-3-4
  template < typename T > __always_inline void constexpr
  div_TW_TW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c0, t0, t1, t2, t3 );
    sub_TW_QW_QW( a0, a1, a2, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c3 = r0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c3, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-4-1
  template < typename T > __always_inline void constexpr
  div_TW_QW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_TW_QW_QW( a0, a1, a2, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 3-4-2
  template < typename T > __always_inline void constexpr
  div_TW_QW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_TW_QW_QW( a0, a1, a2, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-4-3
  template < typename T > __always_inline void constexpr
  div_TW_QW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_TW_QW_QW( a0, a1, a2, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-4-4
  template < typename T > __always_inline void constexpr
  div_TW_QW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_TW_QW_QW( a0, a1, a2, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c3 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c3, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-1-1
  template < typename T > __always_inline void constexpr
  div_QW_SW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_SW_SW_QW( b0, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 4-1-2
  template < typename T > __always_inline void constexpr
  div_QW_SW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_SW_SW_QW( b0, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_SW_SW_QW( b0, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-1-3
  template < typename T > __always_inline void constexpr
  div_QW_SW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_SW_SW_QW( b0, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_SW_SW_QW( b0, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_SW_SW_QW( b0, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-1-4
  template < typename T > __always_inline void constexpr
  div_QW_SW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_SW_SW_QW( b0, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_SW_SW_QW( b0, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_SW_SW_QW( b0, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c3 = r0 / b0;
    mul_SW_SW_QW( b0, c3, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-2-1
  template < typename T > __always_inline void constexpr
  div_QW_DW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_DW_SW_QW( b0, b1, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 4-2-2
  template < typename T > __always_inline void constexpr
  div_QW_DW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_DW_SW_QW( b0, b1, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_DW_SW_QW( b0, b1, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-2-3
  template < typename T > __always_inline void constexpr
  div_QW_DW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_DW_SW_QW( b0, b1, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_DW_SW_QW( b0, b1, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_DW_SW_QW( b0, b1, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-2-4
  template < typename T > __always_inline void constexpr
  div_QW_DW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_DW_SW_QW( b0, b1, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_DW_SW_QW( b0, b1, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_DW_SW_QW( b0, b1, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c3 = r0 / b0;
    mul_DW_SW_QW( b0, b1, c3, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-3-1
  template < typename T > __always_inline void constexpr
  div_QW_TW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 4-3-2
  template < typename T > __always_inline void constexpr
  div_QW_TW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-3-3
  template < typename T > __always_inline void constexpr
  div_QW_TW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-3-4
  template < typename T > __always_inline void constexpr
  div_QW_TW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c3 = r0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c3, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-4-1
  template < typename T > __always_inline void constexpr
  div_QW_QW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 4-4-2
  template < typename T > __always_inline void constexpr
  div_QW_QW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-4-3
  template < typename T > __always_inline void constexpr
  div_QW_QW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-4-4
  template < typename T > __always_inline void constexpr
  div_QW_QW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c1, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c2 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c2, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c3 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c3, t0, t1, t2, t3 );
    sub_QW_QW_QW( r0, r1, r2, r3, t0, t1, t2, t3, r0, r1, r2, r3 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // sqr: 1-2
  template < typename T > __always_inline void constexpr
  sqr_SW_DW( T const a0, T &c0, T &c1 )
  {
    TwoProductFMA( a0, a0, c0, c1 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // sqr: 1-3
  template < typename T > __always_inline void constexpr
  sqr_SW_TW( T const a0, T &c0, T &c1, T &c2 )
  {
    TwoProductFMA( a0, a0, c0, c1 );
    FastTwoSum( c0, c1, c0, c1 );
    c2 = fp_const<T>::zero();
  }

  // sqr: 1-4
  template < typename T > __always_inline void constexpr
  sqr_SW_QW( T const a0, T &c0, T &c1, T &c2, T &c3 )
  {
    TwoProductFMA( a0, a0, c0, c1 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c0, c1, c0, c1 );
    c2 = fp_const<T>::zero();
    c3 = fp_const<T>::zero();
  }

  // sqr: 2-1
  template < typename T > __always_inline void constexpr
  sqr_DW_SW( T const a0, T const a1, T &c0 )
  {
    T t0;
    c0 = a0 * a0;
    t0 = a0 + a0;
    c0 = std::fma ( t0, a1, c0 );
  }

  // sqr: 2-2
  template < typename T > __always_inline void constexpr
  sqr_DW_DW( T const a0, T const a1, T &c0, T &c1 )
  {
    T t0, t1, t2;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    TwoProductFMA( t0, a1, t1, t2 );
    TwoSum( c1, t1, c1, t1 );
    FastTwoSum( c0, c1, c0, c1 );
    t1 = t1 + t2;
    t1 = std::fma ( a1, a1, t1 );
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // sqr: 2-3
  template < typename T > __always_inline void constexpr
  sqr_DW_TW( T const a0, T const a1, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    TwoProductFMA( t0, a1, c2, t1 );
    TwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a1, a1, t2, t3 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    FastTwoSum( c1, c2, c1, c2 );
    t1 = t1 + t2 + t3;
    c2 = c2 + t1;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // sqr: 2-4
  template < typename T > __always_inline void constexpr
  sqr_DW_QW( T const a0, T const a1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    TwoProductFMA( t0, a1, c2, c3 );
    TwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    TwoProductFMA( a1, a1, t1, t2 );
    TwoSum( c2, t1, c2, t1 );
    FastTwoSum( c2, c3, c2, c3 );
    TwoSum( c3, t1, c3, t1 );
    TwoSum( c3, t2, c3, t2 );
    FastTwoSum( c2, c3, c2, c3 );
    t3 = t1 + t2;
    c3 = c3 + t3;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // sqr: 3-1
  template < typename T > __always_inline void constexpr
  sqr_TW_SW( T const a0, T const a1, T const a2, T &c0 )
  {
    T t0, t1;
    c0 = a0 * a0;
    t0 = a0 + a0;
    t1 = a1 + a2;
    c0 = std::fma ( t0, t1, c0 );
  }

  // sqr: 3-2
  template < typename T > __always_inline void constexpr
  sqr_TW_DW( T const a0, T const a1, T const a2, T &c0, T &c1 )
  {
    T t0, t1, t2;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    TwoProductFMA( t0, a1, t1, t2 );
    TwoSum( c1, t1, c1, t1 );
    FastTwoSum( c0, c1, c0, c1 );
    t1 = t1 + t2;
    t1 = std::fma ( a1, a1, t1 );
    t1 = std::fma ( a2, t0, t1 );
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // sqr: 3-3
  template < typename T > __always_inline void constexpr
  sqr_TW_TW( T const a0, T const a1, T const a2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3, t4, t5, t6;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    TwoProductFMA( t0, a1, c2, t1 );
    TwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( t0, a2, t2, t3 );
    TwoProductFMA( a1, a1, t4, t5 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t4, c2, t4 );
    FastTwoSum( c1, c2, c1, c2 );
    t1 = t1 + t2 + t3 + t4 + t5;
    t6 = a2 + a2;
    t1 = std::fma ( t6, a1, t1 );
    c2 = c2 + t1;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // sqr: 3-4
  template < typename T > __always_inline void constexpr
  sqr_TW_QW( T const a0, T const a1, T const a2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5, t6, t7;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    TwoProductFMA( t0, a1, c2, c3 );
    TwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    TwoProductFMA( t0, a2, t1, t2 );
    FastTwoSum( c2, c3, c2, c3 );
    TwoProductFMA( a1, a1, t3, t4 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t3, c2, t3 );
    FastTwoSum( c2, c3, c2, c3 );
    t5 = a1 + a1;
    TwoProductFMA( t5, a2, t6, t7 );
    TwoSum( c3, t1, c3, t1 );
    TwoSum( c3, t2, c3, t2 );
    TwoSum( c3, t3, c3, t3 );
    TwoSum( c3, t6, c3, t6 );
    TwoSum( c3, t4, c3, t4 );
    FastTwoSum( c2, c3, c2, c3 );
    t1 = t1 + t2 + t3 + t4 + t6 + t7;
    t1 = std::fma ( a2, a2, t1 );
    c3 = c3 + t1;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // sqr: 4-1
  template < typename T > __always_inline void constexpr
  sqr_QW_SW( T const a0, T const a1, T const a2, T const a3, T &c0 )
  {
    T t0, t1;
    c0 = a0 * a0;
    t0 = a0 + a0;
    t1 = a1 + a2 + a3;
    c0 = std::fma ( t0, t1, c0 );
  }

  // sqr: 4-2
  template < typename T > __always_inline void constexpr
  sqr_QW_DW( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1 )
  {
    T t0, t1, t2;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    TwoProductFMA( t0, a1, t1, t2 );
    TwoSum( c1, t1, c1, t1 );
    FastTwoSum( c0, c1, c0, c1 );
    t1 = t1 + t2;
    t1 = std::fma ( a1, a1, t1 );
    t1 = std::fma ( a2, t0, t1 );
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // sqr: 4-3
  template < typename T > __always_inline void constexpr
  sqr_QW_TW( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2, t3, t4, t5, t6;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    TwoProductFMA( t0, a1, c2, t1 );
    TwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( t0, a2, t2, t3 );
    TwoProductFMA( a1, a1, t4, t5 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t4, c2, t4 );
    FastTwoSum( c1, c2, c1, c2 );
    t1 = t1 + t2 + t3 + t4 + t5;
    t1 = std::fma ( t0, a3, t1 );
    t6 = a2 + a2;
    t1 = std::fma ( t6, a1, t1 );
    c2 = c2 + t1;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // sqr: 4-4
  template < typename T > __always_inline void constexpr
  sqr_QW_QW( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3, t4, t5, t6, t7, t8, t9;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    TwoProductFMA( t0, a1, c2, c3 );
    TwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    TwoProductFMA( t0, a2, t1, t2 );
    FastTwoSum( c2, c3, c2, c3 );
    TwoProductFMA( a1, a1, t3, t4 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t3, c2, t3 );
    FastTwoSum( c2, c3, c2, c3 );
    t5 = a1 + a1;
    TwoProductFMA( t5, a2, t6, t7 );
    TwoProductFMA( a3, t0, t8, t9 );
    TwoSum( t6, t8, t6, t8 );
    TwoSum( c3, t1, c3, t1 );
    TwoSum( c3, t2, c3, t2 );
    TwoSum( c3, t3, c3, t3 );
    TwoSum( c3, t6, c3, t6 );
    TwoSum( c3, t4, c3, t4 );
    FastTwoSum( c2, c3, c2, c3 );
    t1 = t1 + t2 + t3 + t4 + t6 + t7 + t8 + t9;
    t1 = std::fma ( a3, t5, t1 );
    t1 = std::fma ( a2, a2, t1 );
    c3 = c3 + t1;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }


  // ------------------------
  // Square Root Operations
  // ------------------------

  // sqrt: 1-1
  template < typename T > __always_inline void constexpr
  sqrt_SW_SW ( T const a0, T &c0 )
  {
    c0 = std::sqrt( a0 );
  }

  // sqrt: 1-2
  template < typename T > __always_inline void constexpr
  sqrt_SW_PA ( T const a0, T &c0, T &c1 )
  {
    c0 = std::sqrt( a0 );
    c1 = ( std::fma( -c0, c0, a0 ) ) / (c0 + c0);
  }

  // sqrt: 1-3
  template < typename T > __always_inline void constexpr
  sqrt_SW_QTW ( T const a0, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    sqrt_SW_PA ( a0, c0, c1 );
    sqr_PA_QTW ( c0, c1, t0, t1, t2 );
    sub_SW_QTW_QTW ( a0, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = c0 + c1;
    c2 = tn / td;
    c2 = c2 * fp_const<T>::half();
  }

  // sqrt: 1-4
  template < typename T > __always_inline void constexpr
  sqrt_SW_QQW ( T const a0, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    sqrt_SW_QTW ( a0, c0, c1, c2 );
    sqr_QTW_QQW ( c0, c1, c2, t0, t1, t2, t3 );
    sub_SW_QQW_QQW ( a0, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = c0 + c1 + c2;
    c3 = tn / td;
    c3 = c3 * fp_const<T>::half();
  }

  // sqrt: 2-1
  template < typename T > __always_inline void constexpr
  sqrt_PA_SW ( T const a0, T const a1, T &c0 )
  {
    c0 = std::sqrt( a0 + a1 );
  }

  // sqrt: 2-2
  template < typename T > __always_inline void constexpr
  sqrt_PA_PA ( T const a0, T const a1, T &c0, T &c1 )
  {
    c0 = std::sqrt( a0 );
    c1 = ( std::fma( -c0, c0, a0 ) + a1 ) / (c0 + c0);
  }

  // sqrt: 2-3
  template < typename T > __always_inline void constexpr
  sqrt_PA_QTW ( T const a0, T const a1, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    sqrt_PA_PA ( a0, a1, c0, c1 );
    sqr_PA_QTW ( c0, c1, t0, t1, t2 );
    sub_PA_QTW_QTW ( a0, a1, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = c0 + c1;
    c2 = tn / td;
    c2 = c2 * fp_const<T>::half();
  }

  // sqrt: 2-4
  template < typename T > __always_inline void constexpr
  sqrt_PA_QQW ( T const a0, T const a1, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    sqrt_PA_QTW ( a0, a1, c0, c1, c2 );
    sqr_QTW_QQW ( c0, c1, c2, t0, t1, t2, t3 );
    sub_PA_QQW_QQW ( a0, a1, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = c0 + c1 + c2;
    c3 = tn / td;
    c3 = c3 * fp_const<T>::half();
  }

  // sqrt: 3-1
  template < typename T > __always_inline void constexpr
  sqrt_QTW_SW ( T const a0, T const a1, T const a2, T &c0 )
  {
    c0 = std::sqrt( a0 + a1 + a2 );
  }

  // sqrt: 3-2
  template < typename T > __always_inline void constexpr
  sqrt_QTW_PA ( T const a0, T const a1, T const a2, T &c0, T &c1 )
  {
    c0 = std::sqrt( a0 );
    c1 = ( std::fma( -c0, c0, a0 ) + a1 + a2 ) / (c0 + c0);
  }

  // sqrt: 3-3
  template < typename T > __always_inline void constexpr
  sqrt_QTW_QTW ( T const a0, T const a1, T const a2, T &c0, T &c1, T &c2 )
  {
    T t0, t1, t2;
    sqrt_QTW_PA ( a0, a1, a2, c0, c1 );
    sqr_PA_QTW ( c0, c1, t0, t1, t2 );
    sub_QTW_QTW_QTW ( a0, a1, a2, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = c0 + c1;
    c2 = tn / td;
    c2 = c2 * fp_const<T>::half();
  }

  // sqrt: 3-4
  template < typename T > __always_inline void constexpr
  sqrt_QTW_QQW ( T const a0, T const a1, T const a2, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    sqrt_QTW_QTW ( a0, a1, a2, c0, c1, c2 );
    sqr_QTW_QQW ( c0, c1, c2, t0, t1, t2, t3 );
    sub_QTW_QQW_QQW ( a0, a1, a2, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = c0 + c1 + c2;
    c3 = tn / td;
    c3 = c3 * fp_const<T>::half();
  }

  // sqrt: 4-1
  template < typename T > __always_inline void constexpr
  sqrt_QQW_SW ( T const a0, T const a1, T const a2, T const a3, T &c0 )
  {
    c0 = std::sqrt( a0 + a1 + a2 + a3 );
  }

  // sqrt: 4-2
  template < typename T > __always_inline void constexpr
  sqrt_QQW_PA ( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1 )
  {
    c0 = std::sqrt( a0 );
    c1 = ( std::fma( -c0, c0, a0 ) + a1 + a2 + a3 ) / (c0 + c0);
  }

  // sqrt: 4-3
  template < typename T > __always_inline void constexpr
  sqrt_QQW_QTW ( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1, T &c2 )
  {
    sqrt_QTW_QTW ( a0, a1, a2+a3, c0, c1, c2 );
  }

  // sqrt: 4-4
  template < typename T > __always_inline void constexpr
  sqrt_QQW_QQW ( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1, T &c2, T &c3 )
  {
    T t0, t1, t2, t3;
    sqrt_QQW_QTW ( a0, a1, a2, a3, c0, c1, c2 );
    sqr_QTW_QQW ( c0, c1, c2, t0, t1, t2, t3 );
    sub_QQW_QQW_QQW ( a0, a1, a2, a3, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = c0 + c1 + c2;
    c3 = tn / td;
    c3 = c3 * fp_const<T>::half();
  }

  // sqrt: 1-2
  template < typename T > __always_inline void constexpr
  sqrt_SW_DW ( T const a0, T &c0, T &c1 )
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::half();
    T const ax0 = (a0 * ex) * ex2;
    T const h0 = 3*fp_const<T>::half();
    T t0, t1;
    T r0, r1;
    r0 = e / as;

    sqr_SW_DW ( r0, t0, t1 );
    mul_SW_DW_DW ( ax0, t0, t1, t0, t1 );
    sub_SW_DW_DW ( h0, t0, t1, t0, t1 );
    mul_DW_SW_DW ( t0, t1, r0, r0, r1 );

    mul_SW_DW_DW ( ax0, r0, r1, r0, r1 );
    c0 = r0 * e2;
    c1 = r1 * e2;
  }

  // sqrt: 1-3
  template < typename T > __always_inline void constexpr
  sqrt_SW_TW ( T const a0, T &c0, T &c1, T &c2 )
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::half();
    T const ax0 = (a0 * ex) * ex2;
    T const h0 = 3*fp_const<T>::half();
    T t0, t1, t2;
    T r0, r1, r2;
    r0 = e / as;

    sqr_SW_DW ( r0, t0, t1 );
    mul_SW_DW_TW ( ax0, t0, t1, t0, t1, t2 );
    sub_SW_TW_TW ( h0, t0, t1, t2, t0, t1, t2 );
    mul_TW_SW_TW ( t0, t1, t2, r0, r0, r1, r2 );

    sqr_TW_TW ( r0, r1, r2, t0, t1, t2 );
    mul_SW_TW_TW ( ax0, t0, t1, t2, t0, t1, t2 );
    sub_SW_TW_TW ( h0, t0, t1, t2, t0, t1, t2 );
    mul_TW_TW_TW ( t0, t1, t2, r0, r1, r2, r0, r1, r2 );

    mul_SW_TW_TW ( ax0, r0, r1, r2, r0, r1, r2 );
    c0 = r0 * e2;
    c1 = r1 * e2;
    c2 = r2 * e2;
  }

  // sqrt: 1-4
  template < typename T > __always_inline void constexpr
  sqrt_SW_QW ( T const a0, T &c0, T &c1, T &c2, T &c3 )
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::half();
    T const ax0 = (a0 * ex) * ex2;
    T const h0 = 3*fp_const<T>::half();
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    r0 = e / as;

    sqr_SW_DW ( r0, t0, t1 );
    mul_SW_DW_QW ( ax0, t0, t1, t0, t1, t2, t3 );
    sub_SW_QW_QW ( h0, t0, t1, t2, t3, t0, t1, t2, t3 );
    mul_QW_SW_QW ( t0, t1, t2, t3, r0, r0, r1, r2, r3 );

    sqr_QW_QW ( r0, r1, r2, r3, t0, t1, t2, t3 );
    mul_SW_QW_QW ( ax0, t0, t1, t2, t3, t0, t1, t2, t3 );
    sub_SW_QW_QW ( h0, t0, t1, t2, t3, t0, t1, t2, t3 );
    mul_QW_QW_QW ( t0, t1, t2, t3, r0, r1, r2, r3, r0, r1, r2, r3 );

    sqr_QW_QW ( r0, r1, r2, r3, t0, t1, t2, t3 );
    mul_SW_QW_QW ( ax0, t0, t1, t2, t3, t0, t1, t2, t3 );
    sub_SW_QW_QW ( h0, t0, t1, t2, t3, t0, t1, t2, t3 );
    mul_QW_QW_QW ( t0, t1, t2, t3, r0, r1, r2, r3, r0, r1, r2, r3 );

    mul_SW_QW_QW ( ax0, r0, r1, r2, r3, r0, r1, r2, r3 );
    c0 = r0 * e2;
    c1 = r1 * e2;
    c2 = r2 * e2;
    c3 = r3 * e2;
  }

  // sqrt: 2-1
  template < typename T > __always_inline void constexpr
  sqrt_DW_SW ( T const a0, T const a1, T &c0 )
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent ( as );
    T const ex = fp_const<T>::exponenti( as );
    T const b0 = ( a0 * ex ) * ex;
    T const b1 = ( a1 * ex ) * ex;

    T const x  = e / as;
    T const ax = b0 * x;
    T r0, r1;

    TwoProductFMA( ax, ax, r0, r1 );
    sub_DW_DW_SW( b0, b1, r0, r1, c0 );

    c0 = ax + c0 * ( x * fp_const<T>::half() );
    c0 = c0 * e;
  }

  // sqrt: 2-2
  template < typename T > __always_inline void constexpr
  sqrt_DW_DW ( T const a0, T const a1, T &c0, T &c1 )
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::half();
    T const ax0 = (a0 * ex) * ex2;
    T const ax1 = (a1 * ex) * ex2;
    T const h0 = 3*fp_const<T>::half();
    T t0, t1;
    T r0, r1;
    r0 = e / as;

    sqr_SW_DW ( r0, t0, t1 );
    mul_DW_DW_DW ( ax0, ax1, t0, t1, t0, t1 );
    sub_SW_DW_DW ( h0, t0, t1, t0, t1 );
    mul_DW_SW_DW ( t0, t1, r0, r0, r1 );

    mul_DW_DW_DW ( ax0, ax1, r0, r1, r0, r1 );
    c0 = r0 * e2;
    c1 = r1 * e2;
  }

  // sqrt: 2-3
  template < typename T > __always_inline void constexpr
  sqrt_DW_TW ( T const a0, T const a1, T &c0, T &c1, T &c2 )
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::half();
    T const ax0 = (a0 * ex) * ex2;
    T const ax1 = (a1 * ex) * ex2;
    T const h0 = 3*fp_const<T>::half();
    T t0, t1, t2;
    T r0, r1, r2;
    r0 = e / as;

    sqr_SW_DW ( r0, t0, t1 );
    mul_DW_DW_TW ( ax0, ax1, t0, t1, t0, t1, t2 );
    sub_SW_TW_TW ( h0, t0, t1, t2, t0, t1, t2 );
    mul_TW_SW_TW ( t0, t1, t2, r0, r0, r1, r2 );

    sqr_TW_TW ( r0, r1, r2, t0, t1, t2 );
    mul_DW_TW_TW ( ax0, ax1, t0, t1, t2, t0, t1, t2 );
    sub_SW_TW_TW ( h0, t0, t1, t2, t0, t1, t2 );
    mul_TW_TW_TW ( t0, t1, t2, r0, r1, r2, r0, r1, r2 );

    mul_DW_TW_TW ( ax0, ax1, r0, r1, r2, r0, r1, r2 );
    c0 = r0 * e2;
    c1 = r1 * e2;
    c2 = r2 * e2;
  }

  // sqrt: 2-4
  template < typename T > __always_inline void constexpr
  sqrt_DW_QW ( T const a0, T const a1, T &c0, T &c1, T &c2, T &c3 )
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::half();
    T const ax0 = (a0 * ex) * ex2;
    T const ax1 = (a1 * ex) * ex2;
    T const h0 = 3*fp_const<T>::half();
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    r0 = e / as;

    sqr_SW_DW ( r0, t0, t1 );
    mul_DW_DW_QW ( ax0, ax1, t0, t1, t0, t1, t2, t3 );
    sub_SW_QW_QW ( h0, t0, t1, t2, t3, t0, t1, t2, t3 );
    mul_QW_SW_QW ( t0, t1, t2, t3, r0, r0, r1, r2, r3 );

    sqr_QW_QW ( r0, r1, r2, r3, t0, t1, t2, t3 );
    mul_DW_QW_QW ( ax0, ax1, t0, t1, t2, t3, t0, t1, t2, t3 );
    sub_SW_QW_QW ( h0, t0, t1, t2, t3, t0, t1, t2, t3 );
    mul_QW_QW_QW ( t0, t1, t2, t3, r0, r1, r2, r3, r0, r1, r2, r3 );

    sqr_QW_QW ( r0, r1, r2, r3, t0, t1, t2, t3 );
    mul_DW_QW_QW ( ax0, ax1, t0, t1, t2, t3, t0, t1, t2, t3 );
    sub_SW_QW_QW ( h0, t0, t1, t2, t3, t0, t1, t2, t3 );
    mul_QW_QW_QW ( t0, t1, t2, t3, r0, r1, r2, r3, r0, r1, r2, r3 );

    mul_DW_QW_QW ( ax0, ax1, r0, r1, r2, r3, r0, r1, r2, r3 );
    c0 = r0 * e2;
    c1 = r1 * e2;
    c2 = r2 * e2;
    c3 = r3 * e2;
  }

  // sqrt: 3-1
  template < typename T > __always_inline void constexpr
  sqrt_TW_SW ( T const a0, T const a1, T const a2, T &c0 )
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent ( as );
    T const ex = fp_const<T>::exponenti( as );
    T const b0 = ( a0 * ex ) * ex;
    T const b1 = ( a1 * ex ) * ex;
    T const b2 = ( a2 * ex ) * ex;

    T const x  = e / as;
    T const ax = b0 * x;
    T r0, r1;

    TwoProductFMA( ax, ax, r0, r1 );
    sub_TW_DW_SW( b0, b1, b2, r0, r1, c0 );

    c0 = ax + c0 * ( x * fp_const<T>::half() );
    c0 = c0 * e;
  }

  // sqrt: 3-2
  template < typename T > __always_inline void constexpr
  sqrt_TW_DW ( T const a0, T const a1, T const a2, T &c0, T &c1 )
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::half();
    T const ax0 = (a0 * ex) * ex2;
    T const ax1 = (a1 * ex) * ex2;
    T const ax2 = (a2 * ex) * ex2;
    T const h0 = 3*fp_const<T>::half();
    T t0, t1, t2;
    T r0, r1, r2;
    r0 = e / as;

    sqr_SW_DW ( r0, t0, t1 );
    mul_TW_DW_TW ( ax0, ax1, ax2, t0, t1, t0, t1, t2 );
    sub_SW_TW_TW ( h0, t0, t1, t2, t0, t1, t2 );
    mul_TW_SW_DW ( t0, t1, t2, r0, r0, r1 );

    mul_TW_DW_DW ( ax0, ax1, ax2, r0, r1, r0, r1 );
    c0 = r0 * e2;
    c1 = r1 * e2;
  }

  // sqrt: 3-3
  template < typename T > __always_inline void constexpr
  sqrt_TW_TW ( T const a0, T const a1, T const a2, T &c0, T &c1, T &c2 )
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::half();
    T const ax0 = (a0 * ex) * ex2;
    T const ax1 = (a1 * ex) * ex2;
    T const ax2 = (a2 * ex) * ex2;
    T const h0 = 3*fp_const<T>::half();
    T t0, t1, t2;
    T r0, r1, r2;
    r0 = e / as;

    sqr_SW_DW ( r0, t0, t1 );
    mul_TW_DW_TW ( ax0, ax1, ax2, t0, t1, t0, t1, t2 );
    sub_SW_TW_TW ( h0, t0, t1, t2, t0, t1, t2 );
    mul_TW_SW_TW ( t0, t1, t2, r0, r0, r1, r2 );

    sqr_TW_TW ( r0, r1, r2, t0, t1, t2 );
    mul_TW_TW_TW ( ax0, ax1, ax2, t0, t1, t2, t0, t1, t2 );
    sub_SW_TW_TW ( h0, t0, t1, t2, t0, t1, t2 );
    mul_TW_TW_TW ( t0, t1, t2, r0, r1, r2, r0, r1, r2 );

    mul_TW_TW_TW ( ax0, ax1, ax2, r0, r1, r2, r0, r1, r2 );
    c0 = r0 * e2;
    c1 = r1 * e2;
    c2 = r2 * e2;
  }

  // sqrt: 3-4
  template < typename T > __always_inline void constexpr
  sqrt_TW_QW ( T const a0, T const a1, T const a2, T &c0, T &c1, T &c2, T &c3 )
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::half();
    T const ax0 = (a0 * ex) * ex2;
    T const ax1 = (a1 * ex) * ex2;
    T const ax2 = (a2 * ex) * ex2;
    T const h0 = 3*fp_const<T>::half();
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    r0 = e / as;

    sqr_SW_DW ( r0, t0, t1 );
    mul_TW_DW_QW ( ax0, ax1, ax2, t0, t1, t0, t1, t2, t3 );
    sub_SW_QW_QW ( h0, t0, t1, t2, t3, t0, t1, t2, t3 );
    mul_QW_SW_QW ( t0, t1, t2, t3, r0, r0, r1, r2, r3 );

    sqr_QW_QW ( r0, r1, r2, r3, t0, t1, t2, t3 );
    mul_TW_QW_QW ( ax0, ax1, ax2, t0, t1, t2, t3, t0, t1, t2, t3 );
    sub_SW_QW_QW ( h0, t0, t1, t2, t3, t0, t1, t2, t3 );
    mul_QW_QW_QW ( t0, t1, t2, t3, r0, r1, r2, r3, r0, r1, r2, r3 );

    sqr_QW_QW ( r0, r1, r2, r3, t0, t1, t2, t3 );
    mul_TW_QW_QW ( ax0, ax1, ax2, t0, t1, t2, t3, t0, t1, t2, t3 );
    sub_SW_QW_QW ( h0, t0, t1, t2, t3, t0, t1, t2, t3 );
    mul_QW_QW_QW ( t0, t1, t2, t3, r0, r1, r2, r3, r0, r1, r2, r3 );

    mul_TW_QW_QW ( ax0, ax1, ax2, r0, r1, r2, r3, r0, r1, r2, r3 );
    c0 = r0 * e2;
    c1 = r1 * e2;
    c2 = r2 * e2;
    c3 = r3 * e2;
  }

  // sqrt: 4-1
  template < typename T > __always_inline void constexpr
  sqrt_QW_SW ( T const a0, T const a1, T const a2, T const a3, T &c0 )
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent ( as );
    T const ex = fp_const<T>::exponenti( as );
    T const b0 = ( a0 * ex ) * ex;
    T const b1 = ( a1 * ex ) * ex;
    T const b2 = ( a2 * ex ) * ex;
    T const b3 = ( a3 * ex ) * ex;

    T const x  = e / as;
    T const ax = b0 * x;
    T r0, r1;

    TwoProductFMA( ax, ax, r0, r1 );
    sub_DW_DW_SW( b0, b1, b2, b3, r0, r1, c0 );

    c0 = ax + c0 * ( x * fp_const<T>::half() );
    c0 = c0 * e;
  }

  // sqrt: 4-2
  template < typename T > __always_inline void constexpr
  sqrt_QW_DW ( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1 )
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::half();
    T const ax0 = (a0 * ex) * ex2;
    T const ax1 = (a1 * ex) * ex2;
    T const ax2 = (a2 * ex) * ex2;
    T const ax3 = (a3 * ex) * ex2;
    T const h0 = 3*fp_const<T>::half();
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    r0 = e / as;

    sqr_SW_DW ( r0, t0, t1 );
    mul_QW_DW_QW ( ax0, ax1, ax2, ax3, t0, t1, t0, t1, t2, t3 );
    sub_SW_QW_QW ( h0, t0, t1, t2, t3, t0, t1, t2, t3 );
    mul_QW_SW_DW ( t0, t1, t2, t3, r0, r0, r1 );

    mul_QW_DW_DW ( ax0, ax1, ax2, ax3, r0, r1, r0, r1 );
    c0 = r0 * e2;
    c1 = r1 * e2;
  }

  // sqrt: 4-3
  template < typename T > __always_inline void constexpr
  sqrt_QW_TW ( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1, T &c2 )
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::half();
    T const ax0 = (a0 * ex) * ex2;
    T const ax1 = (a1 * ex) * ex2;
    T const ax2 = (a2 * ex) * ex2;
    T const ax3 = (a3 * ex) * ex2;
    T const h0 = 3*fp_const<T>::half();
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    r0 = e / as;

    sqr_SW_DW ( r0, t0, t1 );
    mul_QW_DW_QW ( ax0, ax1, ax2, ax3, t0, t1, t0, t1, t2, t3 );
    sub_SW_QW_QW ( h0, t0, t1, t2, t3, t0, t1, t2, t3 );
    mul_QW_SW_TW ( t0, t1, t2, t3, r0, r0, r1, r2 );

    sqr_QW_QW ( r0, r1, r2, r3, t0, t1, t2, t3 );
    mul_QW_QW_QW ( ax0, ax1, ax2, ax3, t0, t1, t2, t3, t0, t1, t2, t3 );
    sub_SW_QW_QW ( h0, t0, t1, t2, t3, t0, t1, t2, t3 );
    mul_QW_QW_TW ( t0, t1, t2, t3, r0, r1, r2, r3, r0, r1, r2 );

    mul_QW_TW_TW ( ax0, ax1, ax2, ax3, r0, r1, r2, r0, r1, r2 );
    c0 = r0 * e2;
    c1 = r1 * e2;
    c2 = r2 * e2;
  }

  // sqrt: 4-4
  template < typename T > __always_inline void constexpr
  sqrt_QW_QW ( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1, T &c2, T &c3 )
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::half();
    T const ax0 = (a0 * ex) * ex2;
    T const ax1 = (a1 * ex) * ex2;
    T const ax2 = (a2 * ex) * ex2;
    T const ax3 = (a3 * ex) * ex2;
    T const h0 = 3*fp_const<T>::half();
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    r0 = e / as;

    sqr_SW_DW ( r0, t0, t1 );
    mul_QW_DW_QW ( ax0, ax1, ax2, ax3, t0, t1, t0, t1, t2, t3 );
    sub_SW_QW_QW ( h0, t0, t1, t2, t3, t0, t1, t2, t3 );
    mul_QW_SW_QW ( t0, t1, t2, t3, r0, r0, r1, r2, r3 );

    sqr_QW_QW ( r0, r1, r2, r3, t0, t1, t2, t3 );
    mul_QW_QW_QW ( ax0, ax1, ax2, ax3, t0, t1, t2, t3, t0, t1, t2, t3 );
    sub_SW_QW_QW ( h0, t0, t1, t2, t3, t0, t1, t2, t3 );
    mul_QW_QW_QW ( t0, t1, t2, t3, r0, r1, r2, r3, r0, r1, r2, r3 );

    sqr_QW_QW ( r0, r1, r2, r3, t0, t1, t2, t3 );
    mul_QW_QW_QW ( ax0, ax1, ax2, ax3, t0, t1, t2, t3, t0, t1, t2, t3 );
    sub_SW_QW_QW ( h0, t0, t1, t2, t3, t0, t1, t2, t3 );
    mul_QW_QW_QW ( t0, t1, t2, t3, r0, r1, r2, r3, r0, r1, r2, r3 );

    mul_QW_QW_QW ( ax0, ax1, ax2, ax3, r0, r1, r2, r3, r0, r1, r2, r3 );
    c0 = r0 * e2;
    c1 = r1 * e2;
    c2 = r2 * e2;
    c3 = r3 * e2;
  }

}


#endif
