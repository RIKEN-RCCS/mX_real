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
  INLINE auto const operator+( FLOAT const& a ) { return x + a.x; }
  INLINE auto const operator-( FLOAT const& a ) { return x - a.x; }
  INLINE auto const operator*( FLOAT const& a ) { return x * a.x; }
  INLINE auto const& operator+=( FLOAT const& a ) { return (x += a.x); }
  INLINE auto const& operator-=( FLOAT const& a ) { return (x -= a.x); }
  INLINE auto const& operator*=( FLOAT const& a ) { return (x *= a.x); }
  template < typename T, typename _dummy_=std::enable_if_t<std::is_arithmetic<T>::value> >
  INLINE auto const& operator+=( T const& a ) { return (x += a); }
  template < typename T, typename _dummy_=std::enable_if_t<std::is_arithmetic<T>::value> >
  INLINE auto const& operator-=( T const& a ) { return (x -= a); }
  template < typename T, typename _dummy_=std::enable_if_t<std::is_arithmetic<T>::value> >
  INLINE auto const& operator*=( T const& a ) { return (x *= a); }
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
  INLINE auto const operator+( DOUBLE const& a ) { return x + a.x; }
  INLINE auto const operator-( DOUBLE const& a ) { return x - a.x; }
  INLINE auto const operator*( DOUBLE const& a ) { return x * a.x; }
  INLINE auto const& operator+=( DOUBLE const& a ) { return (x += a.x); }
  INLINE auto const& operator-=( DOUBLE const& a ) { return (x -= a.x); }
  INLINE auto const& operator*=( DOUBLE const& a ) { return (x *= a.x); }
  template < typename T, typename _dummy_=std::enable_if_t<std::is_arithmetic<T>::value> >
  INLINE auto const& operator+=( T const& a ) { return (x += a); }
  template < typename T, typename _dummy_=std::enable_if_t<std::is_arithmetic<T>::value> >
  INLINE auto const& operator-=( T const& a ) { return (x -= a); }
  template < typename T, typename _dummy_=std::enable_if_t<std::is_arithmetic<T>::value> >
  INLINE auto const& operator*=( T const& a ) { return (x *= a); }
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
  beta  = REAL::rand();


  int constexpr ITR = 10;
  double t = 0.;

  for(int itr=0; itr<ITR+1; itr++) {

    auto const ts = omp_get_wtime();

    //
    // optimized GEMV-n kernel
    //
    int constexpr STEP_i = (sizeof(a[0]) >= 3*sizeof(float) ? 36 : 72);
    int constexpr STEP_j = (sizeof(a[0]) >= 4*sizeof(float) ? 36 : 72);
    int constexpr STEP_k = (sizeof(a[0]) >= 4*sizeof(float) ? 36 : 72);

    int constexpr D_i = 2;
    int constexpr D_k = 4;


    #pragma omp parallel
    {
    #pragma omp for collapse(2) schedule(dynamic)
    for(int i_=0; i_<N; i_+=STEP_i) {
    for(int j_=0; j_<N; j_+=STEP_j) {
      auto const Ni_ = std::min( i_+STEP_i, N );
      auto const Nj_ = std::min( j_+STEP_j, N );

      REAL C[STEP_i][STEP_j]; // 36*36*4*4=20.25KB
      for(int i=i_; i<Ni_; i++) {
      #pragma gcc ivdep
      #pragma ivdep
      for(int j=j_; j<Nj_; j++) {
        C[i-i_][j-j_] = c[j+i*N];
      }}
#if 0
      for(int i=i_; i<Ni_; i++) {
      #pragma vector
      for(int j=j_; j<Nj_; j++) {
        C[i-i_][j-j_] *= beta;
      }}
#endif
    for(int k_=0; k_<N; k_+=STEP_k) {
      auto const Nk_ = std::min( k_+STEP_k, N );

      auto const Ni__ = Ni_-(Ni_-i_)%D_i;
      auto const Nk__ = Nk_-(Nk_-k_)%D_k;

      REAL A[STEP_k][STEP_j]; // 36*36*4*4=20.25KB
      for(int k=k_; k<Nk_; k++) {
      #pragma gcc ivdep
      #pragma ivdep
      for(int j=j_; j<Nj_; j++) {
        A[k-k_][j-j_] = a[j+k*N];
      }}

if ( D_k==4 ) {

      for(int k=k_; k<Nk__; k+=D_k) {
      for(int i=i_; i<Ni__; i+=D_i) {
        if ( k_==0 ) {
        for(int j=j_; j<Nj_; j++) {
          C[i-i_+0][j-j_] *= beta;
          C[i-i_+1][j-j_] *= beta;
	}}
        auto const s00 = alpha * b[(k+0)+(i+0)*N];
        auto const s10 = alpha * b[(k+1)+(i+0)*N];
        auto const s20 = alpha * b[(k+2)+(i+0)*N];
        auto const s30 = alpha * b[(k+3)+(i+0)*N];
        auto const s01 = alpha * b[(k+0)+(i+1)*N];
        auto const s11 = alpha * b[(k+1)+(i+1)*N];
        auto const s21 = alpha * b[(k+2)+(i+1)*N];
        auto const s31 = alpha * b[(k+3)+(i+1)*N];
        for(int j=j_; j<Nj_; j++) {
          auto c0 = C[i-i_+0][j-j_];
          auto c1 = C[i-i_+1][j-j_];
          auto a0 = A[k-k_+0][j-j_];
          auto a1 = A[k-k_+1][j-j_];
          c0 += a0 * s00;
          c1 += a0 * s01;
          auto a2 = A[k-k_+2][j-j_];
          c0 += a1 * s10;
          c1 += a1 * s11;
          auto a3 = A[k-k_+3][j-j_];
          c0 += a2 * s20;
          c1 += a2 * s21;
          c0 += a3 * s30;
          c1 += a3 * s31;
          C[i-i_+0][j-j_] = c0;
          C[i-i_+1][j-j_] = c1;
        }
      }}

      for(int k=k_; k<Nk__; k+=D_k) {
      for(int i=Ni__; i<Ni_; i++) {
        if ( k_==0 ) {
        for(int j=j_; j<Nj_; j++) {
          C[i-i_+0][j-j_] = beta * C[i-i_+0][j-j_];
	}}
        auto const s00 = alpha * b[(k+0)+(i+0)*N];
        auto const s10 = alpha * b[(k+1)+(i+0)*N];
        auto const s20 = alpha * b[(k+2)+(i+0)*N];
        auto const s30 = alpha * b[(k+3)+(i+0)*N];
        for(int j=j_; j<Nj_; j++) {
          auto c0 = C[i-i_+0][j-j_];
          auto a0 = A[k-k_+0][j-j_];
          auto a1 = A[k-k_+1][j-j_];
          auto a2 = A[k-k_+2][j-j_];
          c0 += a0 * s00;
          auto a3 = A[k-k_+3][j-j_];
          c0 += a1 * s10;
          c0 += a2 * s20;
          c0 += a3 * s30;
          C[i-i_+0][j-j_] = c0;
        }
      }}

}
if ( D_k==3 ) {

      for(int k=k_; k<Nk__; k+=D_k) {
      for(int i=i_; i<Ni__; i+=D_i) {
        if ( k_==0 ) {
        for(int j=j_; j<Nj_; j++) {
          C[i-i_+0][j-j_] *= beta;
          C[i-i_+1][j-j_] *= beta;
	}}
        auto const s00 = alpha * b[(k+0)+(i+0)*N];
        auto const s10 = alpha * b[(k+1)+(i+0)*N];
        auto const s20 = alpha * b[(k+2)+(i+0)*N];
        auto const s01 = alpha * b[(k+0)+(i+1)*N];
        auto const s11 = alpha * b[(k+1)+(i+1)*N];
        auto const s21 = alpha * b[(k+2)+(i+1)*N];
        for(int j=j_; j<Nj_; j++) {
          auto c0 = C[i-i_+0][j-j_];
          auto c1 = C[i-i_+1][j-j_];
          auto a0 = A[k-k_+0][j-j_];
          auto a1 = A[k-k_+1][j-j_];
          c0 += a0 * s00;
          c1 += a0 * s01;
          auto a2 = A[k-k_+2][j-j_];
          c0 += a1 * s10;
          c1 += a1 * s11;
          c0 += a1 * s20;
          c1 += a1 * s21;
          C[i-i_+0][j-j_] = c0;
          C[i-i_+1][j-j_] = c1;
        }
      }}

      for(int k=k_; k<Nk__; k+=D_k) {
      for(int i=Ni__; i<Ni_; i++) {
        if ( k_==0 ) {
        for(int j=j_; j<Nj_; j++) {
          C[i-i_+0][j-j_] *= beta;
	}}
        auto const s00 = alpha * b[(k+0)+i*N];
        auto const s10 = alpha * b[(k+1)+i*N];
        auto const s20 = alpha * b[(k+2)+i*N];
        for(int j=j_; j<Nj_; j++) {
          auto c0 = C[i-i_+0][j-j_];
          auto a0 = A[k-k_+0][j-j_];
          auto a1 = A[k-k_+1][j-j_];
          c0 += a0 * s00;
          auto a2 = A[k-k_+2][j-j_];
          c0 += a1 * s10;
          c0 += a2 * s20;
          C[i-i_+0][j-j_] = c0;
        }
      }}

}
if ( D_k==2 ) {

      for(int k=k_; k<Nk__; k+=D_k) {
      for(int i=i_; i<Ni__; i+=D_i) {
        if ( k_==0 ) {
        for(int j=j_; j<Nj_; j++) {
          C[i-i_+0][j-j_] *= beta;
          C[i-i_+1][j-j_] *= beta;
	}}
        auto const s00 = alpha * b[(k+0)+(i+0)*N];
        auto const s10 = alpha * b[(k+1)+(i+0)*N];
        auto const s01 = alpha * b[(k+0)+(i+1)*N];
        auto const s11 = alpha * b[(k+1)+(i+1)*N];
        for(int j=j_; j<Nj_; j++) {
          auto c0 = C[i-i_+0][j-j_];
          auto c1 = C[i-i_+1][j-j_];
          auto a0 = A[k-k_+0][j-j_];
          auto a1 = A[k-k_+1][j-j_];
          c0 += a0 * s00;
          c1 += a0 * s01;
          c0 += a1 * s10;
          c1 += a1 * s11;
          C[i-i_+0][j-j_] = c0;
          C[i-i_+1][j-j_] = c1;
        }
      }}

      for(int k=k_; k<Nk__; k+=D_k) {
      for(int i=Ni__; i<Ni_; i++) {
        if ( k_==0 ) {
        for(int j=j_; j<Nj_; j++) {
          C[i-i_+0][j-j_] *= beta;
	}}
        auto const s00 = alpha * b[(k+0)+i*N];
        auto const s10 = alpha * b[(k+1)+i*N];
        for(int j=j_; j<Nj_; j++) {
          auto c0 = C[i-i_+0][j-j_];
          auto a0 = A[k-k_+0][j-j_];
          auto a1 = A[k-k_+1][j-j_];
          c0 += a0 * s00;
          c0 += a1 * s10;
          C[i-i_+0][j-j_] = c0;
        }
      }}

}
if ( D_k==1 ) {

      for(int k=k_; k<Nk__; k+=D_k) {
      for(int i=i_; i<Ni__; i+=D_i) {
        if ( k_==0 ) {
        for(int j=j_; j<Nj_; j++) {
          C[i-i_+0][j-j_] *= beta;
          C[i-i_+1][j-j_] *= beta;
	}}
        auto const s00 = alpha * b[(k+0)+(i+0)*N];
        auto const s01 = alpha * b[(k+0)+(i+1)*N];
        for(int j=j_; j<Nj_; j++) {
          auto c0 = C[i-i_+0][j-j_];
          auto c1 = C[i-i_+1][j-j_];
          auto a0 = A[k-k_+0][j-j_];
          c0 += a0 * s00;
          c1 += a0 * s01;
          C[i-i_+0][j-j_] = c0;
          C[i-i_+1][j-j_] = c1;
        }
      }}

      for(int k=k_; k<Nk__; k+=D_k) {
      for(int i=Ni__; i<Ni_; i++) {
        if ( k_==0 ) {
        for(int j=j_; j<Nj_; j++) {
          C[i-i_+0][j-j_] *= beta;
	}}
        auto const s00 = alpha * b[(k+0)+i*N];
        for(int j=j_; j<Nj_; j++) {
          auto c0 = C[i-i_+0][j-j_];
          auto a0 = A[k-k_+0][j-j_];
          c0 = c0 + a0 * s00;
          C[i-i_+0][j-j_] = c0;
        }
      }}

}
if ( D_k!=1 ) {

      for(int k=Nk__; k<Nk_; k++) {
      for(int i=i_; i<Ni__; i+=D_i) {
        if ( k_==0 ) {
        for(int j=j_; j<Nj_; j++) {
          C[i-i_+0][j-j_] *= beta;
          C[i-i_+1][j-j_] *= beta;
	}}
        auto const s00 = alpha * b[(k+0)+(i+0)*N];
        auto const s01 = alpha * b[(k+0)+(i+1)*N];
        for(int j=j_; j<Nj_; j++) {
          auto c0 = C[i-i_+0][j-j_];
          auto a0 = A[k-k_+0][j-j_];
          c0 += a0 * s00;
          auto c1 = C[i-i_+1][j-j_];
          c1 += a0 * s01;
          C[i-i_+0][j-j_] = c0;
          C[i-i_+1][j-j_] = c1;
        }
      }}

      for(int k=Nk__; k<Nk_; k++) {
      for(int i=Ni__; i<Ni_; i++) {
        if ( k_==0 ) {
        for(int j=j_; j<Nj_; j++) {
          C[i-i_+0][j-j_] *= beta;
	}}
        auto const s00 = alpha * b[(k+0)+(i+0)*N];
        for(int j=j_; j<Nj_; j++) {
          auto c0 = C[i-i_+0][j-j_];
          auto a0 = A[k-k_+0][j-j_];
          c0 += a0 * s00;
          C[i-i_+0][j-j_] = c0;
        }
      }}

}

    }

      for(int i=i_; i<Ni_; i++) {
      #pragma gcc ivdep
      #pragma ivdep
      for(int j=j_; j<Nj_; j++) {
        c[j+i*N] = C[i-i_][j-j_];
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

