// mul: 1-1-2
template < typename T > __always_inline void
mul_SW_SW_PA ( T const a1, T const b1, T & c1, T & c2 )
{
  TwoProductFMA ( a1, b1, c1, c2 );
}

// mul: 1-1-3
template < typename T > __always_inline void
mul_SW_SW_QTW ( T const a1, T const b1, T & c1, T & c2, T & c3 )
{
  TwoProductFMA ( a1, b1, c1, c2 );
  c3 = fp_const<T>::zero();
}

// mul: 1-1-4
template < typename T > __always_inline void
mul_SW_SW_QQW ( T const a1, T const b1, T & c1, T & c2, T & c3, T & c4 )
{
  TwoProductFMA ( a1, b1, c1, c2 );
  c3 = fp_const<T>::zero();
  c4 = fp_const<T>::zero();
}

// mul: 1-2-1
template < typename T > __always_inline void
mul_SW_PA_SW ( T const a1, T const b1, T const b2, T & c1 )
{
  T t1;
  t1 = b1 + b2;
  c1 = a1 * t1;
}

// mul: 1-2-2
template < typename T > __always_inline void
mul_SW_PA_PA ( T const a1, T const b1, T const b2, T & c1, T & c2 )
{
  TwoProductFMA ( a1, b1, c1, c2 );
  c2 = std::fma ( a1, b2, c2 );
}

// mul: 1-2-3
template < typename T > __always_inline void
mul_SW_PA_QTW ( T const a1, T const b1, T const b2, T & c1, T & c2, T & c3 )
{
  T t1;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t1, c3 );
  TwoSum ( c2, t1, c2, t1 );
  c3 = c3 + t1;
}

// mul: 1-2-4
template < typename T > __always_inline void
mul_SW_PA_QQW ( T const a1, T const b1, T const b2, T & c1, T & c2, T & c3, T & c4 )
{
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, c4, c3 );
  TwoSum ( c2, c4, c2, c4 );
  TwoSum ( c3, c4, c3, c4 );
}

// mul: 1-3-1
template < typename T > __always_inline void
mul_SW_QTW_SW ( T const a1, T const b1, T const b2, T const b3, T & c1 )
{
  T t1;
  t1 = b1 + b2;
  t1 = t1 + b3;
  c1 = a1 * t1;
}

// mul: 1-3-2
template < typename T > __always_inline void
mul_SW_QTW_PA ( T const a1, T const b1, T const b2, T const b3, T & c1, T & c2 )
{
  T t1;
  t1 = b2 + b3;
  TwoProductFMA ( a1, b1, c1, c2 );
  c2 = std::fma ( a1, t1, c2 );
}

// mul: 1-3-3
template < typename T > __always_inline void
mul_SW_QTW_QTW ( T const a1, T const b1, T const b2, T const b3, T & c1, T & c2, T & c3 )
{
  T t1,t2;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t1, t2 );
  TwoSum ( c2, t1, c2, t1 );
  t2 = t2 + t1;
  c3 = std::fma ( a1, b3, t2 );
}

// mul: 1-3-4
template < typename T > __always_inline void
mul_SW_QTW_QQW ( T const a1, T const b1, T const b2, T const b3, T & c1, T & c2, T & c3, T & c4 )
{
  T t1,t2;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, c4, c3 );
  TwoProductFMA ( a1, b3, t1, t2 );
  TwoSum ( c2, c4, c2, c4 );
  TwoSum ( c3, t1, c3, t1 );
  TwoSum ( c3, c4, c3, c4 );
  t2 = t2 + t1;
  c4 = c4 + t2;
}

// mul: 1-4-1
template < typename T > __always_inline void
mul_SW_QQW_SW ( T const a1, T const b1, T const b2, T const b3, T const b4, T & c1 )
{
  T t1;
  t1 = b1 + b2;
  t1 = t1 + b3;
  t1 = t1 + b4;
  c1 = a1 * t1;
}

// mul: 1-4-2
template < typename T > __always_inline void
mul_SW_QQW_PA ( T const a1, T const b1, T const b2, T const b3, T const b4, T & c1, T & c2 )
{
  T t1;
  t1 = b2 + b3;
  t1 = t1 + b4;
  TwoProductFMA ( a1, b1, c1, c2 );
  c2 = std::fma ( a1, t1, c2 );
}

