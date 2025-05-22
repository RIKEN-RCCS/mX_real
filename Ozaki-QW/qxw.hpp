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


namespace QxW {
  template< typename T>
  static INLINE T fma_ ( T const a, T const b, T const c ) {   };
  static INLINE float fma_ ( float const a, float const b, float const c ) {
    return std::fma(a, b, c);
  }
  static INLINE double fma_ ( double const a, double const b, double const c ) {
    return std::fma(a, b, c);
  }
  static INLINE mpfrfp fma_ ( mpfrfp const a, mpfrfp const b, mpfrfp const c ) {
    mpfrfp d;
    //#ifndef FLOATHARDWARE
    // #define FLOATHARDWARE
    //#endif
#ifdef  FLOATHARDWARE
    //    fprintf(stderr, "%s %d\n", __FILE__, __LINE__);      
    float aa, bb, cc, dd;
    aa = mpfr_get_flt(a._x, MPFR_RNDN);
    bb = mpfr_get_flt(b._x, MPFR_RNDN);
    cc = mpfr_get_flt(c._x, MPFR_RNDN);
    dd = fmaf(aa, bb, cc);
    mpfr_set_flt(d._x, dd, MPFR_RNDN);
#else
    mpfr_fma(d._x, a._x, b._x, c._x, MPFR_RNDN);
#endif
    return d;
  }

  // ------------------------
  // Basic EFT Part
  // ------------------------

  template < typename T > INLINE void constexpr
  TwoSum ( T const a, T const b, T &x, T &y ) NOEXCEPT
  {
#if defined(__llvm__) || defined(__clang__)
#if __clang_major__ >= 11
#pragma clang fp reassociate(off)
#endif
#if __clang_major__ <= 10
#pragma clang optimize off
#endif
#endif

    //
    // basic cost is 6 flops and 6 cycles per a signle call
    //
#if defined(__CUDA_ARCH__)
    if ( std::is_same<T,double>::value ) {
      x = __dadd_rn(a, b);
    } else if ( std::is_same<T,float>::value ) {
      x = __fadd_rn(a, b);
    } else {
      x = a + b;
    }
#else
    x = a + b;
#endif
    auto z = x - a;
    y = (a - (x - z)) + (b - z);

#if defined(__llvm__) || defined(__clang__)
#if __clang_major__ <= 10
#pragma clang optimize on
#endif
#endif
  }

  template < typename T > INLINE void constexpr
  FastTwoSum ( T const a, T const b, T &x, T &y ) NOEXCEPT
  {
#if defined(__llvm__) || defined(__clang__)
#if __clang_major__ >= 11
#pragma clang fp reassociate(off)
#endif
#if __clang_major__ <= 10
#pragma clang optimize off
#endif
#endif

    //
    // basic cost is 3 flops and 3 cycles per a signle call
    //
#if defined(__CUDA_ARCH__)
    if ( std::is_same<T,double>::value ) {
      x = __dadd_rn(a, b);
    } else if ( std::is_same<T,float>::value ) {
      x = __fadd_rn(a, b);
    } else {
      x = a + b;
    }
#else
    x = a + b;
#endif
    y = (a - x) + b;

#if defined(__llvm__) || defined(__clang__)
#if __clang_major__ <= 10
#pragma clang optimize on
#endif
#endif
  }

  template < typename T > INLINE void constexpr
  TwoProductFMA ( T const a, T const b, T &x, T &y ) NOEXCEPT
  {

    //
    // basic cost is 3 flops but 2 cycles per a signle call
    // (fma with negative arg is expected to be replaced by a sigle instruction
    // at the compilation
    //
    x = a * b;
    y = std::fma(a, b, -x);

  }

  // ------------------------
  // Other DD-Mult Funcs.
  // ------------------------

  template < typename T > INLINE void constexpr
  Nagai_Mul ( T const a0, T const a1, T const b0, T const b1, T &x, T &y ) NOEXCEPT
  {
    auto z = a0 * b1;                 // 1 1
    auto t = std::fma( a1, b0, z );   // 1 2
    x = std::fma( a0, b0, t );        // 1 2
    y = std::fma( a0, b0, -x ) + t;   // 2 3  // total 5 cycles 8 flops
  }

  template < typename T > INLINE void constexpr
  Ya_mul_DW_DW_DW ( T const a0, T const a1, T const b0, T const b1, T &x, T &y ) NOEXCEPT
  {
    //
    // It perform slightly faster but inaccurate than the accurate version.
    // Also, it would be slower but slightly accurate than the Sloppy version.
    //
    T q0, q1, q2, q3;
    T w0, w1, w2;
    T z0, z1, z2;
    T y0, y1, y2;

    //         [q3] = a1 * b1 
    //     [q2, y2] = a0 * b1 + q3
    //     [q1, y1] = a1 * b0 + q2
    // [q0, y0]     = a0 * b0 + q1
    // x = q0, y = y2 + y1 + y0

    q3 = a1 * b1;                     // 1 1

    q2 = std::fma( a0, b1, q3 );      // 1 2
    FastTwoSum( -q2, q3, w2, z2 );    // 3 3
    y2 = std::fma( a0, b1, w2 ) + z2; // 2 3

    q1 = std::fma( a1, b0, q2 );      // 1 2
    TwoSum    ( -q1, q2, w1, z1 );    // 6 6
    y1 = std::fma( a1, b0, w1 ) + z1; // 2 3

    q0 = std::fma( a0, b0, q1 );      // 1 2
    FastTwoSum( -q0, q1, w0, z0 );    // 3 3
    y0 = std::fma( a0, b0, w0 ) + z0; // 2 3

    x = q0;
    y = y2 + y1 + y0;                 // 2 2 // total 24 cycles 30 flops
  }

  template < typename T > INLINE void constexpr
  Ya_sqr_DW_DW ( T const a0, T const a1, T &x, T &y ) NOEXCEPT
  {
    //
    // It perform slightly faster but inaccurate than the accurate version.
    // Also, it would be slower but slightly accurate than the sloppy version.
    //
    T q0, q1, q2, q3;
    T w1, w0;
    T z1, z0;
    T y0, y1, y2;

    q3 = a1 + a1;                     // 1 1
    q2 = a1 * a1;                     // 1 1

    q1 = std::fma( a0, q3, q2 );      // 1 2
    FastTwoSum( -q1, q2, w1, z1 );    // 3 3
    y1 = std::fma( a0, q3, w1 ) + z1; // 2 3

    q0 = std::fma( a0, a0, q1 );      // 1 2
    FastTwoSum( -q0, q1, w0, z0 );    // 3 3
    y0 = std::fma( a0, a0, w0 ) + z0; // 2 3

    x = q0;
    y = y1 + y0;                      // 1 1 // total 15 cycles 19 flops
  }



  // ------------------------
  // PA/Quasi Arithmetic Part
  // ------------------------

  // add: 1-1-1
  template < typename T > INLINE void constexpr
  add_SW_SW_SW( T const a0, T const b0, T &c0 ) NOEXCEPT
  {
    c0 = a0 + b0;
  }

  // add: 1-1-2
  template < typename T > INLINE void constexpr
  add_SW_SW_PA( T const a0, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
  }

  // add: 1-1-3
  template < typename T > INLINE void constexpr
  add_SW_SW_QTW( T const a0, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    c2 = fp_const<T>::zero();
  }

  // add: 1-1-4
  template < typename T > INLINE void constexpr
  add_SW_SW_QQW( T const a0, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    c2 = fp_const<T>::zero();
    c3 = fp_const<T>::zero();
  }

  // add: 1-2-1
  template < typename T > INLINE void constexpr
  add_SW_PA_SW( T const a0, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    c0 = a0 + b0;
    c0 = c0 + b1;
  }

  // add: 1-2-2
  template < typename T > INLINE void constexpr
  add_SW_PA_PA( T const a0, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    c1 = c1 + b1;
  }

