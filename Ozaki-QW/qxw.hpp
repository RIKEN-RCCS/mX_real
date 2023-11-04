#ifndef QXW_H
#define QXW_H

// This prototype code was written 
// by Katsuhisa Ozaki (Shibaura Institute of Technology)
// Debug is not sufficiently done, so please use it carefully and 
// not spread it. 

// C++-fied version 2023.11.2 (converted from Ozaki's code)
// by Toshiyuki Imamura (RIKEN, R-CCS)


#include <stdio.h>
#include <math.h>

namespace QxW {


template < typename T > struct fp_const {};
template <> struct fp_const<float> {
  static inline auto constexpr zero() { return 0.0f; }
  static inline auto constexpr one()  { return 1.0f; }
  static inline auto constexpr half() { return 0.5f; }
};
template <> struct fp_const<double> {
  static inline auto constexpr zero() { return 0.0; }
  static inline auto constexpr one()  { return 1.0; }
  static inline auto constexpr half() { return 0.5; }
};



// ------------------------
// Basic Part
// ------------------------

template < typename T > inline void TwoSum(T const a, T const b, T &x, T &y)
{
    T z;
    x = a + b;
    z = x - a;
    y = ( a - (x-z) ) + (b-z);
}

template < typename T > inline void FastTwoSum(T const a, T const b, T &x, T &y)
{
    x = a + b;
    y = (a-x) + b;
}

template < typename T > inline void TwoProductFMA(T const a, T const b, T &x, T &y)
{
    x = a*b;
    y = std::fma(a,b,-x);
}

//------------------------------
//Addition
//------------------------------

//add: 1-1-2
template < typename T > inline void add_SW_SW_PA(T const& a, T const& b, T &ch, T &cl)
{
    TwoSum(a,b,ch,cl);
}

//add: 1-1-3
template < typename T > inline void add_SW_SW_QTW(T const& a, T const& b, T &c1, T &c2, T &c3)
{
    TwoSum(a,b,c1,c2);
    c3 = fp_const<T>::zero();
}

//add: 1-1-4
template < typename T > inline void add_SW_SW_QQW(T const& a, T const& b, T &c1, T &c2, T &c3, T &c4)
{
    TwoSum(a,b,c1,c2);
    c3= fp_const<T>::zero();
    c4= fp_const<T>::zero();
}

//add: 1-2-1
template < typename T > inline void add_SW_PA_SW(T const& ah, T const& bh, T const& bl, T &c)
{
    c = ah + bh + bl;
}

//add: 1-2-2
template < typename T > inline void add_SW_PA_PA(T const& a, T const& bh, T const& bl, T &ch, T &cl)
{
    T t;
    TwoSum(a,bh,ch,t);
    cl = bl + t;
}

//add: 1-2-3
template < typename T > inline void add_SW_PA_QTW(T const& a, T const& b1, T const& b2, T &c1, T &c2, T &c3)
{
    T e1;
    TwoSum(a,b1,c1,e1);
    TwoSum(b2,e1,c2,c3);
}

//add: 1-2-4
template < typename T > inline void add_SW_PA_QQW(T const& a, T const& b1, T const& b2, T &c1, T &c2, T &c3, T &c4)
{
    T e1;
    TwoSum(a,b1,c1,e1);
//    TwoSum(a,b2,c2,c3);
    TwoSum(b2,e1,c2,c3); //
    c4 = fp_const<T>::zero();
}

//add: 1-3-1
template < typename T > inline void add_SW_QTW_SW(T const& a, T const& b1, T const& b2, T const& b3, T &c)
{
    c = a + b1 + b2 + b3;
}

//add: 1-3-2
template < typename T > inline void add_SW_QTW_PA(T const& a, T const& b1, T const& b2, T const& b3, T &c1, T &c2)
{
    T e1;
    TwoSum(a,b1,c1,e1);
    c2 = b2 + e1 + b3;
}

//add: 1-3-3
template < typename T > inline void add_SW_QTW_QTW(T const& a, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3)
{
    T e1, e3;
    TwoSum(a,b1,c1,e1);
    TwoSum(b2,e1,c2,e3);
    c3 = b3 + e3;
}

//add: 1-3-4
template < typename T > inline void add_SW_QTW_QQW(T const& a, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2;
    TwoSum(a,b1,c1,e1);
    TwoSum(b2,e1,c2,e2);
    TwoSum(b3,e2,c3,c4);
}
//add: 1-4-1
template < typename T > inline void add_SW_QQW_SW(T const& a1, T const& b1, T const& b2, T const& b3, T const& b4, T &c)
{
    c = a1 + b1 + b2 + b3 + b4;
}
//add: 1-4-2
template < typename T > inline void add_SW_QQW_PA(T const& a, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2)
{
    T e1;
    TwoSum(a,b1,c1,e1);
    c2 = b2 + e1 + b3 + b4;
}
//add: 1-4-3
template < typename T > inline void add_SW_QQW_QTW(T const& a, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3)
{
    T e1, e2;
    TwoSum(a,b1,c1,e1);
    TwoSum(e1,b2,c2,e2);
    c3 = b3 + e2 + b4;
}

//add: 1-4-4
template < typename T > inline void add_SW_QQW_QQW(T const& a, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3;
    TwoSum(a,b1,c1,e1);
    TwoSum(e1,b2,c2,e2);
    TwoSum(e2,b3,c3,e3);
    c4 = b4 + e3;
}

//add: 2-1-1
template < typename T > inline void add_PA_SW_SW(T const& ah, T const& al, T const& b, T &c)
{
    c = ah + b + al;
}

//add: 2-1-2
template < typename T > inline void add_PA_SW_PA(T const& ah, T const& al, T const& b, T &ch, T &cl)
{
    T t;
    TwoSum(ah,b,ch,t);
    cl = al + t;
}

//add: 2-1-3
template < typename T > inline void add_PA_SW_QTW(T const& a1, T const& a2, T const& b, T &c1, T &c2, T &c3)
{
    T e1;
    TwoSum(a1,b,c1,e1);
    TwoSum(a2,e1,c2,c3);
}

//add: 2-1-4
template < typename T > inline void add_PA_SW_QQW(T const& a1, T const& a2, T const& b, T &c1, T &c2, T &c3, T &c4)
{
    T e1;
    TwoSum(a1,b,c1,e1);
    TwoSum(a2,e1,c2,c3);
    c4 = fp_const<T>::zero();
}

//add: 2-2-1
template < typename T > inline void add_PA_PA_SW(T const& ah, T const& al, T const& bh, T const& bl, T &c)
{
    c = ah + bh + al + bl;
}

//add: 2-2-2
template < typename T > inline void add_PA_PA_PA(T const& ah, T const& al, T const& bh, T const& bl, T &ch, T &cl)
{
    T t;
    TwoSum(ah,bh,ch,t);
    cl = al + bl + t;
}

//add: 2-2-3
template < typename T > inline void add_PA_PA_QTW(T const& a1, T const& a2, T const& b1, T const& b2, T &c1, T &c2, T &c3)
{
    T e1, e2, e3;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e3);
    c3 = e2 + e3;
}

//add: 2-2-4
template < typename T > inline void add_PA_PA_QQW(T const& a1, T const& a2, T const& b1, T const& b2, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e3);
    TwoSum(e2,e3,c3,c4);
}

//add: 2-3-1
template < typename T > inline void add_PA_QTW_SW(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T &c)
{
    c = a1 + b1 + a2 + b2 + b3;
}

//add: 2-3-2
template < typename T > inline void add_PA_QTW_PA(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T &c1, T &c2)
{
    T e1;
    TwoSum(a1,b1,c1,e1);
    c2 = a2 + b2 + e1 + b3;
}

//add: 2-3-3
template < typename T > inline void add_PA_QTW_QTW(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3)
{
    T e1, e2, e3;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e3);
    c3 = b3 + e2 + e3;
}

//add: 2-3-4
template < typename T > inline void add_PA_QTW_QQW(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3, e5, e6;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e3);
    TwoSum(b3,e2,c3,e5);
    TwoSum(c3,e3,c3,e6);
    c4 = e5 + e6;
}

//add: 2-4-1
template < typename T > inline void add_PA_QQW_SW(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T const& b4, T &c)
{
    c = a1 + b1 + a2 + b2 + b3 + b4;
}

//add: 2-4-2
template < typename T > inline void add_PA_QQW_PA(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2)
{
    T e1;
    TwoSum(a1,b1,c1,e1);
    c2 = a2 + b2 + e1 + b3 + b4;
}

//add: 2-4-3
template < typename T > inline void add_PA_QQW_QTW(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3)
{
    T e1, e2, e3;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e3);
    c3 = b3 + e2 + e3 + b4;
}

//add: 2-4-4
template < typename T > inline void add_PA_QQW_QQW(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3, e5, e6;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e3);
    TwoSum(b3,e2,c3,e5);
    TwoSum(c3,e3,c3,e6);
    c4 = b4 + e5 + e6;
}

//add: 3-1-1
template < typename T > inline void add_QTW_SW_SW(T const& a1, T const& a2, T const& a3, T const& b, T &c)
{
    c = a1 + b + a2 + a3;
}

//add: 3-1-2
template < typename T > inline void add_QTW_SW_PA(T const& a1, T const& a2, T const& a3, T const& b, T &c1, T &c2)
{
    T e1;
    TwoSum(a1,b,c1,e1);
    c2 = a2 + e1 + a3;
}

//add: 3-1-3
template < typename T > inline void add_QTW_SW_QTW(T const& a1, T const& a2, T const& a3, T const& b, T &c1, T &c2, T &c3)
{
    T e1, e3;
    TwoSum(a1,b,c1,e1);
    TwoSum(a2,e1,c2,e3);
    c3 = a3 + e3;
}

//add: 3-1-4
template < typename T > inline void add_QTW_SW_QQW(T const& a1, T const& a2, T const& a3, T const& b, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2;
    TwoSum(a1,b,c1,e1);
    TwoSum(a2,e1,c2,e2);
    TwoSum(a3,e2,c3,c4);
}

//add: 3-2-1
template < typename T > inline void add_QTW_PA_SW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T &c)
{
    c = a1 + b1 + a2 + b2 + a3;
}

//add: 3-2-2
template < typename T > inline void add_QTW_PA_PA(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T &c1, T &c2)
{
    T e1;
    TwoSum(a1,b1,c1,e1);
    c2 = a2 + b2 + e1 + a3;
}

//add: 3-2-3
template < typename T > inline void add_QTW_PA_QTW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T &c1, T &c2, T &c3)
{
    T e1, e2, e3;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e3);
    c3 = a3 + e2 + e3;
}

//add: 3-2-4
template < typename T > inline void add_QTW_PA_QQW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3, e5, e6;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e3);
    TwoSum(a3,e2,c3,e5);
    TwoSum(c3,e3,c3,e6);
    c4 = e5 + e6;
}

//add: 3-3-1
template < typename T > inline void add_QTW_QTW_SW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T &c)
{
    c = a1 + b1 + a2 + b2 + a3 + b3;
}

//add: 3-3-2
template < typename T > inline void add_QTW_QTW_PA(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T &c1, T &c2)
{
    T e1;
    TwoSum(a1,b1,c1,e1);
    c2 = a2 + b2 + e1; //追加してもよい
}

//add: 3-3-3
template < typename T > inline void add_QTW_QTW_QTW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3)
{
    T e1, e2, e3;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e3);
    c3 = a3 + b3 + e2 + e3;
}

//add: 3-3-4
template < typename T > inline void add_QTW_QTW_QQW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3, e4, e5, e6;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e3);
    TwoSum(a3,b3,c3,e4);
    TwoSum(c3,e2,c3,e5);
    TwoSum(c3,e3,c3,e6);
    c4 = e4 + e5 + e6;
}

//add: 3-4-1
template < typename T > inline void add_QTW_QQW_SW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T const& b4, T &c)
{
    c = a1 + b1 + a2 + b2 + a3 + b3 + b4;
}

//add: 3-4-2
template < typename T > inline void add_QTW_QQW_PA(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2)
{
    T e1;
    TwoSum(a1,b1,c1,e1);
    c2 = a2 + b2 + e1 + a3 + b3 + b4;
}

//add: 3-4-3
template < typename T > inline void add_QTW_QQW_QTW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3)
{
    T e1, e2, e3;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e3);
    c3 = a3 + b3 + e2 + e3 + b4;
}

//add: 3-4-4
template < typename T > inline void add_QTW_QQW_QQW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3, e4, e5, e6;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e3);
    TwoSum(a3,b3,c3,e4);
    TwoSum(c3,e2,c3,e5);
    TwoSum(c3,e3,c3,e6);
    c4 = b4 + e4 + e5 + e6;
}

