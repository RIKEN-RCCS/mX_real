// sqrt: 1-1
template < typename T > __always_inline void
sqrt_SW_SW ( T const a0, T &c0 )
{
  c0 = std::sqrt( a0 );
}

// sqrt: 1-2
template < typename T > __always_inline void
sqrt_SW_PA ( T const a0, T &c0, T &c1 )
{
  c0 = std::sqrt( a0 );
  c1 = ( std::fma( -c0, c0, a0 ) ) / (c0 + c0);
}

// sqrt: 1-3
template < typename T > __always_inline void
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
template < typename T > __always_inline void
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
template < typename T > __always_inline void
sqrt_PA_SW ( T const a0, T const a1, T &c0 )
{
  c0 = std::sqrt( a0 + a1 );
}

// sqrt: 2-2
template < typename T > __always_inline void
sqrt_PA_PA ( T const a0, T const a1, T &c0, T &c1 )
{
  c0 = std::sqrt( a0 );
  c1 = ( std::fma( -c0, c0, a0 ) + a1 ) / (c0 + c0);
}

// sqrt: 2-3
template < typename T > __always_inline void
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
template < typename T > __always_inline void
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
template < typename T > __always_inline void
sqrt_QTW_SW ( T const a0, T const a1, T const a2, T &c0 )
{
  c0 = std::sqrt( a0 + a1 + a2 );
}

// sqrt: 3-2
template < typename T > __always_inline void
sqrt_QTW_PA ( T const a0, T const a1, T const a2, T &c0, T &c1 )
{
  c0 = std::sqrt( a0 );
  c1 = ( std::fma( -c0, c0, a0 ) + a1 + a2 ) / (c0 + c0);
}

// sqrt: 3-3
template < typename T > __always_inline void
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
template < typename T > __always_inline void
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
template < typename T > __always_inline void
sqrt_QQW_SW ( T const a0, T const a1, T const a2, T const a3, T &c0 )
{
  c0 = std::sqrt( a0 + a1 + a2 + a3 );
}

// sqrt: 4-2
template < typename T > __always_inline void
sqrt_QQW_PA ( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1 )
{
  c0 = std::sqrt( a0 );
  c1 = ( std::fma( -c0, c0, a0 ) + a1 + a2 + a3 ) / (c0 + c0);
}

// sqrt: 4-3
template < typename T > __always_inline void
sqrt_QQW_QTW ( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1, T &c2 )
{
  sqrt_QTW_QTW ( a0, a1, a2+a3, c0, c1, c2 );
}

// sqrt: 4-4
template < typename T > __always_inline void
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
template < typename T > __always_inline void
sqrt_SW_DW ( T const a0, T &c0, T &c1 )
{
  T const as = std::sqrt( a0 );
  T const e  = fp_const<T>::exponent ( as );
  T const ex = fp_const<T>::exponenti( as );
  T const b0 = ( a0 * ex ) * ex;

  T const x  = e / as;
  T const ax = b0 * x;
  T r0, r1;

  TwoProductFMA( ax, ax, r0, r1 );
  sub_SW_DW_SW( b0, r0, r1, c0 );
  TwoSum( ax, c0 * ( x * fp_const<T>::half() ), c0, c1 );

  c0 = c0 * e;
  c1 = c1 * e;
}

// sqrt: 1-3
template < typename T > __always_inline void
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
template < typename T > __always_inline void
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
template < typename T > __always_inline void
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
template < typename T > __always_inline void
sqrt_DW_DW ( T const a0, T const a1, T &c0, T &c1 )
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
  TwoSum( ax, c0 * ( x * fp_const<T>::half() ), c0, c1 );

  c0 = c0 * e;
  c1 = c1 * e;
}

// sqrt: 2-3
template < typename T > __always_inline void
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
template < typename T > __always_inline void
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
template < typename T > __always_inline void
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
template < typename T > __always_inline void
sqrt_TW_DW ( T const a0, T const a1, T const a2, T &c0, T &c1 )
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
  TwoSum( ax, c0 * ( x * fp_const<T>::half() ), c0, c1 );

  c0 = c0 * e;
  c1 = c1 * e;
}

// sqrt: 3-3
template < typename T > __always_inline void
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
template < typename T > __always_inline void
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
template < typename T > __always_inline void
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
template < typename T > __always_inline void
sqrt_QW_DW ( T const a0, T const a1, T const a2, T const a3, T &c0, T &c1 )
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
  sub_QW_DW_SW( b0, b1, b2, b3, r0, r1, c0 );
  TwoSum( ax, c0 * ( x * fp_const<T>::half() ), c0, c1 );

  c0 = c0 * e;
  c1 = c1 * e;
}

// sqrt: 4-3
template < typename T > __always_inline void
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
template < typename T > __always_inline void
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