  // add: 1-2-3
  template < typename T > INLINE void constexpr
  add_SW_PA_QTW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, c2 );
  }

  // add: 1-2-4
  template < typename T > INLINE void constexpr
  add_SW_PA_QQW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, c2 );
    c3 = fp_const<T>::zero();
  }

  // add: 1-3-1
  template < typename T > INLINE void constexpr
  add_SW_QTW_SW( T const a0, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T t0;
    c0 = a0 + b0;
    t0 = b1 + b2;
    c0 = c0 + t0;
  }

  // add: 1-3-2
  template < typename T > INLINE void constexpr
  add_SW_QTW_PA( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    t0 = b1 + b2;
    c1 = c1 + t0;
  }

  // add: 1-3-3
  template < typename T > INLINE void constexpr
  add_SW_QTW_QTW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, t0 );
    c2 = t0 + b2;
  }

  // add: 1-3-4
  template < typename T > INLINE void constexpr
  add_SW_QTW_QQW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, c2 );
    TwoSum( c2, b2, c2, c3 );
  }

  // add: 1-4-1
  template < typename T > INLINE void constexpr
  add_SW_QQW_SW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T t0;
    c0 = a0 + b0;
    t0 = b1 + b2 + b3;
    c0 = c0 + t0;
  }

  // add: 1-4-2
  template < typename T > INLINE void constexpr
  add_SW_QQW_PA( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    t0 = b1 + b2 + b3;
    c1 = c1 + t0;
  }

  // add: 1-4-3
  template < typename T > INLINE void constexpr
  add_SW_QQW_QTW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, t0 );
    c2 = t0 + b2 + b3;
  }

  // add: 1-4-4
  template < typename T > INLINE void constexpr
  add_SW_QQW_QQW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, c2 );
    TwoSum( c2, b2, c2, t0 );
    c3 = t0 + b3;
  }

  // add: 2-1-1
  template < typename T > INLINE void constexpr
  add_PA_SW_SW( T const a0, T const a1, T const b0, T &c0 ) NOEXCEPT
  {
    c0 = a0 + b0;
    c0 = c0 + a1;
  }

  // add: 2-1-2
  template < typename T > INLINE void constexpr
  add_PA_SW_PA( T const a0, T const a1, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    c1 = c1 + a1;
  }

  // add: 2-1-3
  template < typename T > INLINE void constexpr
  add_PA_SW_QTW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, c2 );
  }

  // add: 2-1-4
  template < typename T > INLINE void constexpr
  add_PA_SW_QQW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, c2 );
    c3 = fp_const<T>::zero();
  }

  // add: 2-2-1
  template < typename T > INLINE void constexpr
  add_PA_PA_SW( T const a0, T const a1, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T t0;
    c0 = a0 + b0;
    t0 = a1 + b1;
    c0 = c0 + t0;
  }

  // add: 2-2-2
  template < typename T > INLINE void constexpr
  add_PA_PA_PA( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + b1;
    c1 = c1 + t0;
  }

  // add: 2-2-3
  template < typename T > INLINE void constexpr
  add_PA_PA_QTW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    c2 = c2 + t0;
  }

  // add: 2-2-4
  template < typename T > INLINE void constexpr
  add_PA_PA_QQW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, c3, c2 );
    TwoSum( c1, c3, c1, c3 );
    TwoSum( c2, c3, c2, c3 );
  }

  // add: 2-3-1
  template < typename T > INLINE void constexpr
  add_PA_QTW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T t0;
    c0 = a0 + b0;
    t0 = a1 + b1;
    t0 = t0 + b2;
    c0 = c0 + t0;
  }

  // add: 2-3-2
  template < typename T > INLINE void constexpr
  add_PA_QTW_PA( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + b1;
    t0 = t0 + b2;
    c1 = c1 + t0;
  }

  // add: 2-3-3
  template < typename T > INLINE void constexpr
  add_PA_QTW_QTW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    c2 = c2 + t0 + b2;
  }

  // add: 2-3-4
  template < typename T > INLINE void constexpr
  add_PA_QTW_QQW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_PA_QQW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T t0;
    c0 = a0 + b0;
    t0 = a1 + b1;
    t0 = t0 + b2 + b3;
    c0 = c0 + t0;
  }

  // add: 2-4-2
  template < typename T > INLINE void constexpr
  add_PA_QQW_PA( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + b1;
    t0 = t0 + b2 + b3;
    c1 = c1 + t0;
  }

  // add: 2-4-3
  template < typename T > INLINE void constexpr
  add_PA_QQW_QTW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    c2 = c2 + t0 + b2 + b3;
  }

  // add: 2-4-4
  template < typename T > INLINE void constexpr
  add_PA_QQW_QQW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_QTW_SW_SW( T const a0, T const a1, T const a2, T const b0, T &c0 ) NOEXCEPT
  {
    T t0;
    c0 = a0 + b0;
    t0 = a1 + a2;
    c0 = c0 + t0;
  }

  // add: 3-1-2
  template < typename T > INLINE void constexpr
  add_QTW_SW_PA( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + a2;
    c1 = c1 + t0;
  }

  // add: 3-1-3
  template < typename T > INLINE void constexpr
  add_QTW_SW_QTW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, t0 );
    c2 = t0 + a2;
  }

  // add: 3-1-4
  template < typename T > INLINE void constexpr
  add_QTW_SW_QQW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, c2 );
    TwoSum( c2, a2, c2, c3 );
  }

  // add: 3-2-1
  template < typename T > INLINE void constexpr
  add_QTW_PA_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T t0;
    c0 = a0 + b0;
    t0 = a1 + b1;
    t0 = t0 + a2;
    c0 = c0 + t0;
  }

  // add: 3-2-2
  template < typename T > INLINE void constexpr
  add_QTW_PA_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + b1;
    t0 = t0 + a2;
    c1 = c1 + t0;
  }

  // add: 3-2-3
  template < typename T > INLINE void constexpr
  add_QTW_PA_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    c2 = c2 + t0 + a2;
  }

  // add: 3-2-4
  template < typename T > INLINE void constexpr
  add_QTW_PA_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_QTW_QTW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    c0 = a0 + b0;
    t0 = a1 + b1;
    t1 = a2 + b2;
    t0 = t0 + t1;
    c0 = c0 + t0;
  }

  // add: 3-3-2
  template < typename T > INLINE void constexpr
  add_QTW_QTW_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + b1;
    t1 = a2 + b2;
    t0 = t0 + t1;
    c1 = c1 + t0;
  }

  // add: 3-3-3
  template < typename T > INLINE void constexpr
  add_QTW_QTW_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    t1 = a2 + b2;
    c2 = c2 + t0 + t1;
  }

  // add: 3-3-4
  template < typename T > INLINE void constexpr
  add_QTW_QTW_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_QTW_QQW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    c0 = a0 + b0;
    t0 = a1 + b1;
    t1 = a2 + b2;
    t0 = t0 + t1 + b3;
    c0 = c0 + t0;
  }

  // add: 3-4-2
  template < typename T > INLINE void constexpr
  add_QTW_QQW_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + b1;
    t1 = a2 + b2;
    t0 = t0 + t1 + b3;
    c1 = c1 + t0;
  }

  // add: 3-4-3
  template < typename T > INLINE void constexpr
  add_QTW_QQW_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    t1 = a2 + b2;
    c2 = c2 + t0 + t1 + b3;
  }

  // add: 3-4-4
  template < typename T > INLINE void constexpr
  add_QTW_QQW_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_QQW_SW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0 ) NOEXCEPT
  {
    T t0;
    c0 = a0 + b0;
    t0 = a1 + a2 + a3;
    c0 = c0 + t0;
  }

  // add: 4-1-2
  template < typename T > INLINE void constexpr
  add_QQW_SW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + a2 + a3;
    c1 = c1 + t0;
  }

  // add: 4-1-3
  template < typename T > INLINE void constexpr
  add_QQW_SW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, t0 );
    c2 = t0 + a2 + a3;
  }

  // add: 4-1-4
  template < typename T > INLINE void constexpr
  add_QQW_SW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, c2 );
    TwoSum( c2, a2, c2, t0 );
    c3 = t0 + a3;
  }

  // add: 4-2-1
  template < typename T > INLINE void constexpr
  add_QQW_PA_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T t0;
    c0 = a0 + b0;
    t0 = a1 + b1;
    t0 = t0 + a2 + a3;
    c0 = c0 + t0;
  }

  // add: 4-2-2
  template < typename T > INLINE void constexpr
  add_QQW_PA_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + b1;
    t0 = t0 + a2 + a3;
    c1 = c1 + t0;
  }

  // add: 4-2-3
  template < typename T > INLINE void constexpr
  add_QQW_PA_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    c2 = c2 + t0 + a2 + a3;
  }

  // add: 4-2-4
  template < typename T > INLINE void constexpr
  add_QQW_PA_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_QQW_QTW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    c0 = a0 + b0;
    t0 = a1 + b1;
    t1 = a2 + b2;
    t0 = t0 + t1 + a3;
    c0 = c0 + t0;
  }

  // add: 4-3-2
  template < typename T > INLINE void constexpr
  add_QQW_QTW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    t0 = a1 + b1;
    t1 = a2 + b2;
    t0 = t0 + t1 + a3;
    c1 = c1 + t0;
  }

  // add: 4-3-3
  template < typename T > INLINE void constexpr
  add_QQW_QTW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    t1 = a2 + b2;
    c2 = c2 + t0 + t1 + a3;
  }

  // add: 4-3-4
  template < typename T > INLINE void constexpr
  add_QQW_QTW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_QQW_QQW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_QQW_QQW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_QQW_QQW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_QQW_QQW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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

  // add: 5-5-5
  template < typename T > INLINE void constexpr
  add_QPW_QPW_QPW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4) NOEXCEPT
  {
    T t0, t1, t2, t3;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( a2, b2, t1, c3 );
    TwoSum( a3, b3, t2, c4 );    
    
    TwoSum( c1, t0, c1, t0 );

    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );    
    
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t1, c3, t1 );
    TwoSum( c3, t2, c3, t2 );
    t3 = a4 + b4;
    c4 = c4 + t0 + t1 + t2 + t3;
  }


    // add: 5-2-5
  template < typename T > INLINE void constexpr
  add_QPW_PA_QPW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4) NOEXCEPT
  {
    T e0, e1;
    TwoSum( a0, b0, c0, e0 );
    
    TwoSum( a1, b1, c1, e1 );
    TwoSum( c1, e0, c1, e0 );
    
    TwoSum( a2, e0, c2, e0 ); 
    TwoSum( c2, e1, c2, e1 );

    TwoSum( a3, e0, c3, e0 );
    TwoSum( c3, e1, c3, e1 ); 

    c4 = a4 + e0 + e1;
  }

    // add: 2-5-5
  template < typename T > INLINE void constexpr
  add_PA_QPW_QPW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4) NOEXCEPT
  {
    T e0, e1, e2, e3, e4, e5, e6, e7, e8, e9;
    TwoSum( a0, b0, c0, e0 );
    
    TwoSum( a1, b1, c1, e1 );
    TwoSum( c1, e0, c1, e0 );
    
    TwoSum( b2, e0, c2, e0 ); 
    TwoSum( c2, e1, c2, e1 );

    TwoSum( b3, e0, c3, e0 );
    TwoSum( c3, e1, c3, e1 ); 

    c4 = b4 + e0 + e1;
  }

    // add: 5-3-5
  template < typename T > INLINE void constexpr
  add_QPW_QTW_QPW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3, T &c4) NOEXCEPT
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
    c4 = a3 + a4 + t0 + t1;
  }

    // add: 3-5-5
  template < typename T > INLINE void constexpr
  add_QTW_QPW_QPW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4) NOEXCEPT
  {
    add_QPW_QTW_QPW(a0, a1, a2, b0, b1, b2, b3, b4, c1, c2, c2, c3, c4);
  }

    // add: 5-4-5
  template < typename T > INLINE void constexpr
  add_QPW_QQW_QPW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3, T &c4) NOEXCEPT
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( a2, b2, t1, c3 );
    TwoSum( a3, b3, t2, c4 );    
    
    TwoSum( c1, t0, c1, t0 );

    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );    
    
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t1, c3, t1 );
    TwoSum( c3, t2, c3, t2 );
    c4 = c4 + a4 + t0 + t1 + t2;
  }

    // add: 4-5-5
  template < typename T > INLINE void constexpr
  add_QQW_QPW_QPW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4) NOEXCEPT
  {
    add_QPW_QQW_QPW(b0, b1, b2, b3, b4, a0, a1, a2, a3, c1, c2, c2, c3, c4);
  }
  
  // sub: 1-1-1
  template < typename T > INLINE void constexpr
  sub_SW_SW_SW( T const a0, T const b0, T &c0 ) NOEXCEPT
  {
    add_SW_SW_SW( a0, -b0, c0 );
  }

  // sub: 1-1-2
  template < typename T > INLINE void constexpr
  sub_SW_SW_PA( T const a0, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    add_SW_SW_PA( a0, -b0, c0, c1 );
  }

  // sub: 1-1-3
  template < typename T > INLINE void constexpr
  sub_SW_SW_QTW( T const a0, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_SW_SW_QTW( a0, -b0, c0, c1, c2 );
  }

  // sub: 1-1-4
  template < typename T > INLINE void constexpr
  sub_SW_SW_QQW( T const a0, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_SW_SW_QQW( a0, -b0, c0, c1, c2, c3 );
  }

  // sub: 1-2-1
  template < typename T > INLINE void constexpr
  sub_SW_PA_SW( T const a0, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    add_SW_PA_SW( a0, -b0, -b1, c0 );
  }

  // sub: 1-2-2
  template < typename T > INLINE void constexpr
  sub_SW_PA_PA( T const a0, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    add_SW_PA_PA( a0, -b0, -b1, c0, c1 );
  }

  // sub: 1-2-3
  template < typename T > INLINE void constexpr
  sub_SW_PA_QTW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_SW_PA_QTW( a0, -b0, -b1, c0, c1, c2 );
  }

  // sub: 1-2-4
  template < typename T > INLINE void constexpr
  sub_SW_PA_QQW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_SW_PA_QQW( a0, -b0, -b1, c0, c1, c2, c3 );
  }

  // sub: 1-3-1
  template < typename T > INLINE void constexpr
  sub_SW_QTW_SW( T const a0, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    add_SW_QTW_SW( a0, -b0, -b1, -b2, c0 );
  }

  // sub: 1-3-2
  template < typename T > INLINE void constexpr
  sub_SW_QTW_PA( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    add_SW_QTW_PA( a0, -b0, -b1, -b2, c0, c1 );
  }

  // sub: 1-3-3
  template < typename T > INLINE void constexpr
  sub_SW_QTW_QTW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_SW_QTW_QTW( a0, -b0, -b1, -b2, c0, c1, c2 );
  }

  // sub: 1-3-4
  template < typename T > INLINE void constexpr
  sub_SW_QTW_QQW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_SW_QTW_QQW( a0, -b0, -b1, -b2, c0, c1, c2, c3 );
  }

  // sub: 1-4-1
  template < typename T > INLINE void constexpr
  sub_SW_QQW_SW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    add_SW_QQW_SW( a0, -b0, -b1, -b2, -b3, c0 );
  }

  // sub: 1-4-2
  template < typename T > INLINE void constexpr
  sub_SW_QQW_PA( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    add_SW_QQW_PA( a0, -b0, -b1, -b2, -b3, c0, c1 );
  }

  // sub: 1-4-3
  template < typename T > INLINE void constexpr
  sub_SW_QQW_QTW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_SW_QQW_QTW( a0, -b0, -b1, -b2, -b3, c0, c1, c2 );
  }

  // sub: 1-4-4
  template < typename T > INLINE void constexpr
  sub_SW_QQW_QQW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_SW_QQW_QQW( a0, -b0, -b1, -b2, -b3, c0, c1, c2, c3 );
  }

  // sub: 2-1-1
  template < typename T > INLINE void constexpr
  sub_PA_SW_SW( T const a0, T const a1, T const b0, T &c0 ) NOEXCEPT
  {
    add_PA_SW_SW( a0, a1, -b0, c0 );
  }

  // sub: 2-1-2
  template < typename T > INLINE void constexpr
  sub_PA_SW_PA( T const a0, T const a1, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    add_PA_SW_PA( a0, a1, -b0, c0, c1 );
  }

  // sub: 2-1-3
  template < typename T > INLINE void constexpr
  sub_PA_SW_QTW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_PA_SW_QTW( a0, a1, -b0, c0, c1, c2 );
  }

  // sub: 2-1-4
  template < typename T > INLINE void constexpr
  sub_PA_SW_QQW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_PA_SW_QQW( a0, a1, -b0, c0, c1, c2, c3 );
  }

  // sub: 2-2-1
  template < typename T > INLINE void constexpr
  sub_PA_PA_SW( T const a0, T const a1, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    add_PA_PA_SW( a0, a1, -b0, -b1, c0 );
  }

  // sub: 2-2-2
  template < typename T > INLINE void constexpr
  sub_PA_PA_PA( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    add_PA_PA_PA( a0, a1, -b0, -b1, c0, c1 );
  }

  // sub: 2-2-3
  template < typename T > INLINE void constexpr
  sub_PA_PA_QTW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_PA_PA_QTW( a0, a1, -b0, -b1, c0, c1, c2 );
  }

  // sub: 2-2-4
  template < typename T > INLINE void constexpr
  sub_PA_PA_QQW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_PA_PA_QQW( a0, a1, -b0, -b1, c0, c1, c2, c3 );
  }

  // sub: 2-3-1
  template < typename T > INLINE void constexpr
  sub_PA_QTW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    add_PA_QTW_SW( a0, a1, -b0, -b1, -b2, c0 );
  }

  // sub: 2-3-2
  template < typename T > INLINE void constexpr
  sub_PA_QTW_PA( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    add_PA_QTW_PA( a0, a1, -b0, -b1, -b2, c0, c1 );
  }

  // sub: 2-3-3
  template < typename T > INLINE void constexpr
  sub_PA_QTW_QTW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_PA_QTW_QTW( a0, a1, -b0, -b1, -b2, c0, c1, c2 );
  }

  // sub: 2-3-4
  template < typename T > INLINE void constexpr
  sub_PA_QTW_QQW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_PA_QTW_QQW( a0, a1, -b0, -b1, -b2, c0, c1, c2, c3 );
  }

  // sub: 2-4-1
  template < typename T > INLINE void constexpr
  sub_PA_QQW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    add_PA_QQW_SW( a0, a1, -b0, -b1, -b2, -b3, c0 );
  }

  // sub: 2-4-2
  template < typename T > INLINE void constexpr
  sub_PA_QQW_PA( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    add_PA_QQW_PA( a0, a1, -b0, -b1, -b2, -b3, c0, c1 );
  }

  // sub: 2-4-3
  template < typename T > INLINE void constexpr
  sub_PA_QQW_QTW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_PA_QQW_QTW( a0, a1, -b0, -b1, -b2, -b3, c0, c1, c2 );
  }

  // sub: 2-4-4
  template < typename T > INLINE void constexpr
  sub_PA_QQW_QQW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_PA_QQW_QQW( a0, a1, -b0, -b1, -b2, -b3, c0, c1, c2, c3 );
  }

  // sub: 3-1-1
  template < typename T > INLINE void constexpr
  sub_QTW_SW_SW( T const a0, T const a1, T const a2, T const b0, T &c0 ) NOEXCEPT
  {
    add_QTW_SW_SW( a0, a1, a2, -b0, c0 );
  }

  // sub: 3-1-2
  template < typename T > INLINE void constexpr
  sub_QTW_SW_PA( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    add_QTW_SW_PA( a0, a1, a2, -b0, c0, c1 );
  }

  // sub: 3-1-3
  template < typename T > INLINE void constexpr
  sub_QTW_SW_QTW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_QTW_SW_QTW( a0, a1, a2, -b0, c0, c1, c2 );
  }

  // sub: 3-1-4
  template < typename T > INLINE void constexpr
  sub_QTW_SW_QQW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_QTW_SW_QQW( a0, a1, a2, -b0, c0, c1, c2, c3 );
  }

  // sub: 3-2-1
  template < typename T > INLINE void constexpr
  sub_QTW_PA_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    add_QTW_PA_SW( a0, a1, a2, -b0, -b1, c0 );
  }

  // sub: 3-2-2
  template < typename T > INLINE void constexpr
  sub_QTW_PA_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    add_QTW_PA_PA( a0, a1, a2, -b0, -b1, c0, c1 );
  }

  // sub: 3-2-3
  template < typename T > INLINE void constexpr
  sub_QTW_PA_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_QTW_PA_QTW( a0, a1, a2, -b0, -b1, c0, c1, c2 );
  }

  // sub: 3-2-4
  template < typename T > INLINE void constexpr
  sub_QTW_PA_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_QTW_PA_QQW( a0, a1, a2, -b0, -b1, c0, c1, c2, c3 );
  }

  // sub: 3-3-1
  template < typename T > INLINE void constexpr
  sub_QTW_QTW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    add_QTW_QTW_SW( a0, a1, a2, -b0, -b1, -b2, c0 );
  }

  // sub: 3-3-2
  template < typename T > INLINE void constexpr
  sub_QTW_QTW_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    add_QTW_QTW_PA( a0, a1, a2, -b0, -b1, -b2, c0, c1 );
  }

  // sub: 3-3-3
  template < typename T > INLINE void constexpr
  sub_QTW_QTW_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_QTW_QTW_QTW( a0, a1, a2, -b0, -b1, -b2, c0, c1, c2 );
  }

  // sub: 3-3-4
  template < typename T > INLINE void constexpr
  sub_QTW_QTW_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_QTW_QTW_QQW( a0, a1, a2, -b0, -b1, -b2, c0, c1, c2, c3 );
  }

  // sub: 3-4-1
  template < typename T > INLINE void constexpr
  sub_QTW_QQW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    add_QTW_QQW_SW( a0, a1, a2, -b0, -b1, -b2, -b3, c0 );
  }

  // sub: 3-4-2
  template < typename T > INLINE void constexpr
  sub_QTW_QQW_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    add_QTW_QQW_PA( a0, a1, a2, -b0, -b1, -b2, -b3, c0, c1 );
  }

  // sub: 3-4-3
  template < typename T > INLINE void constexpr
  sub_QTW_QQW_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_QTW_QQW_QTW( a0, a1, a2, -b0, -b1, -b2, -b3, c0, c1, c2 );
  }

  // sub: 3-4-4
  template < typename T > INLINE void constexpr
  sub_QTW_QQW_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_QTW_QQW_QQW( a0, a1, a2, -b0, -b1, -b2, -b3, c0, c1, c2, c3 );
  }

  // sub: 4-1-1
  template < typename T > INLINE void constexpr
  sub_QQW_SW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0 ) NOEXCEPT
  {
    add_QQW_SW_SW( a0, a1, a2, a3, -b0, c0 );
  }

  // sub: 4-1-2
  template < typename T > INLINE void constexpr
  sub_QQW_SW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    add_QQW_SW_PA( a0, a1, a2, a3, -b0, c0, c1 );
  }

  // sub: 4-1-3
  template < typename T > INLINE void constexpr
  sub_QQW_SW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_QQW_SW_QTW( a0, a1, a2, a3, -b0, c0, c1, c2 );
  }

  // sub: 4-1-4
  template < typename T > INLINE void constexpr
  sub_QQW_SW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_QQW_SW_QQW( a0, a1, a2, a3, -b0, c0, c1, c2, c3 );
  }

  // sub: 4-2-1
  template < typename T > INLINE void constexpr
  sub_QQW_PA_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    add_QQW_PA_SW( a0, a1, a2, a3, -b0, -b1, c0 );
  }

  // sub: 4-2-2
  template < typename T > INLINE void constexpr
  sub_QQW_PA_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    add_QQW_PA_PA( a0, a1, a2, a3, -b0, -b1, c0, c1 );
  }

  // sub: 4-2-3
  template < typename T > INLINE void constexpr
  sub_QQW_PA_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_QQW_PA_QTW( a0, a1, a2, a3, -b0, -b1, c0, c1, c2 );
  }

  // sub: 4-2-4
  template < typename T > INLINE void constexpr
  sub_QQW_PA_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_QQW_PA_QQW( a0, a1, a2, a3, -b0, -b1, c0, c1, c2, c3 );
  }

  // sub: 4-3-1
  template < typename T > INLINE void constexpr
  sub_QQW_QTW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    add_QQW_QTW_SW( a0, a1, a2, a3, -b0, -b1, -b2, c0 );
  }

  // sub: 4-3-2
  template < typename T > INLINE void constexpr
  sub_QQW_QTW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    add_QQW_QTW_PA( a0, a1, a2, a3, -b0, -b1, -b2, c0, c1 );
  }

  // sub: 4-3-3
  template < typename T > INLINE void constexpr
  sub_QQW_QTW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_QQW_QTW_QTW( a0, a1, a2, a3, -b0, -b1, -b2, c0, c1, c2 );
  }

  // sub: 4-3-4
  template < typename T > INLINE void constexpr
  sub_QQW_QTW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_QQW_QTW_QQW( a0, a1, a2, a3, -b0, -b1, -b2, c0, c1, c2, c3 );
  }

  // sub: 4-4-1
  template < typename T > INLINE void constexpr
  sub_QQW_QQW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    add_QQW_QQW_SW( a0, a1, a2, a3, -b0, -b1, -b2, -b3, c0 );
  }

  // sub: 4-4-2
  template < typename T > INLINE void constexpr
  sub_QQW_QQW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    add_QQW_QQW_PA( a0, a1, a2, a3, -b0, -b1, -b2, -b3, c0, c1 );
  }

  // sub: 4-4-3
  template < typename T > INLINE void constexpr
  sub_QQW_QQW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_QQW_QQW_QTW( a0, a1, a2, a3, -b0, -b1, -b2, -b3, c0, c1, c2 );
  }

  // sub: 4-4-4
  template < typename T > INLINE void constexpr
  sub_QQW_QQW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_QQW_QQW_QQW( a0, a1, a2, a3, -b0, -b1, -b2, -b3, c0, c1, c2, c3 );
  }

  // sub: 5-5-5
  template < typename T > INLINE void constexpr
  sub_QPW_QPW_QPW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4) NOEXCEPT
  {
    add_QPW_QPW_QPW( a0, a1, a2, a3, a4, -b0, -b1, -b2, -b3, -b4,
		     c0, c1, c2, c3, c4 );
  }

  // sub: 5-2-5
  template < typename T > INLINE void constexpr
  sub_QPW_PA_QPW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3, T &c4) NOEXCEPT
  {
    //    add_QPW_PA_QPW( a0, a1, a2, a3, a4, -b0, -b1, c0, c1, c2, c3, c4);
    add_PA_QPW_QPW( -b0, -b1, a0, a1, a2, a3, a4, c0, c1, c2, c3, c4);    
  }
  // sub: 2-5-5
  template < typename T > INLINE void constexpr
  sub_PA_QPW_QPW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4) NOEXCEPT
  {
    add_PA_QPW_QPW( a0, a1, -b0, -b1, -b2, -b3, -b4, c0, c1, c2, c3, c4);
  }

  // mul: 1-1-1
  template < typename T > INLINE void constexpr
  mul_SW_SW_SW( T const a0, T const b0, T &c0 ) NOEXCEPT
  {
    c0 = a0 * b0;
  }

  // mul: 1-1-2
  template < typename T > INLINE void constexpr
  mul_SW_SW_PA( T const a0, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    TwoProductFMA( a0, b0, c0, c1 );
  }

  // mul: 1-1-3
  template < typename T > INLINE void constexpr
  mul_SW_SW_QTW( T const a0, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    TwoProductFMA( a0, b0, c0, c1 );
    c2 = fp_const<T>::zero();
  }

  // mul: 1-1-4
  template < typename T > INLINE void constexpr
  mul_SW_SW_QQW( T const a0, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    TwoProductFMA( a0, b0, c0, c1 );
    c2 = fp_const<T>::zero();
    c3 = fp_const<T>::zero();
  }

  // mul: 1-2-1
  template < typename T > INLINE void constexpr
  mul_SW_PA_SW( T const a0, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T t0;
    t0 = b0 + b1;
    c0 = a0 * t0;
  }

  // mul: 1-2-2
  template < typename T > INLINE void constexpr
  mul_SW_PA_PA( T const a0, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    TwoProductFMA( a0, b0, c0, c1 );
    c1 = std::fma ( a0, b1, c1 );
  }

  // mul: 1-2-3
  template < typename T > INLINE void constexpr
  mul_SW_PA_QTW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    c2 = t0 + c2;
  }

  // mul: 1-2-4
  template < typename T > INLINE void constexpr
  mul_SW_PA_QQW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    c2 = t0 + c2;
    c3 = fp_const<T>::zero();
  }

  // mul: 1-3-1
  template < typename T > INLINE void constexpr
  mul_SW_QTW_SW( T const a0, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T t0;
    t0 = b0 + b1 + b2;
    c0 = a0 * t0;
  }

  // mul: 1-3-2
  template < typename T > INLINE void constexpr
  mul_SW_QTW_PA( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = b1 + b2;
    c1 = std::fma ( a0, t0, c1 );
  }

  // mul: 1-3-3
  template < typename T > INLINE void constexpr
  mul_SW_QTW_QTW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    t0 = std::fma ( a0, b2, t0 );
    c2 = t0 + c2;
  }

  // mul: 1-3-4
  template < typename T > INLINE void constexpr
  mul_SW_QTW_QQW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_SW_QQW_SW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T t0;
    t0 = b0 + b1 + b2 + b3;
    c0 = a0 * t0;
  }

  // mul: 1-4-2
  template < typename T > INLINE void constexpr
  mul_SW_QQW_PA( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = b1 + b2 + b3;
    c1 = std::fma ( a0, t0, c1 );
  }

  // mul: 1-4-3
  template < typename T > INLINE void constexpr
  mul_SW_QQW_QTW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    t1 = b3 + b2;
    t0 = std::fma ( a0, t1, t0 );
    c2 = t0 + c2;
  }

  // mul: 1-4-4
  template < typename T > INLINE void constexpr
  mul_SW_QQW_QQW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, c3 );
    TwoSum( c1, c2, c1, c2 );
    TwoProductFMA( a0, b2, t0, t1 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, t0, c2, t0 );
    c3 = std::fma ( a0, b3, c3 );
    c3 = c3 + t0 + t1;
  }

  // mul: 2-1-1
  template < typename T > INLINE void constexpr
  mul_PA_SW_SW( T const a0, T const a1, T const b0, T &c0 ) NOEXCEPT
  {
    T t0;
    t0 = a0 + a1;
    c0 = t0 * b0;
  }

  // mul: 2-1-2
  template < typename T > INLINE void constexpr
  mul_PA_SW_PA( T const a0, T const a1, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    TwoProductFMA( a0, b0, c0, c1 );
    c1 = std::fma ( b0, a1, c1 );
  }

  // mul: 2-1-3
  template < typename T > INLINE void constexpr
  mul_PA_SW_QTW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    c2 = t0 + c2;
  }

  // mul: 2-1-4
  template < typename T > INLINE void constexpr
  mul_PA_SW_QQW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    c2 = t0 + c2;
    c3 = fp_const<T>::zero();
  }

  // mul: 2-2-1
  template < typename T > INLINE void constexpr
  mul_PA_PA_SW( T const a0, T const a1, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    t0 = a0 + a1;
    t1 = b0 + b1;
    c0 = t0 * t1;
  }

  // mul: 2-2-2
  template < typename T > INLINE void constexpr
  mul_PA_PA_PA( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    TwoProductFMA( a0, b0, c0, c1 );
    c1 = std::fma ( a0, b1, c1 );
    c1 = std::fma ( b0, a1, c1 );
  }

  // mul: 2-2-3
  template < typename T > INLINE void constexpr
  mul_PA_PA_QTW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    c2 = std::fma ( a1, b1, c2 );
    c2 = t0 + c2 + t2 + t1;
  }

  // mul: 2-2-4
  template < typename T > INLINE void constexpr
  mul_PA_PA_QQW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_PA_QTW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    t0 = a0 + a1;
    t1 = b0 + b1 + b2;
    c0 = t0 * t1;
  }

  // mul: 2-3-2
  template < typename T > INLINE void constexpr
  mul_PA_QTW_PA( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = b1 + b2;
    c1 = std::fma ( a0, t0, c1 );
    c1 = std::fma ( b0, a1, c1 );
  }

  // mul: 2-3-3
  template < typename T > INLINE void constexpr
  mul_PA_QTW_QTW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    t0 = std::fma ( a0, b2, t0 );
    c2 = std::fma ( a1, b1, c2 );
    c2 = t0 + c2 + t2 + t1;
  }

  // mul: 2-3-4
  template < typename T > INLINE void constexpr
  mul_PA_QTW_QQW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    c3 = std::fma ( a1, b2, c3 );
    c3 = c3 + t0 + t1 + t2 + t3 + t4 + t5;
  }

  // mul: 2-4-1
  template < typename T > INLINE void constexpr
  mul_PA_QQW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    t0 = a0 + a1;
    t1 = b0 + b1 + b2 + b3;
    c0 = t0 * t1;
  }

  // mul: 2-4-2
  template < typename T > INLINE void constexpr
  mul_PA_QQW_PA( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = b1 + b2 + b3;
    c1 = std::fma ( a0, t0, c1 );
    c1 = std::fma ( b0, a1, c1 );
  }

  // mul: 2-4-3
  template < typename T > INLINE void constexpr
  mul_PA_QQW_QTW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    t3 = b3 + b2;
    t0 = std::fma ( a0, t3, t0 );
    c2 = std::fma ( a1, b1, c2 );
    c2 = t0 + c2 + t2 + t1;
  }

  // mul: 2-4-4
  template < typename T > INLINE void constexpr
  mul_PA_QQW_QQW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    c3 = std::fma ( a0, b3, c3 );
    c3 = std::fma ( a1, b2, c3 );
  }
  // madd: 2-4-4
  template < typename T > INLINE void constexpr
  madd_PA_QQW_QQW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
#if 1
#if 1
    T d0, d1, d2, d3, d4, d5, e0, e1, e2, e3;
    TwoProductFMA( a0, b0, d0, d1 );
    TwoProductFMA( a0, b1, d2, d3 );
    TwoProductFMA( a0, b2, d4, d5 );
    TwoProductFMA( a1, b0, e0, e1 );
    TwoProductFMA( a1, b1, e2, e3 );

    TwoSum( d1, d2, d1, d2 );
    TwoSum( d1, e0, d1, e0 );    
    //
    TwoSum( d3, d4, d3, d4 );    
    TwoSum( d3, e1, d3, e1 );
    TwoSum( d3, e2, d3, e2 );
    TwoSum( d3, d2, d3, d2 );
    TwoSum( d3, e0, d3, e0 );

    d5 = d5 + e3 + d4 + e1 + e2 + d2 + e0;
    d5 = std::fma(a0, b3, d5);
    d5 = std::fma(a1, b2, d5);

    TwoSum( c0, d0, c0, d0 );
    
    TwoSum( c1, d1, c1, d1 );
    TwoSum( c1, d0, c1, d0 );        

    TwoSum( c2, d3, c2, d3 );
    TwoSum( c2, d1, c2, d1 );
    TwoSum( c2, d0, c2, d0 );

    c3 = c3 + d5 + d3 + d1 + d0;
#else
    T d0, d1, d2, d3, d4, d5, e0, e1, e2, e3;
    TwoProductFMA( a0, b0, d0, d1 );
    TwoProductFMA( a0, b1, d2, d3 );
    TwoProductFMA( a0, b2, d4, d5 );
    TwoProductFMA( a1, b0, e0, e1 );
    TwoProductFMA( a1, b1, e2, e3 );

    TwoSum( c0, d0, c0, d0 );
    //
    TwoSum( c1, d0, c1, d0 );

    TwoSum( c1, d1, c1, d1 );
    TwoSum( c1, d2, c1, d2 );
    TwoSum( c1, e0, c1, e0 );
    //
    TwoSum( c2, d0, c2, d0 );

    TwoSum( c2, d1, c2, d1 );
    TwoSum( c2, d2, c2, d2 );
    TwoSum( c2, e0, c2, e0 );

    TwoSum( c2, d3, c2, d3 );
    TwoSum( c2, d4, c2, d4 );
    TwoSum( c2, e1, c2, e1 );
    TwoSum( c2, e2, c2, e2 );
    
#if 0
    //
    c3 = c3 + d0 + d1 + d2 + e0 + d3 + d4 + e1 + e2 + d5 + e3;
    c3 = std::fma( a0, b3, c3);
    c3 = std::fma( a1, b2, c3);
    //  c3 = std::fma( a1, b3, c3);
#else
    T d6, d7, e4, e5, c4;
    TwoProductFMA( a0, b3, d6, d7 );
    TwoProductFMA( a1, b2, e4, e5 );
    
    TwoSum( c3, d0, c3, d0 );

    TwoSum( c3, d1, c3, d1 );
    TwoSum( c3, d2, c3, d2 );
    TwoSum( c3, e0, c3, e0 );

    TwoSum( c3, d3, c3, d3 );

    TwoSum( c3, d4, c3, d4 );
    TwoSum( c3, e1, c3, e1 );
    TwoSum( c3, e2, c3, e2 );
    
    TwoSum( c3, d5, c3, d5 );
    TwoSum( c3, d6, c3, d6 );
    TwoSum( c3, e3, c3, e3 );
    TwoSum( c3, e4, c3, e4 );
    c4 = d7 + e5
      + d0 + d1 + d2 + e0
      + d3 + d4 + e1 + e2
      + d5 + d6 + e3 + e4;
    c4 = std::fma(a1, b3, c4);
    //    TwoSum(c0, c1, c0, c1);
    //    TwoSum(c1, c2, c1, c2);
    //    TwoSum(c2, c3, c2, c3);
    TwoSum(c3, c4, c3, c4);
#endif
#endif
#else
    T d0, d1, d2, d3;
    mul_PA_QQW_QQW(a0, a1, b0, b1, b2, b3, d0, d1, d2, d3);
    add_QQW_QQW_QQW(c0, c1, c2, c3, d0, d1, d2, d3, c0, c1, c2, c3);    
