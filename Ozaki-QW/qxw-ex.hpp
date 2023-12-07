//
// written by T.Imamura as a complimental object missing in the Ozaki's version
//
namespace QxW {

template < typename T > __always_inline void const add_TW_SW_TW(T const a1, T const a2, T const a3, T const b1, T &c1, T &c2, T &c3)
{
    T e1;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,e1,c2,e1);
    TwoSum(c2,e1,c2,e1);
    FastTwoSum(c1,c2,c1,c2);

    c3 = a3 + e1;
    FastTwoSum(c2,c3,c2,c3);
    FastTwoSum(c1,c2,c1,c2);
}
template < typename T > __always_inline void const add_TW_DW_TW(T const a1, T const a2, T const a3, T const b1, T const b2, T &c1, T &c2, T &c3)
{
    T e1, e2;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e1);
    FastTwoSum(c1,c2,c1,c2);

    c3 = a3 + e2 + e1;
    FastTwoSum(c2,c3,c2,c3);
    FastTwoSum(c1,c2,c1,c2);
}
template < typename T > __always_inline void const add_TW_TW_TW(T const a1, T const a2, T const a3, T const b1, T const b2, T const b3, T &c1, T &c2, T &c3)
{
    T e1, e2, e3, e4;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e1);
    FastTwoSum(c1,c2,c1,c2);

    TwoSum(a3,b3,c3,e3);
    TwoSum(c3,e1,c3,e1);
    FastTwoSum(c2,c3,c2,c3);

    TwoSum(c3,e2,c3,e2);
    FastTwoSum(c2,c3,c2,c3);

    e4 = e1 + e2 + e3;
    c3 = c3 + e4;
    FastTwoSum(c2,c3,c2,c3);
    FastTwoSum(c1,c2,c1,c2);
}


