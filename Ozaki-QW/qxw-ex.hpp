//
// written by T.Imamura as a complimental object missing in the Ozaki's version
//
namespace QxW {

template < typename T > __always_inline void add_TW_TW_TW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T __restrict__ &c1, T __restrict__ &c2, T __restrict__ &c3)
{
    T e1, e2;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e1);
    c3 = a3 + b3 + e2 + e1;

    FastTwoSum(c1,c2,c1,c2);
    FastTwoSum(c2,c3,c2,c3);
}

template < typename T > __always_inline void add_QW_QW_QW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T __restrict__ &c1, T __restrict__ &c2, T __restrict__ &c3, T __restrict__ &c4)
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

      FastTwoSum(c1,c2,c1,c2);
      FastTwoSum(c2,c3,c2,c3);
      FastTwoSum(c3,c4,c3,c4);
}

}