//add: 4-1-1
template < typename T > inline void add_QQW_SW_SW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b, T &c)
{
    c = a1 + b + a2 + a3 + a4;
}

//add: 4-1-2
template < typename T > inline void add_QQW_SW_PA(T const& a1, T const& a2, T const& a3, T const& a4, T const& b, T &c1, T &c2)
{
    T e1;
    TwoSum(a1,b,c1,e1);
    c2 = a2 + e1 + a3 + a4;
}

//add: 4-1-3
template < typename T > inline void add_QQW_SW_QTW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b, T &c1, T &c2, T &c3)
{
    T e1, e2;
    TwoSum(a1,b,c1,e1);
    TwoSum(a2,e1,c2,e2);
    c3 = a3 + e2 + a4;
}

//add: 4-1-4
template < typename T > inline void add_QQW_SW_QQW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3;
    TwoSum(a1,b,c1,e1);
    TwoSum(a2,e1,c2,e2);
    TwoSum(a3,e2,c3,e3);
    c4 = a4 + e3;
}

//add: 4-2-1
template < typename T > inline void add_QQW_PA_SW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T &c)
{
    c = a1 + b1 + a2 + b2 + a3 + a4;
}

//add: 4-2-2
template < typename T > inline void add_QQW_PA_PA(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T &c1, T &c2)
{
    T e1;
    TwoSum(a1,b1,c1,e1);
    c2 = a2 + b2 + e1 + a3 + a4;
}

//add: 4-2-3
template < typename T > inline void add_QQW_PA_QTW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T &c1, T &c2, T &c3)
{
    T e1, e2, e3;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e3);
    c3 = a3 + e2 + e3 + a4;
}

//add: 4-2-4
template < typename T > inline void add_QQW_PA_QQW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3, e5, e6;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e3);
    TwoSum(a3,e2,c3,e5);
    TwoSum(c3,e3,c3,e6);
    c4 = a4 + e5 + e6;
}

//add: 4-3-1
template < typename T > inline void add_QQW_QTW_SW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T &c)
{
    c = a1 + b1 + a2 + b2 + a3 + b3 + a4;
}

//add: 4-3-2
template < typename T > inline void add_QQW_QTW_PA(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T &c1, T &c2)
{
    T e1;
    TwoSum(a1,b1,c1,e1);
    c2 = a2 + b2 + e1 + a3 + b3 + a4;
}

//add: 4-3-3
template < typename T > inline void add_QQW_QTW_QTW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3)
{
    T e1, e2, e3;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e3);
    c3 = a3 + b3 + e2 + e3 + a4;
}

//add: 4-3-4
template < typename T > inline void add_QQW_QTW_QQW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3, e4, e5, e6;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e3);
    TwoSum(a3,b3,c3,e4);
    TwoSum(c3,e2,c3,e5);
    TwoSum(c3,e3,c3,e6);
    c4 = a4 + e4 + e5 + e6;
}

//add: 4-4-1
template < typename T > inline void add_QQW_QQW_SW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T &c)
{
    c = a1 + b1 + a2 + b2 + a3 + b3 + a4 + b4;
}

//add: 4-4-2
template < typename T > inline void add_QQW_QQW_PA(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2)
{
    T e1;
    TwoSum(a1,b1,c1,e1);
    c2 = a2 + b2 + e1 + a3 + b3 + a4 + b4;
}

//add: 4-4-3
template < typename T > inline void add_QQW_QQW_QTW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3)
{
    T e1, e2, e3;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e3);
    c3 = a3 + b3 + e2 + e3 + a4 + b4;
}

//add: 4-4-4
template < typename T > inline void add_QQW_QQW_QQW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3, e4, e5, e6;
    TwoSum(a1,b1,c1,e1);
    TwoSum(a2,b2,c2,e2);
    TwoSum(c2,e1,c2,e3);
    TwoSum(a3,b3,c3,e4);
    TwoSum(c3,e2,c3,e5);
    TwoSum(c3,e3,c3,e6);
    c4 = a4 + b4 + e4 + e5 + e6;
}

//------------------------------
//subtraction
//------------------------------

//sub: 1-1-2
template < typename T > inline void sub_SW_SW_PA(T const& a, T const& b, T &ch, T &cl)
{
    TwoSum(a,-b,ch,cl);
}

//sub: 1-1-3
template < typename T > inline void sub_SW_SW_QTW(T const& a, T const& b, T &c1, T &c2, T &c3)
{
    add_SW_SW_QTW(a, -b, c1, c2, c3);
}

//sub: 1-1-4
template < typename T > inline void sub_SW_SW_QQW(T const& a, T const& b, T &c1, T &c2, T &c3, T &c4)
{
    add_SW_SW_QQW(a, -b, c1, c2, c3, c4);
}

//sub: 1-2-1
template < typename T > inline void sub_SW_PA_SW(T const& ah, T const& bh, T const& bl, T &c)
{
    c = ah - bh - bl;
}

//sub: 1-2-2
template < typename T > inline void sub_SW_PA_PA(T const& a, T const& bh, T const& bl, T &ch, T &cl)
{
    add_SW_PA_PA(a, -bh, -bl, ch, cl);
}

//sub: 1-2-3
template < typename T > inline void sub_SW_PA_QTW(T const& a, T const& bh, T const& bl, T &c1, T &c2, T &c3)
{
    add_SW_PA_QTW(a, -bh, -bl, c1, c2, c3);
}

//sub: 1-2-4
template < typename T > inline void sub_SW_PA_QQW(T const& a, T const& bh, T const& bl, T &c1, T &c2, T &c3, T &c4)
{
    add_SW_PA_QQW(a, -bh, -bl, c1, c2, c3, c4);
}

//sub: 1-3-1
template < typename T > inline void sub_SW_QTW_SW(T const& a, T const& b1, T const& b2, T const& b3, T &c)
{
    add_SW_QTW_SW(a, -b1, -b2, -b3, c);
}

//sub: 1-3-2
template < typename T > inline void sub_SW_QTW_PA(T const& a, T const& b1, T const& b2, T const& b3, T &c1, T &c2)
{
    add_SW_QTW_PA(a, -b1, -b2, -b3, c1, c2);
}

//sub: 1-3-3
template < typename T > inline void sub_SW_QTW_QTW(T const& a, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3)
{
    add_SW_QTW_QTW(a, -b1, -b2, -b3, c1, c2, c3);
}

//sub: 1-3-4
template < typename T > inline void sub_SW_QTW_QQW(T const& a, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3, T &c4)
{
    add_SW_QTW_QQW(a, -b1, -b2, -b3, c1, c2, c3, c4);
}

//sub: 1-4-1
template < typename T > inline void sub_SW_QQW_SW(T const& a, T const& b1, T const& b2, T const& b3, T const& b4, T &c)
{
    add_SW_QQW_SW(a, -b1, -b2, -b3, -b4, c);
}

//sub: 1-4-2
template < typename T > inline void sub_SW_QQW_PA(T const& a, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2)
{
    add_SW_QQW_PA(a, -b1, -b2, -b3, -b4, c1, c2);
}

//sub: 1-4-3
template < typename T > inline void sub_SW_QQW_QTW(T const& a, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3)
{
    add_SW_QQW_QTW(a, -b1, -b2, -b3, -b4, c1, c2, c3);
}

//sub: 1-4-4
template < typename T > inline void sub_SW_QQW_QQW(T const& a, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3, T &c4)
{
    add_SW_QQW_QQW(a, -b1, -b2, -b3, -b4, c1, c2, c3, c4);
}

//sub: 2-1-1
template < typename T > inline void sub_PA_SW_SW(T const& ah, T const& al, T const& b, T &c)
{
    c = ah - b + al;
}

//sub: 2-1-2
template < typename T > inline void sub_PA_SW_PA(T const& ah, T const& al, T const& b, T &ch, T &cl)
{
    add_PA_SW_PA(ah, al, -b, ch, cl);
}

//sub: 2-1-3
template < typename T > inline void sub_PA_SW_QTW(T const& ah, T const& al, T const& b, T &c1, T &c2, T &c3)
{
    add_PA_SW_QTW(ah, al, -b, c1, c2, c3);
}

//sub: 2-1-4
template < typename T > inline void sub_PA_SW_QQW(T const& ah, T const& al, T const& b, T &c1, T &c2, T &c3, T &c4)
{
    add_PA_SW_QQW(ah, al, -b, c1, c2, c3, c4);
}

//sub: 2-2-1
template < typename T > inline void sub_PA_PA_SW(T const& ah, T const& al, T const& bh, T const& bl, T &c)
{
    T t;
    TwoSum(ah,-bh,c,t);
    c = c + al - bl + t;
}

//sub: 2-2-2
template < typename T > inline void sub_PA_PA_PA(T const& ah, T const& al, T const& bh, T const& bl, T &ch, T &cl)
{
    add_PA_PA_PA(ah, al, -bh, -bl, ch, cl);
}

//sub: 2-2-3
template < typename T > inline void sub_PA_PA_QTW(T const& ah, T const& al, T const& bh, T const& bl, T &c1, T &c2, T &c3)
{
    add_PA_PA_QTW(ah, al, -bh, -bl, c1, c2, c3);
}

//sub: 2-2-4
template < typename T > inline void sub_PA_PA_QQW(T const& ah, T const& al, T const& bh, T const& bl, T &c1, T &c2, T &c3, T &c4)
{
    add_PA_PA_QQW(ah, al, -bh, -bl, c1, c2, c3, c4);
}

//sub: 2-3-1
template < typename T > inline void sub_PA_QTW_SW(T const& ah, T const& al, T const& b1, T const& b2, T const& b3, T &c)
{
    add_PA_QTW_SW(ah, al, -b1, -b2, -b3, c);
}

//sub: 2-3-2
template < typename T > inline void sub_PA_QTW_PA(T const& ah, T const& al, T const& b1, T const& b2, T const& b3, T &c1, T &c2)
{
    add_PA_QTW_PA(ah, al, -b1, -b2, -b3, c1, c2);
}

//sub: 2-3-3
template < typename T > inline void sub_PA_QTW_QTW(T const& ah, T const& al, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3)
{
    add_PA_QTW_QTW(ah, al, -b1, -b2, -b3, c1, c2, c3);
}

//sub: 2-3-4
template < typename T > inline void sub_PA_QTW_QQW(T const& ah, T const& al, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3, T &c4)
{
    add_PA_QTW_QQW(ah, al, -b1, -b2, -b3, c1, c2, c3, c4);
}

//sub: 2-4-1
template < typename T > inline void sub_PA_QQW_SW(T const& ah, T const& al, T const& b1, T const& b2, T const& b3, T const& b4, T &c)
{
    add_PA_QQW_SW(ah, al, -b1, -b2, -b3, -b4, c);
}

//sub: 2-4-2
template < typename T > inline void sub_PA_QQW_PA(T const& ah, T const& al, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2)
{
    add_PA_QQW_PA(ah, al, -b1, -b2, -b3, -b4, c1, c2);
}

//sub: 2-4-3
template < typename T > inline void sub_PA_QQW_QTW(T const& ah, T const& al, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3)
{
    add_PA_QQW_QTW(ah, al, -b1, -b2, -b3, -b4, c1, c2, c3);
}

//sub: 2-4-4
template < typename T > inline void sub_PA_QQW_QQW(T const& ah, T const& al, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3, T &c4)
{
    add_PA_QQW_QQW(ah, al, -b1, -b2, -b3, -b4, c1, c2, c3, c4);
}

//sub: 3-1-1
template < typename T > inline void sub_QTW_SW_SW(T const& a1, T const& a2, T const& a3, T const& b, T &c)
{
    add_QTW_SW_SW(a1, a2, a3, -b, c);
}

//sub: 3-1-2
template < typename T > inline void sub_QTW_SW_PA(T const& a1, T const& a2, T const& a3, T const& b, T &c1, T &c2)
{
    add_QTW_SW_PA(a1, a2, a3, -b, c1, c2);
}

//sub: 3-1-3
template < typename T > inline void sub_QTW_SW_QTW(T const& a1, T const& a2, T const& a3, T const& b, T &c1, T &c2, T &c3)
{
    add_QTW_SW_QTW(a1, a2, a3, -b, c1, c2, c3);
}

//sub: 3-1-4
template < typename T > inline void sub_QTW_SW_QQW(T const& a1, T const& a2, T const& a3, T const& b, T &c1, T &c2, T &c3, T &c4)
{
    add_QTW_SW_QQW(a1, a2, a3, -b, c1, c2, c3, c4);
}