#endif
  }

  // mul: 3-1-1
  template < typename T > INLINE void constexpr
  mul_QTW_SW_SW( T const a0, T const a1, T const a2, T const b0, T &c0 ) NOEXCEPT
  {
    T t0;
    t0 = a0 + a1 + a2;
    c0 = t0 * b0;
  }

  // mul: 3-1-2
  template < typename T > INLINE void constexpr
  mul_QTW_SW_PA( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = a1 + a2;
    c1 = std::fma ( b0, t0, c1 );
  }

  // mul: 3-1-3
  template < typename T > INLINE void constexpr
  mul_QTW_SW_QTW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    t1 = std::fma ( a2, b0, t1 );
    c2 = t1 + t0;
  }

  // mul: 3-1-4
  template < typename T > INLINE void constexpr
  mul_QTW_SW_QQW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    TwoProductFMA( a2, b0, t1, t2 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    c3 = t0 + t1 + t2;
  }

  // mul: 3-2-1
  template < typename T > INLINE void constexpr
  mul_QTW_PA_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    t0 = a0 + a1 + a2;
    t1 = b0 + b1;
    c0 = t0 * t1;
  }

  // mul: 3-2-2
  template < typename T > INLINE void constexpr
  mul_QTW_PA_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = a1 + a2;
    c1 = std::fma ( a0, b1, c1 );
    c1 = std::fma ( b0, t0, c1 );
  }

  // mul: 3-2-3
  template < typename T > INLINE void constexpr
  mul_QTW_PA_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    c2 = std::fma ( a1, b1, c2 );
    t2 = std::fma ( a2, b0, t2 );
    c2 = t0 + c2 + t2 + t1;
  }

  // mul: 3-2-4
  template < typename T > INLINE void constexpr
  mul_QTW_PA_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    c3 = std::fma ( a2, b1, c3 );
    c3 = c3 + t0 + t1 + t2 + t3 + t4 + t5;
  }

  // mul: 3-3-1
  template < typename T > INLINE void constexpr
  mul_QTW_QTW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    t0 = a0 + a1 + a2;
    t1 = b0 + b1 + b2;
    c0 = t0 * t1;
  }

  // mul: 3-3-2
  template < typename T > INLINE void constexpr
  mul_QTW_QTW_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = a1 + a2;
    t1 = b1 + b2;
    c1 = std::fma ( a0, t1, c1 );
    c1 = std::fma ( b0, t0, c1 );
  }

  // mul: 3-3-3
  template < typename T > INLINE void constexpr
  mul_QTW_QTW_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    t0 = std::fma ( a0, b2, t0 );
    c2 = std::fma ( a1, b1, c2 );
    t2 = std::fma ( a2, b0, t2 );
    c2 = t0 + c2 + t2 + t1;
  }

  // mul: 3-3-4
  template < typename T > INLINE void constexpr
  mul_QTW_QTW_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    c3 = std::fma ( a1, b2, c3 );
    c3 = std::fma ( a2, b1, c3 );
    c3 = c3 + t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7;
  }

  // mul: 3-4-1
  template < typename T > INLINE void constexpr
  mul_QTW_QQW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    t0 = a0 + a1 + a2;
    t1 = b0 + b1 + b2 + b3;
    c0 = t0 * t1;
  }

  // mul: 3-4-2
  template < typename T > INLINE void constexpr
  mul_QTW_QQW_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = a1 + a2;
    t1 = b1 + b2 + b3;
    c1 = std::fma ( a0, t1, c1 );
    c1 = std::fma ( b0, t0, c1 );
  }

  // mul: 3-4-3
  template < typename T > INLINE void constexpr
  mul_QTW_QQW_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    t3 = b3 + b2;
    t0 = std::fma ( a0, t3, t0 );
    c2 = std::fma ( a1, b1, c2 );
    t2 = std::fma ( a2, b0, t2 );
    c2 = t0 + c2 + t2 + t1;
  }

  // mul: 3-4-4
  template < typename T > INLINE void constexpr
  mul_QTW_QQW_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    c3 = std::fma ( a0, b3, c3 );
    c3 = std::fma ( a1, b2, c3 );
    c3 = std::fma ( a2, b1, c3 );
    c3 = c3 + t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7;
  }

  // mul: 4-1-1
  template < typename T > INLINE void constexpr
  mul_QQW_SW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0 ) NOEXCEPT
  {
    T t0;
    t0 = a0 + a1 + a2 + a3;
    c0 = t0 * b0;
  }

  // mul: 4-1-2
  template < typename T > INLINE void constexpr
  mul_QQW_SW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = a1 + a2 + a3;
    c1 = std::fma ( b0, t0, c1 );
  }

  // mul: 4-1-3
  template < typename T > INLINE void constexpr
  mul_QQW_SW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, t0, t1 );
    TwoSum( c1, t0, c1, t0 );
    t2 = a3 + a2;
    t1 = std::fma ( t2, b0, t1 );
    c2 = t1 + t0;
  }

  // mul: 4-1-4
  template < typename T > INLINE void constexpr
  mul_QQW_SW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    TwoProductFMA( a2, b0, t1, t2 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    c3 = a3 * b0;
    c3 = c3 + t0 + t1 + t2;
  }

  // mul: 4-2-1
  template < typename T > INLINE void constexpr
  mul_QQW_PA_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    t0 = a0 + a1 + a2 + a3;
    t1 = b0 + b1;
    c0 = t0 * t1;
  }

  // mul: 4-2-2
  template < typename T > INLINE void constexpr
  mul_QQW_PA_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = a1 + a2 + a3;
    c1 = std::fma ( a0, b1, c1 );
    c1 = std::fma ( b0, t0, c1 );
  }

  // mul: 4-2-3
  template < typename T > INLINE void constexpr
  mul_QQW_PA_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    t3 = a3 + a2;
    c2 = std::fma ( a1, b1, c2 );
    t2 = std::fma ( t3, b0, t2 );
    c2 = t0 + c2 + t2 + t1;
  }

  // mul: 4-2-4
  template < typename T > INLINE void constexpr
  mul_QQW_PA_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    c3 = std::fma ( a2, b1, c3 );
    c3 = std::fma ( a3, b0, c3 );
    c3 = c3 + t0 + t1 + t2 + t3 + t4 + t5;
  }

  // mul: 4-3-1
  template < typename T > INLINE void constexpr
  mul_QQW_QTW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    t0 = a0 + a1 + a2 + a3;
    t1 = b0 + b1 + b2;
    c0 = t0 * t1;
  }

  // mul: 4-3-2
  template < typename T > INLINE void constexpr
  mul_QQW_QTW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = a1 + a2 + a3;
    t1 = b1 + b2;
    c1 = std::fma ( a0, t1, c1 );
    c1 = std::fma ( b0, t0, c1 );
  }

  // mul: 4-3-3
  template < typename T > INLINE void constexpr
  mul_QQW_QTW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    t3 = a3 + a2;
    t0 = std::fma ( a0, b2, t0 );
    c2 = std::fma ( a1, b1, c2 );
    t2 = std::fma ( t3, b0, t2 );
    c2 = t0 + c2 + t2 + t1;
  }

  // mul: 4-3-4
  template < typename T > INLINE void constexpr
  mul_QQW_QTW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    c3 = std::fma ( a1, b2, c3 );
    c3 = std::fma ( a2, b1, c3 );
    c3 = std::fma ( a3, b0, c3 );
    c3 = c3 + t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7;
  }

  // mul: 4-4-1
  template < typename T > INLINE void constexpr
  mul_QQW_QQW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    t0 = a0 + a1 + a2 + a3;
    t1 = b0 + b1 + b2 + b3;
    c0 = t0 * t1;
  }

  // mul: 4-4-2
  template < typename T > INLINE void constexpr
  mul_QQW_QQW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    TwoProductFMA( a0, b0, c0, c1 );
    t0 = a1 + a2 + a3;
    t1 = b1 + b2 + b3;
    c1 = std::fma ( a0, t1, c1 );
    c1 = std::fma ( b0, t0, c1 );
  }

  // mul: 4-4-3
  template < typename T > INLINE void constexpr
  mul_QQW_QQW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_QQW_QQW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    c3 = std::fma ( a0, b3, c3 );
    c3 = std::fma ( a1, b2, c3 );
    c3 = std::fma ( a2, b1, c3 );
    c3 = std::fma ( a3, b0, c3 );
    c3 = c3 + t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7;
  }

  // madd: 4-4-4
  template < typename T > INLINE void constexpr
  madd_QQW_QQW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
#if 1
#if 0
    T d0, d1, d2, d3, d4, d5, e0, e1, e2, e3, f0, f1;
    TwoProductFMA( a0, b0, d0, d1 );
    TwoProductFMA( a0, b1, d2, d3 );
    TwoProductFMA( a0, b2, d4, d5 );
    TwoProductFMA( a1, b0, e0, e1 );
    TwoProductFMA( a1, b1, e2, e3 );
    TwoProductFMA( a2, b0, f0, f1 );    
    
    TwoSum( d1, d2, d1, d2 );
    TwoSum( d1, e0, d1, e0 );    
    //
    TwoSum( d3, d4, d3, d4 );    
    TwoSum( d3, e1, d3, e1 );
    TwoSum( d3, e2, d3, e2 );
    TwoSum( d3, f0, d3, f0 );    
    TwoSum( d3, d2, d3, d2 );
    TwoSum( d3, e0, d3, e0 );

    d5 = d5 + e3 + f1 + d4 + e1 + e2 + f0 + d2 + e0;
    d5 = std::fma(a0, b3, d5);
    d5 = std::fma(a1, b2, d5);
    d5 = std::fma(a2, b1, d5);
    d5 = std::fma(a3, b0, d5);            

    TwoSum( c0, d0, c0, d0 );
    
    TwoSum( c1, d1, c1, d1 );
    TwoSum( c1, d0, c1, d0 );        

    TwoSum( c2, d3, c2, d3 );
    TwoSum( c2, d1, c2, d1 );
    TwoSum( c2, d0, c2, d0 );

    c3 = c3 + d5 + d3 + d1 + d0;
    
#else
    T d0, d1, d2, d3, d4, d5, e0, e1, e2, e3, f0, f1;
    TwoProductFMA( a0, b0, d0, d1 );
    TwoProductFMA( a0, b1, d2, d3 );
    TwoProductFMA( a0, b2, d4, d5 );
    TwoProductFMA( a1, b0, e0, e1 );
    TwoProductFMA( a1, b1, e2, e3 );
    TwoProductFMA( a2, b0, f0, f1 );

    TwoSum( c0, d0, c0, d0 );
    //
    TwoSum( c1, d0, c1, d0 );
    
    TwoSum( c1, d1, c1, d1 );
    TwoSum( c1, d2, c1, d2 );
    TwoSum( c1, e0, c1, e0 );
    //
    TwoSum( c2, d0, c2, d0 );

    TwoSum( c2, d1, c2, d1 );
    TwoSum( c2, d2, c2, d2 );
    TwoSum( c2, e0, c2, e0 );

    TwoSum( c2, d3, c2, d3 );
    TwoSum( c2, d4, c2, d4 );
    TwoSum( c2, e1, c2, e1 );
    TwoSum( c2, e2, c2, e2 );
    TwoSum( c2, f0, c2, f0 );
    //
#if 1
    c3 = (c3 + d0 + d1 + d2 + e0 + d3 + d4 + e1 + e2 + f0
	  + d5 + e3 + f1);
    c3 = std::fma( a0, b3, c3);
    c3 = std::fma( a1, b2, c3);
    c3 = std::fma( a2, b1, c3);
    c3 = std::fma( a3, b0, c3);
    //  c3 = std::fma( a1, b3, c3);
    //  c3 = std::fma( a2, b2, c3);
    //  c3 = std::fma( a3, b1, c3);
#else
    T d6, d7, e4, e5, f2, f3, c4, h0, h1;
    TwoProductFMA( a0, b3, d6, d7 );
    TwoProductFMA( a1, b2, e4, e5 );
    TwoProductFMA( a2, b1, f2, f3 );
    TwoProductFMA( a3, b0, h0, h1 );

    TwoSum( c3, d0, c3, d0 );

    TwoSum( c3, d1, c3, d1 );
    TwoSum( c3, d2, c3, d2 );
    TwoSum( c3, e0, c3, e0 );

    TwoSum( c3, d3, c3, d3 );
    TwoSum( c3, d4, c3, d4 );
    TwoSum( c3, e1, c3, e1 );
    TwoSum( c3, e2, c3, e2 );
    TwoSum( c3, f0, c3, f0 );

    TwoSum( c3, d5, c3, d5 );
    TwoSum( c3, d6, c3, d6 );
    TwoSum( c3, e3, c3, e3 );
    TwoSum( c3, e4, c3, e4 );
    TwoSum( c3, f1, c3, f1 );
    TwoSum( c3, f2, c3, f2 );
    TwoSum( c3, h0, c3, h0 );
    //
    c4 = d7 + e5 + f3 + h1 +
      d0 +
      d1 + d2 + e0 +
      d3 + d4 + e1 + e2 + f0 +
      d5 + d6 + e3 + e4 + f1 + f2 + h0;
    c4 = std::fma( a1, b3, c4);
    c4 = std::fma( a2, b2, c4);
    c4 = std::fma( a3, b1, c4);
    TwoSum(c0, c1, c0, c1);
    TwoSum(c1, c2, c1, c2);
    TwoSum(c2, c3, c2, c3);
    TwoSum(c3, c4, c3, c4);
#endif
#endif
#else
    T d0, d1, d2, d3;
    mul_QQW_QQW_QQW(a0, a1, a2, a3, b0, b1, b2, b3, d0, d1, d2, d3);
    add_QQW_QQW_QQW(c0, c1, c2, c3, d0, d1, d2, d3, c0, c1, c2, c3);    
#endif
  }

  // madd: 4-4-5
  template < typename T > INLINE void constexpr
  madd_QQW_QQW_QPW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
#if 1
    T d0, d1, d2, d3, d4, d5, d6, d7;
    T e0, e1, e2, e3, e4, e5;
    T f0, f1, f2, f3, g0, g1;
    
    TwoProductFMA(a0, b0, d0, d1);
    TwoProductFMA(a0, b1, d2, d3);
    TwoProductFMA(a0, b2, d4, d5);
    TwoProductFMA(a0, b3, d6, d7);
    
    TwoProductFMA(a1, b0, e0, e1);
    TwoProductFMA(a1, b1, e2, e3);
    TwoProductFMA(a1, b2, e4, e5);
    
    TwoProductFMA(a2, b0, f0, f1);
    TwoProductFMA(a2, b1, f2, f3);
    
    TwoProductFMA(a3, b0, g0, g1);


    TwoSum(d1, d2, d1, d2);
    TwoSum(d1, e0, d1, e0);    

    TwoSum(d3, d4, d3, d4);
    TwoSum(d3, e1, d3, e1);
    TwoSum(d3, e2, d3, e2);
    TwoSum(d3, f0, d3, f0);
    
    TwoSum(d3, d2, d3, d2);
    TwoSum(d3, e0, d3, e0);   

    TwoSum(d5, d6, d5, d6);
    TwoSum(d5, e3, d5, e3);
    TwoSum(d5, e4, d5, e4);
    TwoSum(d5, f1, d5, f1);
    TwoSum(d5, f2, d5, f2);
    TwoSum(d5, g0, d5, g0);
    
    TwoSum(d5, d4, d5, d4);
    TwoSum(d5, e1, d5, e1);
    TwoSum(d5, e2, d5, e2);
    TwoSum(d5, f0, d5, f0);
    
    TwoSum(d5, d2, d5, d2);
    TwoSum(d5, e0, d5, e0);   
#if 1
    d7 = (d7 + e5 + f3 + g1 
	  + d2 + e0 + d4 + e1 + e2 + f0
	  + d6 + e3 + e4 + f1 + f2 + g0);
      
    //    d7 = std::fma (a0, b4, d7);
    d7 = std::fma (a1, b3, d7);
    d7 = std::fma (a2, b2, d7);
    d7 = std::fma (a3, b1, d7);
    //    d7 = std::fma (a4, b0, d7);
    
    TwoSum(c0, d0, c0, d0);
    TwoSum(c1, d1, c1, d1);
    TwoSum(c1, d0, c1, d0);

    TwoSum(c2, d3, c2, d3);
    TwoSum(c2, d1, c2, d1);
    TwoSum(c2, d0, c2, d0);
    
    TwoSum(c3, d5, c3, d5);
    TwoSum(c3, d3, c3, d3);
    TwoSum(c3, d1, c3, d1);
    TwoSum(c3, d0, c3, d0);

    c4 = c4 + d7 + d0 + d1 + d3 + d5;

#else
    T d8, d9, e6, e7, f4, f5, g2, g3, h0, h1;
    //    TwoProductFMA(a0, b4, d8, d9);
    TwoProductFMA(a1, b3, e6, e7);
    TwoProductFMA(a2, b2, f4, f5);
    TwoProductFMA(a3, b1, g2, g3);
    //    TwoProductFMA(a4, b0, h0, h1);


    TwoSum(d7, d8, d7, d8);
    TwoSum(d7, e5, d7, e5);
    TwoSum(d7, e6, d7, e6);
    TwoSum(d7, f3, d7, f3);
    TwoSum(d7, f4, d7, f4);
    TwoSum(d7, g1, d7, g1);
    TwoSum(d7, g2, d7, g2);
    TwoSum(d7, h0, d7, h0);   

    TwoSum(d7, d6, d7, d6);
    TwoSum(d7, e3, d7, e3);
    TwoSum(d7, e4, d7, e4);
    TwoSum(d7, f1, d7, f1);
    TwoSum(d7, f2, d7, f2);
    TwoSum(d7, g0, d7, g0);

    TwoSum(d7, d4, d7, d4);
    TwoSum(d7, e1, d7, e1);
    TwoSum(d7, e2, d7, e2);
    TwoSum(d7, f0, d7, f0);
    
    TwoSum(d7, d2, d7, d2);
    TwoSum(d7, e0, d7, e0);   

    d9 = (d9
	  + e7 + f5 + g3 + h1
	  + d8 + e5 + e6 + f3 + f4 + g1 + g2 + h0
	  + d6 + e3 + e4 + f1 + f2 + g0
	  + d4 + e1 + e2 + f0
	  + d2 + e0);
    //    d9 = std::fma (a1, b4, d9);
    d9 = std::fma (a2, b3, d9);
    d9 = std::fma (a3, b2, d9);
    //    d9 = std::fma (a4, b1, d9);

    TwoSum(c0, d0, c0, d0);
    TwoSum(c1, d1, c1, d1);
    TwoSum(c1, d0, c1, d0);

    TwoSum(c2, d3, c2, d3);
    TwoSum(c2, d1, c2, d1);
    TwoSum(c2, d0, c2, d0);
    
    TwoSum(c3, d5, c3, d5);
    TwoSum(c3, d3, c3, d3);
    TwoSum(c3, d1, c3, d1);
    TwoSum(c3, d0, c3, d0);

    TwoSum(c4, d7, c4, d7);
    TwoSum(c4, d5, c4, d5);
    TwoSum(c4, d3, c4, d3);
    TwoSum(c4, d1, c4, d1);
    TwoSum(c4, d0, c4, d0);

    d9 = d9 + d0 + d1 + d3 + d5 + d7;
    TwoSum(c4, d9, c4, d9);
#endif
#else
    // TwoSum() is not commutative
    T d0, d1, d2, d3, d4, d5, d6, d7;
    T e0, e1, e2, e3, e4, e5;
    T f0, f1, f2, f3, g0, g1;
    
    TwoProductFMA(a0, b0, d0, d1);
    TwoProductFMA(a0, b1, d2, d3);
    TwoProductFMA(a0, b2, d4, d5);
    TwoProductFMA(a0, b3, d6, d7);
    
    TwoProductFMA(a1, b0, e0, e1);
    TwoProductFMA(a1, b1, e2, e3);
    TwoProductFMA(a1, b2, e4, e5);
    
    TwoProductFMA(a2, b0, f0, f1);
    TwoProductFMA(a2, b1, f2, f3);
    
    TwoProductFMA(a3, b0, g0, g1);

    TwoSum(c0, d0, c0, d0);

    TwoSum(c1, d1, c1, d1);    

    TwoSum(c1, d2, c1, d2);
    TwoSum(c1, e0, c1, e0);
    TwoSum(c1, d0, c1, d0);

    TwoSum(c2, d3, c2, d3);
    TwoSum(c2, d4, c2, d4);
    TwoSum(c2, e1, c2, e1);
    TwoSum(c2, e2, c2, e2);
    TwoSum(c2, f0, c2, f0);
    
    TwoSum(c2, d1, c2, d1);
    TwoSum(c2, e2, c2, e2);
    TwoSum(c2, e0, c2, e0);   
    TwoSum(c2, d0, c2, d0);

    //

    TwoSum(c3, d3, c3, d3);

    TwoSum(c3, d5, c3, d5);
    TwoSum(c3, d6, c3, d6);
    TwoSum(c3, e3, c3, e3);
    TwoSum(c3, e4, c3, e4);   
    TwoSum(c3, f1, c3, f1);
    TwoSum(c3, f2, c3, f2);
    TwoSum(c3, g0, c3, g0);
    
    TwoSum(c3, d4, c3, d4);
    TwoSum(c3, e1, c3, e1);
    TwoSum(c3, e2, c3, e2);
    TwoSum(c3, f0, c3, f0);

    TwoSum(c3, d1, c3, d1);
    TwoSum(c3, d2, c3, d2);
    TwoSum(c3, e0, c3, e0);

    TwoSum(c3, d0, c3, d0);    
#if 1
    T t0 = (d0 + d1 + d2 + e0
	    + d3 + d4 + e1 + e2 + f0
	    + d5 + d6 + e3 + e4 + f1 + f2 + g0);
    c4 = c4 + d7 + e5 + f3 + g1 + t0;
    //    c4 = std::fma (a0, b4, c4);
    c4 = std::fma (a1, b3, c4);
    c4 = std::fma (a2, b2, c4);
    c4 = std::fma (a3, b1, c4);
    //    c4 = std::fma (a4, b0, c4);   

    //    c4 = std::fma (a1, b4, c4);
    //    c4 = std::fma (a2, b3, c4);
    //    c4 = std::fma (a3, b2, c4);
    //    c4 = std::fma (a4, b1, c4);

