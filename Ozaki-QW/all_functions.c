// This prototype code was written 
// by Katsuhisa Ozaki (Shibaura Institute of Technology)
// Debug is not sufficiently done, so please use it carefully and 
// not spread it. 

#include <stdio.h>
#include <math.h>

void sqr_PA_QTW(float, float, float*, float*, float*);


// ------------------------
// Basic Part
// ------------------------

void TwoSum(float a, float b, float *x, float *y)
{
    float z;
    *x = a + b;
    z = *x - a;
    *y = ( a - (*x-z) ) + (b-z);
}

void FastTwoSum(float a, float b, float *x, float *y)
{
    *x = a + b;
    *y = (a-*x) + b;
}

void TwoProductFMA(float a, float b, float *x, float *y)
{
    *x = a*b;
    *y = fma(a,b,-*x);
}

//------------------------------
//Addition
//------------------------------

//add: 1-1-2
void add_SW_SW_PA(float a, float b, float *ch, float *cl)
{
    TwoSum(a,b,ch,cl);
}

//add: 1-1-3
void add_SW_SW_QTW(float a, float b, float *c1, float *c2, float *c3)
{
    TwoSum(a,b,c1,c2);
    *c3 = 0;
}

//add: 1-1-4
void add_SW_SW_QQW(float a, float b, float *c1, float *c2, float *c3, float *c4)
{
    TwoSum(a,b,c1,c2);
    *c3=0;
    *c4=0;
}

//add: 1-2-1
void add_SW_PA_SW(float ah, float bh, float bl, float *c)
{
    *c = ah + bh + bl;
}

//add: 1-2-2
void add_SW_PA_PA(float a, float bh, float bl, float *ch, float *cl)
{
    float t;
    TwoSum(a,bh,ch,&t);
    *cl = bl + t;
}

//add: 1-2-3
void add_SW_PA_QTW(float a, float b1, float b2, float *c1, float *c2, float *c3)
{
    float e1;
    TwoSum(a,b1,c1,&e1);
    TwoSum(b2,e1,c2,c3);
}

//add: 1-2-4
void add_SW_PA_QQW(float a, float b1, float b2, float *c1, float *c2, float *c3, float *c4)
{
    float e1;
    TwoSum(a,b1,c1,&e1);
//    TwoSum(a,b2,c2,c3);
    TwoSum(b2,e1,c2,c3); //
    *c4 = 0;
}

//add: 1-3-1
void add_SW_QTW_SW(float a, float b1, float b2, float b3, float *c)
{
    *c = a + b1 + b2 + b3;
}

//add: 1-3-2
void add_SW_QTW_PA(float a, float b1, float b2, float b3, float *c1, float *c2)
{
    float e1;
    TwoSum(a,b1,c1,&e1);
    *c2 = b2 + e1 + b3;
}

//add: 1-3-3
void add_SW_QTW_QTW(float a, float b1, float b2, float b3, float *c1, float *c2, float *c3)
{
    float e1, e3;
    TwoSum(a,b1,c1,&e1);
    TwoSum(b2,e1,c2,&e3);
    *c3 = b3 + e3;
}

//add: 1-3-4
void add_SW_QTW_QQW(float a, float b1, float b2, float b3, float *c1, float *c2, float *c3, float *c4)
{
    float e1, e2;
    TwoSum(a,b1,c1,&e1);
    TwoSum(b2,e1,c2,&e2);
    TwoSum(b3,e2,c3,c4);
}
//add: 1-4-1
void add_SW_QQW_SW(float a1, float b1, float b2, float b3, float b4, float *c)
{
    *c = a1 + b1 + b2 + b3 + b4;
}
//add: 1-4-2
void add_SW_QQW_PA(float a, float b1, float b2, float b3, float b4, float *c1, float *c2)
{
    float e1;
    TwoSum(a,b1,c1,&e1);
    *c2 = b2 + e1 + b3 + b4;
}
//add: 1-4-3
void add_SW_QQW_QTW(float a, float b1, float b2, float b3, float b4, float *c1, float *c2, float *c3)
{
    float e1, e2;
    TwoSum(a,b1,c1,&e1);
    TwoSum(e1,b2,c2,&e2);
    *c3 = b3 + e2 + b4;
}

//add: 1-4-4
void add_SW_QQW_QQW(float a, float b1, float b2, float b3, float b4, float *c1, float *c2, float *c3, float *c4)
{
    float e1, e2, e3;
    TwoSum(a,b1,c1,&e1);
    TwoSum(e1,b2,c2,&e2);
    TwoSum(e2,b3,c3,&e3);
    *c4 = b4 + e3;
}

//add: 2-1-1
void add_PA_SW_SW(float ah, float al, float b, float *c)
{
    *c = ah + b + al;
}

//add: 2-1-2
void add_PA_SW_PA(float ah, float al, float b, float *ch, float *cl)
{
    float t;
    TwoSum(ah,b,ch,&t);
    *cl = al + t;
}

//add: 2-1-3
void add_PA_SW_QTW(float a1, float a2, float b, float *c1, float *c2, float *c3)
{
    float e1;
    TwoSum(a1,b,c1,&e1);
    TwoSum(a2,e1,c2,c3);
}

//add: 2-1-4
void add_PA_SW_QQW(float a1, float a2, float b, float *c1, float *c2, float *c3, float *c4)
{
    float e1;
    TwoSum(a1,b,c1,&e1);
    TwoSum(a2,e1,c2,c3);
    *c4 = 0;
}

//add: 2-2-1
void add_PA_PA_SW(float ah, float al, float bh, float bl, float *c)
{
    *c = ah + bh + al + bl;
}

//add: 2-2-2
void add_PA_PA_PA(float ah, float al, float bh, float bl, float *ch, float *cl)
{
    float t;
    TwoSum(ah,bh,ch,&t);
    *cl = al + bl + t;
}

//add: 2-2-3
void add_PA_PA_QTW(float a1, float a2, float b1, float b2, float *c1, float *c2, float *c3)
{
    float e1, e2, e3;
    TwoSum(a1,b1,c1,&e1);
    TwoSum(a2,b2,c2,&e2);
    TwoSum(*c2,e1,c2,&e3);
    *c3 = e2 + e3;
}

//add: 2-2-4
void add_PA_PA_QQW(float a1, float a2, float b1, float b2, float *c1, float *c2, float *c3, float *c4)
{
    float e1, e2, e3;
    TwoSum(a1,b1,c1,&e1);
    TwoSum(a2,b2,c2,&e2);
    TwoSum(*c2,e1,c2,&e3);
    TwoSum(e2,e3,c3,c4);
}

//add: 2-3-1
void add_PA_QTW_SW(float a1, float a2, float b1, float b2, float b3, float *c)
{
    *c = a1 + b1 + a2 + b2 + b3;
}

//add: 2-3-2
void add_PA_QTW_PA(float a1, float a2, float b1, float b2, float b3, float *c1, float *c2)
{
    float e1;
    TwoSum(a1,b1,c1,&e1);
    *c2 = a2 + b2 + e1 + b3;
}

//add: 2-3-3
void add_PA_QTW_QTW(float a1, float a2, float b1, float b2, float b3, float *c1, float *c2, float *c3)
{
    float e1, e2, e3;
    TwoSum(a1,b1,c1,&e1);
    TwoSum(a2,b2,c2,&e2);
    TwoSum(*c2,e1,c2,&e3);
    *c3 = b3 + e2 + e3;
}

//add: 2-3-4
void add_PA_QTW_QQW(float a1, float a2, float b1, float b2, float b3, float *c1, float *c2, float *c3, float *c4)
{
    float e1, e2, e3, e5, e6;
    TwoSum(a1,b1,c1,&e1);
    TwoSum(a2,b2,c2,&e2);
    TwoSum(*c2,e1,c2,&e3);
    TwoSum(b3,e2,c3,&e5);
    TwoSum(*c3,e3,c3,&e6);
    *c4 = e5 + e6;
}

//add: 2-4-1
void add_PA_QQW_SW(float a1, float a2, float b1, float b2, float b3, float b4, float *c)
{
    *c = a1 + b1 + a2 + b2 + b3 + b4;
}

//add: 2-4-2
void add_PA_QQW_PA(float a1, float a2, float b1, float b2, float b3, float b4, float *c1, float *c2)
{
    float e1;
    TwoSum(a1,b1,c1,&e1);
    *c2 = a2 + b2 + e1 + b3 + b4;
}

//add: 2-4-3
void add_PA_QQW_QTW(float a1, float a2, float b1, float b2, float b3, float b4, float *c1, float *c2, float *c3)
{
    float e1, e2, e3;
    TwoSum(a1,b1,c1,&e1);
    TwoSum(a2,b2,c2,&e2);
    TwoSum(*c2,e1,c2,&e3);
    *c3 = b3 + e2 + e3 + b4;
}

//add: 2-4-4
void add_PA_QQW_QQW(float a1, float a2, float b1, float b2, float b3, float b4, float *c1, float *c2, float *c3, float *c4)
{
    float e1, e2, e3, e5, e6;
    TwoSum(a1,b1,c1,&e1);
    TwoSum(a2,b2,c2,&e2);
    TwoSum(*c2,e1,c2,&e3);
    TwoSum(b3,e2,c3,&e5);
    TwoSum(*c3,e3,c3,&e6);
    *c4 = b4 + e5 + e6;
}

//add: 3-1-1
void add_QTW_SW_SW(float a1, float a2, float a3, float b, float *c)
{
    *c = a1 + b + a2 + a3;
}

//add: 3-1-2
void add_QTW_SW_PA(float a1, float a2, float a3, float b, float *c1, float *c2)
{
    float e1;
    TwoSum(a1,b,c1,&e1);
    *c2 = a2 + e1 + a3;
}

//add: 3-1-3
void add_QTW_SW_QTW(float a1, float a2, float a3, float b, float *c1, float *c2, float *c3)
{
    float e1, e3;
    TwoSum(a1,b,c1,&e1);
    TwoSum(a2,e1,c2,&e3);
    *c3 = a3 + e3;
}

//add: 3-1-4
void add_QTW_SW_QQW(float a1, float a2, float a3, float b, float *c1, float *c2, float *c3, float *c4)
{
    float e1, e2;
    TwoSum(a1,b,c1,&e1);
    TwoSum(a2,e1,c2,&e2);
    TwoSum(a3,e2,c3,c4);
}

//add: 3-2-1
void add_QTW_PA_SW(float a1, float a2, float a3, float b1, float b2, float *c)
{
    *c = a1 + b1 + a2 + b2 + a3;
}

//add: 3-2-2
void add_QTW_PA_PA(float a1, float a2, float a3, float b1, float b2, float *c1, float *c2)
{
    float e1;
    TwoSum(a1,b1,c1,&e1);
    *c2 = a2 + b2 + e1 + a3;
}

//add: 3-2-3
void add_QTW_PA_QTW(float a1, float a2, float a3, float b1, float b2, float *c1, float *c2, float *c3)
{
    float e1, e2, e3;
    TwoSum(a1,b1,c1,&e1);
    TwoSum(a2,b2,c2,&e2);
    TwoSum(*c2,e1,c2,&e3);
    *c3 = a3 + e2 + e3;
}

//add: 3-2-4
void add_QTW_PA_QQW(float a1, float a2, float a3, float b1, float b2, float *c1, float *c2, float *c3, float *c4)
{
    float e1, e2, e3, e5, e6;
    TwoSum(a1,b1,c1,&e1);
    TwoSum(a2,b2,c2,&e2);
    TwoSum(*c2,e1,c2,&e3);
    TwoSum(a3,e2,c3,&e5);
    TwoSum(*c3,e3,c3,&e6);
    *c4 = e5 + e6;
}

//add: 3-3-1
void add_QTW_QTW_SW(float a1, float a2, float a3, float b1, float b2, float b3, float *c)
{
    *c = a1 + b1 + a2 + b2 + a3 + b3;
}

//add: 3-3-2
void add_QTW_QTW_PA(float a1, float a2, float a3, float b1, float b2, float b3, float *c1, float *c2)
{
    float e1;
    TwoSum(a1,b1,c1,&e1);
    *c2 = a2 + b2 + e1; //追加してもよい
}

//add: 3-3-3
void add_QTW_QTW_QTW(float a1, float a2, float a3, float b1, float b2, float b3, float *c1, float *c2, float *c3)
{
    float e1, e2, e3;
    TwoSum(a1,b1,c1,&e1);
    TwoSum(a2,b2,c2,&e2);
    TwoSum(*c2,e1,c2,&e3);
    *c3 = a3 + b3 + e2 + e3;
}

//add: 3-3-4
void add_QTW_QTW_QQW(float a1, float a2, float a3, float b1, float b2, float b3, float *c1, float *c2, float *c3, float *c4)
{
    float e1, e2, e3, e4, e5, e6;
    TwoSum(a1,b1,c1,&e1);
    TwoSum(a2,b2,c2,&e2);
    TwoSum(*c2,e1,c2,&e3);
    TwoSum(a3,b3,c3,&e4);
    TwoSum(*c3,e2,c3,&e5);
    TwoSum(*c3,e3,c3,&e6);
    *c4 = e4 + e5 + e6;
}

//add: 3-4-1
void add_QTW_QQW_SW(float a1, float a2, float a3, float b1, float b2, float b3, float b4, float *c)
{
    *c = a1 + b1 + a2 + b2 + a3 + b3 + b4;
}

