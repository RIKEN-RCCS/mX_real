#include <stdio.h>
#include <omp.h>
#include <iomanip>

#include <qd/dd_real.h>
#include <qd/qd_real.h>

#include "mpreal.h"

#include "mX_real.hpp"
using namespace mX_real;
#include "io.hpp"
#include "mpfr_convert.hpp"

using mp_real    =  mpfr::mpreal;



template < typename T >
T nrm2( int const& L, T const* x ) {
 T z = 0;
 #pragma gcc ivdep
 for(int i=0; i<L; i++) {
   z = z + x[i] * x[i];
 }
 return sqrt(z);
}

template < typename T >
T asum( int const& L, T const* x ) {
 T z = 0;
 #pragma gcc ivdep
 for(int i=0; i<L; i++) {
   z = z + abs( x[i] );
 }
 return z;
}

template < typename T >
T dot( int const& L, T const* x, T const *y ) {
 T z = 0;
 #pragma gcc ivdep
 for(int i=0; i<L; i++) {
   z = z + x[i] * y[i];
 }
 return z;
}

template < typename T >
void axpy( int const& L, T const& alpha, T const* x, T *y ) {
 #pragma gcc ivdep
 for(int i=0; i<L; i++) {
   y[i] = alpha * x[i] + y[i];
 }
}

template < typename T >
void gemv( int const& L, int const& N, T const& alpha, T const *a, T const* x, T *y ) {
 for(int j=0; j<N; j++) {
   auto s = alpha * x[j];
   #pragma gcc ivdep
   for(int i=0; i<L; i++) {
     y[i] = y[i] + a[i+j*L] * s;
   }
 }
}



void
init( int const L, mp_real const& alpha, mp_real * x, mp_real * y, mp_real * z )
{
  unsigned int seed = 1;
  mpfr::random( seed );

  #pragma gcc ivdep
  for(int i=0; i<L; i++) {
    x[i] = mpfr::random();
    y[i] = mpfr::random();
    z[i+8] = y[i];
  }
  axpy( L, alpha, x, z+8 );

  int M = (int)sqrt((double)L);
  #pragma gcc ivdep
  for(int i=0; i<M; i++) {
    z[i+8+L] = y[i];
  }
  gemv( M, M, alpha, x, y, z+8+L );

  z[0] = dot( L, x, y );
  z[1] = nrm2( L, x );
  z[2] = asum( L, x );
  z[3] = nrm2( L, z+8 );
  z[4] = nrm2( M, z+8+L );

}