// mul: 1-4-3
template < typename T > __always_inline void
mul_SW_QQW_QTW ( T const a1, T const b1, T const b2, T const b3, T const b4, T & c1, T & c2, T & c3 )
{
  T t1,t2,t3;
  t1 = b3 + b4;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t2, t3 );
  TwoSum ( c2, t2, c2, t2 );
  t3 = t3 + t2;
  c3 = std::fma ( a1, t1, t3 );
}

// mul: 1-4-4
template < typename T > __always_inline void
mul_SW_QQW_QQW ( T const a1, T const b1, T const b2, T const b3, T const b4, T & c1, T & c2, T & c3, T & c4 )
{
  T t1,t2;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, c4, c3 );
  TwoProductFMA ( a1, b3, t1, t2 );
  TwoSum ( c2, c4, c2, c4 );
  TwoSum ( c3, t1, c3, t1 );
  TwoSum ( c3, c4, c3, c4 );
  t2 = t2 + t1;
  t2 = std::fma ( a1, b4, t2 );
  c4 = c4 + t2;
}

// mul: 2-1-1
template < typename T > __always_inline void
mul_PA_SW_SW ( T const a1, T const a2, T const b1, T & c1 )
{
  T t1;
  t1 = a1 + a2;
  c1 = t1 * b1;
}

// mul: 2-1-2
template < typename T > __always_inline void
mul_PA_SW_PA ( T const a1, T const a2, T const b1, T & c1, T & c2 )
{
  TwoProductFMA ( a1, b1, c1, c2 );
  c2 = std::fma ( a2, b1, c2 );
}

// mul: 2-1-3
template < typename T > __always_inline void
mul_PA_SW_QTW ( T const a1, T const a2, T const b1, T & c1, T & c2, T & c3 )
{
  T t1;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a2, b1, t1, c3 );
  TwoSum ( c2, t1, c2, t1 );
  c3 = c3 + t1;
}

// mul: 2-1-4
template < typename T > __always_inline void
mul_PA_SW_QQW ( T const a1, T const a2, T const b1, T & c1, T & c2, T & c3, T & c4 )
{
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a2, b1, c4, c3 );
  TwoSum ( c2, c4, c2, c4 );
  TwoSum ( c3, c4, c3, c4 );
}

// mul: 2-2-1
template < typename T > __always_inline void
mul_PA_PA_SW ( T const a1, T const a2, T const b1, T const b2, T & c1 )
{
  T t1,t2;
  t1 = a1 + a2;
  t2 = b1 + b2;
  c1 = t1 * t2;
}

// mul: 2-2-2
template < typename T > __always_inline void
mul_PA_PA_PA ( T const a1, T const a2, T const b1, T const b2, T & c1, T & c2 )
{
  T t1;
  TwoProductFMA ( a1, b1, c1, t1 );

  TwoProductFMA ( a1, b2, c2, t3 );
  TwoProductFMA ( a2, b1, t4, t5 );

  TwoSum( c2, t1, c2, t1 ); // t1 large
  TwoSum( c2, t4, c2, t4 ); // t4 as well
  FastTwoSum( c1, c2, c1, c2 ); // c2 is symmetric wrt a and b ?

  c2 = c2 + (t3 + t5); // large delta
  FastTwoSum( c1, c2, c1, c2 );

  c2 = c2 + (t1 + t4); // small delta
  FastTwoSum( c1, c2, c1, c2 );

}

// mul: 2-2-3
template < typename T > __always_inline void
mul_PA_PA_QTW ( T const a1, T const a2, T const b1, T const b2, T & c1, T & c2, T & c3 )
{
  T t1,t2,t3;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t1, c3 );
  TwoProductFMA ( a2, b1, t2, t3 );
  TwoSum ( c2, t1, c2, t1 );
  TwoSum ( c2, t2, c2, t2 );
  t3 = t3 + t1;
  c3 = c3 + t2;
  t3 = std::fma ( a2, b2, t3 );
  c3 = c3 + t3;
}

// mul: 2-2-4
template < typename T > __always_inline void
mul_PA_PA_QQW ( T const a1, T const a2, T const b1, T const b2, T & c1, T & c2, T & c3, T & c4 )
{
  T t1,t2,t3,t4;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t1, c3 );
  TwoProductFMA ( a2, b1, t2, t3 );
  TwoProductFMA ( a2, b2, t4, c4 );
  TwoSum ( c2, t1, c2, t1 );
  TwoSum ( c3, t3, c3, t3 );
  TwoSum ( t4, t1, t4, t1 );
  TwoSum ( c2, t2, c2, t2 );
  TwoSum ( c3, t4, c3, t4 );
  TwoSum ( c3, t2, c3, t2 );
  t3 = t3 + t1;
  c4 = c4 + t4;
  t2 = t2 + t3;
  c4 = c4 + t2;
}