//add: 3-4-2
void add_QTW_QQW_PA(float a1, float a2, float a3, float b1, float b2, float b3, float b4, float *c1, float *c2)
{
    float e1;
    TwoSum(a1,b1,c1,&e1);
    *c2 = a2 + b2 + e1 + a3 + b3 + b4;
}

//add: 3-4-3
void add_QTW_QQW_QTW(float a1, float a2, float a3, float b1, float b2, float b3, float b4, float *c1, float *c2, float *c3)
{
    float e1, e2, e3;
    TwoSum(a1,b1,c1,&e1);
    TwoSum(a2,b2,c2,&e2);
    TwoSum(*c2,e1,c2,&e3);
    *c3 = a3 + b3 + e2 + e3 + b4;
}

//add: 3-4-4
void add_QTW_QQW_QQW(float a1, float a2, float a3, float b1, float b2, float b3, float b4, float *c1, float *c2, float *c3, float *c4)
{
    float e1, e2, e3, e4, e5, e6;
    TwoSum(a1,b1,c1,&e1);
    TwoSum(a2,b2,c2,&e2);
    TwoSum(*c2,e1,c2,&e3);
    TwoSum(a3,b3,c3,&e4);
    TwoSum(*c3,e2,c3,&e5);
    TwoSum(*c3,e3,c3,&e6);
    *c4 = b4 + e4 + e5 + e6;
}

//add: 4-1-1
void add_QQW_SW_SW(float a1, float a2, float a3, float a4, float b, float *c)
{
    *c = a1 + b + a2 + a3 + a4;
}

//add: 4-1-2
void add_QQW_SW_PA(float a1, float a2, float a3, float a4, float b, float *c1, float *c2)
{
    float e1;
    TwoSum(a1,b,c1,&e1);
    *c2 = a2 + e1 + a3 + a4;
}

//add: 4-1-3
void add_QQW_SW_QTW(float a1, float a2, float a3, float a4, float b, float *c1, float *c2, float *c3)
{
    float e1, e2;
    TwoSum(a1,b,c1,&e1);
    TwoSum(a2,e1,c2,&e2);
    *c3 = a3 + e2 + a4;
}

//add: 4-1-4
void add_QQW_SW_QQW(float a1, float a2, float a3, float a4, float b, float *c1, float *c2, float *c3, float *c4)
{
    float e1, e2, e3;
    TwoSum(a1,b,c1,&e1);
    TwoSum(a2,e1,c2,&e2);
    TwoSum(a3,e2,c3,&e3);
    *c4 = a4 + e3;
}

//add: 4-2-1
void add_QQW_PA_SW(float a1, float a2, float a3, float a4, float b1, float b2, float *c)
{
    *c = a1 + b1 + a2 + b2 + a3 + a4;
}

//add: 4-2-2
void add_QQW_PA_PA(float a1, float a2, float a3, float a4, float b1, float b2, float *c1, float *c2)
{
    float e1;
    TwoSum(a1,b1,c1,&e1);
    *c2 = a2 + b2 + e1 + a3 + a4;
}

//add: 4-2-3
void add_QQW_PA_QTW(float a1, float a2, float a3, float a4, float b1, float b2, float *c1, float *c2, float *c3)
{
    float e1, e2, e3;
    TwoSum(a1,b1,c1,&e1);
    TwoSum(a2,b2,c2,&e2);
    TwoSum(*c2,e1,c2,&e3);
    *c3 = a3 + e2 + e3 + a4;
}

//add: 4-2-4
void add_QQW_PA_QQW(float a1, float a2, float a3, float a4, float b1, float b2, float *c1, float *c2, float *c3, float *c4)
{
    float e1, e2, e3, e5, e6;
    TwoSum(a1,b1,c1,&e1);
    TwoSum(a2,b2,c2,&e2);
    TwoSum(*c2,e1,c2,&e3);
    TwoSum(a3,e2,c3,&e5);
    TwoSum(*c3,e3,c3,&e6);
    *c4 = a4 + e5 + e6;
}

//add: 4-3-1
void add_QQW_QTW_SW(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float *c)
{
    *c = a1 + b1 + a2 + b2 + a3 + b3 + a4;
}

//add: 4-3-2
void add_QQW_QTW_PA(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float *c1, float *c2)
{
    float e1;
    TwoSum(a1,b1,c1,&e1);
    *c2 = a2 + b2 + e1 + a3 + b3 + a4;
}

//add: 4-3-3
void add_QQW_QTW_QTW(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float *c1, float *c2, float *c3)
{
    float e1, e2, e3;
    TwoSum(a1,b1,c1,&e1);
    TwoSum(a2,b2,c2,&e2);
    TwoSum(*c2,e1,c2,&e3);
    *c3 = a3 + b3 + e2 + e3 + a4;
}

//add: 4-3-4
void add_QQW_QTW_QQW(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float *c1, float *c2, float *c3, float *c4)
{
    float e1, e2, e3, e4, e5, e6;
    TwoSum(a1,b1,c1,&e1);
    TwoSum(a2,b2,c2,&e2);
    TwoSum(*c2,e1,c2,&e3);
    TwoSum(a3,b3,c3,&e4);
    TwoSum(*c3,e2,c3,&e5);
    TwoSum(*c3,e3,c3,&e6);
    *c4 = a4 + e4 + e5 + e6;
}

//add: 4-4-1
void add_QQW_QQW_SW(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float *c)
{
    *c = a1 + b1 + a2 + b2 + a3 + b3 + a4 + b4;
}

//add: 4-4-2
void add_QQW_QQW_PA(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float *c1, float *c2)
{
    float e1;
    TwoSum(a1,b1,c1,&e1);
    *c2 = a2 + b2 + e1 + a3 + b3 + a4 + b4;
}

//add: 4-4-3
void add_QQW_QQW_QTW(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float *c1, float *c2, float *c3)
{
    float e1, e2, e3;
    TwoSum(a1,b1,c1,&e1);
    TwoSum(a2,b2,c2,&e2);
    TwoSum(*c2,e1,c2,&e3);
    *c3 = a3 + b3 + e2 + e3 + a4 + b4;
}

//add: 4-4-4
void add_QQW_QQW_QQW(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float *c1, float *c2, float *c3, float *c4)
{
    float e1, e2, e3, e4, e5, e6;
    TwoSum(a1,b1,c1,&e1);
    TwoSum(a2,b2,c2,&e2);
    TwoSum(*c2,e1,c2,&e3);
    TwoSum(a3,b3,c3,&e4);
    TwoSum(*c3,e2,c3,&e5);
    TwoSum(*c3,e3,c3,&e6);
    *c4 = a4 + b4 + e4 + e5 + e6;
}

//------------------------------
//subtraction
//------------------------------

//sub: 1-1-2
void sub_SW_SW_PA(float a, float b, float *ch, float *cl)
{
    TwoSum(a,-b,ch,cl);
}

//sub: 1-1-3
void sub_SW_SW_QTW(float a, float b, float *c1, float *c2, float *c3)
{
    add_SW_SW_QTW(a, -b, c1, c2, c3);
}

//sub: 1-1-4
void sub_SW_SW_QQW(float a, float b, float *c1, float *c2, float *c3, float *c4)
{
    add_SW_SW_QQW(a, -b, c1, c2, c3, c4);
}

//sub: 1-2-1
void sub_SW_PA_SW(float ah, float bh, float bl, float *c)
{
    *c = ah - bh - bl;
}

//sub: 1-2-2
void sub_SW_PA_PA(float a, float bh, float bl, float *ch, float *cl)
{
    add_SW_PA_PA(a, -bh, -bl, ch, cl);
}

//sub: 1-2-3
void sub_SW_PA_QTW(float a, float bh, float bl, float *c1, float *c2, float *c3)
{
    add_SW_PA_QTW(a, -bh, -bl, c1, c2, c3);
}

//sub: 1-2-4
void sub_SW_PA_QQW(float a, float bh, float bl, float *c1, float *c2, float *c3, float *c4)
{
    add_SW_PA_QQW(a, -bh, -bl, c1, c2, c3, c4);
}

//sub: 1-3-1
void sub_SW_QTW_SW(float a, float b1, float b2, float b3, float *c)
{
    add_SW_QTW_SW(a, -b1, -b2, -b3, c);
}

//sub: 1-3-2
void sub_SW_QTW_PA(float a, float b1, float b2, float b3, float *c1, float *c2)
{
    add_SW_QTW_PA(a, -b1, -b2, -b3, c1, c2);
}

//sub: 1-3-3
void sub_SW_QTW_QTW(float a, float b1, float b2, float b3, float *c1, float *c2, float *c3)
{
    add_SW_QTW_QTW(a, -b1, -b2, -b3, c1, c2, c3);
}

//sub: 1-3-4
void sub_SW_QTW_QQW(float a, float b1, float b2, float b3, float *c1, float *c2, float *c3, float *c4)
{
    add_SW_QTW_QQW(a, -b1, -b2, -b3, c1, c2, c3, c4);
}

//sub: 1-4-1
void sub_SW_QQW_SW(float a, float b1, float b2, float b3, float b4, float *c)
{
    add_SW_QQW_SW(a, -b1, -b2, -b3, -b4, c);
}

//sub: 1-4-2
void sub_SW_QQW_PA(float a, float b1, float b2, float b3, float b4, float *c1, float *c2)
{
    add_SW_QQW_PA(a, -b1, -b2, -b3, -b4, c1, c2);
}

//sub: 1-4-3
void sub_SW_QQW_QTW(float a, float b1, float b2, float b3, float b4, float *c1, float *c2, float *c3)
{
    add_SW_QQW_QTW(a, -b1, -b2, -b3, -b4, c1, c2, c3);
}

//sub: 1-4-4
void sub_SW_QQW_QQW(float a, float b1, float b2, float b3, float b4, float *c1, float *c2, float *c3, float *c4)
{
    add_SW_QQW_QQW(a, -b1, -b2, -b3, -b4, c1, c2, c3, c4);
}

//sub: 2-1-1
void sub_PA_SW_SW(float ah, float al, float b, float *c)
{
    *c = ah - b + al;
}

//sub: 2-1-2
void sub_PA_SW_PA(float ah, float al, float b, float *ch, float *cl)
{
    add_PA_SW_PA(ah, al, -b, ch, cl);
}

//sub: 2-1-3
void sub_PA_SW_QTW(float ah, float al, float b, float *c1, float *c2, float *c3)
{
    add_PA_SW_QTW(ah, al, -b, c1, c2, c3);
}

//sub: 2-1-4
void sub_PA_SW_QQW(float ah, float al, float b, float *c1, float *c2, float *c3, float *c4)
{
    add_PA_SW_QQW(ah, al, -b, c1, c2, c3, c4);
}

//sub: 2-2-1
void sub_PA_PA_SW(float ah, float al, float bh, float bl, float *c)
{
    float t;
    TwoSum(ah,-bh,c,&t);
    *c = *c + al - bl + t;
}

//sub: 2-2-2
void sub_PA_PA_PA(float ah, float al, float bh, float bl, float *ch, float *cl)
{
    add_PA_PA_PA(ah, al, -bh, -bl, ch, cl);
}

//sub: 2-2-3
void sub_PA_PA_QTW(float ah, float al, float bh, float bl, float *c1, float *c2, float *c3)
{
    add_PA_PA_QTW(ah, al, -bh, -bl, c1, c2, c3);
}

//sub: 2-2-4
void sub_PA_PA_QQW(float ah, float al, float bh, float bl, float *c1, float *c2, float *c3, float *c4)
{
    add_PA_PA_QQW(ah, al, -bh, -bl, c1, c2, c3, c4);
}

//sub: 2-3-1
void sub_PA_QTW_SW(float ah, float al, float b1, float b2, float b3, float *c)
{
    add_PA_QTW_SW(ah, al, -b1, -b2, -b3, c);
}

//sub: 2-3-2
void sub_PA_QTW_PA(float ah, float al, float b1, float b2, float b3, float *c1, float *c2)
{
    add_PA_QTW_PA(ah, al, -b1, -b2, -b3, c1, c2);
}

//sub: 2-3-3
void sub_PA_QTW_QTW(float ah, float al, float b1, float b2, float b3, float *c1, float *c2, float *c3)
{
    add_PA_QTW_QTW(ah, al, -b1, -b2, -b3, c1, c2, c3);
}

//sub: 2-3-4
void sub_PA_QTW_QQW(float ah, float al, float b1, float b2, float b3, float *c1, float *c2, float *c3, float *c4)
{
    add_PA_QTW_QQW(ah, al, -b1, -b2, -b3, c1, c2, c3, c4);
}

//sub: 2-4-1
void sub_PA_QQW_SW(float ah, float al, float b1, float b2, float b3, float b4, float *c)
{
    add_PA_QQW_SW(ah, al, -b1, -b2, -b3, -b4, c);
}

//sub: 2-4-2
void sub_PA_QQW_PA(float ah, float al, float b1, float b2, float b3, float b4, float *c1, float *c2)
{
    add_PA_QQW_PA(ah, al, -b1, -b2, -b3, -b4, c1, c2);
}

//sub: 2-4-3
void sub_PA_QQW_QTW(float ah, float al, float b1, float b2, float b3, float b4, float *c1, float *c2, float *c3)
{
    add_PA_QQW_QTW(ah, al, -b1, -b2, -b3, -b4, c1, c2, c3);
}