#else
    T d8, d9, e6, e7, f4, f5, g2, g3, h0, h1, c5;
    //    TwoProductFMA(a0, b4, d8, d9);
    TwoProductFMA(a1, b3, e6, e7);
    TwoProductFMA(a2, b2, f4, f5);
    TwoProductFMA(a3, b1, g2, g3);
    //    TwoProductFMA(a4, b0, h0, h1);
    
    TwoSum(c4, d0, c4, d0);
    TwoSum(c4, d1, c4, d1);
    TwoSum(c4, e2, c4, e2);
    TwoSum(c4, e0, c4, e0);   
    TwoSum(c4, d3, c4, d3);
    TwoSum(c4, d4, c4, d4);
    TwoSum(c4, e1, c4, e1);
    TwoSum(c4, e2, c4, e2);
    TwoSum(c4, f0, c4, f0);
    
    TwoSum(c4, d5, c4, d5);
    TwoSum(c4, d6, c4, d6);
    TwoSum(c4, e3, c4, e3);
    TwoSum(c4, e4, c4, e4);   
    TwoSum(c4, f1, c4, f1);
    TwoSum(c4, f2, c4, f2);
    TwoSum(c4, g0, c4, g0);

    TwoSum(c4, d7, c4, d7);
    TwoSum(c4, d8, c4, d8);
    TwoSum(c4, e5, c4, e5);
    TwoSum(c4, e6, c4, e6);   
    TwoSum(c4, f3, c4, f3);
    TwoSum(c4, f4, c4, f4);
    TwoSum(c4, g1, c4, g1);
    TwoSum(c4, g2, c4, g2);
    TwoSum(c4, h0, c4, h0);        

    c5 =( d0
	  + d1 + d2 + e0
	  + d3 + d4 + e1 + e2 + f0
	  + d5 + d6 + e3 + e4 + f1 + f2 + g0
	  + d7 + d8 + e5 + e6 + f3 + f4 + g1 + g2 + h0
	  + d9 + e7 + f5 + g3 + h1 );
    c5 = std::fma (a1, b4, c5);
    c5 = std::fma (a2, b3, c5);
    c5 = std::fma (a3, b2, c5);
    c5 = std::fma (a4, b1, c5);

    TwoSum( c0, c1, c0, c1);
    TwoSum( c1, c2, c1, c2);
    TwoSum( c2, c3, c2, c3);
    TwoSum( c3, c4, c3, c4);
    TwoSum( c4, c5, c4, c5);    
#endif
#endif
  }
  
  // mul: 5-5-5
  template < typename T > INLINE void constexpr
  mul_QPW_QPW_QPW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
   T d2, d3, d4, d5, d6, d7;
   T e0, e1, e2, e3, e4, e5;
   T f0, f1, f2, f3, g0, g1;

   TwoProductFMA(a0, b0, c0, c1);
   TwoProductFMA(a0, b1, d2, c2);
   TwoProductFMA(a0, b2, d4, c3);
   TwoProductFMA(a0, b3, d6, c4);

   TwoProductFMA(a1, b0, e0, e1);
   TwoProductFMA(a1, b1, e2, e3);
   TwoProductFMA(a1, b2, e4, e5);

   TwoProductFMA(a2, b0, f0, f1);
   TwoProductFMA(a2, b1, f2, f3);

   TwoProductFMA(a3, b0, g0, g1);

   TwoSum(c1, d2, c1, d2);
   TwoSum(c1, e0, c1, e0);

   TwoSum(c2, d2, c2, d2);
   TwoSum(c2, e0, c2, e0);

   TwoSum(c2, d4, c2, d4);
   TwoSum(c2, e1, c2, e1);
   TwoSum(c2, e2, c2, e2);
   TwoSum(c2, f0, c2, f0);   

   TwoSum(c3, d2, c3, d2);
   TwoSum(c3, e0, c3, e0);

   TwoSum(c3, d4, c3, d4);
   TwoSum(c3, e1, c3, e1);
   TwoSum(c3, e2, c3, e2);
   TwoSum(c3, f0, c3, f0);   

   TwoSum(c3, d6, c3, d6);
   TwoSum(c3, e3, c3, e3);
   TwoSum(c3, e4, c3, e4);   
   TwoSum(c3, f1, c3, f1);
   TwoSum(c3, f2, c3, f2);
   TwoSum(c3, g0, c3, g0);   
   c4 = (c4 + d2 + e0 + d4 + e1 + e2 + f0 + d6 + e3 + e4 + f1 + f2 + g0
  	    + e5 + f3 + g1);
   c4 = std::fma (a0, b4, c4);
   c4 = std::fma (a1, b3, c4);
   c4 = std::fma (a2, b2, c4);
   c4 = std::fma (a3, b1, c4);
   c4 = std::fma (a4, b0, c4);   

  }

    // mul: 2-5-5
  template < typename T > INLINE void constexpr
  mul_PA_QPW_QPW( T const a0, T const a1, T const b0, T const b1, T const b2 , T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {

    T d2, d4, d6;
    T e0, e1, e2, e3, e4, e5;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, d2, c2 );
    TwoProductFMA( a0, b2, d4, c3 );
    TwoProductFMA( a0, b3, d6, c4 );
    
    TwoProductFMA( a1, b0, e0, e1 );
    TwoProductFMA( a1, b1, e2, e3 );
    TwoProductFMA( a1, b2, e4, e5 );
    //
    TwoSum( c1, d2, c1, d2 );
    TwoSum( c1, e0, c1, e0 );
    //
    TwoSum( c2, d2, c2, d2 );
    TwoSum( c2, e0, c2, e0 );
    
    TwoSum( c2, d4, c2, d4 );
    TwoSum( c2, e1, c2, e1 );
    TwoSum( c2, e2, c2, e2 );
    //
    TwoSum( c3, d2, c3, d2 );
    TwoSum( c3, e0, c3, e0 );

    TwoSum( c3, d4, c3, d4 );
    TwoSum( c3, e1, c3, e1 );
    TwoSum( c3, e2, c3, e2 );

    TwoSum( c3, d6, c3, d6 );
    TwoSum( c3, e3, c3, e3 );
    TwoSum( c3, e4, c3, e4 );
    
    c4 = c4 + d2 + e0 + d4 + e1 + e2
      + d6 + e3 + e4 + e5;
    c4 = std::fma ( a0, b4, c4 );
    c4 = std::fma ( a1, b3, c4 );
  }
    // mul: 5-2-5
    template < typename T > INLINE void constexpr
  mul_QPW_PA_QPW( T const a0, T const a1, T const a2, T const a3, T const a4 , T const b0, T const b1, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
    mul_PA_QPW_QPW(b0, b1, a0, a1, a2, a3, a4, c0, c1, c2, c3, c4);
  }

    // mul: 4-5-5
  template < typename T > INLINE void constexpr
  mul_QQW_QPW_QPW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2 , T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
   T d2, d3, d4, d5, d6, d7;
   T e0, e1, e2, e3, e4, e5;
   T f0, f1, f2, f3, g0, g1;

   TwoProductFMA(a0, b0, c0, c1);
   TwoProductFMA(a0, b1, d2, c2);
   TwoProductFMA(a0, b2, d4, c3);
   TwoProductFMA(a0, b3, d6, c4);

   TwoProductFMA(a1, b0, e0, e1);
   TwoProductFMA(a1, b1, e2, e3);
   TwoProductFMA(a1, b2, e4, e5);

   TwoProductFMA(a2, b0, f0, f1);
   TwoProductFMA(a2, b1, f2, f3);

   TwoProductFMA(a3, b0, g0, g1);

   TwoSum(c1, d2, c1, d2);
   TwoSum(c1, e0, c1, e0);

   TwoSum(c2, d2, c2, d2);
   TwoSum(c2, e0, c2, e0);

   TwoSum(c2, d4, c2, d4);
   TwoSum(c2, e1, c2, e1);
   TwoSum(c2, e2, c2, e2);
   TwoSum(c2, f0, c2, f0);   

   TwoSum(c3, d2, c3, d2);
   TwoSum(c3, e0, c3, e0);

   TwoSum(c3, d4, c3, d4);
   TwoSum(c3, e1, c3, e1);
   TwoSum(c3, e2, c3, e2);
   TwoSum(c3, f0, c3, f0);   

   TwoSum(c3, d6, c3, d6);
   TwoSum(c3, e3, c3, e3);
   TwoSum(c3, e4, c3, e4);   
   TwoSum(c3, f1, c3, f1);
   TwoSum(c3, f2, c3, f2);
   TwoSum(c3, g0, c3, g0);   

   c4 = (c4 + d2 + e0 + d4 + e1 + e2 + f0 + d6 + e3 + e4 + f1 + f2 + g0
  	    + e5 + f3 + g1);
   c4 = std::fma (a0, b4, c4);
   c4 = std::fma (a1, b3, c4);
   c4 = std::fma (a2, b2, c4);
   c4 = std::fma (a3, b1, c4);
  }
  
    // mul: 5-4-5
    template < typename T > INLINE void constexpr
    mul_QPW_QQW_QPW( T const a0, T const a1, T const a2, T const a3, T const a4 , T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
    mul_QQW_QPW_QPW(b0, b1, b2, b3, a0, a1, a2, a3, a4, c0, c1, c2, c3, c4);
  }


  // mul: 3-5-5
  template < typename T > INLINE void constexpr
  mul_QTW_QPW_QPW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2 , T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
   T d2, d3, d4, d5, d6, d7;
   T e0, e1, e2, e3, e4, e5;
   T f0, f1, f2, f3;

   TwoProductFMA(a0, b0, c0, c1);
   TwoProductFMA(a0, b1, d2, c2);
   TwoProductFMA(a0, b2, d4, c3);
   TwoProductFMA(a0, b3, d6, c4);

   TwoProductFMA(a1, b0, e0, e1);
   TwoProductFMA(a1, b1, e2, e3);
   TwoProductFMA(a1, b2, e4, e5);

   TwoProductFMA(a2, b0, f0, f1);
   TwoProductFMA(a2, b1, f2, f3);

   TwoSum(c1, d2, c1, d2);
   TwoSum(c1, e0, c1, e0);

   TwoSum(c2, d2, c2, d2);
   TwoSum(c2, e0, c2, e0);

   TwoSum(c2, d4, c2, d4);
   TwoSum(c2, e1, c2, e1);
   TwoSum(c2, e2, c2, e2);
   TwoSum(c2, f0, c2, f0);   

   TwoSum(c3, d2, c3, d2);
   TwoSum(c3, e0, c3, e0);

   TwoSum(c3, d4, c3, d4);
   TwoSum(c3, e1, c3, e1);
   TwoSum(c3, e2, c3, e2);
   TwoSum(c3, f0, c3, f0);   

   TwoSum(c3, d6, c3, d6);
   TwoSum(c3, e3, c3, e3);
   TwoSum(c3, e4, c3, e4);   
   TwoSum(c3, f1, c3, f1);
   TwoSum(c3, f2, c3, f2);
   //   TwoSum(c3, g0, c3, g0);   

   c4 = (c4 + d2 + e0 + d4 + e1 + e2 + f0 + d6 + e3 + e4 + f1 + f2
  	    + e5 + f3);
   c4 = std::fma (a0, b4, c4);
   c4 = std::fma (a1, b3, c4);
   c4 = std::fma (a2, b2, c4);
  }
    // mul: 5-3-5
    template < typename T > INLINE void constexpr
    mul_QPW_QTW_QPW( T const a0, T const a1, T const a2, T const a3, T const a4 , T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
    mul_QTW_QPW_QPW(b0, b1, b2, a0, a1, a2, a3, a4, c0, c1, c2, c3, c4);
  }
  
  // madd: 2-4-5
  template < typename T > INLINE void constexpr
  madd_PA_QQW_QPW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
    T d0, d1, d2, d3, d4, d5, e0, e1, e2, e3;
    T d6, d7, e4, e5;
    
    TwoProductFMA( a0, b0, d0, d1 );
    TwoProductFMA( a0, b1, d2, d3 );
    TwoProductFMA( a0, b2, d4, d5 );
    TwoProductFMA( a1, b0, e0, e1 );
    TwoProductFMA( a1, b1, e2, e3 );

    TwoSum( c0, d0, c0, d0 );
    //
    TwoSum( c1, d0, c1, d0 );

    TwoSum( c1, d1, c1, d1 );
    TwoSum( c1, d2, c1, d2 );
    TwoSum( c1, e0, c1, e0 );
    //
    TwoSum( c2, d0, c2, d0 );

    TwoSum( c2, d1, c2, d1 );
    TwoSum( c2, d2, c2, d2 );
    TwoSum( c2, e0, c2, e0 );

    TwoSum( c2, d3, c2, d3 );
    TwoSum( c2, d4, c2, d4 );
    TwoSum( c2, e1, c2, e1 );
    TwoSum( c2, e2, c2, e2 );
    

    TwoProductFMA( a0, b3, d6, d7 );
    TwoProductFMA( a1, b2, e4, e5 );
    
    TwoSum( c3, d0, c3, d0 );

    TwoSum( c3, d1, c3, d1 );
    TwoSum( c3, d2, c3, d2 );
    TwoSum( c3, e0, c3, e0 );

    TwoSum( c3, d3, c3, d3 );

    TwoSum( c3, d4, c3, d4 );
    TwoSum( c3, e1, c3, e1 );
    TwoSum( c3, e2, c3, e2 );
    
    TwoSum( c3, d5, c3, d5 );
    TwoSum( c3, d6, c3, d6 );
    TwoSum( c3, e3, c3, e3 );
    TwoSum( c3, e4, c3, e4 );
    c4 = c4 + d7 + e5
      + d0 + d1 + d2 + e0
      + d3 + d4 + e1 + e2
      + d5 + d6 + e3 + e4;
    c4 = std::fma(a1, b3, c4);
    //    TwoSum(c0, c1, c0, c1);
    //    TwoSum(c1, c2, c1, c2);
    //    TwoSum(c2, c3, c2, c3);
    //    TwoSum(c3, c4, c3, c4);

  }  
    // madd: 2-5-5
  template < typename T > INLINE void constexpr
  madd_PA_QPW_QPW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
    T d0, d1, d2, d3, d4, d5, d6, d7;
    T e0, e1, e2, e3, e4, e5;
    T f0, f1, f2, f3, g0, g1;
    
    TwoProductFMA(a0, b0, d0, d1);
    TwoProductFMA(a0, b1, d2, d3);
    TwoProductFMA(a0, b2, d4, d5);
    TwoProductFMA(a0, b3, d6, d7);
    
    TwoProductFMA(a1, b0, e0, e1);
    TwoProductFMA(a1, b1, e2, e3);
    TwoProductFMA(a1, b2, e4, e5);
    

    TwoSum(d1, d2, d1, d2);
    TwoSum(d1, e0, d1, e0);    

    TwoSum(d3, d4, d3, d4);
    TwoSum(d3, e1, d3, e1);
    TwoSum(d3, e2, d3, e2);
    
    TwoSum(d3, d2, d3, d2);
    TwoSum(d3, e0, d3, e0);   

    TwoSum(d5, d6, d5, d6);
    TwoSum(d5, e3, d5, e3);
    TwoSum(d5, e4, d5, e4);
    
    TwoSum(d5, d4, d5, d4);
    TwoSum(d5, e1, d5, e1);
    TwoSum(d5, e2, d5, e2);

    TwoSum(d5, d2, d5, d2);
    TwoSum(d5, e0, d5, e0);   

    d7 = (d7 + e5
	  + d6 + e3 + e4
	  + d4 + e1 + e2
	  + d2 + e0);
      
    d7 = std::fma (a0, b4, d7);
    d7 = std::fma (a1, b3, d7);
    
    TwoSum(c0, d0, c0, d0);

    TwoSum(c1, d1, c1, d1);
    TwoSum(c1, d0, c1, d0);

    TwoSum(c2, d3, c2, d3);
    TwoSum(c2, d1, c2, d1);
    TwoSum(c2, d0, c2, d0);
    
    TwoSum(c3, d5, c3, d5);
    TwoSum(c3, d3, c3, d3);
    TwoSum(c3, d1, c3, d1);
    TwoSum(c3, d0, c3, d0);

    c4 = c4 + d7 + d0 + d1 + d3 + d5;
  }