// mul: 2-3-1
template < typename T > __always_inline void
mul_PA_QTW_SW ( T const a1, T const a2, T const b1, T const b2, T const b3, T & c1 )
{
  T t1,t2;
  t1 = a1 + a2;
  t2 = b1 + b2;
  t2 = t2 + b3;
  c1 = t1 * t2;
}

// mul: 2-3-2
template < typename T > __always_inline void
mul_PA_QTW_PA ( T const a1, T const a2, T const b1, T const b2, T const b3, T & c1, T & c2 )
{
  T t1,t2;
  t1 = b2 + b3;
  TwoProductFMA ( a1, b1, c1, t2 );
  t2 = std::fma ( a1, t1, t2 );
  c2 = std::fma ( a2, b1, t2 );
}

// mul: 2-3-3
template < typename T > __always_inline void
mul_PA_QTW_QTW ( T const a1, T const a2, T const b1, T const b2, T const b3, T & c1, T & c2, T & c3 )
{
  T t1,t2,t3,t4;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t1, t2 );
  TwoProductFMA ( a2, b1, t3, t4 );
  TwoSum ( c2, t1, c2, t1 );
  TwoSum ( c2, t3, c2, t3 );
  t4 = t4 + t1;
  t2 = t2 + t3;
  c3 = std::fma ( a2, b2, t4 );
  t2 = std::fma ( a1, b3, t2 );
  c3 = c3 + t2;
}

// mul: 2-3-4
template < typename T > __always_inline void
mul_PA_QTW_QQW ( T const a1, T const a2, T const b1, T const b2, T const b3, T & c1, T & c2, T & c3, T & c4 )
{
  T t1,t2,t3,t4,t5,t6;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, c4, c3 );
  TwoProductFMA ( a2, b1, t1, t2 );
  TwoProductFMA ( a1, b3, t3, t4 );
  TwoProductFMA ( a2, b2, t5, t6 );
  TwoSum ( c2, c4, c2, c4 );
  TwoSum ( c3, t2, c3, t2 );
  TwoSum ( t3, t5, t3, t5 );
  TwoSum ( c2, t1, c2, t1 );
  TwoSum ( c3, t3, c3, t3 );
  TwoSum ( c4, t1, c4, t1 );
  TwoSum ( c3, c4, c3, c4 );
  t2 = t2 + t5;
  t6 = t6 + t3;
  t2 = std::fma ( a2, b3, t2 );
  t4 = t4 + t1;
  c4 = c4 + t6;
  t2 = t2 + t4;
  c4 = c4 + t2;
}

// mul: 2-4-1
template < typename T > __always_inline void
mul_PA_QQW_SW ( T const a1, T const a2, T const b1, T const b2, T const b3, T const b4, T & c1 )
{
  T t1,t2;
  t1 = a1 + a2;
  t2 = b1 + b2;
  t2 = t2 + b3;
  t2 = t2 + b4;
  c1 = t1 * t2;
}

// mul: 2-4-2
template < typename T > __always_inline void
mul_PA_QQW_PA ( T const a1, T const a2, T const b1, T const b2, T const b3, T const b4, T & c1, T & c2 )
{
  T t1,t2;
  t1 = b2 + b3;
  t1 = t1 + b4;
  TwoProductFMA ( a1, b1, c1, t2 );
  t2 = std::fma ( a1, t1, t2 );
  c2 = std::fma ( a2, b1, t2 );
}

// mul: 2-4-3
template < typename T > __always_inline void
mul_PA_QQW_QTW ( T const a1, T const a2, T const b1, T const b2, T const b3, T const b4, T & c1, T & c2, T & c3 )
{
  T t1,t2,t3,t4,t5;
  t1 = b3 + b4;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t2, t3 );
  TwoProductFMA ( a2, b1, t4, t5 );
  TwoSum ( c2, t2, c2, t2 );
  TwoSum ( c2, t4, c2, t4 );
  t5 = t5 + t2;
  t3 = t3 + t4;
  c3 = std::fma ( a2, b2, t5 );
  t3 = std::fma ( a1, t1, t3 );
  c3 = c3 + t3;
}

