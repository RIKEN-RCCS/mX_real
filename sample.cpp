#include <stdio.h>
#include <omp.h>

#include "mX_real.hpp"
using namespace mX_real;

struct FLOAT {
  float x;
  INLINE FLOAT() {}
  INLINE FLOAT(float const& h) : x(h) {}
  static
  INLINE FLOAT rand() {
    auto constexpr f = 1.0f / (1<<16) / (1<<15);
    auto const b_ = std::rand();
    auto const b = float( b_ & 0x7fff0000 );
    auto const c = b * f;
    return c;
  }
  auto const operator+( FLOAT const& a ) { return x + a.x; }
  auto const operator*( FLOAT const& a ) { return x * a.x; }
};

template < typename REAL >
void benchmark( int const& N ) {

  auto * a = new REAL[N*N];
  auto * b = new REAL[N*N];
  auto * c = new REAL[N*N];
  REAL alpha, beta;

  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      a[j+i*N] = REAL::rand();
      b[j+i*N] = REAL::rand();
      c[j+i*N] = REAL::rand();
    }
  }
  alpha = REAL::rand();
  beta = REAL::rand();


  int constexpr ITR = 10;
  double t = 0.;

  for(int itr=0; itr<ITR+1; itr++) {

    auto const ts = omp_get_wtime();

    //
    // optimized GEMV-n kernel
    //
    int constexpr STEP_i = 32;
    int constexpr STEP_j = 64;
    int constexpr STEP_k = 32;

    #pragma omp parallel for collapse(2)
    for(int i_=0; i_<N; i_+=STEP_i) {
    for(int j_=0; j_<N; j_+=STEP_j) {
      auto const Ni_ = std::min( i_+STEP_i, N );
      auto const Nj_ = std::min( j_+STEP_j, N );

    for(int k_=0; k_<N; k_+=STEP_k) {
      auto const Nk_ = std::min( k_+STEP_k, N );

      if ( k_==0 ) {
        for(int i=i_; i<Ni_; i++) {
        for(int j=j_; j<Nj_; j++) {
          c[j+i*N] = beta * c[j+i*N];
        }
        }
      }

      for(int i=i_; i<Ni_; i++) {
        for(int k=k_; k<Nk_; k++) {
        auto const s = alpha * b[k+i*N];
        #pragma gcc ivdep
        for(int j=j_; j<Nj_; j++) {
          c[j+i*N] = c[j+i*N] + a[j+k*N] * s;
        }
        }
      }

    }

    }
    }

    auto const te = omp_get_wtime();

    if ( itr>0 ) t = t + (te - ts);

  }
  t /= ITR;


  std::cout << N << " ";
  std::cout << t << "[sec] ";
  std::cout << 2e-9*N*N*N/t << "[GFLOPS]\n";
  std::cout << std::flush;

  delete [] a;
  delete [] b;
  delete [] c;

}

int
main(int argc, char *argv[])
{
  while ( 1 ) {
    int N;
    std::cin >> N;
    if ( N <= 0 ) break;

//    benchmark<FLOAT>( N );
//    benchmark<df_Real_quasi>( N );
//    benchmark<df_Real>      ( N );
//    benchmark<tf_Real_quasi>( N );
//    benchmark<tf_Real>( N );
//    benchmark<qf_Real_quasi>( N );
    benchmark<qf_Real>      ( N );

  }
}