template < typename T >
void verify( int const &L, mp_real const& Alpha, mp_real *X, mp_real *Y, mp_real *Z ) {

  int const ITR = 10;
  int const M = (int)sqrt((double)L);

  auto * x = new T[L];
  auto * y = new T[L];

  // round mp_real -> T
  {
    for(int i=0; i<L; i++) {
      x[i] = convert<T>( X[i] );
      y[i] = convert<T>( Y[i] );
    }
  }
  T const alpha = convert<T>( Alpha );


  {
    auto * w = new T[ITR];

    double ts = omp_get_wtime();
    for(int itr=0; itr<ITR; itr++) {
      w[itr] = dot( L, x, y );
    }
    double te = omp_get_wtime();

    printf("  %le[s] : ",(te-ts)/ITR);
    auto ans = convert<T>( abs( ( convert(w[0]) - Z[0] ) / Z[0] ) );
    print( "dot  ", ans );

    delete [] w;
  }


  {
    auto * w = new T[ITR];

    double ts = omp_get_wtime();
    for(int itr=0; itr<ITR; itr++) {
      w[itr] = nrm2( L, x );
    }
    double te = omp_get_wtime();

    printf("  %le[s] : ",(te-ts)/ITR);
    auto ans = convert<T>( abs( ( convert(w[0]) - Z[1] ) / Z[1] ) );
    print( "nrm2 ", ans );

    delete [] w;
  }


  {
    auto * w = new T[ITR];

    double ts = omp_get_wtime();
    for(int itr=0; itr<ITR; itr++) {
      w[itr] = asum( L, x );
    }
    double te = omp_get_wtime();

    printf("  %le[s] : ",(te-ts)/ITR);
    auto ans = convert<T>( abs( ( convert(w[0]) - Z[2] ) / Z[2] ) );
    print( "asum ", ans );

    delete [] w;
  }

  {
    axpy( L, alpha, x, y );
    mp_real r = 0;
    for(int i=0; i<L; i++) {
      auto t = Z[i+8] - convert(y[i]);
      r = r + t * t;
    }
    r = sqrt( r );

    double ts = omp_get_wtime();
    for(int itr=0; itr<ITR; itr++) {
      axpy( L, alpha, x, y );
    }
    double te = omp_get_wtime();

    printf("  %le[s] : ",(te-ts)/ITR);
    auto ans = convert<T>( r/Z[3] );
    print( "axpy ", ans );
  }

  {
    auto * w = new T[M];

    for(int i=0; i<M; i++) {
      y[i] = convert<T>( Y[i] );
      w[i] = y[i];
    }

    gemv( M, M, alpha, x, y, w );
    mp_real r = 0;
    for(int i=0; i<M; i++) {
      auto t = Z[i+8+L] - convert(w[i]);
      r = r + t * t;
    }
    r = sqrt( r );

    double ts = omp_get_wtime();
    for(int itr=0; itr<ITR; itr++) {
      gemv( M, M, alpha, x, y, w );
    }
    double te = omp_get_wtime();

    printf("  %le[s] : ",(te-ts)/ITR);
    auto ans = convert<T>( r/Z[4] );
    print( "gemv ", ans );

    delete [] w;
  }

  delete [] x;
  delete [] y;
}


int
main(int argc, char *argv[])
{
  std::cout << std::scientific << std::setprecision(15);
  mp_real::set_default_prec(64*4);

  int constexpr L = 1000*1000;
  int constexpr M = (int)sqrt((double)L);

  auto * x = new mp_real[L];
  auto * y = new mp_real[L];
  auto * z = new mp_real[L+8+M];

  auto alpha = sqrt( mp_real(2) );

  init( L, alpha, x, y, z );

  std::cout << " BLAS1  : Vector length    = " << L <<"\n",
  std::cout << " BLAS2&3: Matrix dimension = " << M << " x " << M << "\n",
  std::cout << " - elapsed time - : func mode : ----- Relative Error in binary & long double format ----- \n";

  verify<float>         ( L, alpha, x, y, z );
  verify<df_Real_quasi> ( L, alpha, x, y, z );
  verify<df_Real_sloppy>( L, alpha, x, y, z );
  verify<df_Real>       ( L, alpha, x, y, z );
  verify<double>        ( L, alpha, x, y, z );
  verify<tf_Real_quasi> ( L, alpha, x, y, z );
  verify<tf_Real_sloppy>( L, alpha, x, y, z );
  verify<tf_Real>       ( L, alpha, x, y, z );
  verify<qf_Real_quasi> ( L, alpha, x, y, z );
  verify<qf_Real_sloppy>( L, alpha, x, y, z );
  verify<qf_Real>       ( L, alpha, x, y, z );

  verify<dd_Real_quasi> ( L, alpha, x, y, z );
  verify<dd_Real_sloppy>( L, alpha, x, y, z );
#ifdef  _QD_DD_REAL_H
  verify<dd_real>       ( L, alpha, x, y, z ); // DD native
#endif
  verify<dd_Real>       ( L, alpha, x, y, z );
  verify<td_Real_quasi> ( L, alpha, x, y, z );
  verify<td_Real_sloppy>( L, alpha, x, y, z );
  verify<td_Real>       ( L, alpha, x, y, z );
  verify<qd_Real_quasi> ( L, alpha, x, y, z );
  verify<qd_Real_sloppy>( L, alpha, x, y, z );
#ifdef  _QD_QD_REAL_H
  verify<qd_real>       ( L, alpha, x, y, z ); // QD native
#endif
  verify<qd_Real>       ( L, alpha, x, y, z );

  std::cout << std::endl;

  delete [] x;
  delete [] y;
  delete [] z;

  return 0;
}