//sub: 2-4-4
void sub_PA_QQW_QQW(float ah, float al, float b1, float b2, float b3, float b4, float *c1, float *c2, float *c3, float *c4)
{
    add_PA_QQW_QQW(ah, al, -b1, -b2, -b3, -b4, c1, c2, c3, c4);
}

//sub: 3-1-1
void sub_QTW_SW_SW(float a1, float a2, float a3, float b, float *c)
{
    add_QTW_SW_SW(a1, a2, a3, -b, c);
}

//sub: 3-1-2
void sub_QTW_SW_PA(float a1, float a2, float a3, float b, float *c1, float *c2)
{
    add_QTW_SW_PA(a1, a2, a3, -b, c1, c2);
}

//sub: 3-1-3
void sub_QTW_SW_QTW(float a1, float a2, float a3, float b, float *c1, float *c2, float *c3)
{
    add_QTW_SW_QTW(a1, a2, a3, -b, c1, c2, c3);
}

//sub: 3-1-4
void sub_QTW_SW_QQW(float a1, float a2, float a3, float b, float *c1, float *c2, float *c3, float *c4)
{
    add_QTW_SW_QQW(a1, a2, a3, -b, c1, c2, c3, c4);
}

//sub: 3-2-1
void sub_QTW_PA_SW(float a1, float a2, float a3, float b1, float b2, float *c)
{
    add_QTW_PA_SW(a1, a2, a3, -b1, -b2, c);
}

//sub: 3-2-2
void sub_QTW_PA_PA(float a1, float a2, float a3, float b1, float b2, float *c1, float *c2)
{
    add_QTW_PA_PA(a1, a2, a3, -b1, -b2, c1, c2);
}

//sub: 3-2-3
void sub_QTW_PA_QTW(float a1, float a2, float a3, float b1, float b2, float *c1, float *c2, float *c3)
{
    add_QTW_PA_QTW(a1, a2, a3, -b1, -b2, c1, c2, c3);
}

//sub: 3-2-4
void sub_QTW_PA_QQW(float a1, float a2, float a3, float b1, float b2, float *c1, float *c2, float *c3, float *c4)
{
    add_QTW_PA_QQW(a1, a2, a3, -b1, -b2, c1, c2, c3, c4);
}

//sub: 3-3-1
void sub_QTW_QTW_SW(float a1, float a2, float a3, float b1, float b2, float b3, float *c)
{
    add_QTW_QTW_SW(a1, a2, a3, -b1, -b2, -b3, c);
}

//sub: 3-3-2
void sub_QTW_QTW_PA(float a1, float a2, float a3, float b1, float b2, float b3, float *c1, float *c2)
{
    add_QTW_QTW_PA(a1, a2, a3, -b1, -b2, -b3, c1, c2);
}

//sub: 3-3-3
void sub_QTW_QTW_QTW(float a1, float a2, float a3, float b1, float b2, float b3, float *c1, float *c2, float *c3)
{
    add_QTW_QTW_QTW(a1, a2, a3, -b1, -b2, -b3, c1, c2, c3);
}

//sub: 3-3-4
void sub_QTW_QTW_QQW(float a1, float a2, float a3, float b1, float b2, float b3, float *c1, float *c2, float *c3, float *c4)
{
    add_QTW_QTW_QQW(a1, a2, a3, -b1, -b2, -b3, c1, c2, c3, c4);
}


//sub: 3-4-1
void sub_QTW_QQW_SW(float a1, float a2, float a3, float b1, float b2, float b3, float b4, float *c)
{
    add_QTW_QQW_SW(a1, a2, a3, -b1, -b2, -b3, -b4, c);
}

//sub: 3-4-2
void sub_QTW_QQW_PA(float a1, float a2, float a3, float b1, float b2, float b3, float b4, float *c1, float *c2)
{
    add_QTW_QQW_PA(a1, a2, a3, -b1, -b2, -b3, -b4, c1, c2);
}

//sub: 3-4-3
void sub_QTW_QQW_QTW(float a1, float a2, float a3, float b1, float b2, float b3, float b4, float *c1, float *c2, float *c3)
{
    add_QTW_QQW_QTW(a1, a2, a3, -b1, -b2, -b3, -b4, c1, c2, c3);
}

//sub: 3-4-4
void sub_QTW_QQW_QQW(float a1, float a2, float a3, float b1, float b2, float b3, float b4, float *c1, float *c2, float *c3, float *c4)
{
    add_QTW_QQW_QQW(a1, a2, a3, -b1, -b2, -b3, -b4, c1, c2, c3, c4);
}

//sub: 4-1-1
void sub_QQW_SW_SW(float a1, float a2, float a3, float a4, float b, float *c)
{
    add_QQW_SW_SW(a1, a2, a3, a4, -b, c);
}

//sub: 4-1-2
void sub_QQW_SW_PA(float a1, float a2, float a3, float a4, float b, float *c1, float *c2)
{
    add_QQW_SW_PA(a1, a2, a3, a4, -b, c1, c2);
}

//sub: 4-1-3
void sub_QQW_SW_QTW(float a1, float a2, float a3, float a4, float b, float *c1, float *c2, float *c3)
{
    add_QQW_SW_QTW(a1, a2, a3, a4, -b, c1, c2, c3);
}

//sub: 4-1-4
void sub_QQW_SW_QQW(float a1, float a2, float a3, float a4, float b, float *c1, float *c2, float *c3, float *c4)
{
    add_QQW_SW_QQW(a1, a2, a3, a4, -b, c1, c2, c3, c4);
}


//sub: 4-2-1
void sub_QQW_PA_SW(float a1, float a2, float a3, float a4, float b1, float b2, float *c)
{
    add_QQW_PA_SW(a1, a2, a3, a4, -b1, -b2, c);
}

//sub: 4-2-2
void sub_QQW_PA_PA(float a1, float a2, float a3, float a4, float b1, float b2, float *c1, float *c2)
{
    add_QQW_PA_PA(a1, a2, a3, a4, -b1, -b2, c1, c2);
}

//sub: 4-2-3
void sub_QQW_PA_QTW(float a1, float a2, float a3, float a4, float b1, float b2, float *c1, float *c2, float *c3)
{
    add_QQW_PA_QTW(a1, a2, a3, a4, -b1, -b2, c1, c2, c3);
}

//sub: 4-2-4
void sub_QQW_PA_QQW(float a1, float a2, float a3, float a4, float b1, float b2, float *c1, float *c2, float *c3, float *c4)
{
    add_QQW_PA_QQW(a1, a2, a3, a4, -b1, -b2, c1, c2, c3, c4);
}

//sub: 4-3-1
void sub_QQW_QTW_SW(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float *c)
{
    add_QQW_QTW_SW(a1, a2, a3, a4, -b1, -b2, -b3, c);
}

//sub: 4-3-2
void sub_QQW_QTW_PA(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float *c1, float *c2)
{
    add_QQW_QTW_PA(a1, a2, a3, a4, -b1, -b2, -b3, c1, c2);
}

//sub: 4-3-3
void sub_QQW_QTW_QTW(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float *c1, float *c2, float *c3)
{
    add_QQW_QTW_QTW(a1, a2, a3, a4, -b1, -b2, -b3, c1, c2, c3);
}

//sub: 4-3-4
void sub_QQW_QTW_QQW(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float *c1, float *c2, float *c3, float *c4)
{
    add_QQW_QTW_QQW(a1, a2, a3, a4, -b1, -b2, -b3, c1, c2, c3, c4);
}

//sub: 4-4-1
void sub_QQW_QQW_SW(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float *c)
{
    add_QQW_QQW_SW(a1, a2, a3, a4, -b1, -b2, -b3, -b4, c);
}

//sub: 4-4-2
void sub_QQW_QQW_PA(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float *c1, float *c2)
{
    add_QQW_QQW_PA(a1, a2, a3, a4, -b1, -b2, -b3, -b4, c1, c2);
}

//sub: 4-4-3
void sub_QQW_QQW_QTW(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float *c1, float *c2, float *c3)
{
    add_QQW_QQW_QTW(a1, a2, a3, a4, -b1, -b2, -b3, -b4, c1, c2, c3);
}

//sub: 4-4-4
void sub_QQW_QQW_QQW(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float *c1, float *c2, float *c3, float *c4)
{
    add_QQW_QQW_QQW(a1, a2, a3, a4, -b1, -b2, -b3, -b4, c1, c2, c3, c4);
}

//------------------------------
//multiplication
//------------------------------

//mul: 1-1-2
void mul_SW_SW_PA(float a, float b, float *ch, float *cl)
{
    TwoProductFMA(a,b,ch,cl);
}

//mul: 1-1-3
void mul_SW_SW_QTW(float a, float b, float *c1, float *c2, float *c3)
{
    TwoProductFMA(a, b, c1, c2);
    *c3 = 0;
}

//mul: 1-1-4
void mul_SW_SW_QQW(float a, float b, float *c1, float *c2, float *c3, float *c4)
{
    TwoProductFMA(a,b,c1,c2);
    *c3 = 0;
    *c4 = 0;
}

//mul: 1-2-1
void mul_SW_PA_SW(float a, float bh, float bl, float *c)
{
    *c = a*(bh + bl);
}

//mul: 1-2-2
void mul_SW_PA_PA(float a, float bh, float bl, float *ch, float *cl)
{
    float t;
    TwoProductFMA(a,bh,ch,&t);
    *cl = fma(a,bl,t);
}

//mul: 1-2-3
void mul_SW_PA_QTW(float a, float b1, float b2, float *c1, float *c2, float *c3)
{
    float e1, e3, e5, t3;
    TwoProductFMA(a, b1, c1, &e1);
    TwoProductFMA(a, b2, &t3, &e3);
    TwoSum(t3, e1, c2, &e5);
    *c3 = e3 + e5;
}

//mul: 1-2-4
void mul_SW_PA_QQW(float a, float b1, float b2, float *c1, float *c2, float *c3, float *c4)
{
    float t1, e1, e2, e3;
    TwoProductFMA(a,b1,c1,&e1);
    TwoProductFMA(a,b2,&t1,&e2);
    TwoSum(e1,t1,c2,&e3);
    *c3 = e2 + e3;
    *c4 = 0;
}

//mul: 1-3-1
void mul_SW_QTW_SW(float a, float b1, float b2, float b3, float *c)
{
    *c = a * (b3 + b2 + b1);
}

//mul: 1-3-2
void mul_SW_QTW_PA(float a, float b1, float b2, float b3, float *c1, float *c2)
{
    float e1;
    TwoProductFMA(a, b1, c1, &e1);
    *c2 = fma(a, b2 + b3, e1);
}

//mul: 1-3-3
void mul_SW_QTW_QTW(float a, float b1, float b2, float b3, float *c1, float *c2, float *c3)
{
    float e1, e2, e4, e5, t2;
    TwoProductFMA(a, b1, c1, &e1);
    TwoProductFMA(a, b2, &t2, &e2);
    TwoSum(t2, e1, c2, &e5);
    *c3 = e2 + fma(a,b3,e5);
}

//mul: 1-3-4
void mul_SW_QTW_QQW(float a, float b1, float b2, float b3, float *c1, float *c2, float *c3, float *c4)
{
    float t1, t2, t3;
    float e1, e2, e3, e4, e5, e6;
    TwoProductFMA(a,b1,c1,&e1);
    TwoProductFMA(a,b2,&t1,&e2);
    TwoSum(e1,t1,c2,&e3);
    TwoSum(e2,e3,&t2,&e4);
    TwoProductFMA(a,b3,&t3,&e5);
    TwoSum(t2,t3,c3,&e6);
    *c4 = e4 + e5 + e6;
}

//mul: 1-4-1
void mul_SW_QQW_SW(float a1, float b1, float b2, float b3, float b4, float *c)
{
//    *c = a1 + b1 + b2 + b3 + b4;
    *c = a1 * (b4 + b3 + b2 + b1); //
}

//mul: 1-4-2
void mul_SW_QQW_PA(float a,float b1,float b2,float b3,float b4,float *c1,float *c2)
{
    float e1;
    TwoProductFMA(a,b1,c1,&e1);
    *c2 = fma(a,b2,e1);
}

//mul: 1-4-3
void mul_SW_QQW_QTW(float a,float b1,float b2,float b3,float b4,float *c1,float *c2,float *c3)
{
    float e1, e2, e5, e6;
    float t2, t4;

    TwoProductFMA(a,b1,c1,&e1);
    TwoProductFMA(a,b2,&t2,&e2);
    TwoSum(t2,e1,c2,&e5);
    TwoProductFMA(a,b3,&t4,&e6);
    *c3 = t4 + e2 + e5;
}

//mul: 1-4-4
void mul_SW_QQW_QQW(float a,float b1,float b2,float b3,float b4,float *c1,float *c2,float *c3,float *c4)
{
    float e1, e2, e5, e6, e11, e14;
    float t2, t4;

    TwoProductFMA(a,b1,c1,&e1);
    TwoProductFMA(a,b2,&t2,&e2);
    TwoSum(t2,e1,c2,&e5);
    TwoProductFMA(a,b3,&t4,&e6);
    TwoSum(t4,e2,c3,&e11);
    TwoSum(*c3,e5,c3,&e14);
    *c4 = fma(a,b4,e6) + e11 + e14;
}

