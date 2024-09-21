#include <stdio.h>
#include <omp.h>
//#include <iomanip>
#include <iostream>

#include "mX_real.hpp"
using namespace mX_real;

#if (defined(__INTEL_CLANG_COMPILER)||defined(__INTEL_LLVM_COMPILER)||defined(__INTEL_COMPILER))
#define	__isINTEL_COMPILER__	1
#else
#define	__isINTEL_COMPILER__	0
#endif

enum class _Matrix_format_ {
  //
  Column_Major = 0,
  Row_Major    = 1,
  Default      = Column_Major
};

template < typename T, _Matrix_format_ format=_Matrix_format_::Default, bool Check_Index_Row=false, bool Check_Index_Column=false >
struct _Matrix_ {
  private:
    using pure_T   = typename std::remove_const_t<T>;
    using type     = typename std::conditional_t< std::is_const<T>::value, pure_T const, pure_T & >;
    using type_ptr = typename std::conditional_t< std::is_const<T>::value, pure_T const *, pure_T *>;

    T * const pointer = nullptr;
    int64_t const leading_dim = 0; 
    int64_t const Row_size = 0; 
    int64_t const Column_size = 0; 

  public:
    _Matrix_( ... ) { assert(false); };

    _Matrix_( T * const ptr, int64_t const row_size, int64_t const column_size, int64_t const ld ) :
      pointer( const_cast<T*>(ptr) ),
      Row_size( row_size ),
      Column_size( column_size ),
      leading_dim( ld ) {
	assert( ptr != nullptr );
	assert( row_size > 0 );
	assert( column_size > 0 );
	assert( ld>=row_size );
    }

    _Matrix_( T * const ptr, int64_t const row_size, int64_t const column_size ) :
      pointer( const_cast<T*>(ptr) ),
      Row_size( row_size ),
      Column_size( column_size ),
      leading_dim( row_size ) {
	assert( ptr != nullptr );
	assert( row_size > 0 );
	assert( column_size > 0 );
    }

    _Matrix_( T * const ptr, int64_t const row_size ) :
      pointer( const_cast<T*>(ptr) ),
      Row_size( row_size ),
      Column_size( row_size ),
      leading_dim( row_size ) {
	assert( ptr != nullptr );
	assert( row_size > 0 );
    }

  private:
    _Matrix_( _Matrix_ const &any ) = delete;
    _Matrix_ &operator= ( _Matrix_ const &any ) = delete;
    auto operator[] ( int64_t const offset ) { assert( false ); }

  public:
    // Column-major format is default
    inline type operator() ( int64_t const row, int64_t const col ) {
      if ( Check_Index_Row ) {
        assert ( 0 <= row );
       	assert( row < Row_size );
      }
      if ( Check_Index_Column ) {
        assert ( 0 <= col );
       	assert( col < Column_size );
      }
      int64_t index = 0;
      if ( format == _Matrix_format_::Column_Major ) {
        index = row + col * leading_dim;
      }
      if ( format == _Matrix_format_::Row_Major ) {
        index = col + row * leading_dim;
      }
      return * ( pointer + index );
    }
};

struct FLOAT {
  float x;
  INLINE FLOAT() {}
  INLINE FLOAT(float const& h) : x(h) {}
  INLINE FLOAT(FLOAT const& h) : x(h.x) {}
  INLINE operator float() const { return x; }
  static
  INLINE FLOAT rand() {
    auto constexpr f = 1.0f / (1<<16) / (1<<15);
    auto const b_ = std::rand();
    auto const b = float( b_ & 0x7fff0000 );
    auto const c = b * f;
    return FLOAT(c);
  }
  INLINE auto const operator+( FLOAT const& a ) { return FLOAT(x + a.x); }
  INLINE auto const operator-( FLOAT const& a ) { return FLOAT(x - a.x); }
  INLINE auto const operator*( FLOAT const& a ) { return FLOAT(x * a.x); }
  INLINE auto const& operator=( FLOAT const& a ) { x = a.x; return *this; }
  INLINE auto const& operator+=( FLOAT const& a ) { x += a.x; return *this; }
  INLINE auto const& operator-=( FLOAT const& a ) { x -= a.x; return *this; }
  INLINE auto const& operator*=( FLOAT const& a ) { x *= a.x; return *this; }
  template < typename T, typename _dummy_=std::enable_if_t<std::is_arithmetic<T>::value> >
  INLINE auto const& operator=( T const& a ) { x = a; return *this; }
  template < typename T, typename _dummy_=std::enable_if_t<std::is_arithmetic<T>::value> >
  INLINE auto const& operator+=( T const& a ) { x += a; return *this; }
  template < typename T, typename _dummy_=std::enable_if_t<std::is_arithmetic<T>::value> >
  INLINE auto const& operator-=( T const& a ) { x -= a; return *this; }
  template < typename T, typename _dummy_=std::enable_if_t<std::is_arithmetic<T>::value> >
  INLINE auto const& operator*=( T const& a ) { x *= a; return *this; }
};