template < typename T > __always_inline void const add_QW_SW_QW(T const a1, T const a2, T const a3, T const a4, T const b1, T &c1, T &c2, T &c3, T &c4)
{
      T e2,e4,e6;
      TwoSum(a1,b1,c1,e2); // 1   e
      TwoSum(a2,e2,c2,e4); // e   e^2
      FastTwoSum(c1,c2,c1,c2);

      TwoSum(a3,e4,c3,e6); // e^2 e^3
      FastTwoSum(c2,c3,c2,c3);
      FastTwoSum(c1,c2,c1,c2);

      c4 = e6 + a4; // e^3
      FastTwoSum(c3,c4,c3,c4);
      FastTwoSum(c2,c3,c2,c3);
      FastTwoSum(c1,c2,c1,c2);
}
template < typename T > __always_inline void const add_QW_DW_QW(T const a1, T const a2, T const a3, T const a4, T const b1, T const b2, T &c1, T &c2, T &c3, T &c4)
{
      T e2,e3,e4,e5,e6;
      TwoSum(a1,b1,c1,e2); // 1   e
      TwoSum(a2,b2,e3,e4); // e   e^2
      TwoSum(a3,e4,e5,e6); // e^2 e^3

      TwoSum(e2,e3,c2,e3); // e   e^2
      FastTwoSum(c1,c2,c1,c2);

      TwoSum(e3,e5,c3,e5); // e^2 e^3
      FastTwoSum(c2,c3,c2,c3);
      FastTwoSum(c1,c2,c1,c2);

      c4 = e5 + e6 + a4; // e^3
      FastTwoSum(c3,c4,c3,c4);
      FastTwoSum(c2,c3,c2,c3);
      FastTwoSum(c1,c2,c1,c2);
}
template < typename T > __always_inline void const add_QW_TW_QW(T const a1, T const a2, T const a3, T const a4, T const b1, T const b2, T const b3, T &c1, T &c2, T &c3, T &c4)
{
      T e2,e3,e4,e5,e6;
      TwoSum(a1,b1,c1,e2); // 1   e
      TwoSum(a2,b2,e3,e4); // e   e^2
      TwoSum(a3,b3,e5,e6); // e^2 e^3

      TwoSum(e2,e3,c2,e3); // e   e^2
      FastTwoSum(c1,c2,c1,c2);

      TwoSum(e3,e4,e3,e4); // e^2 e^3
      TwoSum(e3,e5,c3,e5); // e^2 e^3
      FastTwoSum(c2,c3,c2,c3);
      FastTwoSum(c1,c2,c1,c2);

      c4 = e4 + e5 + e6 + a4; // e^3
      FastTwoSum(c3,c4,c3,c4);
      FastTwoSum(c2,c3,c2,c3);
      FastTwoSum(c1,c2,c1,c2);
}
template < typename T > __always_inline void const add_QW_QW_QW(T const a1, T const a2, T const a3, T const a4, T const b1, T const b2, T const b3, T const b4, T &c1, T &c2, T &c3, T &c4)
{
#if 0
      T e2,e3,e4,e5,e6;
      TwoSum(a1,b1,c1,e2); // 1   e
      TwoSum(a2,b2,e3,e4); // e   e^2
      TwoSum(a3,b3,e5,e6); // e^2 e^3

      TwoSum(e2,e3,c2,e3); // e   e^2
      FastTwoSum(c1,c2,c1,c2);

      TwoSum(e3,e4,e3,e4); // e^2 e^3
      TwoSum(e3,e5,c3,e5); // e^2 e^3
      FastTwoSum(c2,c3,c2,c3);
      FastTwoSum(c1,c2,c1,c2);

      c4 = e4 + e5 + e6 + a4 + b4; // e^3
      FastTwoSum(c3,c4,c3,c4);
      FastTwoSum(c2,c3,c2,c3);
      FastTwoSum(c1,c2,c1,c2);
#else
  T t1,t2,t3,t4;
  TwoSum ( a1, b1, c1, c2 );
  TwoSum ( a2, b2, t1, c3 );
  TwoSum ( a3, b3, t2, c4 );
  TwoSum ( a4, b4, t3, t4 );
  TwoSum ( c2, t1, c2, t1 );
  FastTwoSum ( c1, c2, c1, c2 );
  TwoSum ( c3, t2, c3, t2 );
  FastTwoSum ( c2, c3, c2, c3 );
  TwoSum ( c4, t3, c4, t3 );
  t4 = t4 + t3;
  TwoSum ( c3, t1, c3, t1 );
  FastTwoSum ( c2, c3, c2, c3 );
  TwoSum ( c4, t2, c4, t2 );
  FastTwoSum ( c3, c4, c3, c4 );
  t4 = t4 + t2;
  TwoSum ( c4, t1, c4, t1 );
  FastTwoSum ( c3, c4, c3, c4 );
  t4 = t4 + t1;
  c4 = c4 + t4;
  FastTwoSum ( c1, c2, c1, c2 );
  FastTwoSum ( c2, c3, c2, c3 );
  FastTwoSum ( c3, c4, c3, c4 );
#endif
}