//mul: 2-1-1
void mul_PA_SW_SW(float ah, float al, float b, float *c)
{
    *c = (ah+al)*b;
}

//mul: 2-1-2
void mul_PA_SW_PA(float ah, float al, float b, float *ch, float *cl)
{
    float t;
    TwoProductFMA(ah,b,ch,&t);
    *cl = fma(b,al,t);
}

//mul: 2-1-3
void mul_PA_SW_QTW(float a1, float a2, float b, float *c1, float *c2, float *c3)
{
    float e1, e3, e4, t3;
    TwoProductFMA(a1, b, c1, &e1);
    TwoProductFMA(a2, b, &t3, &e3);
    TwoSum(e1, t3, c2, &e4);
    *c3 = e3 + e4;
}

//mul: 2-1-4
void mul_PA_SW_QQW(float a1,float a2,float b,float *c1,float *c2,float *c3,float *c4)
{
    float e1, e3, e5;
    float t3;
    
    TwoProductFMA(a1,b,c1,&e1);
    TwoProductFMA(a2,b,&t3,&e3);
    TwoSum(t3,e1,c2,&e5);
    TwoSum(e3,e5,c3,c4);
}

//mul: 2-2-1
void mul_PA_PA_SW(float ah, float al, float bh, float bl, float *c)
{
    *c = fma(al,bh,fma(ah,bh,ah*bl)); 
}

//mul: 2-2-2
void mul_PA_PA_PA(float ah, float al, float bh, float bl, float *ch, float *cl)
{
    float t;
    TwoProductFMA(ah,bh,ch,&t);
    *cl = fma(ah,bl,fma(bh,al,t));
}

//mul: 2-2-3
void mul_PA_PA_QTW(float a1, float a2, float b1, float b2, float *c1, float *c2, float *c3)
{
    float e1, e2, e3, e4, e5, t2, t3;
    TwoProductFMA(a1, b1, c1, &e1);
    TwoProductFMA(a1, b2, &t2, &e2);
    TwoProductFMA(a2, b1, &t3, &e3);
    TwoSum(t2, t3, c2, &e4);
    TwoSum(*c2, e1, c2, &e5);
    *c3 = e2 + fma(a2,b2,e3) + e4 + e5;
}

//mul: 2-2-4
void mul_PA_PA_QQW(float a1,float a2,float b1,float b2,float *c1,float *c2,float *c3,float *c4)
{
    float e1, e2, e3, e4, e5;
    float e7, e11, e12, e13, e14;
    float t2, t3, t5;
    
    TwoProductFMA(a1,b1,c1,&e1);
    TwoProductFMA(a1,b2,&t2,&e2);
    TwoProductFMA(a2,b1,&t3,&e3);
    TwoSum(t2,t3,c2,&e4);
    TwoSum(*c2,e1,c2,&e5);
    TwoProductFMA(a2,b2,&t5,&e7);
    TwoSum(t5,e2,c3,&e11);
    TwoSum(*c3,e3,c3,&e12);
    TwoSum(*c3,e4,c3,&e13);
    TwoSum(*c3,e5,c3,&e14);
    *c4 = e7 + e11 + e12 + e13 + e14;
}

//mul: 2-3-1
void mul_PA_QTW_SW(float a1, float a2, float b1, float b2, float b3, float *c)
{
    *c = (a1+a2)*(b3+b2+b1);
}

//mul: 2-3-2
void mul_PA_QTW_PA(float a1, float a2, float b1, float b2, float b3, float *c1, float *c2)
{
    float e1;
    TwoProductFMA(a1, b1, c1, &e1);
    *c2 = fma(a2,b1,fma(a1,b2,e1));
}

//mul: 2-3-3
void mul_PA_QTW_QTW(float a1, float a2, float b1, float b2, float b3, float *c1, float *c2, float *c3)
{
    float e1, e2, e3, e4, e5, t2, t3;
    TwoProductFMA(a1, b1, c1, &e1);
    TwoProductFMA(a1, b2, &t2, &e2);
    TwoProductFMA(a2, b1, &t3, &e3);
    TwoSum(t2, t3, c2, &e4);
    TwoSum(*c2, e1, c2, &e5);
    *c3 = e2 + fma(a2,b2,e3) + fma(a1,b3,e4) + e5;
}

//mul: 2-3-4
void mul_PA_QTW_QQW(float a1,float a2,float b1,float b2,float b3,float *c1,float *c2,float *c3,float *c4)
{
    float e1, e2, e3, e4, e5;
    float e6, e7, e9, e11, e12, e13, e14;
    float t2, t3, t4, t5;
    
    TwoProductFMA(a1,b1,c1,&e1);
    TwoProductFMA(a1,b2,&t2,&e2);
    TwoProductFMA(a2,b1,&t3,&e3);
    TwoSum(t2,t3,c2,&e4);
    TwoSum(*c2,e1,c2,&e5);
    TwoProductFMA(a1,b3,&t4,&e6);
    TwoProductFMA(a2,b2,&t5,&e7);
    TwoSum(t4,t5,c3,&e9);
    TwoSum(*c3,e2,c3,&e11);
    TwoSum(*c3,e3,c3,&e12);
    TwoSum(*c3,e4,c3,&e13);
    TwoSum(*c3,e5,c3,&e14);
    *c4 = fma(a2,b3,e6) + e7 + e9 + e11 + e12 + e13 + e14;
}

//mul: 2-4-1
void mul_PA_QQW_SW(float a1, float a2, float b1, float b2, float b3, float b4, float *c)
{
//    *c = a1 + b1 + a2 + b2 + b3 + b4;
    *c = (a1 + a2) * (b1 + b2 + b3 + b4); //
}

//mul: 2-4-2
void mul_PA_QQW_PA(float a1,float a2,float b1,float b2,float b3,float b4,float *c1,float *c2)
{
    float e1;
    TwoProductFMA(a1,b1,c1,&e1);
    *c2 = fma(a1,b2,fma(a2,b1,e1));
}

//mul: 2-4-3
void mul_PA_QQW_QTW(float a1,float a2,float b1,float b2,float b3,float b4,float *c1,float *c2,float *c3)
{
    float e1, e2, e3, e4, e5;
    float t2, t3;
    TwoProductFMA(a1,b1,c1,&e1);
    TwoProductFMA(a1,b2,&t2,&e2);
    TwoProductFMA(a2,b1,&t3,&e3);
    TwoSum(t2,t3,c2,&e4);
    TwoSum(*c2,e1,c2,&e5);
    *c3 = fma(a1,b3,fma(a2,b2,e2)) + e3 + e4 + e5;
}

//mul: 2-4-4
void mul_PA_QQW_QQW(float a1,float a2,float b1,float b2,float b3,float b4,float *c1,float *c2,float *c3,float *c4)
{
    float e1, e2, e3, e4, e5;
    float e6, e7, e9;
    float e11, e12, e13, e14;
    float t2, t3, t4, t5;

    TwoProductFMA(a1,b1,c1,&e1);
    TwoProductFMA(a1,b2,&t2,&e2);
    TwoProductFMA(a2,b1,&t3,&e3);
    TwoSum(t2,t3,c2,&e4);
    TwoSum(*c2,e1,c2,&e5);
    TwoProductFMA(a1,b3,&t4,&e6);
    TwoProductFMA(a2,b2,&t5,&e7);
    TwoSum(t4,t5,c3,&e9);
    TwoSum(*c3,e2,c3,&e11);
    TwoSum(*c3,e3,c3,&e12);
    TwoSum(*c3,e4,c3,&e13);
    TwoSum(*c3,e5,c3,&e14);
    *c4 = fma(a2,b3,a1*b4) + e6 + e7 + e9 + e11 + e12 + e13 + e14;
}

//mul: 3-1-1
void mul_QTW_SW_SW(float a1, float a2, float a3, float b, float *c)
{
    *c = (a3+a2+a1)*b;
}

//mul: 3-1-2
void mul_QTW_SW_PA(float a1, float a2, float a3, float b, float *c1, float *c2)
{
    float e1;
    TwoProductFMA(a1, b, c1, &e1);
    *c2 = fma(a2,b,e1);
}

//mul: 3-1-3
void mul_QTW_SW_QTW(float a1, float a2, float a3, float b, float *c1, float *c2, float *c3)
{
    float e1, e3, e5;
    float t3;
    TwoProductFMA(a1, b, c1, &e1);
    TwoProductFMA(a2, b, &t3, &e3);
    TwoSum(t3, e1, c2, &e5);
    *c3 = fma(a3,b,e3) + e5;
}

//mul: 3-1-4
void mul_QTW_SW_QQW(float a1,float a2,float a3,float b,float *c1,float *c2,float *c3,float *c4)
{
    float e1, e3, e5, e8, e12, e14;
    float t3, t4, t5, t6;
    
    TwoProductFMA(a1,b,c1,&e1);
    TwoProductFMA(a2,b,&t3,&e3);
    TwoSum(t3,e1,c2,&e5);
    TwoProductFMA(a3,b,&t6,&e8);
    TwoSum(t6,e3,c3,&e12);
    TwoSum(*c3,e5,c3,&e14);
    *c4 = e8 + e12 + e14;
}

//mul: 3-2-1
void mul_QTW_PA_SW(float a1, float a2, float a3, float b1, float b2, float *c)
{
    *c = (a3+a2+a1)*(b1+b2);
}

//mul: 3-2-2
void mul_QTW_PA_PA(float a1, float a2, float a3, float b1, float b2, float *c1, float *c2)
{
    float e1;
    TwoProductFMA(a1, b1, c1, &e1);
    *c2 = fma(a2+a3,b1,fma(a1,b2,e1));
}

//mul: 3-2-3
void mul_QTW_PA_QTW(float a1, float a2, float a3, float b1, float b2, float *c1, float *c2, float *c3)
{
    float e1, e2, e3, e4, e5, t2, t3;
    TwoProductFMA(a1, b1, c1, &e1);
    TwoProductFMA(a1, b2, &t2, &e2);
    TwoProductFMA(a2, b1, &t3, &e3);
    TwoSum(t2, t3, c2, &e4);
    TwoSum(*c2, e1, c2, &e5);
    *c3 = fma(a3,b1,e2) + fma(a2,b2,e3) + e4 + e5;
}

//mul: 3-2-4
void mul_QTW_PA_QQW(float a1,float a2,float a3,float b1,float b2,float *c1,float *c2,float *c3,float *c4)
{
    float e1, e2, e3, e4, e5;
    float e7, e8, e10;
    float e11, e12, e13, e14;
    float t2, t3, t5, t6;
    
    TwoProductFMA(a1,b1,c1,&e1);
    TwoProductFMA(a1,b2,&t2,&e2);
    TwoProductFMA(a2,b1,&t3,&e3);
    TwoSum(t2,t3,c2,&e4);
    TwoSum(*c2,e1,c2,&e5);
    TwoProductFMA(a2,b2,&t5,&e7);
    TwoProductFMA(a3,b1,&t6,&e8);
    TwoSum(t5,t6,c3,&e10);
    TwoSum(*c3,e2,c3,&e11);
    TwoSum(*c3,e3,c3,&e12);
    TwoSum(*c3,e4,c3,&e13);
    TwoSum(*c3,e5,c3,&e14);
    *c4 = fma(a3,b2,e7) + e8 + e10 + e11 + e12 + e13 + e14;
}

//mul: 3-3-1
void mul_QTW_QTW_SW(float a1, float a2, float a3, float b1, float b2, float b3, float *c)
{
    *c = (a1+a2+a3)*(b1+b2+b3); //減らしてもよい
}

//mul: 3-3-2
void mul_QTW_QTW_PA(float a1, float a2, float a3, float b1, float b2, float b3, float *c1, float *c2)
{
    float e1;
    TwoProductFMA(a1, b1, c1, &e1);
    *c2 = fma(a2,b1,fma(a1,b2,e1));
}

//mul: 3-3-3
void mul_QTW_QTW_QTW(float a1, float a2, float a3, float b1, float b2, float b3, float *c1, float *c2, float *c3)
{
    float e1, e2, e3, e4, e5;
    float t2, t3;
    TwoProductFMA(a1, b1, c1, &e1);
    TwoProductFMA(a1, b2, &t2, &e2);
    TwoProductFMA(a2, b1, &t3, &e3);
    TwoSum(t2, t3, c2, &e4);
    TwoSum(*c2, e1, c2, &e5);
    *c3 = fma(a3,b1,e2) + fma(a2,b2,e3) + fma(a1,b3,e4) + e5;
}

//mul: 3-3-4
void mul_QTW_QTW_QQW(float a1,float a2,float a3,float b1,float b2,float b3,float *c1,float *c2,float *c3,float *c4)
{
    float e1, e2, e3, e4, e5;
    float e6, e7, e8, e9, e10;
    float e11, e12, e13, e14;
    float t2, t3, t4, t5, t6;
    
    TwoProductFMA(a1,b1,c1,&e1);
    TwoProductFMA(a1,b2,&t2,&e2);
    TwoProductFMA(a2,b1,&t3,&e3);
    TwoSum(t2,t3,c2,&e4);
    TwoSum(*c2,e1,c2,&e5);
    TwoProductFMA(a1,b3,&t4,&e6);
    TwoProductFMA(a2,b2,&t5,&e7);
    TwoProductFMA(a3,b1,&t6,&e8);
    TwoSum(t4,t5,c3,&e9);
    TwoSum(*c3,t6,c3,&e10);
    TwoSum(*c3,e2,c3,&e11);
    TwoSum(*c3,e3,c3,&e12);
    TwoSum(*c3,e4,c3,&e13);
    TwoSum(*c3,e5,c3,&e14);
    *c4 = fma(a3,b2,fma(a2,b3,e6)) + e7 + e8 + e9 + e10 + e11 + e12 + e13 + e14;
}