//sub: 3-2-1
template < typename T > inline void sub_QTW_PA_SW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T &c)
{
    add_QTW_PA_SW(a1, a2, a3, -b1, -b2, c);
}

//sub: 3-2-2
template < typename T > inline void sub_QTW_PA_PA(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T &c1, T &c2)
{
    add_QTW_PA_PA(a1, a2, a3, -b1, -b2, c1, c2);
}

//sub: 3-2-3
template < typename T > inline void sub_QTW_PA_QTW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T &c1, T &c2, T &c3)
{
    add_QTW_PA_QTW(a1, a2, a3, -b1, -b2, c1, c2, c3);
}

//sub: 3-2-4
template < typename T > inline void sub_QTW_PA_QQW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T &c1, T &c2, T &c3, T &c4)
{
    add_QTW_PA_QQW(a1, a2, a3, -b1, -b2, c1, c2, c3, c4);
}

//sub: 3-3-1
template < typename T > inline void sub_QTW_QTW_SW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T &c)
{
    add_QTW_QTW_SW(a1, a2, a3, -b1, -b2, -b3, c);
}

//sub: 3-3-2
template < typename T > inline void sub_QTW_QTW_PA(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T &c1, T &c2)
{
    add_QTW_QTW_PA(a1, a2, a3, -b1, -b2, -b3, c1, c2);
}

//sub: 3-3-3
template < typename T > inline void sub_QTW_QTW_QTW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3)
{
    add_QTW_QTW_QTW(a1, a2, a3, -b1, -b2, -b3, c1, c2, c3);
}

//sub: 3-3-4
template < typename T > inline void sub_QTW_QTW_QQW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3, T &c4)
{
    add_QTW_QTW_QQW(a1, a2, a3, -b1, -b2, -b3, c1, c2, c3, c4);
}


//sub: 3-4-1
template < typename T > inline void sub_QTW_QQW_SW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T const& b4, T &c)
{
    add_QTW_QQW_SW(a1, a2, a3, -b1, -b2, -b3, -b4, c);
}

//sub: 3-4-2
template < typename T > inline void sub_QTW_QQW_PA(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2)
{
    add_QTW_QQW_PA(a1, a2, a3, -b1, -b2, -b3, -b4, c1, c2);
}

//sub: 3-4-3
template < typename T > inline void sub_QTW_QQW_QTW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3)
{
    add_QTW_QQW_QTW(a1, a2, a3, -b1, -b2, -b3, -b4, c1, c2, c3);
}

//sub: 3-4-4
template < typename T > inline void sub_QTW_QQW_QQW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3, T &c4)
{
    add_QTW_QQW_QQW(a1, a2, a3, -b1, -b2, -b3, -b4, c1, c2, c3, c4);
}

//sub: 4-1-1
template < typename T > inline void sub_QQW_SW_SW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b, T &c)
{
    add_QQW_SW_SW(a1, a2, a3, a4, -b, c);
}

//sub: 4-1-2
template < typename T > inline void sub_QQW_SW_PA(T const& a1, T const& a2, T const& a3, T const& a4, T const& b, T &c1, T &c2)
{
    add_QQW_SW_PA(a1, a2, a3, a4, -b, c1, c2);
}

//sub: 4-1-3
template < typename T > inline void sub_QQW_SW_QTW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b, T &c1, T &c2, T &c3)
{
    add_QQW_SW_QTW(a1, a2, a3, a4, -b, c1, c2, c3);
}

//sub: 4-1-4
template < typename T > inline void sub_QQW_SW_QQW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b, T &c1, T &c2, T &c3, T &c4)
{
    add_QQW_SW_QQW(a1, a2, a3, a4, -b, c1, c2, c3, c4);
}


//sub: 4-2-1
template < typename T > inline void sub_QQW_PA_SW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T &c)
{
    add_QQW_PA_SW(a1, a2, a3, a4, -b1, -b2, c);
}

//sub: 4-2-2
template < typename T > inline void sub_QQW_PA_PA(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T &c1, T &c2)
{
    add_QQW_PA_PA(a1, a2, a3, a4, -b1, -b2, c1, c2);
}

//sub: 4-2-3
template < typename T > inline void sub_QQW_PA_QTW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T &c1, T &c2, T &c3)
{
    add_QQW_PA_QTW(a1, a2, a3, a4, -b1, -b2, c1, c2, c3);
}

//sub: 4-2-4
template < typename T > inline void sub_QQW_PA_QQW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T &c1, T &c2, T &c3, T &c4)
{
    add_QQW_PA_QQW(a1, a2, a3, a4, -b1, -b2, c1, c2, c3, c4);
}

//sub: 4-3-1
template < typename T > inline void sub_QQW_QTW_SW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T &c)
{
    add_QQW_QTW_SW(a1, a2, a3, a4, -b1, -b2, -b3, c);
}

//sub: 4-3-2
template < typename T > inline void sub_QQW_QTW_PA(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T &c1, T &c2)
{
    add_QQW_QTW_PA(a1, a2, a3, a4, -b1, -b2, -b3, c1, c2);
}

//sub: 4-3-3
template < typename T > inline void sub_QQW_QTW_QTW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3)
{
    add_QQW_QTW_QTW(a1, a2, a3, a4, -b1, -b2, -b3, c1, c2, c3);
}

//sub: 4-3-4
template < typename T > inline void sub_QQW_QTW_QQW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3, T &c4)
{
    add_QQW_QTW_QQW(a1, a2, a3, a4, -b1, -b2, -b3, c1, c2, c3, c4);
}

//sub: 4-4-1
template < typename T > inline void sub_QQW_QQW_SW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T &c)
{
    add_QQW_QQW_SW(a1, a2, a3, a4, -b1, -b2, -b3, -b4, c);
}

//sub: 4-4-2
template < typename T > inline void sub_QQW_QQW_PA(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2)
{
    add_QQW_QQW_PA(a1, a2, a3, a4, -b1, -b2, -b3, -b4, c1, c2);
}

//sub: 4-4-3
template < typename T > inline void sub_QQW_QQW_QTW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3)
{
    add_QQW_QQW_QTW(a1, a2, a3, a4, -b1, -b2, -b3, -b4, c1, c2, c3);
}

//sub: 4-4-4
template < typename T > inline void sub_QQW_QQW_QQW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3, T &c4)
{
    add_QQW_QQW_QQW(a1, a2, a3, a4, -b1, -b2, -b3, -b4, c1, c2, c3, c4);
}

//------------------------------
//multiplication
//------------------------------

//mul: 1-1-2
template < typename T > inline void mul_SW_SW_PA(T const& a, T const& b, T &ch, T &cl)
{
    TwoProductFMA(a,b,ch,cl);
}

//mul: 1-1-3
template < typename T > inline void mul_SW_SW_QTW(T const& a, T const& b, T &c1, T &c2, T &c3)
{
    TwoProductFMA(a, b, c1, c2);
    c3 = fp_const<T>::zero();
}

//mul: 1-1-4
template < typename T > inline void mul_SW_SW_QQW(T const& a, T const& b, T &c1, T &c2, T &c3, T &c4)
{
    TwoProductFMA(a,b,c1,c2);
    c3 = fp_const<T>::zero();
    c4 = fp_const<T>::zero();
}

//mul: 1-2-1
template < typename T > inline void mul_SW_PA_SW(T const& a, T const& bh, T const& bl, T &c)
{
    c = a*(bh + bl);
}

//mul: 1-2-2
template < typename T > inline void mul_SW_PA_PA(T const& a, T const& bh, T const& bl, T &ch, T &cl)
{
    T t;
    TwoProductFMA(a,bh,ch,t);
    cl = std::fma(a,bl,t);
}

//mul: 1-2-3
template < typename T > inline void mul_SW_PA_QTW(T const& a, T const& b1, T const& b2, T &c1, T &c2, T &c3)
{
    T e1, e3, e5, t3;
    TwoProductFMA(a, b1, c1, e1);
    TwoProductFMA(a, b2, t3, e3);
    TwoSum(t3, e1, c2, e5);
    c3 = e3 + e5;
}

//mul: 1-2-4
template < typename T > inline void mul_SW_PA_QQW(T const& a, T const& b1, T const& b2, T &c1, T &c2, T &c3, T &c4)
{
    T t1, e1, e2, e3;
    TwoProductFMA(a,b1,c1,e1);
    TwoProductFMA(a,b2,t1,e2);
    TwoSum(e1,t1,c2,e3);
    c3 = e2 + e3;
    c4 = fp_const<T>::zero();
}

//mul: 1-3-1
template < typename T > inline void mul_SW_QTW_SW(T const& a, T const& b1, T const& b2, T const& b3, T &c)
{
    c = a * (b3 + b2 + b1);
}

//mul: 1-3-2
template < typename T > inline void mul_SW_QTW_PA(T const& a, T const& b1, T const& b2, T const& b3, T &c1, T &c2)
{
    T e1;
    TwoProductFMA(a, b1, c1, e1);
    c2 = std::fma(a, b2 + b3, e1);
}

//mul: 1-3-3
template < typename T > inline void mul_SW_QTW_QTW(T const& a, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3)
{
    T e1, e2, e4, e5, t2;
    TwoProductFMA(a, b1, c1, e1);
    TwoProductFMA(a, b2, t2, e2);
    TwoSum(t2, e1, c2, e5);
    c3 = e2 + std::fma(a,b3,e5);
}

//mul: 1-3-4
template < typename T > inline void mul_SW_QTW_QQW(T const& a, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3;
    T e1, e2, e3, e4, e5, e6;
    TwoProductFMA(a,b1,c1,e1);
    TwoProductFMA(a,b2,t1,e2);
    TwoSum(e1,t1,c2,e3);
    TwoSum(e2,e3,t2,e4);
    TwoProductFMA(a,b3,t3,e5);
    TwoSum(t2,t3,c3,e6);
    c4 = e4 + e5 + e6;
}

//mul: 1-4-1
template < typename T > inline void mul_SW_QQW_SW(T const& a1, T const& b1, T const& b2, T const& b3, T const& b4, T &c)
{
//    c = a1 + b1 + b2 + b3 + b4;
    c = a1 * (b4 + b3 + b2 + b1); //
}

//mul: 1-4-2
template < typename T > inline void mul_SW_QQW_PA(T const& a, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2)
{
    T e1;
    TwoProductFMA(a,b1,c1,e1);
    c2 = std::fma(a,b2,e1);
}

//mul: 1-4-3
template < typename T > inline void mul_SW_QQW_QTW(T const& a, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3)
{
    T e1, e2, e5, e6;
    T t2, t4;

    TwoProductFMA(a,b1,c1,e1);
    TwoProductFMA(a,b2,t2,e2);
    TwoSum(t2,e1,c2,e5);
    TwoProductFMA(a,b3,t4,e6);
    c3 = t4 + e2 + e5;
}

//mul: 1-4-4
template < typename T > inline void mul_SW_QQW_QQW(T const& a, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e5, e6, e11, e14;
    T t2, t4;

    TwoProductFMA(a,b1,c1,e1);
    TwoProductFMA(a,b2,t2,e2);
    TwoSum(t2,e1,c2,e5);
    TwoProductFMA(a,b3,t4,e6);
    TwoSum(t4,e2,c3,e11);
    TwoSum(c3,e5,c3,e14);
    c4 = std::fma(a,b4,e6) + e11 + e14;
}

//mul: 2-1-1
template < typename T > inline void mul_PA_SW_SW(T const& ah, T const& al, T const& b, T &c)
{
    c = (ah+al)*b;
}

//mul: 2-1-2
template < typename T > inline void mul_PA_SW_PA(T const& ah, T const& al, T const& b, T &ch, T &cl)
{
    T t;
    TwoProductFMA(ah,b,ch,t);
    cl = std::fma(b,al,t);
}

//mul: 2-1-3
template < typename T > inline void mul_PA_SW_QTW(T const& a1, T const& a2, T const& b, T &c1, T &c2, T &c3)
{
    T e1, e3, e4, t3;
    TwoProductFMA(a1, b, c1, e1);
    TwoProductFMA(a2, b, t3, e3);
    TwoSum(e1, t3, c2, e4);
    c3 = e3 + e4;
}

//mul: 2-1-4
template < typename T > inline void mul_PA_SW_QQW(T const& a1, T const& a2, T const& b, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e3, e5;
    T t3;
    
    TwoProductFMA(a1,b,c1,e1);
    TwoProductFMA(a2,b,t3,e3);
    TwoSum(t3,e1,c2,e5);
    TwoSum(e3,e5,c3,c4);
}