// mul: 2-4-4
template < typename T > __always_inline void
mul_PA_QQW_QQW ( T const a1, T const a2, T const b1, T const b2, T const b3, T const b4, T & c1, T & c2, T & c3, T & c4 )
{
  T t1,t2,t3,t4,t5,t6,t7;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t1, c3 );
  TwoProductFMA ( a2, b1, t2, t3 );
  TwoProductFMA ( a1, b3, t4, t5 );
  TwoProductFMA ( a2, b2, t6, t7 );
  TwoSum ( c2, t1, c2, t1 );
  TwoSum ( c3, t3, c3, t3 );
  TwoSum ( t4, t6, t4, t6 );
  TwoSum ( c2, t2, c2, t2 );
  TwoSum ( c3, t4, c3, t4 );
  TwoSum ( t1, t2, t1, t2 );
  TwoSum ( c3, t1, c3, t1 );
  t3 = t3 + t6;
  t7 = t7 + t4;
  c4 = std::fma ( a2, b3, t3 );
  t5 = t5 + t2;
  t7 = std::fma ( a1, b4, t7 );
  c4 = c4 + t5;
  t1 = t1 + t7;
  c4 = c4 + t1;
}

// mul: 3-1-1
template < typename T > __always_inline void
mul_QTW_SW_SW ( T const a1, T const a2, T const a3, T const b1, T & c1 )
{
  T t1;
  t1 = a1 + a2;
  t1 = t1 + a3;
  c1 = t1 * b1;
}

// mul: 3-1-2
template < typename T > __always_inline void
mul_QTW_SW_PA ( T const a1, T const a2, T const a3, T const b1, T & c1, T & c2 )
{
  T t1;
  t1 = a2 + a3;
  TwoProductFMA ( a1, b1, c1, c2 );
  c2 = std::fma ( t1, b1, c2 );
}

// mul: 3-1-3
template < typename T > __always_inline void
mul_QTW_SW_QTW ( T const a1, T const a2, T const a3, T const b1, T & c1, T & c2, T & c3 )
{
  T t1,t2;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a2, b1, t1, t2 );
  TwoSum ( c2, t1, c2, t1 );
  t2 = t2 + t1;
  c3 = std::fma ( a3, b1, t2 );
}

// mul: 3-1-4
template < typename T > __always_inline void
mul_QTW_SW_QQW ( T const a1, T const a2, T const a3, T const b1, T & c1, T & c2, T & c3, T & c4 )
{
  T t1,t2;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a2, b1, c4, c3 );
  TwoProductFMA ( a3, b1, t1, t2 );
  TwoSum ( c2, c4, c2, c4 );
  TwoSum ( c3, t1, c3, t1 );
  TwoSum ( c3, c4, c3, c4 );
  t2 = t2 + t1;
  c4 = c4 + t2;
}

// mul: 3-2-1
template < typename T > __always_inline void
mul_QTW_PA_SW ( T const a1, T const a2, T const a3, T const b1, T const b2, T & c1 )
{
  T t1,t2;
  t1 = a1 + a2;
  t1 = t1 + a3;
  t2 = b1 + b2;
  c1 = t1 * t2;
}

// mul: 3-2-2
template < typename T > __always_inline void
mul_QTW_PA_PA ( T const a1, T const a2, T const a3, T const b1, T const b2, T & c1, T & c2 )
{
  T t1,t2;
  t1 = a2 + a3;
  TwoProductFMA ( a1, b1, c1, t2 );
  t2 = std::fma ( a1, b2, t2 );
  c2 = std::fma ( t1, b1, t2 );
}

// mul: 3-2-3
template < typename T > __always_inline void
mul_QTW_PA_QTW ( T const a1, T const a2, T const a3, T const b1, T const b2, T & c1, T & c2, T & c3 )
{
  T t1,t2,t3,t4;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t1, t2 );
  TwoProductFMA ( a2, b1, t3, t4 );
  TwoSum ( c2, t1, c2, t1 );
  TwoSum ( c2, t3, c2, t3 );
  t4 = t4 + t1;
  t2 = t2 + t3;
  c3 = std::fma ( a3, b1, t4 );
  t2 = std::fma ( a2, b2, t2 );
  c3 = c3 + t2;
}

// mul: 3-2-4
template < typename T > __always_inline void
mul_QTW_PA_QQW ( T const a1, T const a2, T const a3, T const b1, T const b2, T & c1, T & c2, T & c3, T & c4 )
{
  T t1,t2,t3,t4,t5,t6;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, c4, c3 );
  TwoProductFMA ( a2, b1, t1, t2 );
  TwoProductFMA ( a2, b2, t3, t4 );
  TwoProductFMA ( a3, b1, t5, t6 );
  TwoSum ( c2, c4, c2, c4 );
  TwoSum ( c3, t2, c3, t2 );
  TwoSum ( t3, t5, t3, t5 );
  TwoSum ( c2, t1, c2, t1 );
  TwoSum ( c3, t3, c3, t3 );
  TwoSum ( c4, t1, c4, t1 );
  TwoSum ( c3, c4, c3, c4 );
  t2 = t2 + t5;
  t6 = t6 + t3;
  t2 = std::fma ( a3, b2, t2 );
  t4 = t4 + t1;
  c4 = c4 + t6;
  t2 = t2 + t4;
  c4 = c4 + t2;
}