//mul: 3-4-1
void mul_QTW_QQW_SW(float a1, float a2, float a3, float b1, float b2, float b3, float b4, float *c)
{
//    *c = a1 + b1 + a2 + b2 + a3 + b3 + b4;
    *c = (a3 + a2 + a1) * (b4 + b3 + b2 + b1); //
}

//mul: 3-4-2
void mul_QTW_QQW_PA(float a1,float a2,float a3,float b1,float b2,float b3,float b4,float *c1,float *c2)
{
    float e1;
    TwoProductFMA(a1,b1,c1,&e1);
    *c2 = fma(a1,b2,fma(a2,b1,e1));
}

//mul: 3-4-3
void mul_QTW_QQW_QTW(float a1,float a2,float a3,float b1,float b2,float b3,float b4,float *c1,float *c2,float *c3)
{
    float e1, e2, e3, e4, e5;
    float t2, t3;

    TwoProductFMA(a1,b1,c1,&e1);
    TwoProductFMA(a1,b2,&t2,&e2);
    TwoProductFMA(a2,b1,&t3,&e3);
    TwoSum(t2,t3,c2,&e4);
    TwoSum(*c2,e1,c2,&e5);
    *c3 = fma(a3,b1,fma(a1,b3,a2*b2)) + e2 + e3 + e4 + e5;
}

//mul: 3-4-4
void mul_QTW_QQW_QQW(float a1,float a2,float a3,float b1,float b2,float b3,float b4,float *c1,float *c2,float *c3,float *c4)
{
    float e1, e2, e3, e4, e5;
    float e6, e7, e8, e9, e10;
    float e11, e12, e13, e14;
    float t2, t3, t4, t5, t6;

    TwoProductFMA(a1,b1,c1,&e1);
    TwoProductFMA(a1,b2,&t2,&e2);
    TwoProductFMA(a2,b1,&t3,&e3);
    TwoSum(t2,t3,c2,&e4);
    TwoSum(*c2,e1,c2,&e5);
    TwoProductFMA(a1,b3,&t4,&e6);
    TwoProductFMA(a2,b2,&t5,&e7);
    TwoProductFMA(a3,b1,&t6,&e8);
    TwoSum(t4,t5,c3,&e9);
    TwoSum(*c3,t6,c3,&e10);
    TwoSum(*c3,e2,c3,&e11);
    TwoSum(*c3,e3,c3,&e12);
    TwoSum(*c3,e4,c3,&e13);
    TwoSum(*c3,e5,c3,&e14);
    *c4 = fma(a3,b2,fma(a2,b3,a1*b4)) + e6 + e7 + e8 + e9 + e10 + e11 + e12 + e13 + e14;
}

//mul: 4-1-1
void mul_QQW_SW_SW(float a1, float a2, float a3, float a4, float b, float *c)
{
//    *c = a1 + b + a2  + a3 + a4;
    *c = (a4 + a3  + a2 + a1) * b; //
}

//mul: 4-1-2
void mul_QQW_SW_PA(float a1,float a2,float a3,float a4,float b,float *c1,float *c2)
{
    float e1;
    TwoProductFMA(a1,b,c1,&e1);
    *c2 = fma(a2,b,e1);
}

//mul: 4-1-3
void mul_QQW_SW_QTW(float a1,float a2,float a3,float a4,float b,float *c1,float *c2,float *c3)
{
    float t3;
    float e1, e3, e5;
    TwoProductFMA(a1,b,c1,&e1);
    TwoProductFMA(a2,b,&t3,&e3);
    TwoSum(t3,e1,c2,&e5);
    *c3 = fma(a3,b,e3) + e5;
}

//mul: 4-1-4
void mul_QQW_SW_QQW(float a1,float a2,float a3,float a4,float b,float *c1,float *c2,float *c3,float *c4)
{
    float e1, e3, e5;
    float e8, e12, e14;
    float t2, t3, t4, t5, t6;
    
    TwoProductFMA(a1,b,c1,&e1);
    TwoProductFMA(a2,b,&t3,&e3);
    TwoSum(*c2,e1,c2,&e5);
    TwoProductFMA(a3,b,&t6,&e8);
    TwoSum(t6,e3,c3,&e12);
    TwoSum(*c3,e5,c3,&e14);
    *c4 = fma(a4,b,e8) + e12 + e14;
}

//mul: 4-2-1
void mul_QQW_PA_SW(float a1, float a2, float a3, float a4, float b1, float b2, float *c)
{
//    *c = a1 + b1 + a2 + b2 + a3 + a4;
    *c = (a4 + a3 + a2 + a1) * (b1 + b2);
}

//mul: 4-2-2
void mul_QQW_PA_PA(float a1,float a2,float a3,float a4,float b1,float b2,float *c1,float *c2)
{
    float e1;
    TwoProductFMA(a1,b1,c1,&e1);
    *c2 = fma(a1,b2,fma(a2,b1,e1));
}

//mul: 4-2-3
void mul_QQW_PA_QTW(float a1,float a2,float a3,float a4,float b1,float b2,float *c1,float *c2,float *c3)
{
    float e1, e2, e3, e4, e5;
    float t2, t3;
    
    TwoProductFMA(a1,b1,c1,&e1);
    TwoProductFMA(a1,b2,&t2,&e2);
    TwoProductFMA(a2,b1,&t3,&e3);
    TwoSum(t2,t3,c2,&e4);
    TwoSum(*c2,e1,c2,&e5);
    *c3 = fma(a2,b2,e2) + fma(a3,b1,e3) + e4 + e5;
}

//mul: 4-2-4
void mul_QQW_PA_QQW(float a1,float a2,float a3,float a4,float b1,float b2,float *c1,float *c2,float *c3,float *c4)
{
    float e1, e2, e3, e4, e5;
    float e7, e8, e10;
    float e11, e12, e13, e14;
    float t2, t3, t5, t6;
    
    TwoProductFMA(a1,b1,c1,&e1);
    TwoProductFMA(a1,b2,&t2,&e2);
    TwoProductFMA(a2,b1,&t3,&e3);
    TwoSum(t2,t3,c2,&e4);
    TwoSum(*c2,e1,c2,&e5);
    TwoProductFMA(a2,b2,&t5,&e7);
    TwoProductFMA(a3,b1,&t6,&e8);
    TwoSum(t5,t6,c3,&e10);
    TwoSum(*c3,e2,c3,&e11);
    TwoSum(*c3,e3,c3,&e12);
    TwoSum(*c3,e4,c3,&e13);
    TwoSum(*c3,e5,c3,&e14);
    *c4 = fma(a4,b1,fma(a3,b2,e7)) + e8 + e10 + e11 + e12 + e13 + e14;
}

//mul: 4-3-1
void mul_QQW_QTW_SW(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float *c)
{
//    *c = a1 + b1 + a2 + b2 + a3 + b3 + a4;
    *c = (a4 + a3 + a2 + a1) * (b3 + b2 + b1);
}

//mul: 4-3-2
void mul_QQW_QTW_PA(float a1,float a2,float a3,float a4,float b1,float b2,float b3,float *c1,float *c2)
{
    float e1;
    TwoProductFMA(a1,b1,c1,&e1);
    *c2 = fma(a1,b2,fma(a2,b1,e1));
}

//mul: 4-3-3
void mul_QQW_QTW_QTW(float a1,float a2,float a3,float a4,float b1,float b2,float b3,float *c1,float *c2,float *c3)
{
    float e1, e2, e3, e4, e5;
    float t2, t3;
    
    TwoProductFMA(a1,b1,c1,&e1);
    TwoProductFMA(a1,b2,&t2,&e2);
    TwoProductFMA(a2,b1,&t3,&e3);
    TwoSum(t2,t3,c2,&e4);
    TwoSum(*c2,e1,c2,&e5);
    *c3 = fma(a1,b3,e2) + fma(a2,b2,e3) + fma(a3,b1,e4) + e5;
}

//mul: 4-3-4
void mul_QQW_QTW_QQW(float a1,float a2,float a3,float a4,float b1,float b2,float b3,float *c1,float *c2,float *c3,float *c4)
{
    float e1, e2, e3, e4, e5;
    float e6, e7, e8, e9, e10;
    float e11, e12, e13, e14;
    float t2, t3, t4, t5, t6;
    
    TwoProductFMA(a1,b1,c1,&e1);
    TwoProductFMA(a1,b2,&t2,&e2);
    TwoProductFMA(a2,b1,&t3,&e3);
    TwoSum(t2,t3,c2,&e4);
    TwoSum(*c2,e1,c2,&e5);
    TwoProductFMA(a1,b3,&t4,&e6);
    TwoProductFMA(a2,b2,&t5,&e7);
    TwoProductFMA(a3,b1,&t6,&e8);
    TwoSum(t4,t5,c3,&e9);
    TwoSum(*c3,t6,c3,&e10);
    TwoSum(*c3,e2,c3,&e11);
    TwoSum(*c3,e3,c3,&e12);
    TwoSum(*c3,e4,c3,&e13);
    TwoSum(*c3,e5,c3,&e14);
    *c4 = fma(a4,b1,fma(a3,b2,fma(a2,b3,e6))) + e7 + e8 + e9 + e10 + e11 + e12 + e13 + e14;
}

//mul: 4-4-1
void mul_QQW_QQW_SW(float a1,float a2,float a3,float a4,float b1,float b2,float b3,float b4,float *c)
{
//    *c = a1 + b1 + a2 + b2 + a3 + b3 + a4 + b4;
    *c = (a4 + a3 + a2 + a1) * (b4 + b3 + b2 + b1);
}

//mul: 4-4-2
void mul_QQW_QQW_PA(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float *c1, float *c2)
{
    float e1;
    TwoProductFMA(a1,b1,c1,&e1);
    *c2 = fma(a1,b2,fma(a2,b1,e1));
}

//mul: 4-4-3
void mul_QQW_QQW_QTW(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float *c1, float *c2, float *c3)
{
    float e1, e2, e3, e4, e5;
    float t2, t3;
    
    TwoProductFMA(a1,b1,c1,&e1);
    TwoProductFMA(a1,b2,&t2,&e2);
    TwoProductFMA(a2,b1,&t3,&e3);
    TwoSum(t2,t3,c2,&e4);
    TwoSum(*c2,e1,c2,&e5);
    *c3 = fma(a1,b3,e2) + fma(a2,b2,e3) + fma(a3,b1,e4) + e5;
}

//mul: 4-4-4
void mul_QQW_QQW_QQW(float a1,float a2,float a3,float a4,float b1,float b2,float b3,float b4,float *c1,float *c2,float *c3,float *c4)
{
    float e1, e2, e3, e4, e5;
    float e6, e7, e8, e9, e10;
    float e11, e12, e13, e14;
    float t2, t3, t4, t5, t6;
    
    TwoProductFMA(a1,b1,c1,&e1);
    TwoProductFMA(a1,b2,&t2,&e2);
    TwoProductFMA(a2,b1,&t3,&e3);
    TwoSum(t2,t3,c2,&e4);
    TwoSum(*c2,e1,c2,&e5);
    TwoProductFMA(a1,b3,&t4,&e6);
    TwoProductFMA(a2,b2,&t5,&e7);
    TwoProductFMA(a3,b1,&t6,&e8);
    TwoSum(t4,t5,c3,&e9);
    TwoSum(*c3,t6,c3,&e10);
    TwoSum(*c3,e2,c3,&e11);
    TwoSum(*c3,e3,c3,&e12);
    TwoSum(*c3,e4,c3,&e13);
    TwoSum(*c3,e5,c3,&e14);
    *c4 = fma(a4,b1,fma(a3,b2,fma(a2,b3,a1*b4))) + e6 + e7 + e8 + e9 + e10 + e11 + e12 + e13 + e14;
}

//------------------------------
//Division
//------------------------------

//div: 1-1-2
void div_SW_SW_PA(float a, float b, float *ch, float *cl)
{
    *ch = a/b;
    *cl = fma(-b,*ch,a)/b;
}

//div: 1-1-3
void div_SW_SW_QTW(float a, float b, float *c1, float *c2, float *c3)
{
    float t1, t2, t3;
    div_SW_SW_PA(a,b,c1,c2); //// 3-3
    mul_PA_SW_QTW(*c1,*c2,b,&t1,&t2,&t3);  //// 11-7
    add_SW_QTW_QTW(a,-t1,-t2,-t3,&t1,&t2,&t3); //// 13-5
    *c3 = (t1 + t2 + t3) / b;
}

//div: 1-1-4
void div_SW_SW_QQW(float a,float b,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    div_SW_SW_QTW(a,b,c1,c2,c3); //// 30-17
    mul_QTW_SW_QQW(*c1,*c2,*c3,b,&t1,&t2,&t3,&t4); //// 12-8
    add_SW_QQW_QQW(a,-t1,-t2,-t3,-t4,&t1,&t2,&t3,&t4); ////  19-7
    *c4 = (t1 + t2 + t3 + t4) / b;
}

