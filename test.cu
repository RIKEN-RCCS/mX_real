#include <stdio.h>
#include <iostream>

#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>

#include "mX_real.hpp"
using namespace mX_real;

template < typename T, T_mX(T) >
__device__ void bench( T * const  __restrict__ out_ ) {
  using TT = typename T::base_T;
  auto const ZERO = T(0);
  auto const ONE  = T(1);
  auto const TWO  = T(2);

  auto y = ZERO;
  auto s1 = 1;
  auto s2 = 2;
  auto s3 = 3;
  double f = 1.0;

  #pragma unroll(1)
  while ( true ) {
    if ( f == 0 ) break;

    auto z = ZERO;
    double h = 1.0;
    #pragma unroll(1)
    for(int i=0; i<9; i++) {

      z += ( ( TWO / s1 + TWO / s2 + ONE / s3 ) * h );

      s1 += 4;
      s2 += 4;
      s3 += 4;
      h *= double(-0.25);
    }
    y += z * f;

    f *= h;
  }

  *out_ = y;
}

template < typename T >
__global__ void benchmark( T * const  __restrict__ out_ ) {
  bench< T > ( out_ );
}

int main( int argc, char *argv[] ) {
  dd_Real_weakaccurate *x = nullptr;
  benchmark <<< 1, 1 >>> ( x );
}