// mul: 3-3-1
template < typename T > __always_inline void
mul_QTW_QTW_SW ( T const a1, T const a2, T const a3, T const b1, T const b2, T const b3, T & c1 )
{
  T t1,t2;
  t1 = a1 + a2;
  t1 = t1 + a3;
  t2 = b1 + b2;
  t2 = t2 + b3;
  c1 = t1 * t2;
}

// mul: 3-3-2
template < typename T > __always_inline void
mul_QTW_QTW_PA ( T const a1, T const a2, T const a3, T const b1, T const b2, T const b3, T & c1, T & c2 )
{
  T t1,t2,t3;
  t1 = a2 + a3;
  t2 = b2 + b3;
  TwoProductFMA ( a1, b1, c1, t3 );
  t3 = std::fma ( a1, t2, t3 );
  c2 = std::fma ( t1, b1, t3 );
}

// mul: 3-3-3
template < typename T > __always_inline void
mul_QTW_QTW_QTW ( T const a1, T const a2, T const a3, T const b1, T const b2, T const b3, T & c1, T & c2, T & c3 )
{
  T t1,t2,t3,t4;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t1, t2 );
  TwoProductFMA ( a2, b1, t3, t4 );
  TwoSum ( c2, t1, c2, t1 );
  TwoSum ( c2, t3, c2, t3 );
  t4 = t4 + t1;
  t2 = t2 + t3;
  t4 = std::fma ( a3, b1, t4 );
  c3 = std::fma ( a2, b2, t2 );
  t4 = std::fma ( a1, b3, t4 );
  c3 = c3 + t4;
}

// mul: 3-3-4
template < typename T > __always_inline void
mul_QTW_QTW_QQW ( T const a1, T const a2, T const a3, T const b1, T const b2, T const b3, T & c1, T & c2, T & c3, T & c4 )
{
  T t1,t2,t3,t4,t5,t6,t7,t8,t9;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t1, c3 );
  TwoProductFMA ( a2, b1, t2, t3 );
  TwoProductFMA ( a1, b3, t4, t5 );
  TwoProductFMA ( a2, b2, t6, t7 );
  TwoProductFMA ( a3, b1, t8, t9 );
  TwoSum ( c2, t1, c2, t1 );
  TwoSum ( c3, t3, c3, t3 );
  TwoSum ( t4, t6, t4, t6 );
  TwoSum ( t8, t1, t8, t1 );
  TwoSum ( c2, t2, c2, t2 );
  TwoSum ( c3, t4, c3, t4 );
  TwoSum ( t8, t2, t8, t2 );
  TwoSum ( c3, t8, c3, t8 );
  t3 = t3 + t6;
  t9 = t9 + t1;
  t3 = std::fma ( a3, b2, t3 );
  t7 = t7 + t4;
  c4 = std::fma ( a2, b3, t9 );
  t5 = t5 + t2;
  t3 = t3 + t7;
  c4 = c4 + t5;
  t8 = t8 + t3;
  c4 = c4 + t8;
}

// mul: 3-4-1
template < typename T > __always_inline void
mul_QTW_QQW_SW ( T const a1, T const a2, T const a3, T const b1, T const b2, T const b3, T const b4, T & c1 )
{
  T t1,t2;
  t1 = a1 + a2;
  t1 = t1 + a3;
  t2 = b1 + b2;
  t2 = t2 + b3;
  t2 = t2 + b4;
  c1 = t1 * t2;
}

// mul: 3-4-2
template < typename T > __always_inline void
mul_QTW_QQW_PA ( T const a1, T const a2, T const a3, T const b1, T const b2, T const b3, T const b4, T & c1, T & c2 )
{
  T t1,t2,t3;
  t1 = a2 + a3;
  t2 = b2 + b3;
  t2 = t2 + b4;
  TwoProductFMA ( a1, b1, c1, t3 );
  t3 = std::fma ( a1, t2, t3 );
  c2 = std::fma ( t1, b1, t3 );
}

