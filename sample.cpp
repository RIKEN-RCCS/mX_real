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

struct DOUBLE {
  double x;
  INLINE DOUBLE() {}
  INLINE DOUBLE(double const& h) : x(h) {}
  static
  INLINE DOUBLE rand() {
    auto constexpr f = 1.0f / (1<<16) / (1<<15);
    auto const b_ = std::rand();
    auto const b = double( b_ );
    auto const c = b * f;
    auto const d_ = std::rand();
    auto const d = double( d_ );
    auto const e = d * f * f;
    return c + e;
  }
  auto const operator+( DOUBLE const& a ) { return x + a.x; }
  auto const operator*( DOUBLE const& a ) { return x * a.x; }
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
  auto one = REAL(1);


  int constexpr ITR = 10;
  double t = 0.;

  for(int itr=0; itr<ITR+1; itr++) {

    auto const ts = omp_get_wtime();

    //
    // optimized GEMV-n kernel
    //
    int constexpr STEP_i = (sizeof(a[0]) >= 3*sizeof(float) ? 16 : 32);
    int constexpr STEP_j = (sizeof(a[0]) >= 4*sizeof(float) ? 32 : 64);
    int constexpr STEP_k = (sizeof(a[0]) >= 4*sizeof(float) ? 32 : 64);

    #pragma omp parallel for collapse(2)
    for(int i_=0; i_<N; i_+=STEP_i) {
    for(int j_=0; j_<N; j_+=STEP_j) {
      auto const Ni_ = std::min( i_+STEP_i, N );
      auto const Nj_ = std::min( j_+STEP_j, N );

      REAL C[STEP_i][STEP_j];
      for(int i=i_; i<Ni_; i++) {
      #pragma gcc ivdep
      for(int j=j_; j<Nj_; j++) {
        C[i-i_][j-j_] = beta * c[j+i*N];
      }
      }

    for(int k_=0; k_<N; k_+=STEP_k) {
      auto const Nk_ = std::min( k_+STEP_k, N );

      int constexpr D_i = 2;
      auto const Ni__ = i_+(Ni_-i_)%D_i;
      int constexpr D_k = 4;
      auto const Nk__ = k_+(Nk_-k_)%D_k;

      REAL A[STEP_k][STEP_j];
      for(int k=k_; k<Nk_; k++) {
      #pragma gcc ivdep
      for(int j=j_; j<Nj_; j++) {
        A[k-k_][j-j_] = a[j+k*N];
      }
      }

      for(int i=i_; i<Ni__; i++) {
        for(int k=k_; k<Nk__; k++) {
        auto const s = alpha * b[k+i*N];
        #pragma gcc ivdep
        for(int j=j_; j<Nj_; j++) {
          C[i-i_][j-j_] = C[i-i_][j-j_] + A[k-k_][j-j_] * s;
        }
        }
      }
      for(int i=i_; i<Ni__; i++) {
        for(int k=Nk__; k<Nk_; k+=D_k) {
        auto const s00 = alpha * b[(k+0)+i*N];
        auto const s10 = alpha * b[(k+1)+i*N];
        auto const s20 = alpha * b[(k+2)+i*N];
        auto const s30 = alpha * b[(k+3)+i*N];
        #pragma gcc ivdep
        for(int j=j_; j<Nj_; j++) {
          C[i-i_][j-j_] = C[i-i_][j-j_] + A[k-k_+0][j-j_] * s00
                                        + A[k-k_+1][j-j_] * s10
                                        + A[k-k_+2][j-j_] * s20
                                        + A[k-k_+3][j-j_] * s30;
        }
        }
      }
      for(int i=Ni__; i<Ni_; i+=D_i) {
        for(int k=k_; k<Nk__; k++) {
        auto const s00 = alpha * b[k+(i+0)*N];
        auto const s01 = alpha * b[k+(i+1)*N];
        #pragma gcc ivdep
        for(int j=j_; j<Nj_; j++) {
          auto a0 = A[k-k_][j-j_];
          C[i-i_+0][j-j_] = C[i-i_+0][j-j_] + a0 * s00;
          C[i-i_+1][j-j_] = C[i-i_+1][j-j_] + a0 * s01;
        }
        }
      }
      for(int i=Ni__; i<Ni_; i+=D_i) {
        for(int k=Nk__; k<Nk_; k+=D_k) {
        auto const s00 = alpha * b[(k+0)+(i+0)*N];
        auto const s10 = alpha * b[(k+1)+(i+0)*N];
        auto const s20 = alpha * b[(k+2)+(i+0)*N];
        auto const s30 = alpha * b[(k+3)+(i+0)*N];
        auto const s01 = alpha * b[(k+0)+(i+1)*N];
        auto const s11 = alpha * b[(k+1)+(i+1)*N];
        auto const s21 = alpha * b[(k+2)+(i+1)*N];
        auto const s31 = alpha * b[(k+3)+(i+1)*N];
        #pragma gcc ivdep
        for(int j=j_; j<Nj_; j++) {
          auto a0 = A[k-k_+0][j-j_];
          auto c0 = C[i-i_+0][j-j_];
          auto c1 = C[i-i_+1][j-j_];
          auto a1 = A[k-k_+1][j-j_];
          c0 = c0 + a0 * s00;
          auto a2 = A[k-k_+2][j-j_];
          c1 = c1 + a0 * s01;
          c0 = c0 + a1 * s10;
          auto a3 = A[k-k_+3][j-j_];
          c1 = c1 + a1 * s11;
          c0 = c0 + a2 * s20;
          c1 = c1 + a2 * s21;
          c0 = c0 + a3 * s30;
          c1 = c1 + a3 * s31;
          C[i-i_+0][j-j_] = c0;
          C[i-i_+1][j-j_] = c1;
        }
        }
      }

    }

      for(int i=i_; i<Ni_; i++) {
      #pragma gcc ivdep
      for(int j=j_; j<Nj_; j++) {
        c[j+i*N] = C[i-i_][j-j_];
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
  int dtype = atoi( argv[1] );
  while ( 1 ) {
    int N;
    std::cin >> N;
    if ( N <= 0 ) break;

    switch ( dtype ) {
#if 1
    case 0:
    benchmark<FLOAT>( N );
    break;
    case 1:
    benchmark<df_Real_quasi>( N );
    break;
    case 2:
    benchmark<df_Real>      ( N );
    break;
    case 3:
    benchmark<tf_Real_quasi>( N );
    break;
    case 4:
    benchmark<tf_Real>( N );
    break;
    case 5:
    benchmark<qf_Real_quasi>( N );
    break;
#endif
    case 6:
    benchmark<qf_Real>      ( N );
    break;
#if 1
    case 10:
    benchmark<DOUBLE>( N );
    break;
#endif
    }
  }
}