//mul: 2-2-1
template < typename T > inline void mul_PA_PA_SW(T const& ah, T const& al, T const& bh, T const& bl, T &c)
{
    c = std::fma(al,bh,std::fma(ah,bh,ah*bl)); 
}

//mul: 2-2-2
template < typename T > inline void mul_PA_PA_PA(T const& ah, T const& al, T const& bh, T const& bl, T &ch, T &cl)
{
    T t;
    TwoProductFMA(ah,bh,ch,t);
    cl = std::fma(ah,bl,std::fma(bh,al,t));
}

//mul: 2-2-3
template < typename T > inline void mul_PA_PA_QTW(T const& a1, T const& a2, T const& b1, T const& b2, T &c1, T &c2, T &c3)
{
    T e1, e2, e3, e4, e5, t2, t3;
    TwoProductFMA(a1, b1, c1, e1);
    TwoProductFMA(a1, b2, t2, e2);
    TwoProductFMA(a2, b1, t3, e3);
    TwoSum(t2, t3, c2, e4);
    TwoSum(c2, e1, c2, e5);
    c3 = e2 + std::fma(a2,b2,e3) + e4 + e5;
}

//mul: 2-2-4
template < typename T > inline void mul_PA_PA_QQW(T const& a1, T const& a2, T const& b1, T const& b2, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3, e4, e5;
    T e7, e11, e12, e13, e14;
    T t2, t3, t5;
    
    TwoProductFMA(a1,b1,c1,e1);
    TwoProductFMA(a1,b2,t2,e2);
    TwoProductFMA(a2,b1,t3,e3);
    TwoSum(t2,t3,c2,e4);
    TwoSum(c2,e1,c2,e5);
    TwoProductFMA(a2,b2,t5,e7);
    TwoSum(t5,e2,c3,e11);
    TwoSum(c3,e3,c3,e12);
    TwoSum(c3,e4,c3,e13);
    TwoSum(c3,e5,c3,e14);
    c4 = e7 + e11 + e12 + e13 + e14;
}

//mul: 2-3-1
template < typename T > inline void mul_PA_QTW_SW(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T &c)
{
    c = (a1+a2)*(b3+b2+b1);
}

//mul: 2-3-2
template < typename T > inline void mul_PA_QTW_PA(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T &c1, T &c2)
{
    T e1;
    TwoProductFMA(a1, b1, c1, e1);
    c2 = std::fma(a2,b1,std::fma(a1,b2,e1));
}

//mul: 2-3-3
template < typename T > inline void mul_PA_QTW_QTW(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3)
{
    T e1, e2, e3, e4, e5, t2, t3;
    TwoProductFMA(a1, b1, c1, e1);
    TwoProductFMA(a1, b2, t2, e2);
    TwoProductFMA(a2, b1, t3, e3);
    TwoSum(t2, t3, c2, e4);
    TwoSum(c2, e1, c2, e5);
    c3 = e2 + std::fma(a2,b2,e3) + std::fma(a1,b3,e4) + e5;
}

//mul: 2-3-4
template < typename T > inline void mul_PA_QTW_QQW(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3, e4, e5;
    T e6, e7, e9, e11, e12, e13, e14;
    T t2, t3, t4, t5;
    
    TwoProductFMA(a1,b1,c1,e1);
    TwoProductFMA(a1,b2,t2,e2);
    TwoProductFMA(a2,b1,t3,e3);
    TwoSum(t2,t3,c2,e4);
    TwoSum(c2,e1,c2,e5);
    TwoProductFMA(a1,b3,t4,e6);
    TwoProductFMA(a2,b2,t5,e7);
    TwoSum(t4,t5,c3,e9);
    TwoSum(c3,e2,c3,e11);
    TwoSum(c3,e3,c3,e12);
    TwoSum(c3,e4,c3,e13);
    TwoSum(c3,e5,c3,e14);
    c4 = std::fma(a2,b3,e6) + e7 + e9 + e11 + e12 + e13 + e14;
}

//mul: 2-4-1
template < typename T > inline void mul_PA_QQW_SW(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T const& b4, T &c)
{
//    c = a1 + b1 + a2 + b2 + b3 + b4;
    c = (a1 + a2) * (b1 + b2 + b3 + b4); //
}

//mul: 2-4-2
template < typename T > inline void mul_PA_QQW_PA(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2)
{
    T e1;
    TwoProductFMA(a1,b1,c1,e1);
    c2 = std::fma(a1,b2,std::fma(a2,b1,e1));
}

//mul: 2-4-3
template < typename T > inline void mul_PA_QQW_QTW(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3)
{
    T e1, e2, e3, e4, e5;
    T t2, t3;
    TwoProductFMA(a1,b1,c1,e1);
    TwoProductFMA(a1,b2,t2,e2);
    TwoProductFMA(a2,b1,t3,e3);
    TwoSum(t2,t3,c2,e4);
    TwoSum(c2,e1,c2,e5);
    c3 = std::fma(a1,b3,std::fma(a2,b2,e2)) + e3 + e4 + e5;
}

//mul: 2-4-4
template < typename T > inline void mul_PA_QQW_QQW(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3, e4, e5;
    T e6, e7, e9;
    T e11, e12, e13, e14;
    T t2, t3, t4, t5;

    TwoProductFMA(a1,b1,c1,e1);
    TwoProductFMA(a1,b2,t2,e2);
    TwoProductFMA(a2,b1,t3,e3);
    TwoSum(t2,t3,c2,e4);
    TwoSum(c2,e1,c2,e5);
    TwoProductFMA(a1,b3,t4,e6);
    TwoProductFMA(a2,b2,t5,e7);
    TwoSum(t4,t5,c3,e9);
    TwoSum(c3,e2,c3,e11);
    TwoSum(c3,e3,c3,e12);
    TwoSum(c3,e4,c3,e13);
    TwoSum(c3,e5,c3,e14);
    c4 = std::fma(a2,b3,a1*b4) + e6 + e7 + e9 + e11 + e12 + e13 + e14;
}

//mul: 3-1-1
template < typename T > inline void mul_QTW_SW_SW(T const& a1, T const& a2, T const& a3, T const& b, T &c)
{
    c = (a3+a2+a1)*b;
}

//mul: 3-1-2
template < typename T > inline void mul_QTW_SW_PA(T const& a1, T const& a2, T const& a3, T const& b, T &c1, T &c2)
{
    T e1;
    TwoProductFMA(a1, b, c1, e1);
    c2 = std::fma(a2,b,e1);
}

//mul: 3-1-3
template < typename T > inline void mul_QTW_SW_QTW(T const& a1, T const& a2, T const& a3, T const& b, T &c1, T &c2, T &c3)
{
    T e1, e3, e5;
    T t3;
    TwoProductFMA(a1, b, c1, e1);
    TwoProductFMA(a2, b, t3, e3);
    TwoSum(t3, e1, c2, e5);
    c3 = std::fma(a3,b,e3) + e5;
}

//mul: 3-1-4
template < typename T > inline void mul_QTW_SW_QQW(T const& a1, T const& a2, T const& a3, T const& b, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e3, e5, e8, e12, e14;
    T t3, t4, t5, t6;
    
    TwoProductFMA(a1,b,c1,e1);
    TwoProductFMA(a2,b,t3,e3);
    TwoSum(t3,e1,c2,e5);
    TwoProductFMA(a3,b,t6,e8);
    TwoSum(t6,e3,c3,e12);
    TwoSum(c3,e5,c3,e14);
    c4 = e8 + e12 + e14;
}

//mul: 3-2-1
template < typename T > inline void mul_QTW_PA_SW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T &c)
{
    c = (a3+a2+a1)*(b1+b2);
}

//mul: 3-2-2
template < typename T > inline void mul_QTW_PA_PA(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T &c1, T &c2)
{
    T e1;
    TwoProductFMA(a1, b1, c1, e1);
    c2 = std::fma(a2+a3,b1,std::fma(a1,b2,e1));
}

//mul: 3-2-3
template < typename T > inline void mul_QTW_PA_QTW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T &c1, T &c2, T &c3)
{
    T e1, e2, e3, e4, e5, t2, t3;
    TwoProductFMA(a1, b1, c1, e1);
    TwoProductFMA(a1, b2, t2, e2);
    TwoProductFMA(a2, b1, t3, e3);
    TwoSum(t2, t3, c2, e4);
    TwoSum(c2, e1, c2, e5);
    c3 = std::fma(a3,b1,e2) + std::fma(a2,b2,e3) + e4 + e5;
}

//mul: 3-2-4
template < typename T > inline void mul_QTW_PA_QQW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3, e4, e5;
    T e7, e8, e10;
    T e11, e12, e13, e14;
    T t2, t3, t5, t6;
    
    TwoProductFMA(a1,b1,c1,e1);
    TwoProductFMA(a1,b2,t2,e2);
    TwoProductFMA(a2,b1,t3,e3);
    TwoSum(t2,t3,c2,e4);
    TwoSum(c2,e1,c2,e5);
    TwoProductFMA(a2,b2,t5,e7);
    TwoProductFMA(a3,b1,t6,e8);
    TwoSum(t5,t6,c3,e10);
    TwoSum(c3,e2,c3,e11);
    TwoSum(c3,e3,c3,e12);
    TwoSum(c3,e4,c3,e13);
    TwoSum(c3,e5,c3,e14);
    c4 = std::fma(a3,b2,e7) + e8 + e10 + e11 + e12 + e13 + e14;
}

//mul: 3-3-1
template < typename T > inline void mul_QTW_QTW_SW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T &c)
{
    c = (a1+a2+a3)*(b1+b2+b3); //減らしてもよい
}

//mul: 3-3-2
template < typename T > inline void mul_QTW_QTW_PA(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T &c1, T &c2)
{
    T e1;
    TwoProductFMA(a1, b1, c1, e1);
    c2 = std::fma(a2,b1,std::fma(a1,b2,e1));
}

//mul: 3-3-3
template < typename T > inline void mul_QTW_QTW_QTW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3)
{
    T e1, e2, e3, e4, e5;
    T t2, t3;
    TwoProductFMA(a1, b1, c1, e1);
    TwoProductFMA(a1, b2, t2, e2);
    TwoProductFMA(a2, b1, t3, e3);
    TwoSum(t2, t3, c2, e4);
    TwoSum(c2, e1, c2, e5);
    c3 = std::fma(a3,b1,e2) + std::fma(a2,b2,e3) + std::fma(a1,b3,e4) + e5;
}

//mul: 3-3-4
template < typename T > inline void mul_QTW_QTW_QQW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3, e4, e5;
    T e6, e7, e8, e9, e10;
    T e11, e12, e13, e14;
    T t2, t3, t4, t5, t6;
    
    TwoProductFMA(a1,b1,c1,e1);
    TwoProductFMA(a1,b2,t2,e2);
    TwoProductFMA(a2,b1,t3,e3);
    TwoSum(t2,t3,c2,e4);
    TwoSum(c2,e1,c2,e5);
    TwoProductFMA(a1,b3,t4,e6);
    TwoProductFMA(a2,b2,t5,e7);
    TwoProductFMA(a3,b1,t6,e8);
    TwoSum(t4,t5,c3,e9);
    TwoSum(c3,t6,c3,e10);
    TwoSum(c3,e2,c3,e11);
    TwoSum(c3,e3,c3,e12);
    TwoSum(c3,e4,c3,e13);
    TwoSum(c3,e5,c3,e14);
    c4 = std::fma(a3,b2,std::fma(a2,b3,e6)) + e7 + e8 + e9 + e10 + e11 + e12 + e13 + e14;
}

//mul: 3-4-1
template < typename T > inline void mul_QTW_QQW_SW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T const& b4, T &c)
{
//    c = a1 + b1 + a2 + b2 + a3 + b3 + b4;
    c = (a3 + a2 + a1) * (b4 + b3 + b2 + b1); //
}

//mul: 3-4-2
template < typename T > inline void mul_QTW_QQW_PA(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2)
{
    T e1;
    TwoProductFMA(a1,b1,c1,e1);
    c2 = std::fma(a1,b2,std::fma(a2,b1,e1));
}

//mul: 3-4-3
template < typename T > inline void mul_QTW_QQW_QTW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3)
{
    T e1, e2, e3, e4, e5;
    T t2, t3;

    TwoProductFMA(a1,b1,c1,e1);
    TwoProductFMA(a1,b2,t2,e2);
    TwoProductFMA(a2,b1,t3,e3);
    TwoSum(t2,t3,c2,e4);
    TwoSum(c2,e1,c2,e5);
    c3 = std::fma(a3,b1,std::fma(a1,b3,a2*b2)) + e2 + e3 + e4 + e5;
}