// mul: 3-4-3
template < typename T > __always_inline void
mul_QTW_QQW_QTW ( T const a1, T const a2, T const a3, T const b1, T const b2, T const b3, T const b4, T & c1, T & c2, T & c3 )
{
  T t1,t2,t3,t4,t5;
  t1 = b3 + b4;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t2, t3 );
  TwoProductFMA ( a2, b1, t4, t5 );
  TwoSum ( c2, t2, c2, t2 );
  TwoSum ( c2, t4, c2, t4 );
  t5 = t5 + t2;
  t3 = t3 + t4;
  t5 = std::fma ( a3, b1, t5 );
  c3 = std::fma ( a2, b2, t3 );
  t5 = std::fma ( a1, t1, t5 );
  c3 = c3 + t5;
}

// mul: 3-4-4
template < typename T > __always_inline void
mul_QTW_QQW_QQW ( T const a1, T const a2, T const a3, T const b1, T const b2, T const b3, T const b4, T & c1, T & c2, T & c3, T & c4 )
{
  T t1,t2,t3,t4,t5,t6,t7,t8;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t1, c3 );
  TwoProductFMA ( a2, b1, t2, t3 );
  TwoProductFMA ( a1, b3, t4, t5 );
  TwoProductFMA ( a2, b2, t6, t7 );
  TwoProductFMA ( a3, b1, c4, t8 );
  TwoSum ( c2, t1, c2, t1 );
  TwoSum ( c3, t3, c3, t3 );
  TwoSum ( t4, t6, t4, t6 );
  TwoSum ( c4, t1, c4, t1 );
  TwoSum ( c2, t2, c2, t2 );
  TwoSum ( c3, t4, c3, t4 );
  TwoSum ( c4, t2, c4, t2 );
  TwoSum ( c3, c4, c3, c4 );
  t3 = t3 + t6;
  t8 = t8 + t1;
  t3 = std::fma ( a3, b2, t3 );
  t7 = t7 + t4;
  t8 = std::fma ( a2, b3, t8 );
  t5 = t5 + t2;
  t3 = t3 + t7;
  t8 = t8 + t5;
  c4 = std::fma ( a1, b4, c4 );
  t3 = t3 + t8;
  c4 = c4 + t3;
}

// mul: 4-1-1
template < typename T > __always_inline void
mul_QQW_SW_SW ( T const a1, T const a2, T const a3, T const a4, T const b1, T & c1 )
{
  T t1;
  t1 = a1 + a2;
  t1 = t1 + a3;
  t1 = t1 + a4;
  c1 = t1 * b1;
}

// mul: 4-1-2
template < typename T > __always_inline void
mul_QQW_SW_PA ( T const a1, T const a2, T const a3, T const a4, T const b1, T & c1, T & c2 )
{
  T t1;
  t1 = a2 + a3;
  t1 = t1 + a4;
  TwoProductFMA ( a1, b1, c1, c2 );
  c2 = std::fma ( t1, b1, c2 );
}

// mul: 4-1-3
template < typename T > __always_inline void
mul_QQW_SW_QTW ( T const a1, T const a2, T const a3, T const a4, T const b1, T & c1, T & c2, T & c3 )
{
  T t1,t2,t3;
  t1 = a3 + a4;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a2, b1, t2, t3 );
  TwoSum ( c2, t2, c2, t2 );
  t3 = t3 + t2;
  c3 = std::fma ( t1, b1, t3 );
}

// mul: 4-1-4
template < typename T > __always_inline void
mul_QQW_SW_QQW ( T const a1, T const a2, T const a3, T const a4, T const b1, T & c1, T & c2, T & c3, T & c4 )
{
  T t1,t2;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a2, b1, c4, c3 );
  TwoProductFMA ( a3, b1, t1, t2 );
  TwoSum ( c2, c4, c2, c4 );
  TwoSum ( c3, t1, c3, t1 );
  TwoSum ( c3, c4, c3, c4 );
  t2 = t2 + t1;
  t2 = std::fma ( a4, b1, t2 );
  c4 = c4 + t2;
}

// mul: 4-2-1
template < typename T > __always_inline void
mul_QQW_PA_SW ( T const a1, T const a2, T const a3, T const a4, T const b1, T const b2, T & c1 )
{
  T t1,t2;
  t1 = a1 + a2;
  t1 = t1 + a3;
  t1 = t1 + a4;
  t2 = b1 + b2;
  c1 = t1 * t2;
}