// madd: 5-5-5
  template < typename T > INLINE void constexpr
  madd_QPW_QPW_QPW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {

    T d0, d1, d2, d3, d4, d5, d6, d7;
    T e0, e1, e2, e3, e4, e5;
    T f0, f1, f2, f3, g0, g1;
    
    TwoProductFMA(a0, b0, d0, d1);
    TwoProductFMA(a0, b1, d2, d3);
    TwoProductFMA(a0, b2, d4, d5);
    TwoProductFMA(a0, b3, d6, d7);
    
    TwoProductFMA(a1, b0, e0, e1);
    TwoProductFMA(a1, b1, e2, e3);
    TwoProductFMA(a1, b2, e4, e5);
    
    TwoProductFMA(a2, b0, f0, f1);
    TwoProductFMA(a2, b1, f2, f3);
    
    TwoProductFMA(a3, b0, g0, g1);


    TwoSum(d1, d2, d1, d2);
    TwoSum(d1, e0, d1, e0);    

    TwoSum(d3, d4, d3, d4);
    TwoSum(d3, e1, d3, e1);
    TwoSum(d3, e2, d3, e2);
    TwoSum(d3, f0, d3, f0);
    
    TwoSum(d3, d2, d3, d2);
    TwoSum(d3, e0, d3, e0);   

    TwoSum(d5, d6, d5, d6);
    TwoSum(d5, e3, d5, e3);
    TwoSum(d5, e4, d5, e4);
    TwoSum(d5, f1, d5, f1);
    TwoSum(d5, f2, d5, f2);
    TwoSum(d5, g0, d5, g0);
    
    TwoSum(d5, d4, d5, d4);
    TwoSum(d5, e1, d5, e1);
    TwoSum(d5, e2, d5, e2);
    TwoSum(d5, f0, d5, f0);
    
    TwoSum(d5, d2, d5, d2);
    TwoSum(d5, e0, d5, e0);   

    d7 = (d7 + e5 + f3 + g1 
	  + d2 + e0 + d4 + e1 + e2 + f0
	  + d6 + e3 + e4 + f1 + f2 + g0);
      
    d7 = std::fma (a0, b4, d7);
    d7 = std::fma (a1, b3, d7);
    d7 = std::fma (a2, b2, d7);
    d7 = std::fma (a3, b1, d7);
    d7 = std::fma (a4, b0, d7);
    
    TwoSum(c0, d0, c0, d0);
    TwoSum(c1, d1, c1, d1);
    TwoSum(c1, d0, c1, d0);

    TwoSum(c2, d3, c2, d3);
    TwoSum(c2, d1, c2, d1);
    TwoSum(c2, d0, c2, d0);
    
    TwoSum(c3, d5, c3, d5);
    TwoSum(c3, d3, c3, d3);
    TwoSum(c3, d1, c3, d1);
    TwoSum(c3, d0, c3, d0);

    c4 = c4 + d7 + d0 + d1 + d3 + d5;
  }

  // div: 1-1-1
  template < typename T > INLINE void constexpr
  div_SW_SW_SW( T const a0, T const b0, T &c0 ) NOEXCEPT
  {
    c0 = a0 / b0;
  }

  // div: 1-1-2
  template < typename T > INLINE void constexpr
  div_SW_SW_PA( T const a0, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    T r = a0;
    c0 = r / b0;
    r = std::fma( -b0, c0, r );
    c1 = r / b0;
  }

  // div: 1-1-3
  template < typename T > INLINE void constexpr
  div_SW_SW_QTW( T const a0, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T r = a0;
    c0 = r / b0;
    r = std::fma( -b0, c0, r );
    c1 = r / b0;
    r = std::fma( -b0, c1, r );
    c2 = r / b0;
  }

  // div: 1-1-4
  template < typename T > INLINE void constexpr
  div_SW_SW_QQW( T const a0, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  div_SW_PA_SW( T const a0, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T e40, e41;
    e40 = a0;
    e41 = b0 + b1;
    c0 = e40 / e41;
  }

  // div: 1-2-2
  template < typename T > INLINE void constexpr
  div_SW_PA_PA( T const a0, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    T e40;
    e40 = b0 + b1;
    div_SW_SW_SW( a0, e40, c0 );
    mul_SW_PA_PA( c0, b0, b1, t0, t1 );
    sub_SW_PA_PA( a0, t0, t1, t0, t1 );
    T tn, td;
    tn = t0 + t1;
    td = e40;
    c1 = tn / td;
  }

  // div: 1-2-3
  template < typename T > INLINE void constexpr
  div_SW_PA_QTW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    div_SW_PA_PA( a0, b0, b1, c0, c1 );
    mul_PA_PA_QTW( c0, c1, b0, b1, t0, t1, t2 );
    sub_SW_QTW_QTW( a0, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0 + b1;
    c2 = tn / td;
  }

  // div: 1-2-4
  template < typename T > INLINE void constexpr
  div_SW_PA_QQW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    div_SW_PA_QTW( a0, b0, b1, c0, c1, c2 );
    mul_QTW_PA_QQW( c0, c1, c2, b0, b1, t0, t1, t2, t3 );
    sub_SW_QQW_QQW( a0, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1;
    c3 = tn / td;
  }

  // div: 1-3-1
  template < typename T > INLINE void constexpr
  div_SW_QTW_SW( T const a0, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T e40, e41;
    e40 = a0;
    e41 = b0 + b1 + b2;
    c0 = e40 / e41;
  }

  // div: 1-3-2
  template < typename T > INLINE void constexpr
  div_SW_QTW_PA( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    T e40;
    e40 = b0 + b1 + b2;
    div_SW_SW_SW( a0, e40, c0 );
    mul_SW_QTW_PA( c0, b0, b1, b2, t0, t1 );
    sub_SW_PA_PA( a0, t0, t1, t0, t1 );
    T tn, td;
    tn = t0 + t1;
    td = e40;
    c1 = tn / td;
  }

  // div: 1-3-3
  template < typename T > INLINE void constexpr
  div_SW_QTW_QTW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    T e40;
    e40 = b1 + b2;
    div_SW_PA_PA( a0, b0, e40, c0, c1 );
    mul_PA_QTW_QTW( c0, c1, b0, b1, b2, t0, t1, t2 );
    sub_SW_QTW_QTW( a0, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0 + b1 + b2;
    c2 = tn / td;
  }

  // div: 1-3-4
  template < typename T > INLINE void constexpr
  div_SW_QTW_QQW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    div_SW_QTW_QTW( a0, b0, b1, b2, c0, c1, c2 );
    mul_QTW_QTW_QQW( c0, c1, c2, b0, b1, b2, t0, t1, t2, t3 );
    sub_SW_QQW_QQW( a0, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1 + b2;
    c3 = tn / td;
  }

  // div: 1-4-1
  template < typename T > INLINE void constexpr
  div_SW_QQW_SW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T e40, e41;
    e40 = a0;
    e41 = b0 + b1 + b2 + b3;
    c0 = e40 / e41;
  }

  // div: 1-4-2
  template < typename T > INLINE void constexpr
  div_SW_QQW_PA( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    T e40;
    e40 = b0 + b1 + b2 + b3;
    div_SW_SW_SW( a0, e40, c0 );
    mul_SW_QQW_PA( c0, b0, b1, b2, b3, t0, t1 );
    sub_SW_PA_PA( a0, t0, t1, t0, t1 );
    T tn, td;
    tn = t0 + t1;
    td = e40;
    c1 = tn / td;
  }

  // div: 1-4-3
  template < typename T > INLINE void constexpr
  div_SW_QQW_QTW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    T e40;
    e40 = b1 + b2 + b3;
    div_SW_PA_PA( a0, b0, e40, c0, c1 );
    mul_PA_QQW_QTW( c0, c1, b0, b1, b2, b3, t0, t1, t2 );
    sub_SW_QTW_QTW( a0, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0 + b1 + b2 + b3;
    c2 = tn / td;
  }

  // div: 1-4-4
  template < typename T > INLINE void constexpr
  div_SW_QQW_QQW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T e40;
    e40 = b2 + b3;
    div_SW_QTW_QTW( a0, b0, b1, e40, c0, c1, c2 );
    mul_QTW_QQW_QQW( c0, c1, c2, b0, b1, b2, b3, t0, t1, t2, t3 );
    sub_SW_QQW_QQW( a0, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1 + b2 + b3;
    c3 = tn / td;
  }

  // div: 2-1-1
  template < typename T > INLINE void constexpr
  div_PA_SW_SW( T const a0, T const a1, T const b0, T &c0 ) NOEXCEPT
  {
    T e40, e41;
    e40 = a0 + a1;
    e41 = b0;
    c0 = e40 / e41;
  }

  // div: 2-1-2
  template < typename T > INLINE void constexpr
  div_PA_SW_PA( T const a0, T const a1, T const b0, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  div_PA_SW_QTW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  div_PA_SW_QQW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  div_PA_PA_SW( T const a0, T const a1, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T ah, bh;
    ah = a0 + a1;
    bh = b0 + b1;
    c0 = ah / bh;
  }

  // div: 2-2-2
  template < typename T > INLINE void constexpr
  div_PA_PA_PA( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    T bh = b0 + b1;
    c0 = a0 / bh;
    c1 = std::fma(-b0, c0, a0 ) + a1;
    c1 = std::fma(-b1, c0, c1 ) / bh;
  }

  // div: 2-2-3
  template < typename T > INLINE void constexpr
  div_PA_PA_QTW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    div_PA_PA_PA( a0, a1, b0, b1, c0, c1 );
    mul_PA_PA_QTW( c0, c1, b0, b1, t0, t1, t2 );
    sub_PA_QTW_QTW( a0, a1, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0 + b1;
    c2 = tn / td;
  }

  // div: 2-2-4
  template < typename T > INLINE void constexpr
  div_PA_PA_QQW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    div_PA_PA_QTW( a0, a1, b0, b1, c0, c1, c2 );
    mul_QTW_PA_QQW( c0, c1, c2, b0, b1, t0, t1, t2, t3 );
    sub_PA_QQW_QQW( a0, a1, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1;
    c3 = tn / td;
  }

  // div: 2-3-1
  template < typename T > INLINE void constexpr
  div_PA_QTW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T e40, e41;
    e40 = a0 + a1;
    e41 = b0 + b1 + b2;
    c0 = e40 / e41;
  }

  // div: 2-3-2
  template < typename T > INLINE void constexpr
  div_PA_QTW_PA( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    T e40;
    e40 = b1 + b2;
    div_PA_PA_PA( a0, a1, b0, e40, c0, c1 );
  }

  // div: 2-3-3
  template < typename T > INLINE void constexpr
  div_PA_QTW_QTW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    T e40;
    e40 = b1 + b2;
    div_PA_PA_PA( a0, a1, b0, e40, c0, c1 );
    mul_PA_QTW_QTW( c0, c1, b0, b1, b2, t0, t1, t2 );
    sub_PA_QTW_QTW( a0, a1, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0 + b1 + b2;
    c2 = tn / td;
  }

  // div: 2-3-4
  template < typename T > INLINE void constexpr
  div_PA_QTW_QQW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    div_PA_QTW_QTW( a0, a1, b0, b1, b2, c0, c1, c2 );
    mul_QTW_QTW_QQW( c0, c1, c2, b0, b1, b2, t0, t1, t2, t3 );
    sub_PA_QQW_QQW( a0, a1, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1 + b2;
    c3 = tn / td;
  }

  // div: 2-4-1
  template < typename T > INLINE void constexpr
  div_PA_QQW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T e40, e41;
    e40 = a0 + a1;
    e41 = b0 + b1 + b2 + b3;
    c0 = e40 / e41;
  }

  // div: 2-4-2
  template < typename T > INLINE void constexpr
  div_PA_QQW_PA( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    T e40;
    e40 = b1 + b2 + b3;
    div_PA_PA_PA( a0, a1, b0, e40, c0, c1 );
  }

  // div: 2-4-3
  template < typename T > INLINE void constexpr
  div_PA_QQW_QTW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    T e40;
    e40 = b1 + b2 + b3;
    div_PA_PA_PA( a0, a1, b0, e40, c0, c1 );
    mul_PA_QQW_QTW( c0, c1, b0, b1, b2, b3, t0, t1, t2 );
    sub_PA_QTW_QTW( a0, a1, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0 + b1 + b2 + b3;
    c2 = tn / td;
  }

  // div: 2-4-4
  template < typename T > INLINE void constexpr
  div_PA_QQW_QQW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T e40;
    e40 = b2 + b3;
    div_PA_QTW_QTW( a0, a1, b0, b1, e40, c0, c1, c2 );
    mul_QTW_QQW_QQW( c0, c1, c2, b0, b1, b2, b3, t0, t1, t2, t3 );
    sub_PA_QQW_QQW( a0, a1, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1 + b2 + b3;
    c3 = tn / td;
  }

  // div: 3-1-1
  template < typename T > INLINE void constexpr
  div_QTW_SW_SW( T const a0, T const a1, T const a2, T const b0, T &c0 ) NOEXCEPT
  {
    T e40, e41;
    e40 = a0 + a1 + a2;
    e41 = b0;
    c0 = e40 / e41;
  }

  // div: 3-1-2
  template < typename T > INLINE void constexpr
  div_QTW_SW_PA( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  div_QTW_SW_QTW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  div_QTW_SW_QQW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  div_QTW_PA_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T e40, e41;
    e40 = a0 + a1 + a2;
    e41 = b0 + b1;
    c0 = e40 / e41;
  }

  // div: 3-2-2
  template < typename T > INLINE void constexpr
  div_QTW_PA_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    T e40;
    e40 = a1 + a2;
    div_PA_PA_PA( a0, e40, b0, b1, c0, c1 );
  }

  // div: 3-2-3
  template < typename T > INLINE void constexpr
  div_QTW_PA_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    T e40;
    e40 = a1 + a2;
    div_PA_PA_PA( a0, e40, b0, b1, c0, c1 );
    mul_PA_PA_QTW( c0, c1, b0, b1, t0, t1, t2 );
    sub_QTW_QTW_QTW( a0, a1, a2, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0 + b1;
    c2 = tn / td;
  }

  // div: 3-2-4
  template < typename T > INLINE void constexpr
  div_QTW_PA_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    div_QTW_PA_QTW( a0, a1, a2, b0, b1, c0, c1, c2 );
    mul_QTW_PA_QQW( c0, c1, c2, b0, b1, t0, t1, t2, t3 );
    sub_QTW_QQW_QQW( a0, a1, a2, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1;
    c3 = tn / td;
  }

  // div: 3-3-1
  template < typename T > INLINE void constexpr
  div_QTW_QTW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T e40, e41;
    e40 = a0 + a1 + a2;
    e41 = b0 + b1 + b2;
    c0 = e40 / e41;
  }

  // div: 3-3-2
  template < typename T > INLINE void constexpr
  div_QTW_QTW_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    T e40;
    e40 = a1 + a2;
    T e41;
    e41 = b1 + b2;
    div_PA_PA_PA( a0, e40, b0, e41, c0, c1 );
  }

  // div: 3-3-3
  template < typename T > INLINE void constexpr
  div_QTW_QTW_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    td = b0 + b1 + b2;
    c2 = tn / td;
  }

  // div: 3-3-4
  template < typename T > INLINE void constexpr
  div_QTW_QTW_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    div_QTW_QTW_QTW( a0, a1, a2, b0, b1, b2, c0, c1, c2 );
    mul_QTW_QTW_QQW( c0, c1, c2, b0, b1, b2, t0, t1, t2, t3 );
    sub_QTW_QQW_QQW( a0, a1, a2, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1 + b2;
    c3 = tn / td;
  }

  // div: 3-4-1
  template < typename T > INLINE void constexpr
  div_QTW_QQW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T e40, e41;
    e40 = a0 + a1 + a2;
    e41 = b0 + b1 + b2 + b3;
    c0 = e40 / e41;
  }

  // div: 3-4-2
  template < typename T > INLINE void constexpr
  div_QTW_QQW_PA( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    T e40;
    e40 = a1 + a2;
    T e41;
    e41 = b1 + b2 + b3;
    div_PA_PA_PA( a0, e40, b0, e41, c0, c1 );
  }

  // div: 3-4-3
  template < typename T > INLINE void constexpr
  div_QTW_QQW_QTW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    T e40;
    e40 = b2 + b3;
    div_QTW_QTW_QTW( a0, a1, a2, b0, b1, e40, c0, c1, c2 );
  }

  // div: 3-4-4
  template < typename T > INLINE void constexpr
  div_QTW_QQW_QQW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T e40;
    e40 = b2 + b3;
    div_QTW_QTW_QTW( a0, a1, a2, b0, b1, e40, c0, c1, c2 );
    mul_QTW_QQW_QQW( c0, c1, c2, b0, b1, b2, b3, t0, t1, t2, t3 );
    sub_QTW_QQW_QQW( a0, a1, a2, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1 + b2 + b3;
    c3 = tn / td;
  }

  // div: 4-1-1
  template < typename T > INLINE void constexpr
  div_QQW_SW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0 ) NOEXCEPT
  {
    T e40, e41;
    e40 = a0 + a1 + a2 + a3;
    e41 = b0;
    c0 = e40 / e41;
  }

  // div: 4-1-2
  template < typename T > INLINE void constexpr
  div_QQW_SW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  div_QQW_SW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  div_QQW_SW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  div_QQW_PA_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T e40, e41;
    e40 = a0 + a1 + a2 + a3;
    e41 = b0 + b1;
    c0 = e40 / e41;
  }

  // div: 4-2-2
  template < typename T > INLINE void constexpr
  div_QQW_PA_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    T e40;
    e40 = a1 + a2 + a3;
    div_PA_PA_PA( a0, e40, b0, b1, c0, c1 );
  }

  // div: 4-2-3
  template < typename T > INLINE void constexpr
  div_QQW_PA_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    T e40;
    e40 = a1 + a2 + a3;
    div_PA_PA_PA( a0, e40, b0, b1, c0, c1 );
    mul_PA_PA_QTW( c0, c1, b0, b1, t0, t1, t2 );
    sub_QQW_QTW_QTW( a0, a1, a2, a3, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = b0 + b1;
    c2 = tn / td;
  }

  // div: 4-2-4
  template < typename T > INLINE void constexpr
  div_QQW_PA_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T e40;
    e40 = a2 + a3;
    div_QTW_PA_QTW( a0, a1, e40, b0, b1, c0, c1, c2 );
    mul_QTW_PA_QQW( c0, c1, c2, b0, b1, t0, t1, t2, t3 );
    sub_QQW_QQW_QQW( a0, a1, a2, a3, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1;
    c3 = tn / td;
  }

  // div: 4-3-1
  template < typename T > INLINE void constexpr
  div_QQW_QTW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T e40, e41;
    e40 = a0 + a1 + a2 + a3;
    e41 = b0 + b1 + b2;
    c0 = e40 / e41;
  }

  // div: 4-3-2
  template < typename T > INLINE void constexpr
  div_QQW_QTW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    T e40;
    e40 = a1 + a2 + a3;
    T e41;
    e41 = b1 + b2;
    div_PA_PA_PA( a0, e40, b0, e41, c0, c1 );
  }

  // div: 4-3-3
  template < typename T > INLINE void constexpr
  div_QQW_QTW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    T e40;
    e40 = a2 + a3;
    div_QTW_QTW_QTW( a0, a1, e40, b0, b1, b2, c0, c1, c2 );
  }

  // div: 4-3-4
  template < typename T > INLINE void constexpr
  div_QQW_QTW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T e40;
    e40 = a2 + a3;
    div_QTW_QTW_QTW( a0, a1, e40, b0, b1, b2, c0, c1, c2 );
    mul_QTW_QTW_QQW( c0, c1, c2, b0, b1, b2, t0, t1, t2, t3 );
    sub_QQW_QQW_QQW( a0, a1, a2, a3, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = b0 + b1 + b2;
    c3 = tn / td;
  }

  // div: 4-4-1
  template < typename T > INLINE void constexpr
  div_QQW_QQW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T e40, e41;
    e40 = a0 + a1 + a2 + a3;
    e41 = b0 + b1 + b2 + b3;
    c0 = e40 / e41;
  }

  // div: 4-4-2
  template < typename T > INLINE void constexpr
  div_QQW_QQW_PA( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    T e40;
    e40 = a1 + a2 + a3;
    T e41;
    e41 = b1 + b2 + b3;
    div_PA_PA_PA( a0, e40, b0, e41, c0, c1 );
  }

  // div: 4-4-3
  template < typename T > INLINE void constexpr
  div_QQW_QQW_QTW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    T e40;
    e40 = a2 + a3;
    T e41;
    e41 = b2 + b3;
    div_QTW_QTW_QTW( a0, a1, e40, b0, b1, e41, c0, c1, c2 );
  }

  // div: 4-4-4
  template < typename T > INLINE void constexpr
  div_QQW_QQW_QQW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    td = b0 + b1 + b2 + b3;
    c3 = tn / td;
  }

  // div: 5-5-5
  template < typename T > INLINE void constexpr
  div_QPW_QPW_QPW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4;
    T a34;
    a34 = a3 + a4;
    T b34;
    b34 = b3 + b4;
    div_QQW_QQW_QQW(a0, a1, a2, a34, b0, b1, b2, b34, c0, c1, c2, c3);
    mul_QPW_QPW_QPW(c0, c1, c2, c3, fp_const<T>::zero(),
		     b0, b1, b2, b3, b4, t0, t1, t2, t3, t4);
    sub_QPW_QPW_QPW(a0, a1, a2, a3, a4, t0, t1, t2, t3, t4, t0, t1, t2, t3, t4);
    T tn, td;
    tn = t0 + t1 + t2 + t3 + t4;
    td = b0 + b1 + b2 + b3 + b4;
    c4 = tn / td;
  }

    // div: 5-2-5
 template < typename T > INLINE void constexpr
  div_QPW_PA_QPW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4, tn, td;  
    // [c1,c2,c3,c4] = QQW_div(a1,a2,a3,a4,b1,b2,b3,b4); 
    div_QQW_PA_QQW( a0, a1, a2, a3, b0, b1, c0, c1, c2, c3);
    td = b0 + b1;                // bunbo = b1 + b2 + b3 + b4 + b5;
    //  [t1,t2,t3,t4,t5] = Q5W_mul(c1,c2,c3,c4,0,b1,b2,b3,b4,b5);
    mul_QPW_PA_QPW( c0, c1, c2, c3, fp_const<T>::zero(),
		     b0, b1, t0, t1, t2, t3, t4);
    // [t1,t2,t3,t4,t5] = Q5W_sum(a1,a2,a3,a4,a5,-t1,-t2,-t3,-t4,-t5);
    add_QPW_QPW_QPW( a0, a1, a2, a3, a4, -t0, -t1, -t2, -t3, -t4,
		     t0, t1, t2, t3, t4);
    tn = t0 + t1 + t2 + t3 + t4; // bunshi = t1 + t2 + t3 + t4 + t5;
    c4 = tn / td;                // c5 = bunshi / bunbo;
  }

    // div: 5-3-5
 template < typename T > INLINE void constexpr
 div_QPW_QTW_QPW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4, tn, td;  
    // [c1,c2,c3,c4] = QQW_div(a1,a2,a3,a4,b1,b2,b3,b4); 
    div_QQW_QTW_QQW( a0, a1, a2, a3, b0, b1, b2, c0, c1, c2, c3);
    td = b0 + b1;                // bunbo = b1 + b2 + b3 + b4 + b5;
    //  [t1,t2,t3,t4,t5] = Q5W_mul(c1,c2,c3,c4,0,b1,b2,b3,b4,b5);
    mul_QPW_QTW_QPW( c0, c1, c2, c3, fp_const<T>::zero(),
		     b0, b1, b2, t0, t1, t2, t3, t4);
    // [t1,t2,t3,t4,t5] = Q5W_sum(a1,a2,a3,a4,a5,-t1,-t2,-t3,-t4,-t5);
    add_QPW_QPW_QPW( a0, a1, a2, a3, a4, -t0, -t1, -t2, -t3, -t4,
		     t0, t1, t2, t3, t4);
    tn = t0 + t1 + t2 + t3 + t4; // bunshi = t1 + t2 + t3 + t4 + t5;
    c4 = tn / td;                // c5 = bunshi / bunbo;
  }

      // div: 3-5-5
 template < typename T > INLINE void constexpr
 div_QTW_QPW_QPW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
 {
   T t0, t1, t2, t3, t4, tn, td;  
   div_QTW_QQW_QQW( a0, a1, a2, b0, b1, b2, b3, c0, c1, c2, c3);
    td = b0 + b1 + b2 + b3 + b4; // bunbo = b1 + b2 + b3 + b4 + b5;
    //  [t1,t2,t3,t4,t5] = Q5W_mul(c1,c2,c3,c4,0,b1,b2,b3,b4,b5);
    mul_QPW_QPW_QPW( c0, c1, c2, c3, fp_const<T>::zero(),
		     b0, b1, b2, b3, b4, t0, t1, t2, t3, t4);
    // [t1,t2,t3,t4,t5] = Q5W_sum(a1,a2,a3,a4,a5,-t1,-t2,-t3,-t4,-t5);
    add_QTW_QPW_QPW( a0, a1, a2, -t0, -t1, -t2, -t3, -t4,
		     t0, t1, t2, t3, t4);
    tn = t0 + t1 + t2 + t3 + t4; // bunshi = t1 + t2 + t3 + t4 + t5;
    c4 = tn / td;                // c5 = bunshi / bunbo;
   }
      // div: 5-4-5
 template < typename T > INLINE void constexpr
 div_QPW_QQW_QPW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4, tn, td;  
    div_QQW_QQW_QQW( a0, a1, a2, a3, b0, b1, b2, b3, c0, c1, c2, c3);
    td = b0 + b1 + b2 + b3; // bunbo = b1 + b2 + b3 + b4 + b5;
    //  [t1,t2,t3,t4,t5] = Q5W_mul(c1,c2,c3,c4,0,b1,b2,b3,b4,b5);
    mul_QPW_QQW_QPW( c0, c1, c2, c3, fp_const<T>::zero(),
		     b0, b1, b2, b3, t0, t1, t2, t3, t4);
    // [t1,t2,t3,t4,t5] = Q5W_sum(a1,a2,a3,a4,a5,-t1,-t2,-t3,-t4,-t5);
    add_QQW_QPW_QPW( a0, a1, a2, a3, -t0, -t1, -t2, -t3, -t4,
		     t0, t1, t2, t3, t4);
    tn = t0 + t1 + t2 + t3 + t4; // bunshi = t1 + t2 + t3 + t4 + t5;
    c4 = tn / td;                // c5 = bunshi / bunbo;
  }
      // div: 4-5-5
 template < typename T > INLINE void constexpr
 div_QQW_QPW_QPW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
   T t0, t1, t2, t3, t4, tn, td;  
   div_QQW_QQW_QQW( a0, a1, a2, a3, b0, b1, b2, b3, c0, c1, c2, c3);
    td = b0 + b1 + b2 + b3 + b4; // bunbo = b1 + b2 + b3 + b4 + b5;
    //  [t1,t2,t3,t4,t5] = Q5W_mul(c1,c2,c3,c4,0,b1,b2,b3,b4,b5);
    mul_QPW_QPW_QPW( c0, c1, c2, c3, fp_const<T>::zero(),
		     b0, b1, b2, b3, b4, t0, t1, t2, t3, t4);
    // [t1,t2,t3,t4,t5] = Q5W_sum(a1,a2,a3,a4,a5,-t1,-t2,-t3,-t4,-t5);
    add_QTW_QPW_QPW( a0, a1, a2, -t0, -t1, -t2, -t3, -t4,
		     t0, t1, t2, t3, t4);
    tn = t0 + t1 + t2 + t3 + t4; // bunshi = t1 + t2 + t3 + t4 + t5;
    c4 = tn / td;                // c5 = bunshi / bunbo;
  }
  
    // div: 2-5-5
  template < typename T > INLINE void constexpr
  div_PA_QPW_QPW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4, tn, td;

    // [c1,c2,c3,c4] = QQW_div(a1,a2,a3,a4,b1,b2,b3,b4); 
    div_PA_QQW_QQW( a0, a1, b0, b1, b2, b3, c0, c1, c2, c3);
    td = b0 + b1 + b2 + b3 + b4; // bunbo = b1 + b2 + b3 + b4 + b5;
    //  [t1,t2,t3,t4,t5] = Q5W_mul(c1,c2,c3,c4,0,b1,b2,b3,b4,b5);
    mul_QPW_QPW_QPW( c0, c1, c2, c3, fp_const<T>::zero(),
		     b0, b1, b2, b3, b4, t0, t1, t2, t3, t4);
    // [t1,t2,t3,t4,t5] = Q5W_sum(a1,a2,a3,a4,a5,-t1,-t2,-t3,-t4,-t5);
    add_PA_QPW_QPW( a0, a1, -t0, -t1, -t2, -t3, -t4,
		     t0, t1, t2, t3, t4);
    tn = t0 + t1 + t2 + t3 + t4; // bunshi = t1 + t2 + t3 + t4 + t5;
    c4 = tn / td;                // c5 = bunshi / bunbo;
  }

  // sqr: 1-1
  template < typename T > INLINE void constexpr
  sqr_SW_SW( T const a0, T &c0 ) NOEXCEPT
  {
    c0 = a0 * a0;
  }

  // sqr: 1-2
  template < typename T > INLINE void constexpr
  sqr_SW_PA( T const a0, T &c0, T &c1 ) NOEXCEPT
  {
    TwoProductFMA( a0, a0, c0, c1 );
  }

  // sqr: 1-3
  template < typename T > INLINE void constexpr
  sqr_SW_QTW( T const a0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    TwoProductFMA( a0, a0, c0, c1 );
    c2 = fp_const<T>::zero();
  }

  // sqr: 1-4
  template < typename T > INLINE void constexpr
  sqr_SW_QQW( T const a0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    TwoProductFMA( a0, a0, c0, c1 );
    c2 = fp_const<T>::zero();
    c3 = fp_const<T>::zero();
  }

  // sqr: 2-1
  template < typename T > INLINE void constexpr
  sqr_PA_SW( T const a0, T const a1, T &c0 ) NOEXCEPT
  {
    T t0;
    t0 = a0 + a1;
    c0 = t0 * t0;
  }

  // sqr: 2-2
  template < typename T > INLINE void constexpr
  sqr_PA_PA( T const a0, T const a1, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    c1 = std::fma ( t0, a1, c1 );
  }

  // sqr: 2-3
  template < typename T > INLINE void constexpr
  sqr_PA_QTW( T const a0, T const a1, T &c0, T &c1, T &c2 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  sqr_PA_QQW( T const a0, T const a1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  sqr_QTW_SW( T const a0, T const a1, T const a2, T &c0 ) NOEXCEPT
  {
    T t0;
    t0 = a0 + a1 + a2;
    c0 = t0 * t0;
  }

  // sqr: 3-2
  template < typename T > INLINE void constexpr
  sqr_QTW_PA( T const a0, T const a1, T const a2, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    t1 = a1 + a2;
    c1 = std::fma ( t0, t1, c1 );
  }

  // sqr: 3-3
  template < typename T > INLINE void constexpr
  sqr_QTW_QTW( T const a0, T const a1, T const a2, T &c0, T &c1, T &c2 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  sqr_QTW_QQW( T const a0, T const a1, T const a2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  sqr_QQW_SW( T const a0, T const a1, T const a2, T const a3, T &c0 ) NOEXCEPT
  {
    T t0;
    t0 = a0 + a1 + a2 + a3;
    c0 = t0 * t0;
  }

  // sqr: 4-2
  template < typename T > INLINE void constexpr
  sqr_QQW_PA( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    TwoProductFMA( a0, a0, c0, c1 );
    t0 = a0 + a0;
    t1 = a1 + a2 + a3;
    c1 = std::fma ( t0, t1, c1 );
  }

  // sqr: 4-3
  template < typename T > INLINE void constexpr
  sqr_QQW_QTW( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1, T &c2 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  sqr_QQW_QQW( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_SW_SW_DW( T const a0, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 1-1-3
  template < typename T > INLINE void constexpr
  add_SW_SW_TW( T const a0, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    FastTwoSum( c0, c1, c0, c1 );
    c2 = fp_const<T>::zero();
  }

  // add: 1-1-4
  template < typename T > INLINE void constexpr
  add_SW_SW_QW( T const a0, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    FastTwoSum( c0, c1, c0, c1 );
    c2 = fp_const<T>::zero();
    c3 = fp_const<T>::zero();
  }

  // add: 1-2-1
  template < typename T > INLINE void constexpr
  add_SW_DW_SW( T const a0, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, t0 );
    c0 = c0 + b1;
    c0 = c0 + t0;
  }

  // add: 1-2-2
  template < typename T > INLINE void constexpr
  add_SW_DW_DW( T const a0, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, t0 );
    c1 = c1 + t0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 1-2-3
  template < typename T > INLINE void constexpr
  add_SW_DW_TW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, c2 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 1-2-4
  template < typename T > INLINE void constexpr
  add_SW_DW_QW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    c3 = fp_const<T>::zero();
  }

  // add: 1-3-1
  template < typename T > INLINE void constexpr
  add_SW_TW_SW( T const a0, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    TwoSum( a0, b0, c0, t0 );
    t1 = b1 + b2;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 1-3-2
  template < typename T > INLINE void constexpr
  add_SW_TW_DW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, t0 );
    c1 = c1 + t0;
    c1 = c1 + b2;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 1-3-3
  template < typename T > INLINE void constexpr
  add_SW_TW_TW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, c2 );
    TwoSum( c2, b2, c2, t0 );
    FastTwoSum( c1, c2, c1, c2 );
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 1-3-4
  template < typename T > INLINE void constexpr
  add_SW_TW_QW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, c2 );
    TwoSum( c2, b2, c2, c3 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 1-4-1
  template < typename T > INLINE void constexpr
  add_SW_QW_SW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    TwoSum( a0, b0, c0, t0 );
    t1 = b1 + b2 + b3;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 1-4-2
  template < typename T > INLINE void constexpr
  add_SW_QW_DW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_SW_QW_TW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, c2 );
    TwoSum( c2, b2, c2, t0 );
    FastTwoSum( c1, c2, c1, c2 );
    c2 = c2 + b3;
    FastTwoSum( c1, c2, c1, c2 );
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 1-4-4
  template < typename T > INLINE void constexpr
  add_SW_QW_QW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, b1, c1, c2 );
    TwoSum( c2, b2, c2, c3 );
    TwoSum( c3, b3, c3, t0 );
    FastTwoSum( c2, c3, c2, c3 );
    c3 = c3 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-1-1
  template < typename T > INLINE void constexpr
  add_DW_SW_SW( T const a0, T const a1, T const b0, T &c0 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, t0 );
    c0 = c0 + a1;
    c0 = c0 + t0;
  }

  // add: 2-1-2
  template < typename T > INLINE void constexpr
  add_DW_SW_DW( T const a0, T const a1, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, t0 );
    c1 = c1 + t0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-1-3
  template < typename T > INLINE void constexpr
  add_DW_SW_TW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, c2 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-1-4
  template < typename T > INLINE void constexpr
  add_DW_SW_QW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    c3 = fp_const<T>::zero();
  }

  // add: 2-2-1
  template < typename T > INLINE void constexpr
  add_DW_DW_SW( T const a0, T const a1, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + b1;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 2-2-2
  template < typename T > INLINE void constexpr
  add_DW_DW_DW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, t1 );
    c1 = c1 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-2-3
  template < typename T > INLINE void constexpr
  add_DW_DW_TW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    FastTwoSum( c1, c2, c1, c2 );
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-2-4
  template < typename T > INLINE void constexpr
  add_DW_DW_QW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, c3, c2 );
    TwoSum( c1, c3, c1, c3 );
    TwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-3-1
  template < typename T > INLINE void constexpr
  add_DW_TW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + b1;
    t1 = t1 + b2;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 2-3-2
  template < typename T > INLINE void constexpr
  add_DW_TW_DW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_DW_TW_TW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-3-4
  template < typename T > INLINE void constexpr
  add_DW_TW_QW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, c3, c2 );
    TwoSum( c1, c3, c1, c3 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, b2, c2, t0 );
    TwoSum( c3, t0, c3, t0 );
    FastTwoSum( c2, c3, c2, c3 );
    c3 = c3 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-4-1
  template < typename T > INLINE void constexpr
  add_DW_QW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + b1;
    t1 = t1 + b2 + b3;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 2-4-2
  template < typename T > INLINE void constexpr
  add_DW_QW_DW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_DW_QW_TW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-4-4
  template < typename T > INLINE void constexpr
  add_DW_QW_QW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, c3, c2 );
    TwoSum( c1, c3, c1, c3 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, b2, c2, t0 );
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, b3, c3, t1 );
    FastTwoSum( c2, c3, c2, c3 );
    t2 = t0 + t1;
    c3 = c3 + t2;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 2-5-5
  template < typename T > INLINE void constexpr
  add_DW_PW_PW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, b2, c2, t1 );
    TwoSum( t0, t1, c3, t1 );
    TwoSum( c3, b3, c3, t2 );
    TwoSum( t1, t2, c4, t2 );
    TwoSum( c4, b4, c4, t3 );
    FastTwoSum( c3, c4, c3, c4 );
    t0 = t2 + t3;
    c4 = c4 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c3, c4, c3, c4 );

    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );

    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );

    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-1-1
  template < typename T > INLINE void constexpr
  add_TW_SW_SW( T const a0, T const a1, T const a2, T const b0, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + a2;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 3-1-2
  template < typename T > INLINE void constexpr
  add_TW_SW_DW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, t0 );
    c1 = c1 + t0;
    c1 = c1 + a2;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-1-3
  template < typename T > INLINE void constexpr
  add_TW_SW_TW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, c2 );
    TwoSum( c2, a2, c2, t0 );
    FastTwoSum( c1, c2, c1, c2 );
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-1-4
  template < typename T > INLINE void constexpr
  add_TW_SW_QW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, c2 );
    TwoSum( c2, a2, c2, c3 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-2-1
  template < typename T > INLINE void constexpr
  add_TW_DW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + b1;
    t1 = t1 + a2;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 3-2-2
  template < typename T > INLINE void constexpr
  add_TW_DW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_TW_DW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-2-4
  template < typename T > INLINE void constexpr
  add_TW_DW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, c3, c2 );
    TwoSum( c1, c3, c1, c3 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, a2, c2, t0 );
    TwoSum( c3, t0, c3, t0 );
    FastTwoSum( c2, c3, c2, c3 );
    c3 = c3 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-3-1
  template < typename T > INLINE void constexpr
  add_TW_TW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_TW_TW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_TW_TW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-3-4
  template < typename T > INLINE void constexpr
  add_TW_TW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_TW_QW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_TW_QW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_TW_QW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 3-4-4
  template < typename T > INLINE void constexpr
  add_TW_QW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_QW_SW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + a2 + a3;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 4-1-2
  template < typename T > INLINE void constexpr
  add_QW_SW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_QW_SW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, c2 );
    TwoSum( c2, a2, c2, t0 );
    FastTwoSum( c1, c2, c1, c2 );
    c2 = c2 + a3;
    FastTwoSum( c1, c2, c1, c2 );
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 4-1-4
  template < typename T > INLINE void constexpr
  add_QW_SW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( c1, a1, c1, c2 );
    TwoSum( c2, a2, c2, c3 );
    TwoSum( c3, a3, c3, t0 );
    FastTwoSum( c2, c3, c2, c3 );
    c3 = c3 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 4-2-1
  template < typename T > INLINE void constexpr
  add_QW_DW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + b1;
    t1 = t1 + a2 + a3;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 4-2-2
  template < typename T > INLINE void constexpr
  add_QW_DW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_QW_DW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 4-2-4
  template < typename T > INLINE void constexpr
  add_QW_DW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, c3, c2 );
    TwoSum( c1, c3, c1, c3 );
    TwoSum( c2, c3, c2, c3 );
    TwoSum( c2, a2, c2, t0 );
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, a3, c3, t1 );
    FastTwoSum( c2, c3, c2, c3 );
    t2 = t0 + t1;
    c3 = c3 + t2;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 4-3-1
  template < typename T > INLINE void constexpr
  add_QW_TW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_QW_TW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_QW_TW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 4-3-4
  template < typename T > INLINE void constexpr
  add_QW_TW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_QW_QW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_QW_QW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  add_QW_QW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // add: 4-4-4
  template < typename T > INLINE void constexpr
  add_QW_QW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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

  // add: 5-5-1
  template < typename T > INLINE void constexpr
  add_PW_PW_SW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4;
    TwoSum( a0, b0, c0, t0 );
    t1 = a1 + b1;
    t2 = a2 + b2;
    t3 = a3 + b3;
    t3 = a4 + b4;    
    t1 = t1 + t2 + t3 + t4;
    c0 = c0 + t1;
    c0 = c0 + t0;
  }

  // add: 5-5-5
  template < typename T > INLINE void constexpr
  add_PW_PW_PW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4;
    TwoSum( a0, b0, c0, c1 );
    TwoSum( a1, b1, t0, c2 );
    TwoSum( a2, b2, t1, c3 );
    TwoSum( a3, b3, t2, c4 );
    TwoSum( a4, b4, t3, t4 );
    
    
    TwoSum( c1, t0, c1, t0 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
        
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t1, c3, t1 );
    TwoSum( c3, t2, c3, t2 );

    TwoSum( c4, t0, c4, t0 );
    TwoSum( c4, t1, c4, t1 );
    TwoSum( c4, t2, c4, t2 );
    TwoSum( c4, t3, c4, t3 );    
        
    FastTwoSum( c3, c4, c3, c4 );
    t0 = t0 + t1 + t2 + t3 + t4;
    
    c4 = c4 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c3, c4, c3, c4 );    
        
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  } 
  // sub: 1-1-2
  template < typename T > INLINE void constexpr
  sub_SW_SW_DW( T const a0, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    add_SW_SW_DW( a0, -b0, c0, c1 );
  }

  // sub: 1-1-3
  template < typename T > INLINE void constexpr
  sub_SW_SW_TW( T const a0, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_SW_SW_TW( a0, -b0, c0, c1, c2 );
  }

  // sub: 1-1-4
  template < typename T > INLINE void constexpr
  sub_SW_SW_QW( T const a0, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_SW_SW_QW( a0, -b0, c0, c1, c2, c3 );
  }

  // sub: 1-2-1
  template < typename T > INLINE void constexpr
  sub_SW_DW_SW( T const a0, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    add_SW_DW_SW( a0, -b0, -b1, c0 );
  }

  // sub: 1-2-2
  template < typename T > INLINE void constexpr
  sub_SW_DW_DW( T const a0, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    add_SW_DW_DW( a0, -b0, -b1, c0, c1 );
  }

  // sub: 1-2-3
  template < typename T > INLINE void constexpr
  sub_SW_DW_TW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_SW_DW_TW( a0, -b0, -b1, c0, c1, c2 );
  }

  // sub: 1-2-4
  template < typename T > INLINE void constexpr
  sub_SW_DW_QW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_SW_DW_QW( a0, -b0, -b1, c0, c1, c2, c3 );
  }

  // sub: 1-3-1
  template < typename T > INLINE void constexpr
  sub_SW_TW_SW( T const a0, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    add_SW_TW_SW( a0, -b0, -b1, -b2, c0 );
  }

  // sub: 1-3-2
  template < typename T > INLINE void constexpr
  sub_SW_TW_DW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    add_SW_TW_DW( a0, -b0, -b1, -b2, c0, c1 );
  }

  // sub: 1-3-3
  template < typename T > INLINE void constexpr
  sub_SW_TW_TW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_SW_TW_TW( a0, -b0, -b1, -b2, c0, c1, c2 );
  }

  // sub: 1-3-4
  template < typename T > INLINE void constexpr
  sub_SW_TW_QW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_SW_TW_QW( a0, -b0, -b1, -b2, c0, c1, c2, c3 );
  }

  // sub: 1-4-1
  template < typename T > INLINE void constexpr
  sub_SW_QW_SW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    add_SW_QW_SW( a0, -b0, -b1, -b2, -b3, c0 );
  }

  // sub: 1-4-2
  template < typename T > INLINE void constexpr
  sub_SW_QW_DW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    add_SW_QW_DW( a0, -b0, -b1, -b2, -b3, c0, c1 );
  }

  // sub: 1-4-3
  template < typename T > INLINE void constexpr
  sub_SW_QW_TW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_SW_QW_TW( a0, -b0, -b1, -b2, -b3, c0, c1, c2 );
  }

  // sub: 1-4-4
  template < typename T > INLINE void constexpr
  sub_SW_QW_QW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_SW_QW_QW( a0, -b0, -b1, -b2, -b3, c0, c1, c2, c3 );
  }

  // sub: 2-1-1
  template < typename T > INLINE void constexpr
  sub_DW_SW_SW( T const a0, T const a1, T const b0, T &c0 ) NOEXCEPT
  {
    add_DW_SW_SW( a0, a1, -b0, c0 );
  }

  // sub: 2-1-2
  template < typename T > INLINE void constexpr
  sub_DW_SW_DW( T const a0, T const a1, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    add_DW_SW_DW( a0, a1, -b0, c0, c1 );
  }

  // sub: 2-1-3
  template < typename T > INLINE void constexpr
  sub_DW_SW_TW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_DW_SW_TW( a0, a1, -b0, c0, c1, c2 );
  }

  // sub: 2-1-4
  template < typename T > INLINE void constexpr
  sub_DW_SW_QW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_DW_SW_QW( a0, a1, -b0, c0, c1, c2, c3 );
  }

  // sub: 2-2-1
  template < typename T > INLINE void constexpr
  sub_DW_DW_SW( T const a0, T const a1, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    add_DW_DW_SW( a0, a1, -b0, -b1, c0 );
  }

  // sub: 2-2-2
  template < typename T > INLINE void constexpr
  sub_DW_DW_DW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    add_DW_DW_DW( a0, a1, -b0, -b1, c0, c1 );
  }

  // sub: 2-2-3
  template < typename T > INLINE void constexpr
  sub_DW_DW_TW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_DW_DW_TW( a0, a1, -b0, -b1, c0, c1, c2 );
  }

  // sub: 2-2-4
  template < typename T > INLINE void constexpr
  sub_DW_DW_QW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_DW_DW_QW( a0, a1, -b0, -b1, c0, c1, c2, c3 );
  }

  // sub: 2-3-1
  template < typename T > INLINE void constexpr
  sub_DW_TW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    add_DW_TW_SW( a0, a1, -b0, -b1, -b2, c0 );
  }

  // sub: 2-3-2
  template < typename T > INLINE void constexpr
  sub_DW_TW_DW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    add_DW_TW_DW( a0, a1, -b0, -b1, -b2, c0, c1 );
  }

  // sub: 2-3-3
  template < typename T > INLINE void constexpr
  sub_DW_TW_TW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_DW_TW_TW( a0, a1, -b0, -b1, -b2, c0, c1, c2 );
  }

  // sub: 2-3-4
  template < typename T > INLINE void constexpr
  sub_DW_TW_QW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_DW_TW_QW( a0, a1, -b0, -b1, -b2, c0, c1, c2, c3 );
  }

  // sub: 2-4-1
  template < typename T > INLINE void constexpr
  sub_DW_QW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    add_DW_QW_SW( a0, a1, -b0, -b1, -b2, -b3, c0 );
  }

  // sub: 2-4-2
  template < typename T > INLINE void constexpr
  sub_DW_QW_DW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    add_DW_QW_DW( a0, a1, -b0, -b1, -b2, -b3, c0, c1 );
  }

  // sub: 2-4-3
  template < typename T > INLINE void constexpr
  sub_DW_QW_TW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_DW_QW_TW( a0, a1, -b0, -b1, -b2, -b3, c0, c1, c2 );
  }

  // sub: 2-4-4
  template < typename T > INLINE void constexpr
  sub_DW_QW_QW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_DW_QW_QW( a0, a1, -b0, -b1, -b2, -b3, c0, c1, c2, c3 );
  }

  // sub: 3-1-1
  template < typename T > INLINE void constexpr
  sub_TW_SW_SW( T const a0, T const a1, T const a2, T const b0, T &c0 ) NOEXCEPT
  {
    add_TW_SW_SW( a0, a1, a2, -b0, c0 );
  }

  // sub: 3-1-2
  template < typename T > INLINE void constexpr
  sub_TW_SW_DW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    add_TW_SW_DW( a0, a1, a2, -b0, c0, c1 );
  }

  // sub: 3-1-3
  template < typename T > INLINE void constexpr
  sub_TW_SW_TW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_TW_SW_TW( a0, a1, a2, -b0, c0, c1, c2 );
  }

  // sub: 3-1-4
  template < typename T > INLINE void constexpr
  sub_TW_SW_QW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_TW_SW_QW( a0, a1, a2, -b0, c0, c1, c2, c3 );
  }

  // sub: 3-2-1
  template < typename T > INLINE void constexpr
  sub_TW_DW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    add_TW_DW_SW( a0, a1, a2, -b0, -b1, c0 );
  }

  // sub: 3-2-2
  template < typename T > INLINE void constexpr
  sub_TW_DW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    add_TW_DW_DW( a0, a1, a2, -b0, -b1, c0, c1 );
  }

  // sub: 3-2-3
  template < typename T > INLINE void constexpr
  sub_TW_DW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_TW_DW_TW( a0, a1, a2, -b0, -b1, c0, c1, c2 );
  }

  // sub: 3-2-4
  template < typename T > INLINE void constexpr
  sub_TW_DW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_TW_DW_QW( a0, a1, a2, -b0, -b1, c0, c1, c2, c3 );
  }

  // sub: 3-3-1
  template < typename T > INLINE void constexpr
  sub_TW_TW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    add_TW_TW_SW( a0, a1, a2, -b0, -b1, -b2, c0 );
  }

  // sub: 3-3-2
  template < typename T > INLINE void constexpr
  sub_TW_TW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    add_TW_TW_DW( a0, a1, a2, -b0, -b1, -b2, c0, c1 );
  }

  // sub: 3-3-3
  template < typename T > INLINE void constexpr
  sub_TW_TW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_TW_TW_TW( a0, a1, a2, -b0, -b1, -b2, c0, c1, c2 );
  }

  // sub: 3-3-4
  template < typename T > INLINE void constexpr
  sub_TW_TW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_TW_TW_QW( a0, a1, a2, -b0, -b1, -b2, c0, c1, c2, c3 );
  }

  // sub: 3-4-1
  template < typename T > INLINE void constexpr
  sub_TW_QW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    add_TW_QW_SW( a0, a1, a2, -b0, -b1, -b2, -b3, c0 );
  }

  // sub: 3-4-2
  template < typename T > INLINE void constexpr
  sub_TW_QW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    add_TW_QW_DW( a0, a1, a2, -b0, -b1, -b2, -b3, c0, c1 );
  }

  // sub: 3-4-3
  template < typename T > INLINE void constexpr
  sub_TW_QW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_TW_QW_TW( a0, a1, a2, -b0, -b1, -b2, -b3, c0, c1, c2 );
  }

  // sub: 3-4-4
  template < typename T > INLINE void constexpr
  sub_TW_QW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_TW_QW_QW( a0, a1, a2, -b0, -b1, -b2, -b3, c0, c1, c2, c3 );
  }

  // sub: 4-1-1
  template < typename T > INLINE void constexpr
  sub_QW_SW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0 ) NOEXCEPT
  {
    add_QW_SW_SW( a0, a1, a2, a3, -b0, c0 );
  }

  // sub: 4-1-2
  template < typename T > INLINE void constexpr
  sub_QW_SW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    add_QW_SW_DW( a0, a1, a2, a3, -b0, c0, c1 );
  }

  // sub: 4-1-3
  template < typename T > INLINE void constexpr
  sub_QW_SW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_QW_SW_TW( a0, a1, a2, a3, -b0, c0, c1, c2 );
  }

  // sub: 4-1-4
  template < typename T > INLINE void constexpr
  sub_QW_SW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_QW_SW_QW( a0, a1, a2, a3, -b0, c0, c1, c2, c3 );
  }

  // sub: 4-2-1
  template < typename T > INLINE void constexpr
  sub_QW_DW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    add_QW_DW_SW( a0, a1, a2, a3, -b0, -b1, c0 );
  }

  // sub: 4-2-2
  template < typename T > INLINE void constexpr
  sub_QW_DW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    add_QW_DW_DW( a0, a1, a2, a3, -b0, -b1, c0, c1 );
  }

  // sub: 4-2-3
  template < typename T > INLINE void constexpr
  sub_QW_DW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_QW_DW_TW( a0, a1, a2, a3, -b0, -b1, c0, c1, c2 );
  }

  // sub: 4-2-4
  template < typename T > INLINE void constexpr
  sub_QW_DW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_QW_DW_QW( a0, a1, a2, a3, -b0, -b1, c0, c1, c2, c3 );
  }

  // sub: 4-3-1
  template < typename T > INLINE void constexpr
  sub_QW_TW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    add_QW_TW_SW( a0, a1, a2, a3, -b0, -b1, -b2, c0 );
  }

  // sub: 4-3-2
  template < typename T > INLINE void constexpr
  sub_QW_TW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    add_QW_TW_DW( a0, a1, a2, a3, -b0, -b1, -b2, c0, c1 );
  }

  // sub: 4-3-3
  template < typename T > INLINE void constexpr
  sub_QW_TW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_QW_TW_TW( a0, a1, a2, a3, -b0, -b1, -b2, c0, c1, c2 );
  }

  // sub: 4-3-4
  template < typename T > INLINE void constexpr
  sub_QW_TW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_QW_TW_QW( a0, a1, a2, a3, -b0, -b1, -b2, c0, c1, c2, c3 );
  }

  // sub: 4-4-1
  template < typename T > INLINE void constexpr
  sub_QW_QW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    add_QW_QW_SW( a0, a1, a2, a3, -b0, -b1, -b2, -b3, c0 );
  }

  // sub: 4-4-2
  template < typename T > INLINE void constexpr
  sub_QW_QW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    add_QW_QW_DW( a0, a1, a2, a3, -b0, -b1, -b2, -b3, c0, c1 );
  }

  // sub: 4-4-3
  template < typename T > INLINE void constexpr
  sub_QW_QW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    add_QW_QW_TW( a0, a1, a2, a3, -b0, -b1, -b2, -b3, c0, c1, c2 );
  }

  // sub: 4-4-4
  template < typename T > INLINE void constexpr
  sub_QW_QW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    add_QW_QW_QW( a0, a1, a2, a3, -b0, -b1, -b2, -b3, c0, c1, c2, c3 );
  }


  // sub: 5-5-1
  template < typename T > INLINE void constexpr
  sub_PW_PW_SW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0 ) NOEXCEPT
  {
    add_PW_PW_SW( a0, a1, a2, a3, a4, -b0, -b1, -b2, -b3, -b4, c0 );
  }

    // sub: 2-5-5
  template < typename T > INLINE void constexpr
  sub_DW_PW_PW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
    add_DW_PW_PW( a0, a1, -b0, -b1, -b2, -b3, -b4, c0, c1, c2, c3, c4 );
  }


  // sub: 5-2-5
  template < typename T > INLINE void constexpr
  sub_PW_DW_PW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
    add_DW_PW_PW( -b0, -b1, a0, a1, a2, a3, a4, c0, c1, c2, c3, c4);
  }

  // sub: 5-5-5
  template < typename T > INLINE void constexpr
  sub_PW_PW_PW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
    add_PW_PW_PW( a0, a1, a2, a3, a4, -b0, -b1, -b2, -b3, -b4, c0, c1, c2, c3, c4 );
  }
  
  // mul: 1-1-2
  template < typename T > INLINE void constexpr
  mul_SW_SW_DW( T const a0, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    TwoProductFMA( a0, b0, c0, c1 );
  }

  // mul: 1-1-3
  template < typename T > INLINE void constexpr
  mul_SW_SW_TW( T const a0, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    TwoProductFMA( a0, b0, c0, c1 );
    c2 = fp_const<T>::zero();
  }

  // mul: 1-1-4
  template < typename T > INLINE void constexpr
  mul_SW_SW_QW( T const a0, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    TwoProductFMA( a0, b0, c0, c1 );
    c2 = fp_const<T>::zero();
    c3 = fp_const<T>::zero();
  }

  // mul: 1-2-1
  template < typename T > INLINE void constexpr
  mul_SW_DW_SW( T const a0, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    c0 = a0 * b0;
    c0 = std::fma ( a0, b1, c0 );
  }

  // mul: 1-2-2
  template < typename T > INLINE void constexpr
  mul_SW_DW_DW( T const a0, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_SW_DW_TW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 1-2-4
  template < typename T > INLINE void constexpr
  mul_SW_DW_QW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    c2 = t0 + c2;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    c3 = fp_const<T>::zero();
  }

  // mul: 1-3-1
  template < typename T > INLINE void constexpr
  mul_SW_TW_SW( T const a0, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T t0;
    c0 = a0 * b0;
    t0 = b1 + b2;
    c0 = std::fma ( a0, t0, c0 );
  }

  // mul: 1-3-2
  template < typename T > INLINE void constexpr
  mul_SW_TW_DW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_SW_TW_TW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
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
  template < typename T > INLINE void constexpr
  mul_SW_TW_QW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_SW_QW_SW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T t0;
    c0 = a0 * b0;
    t0 = b1 + b2 + b3;
    c0 = std::fma ( a0, t0, c0 );
  }

  // mul: 1-4-2
  template < typename T > INLINE void constexpr
  mul_SW_QW_DW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_SW_QW_TW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
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
  template < typename T > INLINE void constexpr
  mul_SW_QW_QW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_DW_SW_SW( T const a0, T const a1, T const b0, T &c0 ) NOEXCEPT
  {
    c0 = a0 * b0;
    c0 = std::fma ( b0, a1, c0 );
  }

  // mul: 2-1-2
  template < typename T > INLINE void constexpr
  mul_DW_SW_DW( T const a0, T const a1, T const b0, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_DW_SW_TW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 2-1-4
  template < typename T > INLINE void constexpr
  mul_DW_SW_QW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    c2 = t0 + c2;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    c3 = fp_const<T>::zero();
  }

  // mul: 2-2-1
  template < typename T > INLINE void constexpr
  mul_DW_DW_SW( T const a0, T const a1, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    c0 = a0 * b0;
    c0 = std::fma ( a0, b1, c0 );
    c0 = std::fma ( b0, a1, c0 );
    c0 = std::fma ( a1, b1, c0 );
  }

  // mul: 2-2-2
  template < typename T > INLINE void constexpr
  mul_DW_DW_DW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_DW_DW_TW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    TwoProductFMA( a1, b1, t3, t4 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t3, c2, t3 );
    t0 = t0 + t1 + t2 + t3 + t4;
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 2-2-4
  template < typename T > INLINE void constexpr
  mul_DW_DW_QW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_DW_TW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T t0;
    c0 = a0 * b0;
    t0 = b1 + b2;
    c0 = std::fma ( a0, t0, c0 );
    c0 = std::fma ( b0, a1, c0 );
    c0 = std::fma ( a1, t0, c0 );
  }

  // mul: 2-3-2
  template < typename T > INLINE void constexpr
  mul_DW_TW_DW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_DW_TW_TW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4, t5, t6;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    TwoProductFMA( a0, b2, t3, t4 );
    TwoProductFMA( a1, b1, t5, t6 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
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
  template < typename T > INLINE void constexpr
  mul_DW_TW_QW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_DW_QW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T t0;
    c0 = a0 * b0;
    t0 = b1 + b2 + b3;
    c0 = std::fma ( a0, t0, c0 );
    c0 = std::fma ( b0, a1, c0 );
    c0 = std::fma ( a1, t0, c0 );
  }

  // mul: 2-4-2
  template < typename T > INLINE void constexpr
  mul_DW_QW_DW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_DW_QW_TW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4, t5, t6;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    TwoProductFMA( a0, b2, t3, t4 );
    TwoProductFMA( a1, b1, t5, t6 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
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
  template < typename T > INLINE void constexpr
  mul_DW_QW_QW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_TW_SW_SW( T const a0, T const a1, T const a2, T const b0, T &c0 ) NOEXCEPT
  {
    T t0;
    c0 = a0 * b0;
    t0 = a1 + a2;
    c0 = std::fma ( b0, t0, c0 );
  }

  // mul: 3-1-2
  template < typename T > INLINE void constexpr
  mul_TW_SW_DW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_TW_SW_TW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    TwoProductFMA( a2, b0, t1, t2 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    t3 = t0 + t1 + t2;
    c2 = c2 + t3;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 3-1-4
  template < typename T > INLINE void constexpr
  mul_TW_SW_QW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a2, b0, c3, t1 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
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

  // mul: 3-2-1
  template < typename T > INLINE void constexpr
  mul_TW_DW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T t0;
    c0 = a0 * b0;
    t0 = a1 + a2;
    c0 = std::fma ( a0, b1, c0 );
    c0 = std::fma ( b0, t0, c0 );
    c0 = std::fma ( t0, b1, c0 );
  }

  // mul: 3-2-2
  template < typename T > INLINE void constexpr
  mul_TW_DW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_TW_DW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4, t5, t6;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    TwoProductFMA( a2, b0, t3, t4 );
    TwoProductFMA( a1, b1, t5, t6 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
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
  template < typename T > INLINE void constexpr
  mul_TW_DW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_TW_TW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_TW_TW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
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
    t0 = t0 * fp_const<T>::nhalf();
    t1 = std::fma ( a0, b2, t0 );
    t2 = std::fma ( b0, a2, t0 );
    t1 = t1 + t2;
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 3-3-3
  template < typename T > INLINE void constexpr
  mul_TW_TW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4, t5, t6, t7, t8;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    TwoProductFMA( a0, b2, t3, t4 );
    TwoProductFMA( a2, b0, t5, t6 );
    TwoProductFMA( a1, b1, t7, t8 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t3, c2, t3 );
    TwoSum( c2, t5, c2, t5 );
    TwoSum( c2, t7, c2, t7 );
    t0 = t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8;
    t0 = t0 * fp_const<T>::nhalf();
    t1 = std::fma ( a1, b2, t0 );
    t2 = std::fma ( a2, b1, t0 );
    t0 = t1 + t2;
    c2 = c2 + t0;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 3-3-4
  template < typename T > INLINE void constexpr
  mul_TW_TW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_TW_QW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_TW_QW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_TW_QW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4, t5, t6, t7, t8;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    TwoProductFMA( a0, b2, t3, t4 );
    TwoProductFMA( a2, b0, t5, t6 );
    TwoProductFMA( a1, b1, t7, t8 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
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
  template < typename T > INLINE void constexpr
  mul_TW_QW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_QW_SW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0 ) NOEXCEPT
  {
    T t0;
    c0 = a0 * b0;
    t0 = a1 + a2 + a3;
    c0 = std::fma ( b0, t0, c0 );
  }

  // mul: 4-1-2
  template < typename T > INLINE void constexpr
  mul_QW_SW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_QW_SW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    TwoProductFMA( a2, b0, t1, t2 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    t3 = t0 + t1 + t2;
    t3 = std::fma ( a3, b0, t3 );
    c2 = c2 + t3;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 4-1-4
  template < typename T > INLINE void constexpr
  mul_QW_SW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a1, b0, c2, t0 );
    TwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
    TwoProductFMA( a2, b0, c3, t1 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    TwoSum( c3, t0, c3, t0 );
    TwoSum( c3, t1, c3, t1 );
    t2 = t0 + t1;
    TwoProductFMA( a3, b0, t1, t3 );
    TwoSum( c3, t1, c3, t0 );
    t4 = t2 + t0 + t3;
    c3 = c3 + t4;
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 4-2-1
  template < typename T > INLINE void constexpr
  mul_QW_DW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T t0;
    c0 = a0 * b0;
    t0 = a1 + a2 + a3;
    c0 = std::fma ( a0, b1, c0 );
    c0 = std::fma ( b0, t0, c0 );
    c0 = std::fma ( t0, b1, c0 );
  }

  // mul: 4-2-2
  template < typename T > INLINE void constexpr
  mul_QW_DW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_QW_DW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4, t5, t6;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    TwoProductFMA( a2, b0, t3, t4 );
    TwoProductFMA( a1, b1, t5, t6 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
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
  template < typename T > INLINE void constexpr
  mul_QW_DW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_QW_TW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_QW_TW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_QW_TW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4, t5, t6, t7, t8;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    TwoProductFMA( a0, b2, t3, t4 );
    TwoProductFMA( a2, b0, t5, t6 );
    TwoProductFMA( a1, b1, t7, t8 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
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
  template < typename T > INLINE void constexpr
  mul_QW_TW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_QW_QW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  mul_QW_QW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
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
    t0 = t0 * fp_const<T>::nhalf();
    t1 = std::fma ( a0, t5, t0 );
    t2 = std::fma ( b0, t4, t0 );
    t1 = t1 + t2;
    c1 = c1 + t1;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // mul: 4-4-3
  template < typename T > INLINE void constexpr
  mul_QW_QW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4, t5, t6, t7, t8;
    TwoProductFMA( a0, b0, c0, c1 );
    TwoProductFMA( a0, b1, c2, t0 );
    TwoProductFMA( a1, b0, t1, t2 );
    TwoSum( c1, c2, c1, c2 );
    TwoSum( c1, t1, c1, t1 );
    TwoProductFMA( a0, b2, t3, t4 );
    TwoProductFMA( a2, b0, t5, t6 );
    TwoProductFMA( a1, b1, t7, t8 );
    TwoSum( c2, t0, c2, t0 );
    TwoSum( c2, t1, c2, t1 );
    TwoSum( c2, t2, c2, t2 );
    TwoSum( c2, t3, c2, t3 );
    TwoSum( c2, t5, c2, t5 );
    TwoSum( c2, t7, c2, t7 );
    t0 = t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8;
    t0 = t0 * fp_const<T>::nhalf();
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
  template < typename T > INLINE void constexpr
  mul_QW_QW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    t0 = t9 * fp_const<T>::nhalf();
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


    // mul: 2-1-5
  template < typename T > INLINE void constexpr
  mul_DW_SW_PW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
#if 0
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
    FastTwoSum( c0, c1, c0,< c1 );
#else
    fprintf(stderr, "%s %d : div_QW_DW_QW() not implemented\n", __FILE__, __LINE__);
#endif
  }

    // mul: 5-1-5
 template < typename T > INLINE void constexpr
    mul_PW_SW_PW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4, t5, t6, t7;
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
    FastTwoSum( c3, c4, c3, c4 );    

    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c2, c3, c2, c3 );
    
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c1, c2, c1, c2 );
    
    FastTwoSum( c0, c1, c0, c1 );    
  }
  // mul: 5-5-1