//mul: 3-4-4
template < typename T > inline void mul_QTW_QQW_QQW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3, e4, e5;
    T e6, e7, e8, e9, e10;
    T e11, e12, e13, e14;
    T t2, t3, t4, t5, t6;

    TwoProductFMA(a1,b1,c1,e1);
    TwoProductFMA(a1,b2,t2,e2);
    TwoProductFMA(a2,b1,t3,e3);
    TwoSum(t2,t3,c2,e4);
    TwoSum(c2,e1,c2,e5);
    TwoProductFMA(a1,b3,t4,e6);
    TwoProductFMA(a2,b2,t5,e7);
    TwoProductFMA(a3,b1,t6,e8);
    TwoSum(t4,t5,c3,e9);
    TwoSum(c3,t6,c3,e10);
    TwoSum(c3,e2,c3,e11);
    TwoSum(c3,e3,c3,e12);
    TwoSum(c3,e4,c3,e13);
    TwoSum(c3,e5,c3,e14);
    c4 = std::fma(a3,b2,std::fma(a2,b3,a1*b4)) + e6 + e7 + e8 + e9 + e10 + e11 + e12 + e13 + e14;
}

//mul: 4-1-1
template < typename T > inline void mul_QQW_SW_SW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b, T &c)
{
//    c = a1 + b + a2  + a3 + a4;
    c = (a4 + a3  + a2 + a1) * b; //
}

//mul: 4-1-2
template < typename T > inline void mul_QQW_SW_PA(T const& a1, T const& a2, T const& a3, T const& a4, T const& b, T &c1, T &c2)
{
    T e1;
    TwoProductFMA(a1,b,c1,e1);
    c2 = std::fma(a2,b,e1);
}

//mul: 4-1-3
template < typename T > inline void mul_QQW_SW_QTW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b, T &c1, T &c2, T &c3)
{
    T t3;
    T e1, e3, e5;
    TwoProductFMA(a1,b,c1,e1);
    TwoProductFMA(a2,b,t3,e3);
    TwoSum(t3,e1,c2,e5);
    c3 = std::fma(a3,b,e3) + e5;
}

//mul: 4-1-4
template < typename T > inline void mul_QQW_SW_QQW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e3, e5;
    T e8, e12, e14;
    T t2, t3, t4, t5, t6;
    
    TwoProductFMA(a1,b,c1,e1);
    TwoProductFMA(a2,b,t3,e3);
    TwoSum(c2,e1,c2,e5);
    TwoProductFMA(a3,b,t6,e8);
    TwoSum(t6,e3,c3,e12);
    TwoSum(c3,e5,c3,e14);
    c4 = std::fma(a4,b,e8) + e12 + e14;
}

//mul: 4-2-1
template < typename T > inline void mul_QQW_PA_SW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T &c)
{
//    c = a1 + b1 + a2 + b2 + a3 + a4;
    c = (a4 + a3 + a2 + a1) * (b1 + b2);
}

//mul: 4-2-2
template < typename T > inline void mul_QQW_PA_PA(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T &c1, T &c2)
{
    T e1;
    TwoProductFMA(a1,b1,c1,e1);
    c2 = std::fma(a1,b2,std::fma(a2,b1,e1));
}

//mul: 4-2-3
template < typename T > inline void mul_QQW_PA_QTW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T &c1, T &c2, T &c3)
{
    T e1, e2, e3, e4, e5;
    T t2, t3;
    
    TwoProductFMA(a1,b1,c1,e1);
    TwoProductFMA(a1,b2,t2,e2);
    TwoProductFMA(a2,b1,t3,e3);
    TwoSum(t2,t3,c2,e4);
    TwoSum(c2,e1,c2,e5);
    c3 = std::fma(a2,b2,e2) + std::fma(a3,b1,e3) + e4 + e5;
}

//mul: 4-2-4
template < typename T > inline void mul_QQW_PA_QQW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3, e4, e5;
    T e7, e8, e10;
    T e11, e12, e13, e14;
    T t2, t3, t5, t6;
    
    TwoProductFMA(a1,b1,c1,e1);
    TwoProductFMA(a1,b2,t2,e2);
    TwoProductFMA(a2,b1,t3,e3);
    TwoSum(t2,t3,c2,e4);
    TwoSum(c2,e1,c2,e5);
    TwoProductFMA(a2,b2,t5,e7);
    TwoProductFMA(a3,b1,t6,e8);
    TwoSum(t5,t6,c3,e10);
    TwoSum(c3,e2,c3,e11);
    TwoSum(c3,e3,c3,e12);
    TwoSum(c3,e4,c3,e13);
    TwoSum(c3,e5,c3,e14);
    c4 = std::fma(a4,b1,std::fma(a3,b2,e7)) + e8 + e10 + e11 + e12 + e13 + e14;
}

//mul: 4-3-1
template < typename T > inline void mul_QQW_QTW_SW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T &c)
{
//    c = a1 + b1 + a2 + b2 + a3 + b3 + a4;
    c = (a4 + a3 + a2 + a1) * (b3 + b2 + b1);
}

//mul: 4-3-2
template < typename T > inline void mul_QQW_QTW_PA(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T &c1, T &c2)
{
    T e1;
    TwoProductFMA(a1,b1,c1,e1);
    c2 = std::fma(a1,b2,std::fma(a2,b1,e1));
}

//mul: 4-3-3
template < typename T > inline void mul_QQW_QTW_QTW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3)
{
    T e1, e2, e3, e4, e5;
    T t2, t3;
    
    TwoProductFMA(a1,b1,c1,e1);
    TwoProductFMA(a1,b2,t2,e2);
    TwoProductFMA(a2,b1,t3,e3);
    TwoSum(t2,t3,c2,e4);
    TwoSum(c2,e1,c2,e5);
    c3 = std::fma(a1,b3,e2) + std::fma(a2,b2,e3) + std::fma(a3,b1,e4) + e5;
}

//mul: 4-3-4
template < typename T > inline void mul_QQW_QTW_QQW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3, e4, e5;
    T e6, e7, e8, e9, e10;
    T e11, e12, e13, e14;
    T t2, t3, t4, t5, t6;
    
    TwoProductFMA(a1,b1,c1,e1);
    TwoProductFMA(a1,b2,t2,e2);
    TwoProductFMA(a2,b1,t3,e3);
    TwoSum(t2,t3,c2,e4);
    TwoSum(c2,e1,c2,e5);
    TwoProductFMA(a1,b3,t4,e6);
    TwoProductFMA(a2,b2,t5,e7);
    TwoProductFMA(a3,b1,t6,e8);
    TwoSum(t4,t5,c3,e9);
    TwoSum(c3,t6,c3,e10);
    TwoSum(c3,e2,c3,e11);
    TwoSum(c3,e3,c3,e12);
    TwoSum(c3,e4,c3,e13);
    TwoSum(c3,e5,c3,e14);
    c4 = std::fma(a4,b1,std::fma(a3,b2,std::fma(a2,b3,e6))) + e7 + e8 + e9 + e10 + e11 + e12 + e13 + e14;
}

//mul: 4-4-1
template < typename T > inline void mul_QQW_QQW_SW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T &c)
{
//    c = a1 + b1 + a2 + b2 + a3 + b3 + a4 + b4;
    c = (a4 + a3 + a2 + a1) * (b4 + b3 + b2 + b1);
}

//mul: 4-4-2
template < typename T > inline void mul_QQW_QQW_PA(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2)
{
    T e1;
    TwoProductFMA(a1,b1,c1,e1);
    c2 = std::fma(a1,b2,std::fma(a2,b1,e1));
}

//mul: 4-4-3
template < typename T > inline void mul_QQW_QQW_QTW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3)
{
    T e1, e2, e3, e4, e5;
    T t2, t3;
    
    TwoProductFMA(a1,b1,c1,e1);
    TwoProductFMA(a1,b2,t2,e2);
    TwoProductFMA(a2,b1,t3,e3);
    TwoSum(t2,t3,c2,e4);
    TwoSum(c2,e1,c2,e5);
    c3 = std::fma(a1,b3,e2) + std::fma(a2,b2,e3) + std::fma(a3,b1,e4) + e5;
}

//mul: 4-4-4
template < typename T > inline void mul_QQW_QQW_QQW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e3, e4, e5;
    T e6, e7, e8, e9, e10;
    T e11, e12, e13, e14;
    T t2, t3, t4, t5, t6;
    
    TwoProductFMA(a1,b1,c1,e1);
    TwoProductFMA(a1,b2,t2,e2);
    TwoProductFMA(a2,b1,t3,e3);
    TwoSum(t2,t3,c2,e4);
    TwoSum(c2,e1,c2,e5);
    TwoProductFMA(a1,b3,t4,e6);
    TwoProductFMA(a2,b2,t5,e7);
    TwoProductFMA(a3,b1,t6,e8);
    TwoSum(t4,t5,c3,e9);
    TwoSum(c3,t6,c3,e10);
    TwoSum(c3,e2,c3,e11);
    TwoSum(c3,e3,c3,e12);
    TwoSum(c3,e4,c3,e13);
    TwoSum(c3,e5,c3,e14);
    c4 = std::fma(a4,b1,std::fma(a3,b2,std::fma(a2,b3,a1*b4))) + e6 + e7 + e8 + e9 + e10 + e11 + e12 + e13 + e14;
}

//------------------------------
//Division
//------------------------------

//div: 1-1-2
template < typename T > inline void div_SW_SW_PA(T const& a, T const& b, T &ch, T &cl)
{
    ch = a/b;
    cl = std::fma(-b,ch,a)/b;
}

//div: 1-1-3
template < typename T > inline void div_SW_SW_QTW(T const& a, T const& b, T &c1, T &c2, T &c3)
{
    T t1, t2, t3;
    div_SW_SW_PA(a,b,c1,c2); //// 3-3
    mul_PA_SW_QTW(c1,c2,b,t1,t2,t3);  //// 11-7
    add_SW_QTW_QTW(a,-t1,-t2,-t3,t1,t2,t3); //// 13-5
    c3 = (t1 + t2 + t3) / b;
}

//div: 1-1-4
template < typename T > inline void div_SW_SW_QQW(T const& a, T const& b, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    div_SW_SW_QTW(a,b,c1,c2,c3); //// 30-17
    mul_QTW_SW_QQW(c1,c2,c3,b,t1,t2,t3,t4); //// 12-8
    add_SW_QQW_QQW(a,-t1,-t2,-t3,-t4,t1,t2,t3,t4); ////  19-7
    c4 = (t1 + t2 + t3 + t4) / b;
}

//div: 1-2-1
template < typename T > inline void div_SW_PA_SA(T const& a, T const& bh, T const& bl, T &c)
{
    c = a/(bh+bl);
}

//div: 1-2-2
template < typename T > inline void div_SW_PA_PA(T const& a, T const& bh, T const& bl, T &ch, T &cl)
{
    T t;
    ch = a/bh;
    t = std::fma(-bh,ch,a);
    cl= std::fma(ch,-bl,t)/(bh + bl);
}

//div: 1-2-3
template < typename T > inline void div_SW_PA_QTW(T const& a, T const& b1, T const& b2, T &c1, T &c2, T &c3)
{
    T t1, t2, t3;
    div_SW_PA_PA(a,b1,b2,c1,c2); //// 5-5
    mul_PA_PA_QTW(c1,c2,b1,b2,t1,t2,t3);  //// 20-13
    add_SW_QTW_QTW(a,-t1,-t2,-t3,t1,t2,t3); //// 13-5
    c3 = (t1 + t2 + t3) / (b1 + b2);
}

//div: 1-2-4
template < typename T > inline void div_SW_PA_QQW(T const& a, T const& b1, T const& b2, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    div_SW_PA_QTW(a,b1,b2,c1,c2,c3); //// 42-26
    mul_QTW_PA_QQW(c1,c2,c3,b1,b2,t1,t2,t3,t4); //// 59-28
    add_SW_QQW_QQW(a,-t1,-t2,-t3,-t4,t1,t2,t3,t4); //// 19-7
    c4 = (t1 + t2 + t3 + t4) / (b1 + b2);
}

//div: 1-3-1
template < typename T > inline void div_SW_QTW_SW(T const& a, T const& b1, T const& b2, T const& b3, T &c)
{
    c = a/(b1+b2+b3);
}

//div: 1-3-2
template < typename T > inline void div_SW_QTW_PA(T const& a, T const& b1, T const& b2, T const& b3, T &c1, T &c2)
{
    div_SW_PA_PA(a,b1,b2+b3,c1,c2);
}