// mul: 4-2-2
template < typename T > __always_inline void
mul_QQW_PA_PA ( T const a1, T const a2, T const a3, T const a4, T const b1, T const b2, T & c1, T & c2 )
{
  T t1,t2;
  t1 = a2 + a3;
  t1 = t1 + a4;
  TwoProductFMA ( a1, b1, c1, t2 );
  t2 = std::fma ( a1, b2, t2 );
  c2 = std::fma ( t1, b1, t2 );
}

// mul: 4-2-3
template < typename T > __always_inline void
mul_QQW_PA_QTW ( T const a1, T const a2, T const a3, T const a4, T const b1, T const b2, T & c1, T & c2, T & c3 )
{
  T t1,t2,t3,t4,t5;
  t1 = a3 + a4;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t2, t3 );
  TwoProductFMA ( a2, b1, t4, t5 );
  TwoSum ( c2, t2, c2, t2 );
  TwoSum ( c2, t4, c2, t4 );
  t5 = t5 + t2;
  t3 = t3 + t4;
  c3 = std::fma ( t1, b1, t5 );
  t3 = std::fma ( a2, b2, t3 );
  c3 = c3 + t3;
}

// mul: 4-2-4
template < typename T > __always_inline void
mul_QQW_PA_QQW ( T const a1, T const a2, T const a3, T const a4, T const b1, T const b2, T & c1, T & c2, T & c3, T & c4 )
{
  T t1,t2,t3,t4,t5,t6,t7;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t1, c3 );
  TwoProductFMA ( a2, b1, t2, t3 );
  TwoProductFMA ( a2, b2, t4, t5 );
  TwoProductFMA ( a3, b1, t6, t7 );
  TwoSum ( c2, t1, c2, t1 );
  TwoSum ( c3, t3, c3, t3 );
  TwoSum ( t4, t6, t4, t6 );
  TwoSum ( c2, t2, c2, t2 );
  TwoSum ( c3, t4, c3, t4 );
  TwoSum ( t1, t2, t1, t2 );
  TwoSum ( c3, t1, c3, t1 );
  t3 = t3 + t6;
  t7 = t7 + t4;
  c4 = std::fma ( a4, b1, t3 );
  t5 = t5 + t2;
  t7 = std::fma ( a3, b2, t7 );
  c4 = c4 + t5;
  t1 = t1 + t7;
  c4 = c4 + t1;
}

// mul: 4-3-1
template < typename T > __always_inline void
mul_QQW_QTW_SW ( T const a1, T const a2, T const a3, T const a4, T const b1, T const b2, T const b3, T & c1 )
{
  T t1,t2;
  t1 = a1 + a2;
  t1 = t1 + a3;
  t1 = t1 + a4;
  t2 = b1 + b2;
  t2 = t2 + b3;
  c1 = t1 * t2;
}

// mul: 4-3-2
template < typename T > __always_inline void
mul_QQW_QTW_PA ( T const a1, T const a2, T const a3, T const a4, T const b1, T const b2, T const b3, T & c1, T & c2 )
{
  T t1,t2,t3;
  t1 = a2 + a3;
  t1 = t1 + a4;
  t2 = b2 + b3;
  TwoProductFMA ( a1, b1, c1, t3 );
  t3 = std::fma ( a1, t2, t3 );
  c2 = std::fma ( t1, b1, t3 );
}

// mul: 4-3-3
template < typename T > __always_inline void
mul_QQW_QTW_QTW ( T const a1, T const a2, T const a3, T const a4, T const b1, T const b2, T const b3, T & c1, T & c2, T & c3 )
{
  T t1,t2,t3,t4,t5;
  t1 = a3 + a4;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t2, t3 );
  TwoProductFMA ( a2, b1, t4, t5 );
  TwoSum ( c2, t2, c2, t2 );
  TwoSum ( c2, t4, c2, t4 );
  t5 = t5 + t2;
  t3 = t3 + t4;
  t5 = std::fma ( t1, b1, t5 );
  c3 = std::fma ( a2, b2, t3 );
  t5 = std::fma ( a1, b3, t5 );
  c3 = c3 + t5;
}