struct DOUBLE {
  double x;
  INLINE DOUBLE() {}
  INLINE DOUBLE(double const& h) : x(h) {}
  INLINE DOUBLE(DOUBLE const& h) : x(h.x) {}
  INLINE operator double() const { return x; }
  static
  INLINE DOUBLE rand() {
    auto constexpr f = 1.0f / (1<<16) / (1<<15);
    auto const b_ = std::rand();
    auto const b = double( b_ );
    auto const c = b * f;
    auto const d_ = std::rand();
    auto const d = double( d_ );
    auto const e = d * f * f;
    return DOUBLE(c + e);
  }
  INLINE auto const operator+( DOUBLE const& a ) { return DOUBLE(x + a.x); }
  INLINE auto const operator-( DOUBLE const& a ) { return DOUBLE(x - a.x); }
  INLINE auto const operator*( DOUBLE const& a ) { return DOUBLE(x * a.x); }
  INLINE auto const& operator=( DOUBLE const& a ) { x = a.x; return *this; }
  INLINE auto const& operator+=( DOUBLE const& a ) { x += a.x; return *this; }
  INLINE auto const& operator-=( DOUBLE const& a ) { x -= a.x; return *this; }
  INLINE auto const& operator*=( DOUBLE const& a ) { x *= a.x; return *this; }
  template < typename T, typename _dummy_=std::enable_if_t<std::is_arithmetic<T>::value> >
  INLINE auto const& operator=( T const& a ) { x = a; return *this; }
  template < typename T, typename _dummy_=std::enable_if_t<std::is_arithmetic<T>::value> >
  INLINE auto const& operator+=( T const& a ) { x += a; return *this; }
  template < typename T, typename _dummy_=std::enable_if_t<std::is_arithmetic<T>::value> >
  INLINE auto const& operator-=( T const& a ) { x -= a; return *this; }
  template < typename T, typename _dummy_=std::enable_if_t<std::is_arithmetic<T>::value> >
  INLINE auto const& operator*=( T const& a ) { x *= a; return *this; }
};