template < typename T > INLINE void constexpr
  mul_PW_PW_SW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    c0 = a0 * b0;
    t0 = a1 + a2 + a3 + a4;
    t1 = b1 + b2 + b3 + b4;
    c0 = std::fma ( a0, t1, c0 );
    c0 = std::fma ( b0, t0, c0 );
    c0 = std::fma ( t0, t1, c0 );
  }
  
  // div: 1-1-2
  template < typename T > INLINE void constexpr
  div_SW_SW_DW( T const a0, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    T r = a0;
    c0 = r / b0;
    r = std::fma( -b0, c0, r );
    c1 = r / b0;
  }

  // div: 1-1-3
  template < typename T > INLINE void constexpr
  div_SW_SW_TW( T const a0, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T r = a0;
    c0 = r / b0;
    r = std::fma( -b0, c0, r );
    c1 = r / b0;
    r = std::fma( -b0, c1, r );
    c2 = r / b0;
  }

  // div: 1-1-4
  template < typename T > INLINE void constexpr
  div_SW_SW_QW( T const a0, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  div_SW_DW_SW( T const a0, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    T r0, r1;
    c0 = a0 / b0;
    mul_DW_SW_DW( b0, b1, c0, t0, t1 );
    sub_SW_DW_SW( a0, t0, t1, r0 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 1-2-2
  template < typename T > INLINE void constexpr
  div_SW_DW_DW( T const a0, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    T r0, r1;
    c0 = a0 / b0;
    mul_DW_SW_DW( b0, b1, c0, t0, t1 );
    sub_SW_DW_DW( a0, t0, t1, r0, r1 );
    c1 = r0 / b0;
    mul_DW_SW_DW( b0, b1, c1, t0, t1 );
    sub_DW_DW_SW( r0, r1, t0, t1, r0 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 1-2-3
  template < typename T > INLINE void constexpr
  div_SW_DW_TW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    sub_TW_TW_SW( r0, r1, r2, t0, t1, t2, r0 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 1-2-4
  template < typename T > INLINE void constexpr
  div_SW_DW_QW( T const a0, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 1-3-1
  template < typename T > INLINE void constexpr
  div_SW_TW_SW( T const a0, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c0, t0, t1, t2 );
    sub_SW_TW_SW( a0, t0, t1, t2, r0 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 1-3-2
  template < typename T > INLINE void constexpr
  div_SW_TW_DW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c0, t0, t1, t2 );
    sub_SW_TW_TW( a0, t0, t1, t2, r0, r1, r2 );
    c1 = r0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c1, t0, t1, t2 );
    sub_TW_TW_SW( r0, r1, r2, t0, t1, t2, r0 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 1-3-3
  template < typename T > INLINE void constexpr
  div_SW_TW_TW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    sub_TW_TW_SW( r0, r1, r2, t0, t1, t2, r0 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 1-3-4
  template < typename T > INLINE void constexpr
  div_SW_TW_QW( T const a0, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 1-4-1
  template < typename T > INLINE void constexpr
  div_SW_QW_SW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_SW_QW_SW( a0, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 1-4-2
  template < typename T > INLINE void constexpr
  div_SW_QW_DW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_SW_QW_QW( a0, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c1, t0, t1, t2, t3 );
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 1-4-3
  template < typename T > INLINE void constexpr
  div_SW_QW_TW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 1-4-4
  template < typename T > INLINE void constexpr
  div_SW_QW_QW( T const a0, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-1-1
  template < typename T > INLINE void constexpr
  div_DW_SW_SW( T const a0, T const a1, T const b0, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    T r0, r1;
    c0 = a0 / b0;
    mul_SW_SW_DW( b0, c0, t0, t1 );
    sub_DW_DW_SW( a0, a1, t0, t1, r0 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 2-1-2
  template < typename T > INLINE void constexpr
  div_DW_SW_DW( T const a0, T const a1, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1;
    T r0, r1;
    c0 = a0 / b0;
    mul_SW_SW_DW( b0, c0, t0, t1 );
    sub_DW_DW_DW( a0, a1, t0, t1, r0, r1 );
    c1 = r0 / b0;
    mul_SW_SW_DW( b0, c1, t0, t1 );
    sub_DW_DW_SW( r0, r1, t0, t1, r0 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-1-3
  template < typename T > INLINE void constexpr
  div_DW_SW_TW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    sub_TW_TW_SW( r0, r1, r2, t0, t1, t2, r0 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-1-4
  template < typename T > INLINE void constexpr
  div_DW_SW_QW( T const a0, T const a1, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-2-1
  template < typename T > INLINE void constexpr
  div_DW_DW_SW( T const a0, T const a1, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T t0, t1, s0, s1;
    c0 = a0 / b0;
    mul_DW_SW_DW( b0, b1, c0, t0, t1 );
    TwoSum( a0, -t0, s0, s1 );
    s1 = s1 - t1 + a1;
    c0 = c0 + (s0 + s1) / b0;
  }

  // div: 2-2-2
  template < typename T > INLINE void constexpr
  div_DW_DW_DW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
#if 1
    // Ozaki's division scheme is described here.
    T r0, r1, q0, q1, s0, s1;
    q0 = a0 / b0;
    TwoProductFMA( b0, q0, r0, r1 );
    r1 = std::fma( b1, q0, r1 );
    TwoSum( a0, -r0, s0, s1 );
    s1 = ( s1 - r1 ) + a1;
    q1 = ( s0 + s1 ) / b0;
    FastTwoSum( q0, q1, c0, c1 );
#else
    // Meanwhile, dd-compatible one is commented out.
    T t0, t1;
    T r0, r1;
    c0 = a0 / b0;
    mul_DW_SW_DW( b0, b1, c0, t0, t1 );
    sub_DW_DW_DW( a0, a1, t0, t1, r0, r1 );
    c1 = r0 / b0;
    mul_DW_SW_DW( b0, b1, c1, t0, t1 );
    sub_DW_DW_SW( r0, r1, t0, t1, r0 );
    r0 = r0 / b0;
    FastTwoSum( c0, c1, c0, c1 );
    add_DW_SW_DW( c0, c1, r0, c0, c1 );
#endif
  }

  // div: 2-2-3
  template < typename T > INLINE void constexpr
  div_DW_DW_TW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    sub_TW_TW_SW( r0, r1, r2, t0, t1, t2, r0 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-2-4
  template < typename T > INLINE void constexpr
  div_DW_DW_QW( T const a0, T const a1, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-3-1
  template < typename T > INLINE void constexpr
  div_DW_TW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c0, t0, t1, t2 );
    sub_DW_TW_SW( a0, a1, t0, t1, t2, r0 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 2-3-2
  template < typename T > INLINE void constexpr
  div_DW_TW_DW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c0, t0, t1, t2 );
    sub_DW_TW_TW( a0, a1, t0, t1, t2, r0, r1, r2 );
    c1 = r0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c1, t0, t1, t2 );
    sub_TW_TW_SW( r0, r1, r2, t0, t1, t2, r0 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-3-3
  template < typename T > INLINE void constexpr
  div_DW_TW_TW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    sub_TW_TW_SW( r0, r1, r2, t0, t1, t2, r0 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-3-4
  template < typename T > INLINE void constexpr
  div_DW_TW_QW( T const a0, T const a1, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-4-1
  template < typename T > INLINE void constexpr
  div_DW_QW_SW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_DW_QW_SW( a0, a1, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 2-4-2
  template < typename T > INLINE void constexpr
  div_DW_QW_DW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_DW_QW_QW( a0, a1, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c1, t0, t1, t2, t3 );
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-4-3
  template < typename T > INLINE void constexpr
  div_DW_QW_TW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 2-4-4
  template < typename T > INLINE void constexpr
  div_DW_QW_QW( T const a0, T const a1, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-1-1
  template < typename T > INLINE void constexpr
  div_TW_SW_SW( T const a0, T const a1, T const a2, T const b0, T &c0 ) NOEXCEPT
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_SW_SW_TW( b0, c0, t0, t1, t2 );
    sub_TW_TW_SW( a0, a1, a2, t0, t1, t2, r0 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 3-1-2
  template < typename T > INLINE void constexpr
  div_TW_SW_DW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_SW_SW_TW( b0, c0, t0, t1, t2 );
    sub_TW_TW_TW( a0, a1, a2, t0, t1, t2, r0, r1, r2 );
    c1 = r0 / b0;
    mul_SW_SW_TW( b0, c1, t0, t1, t2 );
    sub_TW_TW_SW( r0, r1, r2, t0, t1, t2, r0 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-1-3
  template < typename T > INLINE void constexpr
  div_TW_SW_TW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    sub_TW_TW_SW( r0, r1, r2, t0, t1, t2, r0 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-1-4
  template < typename T > INLINE void constexpr
  div_TW_SW_QW( T const a0, T const a1, T const a2, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-2-1
  template < typename T > INLINE void constexpr
  div_TW_DW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_DW_SW_TW( b0, b1, c0, t0, t1, t2 );
    sub_TW_TW_SW( a0, a1, a2, t0, t1, t2, r0 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 3-2-2
  template < typename T > INLINE void constexpr
  div_TW_DW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_DW_SW_TW( b0, b1, c0, t0, t1, t2 );
    sub_TW_TW_TW( a0, a1, a2, t0, t1, t2, r0, r1, r2 );
    c1 = r0 / b0;
    mul_DW_SW_TW( b0, b1, c1, t0, t1, t2 );
    sub_TW_TW_SW( r0, r1, r2, t0, t1, t2, r0 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-2-3
  template < typename T > INLINE void constexpr
  div_TW_DW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    sub_TW_TW_SW( r0, r1, r2, t0, t1, t2, r0 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-2-4
  template < typename T > INLINE void constexpr
  div_TW_DW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-3-1
  template < typename T > INLINE void constexpr
  div_TW_TW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c0, t0, t1, t2 );
    sub_TW_TW_SW( a0, a1, a2, t0, t1, t2, r0 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 3-3-2
  template < typename T > INLINE void constexpr
  div_TW_TW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1, t2;
    T r0, r1, r2;
    c0 = a0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c0, t0, t1, t2 );
    sub_TW_TW_TW( a0, a1, a2, t0, t1, t2, r0, r1, r2 );
    c1 = r0 / b0;
    mul_TW_SW_TW( b0, b1, b2, c1, t0, t1, t2 );
    sub_TW_TW_SW( r0, r1, r2, t0, t1, t2, r0 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-3-3
  template < typename T > INLINE void constexpr
  div_TW_TW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    sub_TW_TW_SW( r0, r1, r2, t0, t1, t2, r0 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-3-4
  template < typename T > INLINE void constexpr
  div_TW_TW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-4-1
  template < typename T > INLINE void constexpr
  div_TW_QW_SW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_TW_QW_SW( a0, a1, a2, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 3-4-2
  template < typename T > INLINE void constexpr
  div_TW_QW_DW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_TW_QW_QW( a0, a1, a2, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c1, t0, t1, t2, t3 );
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-4-3
  template < typename T > INLINE void constexpr
  div_TW_QW_TW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 3-4-4
  template < typename T > INLINE void constexpr
  div_TW_QW_QW( T const a0, T const a1, T const a2, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-1-1
  template < typename T > INLINE void constexpr
  div_QW_SW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_SW_SW_QW( b0, c0, t0, t1, t2, t3 );
    sub_QW_QW_SW( a0, a1, a2, a3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 4-1-2
  template < typename T > INLINE void constexpr
  div_QW_SW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_SW_SW_QW( b0, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_SW_SW_QW( b0, c1, t0, t1, t2, t3 );
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-1-3
  template < typename T > INLINE void constexpr
  div_QW_SW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-1-4
  template < typename T > INLINE void constexpr
  div_QW_SW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-2-1
  template < typename T > INLINE void constexpr
  div_QW_DW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_DW_SW_QW( b0, b1, c0, t0, t1, t2, t3 );
    sub_QW_QW_SW( a0, a1, a2, a3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 4-2-2
  template < typename T > INLINE void constexpr
  div_QW_DW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_DW_SW_QW( b0, b1, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_DW_SW_QW( b0, b1, c1, t0, t1, t2, t3 );
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-2-3
  template < typename T > INLINE void constexpr
  div_QW_DW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-2-4
  template < typename T > INLINE void constexpr
  div_QW_DW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-3-1
  template < typename T > INLINE void constexpr
  div_QW_TW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c0, t0, t1, t2, t3 );
    sub_QW_QW_SW( a0, a1, a2, a3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 4-3-2
  template < typename T > INLINE void constexpr
  div_QW_TW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_TW_SW_QW( b0, b1, b2, c1, t0, t1, t2, t3 );
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-3-3
  template < typename T > INLINE void constexpr
  div_QW_TW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-3-4
  template < typename T > INLINE void constexpr
  div_QW_TW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-4-1
  template < typename T > INLINE void constexpr
  div_QW_QW_SW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_QW_QW_SW( a0, a1, a2, a3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c0 = c0 + r0;
  }

  // div: 4-4-2
  template < typename T > INLINE void constexpr
  div_QW_QW_DW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c0, t0, t1, t2, t3 );
    sub_QW_QW_QW( a0, a1, a2, a3, t0, t1, t2, t3, r0, r1, r2, r3 );
    c1 = r0 / b0;
    mul_QW_SW_QW( b0, b1, b2, b3, c1, t0, t1, t2, t3 );
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c1 = c1 + r0;
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-4-3
  template < typename T > INLINE void constexpr
  div_QW_QW_TW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c2 = c2 + r0;
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 4-4-4
  template < typename T > INLINE void constexpr
  div_QW_QW_QW( T const a0, T const a1, T const a2, T const a3, T const b0, T const b1, T const b2, T const b3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
    sub_QW_QW_SW( r0, r1, r2, r3, t0, t1, t2, t3, r0 );
    r0 = r0 / b0;
    c3 = c3 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // div: 5-2-5
  template < typename T > INLINE void constexpr
  div_PW_DW_PW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T &c0, T &c1, T &c2, T &c3, T &c4) NOEXCEPT
  {
#if 0
    T t0, t1, t2, t3;
    T r0, r1, r2, r3;
    c0 = a0 / b0;
    mul_DW_SW_PW( b0, b1, c0, t0, t1, t2, t3, t4 );
    sub_PW_PW_PW( a0, a1, a2, a3, a4, t0, t1, t2, t3, t4, r0, r1, r2, r3, r4 );
    c1 = r0 / b0;
    mul_DW_SW_PW( b0, b1, c1, t0, t1, t2, t3, t4 );
    sub_PW_PW_PW( r0, r1, r2, r3, r4, t0, t1, t2, t3, t4, r0, r1, r2, r3, r4 );
    c2 = r0 / b0;
    mul_DW_SW_PW( b0, b1, c2, t0, t1, t2, t3, t4 );
    sub_PW_PW_PW( r0, r1, r2, r3, r4, t0, t1, t2, t3, t4, r0, r1, r2, r3, r4 );
    c3 = r0 / b0;
    mul_DW_SW_PW( b0, b1, c3, t0, t1, t2, t3, t4);
    sub_PW_PW_SW( r0, r1, r2, r3, r4, t0, t1, t2, t3, t4, r0 );
    c4 = r0 / b0;
    mul_DW_SW_PW( b0, b1, c4, t0, t1, t2, t3, t4);
    sub_PW_PW_SW( r0, r1, r2, r3, r4, t0, t1, t2, t3, t4, r0 );
    r0 = r0 / b0;    
    c4 = c4 + r0;
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
#else
    fprintf(stderr, "%s %d : div_QW_DW_QW() not implemented\n", __FILE__, __LINE__);
#endif
  }  
  // div: 5-5-5
  template < typename T > INLINE void constexpr
  div_PW_PW_PW( T const a0, T const a1, T const a2, T const a3, T const a4, T const b0, T const b1, T const b2, T const b3, T const b4, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4;
    T r0, r1, r2, r3, r4;
    c0 = a0 / b0;
    mul_PW_SW_PW( b0, b1, b2, b3, b4, c0, t0, t1, t2, t3, t4);
    sub_PW_PW_PW( a0, a1, a2, a3, a4, t0, t1, t2, t3, t4, r0, r1, r2, r3, r4 );
    c1 = r0 / b0;
    mul_PW_SW_PW( b0, b1, b2, b3, b4, c1, t0, t1, t2, t3, t4 );
    sub_PW_PW_PW( r0, r1, r2, r3, r4, t0, t1, t2, t3, t4, r0, r1, r2, r3, r4 );
    c2 = r0 / b0;
    mul_PW_SW_PW( b0, b1, b2, b3, b4, c2, t0, t1, t2, t3, t4 );
    sub_PW_PW_PW( r0, r1, r2, r3, r4, t0, t1, t2, t3, t4, r0, r1, r2, r3, r4 );
    c3 = r0 / b0;
    mul_PW_SW_PW( b0, b1, b2, b3, b4, c3, t0, t1, t2, t3, t4 );
    sub_PW_PW_SW( r0, r1, r2, r3, r4, t0, t1, t2, t3, t4, r0 );
    c4 = r0 / b0;
    mul_PW_SW_PW( b0, b1, b2, b3, b4, c4, t0, t1, t2, t3, t4 );
    sub_PW_PW_SW( r0, r1, r2, r3, r4, t0, t1, t2, t3, t4, r0 );
    r0 = r0 / b0;
    c4 = c4 + r0;
    FastTwoSum( c3, c4, c3, c4 );    
    FastTwoSum( c2, c3, c2, c3 );
    FastTwoSum( c1, c2, c1, c2 );
    FastTwoSum( c0, c1, c0, c1 );
  }
  
  // sqr: 1-2
  template < typename T > INLINE void constexpr
  sqr_SW_DW( T const a0, T &c0, T &c1 ) NOEXCEPT
  {
    TwoProductFMA( a0, a0, c0, c1 );
    FastTwoSum( c0, c1, c0, c1 );
  }

  // sqr: 1-3
  template < typename T > INLINE void constexpr
  sqr_SW_TW( T const a0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    TwoProductFMA( a0, a0, c0, c1 );
    FastTwoSum( c0, c1, c0, c1 );
    c2 = fp_const<T>::zero();
  }

  // sqr: 1-4
  template < typename T > INLINE void constexpr
  sqr_SW_QW( T const a0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    TwoProductFMA( a0, a0, c0, c1 );
    FastTwoSum( c0, c1, c0, c1 );
    FastTwoSum( c0, c1, c0, c1 );
    c2 = fp_const<T>::zero();
    c3 = fp_const<T>::zero();
  }

  // sqr: 2-1
  template < typename T > INLINE void constexpr
  sqr_DW_SW( T const a0, T const a1, T &c0 ) NOEXCEPT
  {
    T t0;
    c0 = a0 * a0;
    t0 = a0 + a0;
    c0 = std::fma ( t0, a1, c0 );
  }

  // sqr: 2-2
  template < typename T > INLINE void constexpr
  sqr_DW_DW( T const a0, T const a1, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  sqr_DW_TW( T const a0, T const a1, T &c0, T &c1, T &c2 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  sqr_DW_QW( T const a0, T const a1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  sqr_TW_SW( T const a0, T const a1, T const a2, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    c0 = a0 * a0;
    t0 = a0 + a0;
    t1 = a1 + a2;
    c0 = std::fma ( t0, t1, c0 );
  }

  // sqr: 3-2
  template < typename T > INLINE void constexpr
  sqr_TW_DW( T const a0, T const a1, T const a2, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  sqr_TW_TW( T const a0, T const a1, T const a2, T &c0, T &c1, T &c2 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  sqr_TW_QW( T const a0, T const a1, T const a2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  sqr_QW_SW( T const a0, T const a1, T const a2, T const a3, T &c0 ) NOEXCEPT
  {
    T t0, t1;
    c0 = a0 * a0;
    t0 = a0 + a0;
    t1 = a1 + a2 + a3;
    c0 = std::fma ( t0, t1, c0 );
  }

  // sqr: 4-2
  template < typename T > INLINE void constexpr
  sqr_QW_DW( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  sqr_QW_TW( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1, T &c2 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  sqr_QW_QW( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
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
  template < typename T > INLINE void constexpr
  sqrt_SW_SW ( T const a0, T &c0 ) NOEXCEPT
  {
    c0 = std::sqrt( a0 );
  }

  // sqrt: 1-2
  template < typename T > INLINE void constexpr
  sqrt_SW_PA ( T const a0, T &c0, T &c1 ) NOEXCEPT
  {
    c0 = std::sqrt( a0 );
    c1 = ( std::fma( -c0, c0, a0 ) ) / (c0 + c0);
  }

  // sqrt: 1-3
  template < typename T > INLINE void constexpr
  sqrt_SW_QTW ( T const a0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    sqrt_SW_PA ( a0, c0, c1 );
    sqr_PA_QTW ( c0, c1, t0, t1, t2 );
    sub_SW_QTW_QTW ( a0, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = c0 + c1;
    td = td * fp_const<T>::two();
    c2 = tn / td;
  }

  // sqrt: 1-4
  template < typename T > INLINE void constexpr
  sqrt_SW_QQW ( T const a0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    sqrt_SW_QTW ( a0, c0, c1, c2 );
    sqr_QTW_QQW ( c0, c1, c2, t0, t1, t2, t3 );
    sub_SW_QQW_QQW ( a0, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = c0 + c1 + c2;
    td = td * fp_const<T>::two();
    c3 = tn / td;
  }

  // sqrt: 2-1
  template < typename T > INLINE void constexpr
  sqrt_PA_SW ( T const a0, T const a1, T &c0 ) NOEXCEPT
  {
    c0 = std::sqrt( a0 + a1 );
  }

  // sqrt: 2-2
  template < typename T > INLINE void constexpr
  sqrt_PA_PA ( T const a0, T const a1, T &c0, T &c1 ) NOEXCEPT
  {
    c0 = std::sqrt( a0 );
    c1 = ( std::fma( -c0, c0, a0 ) + a1 ) / (c0 + c0);
  }

  // sqrt: 2-3
  template < typename T > INLINE void constexpr
  sqrt_PA_QTW ( T const a0, T const a1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    sqrt_PA_PA ( a0, a1, c0, c1 );
    sqr_PA_QTW ( c0, c1, t0, t1, t2 );
    sub_PA_QTW_QTW ( a0, a1, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = c0 + c1;
    td = td * fp_const<T>::two();
    c2 = tn / td;
  }

  // sqrt: 2-4
  template < typename T > INLINE void constexpr
  sqrt_PA_QQW ( T const a0, T const a1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    sqrt_PA_QTW ( a0, a1, c0, c1, c2 );
    sqr_QTW_QQW ( c0, c1, c2, t0, t1, t2, t3 );
    sub_PA_QQW_QQW ( a0, a1, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = c0 + c1 + c2;
    td = td * fp_const<T>::two();
    c3 = tn / td;
  }

  // sqrt: 3-1
  template < typename T > INLINE void constexpr
  sqrt_QTW_SW ( T const a0, T const a1, T const a2, T &c0 ) NOEXCEPT
  {
    c0 = std::sqrt( a0 + a1 + a2 );
  }

  // sqrt: 3-2
  template < typename T > INLINE void constexpr
  sqrt_QTW_PA ( T const a0, T const a1, T const a2, T &c0, T &c1 ) NOEXCEPT
  {
    c0 = std::sqrt( a0 );
    c1 = ( std::fma( -c0, c0, a0 ) + a1 + a2 ) / (c0 + c0);
  }

  // sqrt: 3-3
  template < typename T > INLINE void constexpr
  sqrt_QTW_QTW ( T const a0, T const a1, T const a2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T t0, t1, t2;
    sqrt_QTW_PA ( a0, a1, a2, c0, c1 );
    sqr_PA_QTW ( c0, c1, t0, t1, t2 );
    sub_QTW_QTW_QTW ( a0, a1, a2, t0, t1, t2, t0, t1, t2 );
    T tn, td;
    tn = t0 + t1 + t2;
    td = c0 + c1;
    td = td * fp_const<T>::two();
    c2 = tn / td;
  }

  // sqrt: 3-4
  template < typename T > INLINE void constexpr
  sqrt_QTW_QQW ( T const a0, T const a1, T const a2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    sqrt_QTW_QTW ( a0, a1, a2, c0, c1, c2 );
    sqr_QTW_QQW ( c0, c1, c2, t0, t1, t2, t3 );
    sub_QTW_QQW_QQW ( a0, a1, a2, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = c0 + c1 + c2;
    td = td * fp_const<T>::two();
    c3 = tn / td;
  }

  // sqrt: 4-1
  template < typename T > INLINE void constexpr
  sqrt_QQW_SW ( T const a0, T const a1, T const a2, T const a3, T &c0 ) NOEXCEPT
  {
    c0 = std::sqrt( a0 + a1 + a2 + a3 );
  }

  // sqrt: 4-2
  template < typename T > INLINE void constexpr
  sqrt_QQW_PA ( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1 ) NOEXCEPT
  {
    c0 = std::sqrt( a0 );
    c1 = ( std::fma( -c0, c0, a0 ) + a1 + a2 + a3 ) / (c0 + c0);
  }

  // sqrt: 4-3
  template < typename T > INLINE void constexpr
  sqrt_QQW_QTW ( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    sqrt_QTW_QTW ( a0, a1, a2+a3, c0, c1, c2 );
  }

  // sqrt: 4-4
  template < typename T > INLINE void constexpr
  sqrt_QQW_QQW ( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T t0, t1, t2, t3;
    sqrt_QQW_QTW ( a0, a1, a2, a3, c0, c1, c2 );
    sqr_QTW_QQW ( c0, c1, c2, t0, t1, t2, t3 );
    sub_QQW_QQW_QQW ( a0, a1, a2, a3, t0, t1, t2, t3, t0, t1, t2, t3 );
    T tn, td;
    tn = t0 + t1 + t2 + t3;
    td = c0 + c1 + c2;
    td = td * fp_const<T>::two();
    c3 = tn / td;
  }

  // sqrt: 5-5
  template < typename T > INLINE void constexpr
  sqrt_QPW_QPW ( T const a0, T const a1, T const a2, T const a3, T const a4, T &c0, T &c1, T &c2, T &c3, T &c4 ) NOEXCEPT
  {
    T t0, t1, t2, t3, t4;
    // [c1,c2,c3,c4] = QQW_sqrt(a1,a2,a3,a4);
    sqrt_QQW_QQW ( a0, a1, a2, a3, c0, c1, c2, c3 );
    // [c1,c2,c3,c4] = QQW_normal(c1,c2,c3,c4);
#if 0
    //if ( N_accuracy > 0 || A == Algorithm::Quasi ) {
    TwoSum( c0, c1, c0, c1 );        //    twoSum( c.x[0], c.x[1] );
    TwoSum( c1, c2, c1, c2 );        //    twoSum( c.x[1], c.x[2] );
    TwoSum( c2, c3, c2, c3 );        //    twoSum( c.x[2], c.x[3] );
    
    FastTwoSum( c0, c1, c0, c1 );      //    quickSum( c.x[0], c.x[1] );
    FastTwoSum( c1, c2, c1, c2 );      //    quickSum( c.x[1], c.x[2] );

    FastTwoSum( c0, c1, c0, c1 );      //     quickSum( c.x[0], c.x[1] );
#endif
    // [t1,t2,t3,t4,t5] = Q5W_mul(c1,c2,c3,c4,0,c1,c2,c3,c4,0);    
    mul_QPW_QPW_QPW ( c0, c1, c2, c3, fp_const<T>::zero(),
		      c0, c1, c2, c3, fp_const<T>::zero(),
		      t0, t1, t2, t3, t4);
    // [t1,t2,t3,t4,t5] = Q5W_sum(t1,t2,t3,t4,t5,-a1,-a2,-a3,-a4,-a5);
    sub_QPW_QPW_QPW (a0, a1, a2, a3, a4, t0, t1, t2, t3, t4, 
		     t0, t1, t2, t3, t4);  // t = (a - t) <= t = (t - a)
    T tn, td;
    tn = t0 + t1 + t2 + t3 + t4; // bunshi = t1 + t2 + t3 + t4 + t5;
    td = c0 + c1 + c2 + c3; // bunbo = 2*(c1 + c2 + c3 + c4);
    c4 = tn / td; // c5 = - bunshi / bunbo; 
    c4 = c4 * fp_const<T>::nhalf();
  }  
  // sqrt: 1-2
  template < typename T > INLINE void constexpr
  sqrt_SW_DW ( T const a0, T &c0, T &c1 ) NOEXCEPT
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::nhalf();
    T const ax0 = (a0 * ex) * ex2;
    T const h0 = fp_const<T>::threehalves();
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
  template < typename T > INLINE void constexpr
  sqrt_SW_TW ( T const a0, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::nhalf();
    T const ax0 = (a0 * ex) * ex2;
    T const h0 = fp_const<T>::threehalves();
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
  template < typename T > INLINE void constexpr
  sqrt_SW_QW ( T const a0, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::nhalf();
    T const ax0 = (a0 * ex) * ex2;
    T const h0 = fp_const<T>::threehalves();
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
  template < typename T > INLINE void constexpr
  sqrt_DW_SW ( T const a0, T const a1, T &c0 ) NOEXCEPT
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

    c0 = ax + c0 * ( x * fp_const<T>::nhalf() );
    c0 = c0 * e;
  }

  // sqrt: 2-2
  template < typename T > INLINE void constexpr
  sqrt_DW_DW ( T const a0, T const a1, T &c0, T &c1 ) NOEXCEPT
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::nhalf();
    T const ax0 = (a0 * ex) * ex2;
    T const ax1 = (a1 * ex) * ex2;
    T const h0 = fp_const<T>::threehalves();
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
  template < typename T > INLINE void constexpr
  sqrt_DW_TW ( T const a0, T const a1, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::nhalf();
    T const ax0 = (a0 * ex) * ex2;
    T const ax1 = (a1 * ex) * ex2;
    T const h0 = fp_const<T>::threehalves();
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
  template < typename T > INLINE void constexpr
  sqrt_DW_QW ( T const a0, T const a1, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::nhalf();
    T const ax0 = (a0 * ex) * ex2;
    T const ax1 = (a1 * ex) * ex2;
    T const h0 = fp_const<T>::threehalves();
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
  template < typename T > INLINE void constexpr
  sqrt_TW_SW ( T const a0, T const a1, T const a2, T &c0 ) NOEXCEPT
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

    c0 = ax + c0 * ( x * fp_const<T>::nhalf() );
    c0 = c0 * e;
  }

  // sqrt: 3-2
  template < typename T > INLINE void constexpr
  sqrt_TW_DW ( T const a0, T const a1, T const a2, T &c0, T &c1 ) NOEXCEPT
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::nhalf();
    T const ax0 = (a0 * ex) * ex2;
    T const ax1 = (a1 * ex) * ex2;
    T const ax2 = (a2 * ex) * ex2;
    T const h0 = fp_const<T>::threehalves();
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
  template < typename T > INLINE void constexpr
  sqrt_TW_TW ( T const a0, T const a1, T const a2, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::nhalf();
    T const ax0 = (a0 * ex) * ex2;
    T const ax1 = (a1 * ex) * ex2;
    T const ax2 = (a2 * ex) * ex2;
    T const h0 = fp_const<T>::threehalves();
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
  template < typename T > INLINE void constexpr
  sqrt_TW_QW ( T const a0, T const a1, T const a2, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::nhalf();
    T const ax0 = (a0 * ex) * ex2;
    T const ax1 = (a1 * ex) * ex2;
    T const ax2 = (a2 * ex) * ex2;
    T const h0 = fp_const<T>::threehalves();
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
  template < typename T > INLINE void constexpr
  sqrt_QW_SW ( T const a0, T const a1, T const a2, T const a3, T &c0 ) NOEXCEPT
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

    c0 = ax + c0 * ( x * fp_const<T>::nhalf() );
    c0 = c0 * e;
  }

  // sqrt: 4-2
  template < typename T > INLINE void constexpr
  sqrt_QW_DW ( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1 ) NOEXCEPT
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::nhalf();
    T const ax0 = (a0 * ex) * ex2;
    T const ax1 = (a1 * ex) * ex2;
    T const ax2 = (a2 * ex) * ex2;
    T const ax3 = (a3 * ex) * ex2;
    T const h0 = fp_const<T>::threehalves();
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
  template < typename T > INLINE void constexpr
  sqrt_QW_TW ( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1, T &c2 ) NOEXCEPT
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::nhalf();
    T const ax0 = (a0 * ex) * ex2;
    T const ax1 = (a1 * ex) * ex2;
    T const ax2 = (a2 * ex) * ex2;
    T const ax3 = (a3 * ex) * ex2;
    T const h0 = fp_const<T>::threehalves();
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
  template < typename T > INLINE void constexpr
  sqrt_QW_QW ( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1, T &c2, T &c3 ) NOEXCEPT
  {
    T const as = std::sqrt( a0 );
    T const e  = fp_const<T>::exponent( as );
    T const e2 = e * fp_const<T>::two();
    T const ex = fp_const<T>::exponenti( as );
    T const ex2 = ex * fp_const<T>::nhalf();
    T const ax0 = (a0 * ex) * ex2;
    T const ax1 = (a1 * ex) * ex2;
    T const ax2 = (a2 * ex) * ex2;
    T const ax3 = (a3 * ex) * ex2;
    T const h0 = fp_const<T>::threehalves();
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