//div: 1-2-1
void div_SW_PA_SA(float a, float bh, float bl, float *c)
{
    *c = a/(bh+bl);
}

//div: 1-2-2
void div_SW_PA_PA(float a, float bh, float bl, float *ch, float *cl)
{
    float t;
    *ch = a/bh;
    t = fma(-bh,*ch,a);
    *cl= fma(*ch,-bl,t)/(bh + bl);
}

//div: 1-2-3
void div_SW_PA_QTW(float a, float b1, float b2, float *c1, float *c2, float *c3)
{
    float t1, t2, t3;
    div_SW_PA_PA(a,b1,b2,c1,c2); //// 5-5
    mul_PA_PA_QTW(*c1,*c2,b1,b2,&t1,&t2,&t3);  //// 20-13
    add_SW_QTW_QTW(a,-t1,-t2,-t3,&t1,&t2,&t3); //// 13-5
    *c3 = (t1 + t2 + t3) / (b1 + b2);
}

//div: 1-2-4
void div_SW_PA_QQW(float a,float b1,float b2,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    div_SW_PA_QTW(a,b1,b2,c1,c2,c3); //// 42-26
    mul_QTW_PA_QQW(*c1,*c2,*c3,b1,b2,&t1,&t2,&t3,&t4); //// 59-28
    add_SW_QQW_QQW(a,-t1,-t2,-t3,-t4,&t1,&t2,&t3,&t4); //// 19-7
    *c4 = (t1 + t2 + t3 + t4) / (b1 + b2);
}

//div: 1-3-1
void div_SW_QTW_SW(float a, float b1, float b2, float b3, float *c)
{
    *c = a/(b1+b2+b3);
}

//div: 1-3-2
void div_SW_QTW_PA(float a, float b1, float b2, float b3, float *c1, float *c2)
{
    div_SW_PA_PA(a,b1,b2+b3,c1,c2);
}

//div: 1-3-3
void div_SW_QTW_QTW(float a, float b1, float b2, float b3, float *c1, float *c2, float *c3)
{
    float t1, t2, t3;
    div_SW_PA_PA(a,b1,b2,c1,c2); //// 5-5
    mul_PA_QTW_QTW(*c1,*c2,b1,b2,b3,&t1,&t2,&t3); //// 21-14
    add_SW_QTW_QTW(a,-t1,-t2,-t3,&t1,&t2,&t3); //// 13-5
    *c3 = (t1 + t2 + t3) / (b1 + b2);
}

//div: 1-3-4
void div_SW_QTW_QQW(float a,float b1,float b2,float b3,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    div_SW_QTW_QTW(a,b1,b2,b3,c1,c2,c3); //// 43-27
    mul_QTW_QTW_QQW(*c1,*c2,*c3,b1,b2,b3,&t1,&t2,&t3,&t4); ////70-34 
    add_SW_QQW_QQW(a,-t1,-t2,-t3,-t4,&t1,&t2,&t3,&t4); //// 19-7
    *c4 = (t1 + t2 + t3 + t4) / (b1 + b2 + b3);
}

//div: 1-4-1
void div_SW_QQW_SW(float a,float b1,float b2,float b3,float b4,float *c)
{
    *c = a / ( b1 + b2 + b3 + b4);
}

//div: 1-4-2
void div_SW_QQW_PA(float a,float b1,float b2,float b3,float b4,float *c1,float *c2)
{
    div_SW_PA_PA(a,b1,b2+b3+b4,c1,c2);
}

//div: 1-4-3
void div_SW_QQW_QTW(float a,float b1,float b2,float b3,float b4,float *c1,float *c2,float *c3)
{
    div_SW_QTW_QTW(a,b1,b2,b3+b4,c1,c2,c3);
}


//div: 1-4-4
void div_SW_QQW_QQW(float a,float b1,float b2,float b3,float b4,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    div_SW_QTW_QTW(a,b1,b2,b3,c1,c2,c3);//// 43-27
    mul_QTW_QQW_QQW(*c1,*c2,*c3,b1,b2,b3,b4,&t1,&t2,&t3,&t4); //// 72-36
    add_SW_QQW_QQW(a,-t1,-t2,-t3,-t4,&t1,&t2,&t3,&t4); //// 19-7
    *c4 = (t1 + t2 + t3 + t4) / (b1 + b2 + b3);
}

//div: 2-1-1
void div_PA_SW_SW(float ah, float al, float b, float *c)
{
    *c = (ah+al)/b;
}

//div: 2-1-2
void div_PA_SW_PA(float ah, float al, float b, float *ch, float *cl)
{
    *ch = ah/b;
    *cl = (fma(-b,*ch,ah) + al)/b;
}

//div: 2-1-3
void div_PA_SW_QTW(float a1, float a2, float b, float *c1, float *c2, float *c3)
{
    float t1, t2, t3;
    div_PA_SW_PA(a1,a2,b,c1,c2); //// 4-4
    mul_PA_SW_QTW(*c1,*c2,b,&t1,&t2,&t3); //// 11-7
    add_PA_QTW_QTW(a1,a2,-t1,-t2,-t3,&t1,&t2,&t3); //// 20-7
    *c3 = (t1 + t2 + t3) / b;
}

//div: 2-1-4
void div_PA_SW_QQW(float a1,float a2,float b,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    div_PA_SW_QTW(a1,a2,b,c1,c2,c3); //// 38-20
    mul_QTW_SW_QQW(*c1,*c2,*c3,b,&t1,&t2,&t3,&t4); //// 26-13
    add_PA_QQW_QQW(a1,a2,-t1,-t2,-t3,-t4,&t1,&t2,&t3,&t4); //// 32-11
    *c4 = (t1 + t2 + t3 + t4) / b;
}

//div: 2-2-1
void div_PA_PA_SW(float ah, float al, float bh, float bl, float *c)
{
    *c = (ah+al)/(bh+bl);
}

//div: 2-2-2
void div_PA_PA_PA(float ah, float al, float bh, float bl, float *ch, float *cl)
{
    float t, p;
    *ch = ah/bh;
    t = fma(-bh,*ch,ah);
    p = t + al;
    *cl = fma(*ch,-bl,p)/(bh + bl);
}

//div: 2-2-3
void div_PA_PA_QTW(float a1, float a2, float b1, float b2, float *c1, float *c2, float *c3)
{
    float t1, t2, t3;
    div_PA_PA_PA(a1,a2,b1,b2,c1,c2); //// 6-6
    mul_PA_PA_QTW(*c1,*c2,b1,b2,&t1,&t2,&t3); ////20-13
    add_PA_QTW_QTW(a1,a2,-t1,-t2,-t3,&t1,&t2,&t3); //// 20-7
    *c3 = (t1 + t2 + t3) / (b1 + b2);
}

//div: 2-2-4
void div_PA_PA_QQW(float a1,float a2,float b1,float b2,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    div_PA_PA_QTW(a1,a2,b1,b2,c1,c2,c3); //// 50-29
    mul_QTW_PA_QQW(*c1,*c2,*c3,b1,b2,&t1,&t2,&t3,&t4); //// 59-28
    add_PA_QQW_QQW(a1,a2,-t1,-t2,-t3,-t4,&t1,&t2,&t3,&t4); //// 32-11
    *c4 = (t1 + t2 + t3 + t4) / (b1 + b2);
}

//div: 2-3-1
void div_PA_QTW_SW(float a1, float a2, float b1, float b2, float b3, float *c)
{
    *c = (a1+a2)/(b1+b2+b3);
}

//div: 2-3-2
void div_PA_QTW_PA(float a1, float a2, float b1, float b2, float b3, float *c1, float *c2)
{
    div_PA_PA_PA(a1,a2,b1,b2+b3,c1,c2);
}

//div: 2-3-3
void div_PA_QTW_QTW(float a1, float a2, float b1, float b2, float b3, float *c1, float *c2, float *c3)
{
    float t1, t2, t3;
    div_PA_PA_PA(a1,a2,b1,b2,c1,c2); //// 6-6
    mul_PA_QTW_QTW(*c1,*c2,b1,b2,b3,&t1,&t2,&t3); //// 21-14
    add_PA_QTW_QTW(a1,a2,-t1,-t2,-t3,&t1,&t2,&t3); //// 20-7
    *c3 = (t1 + t2 + t3) / (b1 + b2);
}

//div: 2-3-4
void div_PA_QTW_QQW(float a1,float a2,float b1,float b2,float b3,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    div_PA_QTW_QTW(a1,a2,b1,b2,b3,c1,c2,c3); //// 51-30
    mul_QTW_QTW_QQW(*c1,*c2,*c3,b1,b2,b3,&t1,&t2,&t3,&t4); //// 70-34
    add_PA_QQW_QQW(a1,a2,-t1,-t2,-t3,-t4,&t1,&t2,&t3,&t4); //// 32-11
    *c4 = (t1 + t2 + t3 + t4) / (b1 + b2 + b3);
}

//div: 2-4-1
void div_PA_QQW_SW(float a1,float a2,float b1,float b2,float b3,float b4,float *c)
{
    *c = (a1 + a2)/(b1 + b2 + b3 + b4);
}

//div: 2-4-2
void div_PA_QQW_PA(float a1,float a2,float b1,float b2,float b3,float b4,float *c1,float *c2)
{
    div_PA_PA_PA(a1,a2,b1,b2+b3+b4,c1,c2);
}

//div: 2-4-3
void div_PA_QQW_QTW(float a1,float a2,float b1,float b2,float b3,float b4,float *c1,float *c2,float *c3)
{
    div_PA_QTW_QTW(a1,a2,b1,b2,b3+b4,c1,c2,c3);
}

//div: 2-4-4
void div_PA_QQW_QQW(float a1,float a2,float b1,float b2,float b3,float b4,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    div_PA_QTW_QTW(a1,a2,b1,b2,b3,c1,c2,c3); ////51-30
    mul_QTW_QQW_QQW(*c1,*c2,*c3,b1,b2,b3,b4,&t1,&t2,&t3,&t4); //// 72-36
    add_PA_QQW_QQW(a1,a2,-t1,-t2,-t3,-t4,&t1,&t2,&t3,&t4); //// 32-11
    *c4 = (t1 + t2 + t3 + t4) / (b1 + b2 + b3);
}

//div: 3-1-1
void div_QTW_SW_SW(float a1, float a2, float a3, float b, float *c)
{
    *c = (a1+a2+a3)/b;
}

//div: 3-1-2
void div_QTW_SW_PA(float a1, float a2, float a3, float b, float *c1, float *c2)
{
    div_PA_SW_PA(a1,a2+a3,b,c1,c2);
}

//div: 3-1-3
void div_QTW_SW_QTW(float a1, float a2, float a3, float b, float *c1, float *c2, float *c3)
{
    float t1, t2, t3;
    div_PA_SW_PA(a1,a2,b,c1,c2); //// 4-4
    mul_PA_SW_QTW(*c1,*c2,b,&t1,&t2,&t3); //// 11-7
    add_QTW_QTW_QTW(a1,a2,a3,-t1,-t2,-t3,&t1,&t2,&t3); //// 21-8
    *c3 = (t1 + t2 + t3) / b;
}

//div: 3-1-4
void div_QTW_SW_QQW(float a1,float a2,float a3,float b,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    div_QTW_SW_QTW(a1,a2,a3,b,c1,c2,c3); //// 39-21
    mul_QTW_SW_QQW(*c1,*c2,*c3,b,&t1,&t2,&t3,&t4); //// 12-8
    add_QTW_QQW_QQW(a1,a2,a3,-t1,-t2,-t3,-t4,&t1,&t2,&t3,&t4); ////39-14 
    *c4 = (t1 + t2 + t3 + t4) / b;
}

//div: 3-2-1
void div_QTW_PA_SW(float a1, float a2, float a3, float b1, float b2, float *c)
{
    *c = (a1+a2+a3)/(b1+b2);
}

//div: 3-2-2
void div_QTW_PA_PA(float a1, float a2, float a3, float b1, float b2, float *c1, float *c2)
{
    div_PA_PA_PA(a1,a2+a3,b1,b2,c1,c2);
}

//div: 3-2-3
void div_QTW_PA_QTW(float a1, float a2, float a3, float b1, float b2, float *c1, float *c2, float *c3)
{
    float t1, t2, t3;
    div_PA_PA_PA(a1,a2,b1,b2,c1,c2); //// 6-6
    mul_PA_PA_QTW(*c1,*c2,b1,b2,&t1,&t2,&t3); //// 20-13
    add_QTW_QTW_QTW(a1,a2,a3,-t1,-t2,-t3,&t1,&t2,&t3); //// 21-8
    *c3 = (t1 + t2 + t3) / (b1 + b2);
}

//div: 3-2-4
void div_QTW_PA_QQW(float a1,float a2,float a3,float b1,float b2,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    div_QTW_PA_QTW(a1,a2,a3,b1,b2,c1,c2,c3);//// 51-30
    mul_QTW_PA_QQW(*c1,*c2,*c3,b1,b2,&t1,&t2,&t3,&t4); //// 59-28
    add_QTW_QQW_QQW(a1,a2,a3,-t1,-t2,-t3,-t4,&t1,&t2,&t3,&t4); //// 39-14
    *c4 = (t1 + t2 + t3 + t4) / (b1 + b2);
}

//div: 3-3-1
void div_QTW_QTW_SW(float a1, float a2, float a3, float b1, float b2, float b3, float *c)
{
    *c = (a1+a2+a3)/(b1+b2+b3);
}

