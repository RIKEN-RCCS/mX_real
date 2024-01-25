#include <stdio.h>
#include <omp.h>
#include <iomanip>

#include <qd/dd_real.h>
#include <qd/qd_real.h>
#include "mpreal.h"

//
// if some of {qd/dd_real.h, qd/qd_real.h, mpreal.h} are needed,
// include their headers in advance of the inclusion of mX_real.hpp
//


#include "mX_real.hpp"
using namespace mX_real;

#include "io.hpp"
#include "mpfr_convert.hpp"
using mp_real = mpfr::mpreal;

#if 0
//
// next function helps the detection of typenames, but
// has to be availabel for the only developper purpose.
//
#include <boost/type_index.hpp>
template < typename T > void printTYPE( void ) {
  std::cout << boost::typeindex::type_id_with_cvr<T>().pretty_name(); }
#endif


template < typename T >
T nrm2( int const& L, T const* x ) {
  T z = 0;
#ifdef __INTEL_COMPILER
#pragma ivdep
#else
#pragma omp simd
#endif
  for(int i=0; i<L; i++) {
    z += x[i] * x[i];
  }
  return sqrt(z);
}

template < typename T >
T asum( int const& L, T const* x ) {
  T z = 0;
#ifdef __INTEL_COMPILER
#pragma ivdep
#else
#pragma omp simd
#endif
  for(int i=0; i<L; i++) {
    z += abs( x[i] );
  }
  return z;
}

template < typename T >
T dot( int const& L, T const* x, T const *y ) {
  T z = 0;
#ifdef __INTEL_COMPILER
#pragma ivdep
#else
#pragma omp simd
#endif
  for(int i=0; i<L; i++) {
    z += x[i] * y[i];
  }
  return z;
}

template < typename T >
void axpy( int const& L, T const& alpha, T const* x, T *y ) {
#ifdef __INTEL_COMPILER
#pragma ivdep
#else
#pragma omp simd
#endif
  for(int i=0; i<L; i++) {
    y[i] += alpha * x[i];
  }
}

template < typename T >
void gemv( int const& L, int const& N, T const& alpha, T const *a, T const* x, T *y ) {
  for(int j=0; j<N; j++) {
    auto s = alpha * x[j];
#ifdef __INTEL_COMPILER
#pragma ivdep
#else
#pragma omp simd
#endif
    for(int i=0; i<L; i++) {
      y[i] += a[i+j*L] * s;
    }
  }
}



void
init( int const L, mp_real const& alpha, mp_real * x, mp_real * y, mp_real * z )
{
  unsigned int seed = 1;
  mpfr::random( seed );
  
#ifdef __INTEL_COMPILER
#pragma ivdep
#else
#pragma omp simd
#endif
  for(int i=0; i<L; i++) {
    x[i] = 2*mpfr::random() - 1;
    y[i] = 2*mpfr::random() - 1;
    z[i+8] = y[i];
  }
  axpy( L, alpha, x, z+8 );
  
  int M = (int)sqrt((double)L);
#ifdef __INTEL_COMPILER
#pragma ivdep
#else
#pragma omp simd
#endif
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
  
  {
    auto y = T(0);
    auto x = T(0);
    //while ( true ) {
    for(int i=0; i<=400; i++) {
#if 0
      x = x + T(1);
      auto x2 = x * x;
      auto x4 = x2 * x2;
      auto z = T(1) / x4;
      auto y0= y;
      y = y + z;
      if ( abs(y0-y) == T(0) ) break;
#else
      auto s1 = double(2) / (4*x+double(1));
      auto s2 = double(2) / (4*x+double(2));
      auto s3 = double(1) / (4*x+double(3));
      auto z = s1 + s2 + s3;
      for(int j=0; j<i; j++) z = z * double(-0.25);
      if ( z == T(0) ) break; // uderflow
      y += z;
      x += double(1);
#endif
    }
    auto pi = mpfr::const_pi();
    auto pai = convert<T>( mpfr::const_pi() );
    
    //    auto ans = sqrt( sqrt( y*90 ) );
    auto ans = y;
#if 0
    print( "pai by sqrt(sqrt(90*sum 1/k^4)) ", ans );
    
    auto pai4 = convert<T>( pi*pi*pi*pi );
    
    print( "pi.appx^4                    =  ", y*90 );
    print( "pi[mpfr::const_pi]^4         =  ", pai4 );
    print( "pi[mpfr::const_pi]           =  ", pai );
    print( "pi[mpfr::const_pi] - pi.appx =  ", pai - ans );
#else
    print( "pi.appx                      =  ", ans );
    print( "pi[mpfr::const_pi]           =  ", pai );
    print( "pi[mpfr::const_pi] - pi.appx =  ", pai - ans );
#endif
  }
}