//div: 1-3-3
template < typename T > inline void div_SW_QTW_QTW(T const& a, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3)
{
    T t1, t2, t3;
    div_SW_PA_PA(a,b1,b2,c1,c2); //// 5-5
    mul_PA_QTW_QTW(c1,c2,b1,b2,b3,t1,t2,t3); //// 21-14
    add_SW_QTW_QTW(a,-t1,-t2,-t3,t1,t2,t3); //// 13-5
    c3 = (t1 + t2 + t3) / (b1 + b2);
}

//div: 1-3-4
template < typename T > inline void div_SW_QTW_QQW(T const& a, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    div_SW_QTW_QTW(a,b1,b2,b3,c1,c2,c3); //// 43-27
    mul_QTW_QTW_QQW(c1,c2,c3,b1,b2,b3,t1,t2,t3,t4); ////70-34 
    add_SW_QQW_QQW(a,-t1,-t2,-t3,-t4,t1,t2,t3,t4); //// 19-7
    c4 = (t1 + t2 + t3 + t4) / (b1 + b2 + b3);
}

//div: 1-4-1
template < typename T > inline void div_SW_QQW_SW(T const& a, T const& b1, T const& b2, T const& b3, T const& b4, T &c)
{
    c = a / ( b1 + b2 + b3 + b4);
}

//div: 1-4-2
template < typename T > inline void div_SW_QQW_PA(T const& a, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2)
{
    div_SW_PA_PA(a,b1,b2+b3+b4,c1,c2);
}

//div: 1-4-3
template < typename T > inline void div_SW_QQW_QTW(T const& a, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3)
{
    div_SW_QTW_QTW(a,b1,b2,b3+b4,c1,c2,c3);
}


//div: 1-4-4
template < typename T > inline void div_SW_QQW_QQW(T const& a, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    div_SW_QTW_QTW(a,b1,b2,b3,c1,c2,c3);//// 43-27
    mul_QTW_QQW_QQW(c1,c2,c3,b1,b2,b3,b4,t1,t2,t3,t4); //// 72-36
    add_SW_QQW_QQW(a,-t1,-t2,-t3,-t4,t1,t2,t3,t4); //// 19-7
    c4 = (t1 + t2 + t3 + t4) / (b1 + b2 + b3);
}

//div: 2-1-1
template < typename T > inline void div_PA_SW_SW(T const& ah, T const& al, T const& b, T &c)
{
    c = (ah+al)/b;
}

//div: 2-1-2
template < typename T > inline void div_PA_SW_PA(T const& ah, T const& al, T const& b, T &ch, T &cl)
{
    ch = ah/b;
    cl = (std::fma(-b,ch,ah) + al)/b;
}

//div: 2-1-3
template < typename T > inline void div_PA_SW_QTW(T const& a1, T const& a2, T const& b, T &c1, T &c2, T &c3)
{
    T t1, t2, t3;
    div_PA_SW_PA(a1,a2,b,c1,c2); //// 4-4
    mul_PA_SW_QTW(c1,c2,b,t1,t2,t3); //// 11-7
    add_PA_QTW_QTW(a1,a2,-t1,-t2,-t3,t1,t2,t3); //// 20-7
    c3 = (t1 + t2 + t3) / b;
}

//div: 2-1-4
template < typename T > inline void div_PA_SW_QQW(T const& a1, T const& a2, T const& b, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    div_PA_SW_QTW(a1,a2,b,c1,c2,c3); //// 38-20
    mul_QTW_SW_QQW(c1,c2,c3,b,t1,t2,t3,t4); //// 26-13
    add_PA_QQW_QQW(a1,a2,-t1,-t2,-t3,-t4,t1,t2,t3,t4); //// 32-11
    c4 = (t1 + t2 + t3 + t4) / b;
}

//div: 2-2-1
template < typename T > inline void div_PA_PA_SW(T const& ah, T const& al, T const& bh, T const& bl, T &c)
{
    c = (ah+al)/(bh+bl);
}

//div: 2-2-2
template < typename T > inline void div_PA_PA_PA(T const& ah, T const& al, T const& bh, T const& bl, T &ch, T &cl)
{
    T t, p;
    ch = ah/bh;
    t = std::fma(-bh,ch,ah);
    p = t + al;
    cl = std::fma(ch,-bl,p)/(bh + bl);
}

//div: 2-2-3
template < typename T > inline void div_PA_PA_QTW(T const& a1, T const& a2, T const& b1, T const& b2, T &c1, T &c2, T &c3)
{
    T t1, t2, t3;
    div_PA_PA_PA(a1,a2,b1,b2,c1,c2); //// 6-6
    mul_PA_PA_QTW(c1,c2,b1,b2,t1,t2,t3); ////20-13
    add_PA_QTW_QTW(a1,a2,-t1,-t2,-t3,t1,t2,t3); //// 20-7
    c3 = (t1 + t2 + t3) / (b1 + b2);
}

//div: 2-2-4
template < typename T > inline void div_PA_PA_QQW(T const& a1, T const& a2, T const& b1, T const& b2, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    div_PA_PA_QTW(a1,a2,b1,b2,c1,c2,c3); //// 50-29
    mul_QTW_PA_QQW(c1,c2,c3,b1,b2,t1,t2,t3,t4); //// 59-28
    add_PA_QQW_QQW(a1,a2,-t1,-t2,-t3,-t4,t1,t2,t3,t4); //// 32-11
    c4 = (t1 + t2 + t3 + t4) / (b1 + b2);
}

//div: 2-3-1
template < typename T > inline void div_PA_QTW_SW(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T &c)
{
    c = (a1+a2)/(b1+b2+b3);
}

//div: 2-3-2
template < typename T > inline void div_PA_QTW_PA(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T &c1, T &c2)
{
    div_PA_PA_PA(a1,a2,b1,b2+b3,c1,c2);
}

//div: 2-3-3
template < typename T > inline void div_PA_QTW_QTW(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3)
{
    T t1, t2, t3;
    div_PA_PA_PA(a1,a2,b1,b2,c1,c2); //// 6-6
    mul_PA_QTW_QTW(c1,c2,b1,b2,b3,t1,t2,t3); //// 21-14
    add_PA_QTW_QTW(a1,a2,-t1,-t2,-t3,t1,t2,t3); //// 20-7
    c3 = (t1 + t2 + t3) / (b1 + b2);
}

//div: 2-3-4
template < typename T > inline void div_PA_QTW_QQW(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    div_PA_QTW_QTW(a1,a2,b1,b2,b3,c1,c2,c3); //// 51-30
    mul_QTW_QTW_QQW(c1,c2,c3,b1,b2,b3,t1,t2,t3,t4); //// 70-34
    add_PA_QQW_QQW(a1,a2,-t1,-t2,-t3,-t4,t1,t2,t3,t4); //// 32-11
    c4 = (t1 + t2 + t3 + t4) / (b1 + b2 + b3);
}

//div: 2-4-1
template < typename T > inline void div_PA_QQW_SW(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T const& b4, T &c)
{
    c = (a1 + a2)/(b1 + b2 + b3 + b4);
}

//div: 2-4-2
template < typename T > inline void div_PA_QQW_PA(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2)
{
    div_PA_PA_PA(a1,a2,b1,b2+b3+b4,c1,c2);
}

//div: 2-4-3
template < typename T > inline void div_PA_QQW_QTW(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3)
{
    div_PA_QTW_QTW(a1,a2,b1,b2,b3+b4,c1,c2,c3);
}

//div: 2-4-4
template < typename T > inline void div_PA_QQW_QQW(T const& a1, T const& a2, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    div_PA_QTW_QTW(a1,a2,b1,b2,b3,c1,c2,c3); ////51-30
    mul_QTW_QQW_QQW(c1,c2,c3,b1,b2,b3,b4,t1,t2,t3,t4); //// 72-36
    add_PA_QQW_QQW(a1,a2,-t1,-t2,-t3,-t4,t1,t2,t3,t4); //// 32-11
    c4 = (t1 + t2 + t3 + t4) / (b1 + b2 + b3);
}

//div: 3-1-1
template < typename T > inline void div_QTW_SW_SW(T const& a1, T const& a2, T const& a3, T const& b, T &c)
{
    c = (a1+a2+a3)/b;
}

//div: 3-1-2
template < typename T > inline void div_QTW_SW_PA(T const& a1, T const& a2, T const& a3, T const& b, T &c1, T &c2)
{
    div_PA_SW_PA(a1,a2+a3,b,c1,c2);
}

//div: 3-1-3
template < typename T > inline void div_QTW_SW_QTW(T const& a1, T const& a2, T const& a3, T const& b, T &c1, T &c2, T &c3)
{
    T t1, t2, t3;
    div_PA_SW_PA(a1,a2,b,c1,c2); //// 4-4
    mul_PA_SW_QTW(c1,c2,b,t1,t2,t3); //// 11-7
    add_QTW_QTW_QTW(a1,a2,a3,-t1,-t2,-t3,t1,t2,t3); //// 21-8
    c3 = (t1 + t2 + t3) / b;
}

//div: 3-1-4
template < typename T > inline void div_QTW_SW_QQW(T const& a1, T const& a2, T const& a3, T const& b, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    div_QTW_SW_QTW(a1,a2,a3,b,c1,c2,c3); //// 39-21
    mul_QTW_SW_QQW(c1,c2,c3,b,t1,t2,t3,t4); //// 12-8
    add_QTW_QQW_QQW(a1,a2,a3,-t1,-t2,-t3,-t4,t1,t2,t3,t4); ////39-14 
    c4 = (t1 + t2 + t3 + t4) / b;
}

//div: 3-2-1
template < typename T > inline void div_QTW_PA_SW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T &c)
{
    c = (a1+a2+a3)/(b1+b2);
}

//div: 3-2-2
template < typename T > inline void div_QTW_PA_PA(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T &c1, T &c2)
{
    div_PA_PA_PA(a1,a2+a3,b1,b2,c1,c2);
}

//div: 3-2-3
template < typename T > inline void div_QTW_PA_QTW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T &c1, T &c2, T &c3)
{
    T t1, t2, t3;
    div_PA_PA_PA(a1,a2,b1,b2,c1,c2); //// 6-6
    mul_PA_PA_QTW(c1,c2,b1,b2,t1,t2,t3); //// 20-13
    add_QTW_QTW_QTW(a1,a2,a3,-t1,-t2,-t3,t1,t2,t3); //// 21-8
    c3 = (t1 + t2 + t3) / (b1 + b2);
}

//div: 3-2-4
template < typename T > inline void div_QTW_PA_QQW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    div_QTW_PA_QTW(a1,a2,a3,b1,b2,c1,c2,c3);//// 51-30
    mul_QTW_PA_QQW(c1,c2,c3,b1,b2,t1,t2,t3,t4); //// 59-28
    add_QTW_QQW_QQW(a1,a2,a3,-t1,-t2,-t3,-t4,t1,t2,t3,t4); //// 39-14
    c4 = (t1 + t2 + t3 + t4) / (b1 + b2);
}

//div: 3-3-1
template < typename T > inline void div_QTW_QTW_SW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T &c)
{
    c = (a1+a2+a3)/(b1+b2+b3);
}

//div: 3-3-2
template < typename T > inline void div_QTW_QTW_PA(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T &c1, T &c2)
{
    div_PA_PA_PA(a1,a2+a3,b1,b2+b3,c1,c2);
}

//div: 3-3-3
template < typename T > inline void div_QTW_QTW_QTW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3)
{
    T t1, t2, t3;
    div_PA_PA_PA(a1,a2,b1,b2,c1,c2); //// 6-6
    mul_PA_QTW_QTW(c1,c2,b1,b2,b3,t1,t2,t3); //// 21-14
    add_QTW_QTW_QTW(a1,a2,a3,-t1,-t2,-t3,t1,t2,t3); //// 21-8
    c3 = (t1 + t2 + t3) / (b1 + b2);
}

//div: 3-3-4
template < typename T > inline void div_QTW_QTW_QQW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    div_QTW_QTW_QTW(a1,a2,a3,b1,b2,b3,c1,c2,c3);//// 52-31
    mul_QTW_QTW_QQW(c1,c2,c3,b1,b2,b3,t1,t2,t3,t4); //// 70-34
    add_QTW_QQW_QQW(a1,a2,a3,-t1,-t2,-t3,-t4,t1,t2,t3,t4); //// 39-14
    c4 = (t1 + t2 + t3 + t4) / (b1 + b2 + b3);
}

//div: 3-4-1
template < typename T > inline void div_QTW_QQW_SW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T const& b4, T &c)
{
    c = (a1+a2+a3)/(b1+b2+b3+b4);
}

//div: 3-4-2
template < typename T > inline void div_QTW_QQW_PA(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2)
{
    div_PA_PA_PA(a1,a2+a3,b1,b2+b3+b4,c1,c2);
}