template < typename REAL >
void benchmark( int const& N ) {
  
  auto * a = new REAL[N*N];
  auto * b = new REAL[N*N];
  auto * c = new REAL[N*N];
  REAL alpha, beta;
  
  _Matrix_<REAL> a_( a, N, N, N );
  _Matrix_<REAL> b_( b, N, N, N );
  _Matrix_<REAL> c_( c, N, N, N );

  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      a_(i,j) = REAL::rand();
      b_(i,j) = REAL::rand();
      c_(i,j) = REAL::rand();
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
    int constexpr STEP_j = (sizeof(a[0]) >= 4*sizeof(float) ? 32 : 64);
    int constexpr STEP_k = (sizeof(a[0]) >= 4*sizeof(float) ? 36 : 72);
    
    int constexpr D_i = 2;
//    int constexpr D_k = 2;
//    int constexpr D_k = 3;
    int constexpr D_k = 4;
    
    
#pragma omp parallel
    {

      // private buffers for matrices a and c
      auto * _A_ = new REAL[STEP_k*STEP_j];
      auto * _C_ = new REAL[STEP_i*STEP_j];

      _Matrix_<REAL> A( _A_, STEP_j, STEP_k, STEP_j );
      _Matrix_<REAL> C( _C_, STEP_j, STEP_i, STEP_j );

#pragma omp for collapse(2) schedule(dynamic)
      for(int i_=0; i_<N; i_+=STEP_i) {
        for(int j_=0; j_<N; j_+=STEP_j) {
          auto const Ni_ = std::min( i_+STEP_i, N );
          auto const Nj_ = std::min( j_+STEP_j, N );
          
	  //
	  //
          for(int i=i_; i<Ni_; i++) {
            for(int j=j_; j<Nj_; j++) {
              C(j-j_,i-i_) = c_(j,i);
            } // j
	  } // i
	  //
	  //

          for(int k_=0; k_<N; k_+=STEP_k) {
            auto const Nk_ = std::min( k_+STEP_k, N );
            
            auto const Ni__ = Ni_-(Ni_-i_)%D_i;
            auto const Nk__ = Nk_-(Nk_-k_)%D_k;
            
	    //
	    //
            for(int k=k_; k<Nk_; k++) {
              for(int j=j_; j<Nj_; j++) {
                A(j-j_,k-k_) = alpha * a_(j,k);
              } // j
	    } // i
	    //
	    //
            
            if ( D_k==4 ) {
              
              for(int k=k_; k<Nk__; k+=D_k) {
                for(int i=i_; i<Ni__; i+=D_i) {

                  if ( k==0 ) {
                    for(int j=j_; j<Nj_; j++) {
                      C(j-j_,i-i_+0) *= beta;
                      C(j-j_,i-i_+1) *= beta;
                    } // j
		  }

                  auto const s00 = b_(k+0,i+0);
                  auto const s10 = b_(k+1,i+0);
                  auto const s20 = b_(k+2,i+0);
                  auto const s30 = b_(k+3,i+0);
                  auto const s01 = b_(k+0,i+1);
                  auto const s11 = b_(k+1,i+1);
                  auto const s21 = b_(k+2,i+1);
                  auto const s31 = b_(k+3,i+1);
                  for(int j=j_; j<Nj_; j++) {
                    auto c0 = C(j-j_,i-i_+0);
                    auto c1 = C(j-j_,i-i_+1);
                    auto a0 = A(j-j_,k-k_+0);
                    auto a1 = A(j-j_,k-k_+1);
                    auto a2 = A(j-j_,k-k_+2);
                    auto a3 = A(j-j_,k-k_+3);
                    c0 += a0 * s00;
                    c1 += a0 * s01;
                    c0 += a1 * s10;
                    c1 += a1 * s11;
                    c0 += a2 * s20;
                    c1 += a2 * s21;
                    c0 += a3 * s30;
                    c1 += a3 * s31;
                    C(j-j_,i-i_+0) = c0;
                    C(j-j_,i-i_+1) = c1;
                  } // j
                } // i
	      } // k
              
              for(int k=k_; k<Nk__; k+=D_k) {
                for(int i=Ni__; i<Ni_; i++) {

                  if ( k==0 ) {
                    for(int j=j_; j<Nj_; j++) {
                      C(i-i_+0,j-j_) *= beta;
                    } // j
		  }

                  auto const s00 = b_(k+0,i+0);
                  auto const s10 = b_(k+1,i+0);
                  auto const s20 = b_(k+2,i+0);
                  auto const s30 = b_(k+3,i+0);
                  for(int j=j_; j<Nj_; j++) {
                    auto c0 = C(j-j_,i-i_+0);
                    auto a0 = A(j-j_,k-k_+0);
                    auto a1 = A(j-j_,k-k_+1);
                    auto a2 = A(j-j_,k-k_+2);
                    auto a3 = A(j-j_,k-k_+3);
                    c0 += a0 * s00;
                    c0 += a1 * s10;
                    c0 += a2 * s20;
                    c0 += a3 * s30;
                    C(j-j_,i-i_+0) = c0;
                  } // j
                } // i
	      } // k
              
            }
            if ( D_k==3 ) {
              
              for(int k=k_; k<Nk__; k+=D_k) {
                for(int i=i_; i<Ni__; i+=D_i) {

                  if ( k==0 ) {
                    for(int j=j_; j<Nj_; j++) {
                      C(j-j_,i-i_+0) *= beta;
                      C(j-j_,i-i_+1) *= beta;
                    } // j
		  }

                  auto const s00 = b_(k+0,i+0);
                  auto const s10 = b_(k+1,i+0);
                  auto const s20 = b_(k+2,i+0);
                  auto const s01 = b_(k+0,i+1);
                  auto const s11 = b_(k+1,i+1);
                  auto const s21 = b_(k+2,i+1);
                  for(int j=j_; j<Nj_; j++) {
                    auto c0 = C(j-j_,i-i_+0);
                    auto c1 = C(j-j_,i-i_+1);
                    auto a0 = A(j-j_,k-k_+0);
                    auto a1 = A(j-j_,k-k_+1);
                    auto a2 = A(j-j_,k-k_+2);
                    c0 += a0 * s00;
                    c1 += a0 * s01;
                    c0 += a1 * s10;
                    c1 += a1 * s11;
                    c0 += a2 * s20;
                    c1 += a2 * s21;
                    C(j-j_,i-i_+0) = c0;
                    C(j-j_,i-i_+1) = c1;
                  } // j
                } // i
	      } // k
              
              for(int k=k_; k<Nk__; k+=D_k) {
                for(int i=Ni__; i<Ni_; i++) {

                  if ( k==0 ) {
                    for(int j=j_; j<Nj_; j++) {
                      C(j-j_,i-i_+0) *= beta;
                    } // j
		  }

                  auto const s00 = b_(k+0,i+0);
                  auto const s10 = b_(k+1,i+0);
                  auto const s20 = b_(k+2,i+0);
                  for(int j=j_; j<Nj_; j++) {
                    auto c0 = C(j-j_,i-i_+0);
                    auto a0 = A(j-j_,k-k_+0);
                    auto a1 = A(j-j_,k-k_+1);
                    auto a2 = A(j-j_,k-k_+2);
                    c0 += a0 * s00;
                    c0 += a1 * s10;
                    c0 += a2 * s20;
                    C(j-j_,i-i_+0) = c0;
                  } // j
                } // i
	      } // k
              
            }
            if ( D_k==2 ) {
              
              for(int k=k_; k<Nk__; k+=D_k) {
                for(int i=i_; i<Ni__; i+=D_i) {

                  if ( k==0 ) {
                    for(int j=j_; j<Nj_; j++) {
                      C(j-j_,i-i_+0) *= beta;
                      C(j-j_,i-i_+1) *= beta;
                    } // j
		  }

                  auto const s00 = b_(k+0,i+0);
                  auto const s10 = b_(k+1,i+0);
                  auto const s01 = b_(k+0,i+1);
                  auto const s11 = b_(k+1,i+1);
                  for(int j=j_; j<Nj_; j++) {
                    auto c0 = C(j-j_,i-i_+0);
                    auto c1 = C(j-j_,i-i_+1);
                    auto a0 = A(j-j_,k-k_+0);
                    auto a1 = A(j-j_,k-k_+1);
                    c0 += a0 * s00;
                    c1 += a0 * s01;
                    c0 += a1 * s10;
                    c1 += a1 * s11;
                    C(j-j_,i-i_+0) = c0;
                    C(j-j_,i-i_+1) = c1;
                  } // j
                } // i
	      } // k
              
              for(int k=k_; k<Nk__; k+=D_k) {
                for(int i=Ni__; i<Ni_; i++) {

                  if ( k==0 ) {
                    for(int j=j_; j<Nj_; j++) {
                      C(j-j_,i-i_+0) *= beta;
                    } // j
		  }

                  auto const s00 = b_(k+0,i+0);
                  auto const s10 = b_(k+1,i+0);
                  for(int j=j_; j<Nj_; j++) {
                    auto c0 = C(j-j_,i-i_+0);
                    auto a0 = A(j-j_,k-k_+0);
                    auto a1 = A(j-j_,k-k_+1);
                    c0 += a0 * s00;
                    c0 += a1 * s10;
                    C(j-j_,i-i_+0) = c0;
                  } // j
                } // i
	      } // k
              
            }
            if ( D_k==1 ) {
              
              for(int k=k_; k<Nk__; k+=D_k) {
                for(int i=i_; i<Ni__; i+=D_i) {

                  if ( k==0 ) {
                    for(int j=j_; j<Nj_; j++) {
                      C(j-j_,i-i_+0) *= beta;
                      C(j-j_,i-i_+1) *= beta;
                    } // j
		  }

                  auto const s00 = b_(k+0,i+0);
                  auto const s01 = b_(k+0,i+1);
                  for(int j=j_; j<Nj_; j++) {
                    auto c0 = C(j-j_,i-i_+0);
                    auto c1 = C(j-j_,i-i_+1);
                    auto a0 = A(j-j_,k-k_+0);
                    c0 += a0 * s00;
                    c1 += a0 * s01;
                    C(j-j_,i-i_+0) = c0;
                    C(j-j_,i-i_+1) = c1;
                  } // j
                } // i
	      } // k
              
              for(int k=k_; k<Nk__; k+=D_k) {
                for(int i=Ni__; i<Ni_; i++) {

                  if ( k==0 ) {
                    for(int j=j_; j<Nj_; j++) {
                      C(j-j_,i-i_+0) *= beta;
                    } // j
		  }

                  auto const s00 = b_(k+0,i+0);
                  for(int j=j_; j<Nj_; j++) {
                    auto c0 = C(j-j_,i-i_+0);
                    auto a0 = A(j-j_,k-k_+0);
                    c0 = c0 + a0 * s00;
                    C(j-j_,i-i_+0) = c0;
                  } // j
                } // i
	      } // k
              
            }
            if ( D_k!=1 ) {
              
              for(int k=Nk__; k<Nk_; k++) {
                for(int i=i_; i<Ni__; i+=D_i) {

                  if ( k==0 ) {
                    for(int j=j_; j<Nj_; j++) {
                      C(j-j_,i-i_+0) *= beta;
                      C(j-j_,i-i_+1) *= beta;
                    } // j
		  }

                  auto const s00 = b_(k+0,i+0);
                  auto const s01 = b_(k+0,i+1);
                  for(int j=j_; j<Nj_; j++) {
                    auto c0 = C(j-j_,i-i_+0);
                    auto c1 = C(j-j_,i-i_+1);
                    auto a0 = A(j-j_,k-k_+0);
                    c0 += a0 * s00;
                    c1 += a0 * s01;
                    C(j-j_,i-i_+0) = c0;
                    C(j-j_,i-i_+1) = c1;
                  } // j
                } // i
	      } // k

              for(int k=Nk__; k<Nk_; k++) {
                for(int i=Ni__; i<Ni_; i++) {

                  if ( k==0 ) {
                    for(int j=j_; j<Nj_; j++) {
                      C(j-j_,i-i_+0) *= beta;
                    } // j
		  }

                  auto const s00 = b_(k+0,i+0);
                  for(int j=j_; j<Nj_; j++) {
                    auto c0 = C(j-j_,i-i_+0);
                    auto a0 = A(j-j_,k-k_+0);
                    c0 += a0 * s00;
                    C(j-j_,i-i_+0) = c0;
                  } // j
                } // i
	      } // k
              
            }
            
          }
          
	  //
	  //
          for(int i=i_; i<Ni_; i++) {
            for(int j=j_; j<Nj_; j++) {
              c_(j,i) = C(j-j_,i-i_);
            } // j
          } // i
	  //
	  //
          
        }
      }

      delete [] _A_;
      delete [] _C_;
#undef	A
#undef	C

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
    case 0:
      std::cout << "[0]: float         : ";
      benchmark<FLOAT>         ( N );
      break;

    case 1:
      std::cout << "[1]: df_Real_quasi : ";
      benchmark<df_Real_quasi> ( N );
      break;
    case 2:
      std::cout << "[2]: df_Real_sloppy: ";
      benchmark<df_Real_sloppy>( N );
      break;
    case 3:
      std::cout << "[3]: df_Real       : ";
      benchmark<df_Real>       ( N );
      break;

    case 4:
      std::cout << "[4]: tf_Real_quasi : ";
      benchmark<tf_Real_quasi> ( N );
      break;
    case 5:
      std::cout << "[5]: tf_Real_sloppy: ";
      benchmark<tf_Real_sloppy>( N );
      break;
    case 6:
      std::cout << "[6]: tf_Real       : ";
      benchmark<tf_Real>       ( N );
      break;

    case 7:
      std::cout << "[7]: qf_Real_quasi : ";
      benchmark<qf_Real_quasi> ( N );
      break;
    case 8:
      std::cout << "[8]: qf_Real_sloppy: ";
      benchmark<qf_Real_sloppy>( N );
      break;
    case 9:
      std::cout << "[9]: qf_Real       : ";
      benchmark<qf_Real>       ( N );
      break;

#if 1
    case 10:
      std::cout << "[10]: double       : ";
      benchmark<DOUBLE>        ( N );
      break;
#endif
    }
  }
}