//div: 3-3-2
void div_QTW_QTW_PA(float a1, float a2, float a3, float b1, float b2, float b3, float *c1, float *c2)
{
    div_PA_PA_PA(a1,a2+a3,b1,b2+b3,c1,c2);
}

//div: 3-3-3
void div_QTW_QTW_QTW(float a1, float a2, float a3, float b1, float b2, float b3, float *c1, float *c2, float *c3)
{
    float t1, t2, t3;
    div_PA_PA_PA(a1,a2,b1,b2,c1,c2); //// 6-6
    mul_PA_QTW_QTW(*c1,*c2,b1,b2,b3,&t1,&t2,&t3); //// 21-14
    add_QTW_QTW_QTW(a1,a2,a3,-t1,-t2,-t3,&t1,&t2,&t3); //// 21-8
    *c3 = (t1 + t2 + t3) / (b1 + b2);
}

//div: 3-3-4
void div_QTW_QTW_QQW(float a1,float a2,float a3,float b1,float b2,float b3,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    div_QTW_QTW_QTW(a1,a2,a3,b1,b2,b3,c1,c2,c3);//// 52-31
    mul_QTW_QTW_QQW(*c1,*c2,*c3,b1,b2,b3,&t1,&t2,&t3,&t4); //// 70-34
    add_QTW_QQW_QQW(a1,a2,a3,-t1,-t2,-t3,-t4,&t1,&t2,&t3,&t4); //// 39-14
    *c4 = (t1 + t2 + t3 + t4) / (b1 + b2 + b3);
}

//div: 3-4-1
void div_QTW_QQW_SW(float a1,float a2,float a3,float b1,float b2,float b3,float b4,float *c)
{
    *c = (a1+a2+a3)/(b1+b2+b3+b4);
}

//div: 3-4-2
void div_QTW_QQW_PA(float a1,float a2,float a3,float b1,float b2,float b3,float b4,float *c1,float *c2)
{
    div_PA_PA_PA(a1,a2+a3,b1,b2+b3+b4,c1,c2);
}

//div: 3-4-3
void div_QTW_QQW_QTW(float a1,float a2,float a3,float b1,float b2,float b3,float b4,float *c1,float *c2,float *c3)
{
    div_QTW_QTW_QTW(a1,a2,a3,b1,b2,b3+b4,c1,c2,c3);
}

//div: 3-4-4
void div_QTW_QQW_QQW(float a1,float a2,float a3,float b1,float b2,float b3,float b4,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    div_QTW_QTW_QTW(a1,a2,a3,b1,b2,b3,c1,c2,c3);//// 53-32
    mul_QTW_QQW_QQW(*c1,*c2,*c3,b1,b2,b3,b4,&t1,&t2,&t3,&t4); //// 72-36
    add_QTW_QQW_QQW(a1,a2,a3,-t1,-t2,-t3,-t4,&t1,&t2,&t3,&t4); //// 39-14
    *c4 = (t1 + t2 + t3 + t4) / (b1 + b2 + b3);
}

//div: 4-1-1
void div_QQW_SW_SW(float a1,float a2,float a3,float a4,float b,float *c)
{
    *c = (a1+a2+a3+a4)/b;
}

//div: 4-1-2
void div_QQW_SW_PA(float a1,float a2,float a3,float a4,float b,float *c1,float *c2)
{
    div_PA_SW_PA(a1,a2+a3+a4,b,c1,c2);
}

//div: 4-1-3
void div_QQW_SW_QTW(float a1,float a2,float a3,float a4,float b,float *c1,float *c2,float *c3)
{
    div_QTW_SW_QTW(a1,a2,a3+a4,b,c1,c2,c3);
}

//div: 4-1-4
void div_QQW_SW_QQW(float a1,float a2,float a3,float a4,float b,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    div_QTW_SW_QTW(a1,a2,a3,b,c1,c2,c3);////40-22
    mul_QTW_SW_QQW(*c1,*c2,*c3,b,&t1,&t2,&t3,&t4);////26-13
    add_QQW_QQW_QQW(a1,a2,a3,a4,-t1,-t2,-t3,-t4,&t1,&t2,&t3,&t4); ////40-14
    *c4 = (t1 + t2 + t3 + t4) / b;
}

//div: 4-2-1
void div_QQW_PA_SW(float a1,float a2,float a3,float a4,float b1,float b2,float *c)
{
    *c = (a1+a2+a3+a4)/(b1+b2);
}

//div: 4-2-2
void div_QQW_PA_PA(float a1,float a2,float a3,float a4,float b1,float b2,float *c1,float *c2)
{
    div_PA_PA_PA(a1,a2+a3+a4,b1,b2,c1,c2);
}

//div: 4-2-3
void div_QQW_PA_QTW(float a1,float a2,float a3,float a4,float b1,float b2,float *c1,float *c2,float *c3)
{
    div_QTW_PA_QTW(a1,a2,a3+a4,b1,b2,c1,c2,c3);
}

//div: 4-2-4
void div_QQW_PA_QQW(float a1,float a2,float a3,float a4,float b1,float b2,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    div_QTW_PA_QTW(a1,a2,a3,b1,b2,c1,c2,c3); //// 51-30
    mul_QTW_PA_QQW(*c1,*c2,*c3,b1,b2,&t1,&t2,&t3,&t4);////59-28
    add_QQW_QQW_QQW(a1,a2,a3,a4,-t1,-t2,-t3,-t4,&t1,&t2,&t3,&t4); ////40-14 
    *c4 = (t1 + t2 + t3 + t4) / (b1 + b2);
}

//div: 4-3-1
void div_QQW_QTW_SW(float a1,float a2,float a3,float a4,float b1,float b2,float b3,float *c)
{
    *c = (a1+a2+a3+a4)/(b1+b2+b3);
}

//div: 4-3-2
void div_QQW_QTW_PA(float a1,float a2,float a3,float a4,float b1,float b2,float b3,float *c1,float *c2)
{
    div_PA_PA_PA(a1,a2+a3+a4,b1,b2+b3,c1,c2);
}

//div: 4-3-3
void div_QQW_QTW_QTW(float a1,float a2,float a3,float a4,float b1,float b2,float b3,float *c1,float *c2,float *c3)
{
    div_QTW_QTW_QTW(a1,a2,a3+a4,b1,b2,b3,c1,c2,c3);
}

//div: 4-3-4
void div_QQW_QTW_QQW(float a1,float a2,float a3,float a4,float b1,float b2,float b3,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    div_QTW_QTW_QTW(a1,a2,a3,b1,b2,b3,c1,c2,c3);//// 52-31
    mul_QTW_QTW_QQW(*c1,*c2,*c3,b1,b2,b3,&t1,&t2,&t3,&t4); //// 24-15
    add_QQW_QQW_QQW(a1,a2,a3,a4,-t1,-t2,-t3,-t4,&t1,&t2,&t3,&t4); //// 40-14
    *c4 = (t1 + t2 + t3 + t4) / (b1 + b2 + b3);
}

//div: 4-4-1
void div_QQW_QQW_SW(float a1,float a2,float a3,float a4,float b1,float b2,float b3,float b4,float *c)
{
    *c = (a1+a2+a3+a4)/(b1+b2+b3+b4);
}

//div: 4-4-2
void div_QQW_QQW_PA(float a1,float a2,float a3,float a4,float b1,float b2,float b3,float b4,float *c1,float *c2)
{
    div_PA_PA_PA(a1,a2+a3+a4,b1,b2+b3+b4,c1,c2);
}

//div: 4-4-3
void div_QQW_QQW_QTW(float a1,float a2,float a3,float a4,float b1,float b2,float b3,float b4,float *c1,float *c2,float *c3)
{
    div_QTW_QTW_QTW(a1,a2,a3+a4,b1,b2,b3+b4,c1,c2,c3);
}

//div: 4-4-4
void div_QQW_QQW_QQW(float a1,float a2,float a3,float a4,float b1,float b2,float b3,float b4,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    div_QTW_QTW_QTW(a1,a2,a3,b1,b2,b3,c1,c2,c3);//// 52-31
    mul_QTW_QQW_QQW(*c1,*c2,*c3,b1,b2,b3,b4,&t1,&t2,&t3,&t4);////72-26
    add_QQW_QQW_QQW(a1,a2,a3,a4,-t1,-t2,-t3,-t4,&t1,&t2,&t3,&t4); ////40-14
    *c4 = (t1 + t2 + t3 + t4) / (b1 + b2 + b3);
}

//--------------------
//square
//--------------------

//sqr:1-2
void sqr_SW_PA(float a, float *c1, float *c2)
{
    TwoProductFMA(a,a,c1,c2);
}

//sqr:1-3
void sqr_SW_QTW(float a, float *c1, float *c2, float *c3)
{
    TwoProductFMA(a,a,c1,c2);
    *c3 = 0;
}

//sqr:1-4
void sqr_SW_QQW(float a, float *c1, float *c2, float *c3, float *c4)
{
    TwoProductFMA(a,a,c1,c2);
    *c3 = 0;
    *c4 = 0;
}

//sqr:2-1
void sqr_PA_SW(float a1,float a2,float *c)
{
    *c = (a1+a2)*(a1+a2);
}

//sqr:2-2
void sqr_PA_PA(float a1,float a2,float *c1,float *c2)
{
    float e1;
    TwoProductFMA(a1,a1,c1,&e1);
    *c2 = fma(a1,2*a2,e1);
}

//sqr:2-3
void sqr_PA_QTW(float a1, float a2, float *c1, float *c2, float *c3)
{
    float e1, e2, e3;
    float t2;
    TwoProductFMA(a1,a1,c1,&e1);
    TwoProductFMA(a1,2*a2,&t2,&e2);
    TwoSum(t2,e1,c2,&e3);
    *c3 = fma(a2,a2,e2) + e3;
}

//sqr:2-4
void sqr_PA_QQW(float a1,float a2,float *c1,float *c2,float *c3,float *c4)
{
    float e1, e2, e5, e7, e14;
    float t2, t4, t5;
    
    TwoProductFMA(a1,a1,c1,&e1);
    TwoProductFMA(a1,2*a2,&t2,&e2);
    TwoSum(t2,e1,c2,&e5);
    TwoProductFMA(a2,a2,&t5,&e7);
    TwoSum(t5,e5,c3,&e14);
    *c4 = e7 + e14;
}

//sqr:3-1
void sqr_QTW_SW(float a1,float a2,float a3,float *c)
{
    float t;
    t = a1+a2+a3;
    *c = t*t;
}

//sqr:3-2
void sqr_QTW_PA(float a1,float a2,float a3,float *c1,float *c2)
{
    float e1;
    TwoProductFMA(a1,a1,c1,&e1);
    *c2 = fma(2*a1,a2,e1);
}

//sqr:3-3
void sqr_QTW_QTW(float a1,float a2,float a3,float *c1,float *c2,float *c3)
{
    float e1, e2, e5;
    float t2;
    
    TwoProductFMA(a1,a1,c1,&e1);
    TwoProductFMA(a1,2*a2,&t2,&e2);
    TwoSum(t2,e1,c2,&e5);
    *c3 = fma(a1,2*a3,e2) + fma(a2,a2,e5);
}

//sqr:3-4
void sqr_QTW_QQW(float a1,float a2,float a3,float *c1,float *c2,float *c3,float *c4)
{
    float e1, e2, e5;
    float e6, e7, e9, e11, e14;
    float t2, t4, t5;

    TwoProductFMA(a1,a1,c1,&e1);
    TwoProductFMA(a1,2*a2,&t2,&e2);
    TwoSum(t2,e1,c2,&e5);
    TwoProductFMA(a1,2*a3,&t4,&e6);
    TwoProductFMA(a2,a2,&t5,&e7);
    TwoSum(t4,t5,c3,&e9);
    TwoSum(*c3,e2,c3,&e11);
    TwoSum(*c3,e5,c3,&e14);
    *c4 = fma(a2,2*a3,e6) + e7 + e9 + e11 + e14;
}

//sqr:4-1
void sqr_QQW_SW(float a1, float a2, float a3, float a4, float *c)
{
    float t;
    t = a1 + a2  + a3 + a4;
    *c = t*t;
}

//sqr:4-2
void sqr_QQW_PA(float a1,float a2,float a3,float a4,float *c1,float *c2)
{
    float e1;
    TwoProductFMA(a1,a1,c1,&e1);
    *c2 = fma(2*a1,a2,e1);
}

//sqr:4-3
void sqr_QQW_QTW(float a1,float a2,float a3,float a4,float *c1,float *c2,float *c3)
{
    float e1, e2, e5;
    float t2;
    
    TwoProductFMA(a1,a1,c1,&e1);
    TwoProductFMA(a1,2*a2,&t2,&e2);
    TwoSum(t2,e1,c2,&e5);
    *c3 = fma(a1,2*a3,fma(a2,a2,e2)) + e5;
}

//sqr:4-4
void sqr_QQW_QQW(float a1,float a2,float a3,float a4,float *c1,float *c2,float *c3,float *c4)
{
    float e1, e2, e5, e6, e7, e9, e11, e14;
    float t2, t4, t5;
    
    TwoProductFMA(a1,a1,c1,&e1);
    TwoProductFMA(a1,2*a2,&t2,&e2);
    TwoSum(t2,e1,c2,&e5);
    TwoProductFMA(a1,2*a3,&t4,&e6);
    TwoProductFMA(a2,a2,&t5,&e7);
    TwoSum(t4,t5,c3,&e9);
    TwoSum(*c3,e2,c3,&e11);
    TwoSum(*c3,e5,c3,&e14);
    *c4 = fma(2*a2,a3,2*a1*a4) + e6 + e7 + e9 + e11 + e14;
}