int
main(int argc, char *argv[])
{
  std::cout << std::scientific << std::setprecision(15);
  mp_real::set_default_prec(64*4);
  
  int constexpr L = 1000*1000;
  int const     M = (int)sqrt((double)L);
  
  auto * x = new mp_real[L];
  auto * y = new mp_real[L];
  auto * z = new mp_real[L+8+M];
  
  auto alpha = sqrt( mp_real(2) );
  
  {
    auto cp = df_Real_quasi( 1); cp.x[1] = fp<float>::epsilon/4;
    auto cn = df_Real_quasi(-1); cn.x[1] = fp<float>::epsilon/4;
    print( "Quasi cp=", cp );
    print( "Quasi cn=", cn );
    auto c = cp + cn;
    print( "Quasi c= cp + cn", c );
    auto cx = df_Real( cp ) + df_Real( cn );
    print( "Acc cx = Acc(cp) + Acc(cn)", cx );
  }
  for(int i=1;i<3;i++){
    std::srand(i); print( "rand", df_Real::rand() );
    std::srand(i); print( "rand", tf_Real::rand() );
    std::srand(i); print( "rand", qf_Real::rand() );
    std::srand(i); print( "rand", dd_Real::rand() );
    std::srand(i); print( "rand", td_Real::rand() );
    std::srand(i); print( "rand", qd_Real::rand() );
  }
  {
    print( "eps", std::numeric_limits<double>::epsilon() );
    print( "eps", std::pow( (double)2, -(double)52 ) );
    print( "eps", std::numeric_limits<dd_Real>::epsilon() );
    print( "eps", std::pow( (double)2, -(double)105 ) );
    print( "eps", std::numeric_limits<td_Real>::epsilon() );
    print( "eps", std::pow( (double)2, -(double)158 ) );
    print( "eps", std::numeric_limits<qd_Real>::epsilon() );
    print( "eps", std::pow( (double)2, -(double)211 ) );
    
    print( "eps", std::numeric_limits<float>::epsilon() );
    print( "eps", std::pow( (float)2, -(float)23 ) );
    print( "eps", std::numeric_limits<df_Real>::epsilon() );
    print( "eps", std::pow( (float)2, -(float)47 ) );
    print( "eps", std::numeric_limits<tf_Real>::epsilon() );
    print( "eps", std::pow( (float)2, -(float)71 ) );
    print( "eps", std::numeric_limits<qf_Real>::epsilon() );
    print( "eps", std::pow( (float)2, -(float)95 ) );
  }
  {
    tf_Real r = tf_Real::rand();
    tf_Real x;
    x.x[0] =  9*r.x[0];
    x.x[1] = -9*r.x[0];
    x.x[2] = 1*r.x[2];
    print( "normalize test A0", x );
    QxW::FastTwoSum( x.x[1], x.x[2], x.x[1], x.x[2] );
    QxW::FastTwoSum( x.x[0], x.x[1], x.x[0], x.x[1] );
    print( "normalize test A1", x );
    QxW::FastTwoSum( x.x[1], x.x[2], x.x[1], x.x[2] );
    print( "normalize test A2", x );
    QxW::FastTwoSum( x.x[0], x.x[1], x.x[0], x.x[1] );
    print( "normalize test A3", x );
    
    x.x[0] =  9*r.x[0];
    x.x[1] = -9*r.x[0];
    x.x[2] = 1*r.x[2];
    print( "normalize test B0", x );
    QxW::FastTwoSum( x.x[0], x.x[1], x.x[0], x.x[1] );
    QxW::FastTwoSum( x.x[1], x.x[2], x.x[1], x.x[2] );
    print( "normalize test B1", x );
    QxW::FastTwoSum( x.x[0], x.x[1], x.x[0], x.x[1] );
    print( "normalize test B2", x );
    
    x.x[0] =  9*r.x[0];
    x.x[1] = -9*r.x[0];
    x.x[2] = 1*r.x[2];
    print( "normalize test C0", x );
    QxW::TwoSum( x.x[1], x.x[2], x.x[1], x.x[2] );
    QxW::TwoSum( x.x[0], x.x[1], x.x[0], x.x[1] );
    print( "normalize test C1", x );
    QxW::FastTwoSum( x.x[1], x.x[2], x.x[1], x.x[2] );
    print( "normalize test C2", x );
    QxW::FastTwoSum( x.x[0], x.x[1], x.x[0], x.x[1] );
    print( "normalize test C3", x );
    
    x.x[0] =  9*r.x[0];
    x.x[1] = -9*r.x[0];
    x.x[2] = 1*r.x[2];
    print( "normalize test D0", x );
    QxW::TwoSum( x.x[0], x.x[1], x.x[0], x.x[1] );
    QxW::TwoSum( x.x[1], x.x[2], x.x[1], x.x[2] );
    print( "normalize test D1", x );
    QxW::FastTwoSum( x.x[0], x.x[1], x.x[0], x.x[1] );
    print( "normalize test D2", x );
    
  }
#if 1
  {
    print( "min", std::numeric_limits<float>::min() );
    print( "min", std::numeric_limits<dd_Real>::min() );
    auto s = df_Real::min();
    print( "min", s );
    quickSum(s.x[0], s.x[1]);
    print( "min", s );
    
    print( "min", std::numeric_limits<double>::min() );
    print( "min", std::numeric_limits<dd_Real>::min() );
    auto d = dd_Real::min();
    print( "min", d );
    quickSum(d.x[0],d.x[1]);
    print( "min", d );
  }
  {
    print( "max", std::numeric_limits<float>::max() );
    print( "max", std::numeric_limits<df_Real>::max() );
    auto s = df_Real::max();
    print( "max", s );
    quickSum(s.x[0], s.x[1]);
    print( "max", s );
    
    print( "max", std::numeric_limits<double>::max() );
    print( "max", std::numeric_limits<dd_Real>::max() );
    auto d = dd_Real::max();
    print( "max", d );
    quickSum(d.x[0], d.x[1]);
    print( "max", d );
  }
  {
    auto s = df_Real( 0.1 + 0.1/(1<<23), -0.1/(1<<24) );
    print( "test negative", s );
    quickSum( s.x[0], s.x[1] );
    print( "test negative", s );
    quickSum( s.x[0], s.x[1] );
    print( "test negative", s );
    print( "sqrt f+s", dX_real::sqrt( df_Real_quasi( 0.1f ) + s ) );
    print( "sqrt f+s", dX_real::sqrt( df_Real_sloppy( 0.1f ) + s ) );
    print( "sqrt f+s", dX_real::sqrt( df_Real( 0.1f ) + s ) );
    print( "sqrt f", dX_real::sqrt( 0.1f ) );
    print( "sqrt f", tX_real::sqrt( 0.1f ) );
    print( "sqrt f", qX_real::sqrt( 0.1f ) );
    print( "sqrt s", dX_real::sqrt( s ) );
    print( "sqrt s", tX_real::sqrt( s ) );
    print( "sqrt s", qX_real::sqrt( s ) );
    float a = 0.1;
    float b = 0.3;
    print( "mul ", a * b );
    print( "mulx", dX_real::operator_mul(a,b) );
    print( "add ", a + b );
    print( "addx", dX_real::operator_add(a,b) );
  }
#if 0
  {
    // check ow operators with different Algorithm
    auto a = df_Real::rand();
    auto s = df_Real_sloppy::rand();
    auto q = df_Real_quasi::rand();
    a += a;
    a += s; // error
    a += q; // error
    s += a;
    s += s;
    s += q; // error
    q += a;
    q += s;
    q += q;
    auto A = tf_Real::rand();
    a += A;
    A += a;
  }
#endif
  {
    double ss[] = { 
                   9.3493629504948217e-11,
                   8.9988572149479751e-11,
                   6.0181269689874028e-11,
                   1.1177540837348232e-10,
                   1.4183408614254489e-10,
    };
    for(int i=0; i<5;i++) {
      print( ">>", ss[i] );
      print( ">>", float(ss[i]) );
      mp_real sp = ss[i];
      tf_Real_quasi s0 = convert<tf_Real_quasi>(sp);
      
      print( ">>", s0 );
      tf_Real_quasi s1 = 1 / sqrt(s0);
      tf_Real_quasi s2 = 1 / s1;
      tf_Real_quasi s3 = s2 * s2;
      print( ">>", s3 );
      
      tf_Real_sloppy t0 = s0;
      
      print( ">>", t0 );
      tf_Real_sloppy t1 = 1 / sqrt(t0);
      tf_Real_sloppy t2 = 1 / t1;
      tf_Real_sloppy t3 = t2 * t2;
      print( ">>", t3 );
      
      tf_Real c0 = s0;
      
      print( ">>", c0 );
      tf_Real c1 = 1 / sqrt(c0);
      tf_Real c2 = 1 / c1;
      tf_Real c3 = c2 * c2;
      print( ">>", c3 );
    }
  }
  {
    float x = std::numeric_limits<float>::quiet_NaN();
    float y = -x;
    std::cout << "nan=" << x << "\n" << "-nan=" << y << "\n";
    std::cout << (x == y) << " " << (-x == y) << " " << ( x == -y) << "\n";
  }
  {
    float x = std::numeric_limits<float>::infinity();
    float y = -x;
    std::cout << "inf=" << x << "\n" << "-inf=" << y << "\n";
    std::cout << (x == y) << " " << (-x == y) << " " << ( x == -y) << "\n";
    std::cout << x + x << " " << x - x << " " << x + y << "\n";
    std::cout << y + y << " " << y - y << " " << y + x << "\n";
  }
  {
    float x = +0;
    float y = -x;
    std::cout << "+0=" << x << "\n" << "-0=" << y << "\n";
    std::cout << (x == y) << " " << (-x == y) << " " << ( x == -y) << "\n";
  }
  
  {
    float e = 1;
    print( "one1=", e );
    e = e - std::numeric_limits<float>::epsilon()/2;
    print( "one1-u1=", e );
    
    df_Real f = df_Real( e,e*std::numeric_limits<float>::epsilon()/2 );;
    print( "(one1-u1,(one1-u1)*u1)=", f );
    
    df_Real c = df_Real(1);
    print( "one2=", c );
    c = c - df_Real::epsilon() * df_Real::half();
    print( "one2-u2=", c );
    
    c = c - f;
    print( "(one2-u2)-(one1-u1,(one1-u1)*u1)=", c );
    
    c = f + df_Real::epsilon() * df_Real::half();
    print( "one2-u2=", c );
  }
  
  init( L, alpha, x, y, z );
  
  std::cout << " BLAS1  : Vector length    = " << L <<"\n",
    std::cout << " BLAS2&3: Matrix dimension = " << M << " x " << M << "\n",
    std::cout << " - elapsed time - : func mode : ----- Relative Error in binary & long double format ----- \n";
  
#if 1
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
#endif
  verify<qd_Real>       ( L, alpha, x, y, z );
  
  std::cout << std::endl;
  
#endif
  
  delete [] x;
  delete [] y;
  delete [] z;
  
  return 0;
}