template < typename T > __always_inline void const mul_TW_TW_TW(T const a1, T const a2, T const a3, T const b1, T const b2, T const b3, T &c1, T &c2, T &c3)
{
#if 0
    T e1, e2, e3, e4, e5;
    T t2, t3;

      TwoProductFMA( a1, b1, c1, e1 ); // 1. e
      TwoProductFMA( a1, b2, t2, e2 ); // e, e^2
      TwoProductFMA( a2, b1, t3, e3 ); // e, e^2

      TwoSum( t2, t3, c2, e4 ); // e, e^2
      TwoSum( c2, e1, c2, e5 ); // e, e^2

      c3 =
             std::fma( a3, b1, e2 ) // e^2+e^2 -> e^2
           + std::fma( a2, b2, e3 ) // e^2+e^2 -> e^2
           + std::fma( a1, b3, e4 ) // e^2+e^2 -> e^2
	   + e5;

    FastTwoSum(c2,c3,c2,c3);
    FastTwoSum(c1,c2,c1,c2);
#else
    T z1, z2, z3;
    T f1, f2, f3;
    T e1, e2, e3;
    T t1, t2, t3;

      TwoProductFMA( a3, b1, z1, f1 );	// e^2. e^3 | {z1}, {f1}
      TwoProductFMA( a2, b2, z2, f2 );	// e^2, e^3 | {z1,z2}, {f1,f2}
      TwoProductFMA( a1, b3, z3, f3 );	// e^2, e^3 | {z1,z2,z3}, {f1,f2,f3}

      TwoProductFMA( a1, b2, t1, e1 );	// e^1, e^2 | {t1}, {z1,z2,z3, e1}, {f1,f2,f3}
      TwoProductFMA( a2, b1, t2, e2 );	// e^1, e^2 | {t1,t2}, {z1,z2,z3, e1,e2}, {f1,f2,f3}

      TwoProductFMA( a1, b1, c1, t3 );	// e^0, e^1 | c1 | {}, {t1,t2,t3}, {z1,z2,z3, e1,e2}, {f1,f2,f3}

      TwoSum( z2, z1, z2, z1 );		// e^2, e^3 | c1 | {}, {t1,t2,t3}, {z2,z3, e1,e2}, {f1,f2,f3, [z1]}
      TwoSum( z3, z2, c3, z2 );		// e^2, e^3 | c1, c3 | {}, {t1,t2,t3}, {e1,e2}, {f1,f2,f3, [z1]}
z1 = z1 + z2;
      TwoSum( e1, e2, e1, e2 );		// e^2, e^3 | c1, c3 | {}, {t1,t2,t3}, {e1}, {f1,f2,f3, [z1]}
z1 = z1 + e2;
      TwoSum( c3, e1, c3, e1 );		// e^2, e^3 | c1, c3 | {}, {t1,t2,t3}, {}, {f1,f2,f3, [z1]}
z1 = z1 + e1;

      TwoSum( t2, t1, t2, t1 );		// e^1, e^2 | c1, c3 | {}, {t2,t3}, {[t1]}, {f1,f2,f3, [z1]}
      TwoSum( t3, t2, c2, t2 );		// e^1, e^2 | c1, c2, c3 | {}, {}, {[t1]}, {f1,f2,f3, [z1]}
t1 = t1 + t2;

    FastTwoSum(c1,c2,c1,c2);

      TwoSum( c3, t1, c3, t1 );		// e^2, e^3 | c1, c2, c3 | {}, {}, {}, {f1,f2,f3, [z1]}
z1 = z1 + t1;

    FastTwoSum(c2,c3,c2,c3);

      e1 = std::fma( a3, b3, z1 );
      f1 = std::fma( a3, b2, f1 ) + std::fma( a2, b3, f2 ) + f3;
      c3 += e1 + f1;

    FastTwoSum(c1,c2,c1,c2);
    FastTwoSum(c2,c3,c2,c3);

#endif

}

template < typename T > __always_inline void const mul_QW_QW_QW(T const a1, T const a2, T const a3, T const a4, T const b1, T const b2, T const b3, T const b4, T &c1, T &c2, T &c3, T &c4)
{
      T c5;
      T q10,q11,q12;
      T q2[6];
      T q30,q31,q32,q33,q34;

      TwoProductFMA( a1, b1, c1, q10 ); // O(1,e)

      TwoProductFMA( a1, b2, q11, q2[0] ); // O(e,e^2)
      TwoProductFMA( a2, b1, q12, q2[1] ); // O(e,e^2)

      TwoSum( q10, q11, q10, q11 ); // O(e,e^2)
      TwoSum( q10, q12, c2, q12 ); // O(e,e^2)

      TwoProductFMA( a1, b3, q2[2], q30 ); // O(e^2,e^3)
      TwoProductFMA( a2, b2, q2[3], q31 ); // O(e^2,e^3)
      TwoProductFMA( a3, b1, q2[4], q32 ); // O(e^2,e^3)

      TwoSum( q11, q12, q2[5], c4 ); // O(e^2,e^3)
      c4 = c4 + q30 + q31 + q32;

      TwoSum( q2[0], q2[1], q2[0], q30 ); // O(e^2,e^3)
      TwoSum( q2[3], q2[4], q2[3], q31 ); // O(e^2,e^3)
      TwoSum( q2[0], q2[2], q2[0], q32 ); // O(e^2,e^3)
      TwoSum( q2[3], q2[5], q2[3], q33 ); // O(e^2,e^3)
      TwoSum( q2[0], q2[3], c3, q34 ); // O(e^2,e^3)
      c4 = c4 + q30 + q31 + q32 + q33 + q34; // O(e^3)

      c4 = std::fma( a1, b4,
           std::fma( a2, b3, 
           std::fma( a3, b2, 
           std::fma( a4, b1, c4 ) ) ) );

      FastTwoSum(c3,c4,c3,c4);
      FastTwoSum(c2,c3,c2,c3);
      FastTwoSum(c1,c2,c1,c2);

}

}