//------------------------------
//Sqrt
//------------------------------

//sqrt: 1-2
void sqrt_SW_PA(float a, float *ch, float *cl)
{
    *ch = sqrt(a);
    *cl = fma(-*ch,*ch,a)/(*ch+*ch);
}

//sqrt: 1-3
void sqrt_SW_QTW(float a, float *c1, float *c2, float *c3)
{
    float t1, t2 ,t3;
    sqrt_SW_PA(a,c1,c2);
    sqr_PA_QTW(*c1,*c2,&t1,&t2,&t3);
    add_QTW_SW_QTW(t1,t2,t3,-a,&t1,&t2,&t3);
    *c3 = - (t1+t2+t3) / (2*(*c1 + *c2));
}

//sqrt: 1-4
void sqrt_SW_QQW(float a,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    sqrt_SW_QTW(a,c1,c2,c3);
    sqr_QTW_QQW(*c1,*c2,*c3,&t1,&t2,&t3,&t4);
    add_QQW_SW_QQW(t1,t2,t3,t4,-a,&t1,&t2,&t3,&t4);
    *c4 = - (t1+t2+t3+t4) / (2*(*c1 + *c2 + *c3));
}


//sqrt: 2-1
void sqrt_PA_SW(float ah, float al, float *c)
{
    *c = sqrt(ah+al);
}

//sqrt: 2-2
void sqrt_PA_PA(float ah, float al, float *ch, float *cl)
{
    *ch = sqrt(ah);
    *cl = (fma(-*ch,*ch,ah) + al)/(*ch + *ch);
}

//sqrt: 2-3
void sqrt_PA_QTW(float a1, float a2, float *c1, float *c2, float *c3)
{
    float t1, t2 ,t3;
    sqrt_PA_PA(a1,a2,c1,c2);
    sqr_PA_QTW(*c1,*c2,&t1,&t2,&t3);
    add_QTW_PA_QTW(t1,t2,t3,-a1,-a2,&t1,&t2,&t3);
    *c3 = - (t1+t2+t3) / (2*(*c1 + *c2));
}

//sqrt: 2-4
void sqrt_PA_QQW(float a1,float a2,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    sqrt_PA_QTW(a1,a2,c1,c2,c3);
    sqr_QTW_QQW(*c1,*c2,*c3,&t1,&t2,&t3,&t4);
    add_QQW_PA_QQW(t1,t2,t3,t4,-a1,-a2,&t1,&t2,&t3,&t4);
    *c4 = - (t1+t2+t3+t4) / (2*(*c1 + *c2 + *c3));
}

//sqrt: 3-1
void sqrt_QTW_SW(float a1, float a2, float a3, float *c)
{
    *c = sqrt(a1+a2+a3);
}

//sqrt: 3-2
void sqrt_QTW_PA(float a1, float a2, float a3, float *c1, float *c2)
{
    sqrt_PA_PA(a1,a2+a3,c1,c2);
}

//sqrt: 3-3
void sqrt_QTW_QTW(float a1, float a2, float a3, float *c1, float *c2, float *c3)
{
    float t1, t2 ,t3;
    sqrt_PA_PA(a1,a2,c1,c2);
    sqr_PA_QTW(*c1,*c2,&t1,&t2,&t3);
    add_QTW_QTW_QTW(t1,t2,t3,-a1,-a2,-a3,&t1,&t2,&t3);
    *c3 = - (t1+t2+t3) / (2*(*c1 + *c2));
}

//sqrt: 3-4
void sqrt_QTW_QQW(float a1,float a2,float a3,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    sqrt_QTW_QTW(a1,a2,a3,c1,c2,c3);
    sqr_QTW_QQW(*c1,*c2,*c3,&t1,&t2,&t3,&t4);
    add_QQW_QTW_QQW(t1,t2,t3,t4,-a1,-a2,-a3,&t1,&t2,&t3,&t4);
    *c4 = - (t1+t2+t3+t4) / (2*(*c1 + *c2 + *c3));
}

//sqrt: 4-1
void sqrt_QQW_SW(float a1,float a2,float a3,float a4,float *c)
{
    *c = sqrt(a1+a2+a3+a4);
}

//sqrt: 4-2
void sqrt_QQW_PA(float a1,float a2,float a3,float a4,float *c1,float *c2)
{
    sqrt_QTW_PA(a1,a2,a3+a4,c1,c2);
}

//sqrt: 4-3
void sqrt_QQW_QTW(float a1,float a2,float a3,float a4,float *c1,float *c2,float *c3)
{
    sqrt_QTW_QTW(a1,a2,a3+a4,c1,c2,c3);
}

//sqrt: 4-4
void sqrt_QQW_QQW(float a1,float a2,float a3,float a4,float *c1,float *c2,float *c3,float *c4)
{
    float t1, t2, t3, t4;
    sqrt_QTW_QTW(a1,a2,a3,c1,c2,c3);
    sqr_QTW_QQW(*c1,*c2,*c3,&t1,&t2,&t3,&t4);
    add_QQW_QQW_QQW(t1,t2,t3,t4,-a1,-a2,-a3,-a4,&t1,&t2,&t3,&t4);
    *c4 = - (t1+t2+t3+t4) / (2*(*c1 + *c2 + *c3));
}



// -------------------------------
// Double word Part
// -------------------------------
//Double word arithmetic: add
void add_SW_SW_DW(float a, float b, float *ch, float *cl)
{
    TwoSum(a, b, ch,cl);
}

void add_SW_DW_SW(float a, float bh, float bl, float *c)
{
//    *c = a + bh;
    float sh, se, eh; //
    TwoSum(a, bh, &sh, &eh); //
    se = eh + bl; //
    *c = sh + se; //
}

void add_SW_DW_DW(float a, float bh, float bl, float *ch, float *cl)
{
    float sh, sl, eh, el, se, te;
    TwoSum(a, bh, &sh, &eh);
    se = eh + bl;
//    FastTwoSum(sh, se, &sh, &se);
    FastTwoSum(sh, se, ch, cl);
}

void add_DW_SW_SW(float ah, float al, float b, float *c)
{
    float sh, eh, el, se, te;
    TwoSum(ah, b, &sh, &eh);
    se = eh + al;
    FastTwoSum(sh, se, &sh, &se);
    *c = sh + se;
}

void add_DW_SW_DW(float ah, float al, float bh, float *ch, float *cl)
{
    float sh, eh, el, se, te;
    TwoSum(ah, bh, &sh, &eh);
    se = eh + al;
//    FastTwoSum(sh, se, &sh, &se);
    FastTwoSum(sh, se, ch, cl);
}

void add_DW_DW_SW(float ah, float al, float bh, float bl, float *c)
{
    float sh, sl, eh, el, se, te;
    TwoSum(ah, bh, &sh, &eh);
    TwoSum(al, bl, &sl, &el);
    se = eh + sl;
    FastTwoSum(sh, se, &sh, &se);
    te = se + el;
    *c = sh + te;
}

void add_DW_DW_DW(float ah, float al, float bh, float bl, float *ch, float *cl)
{
    float sh, sl, eh, el, se, te;
    TwoSum(ah, bh, &sh, &eh);
    TwoSum(al, bl, &sl, &el);
    se = eh + sl;
    FastTwoSum(sh, se, &sh, &se);
    te = se + el;
    FastTwoSum(sh, te, ch, cl);
}


//Double word arithmetic: mul
void mul_SW_SW_DW(float a, float b, float *ch, float *cl)
{
    TwoProductFMA(a, b, ch, cl);
}

void mul_SW_DW_SW(float a, float bh, float bl, float *c)
{
    float p1, p2;
    TwoProductFMA(a, bh, &p1, &p2);
    *c = p1 + fma(a,bl,p2);
}

void mul_SW_DW_DW(float ah, float bh, float bl, float *ch, float *cl)
{
    float p1, p2;
    TwoProductFMA(ah, bh, &p1, &p2);
    p2 = fma(ah,bl,p2);
    FastTwoSum(p1, p2, ch, cl);
}

void mul_DW_SW_SW(float ah, float al, float b, float *c)
{
    float p1, p2;
    TwoProductFMA(ah, b, &p1, &p2);
    p2 = fma(al,b,p2);
    *c = p1 + p2;
}

void mul_DW_SW_DW(float ah, float al, float b, float *ch, float *cl)
{
    float p1, p2;
    TwoProductFMA(ah, b, &p1, &p2);
    p2 = fma(al,b,p2);
    FastTwoSum(p1, p2, ch, cl);
}

void mul_DW_DW_SW(float ah, float al, float bh, float bl, float *c)
{
    float p1, p2;
    TwoProductFMA(ah, bh, &p1, &p2);
    p2 = fma(ah,bl,p2);
    p2 = fma(al,bh,p2);
    *c = p1 + p2;
}

void mul_DW_DW_DW(float ah, float al, float bh, float bl, float *ch, float *cl)
{
    float p1, p2;
    TwoProductFMA(ah, bh, &p1, &p2);
    p2 = fma(ah,bl,p2);
    p2 = fma(al,bh,p2);
    FastTwoSum(p1, p2, ch, cl);
}

//Double word arithmetic: div

void div_SW_SW_DW(float a, float b, float *ch, float *cl)
{
    float dh, dl, s1, s2, t;
    *ch = a / b;
    mul_SW_SW_DW(*ch,b,&dh,&dl);
    TwoSum(a,-dh, &s1, &s2);
    s2 = s2 - dl;
    t = (s1 + s2)/b;
    FastTwoSum(*ch,t,ch,cl);
}

////////////////?????/////////////////
void div_SW_DW_SW(float a, float bh, float bl,float *c)
{
    float dh, dl, s1, s2;
    *c = a / bh;
    mul_SW_DW_DW(*c,bh,bl,&dh,&dl);
    TwoSum(a,-dh, &s1, &s2);
    s2 = s2 - dl;
    *c = *c + (s1 + s2)/bh;
}

void div_SW_DW_DW(float a, float bh, float bl,float *ch, float *cl)
{
    float dh, dl, s1, s2, t;
    *ch = a / bh;
    mul_SW_DW_DW(*ch,bh,bl,&dh,&dl);
    TwoSum(a,-dh, &s1, &s2);
    s2 = s2 - dl;
    t = (s1 + s2)/bh;
    FastTwoSum(*ch,t,ch,cl);
}

////???????????????/////////
void div_DW_SW_SW(float ah, float al, float b, float *c)
{
    float dh, dl, s1, s2;
    *c = ah / b;
    mul_SW_SW_DW(*c,b,&dh,&dl);
    TwoSum(ah,-dh, &s1, &s2);
    s2 = s2 - dl + al;
    *c = *c + (s1 + s2)/b;
}

void div_DW_SW_DW(float ah, float al, float b, float *ch, float *cl)
{
    float dh, dl, s1, s2, t;
    *ch = ah / b;
    mul_SW_SW_DW(*ch,b,&dh,&dl);
    TwoSum(ah,-dh, &s1, &s2);
    s2 = s2 - dl + al;
    t = (s1 + s2)/b;
    FastTwoSum(*ch,t,ch,cl);
}

///?????////
void div_DW_DW_SW(float ah, float al, float bh, float bl,float *c)
{
    float dh, dl, s1, s2;
    *c = ah / bh;
    mul_SW_DW_DW(*c,bh,bl,&dh,&dl);
    TwoSum(ah,-dh, &s1, &s2);
    s2 = s2 - dl + al;
    *c = *c + (s1 + s2)/bh;
}

void div_DW_DW_DW(float ah, float al, float bh, float bl,float *ch, float *cl)
{
    float dh, dl, s1, s2, t;
    *ch = ah / bh;
    mul_SW_DW_DW(*ch,bh,bl,&dh,&dl);
    TwoSum(ah,-dh, &s1, &s2);
    s2 = s2 - dl + al;
    t = (s1 + s2)/bh;
    FastTwoSum(*ch,t,ch,cl);
}

//Double word arithmetic: sqrt
void sqrt_SW_DW(float a, float *ch, float *cl)
{
    float x, ax, r1, r2, s1;
    x = 1/sqrt(a);
    ax = a*x;
    TwoProductFMA(ax,ax, &r1, &r2);
    add_SW_DW_SW(a, -r1, -r2, &s1);
    TwoSum(ax,s1*(x*0.5),ch,cl);
}

void sqrt_DW_SW(float ah, float al, float *c)
{
    float x, ax, r1, r2, s1;
    x = 1/sqrt(ah);
    ax = ah*x;
    TwoProductFMA(ax,ax, &r1, &r2);
    add_DW_DW_SW(ah, al, -r1, -r2, &s1);
    *c = ax + s1*(x*0.5);
}


void sqrt_DW_DW(float ah, float al, float *ch, float *cl)
{
    float x, ax, r1, r2, s1;
    x = 1/sqrt(ah);
    ax = ah*x;
    TwoProductFMA(ax,ax, &r1, &r2);
    add_DW_DW_SW(ah, al, -r1, -r2, &s1);
    TwoSum(ax,s1*(x*0.5),ch,cl);
}
