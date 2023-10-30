#include <stdio.h>
#include <omp.h>

#include "mX_real.hpp"
using namespace mX_real;


template < typename REAL >
void benchmark( int const& N ) {

  auto * a = new REAL[N*N];
  auto * x = new REAL[N];
  auto * y = new REAL[N];
  REAL alpha;

  int constexpr ITR = 10;
  double t = 0.;

  for(int itr=0; itr<ITR+1; itr++) {

    for(int i=0; i<N; i++){
      for(int j=0; j<N; j++){
        a[j+i*N] = REAL(i-j)/N;
      }
      x[i] = one<REAL>();
      y[i] = zero<REAL>();
    }
    alpha = REAL(rand())/RAND_MAX;

    double ts = omp_get_wtime();

    //
    // optimized GEMV-n kernel
    //
    int constexpr STEP_i = 96;
    int constexpr STEP_j = 64;

    #pragma omp parallel
    for(int i_=0; i_<N; i_+=STEP_i){
    #pragma omp for nowait
    for(int j_=0; j_<N; j_+=STEP_j){
      auto Ni_ = std::min( i_+STEP_i, N );
      auto Nj_ = std::min( j_+STEP_j, N );

      if ( i_==0 ) {
        #pragma omp simd
        for(int j=j_; j<Nj_; j++) {
          y[j] = zero<REAL>();
        }
      }

      for(int i=i_; i<Ni_; i++) {
        auto xt = alpha * x[i];
        #pragma omp simd
        for(int j=j_; j<Nj_; j++) {
          y[j] = y[j] + a[j+i*N] * xt;
        }
      }

    }
    }

    double te = omp_get_wtime();

    if ( itr>0 ) t = t + (te - ts);

  }
  t /= ITR;


  std::cout << N << " ";
  std::cout << t << "[sec] ";
  std::cout << 2e-9*N*N/t << "[GFLOPS]\n";

  delete [] a;
  delete [] x;
  delete [] y;

}

int
main(int argc, char *argv[])
{
  while ( 1 ) {
    int N;
    std::cin >> N;
    if ( N <= 0 ) break;

    benchmark<double>( N );
    benchmark<df_Real_quasi>( N );
    benchmark<df_Real_sloppy>( N );
    benchmark<df_Real>( N );
  }
}

