//
// written by T.Imamura as a complimental object missing in the Ozaki's version
//
namespace QxW {

template < typename T > __always_inline void const add_TW_TW_TW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3)
{
    T e1, e2;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e1);
    c3 = a3 + b3 + e2 + e1;

    FastTwoSum(c2,c3,c2,c3);
    FastTwoSum(c1,c2,c1,c2);
}

template < typename T > __always_inline void const add_QW_QW_QW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3, T &c4)
{
      T e2,e3,e4,e5,e6;
      TwoSum(a1,b1,c1,e2); // 1   e
      TwoSum(a2,b2,e3,e4); // e   e^2
      TwoSum(a3,b3,e5,e6); // e^2 e^3
      e6 = e6 + a4 + b4; // e^3
      TwoSum(e2,e3,c2,e3); // e   e^2
      TwoSum(e3,e4,e3,e4); // e^2 e^3
      TwoSum(e3,e5,c3,e5); // e^2 e^3
      c4 = e4 + e5 + e6; // e^3

      FastTwoSum(c3,c4,c3,c4);
      FastTwoSum(c2,c3,c2,c3);
      FastTwoSum(c1,c2,c1,c2);
}

template < typename T > __always_inline void const mul_TW_TW_TW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3)
{
      T p00, q00;
      T p01, q01;
      T p10, q10;
      TwoProductFMA( a1, b1, p00, q00 ); // 1. e
      TwoProductFMA( a1, b2, p01, q01 ); // e, e^2
      TwoProductFMA( a2, b1, p10, q10 ); // e, e^2

      c1 = p00;
      TwoSum( q00, p01, q00, p01 );
      TwoSum( q00, p10, c2, p10 );

      c3 = p01
           + std::fma( a2, b2, p10 ) // e^2+e^2 -> e^2
           + std::fma( a1, b3, q01 ) // e^2+e^2 -> e^2
           + std::fma( a3, b1, q10 ); // e^2+e^2 -> e^2

    FastTwoSum(c2,c3,c2,c3);
    FastTwoSum(c1,c2,c1,c2);
}

template < typename T > __always_inline void const mul_QW_QW_QW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3, T &c4)
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