//div: 3-4-3
template < typename T > inline void div_QTW_QQW_QTW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3)
{
    div_QTW_QTW_QTW(a1,a2,a3,b1,b2,b3+b4,c1,c2,c3);
}

//div: 3-4-4
template < typename T > inline void div_QTW_QQW_QQW(T const& a1, T const& a2, T const& a3, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    div_QTW_QTW_QTW(a1,a2,a3,b1,b2,b3,c1,c2,c3);//// 53-32
    mul_QTW_QQW_QQW(c1,c2,c3,b1,b2,b3,b4,t1,t2,t3,t4); //// 72-36
    add_QTW_QQW_QQW(a1,a2,a3,-t1,-t2,-t3,-t4,t1,t2,t3,t4); //// 39-14
    c4 = (t1 + t2 + t3 + t4) / (b1 + b2 + b3);
}

//div: 4-1-1
template < typename T > inline void div_QQW_SW_SW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b, T &c)
{
    c = (a1+a2+a3+a4)/b;
}

//div: 4-1-2
template < typename T > inline void div_QQW_SW_PA(T const& a1, T const& a2, T const& a3, T const& a4, T const& b, T &c1, T &c2)
{
    div_PA_SW_PA(a1,a2+a3+a4,b,c1,c2);
}

//div: 4-1-3
template < typename T > inline void div_QQW_SW_QTW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b, T &c1, T &c2, T &c3)
{
    div_QTW_SW_QTW(a1,a2,a3+a4,b,c1,c2,c3);
}

//div: 4-1-4
template < typename T > inline void div_QQW_SW_QQW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    div_QTW_SW_QTW(a1,a2,a3,b,c1,c2,c3);////40-22
    mul_QTW_SW_QQW(c1,c2,c3,b,t1,t2,t3,t4);////26-13
    add_QQW_QQW_QQW(a1,a2,a3,a4,-t1,-t2,-t3,-t4,t1,t2,t3,t4); ////40-14
    c4 = (t1 + t2 + t3 + t4) / b;
}

//div: 4-2-1
template < typename T > inline void div_QQW_PA_SW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T &c)
{
    c = (a1+a2+a3+a4)/(b1+b2);
}

//div: 4-2-2
template < typename T > inline void div_QQW_PA_PA(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T &c1, T &c2)
{
    div_PA_PA_PA(a1,a2+a3+a4,b1,b2,c1,c2);
}

//div: 4-2-3
template < typename T > inline void div_QQW_PA_QTW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T &c1, T &c2, T &c3)
{
    div_QTW_PA_QTW(a1,a2,a3+a4,b1,b2,c1,c2,c3);
}

//div: 4-2-4
template < typename T > inline void div_QQW_PA_QQW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    div_QTW_PA_QTW(a1,a2,a3,b1,b2,c1,c2,c3); //// 51-30
    mul_QTW_PA_QQW(c1,c2,c3,b1,b2,t1,t2,t3,t4);////59-28
    add_QQW_QQW_QQW(a1,a2,a3,a4,-t1,-t2,-t3,-t4,t1,t2,t3,t4); ////40-14 
    c4 = (t1 + t2 + t3 + t4) / (b1 + b2);
}

//div: 4-3-1
template < typename T > inline void div_QQW_QTW_SW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T &c)
{
    c = (a1+a2+a3+a4)/(b1+b2+b3);
}

//div: 4-3-2
template < typename T > inline void div_QQW_QTW_PA(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T &c1, T &c2)
{
    div_PA_PA_PA(a1,a2+a3+a4,b1,b2+b3,c1,c2);
}

//div: 4-3-3
template < typename T > inline void div_QQW_QTW_QTW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3)
{
    div_QTW_QTW_QTW(a1,a2,a3+a4,b1,b2,b3,c1,c2,c3);
}

//div: 4-3-4
template < typename T > inline void div_QQW_QTW_QQW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    div_QTW_QTW_QTW(a1,a2,a3,b1,b2,b3,c1,c2,c3);//// 52-31
    mul_QTW_QTW_QQW(c1,c2,c3,b1,b2,b3,t1,t2,t3,t4); //// 24-15
    add_QQW_QQW_QQW(a1,a2,a3,a4,-t1,-t2,-t3,-t4,t1,t2,t3,t4); //// 40-14
    c4 = (t1 + t2 + t3 + t4) / (b1 + b2 + b3);
}

//div: 4-4-1
template < typename T > inline void div_QQW_QQW_SW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T &c)
{
    c = (a1+a2+a3+a4)/(b1+b2+b3+b4);
}

//div: 4-4-2
template < typename T > inline void div_QQW_QQW_PA(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2)
{
    div_PA_PA_PA(a1,a2+a3+a4,b1,b2+b3+b4,c1,c2);
}

//div: 4-4-3
template < typename T > inline void div_QQW_QQW_QTW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3)
{
    div_QTW_QTW_QTW(a1,a2,a3+a4,b1,b2,b3+b4,c1,c2,c3);
}

//div: 4-4-4
template < typename T > inline void div_QQW_QQW_QQW(T const& a1, T const& a2, T const& a3, T const& a4, T const& b1, T const& b2, T const& b3, T const& b4, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    div_QTW_QTW_QTW(a1,a2,a3,b1,b2,b3,c1,c2,c3);//// 52-31
    mul_QTW_QQW_QQW(c1,c2,c3,b1,b2,b3,b4,t1,t2,t3,t4);////72-26
    add_QQW_QQW_QQW(a1,a2,a3,a4,-t1,-t2,-t3,-t4,t1,t2,t3,t4); ////40-14
    c4 = (t1 + t2 + t3 + t4) / (b1 + b2 + b3);
}

//--------------------
//square
//--------------------

//sqr:1-2
template < typename T > inline void sqr_SW_PA(T const& a, T &c1, T &c2)
{
    TwoProductFMA(a,a,c1,c2);
}

//sqr:1-3
template < typename T > inline void sqr_SW_QTW(T const& a, T &c1, T &c2, T &c3)
{
    TwoProductFMA(a,a,c1,c2);
    c3 = fp_const<T>::zero();
}

//sqr:1-4
template < typename T > inline void sqr_SW_QQW(T const& a, T &c1, T &c2, T &c3, T &c4)
{
    TwoProductFMA(a,a,c1,c2);
    c3 = fp_const<T>::zero();
    c4 = fp_const<T>::zero();
}

//sqr:2-1
template < typename T > inline void sqr_PA_SW(T const& a1, T const& a2, T &c)
{
    c = (a1+a2)*(a1+a2);
}

//sqr:2-2
template < typename T > inline void sqr_PA_PA(T const& a1, T const& a2, T &c1, T &c2)
{
    T e1;
    TwoProductFMA(a1,a1,c1,e1);
    c2 = std::fma(a1,2*a2,e1);
}

//sqr:2-3
template < typename T > inline void sqr_PA_QTW(T const& a1, T const& a2, T &c1, T &c2, T &c3)
{
    T e1, e2, e3;
    T t2;
    TwoProductFMA(a1,a1,c1,e1);
    TwoProductFMA(a1,2*a2,t2,e2);
    TwoSum(t2,e1,c2,e3);
    c3 = std::fma(a2,a2,e2) + e3;
}

//sqr:2-4
template < typename T > inline void sqr_PA_QQW(T const& a1, T const& a2, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e5, e7, e14;
    T t2, t4, t5;
    
    TwoProductFMA(a1,a1,c1,e1);
    TwoProductFMA(a1,2*a2,t2,e2);
    TwoSum(t2,e1,c2,e5);
    TwoProductFMA(a2,a2,t5,e7);
    TwoSum(t5,e5,c3,e14);
    c4 = e7 + e14;
}

//sqr:3-1
template < typename T > inline void sqr_QTW_SW(T const& a1, T const& a2, T const& a3, T &c)
{
    T t;
    t = a1+a2+a3;
    c = t*t;
}

//sqr:3-2
template < typename T > inline void sqr_QTW_PA(T const& a1, T const& a2, T const& a3, T &c1, T &c2)
{
    T e1;
    TwoProductFMA(a1,a1,c1,e1);
    c2 = std::fma(2*a1,a2,e1);
}

//sqr:3-3
template < typename T > inline void sqr_QTW_QTW(T const& a1, T const& a2, T const& a3, T &c1, T &c2, T &c3)
{
    T e1, e2, e5;
    T t2;
    
    TwoProductFMA(a1,a1,c1,e1);
    TwoProductFMA(a1,2*a2,t2,e2);
    TwoSum(t2,e1,c2,e5);
    c3 = std::fma(a1,2*a3,e2) + std::fma(a2,a2,e5);
}

//sqr:3-4
template < typename T > inline void sqr_QTW_QQW(T const& a1, T const& a2, T const& a3, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e5;
    T e6, e7, e9, e11, e14;
    T t2, t4, t5;

    TwoProductFMA(a1,a1,c1,e1);
    TwoProductFMA(a1,2*a2,t2,e2);
    TwoSum(t2,e1,c2,e5);
    TwoProductFMA(a1,2*a3,t4,e6);
    TwoProductFMA(a2,a2,t5,e7);
    TwoSum(t4,t5,c3,e9);
    TwoSum(c3,e2,c3,e11);
    TwoSum(c3,e5,c3,e14);
    c4 = std::fma(a2,2*a3,e6) + e7 + e9 + e11 + e14;
}

//sqr:4-1
template < typename T > inline void sqr_QQW_SW(T const& a1, T const& a2, T const& a3, T const& a4, T &c)
{
    T t;
    t = a1 + a2  + a3 + a4;
    c = t*t;
}

//sqr:4-2
template < typename T > inline void sqr_QQW_PA(T const& a1, T const& a2, T const& a3, T const& a4, T &c1, T &c2)
{
    T e1;
    TwoProductFMA(a1,a1,c1,e1);
    c2 = std::fma(2*a1,a2,e1);
}

//sqr:4-3
template < typename T > inline void sqr_QQW_QTW(T const& a1, T const& a2, T const& a3, T const& a4, T &c1, T &c2, T &c3)
{
    T e1, e2, e5;
    T t2;
    
    TwoProductFMA(a1,a1,c1,e1);
    TwoProductFMA(a1,2*a2,t2,e2);
    TwoSum(t2,e1,c2,e5);
    c3 = std::fma(a1,2*a3,std::fma(a2,a2,e2)) + e5;
}

//sqr:4-4
template < typename T > inline void sqr_QQW_QQW(T const& a1, T const& a2, T const& a3, T const& a4, T &c1, T &c2, T &c3, T &c4)
{
    T e1, e2, e5, e6, e7, e9, e11, e14;
    T t2, t4, t5;
    
    TwoProductFMA(a1,a1,c1,e1);
    TwoProductFMA(a1,2*a2,t2,e2);
    TwoSum(t2,e1,c2,e5);
    TwoProductFMA(a1,2*a3,t4,e6);
    TwoProductFMA(a2,a2,t5,e7);
    TwoSum(t4,t5,c3,e9);
    TwoSum(c3,e2,c3,e11);
    TwoSum(c3,e5,c3,e14);
    c4 = std::fma(2*a2,a3,2*a1*a4) + e6 + e7 + e9 + e11 + e14;
}

//------------------------------
//Sqrt
//------------------------------

//sqrt: 1-2
template < typename T > inline void sqrt_SW_PA(T const& a, T &ch, T &cl)
{
    ch = std::sqrt(a);
    cl = std::fma(-ch,ch,a)/(ch+ch);
}

//sqrt: 1-3
template < typename T > inline void sqrt_SW_QTW(T const& a, T &c1, T &c2, T &c3)
{
    T t1, t2 ,t3;
    sqrt_SW_PA(a,c1,c2);
    sqr_PA_QTW(c1,c2,t1,t2,t3);
    add_QTW_SW_QTW(t1,t2,t3,-a,t1,t2,t3);
    c3 = - (t1+t2+t3) / (2*(c1 + c2));
}

//sqrt: 1-4
template < typename T > inline void sqrt_SW_QQW(T const& a, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    sqrt_SW_QTW(a,c1,c2,c3);
    sqr_QTW_QQW(c1,c2,c3,t1,t2,t3,t4);
    add_QQW_SW_QQW(t1,t2,t3,t4,-a,t1,t2,t3,t4);
    c4 = - (t1+t2+t3+t4) / (2*(c1 + c2 + c3));
}


//sqrt: 2-1
template < typename T > inline void sqrt_PA_SW(T const& ah, T const& al, T &c)
{
    c = std::sqrt(ah+al);
}

