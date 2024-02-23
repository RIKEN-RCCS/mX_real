#include <stdio.h>
#include <iostream>

#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>

#include "mX_real.hpp"
using namespace mX_real;

template < typename T, T_mX(T) >
__device__ void bench( int const n, T const * const x, T const * const y, T * const out_ ) {

  using TT = typename T::base_T;
  T const ZERO = T(0);
  T const ONE = T(1);
  T const TWO = T(2);
  T const THREE = T(3);
  T const FOUR = T(4);
  T const MQUATER = T(-0.25);

#if 0
  T z = ZERO;
  #pragma unroll(1)
  for(int i=0; i<n; i++) {
    z += x[i]*y[i];
  }
  *out_ = z;
#endif

  //
  {
    auto y = ZERO;
    auto t1 = ONE;
    auto t2 = TWO;
    auto t3 = THREE;
    auto t4 = ONE;
    #pragma unroll(1)
    for(int i=0; i<=400; i++) {
      auto z = ( TWO / t1 + TWO / t2 + ONE / t3 ) * t4;
      if ( z == ZERO ) break; // uderflow
      y += z;

      t1 += 4;
      t2 += 4;
      t3 += 4;
      t4 /= (-4);
    }
    *out_ = y;
  }

}

template < typename T >
__global__ void benchmark( int const n, T const * const x, T const * const y, T * const out_ ) {
  bench< T > ( n, x, y, out_ );
}

int main( int argc, char *argv[] ) {
  int n = 0;
  df_Real_sloppy *x = nullptr;
  df_Real_sloppy *y = nullptr;
  df_Real_sloppy *z = nullptr;
  std::cin >> n;
  benchmark <<< 1, 1 >>> ( n, x, y, z );
}