// mul: 4-3-4
template < typename T > __always_inline void
mul_QQW_QTW_QQW ( T const a1, T const a2, T const a3, T const a4, T const b1, T const b2, T const b3, T & c1, T & c2, T & c3, T & c4 )
{
  T t1,t2,t3,t4,t5,t6,t7,t8;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t1, c3 );
  TwoProductFMA ( a2, b1, t2, t3 );
  TwoProductFMA ( a1, b3, t4, t5 );
  TwoProductFMA ( a2, b2, t6, t7 );
  TwoProductFMA ( a3, b1, c4, t8 );
  TwoSum ( c2, t1, c2, t1 );
  TwoSum ( c3, t3, c3, t3 );
  TwoSum ( t4, t6, t4, t6 );
  TwoSum ( c4, t1, c4, t1 );
  TwoSum ( c2, t2, c2, t2 );
  TwoSum ( c3, t4, c3, t4 );
  TwoSum ( c4, t2, c4, t2 );
  TwoSum ( c3, c4, c3, c4 );
  t3 = t3 + t6;
  t8 = t8 + t1;
  t3 = std::fma ( a4, b1, t3 );
  t7 = t7 + t4;
  t8 = std::fma ( a3, b2, t8 );
  t5 = t5 + t2;
  t3 = t3 + t7;
  t8 = t8 + t5;
  c4 = std::fma ( a2, b3, c4 );
  t3 = t3 + t8;
  c4 = c4 + t3;
}

// mul: 4-4-1
template < typename T > __always_inline void
mul_QQW_QQW_SW ( T const a1, T const a2, T const a3, T const a4, T const b1, T const b2, T const b3, T const b4, T & c1 )
{
  T t1,t2;
  t1 = a1 + a2;
  t1 = t1 + a3;
  t1 = t1 + a4;
  t2 = b1 + b2;
  t2 = t2 + b3;
  t2 = t2 + b4;
  c1 = t1 * t2;
}

// mul: 4-4-2
template < typename T > __always_inline void
mul_QQW_QQW_PA ( T const a1, T const a2, T const a3, T const a4, T const b1, T const b2, T const b3, T const b4, T & c1, T & c2 )
{
  T t1,t2,t3;
  t1 = a2 + a3;
  t1 = t1 + a4;
  t2 = b2 + b3;
  t2 = t2 + b4;
  TwoProductFMA ( a1, b1, c1, t3 );
  t3 = std::fma ( a1, t2, t3 );
  c2 = std::fma ( t1, b1, t3 );
}

// mul: 4-4-3
template < typename T > __always_inline void
mul_QQW_QQW_QTW ( T const a1, T const a2, T const a3, T const a4, T const b1, T const b2, T const b3, T const b4, T & c1, T & c2, T & c3 )
{
  T t1,t2,t3,t4,t5,t6;
  t1 = a3 + a4;
  t2 = b3 + b4;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t3, t4 );
  TwoProductFMA ( a2, b1, t5, t6 );
  TwoSum ( c2, t3, c2, t3 );
  TwoSum ( c2, t5, c2, t5 );
  t6 = t6 + t3;
  t4 = t4 + t5;
  t6 = std::fma ( t1, b1, t6 );
  c3 = std::fma ( a2, b2, t4 );
  t6 = std::fma ( a1, t2, t6 );
  c3 = c3 + t6;
}

// mul: 4-4-4
template < typename T > __always_inline void
mul_QQW_QQW_QQW ( T const a1, T const a2, T const a3, T const a4, T const b1, T const b2, T const b3, T const b4, T & c1, T & c2, T & c3, T & c4 )
{
  T t1,t2,t3,t4,t5,t6,t7,t8,t9;
  TwoProductFMA ( a1, b1, c1, c2 );
  TwoProductFMA ( a1, b2, t1, c3 );
  TwoProductFMA ( a2, b1, t2, t3 );
  TwoProductFMA ( a1, b3, t4, t5 );
  TwoProductFMA ( a2, b2, t6, t7 );
  TwoProductFMA ( a3, b1, t8, t9 );
  TwoSum ( c2, t1, c2, t1 );
  TwoSum ( c3, t3, c3, t3 );
  TwoSum ( t4, t6, t4, t6 );
  TwoSum ( t8, t1, t8, t1 );
  TwoSum ( c2, t2, c2, t2 );
  TwoSum ( c3, t4, c3, t4 );
  TwoSum ( t8, t2, t8, t2 );
  TwoSum ( c3, t8, c3, t8 );
  t3 = t3 + t6;
  t9 = t9 + t1;
  t3 = std::fma ( a4, b1, t3 );
  t7 = t7 + t4;
  t9 = std::fma ( a3, b2, t9 );
  t5 = t5 + t2;
  t3 = t3 + t7;
  t9 = t9 + t5;
  t8 = std::fma ( a1, b4, t8 );
  c4 = std::fma ( a2, b3, t3 );
  t9 = t9 + t8;
  c4 = c4 + t9;
}