//sqrt: 2-2
template < typename T > inline void sqrt_PA_PA(T const& ah, T const& al, T &ch, T &cl)
{
    ch = std::sqrt(ah);
    cl = (std::fma(-ch,ch,ah) + al)/(ch + ch);
}

//sqrt: 2-3
template < typename T > inline void sqrt_PA_QTW(T const& a1, T const& a2, T &c1, T &c2, T &c3)
{
    T t1, t2 ,t3;
    sqrt_PA_PA(a1,a2,c1,c2);
    sqr_PA_QTW(c1,c2,t1,t2,t3);
    add_QTW_PA_QTW(t1,t2,t3,-a1,-a2,t1,t2,t3);
    c3 = - (t1+t2+t3) / (2*(c1 + c2));
}

//sqrt: 2-4
template < typename T > inline void sqrt_PA_QQW(T const& a1, T const& a2, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    sqrt_PA_QTW(a1,a2,c1,c2,c3);
    sqr_QTW_QQW(c1,c2,c3,t1,t2,t3,t4);
    add_QQW_PA_QQW(t1,t2,t3,t4,-a1,-a2,t1,t2,t3,t4);
    c4 = - (t1+t2+t3+t4) / (2*(c1 + c2 + c3));
}

//sqrt: 3-1
template < typename T > inline void sqrt_QTW_SW(T const& a1, T const& a2, T const& a3, T &c)
{
    c = std::sqrt(a1+a2+a3);
}

//sqrt: 3-2
template < typename T > inline void sqrt_QTW_PA(T const& a1, T const& a2, T const& a3, T &c1, T &c2)
{
    sqrt_PA_PA(a1,a2+a3,c1,c2);
}

//sqrt: 3-3
template < typename T > inline void sqrt_QTW_QTW(T const& a1, T const& a2, T const& a3, T &c1, T &c2, T &c3)
{
    T t1, t2 ,t3;
    sqrt_PA_PA(a1,a2,c1,c2);
    sqr_PA_QTW(c1,c2,t1,t2,t3);
    add_QTW_QTW_QTW(t1,t2,t3,-a1,-a2,-a3,t1,t2,t3);
    c3 = - (t1+t2+t3) / (2*(c1 + c2));
}

//sqrt: 3-4
template < typename T > inline void sqrt_QTW_QQW(T const& a1, T const& a2, T const& a3, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    sqrt_QTW_QTW(a1,a2,a3,c1,c2,c3);
    sqr_QTW_QQW(c1,c2,c3,t1,t2,t3,t4);
    add_QQW_QTW_QQW(t1,t2,t3,t4,-a1,-a2,-a3,t1,t2,t3,t4);
    c4 = - (t1+t2+t3+t4) / (2*(c1 + c2 + c3));
}

//sqrt: 4-1
template < typename T > inline void sqrt_QQW_SW(T const& a1, T const& a2, T const& a3, T const& a4, T &c)
{
    c = std::sqrt(a1+a2+a3+a4);
}

//sqrt: 4-2
template < typename T > inline void sqrt_QQW_PA(T const& a1, T const& a2, T const& a3, T const& a4, T &c1, T &c2)
{
    sqrt_QTW_PA(a1,a2,a3+a4,c1,c2);
}

//sqrt: 4-3
template < typename T > inline void sqrt_QQW_QTW(T const& a1, T const& a2, T const& a3, T const& a4, T &c1, T &c2, T &c3)
{
    sqrt_QTW_QTW(a1,a2,a3+a4,c1,c2,c3);
}

//sqrt: 4-4
template < typename T > inline void sqrt_QQW_QQW(T const& a1, T const& a2, T const& a3, T const& a4, T &c1, T &c2, T &c3, T &c4)
{
    T t1, t2, t3, t4;
    sqrt_QTW_QTW(a1,a2,a3,c1,c2,c3);
    sqr_QTW_QQW(c1,c2,c3,t1,t2,t3,t4);
    add_QQW_QQW_QQW(t1,t2,t3,t4,-a1,-a2,-a3,-a4,t1,t2,t3,t4);
    c4 = - (t1+t2+t3+t4) / (2*(c1 + c2 + c3));
}



// -------------------------------
// Double word Part
// -------------------------------
//Double word arithmetic: add
template < typename T > inline void add_SW_SW_DW(T const& a, T const& b, T &ch, T &cl)
{
    TwoSum(a, b, ch,cl);
}

template < typename T > inline void add_SW_DW_SW(T const& a, T const& bh, T const& bl, T &c)
{
//    c = a + bh;
    T sh, se, eh; //
    TwoSum(a, bh, sh, eh); //
    se = eh + bl; //
    c = sh + se; //
}

template < typename T > inline void add_SW_DW_DW(T const& a, T const& bh, T const& bl, T &ch, T &cl)
{
    T sh, sl, eh, el, se, te;
    TwoSum(a, bh, sh, eh);
    se = eh + bl;
//    FastTwoSum(sh, se, sh, se);
    FastTwoSum(sh, se, ch, cl);
}

template < typename T > inline void add_DW_SW_SW(T const& ah, T const& al, T const& b, T &c)
{
    T sh, eh, el, se, te;
    TwoSum(ah, b, sh, eh);
    se = eh + al;
    FastTwoSum(sh, se, sh, se);
    c = sh + se;
}

template < typename T > inline void add_DW_SW_DW(T const& ah, T const& al, T const& bh, T &ch, T &cl)
{
    T sh, eh, el, se, te;
    TwoSum(ah, bh, sh, eh);
    se = eh + al;
//    FastTwoSum(sh, se, sh, se);
    FastTwoSum(sh, se, ch, cl);
}

template < typename T > inline void add_DW_DW_SW(T const& ah, T const& al, T const& bh, T const& bl, T &c)
{
    T sh, sl, eh, el, se, te;
    TwoSum(ah, bh, sh, eh);
    TwoSum(al, bl, sl, el);
    se = eh + sl;
    FastTwoSum(sh, se, sh, se);
    te = se + el;
    c = sh + te;
}

template < typename T > inline void add_DW_DW_DW(T const& ah, T const& al, T const& bh, T const& bl, T &ch, T &cl)
{
    T sh, sl, eh, el, se, te;
    TwoSum(ah, bh, sh, eh);
    TwoSum(al, bl, sl, el);
    se = eh + sl;
    FastTwoSum(sh, se, sh, se);
    te = se + el;
    FastTwoSum(sh, te, ch, cl);
}


//Double word arithmetic: mul
template < typename T > inline void mul_SW_SW_DW(T const& a, T const& b, T &ch, T &cl)
{
    TwoProductFMA(a, b, ch, cl);
}

template < typename T > inline void mul_SW_DW_SW(T const& a, T const& bh, T const& bl, T &c)
{
    T p1, p2;
    TwoProductFMA(a, bh, p1, p2);
    c = p1 + std::fma(a,bl,p2);
}

template < typename T > inline void mul_SW_DW_DW(T const& ah, T const& bh, T const& bl, T &ch, T &cl)
{
    T p1, p2;
    TwoProductFMA(ah, bh, p1, p2);
    p2 = std::fma(ah,bl,p2);
    FastTwoSum(p1, p2, ch, cl);
}

template < typename T > inline void mul_DW_SW_SW(T const& ah, T const& al, T const& b, T &c)
{
    T p1, p2;
    TwoProductFMA(ah, b, p1, p2);
    p2 = std::fma(al,b,p2);
    c = p1 + p2;
}

template < typename T > inline void mul_DW_SW_DW(T const& ah, T const& al, T const& b, T &ch, T &cl)
{
    T p1, p2;
    TwoProductFMA(ah, b, p1, p2);
    p2 = std::fma(al,b,p2);
    FastTwoSum(p1, p2, ch, cl);
}

template < typename T > inline void mul_DW_DW_SW(T const& ah, T const& al, T const& bh, T const& bl, T &c)
{
    T p1, p2;
    TwoProductFMA(ah, bh, p1, p2);
    p2 = std::fma(ah,bl,p2);
    p2 = std::fma(al,bh,p2);
    c = p1 + p2;
}

template < typename T > inline void mul_DW_DW_DW(T const& ah, T const& al, T const& bh, T const& bl, T &ch, T &cl)
{
    T p1, p2;
    TwoProductFMA(ah, bh, p1, p2);
    p2 = std::fma(ah,bl,p2);
    p2 = std::fma(al,bh,p2);
    FastTwoSum(p1, p2, ch, cl);
}

//Double word arithmetic: div

template < typename T > inline void div_SW_SW_DW(T const& a, T const& b, T &ch, T &cl)
{
    T dh, dl, s1, s2, t;
    ch = a / b;
    mul_SW_SW_DW(ch,b,dh,dl);
    TwoSum(a,-dh, s1, s2);
    s2 = s2 - dl;
    t = (s1 + s2)/b;
    FastTwoSum(ch,t,ch,cl);
}

////////////////?????/////////////////
template < typename T > inline void div_SW_DW_SW(T const& a, T const& bh, T const& bl, T &c)
{
    T dh, dl, s1, s2;
    c = a / bh;
    mul_SW_DW_DW(c,bh,bl,dh,dl);
    TwoSum(a,-dh, s1, s2);
    s2 = s2 - dl;
    c = c + (s1 + s2)/bh;
}

template < typename T > inline void div_SW_DW_DW(T const& a, T const& bh, T const& bl, T &ch, T &cl)
{
    T dh, dl, s1, s2, t;
    ch = a / bh;
    mul_SW_DW_DW(ch,bh,bl,dh,dl);
    TwoSum(a,-dh, s1, s2);
    s2 = s2 - dl;
    t = (s1 + s2)/bh;
    FastTwoSum(ch,t,ch,cl);
}

////???????????????/////////
template < typename T > inline void div_DW_SW_SW(T const& ah, T const& al, T const& b, T &c)
{
    T dh, dl, s1, s2;
    c = ah / b;
    mul_SW_SW_DW(c,b,dh,dl);
    TwoSum(ah,-dh, s1, s2);
    s2 = s2 - dl + al;
    c = c + (s1 + s2)/b;
}

template < typename T > inline void div_DW_SW_DW(T const& ah, T const& al, T const& b, T &ch, T &cl)
{
    T dh, dl, s1, s2, t;
    ch = ah / b;
    mul_SW_SW_DW(ch,b,dh,dl);
    TwoSum(ah,-dh, s1, s2);
    s2 = s2 - dl + al;
    t = (s1 + s2)/b;
    FastTwoSum(ch,t,ch,cl);
}

///?????////
template < typename T > inline void div_DW_DW_SW(T const& ah, T const& al, T const& bh, T const& bl, T &c)
{
    T dh, dl, s1, s2;
    c = ah / bh;
    mul_SW_DW_DW(c,bh,bl,dh,dl);
    TwoSum(ah,-dh, s1, s2);
    s2 = s2 - dl + al;
    c = c + (s1 + s2)/bh;
}

template < typename T > inline void div_DW_DW_DW(T const& ah, T const& al, T const& bh, T const& bl, T &ch, T &cl)
{
    T dh, dl, s1, s2, t;
    ch = ah / bh;
    mul_SW_DW_DW(ch,bh,bl,dh,dl);
    TwoSum(ah,-dh, s1, s2);
    s2 = s2 - dl + al;
    t = (s1 + s2)/bh;
    FastTwoSum(ch,t,ch,cl);
}

//Double word arithmetic: sqrt
template < typename T > inline void sqrt_SW_DW(T const& a, T &ch, T &cl)
{
    T x, ax, r1, r2, s1;
    x = fp_const<T>::one()/std::sqrt(a);
    ax = a*x;
    TwoProductFMA(ax,ax, r1, r2);
    add_SW_DW_SW(a, -r1, -r2, s1);
    TwoSum(ax,s1*(x*fp_const<T>::half()),ch,cl);
}

template < typename T > inline void sqrt_DW_SW(T const& ah, T const& al, T &c)
{
    T x, ax, r1, r2, s1;
    x = fp_const<T>::one()/std::sqrt(ah);
    ax = ah*x;
    TwoProductFMA(ax,ax, r1, r2);
    add_DW_DW_SW(ah, al, -r1, -r2, s1);
    c = ax + s1*(x*fp_const<T>::half());
}


template < typename T > inline void sqrt_DW_DW(T const& ah, T const& al, T &ch, T &cl)
{
    T x, ax, r1, r2, s1;
    x = fp_const<T>::one()/std::sqrt(ah);
    ax = ah*x;
    TwoProductFMA(ax,ax, r1, r2);
    add_DW_DW_SW(ah, al, -r1, -r2, s1);
    TwoSum(ax,s1*(x*fp_const<T>::half()),ch,cl);
}

}

#endif
