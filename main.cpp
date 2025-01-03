#include <stdio.h>
#include <omp.h>
#include <iomanip>

//
// if some of {qd/dd_real.h, qd/qd_real.h, mpreal.h} are needed,
// include their headers in advance of the inclusion of mX_real.hpp
//

#include "mX_real.hpp"
#if USE_QDREAL || USE_DDREAL
#include "mX_real_qd_dd.hpp"
#endif
#include "mX_real_mpreal.hpp"
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
template < typename T > auto getTYPE( void ) {
  return boost::typeindex::type_id_with_cvr<T>().pretty_name(); }
template < typename T > void printTYPE( void ) {
  std::cout << getTYPE<T>(); }
#endif


template < template < class... > class TestFunc, class... Tlist_base >
struct For {
private:
  // inner loop
  template < typename dummy> static void run0_impl () { }
  template < typename dummy, typename T0, class... Tlist >
  static void run0_impl () {
    TestFunc < T0 >:: run ();
    if ( sizeof...(Tlist) ) run0_impl < dummy, Tlist... > ();
  }
public:
  static void run () { run0_impl < void, Tlist_base... > (); }
};

template < template < class... > class TestFunc, class... Tlist_base >
struct ForFor {
private:
  // inner loop
  template < typename T0> static void run1_impl () { }
  template < typename T0, typename T1, class... Tlist >
  static void run1_impl () {
    TestFunc < T0, T1 >:: run ();
    if ( sizeof...(Tlist) ) run1_impl < T0, Tlist... > ();
  };

  // outer loop
  template < typename dummy > static void run0_impl () { }
  template < typename dummy, typename T0, class... Tlist >
  static void run0_impl () {
    run1_impl < T0, Tlist_base... > ();
    if ( sizeof...(Tlist) ) run0_impl < dummy, Tlist... > ();
  }
public:
  static void run () { run0_impl < void, Tlist_base... > (); }
};

template < typename TX, T_mX(TX) >
TX random_number() {
  using T = typename TX::base_T;
  using TX_accurate = typename TX::typeAccurate;
  auto r = TX{ 2*TX_accurate::rand() -TX_accurate::one() };
  auto f = std::pow(2.0,double(rand()%60)-30.0);
  return r * f;
}

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
  return std::sqrt(z);
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
    z += std::fabs( x[i] );
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
  

  char s[64]; TYPE_name<T>( s );
  std::cout << std::left
            << std::setw(10)
	    << "Type = "
            << std::string(s) << "\n";

  auto o_prec = std::cout.precision();
  std::cout << std::scientific
            << std::setprecision(32);

  {
    auto * w = new T[ITR];
    
    double ts = omp_get_wtime();
    for(int itr=0; itr<ITR; itr++) {
      w[itr] = dot( L, x, y );
    }
    double te = omp_get_wtime();
    
    printf("  %le[s] : ",(te-ts)/ITR);
    auto ans = abs( ( convert(w[0]) - Z[0] ) / Z[0] );
    std::cout << std::setw(5) << "dot  ";
    std::cout << std::setw(40) << ans << " " << abs(w[0]) << "\n";
    
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
    auto ans = abs( ( convert(w[0]) - Z[1] ) / Z[1] );
    std::cout << std::setw(5) << "nrm2 ";
    std::cout << std::setw(40) << ans << " " << w[0] << "\n";
    
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
    auto ans = abs( ( convert(w[0]) - Z[2] ) / Z[2] );
    std::cout << std::setw(5) << "asum ";
    std::cout << std::setw(40) << ans << " " << w[0] << "\n";
    
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
    auto ans = r/Z[3];
    std::cout << std::setw(5) << "axpy ";
    std::cout << std::setw(40) << ans << " " << r << "\n";
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
    auto ans = r/Z[4];
    std::cout << std::setw(5) << "gemv ";
    std::cout << std::setw(40) << ans << " " << r << "\n";
    
    delete [] w;
  }
  
  delete [] x;
  delete [] y;
  
  {
    auto const ZERO = zero<T>();
    auto const ONE  = one<T>();
    auto const TWO  = two<T>();

    auto y = ZERO;
    auto s1 = 1;
    auto s2 = 2;
    auto s3 = 3;
    double f = 1.0;

    while ( true ) {
      if ( f == 0 ) break;

      auto z = ZERO;
      int h = 1;
      for(int i=0; i<9; i++) {

        z += ( ( TWO / s1 + TWO / s2 + ONE / s3 ) / h );

        s1 += 4;
        s2 += 4;
        s3 += 4;
        h *= (-4);
      }
      y += z * f;

      f /= h;
    }
    auto ans = convert( y );
    std::cout << "pi.appx                    " << ans << "\n";
    auto pi = mpfr::const_pi();
    std::cout << "pi[mpfr::const_pi]         " << pi << "\n";
    ans -= pi;
    std::cout << "pi[mpfr::const_pi]-pi.appx " << ans << "\n";
  }

  std::cout << std::setprecision( o_prec );
}


template < typename TXa, typename TXb >
mp_real get_tol() {
  using TXc = largeType<TXa,TXb>;
  using TXd = smallType<TXa,TXb>;
  Algorithm A = commonAlgorithm<TXa::base_A,TXb::base_A>::algorithm;
  int ext = 4 * (TXd::L + (A!=Algorithm::Accurate)) * (TXc::L - TXd::L + 1) + 2;
  auto tol  = convert( std::ldexp( 1.0, ext ) );
  return tol;
}

template < typename TXa >
struct Test_sizeof
{
  template < T_mX(TXa) >
  static void run()
  {
    print( "sizeof", TXa(sizeof(TXa)) );
  }
};

template < typename TXa, typename TXb >
struct Test_add
{
  template < T_mX(TXa), T_mX(TXb), T_assert( std::is_same<typename TXa::base_T, typename TXb::base_T>::value ) >
  static void run()
  {
  using T = typename TXa::base_T;
  Algorithm const A = commonAlgorithm<TXa::base_A,TXb::base_A>::algorithm;
  using TX = typename largeType<TXa,TXb>::template type_with_Algorithm<A>;
  auto eps = convert( TX::epsilon() );
  auto tol = get_tol<TXa,TXb>();

  unsigned int seed = 1;
  srand( seed );

  auto res = -eps;
  for(int i=1;i<10000;i++){
    auto a = random_number<TXa>();
    auto b = random_number<TXb>();
    auto c = a + b;

    auto a_ = convert( a );
    auto b_ = convert( b );
    auto c_ = a_ + b_;

    auto e_ = fabs( c_ - convert( c ) );
    auto f_ = fabs( c_ ) * eps;
    auto g_ = e_/f_;

    res = fmax( res, g_ );
  }

  char sa[64]; TYPE_name<TXa>( sa );
  char sb[64]; TYPE_name<TXb>( sb );
  char sc[64]; TYPE_name<TX >( sc );
  std::cout << ( res <= tol ? "[ PASS  ]" : "[ FALSE ]" )
          << " : ADD " << std::string( sa ) << " + " << std::string( sb ) << " => " << std::string( sc )
	  << " :: Res=" << res << " Eps=" << eps << " Tol=" << tol << " "
	  << std::numeric_limits<T>::max_digits10 << "\n";
  }
};

template < typename TXa, typename TXb >
struct Test_mul
{
  template < T_mX(TXa), T_mX(TXb), T_assert( std::is_same<typename TXa::base_T, typename TXb::base_T>::value ) >
  static void run()
  {
  using T = typename TXa::base_T;
  Algorithm const A = commonAlgorithm<TXa::base_A,TXb::base_A>::algorithm;
  using TX = typename largeType<TXa,TXb>::template type_with_Algorithm<A>;
  auto eps = convert( TX::epsilon() );
  auto tol = get_tol<TXa,TXb>();

  unsigned int seed = 1;
  srand( seed );

  auto res = -eps;
  for(int i=1;i<10000;i++){
    auto a = random_number<TXa>();
    auto b = random_number<TXb>();
    auto c = a * b;

    auto a_ = convert( a );
    auto b_ = convert( b );
    auto c_ = a_ * b_;

    auto e_ = fabs( c_ - convert( c ) );
    auto f_ = fabs( c_ ) * eps;
    auto g_ = e_/f_;

    res = fmax( res, g_ );
  }

  char sa[64]; TYPE_name<TXa>( sa );
  char sb[64]; TYPE_name<TXb>( sb );
  char sc[64]; TYPE_name<TX >( sc );
  std::cout << ( res <= tol ? "[ PASS  ]" : "[ FALSE ]" )
          << " : MUL " << std::string( sa ) << " * " << std::string( sb ) << " => " << std::string( sc )
	  << " :: Res=" << res << " Eps=" << eps << " Tol=" << tol << " "
	  << std::numeric_limits<T>::max_digits10 << "\n";
  }
};

template < typename TXa, typename TXb >
struct Test_div
{
  template < T_mX(TXa), T_mX(TXb), T_assert( std::is_same<typename TXa::base_T, typename TXb::base_T>::value ) >
  static void run()
  {
  using T = typename TXa::base_T;
  Algorithm const A = commonAlgorithm<TXa::base_A,TXb::base_A>::algorithm;
  using TX = typename largeType<TXa,TXb>::template type_with_Algorithm<A>;
  auto eps = convert( TX::epsilon() );
  auto tol = get_tol<TXa,TXb>(); tol = tol*tol;

  unsigned int seed = 1;
  srand( seed );

  auto res = -eps;
  for(int i=1;i<10000;i++){
    auto a = random_number<TXa>();
    auto b = random_number<TXb>();
    auto c = a / b;

    auto a_ = convert( a );
    auto b_ = convert( b );
    auto c_ = a_ / b_;

    auto e_ = fabs( c_ - convert( c ) );
    auto f_ = fabs( c_ ) * eps;
    auto g_ = e_/f_;

    res = fmax( res, g_ );
  }

  char sa[64]; TYPE_name<TXa>( sa );
  char sb[64]; TYPE_name<TXb>( sb );
  char sc[64]; TYPE_name<TX >( sc );
  std::cout << ( res <= tol ? "[ PASS  ]" : "[ FALSE ]" )
          << " : DIV " << std::string( sa ) << " / " << std::string( sb ) << " => " << std::string( sc )
	  << " :: Res=" << res << " Eps=" << eps << " Tol=" << tol << " "
	  << std::numeric_limits<T>::max_digits10 << "\n";
  }
};

template < typename TXa >
struct Test_sqr
{
  template < T_mX(TXa) >
  static void run()
  {
  using T = typename TXa::base_T;
  using TX = TXa;
  auto eps = convert( TX::epsilon() );
  auto tol = get_tol<TXa,TXa>();

  unsigned int seed = 1;
  srand( seed );

  auto res = -eps;
  for(int i=1;i<10000;i++){
    auto a = random_number<TXa>();
    auto c = a * a;

    auto a_ = convert( a );
    auto c_ = a_ * a_;

    auto e_ = fabs( c_ - convert( c ) );
    auto f_ = fabs( c_ ) * eps;
    auto g_ = e_/f_;

    if ( fabs(a_) > convert( TXa::safe_min() / 4 ) ) {
      res = fmax( res, g_ );
    }
  }

  char sa[64]; TYPE_name<TXa>( sa );
  char sc[64]; TYPE_name<TX >( sc );
  std::cout << ( res <= tol ? "[ PASS  ]" : "[ FALSE ]" )
	  << " : SQR  " << std::string( sa ) << " => " << std::string( sc )
	  << " :: Res=" << res << " Eps=" << eps << " Tol=" << tol << " "
	  << std::numeric_limits<T>::max_digits10 << "\n";
  }
};

template < typename TXa >
struct Test_sqrt
{
  template < T_mX(TXa) >
  static void run()
  {
  using T = typename TXa::base_T;
  using TX = TXa;
  auto eps = convert( TX::epsilon() );
  auto tol = get_tol<TXa,TXa>();

  unsigned int seed = 1;
  srand( seed );

  auto res = -eps;
  for(int i=1;i<10000;i++){
    auto a = fabs( random_number<TXa>() );
    auto c = sqrt( a );

    auto a_ = convert( a );
    auto c_ = sqrt( a_ );

    auto e_ = fabs( c_ - convert( c ) );
    auto f_ = fabs( c_ ) * eps;
    auto g_ = e_/f_;

    res = fmax( res, g_ );
  }
  char sa[64]; TYPE_name<TXa>( sa );
  char sc[64]; TYPE_name<TX >( sc );
  std::cout << ( res <= tol ? "[ PASS  ]" : "[ FALSE ]" )
	  << " : SQRT " << std::string( sa ) << " => " << std::string( sc )
	  << " :: Res=" << res << " Eps=" << eps << " Tol=" << tol << " "
	  << std::numeric_limits<T>::max_digits10 << "\n";
  }
};

template < typename TXa >
struct Test_NaN
{
  template < T_mX(TXa) >
  static void run()
  {
    print( "nan=", TXa::nan() );
  }
};

template < typename TXa >
struct Test_inf
{
  template < T_mX(TXa) >
  static void run()
  {
    print( "inf=", TXa::inf() );
  }
};

template < typename TXa >
struct Test_epsilon
{
  template < T_mX(TXa) >
  static void run()
  {
    print( "epsilon=", TXa::epsilon() );
  }
};

template < typename TXa >
struct Test_denorm_min
{
  template < T_mX(TXa) >
  static void run()
  {
    print( "denorm_min=", TXa::denorm_min() );
  }
};

template < typename TXa >
struct Test_min
{
  template < T_mX(TXa) >
  static void run()
  {
    print( "min=", TXa::min() );
  }
};

template < typename TXa >
struct Test_maxmax
{
  template < T_mX(TXa) >
  static void run()
  {
    print( "maxmax=", TXa::maxmax() );
  }
};

template < typename TXa >
struct Test_max
{
  template < T_mX(TXa) >
  static void run()
  {
    print( "max=", TXa::max() );
  }
};

template < typename TXa >
struct Test_safe_min
{
  template < T_mX(TXa) >
  static void run()
  {
    print( "safe_min=", TXa::safe_min() );
  }
};

template < typename TXa >
struct Test_safe_max
{
  template < T_mX(TXa) >
  static void run()
  {
    print( "safe_max=", TXa::safe_max() );
  }
};

template < typename TXa >
struct Test_rand
{
  template < T_mX(TXa) >
  static void run()
  {
    for(int i=1;i<=5;i++){
      std::srand(i);
      print( "rand=", TXa::rand() );
    }
  }
};

template < typename TXa >
struct Test_Quasi_Accurate
{
  template < T_mX(TXa) >
  static void run()
  {
    using T = typename TXa::base_T;
    using TX_quasi = typename TXa::typeAccurate;
    using TX_accurate = typename TXa::typeQuasi;

    auto cp = TX_quasi( 1024);
    auto cn = TX_quasi(-1024);
    auto s = fp<T>::epsilon()/4;
    for(int i=1; i<TXa::L; i++) {
    	cn.x[i] = cp.x[i] = s;
	s *= fp<T>::epsilon()/2;
    }
    print( "Quasi cp=", cp );
    print( "Quasi cn=", cn );
    auto c = cp + cn;
    print( "Quasi c= cp + cn", c );
    auto xp = TX_accurate( cp );
    auto xn = TX_accurate( cn );
    auto x = xp + xn;
    print( "Acc cp=", xp );
    print( "Acc cn=", xn );
    print( "Acc cx = Acc(cp) + Acc(cn)", x );
    print( "Acc(cp+cn) =", TX_accurate(cp+cn) );
    c = cp+cn;
    Normalize<NormalizeOption::VsumForward>( c );
    print( "Normalize(c) =", c );
  }
};

template < typename TXa >
struct Test_Normalize
{
  template < T_mX(TXa) >
  static void run()
  {
    using T = typename TXa::base_T;
    TXa r;
    {
      TXa r0 = TXa::rand();
      TXa r1 = TXa::rand();
      for(int i=1; i<TXa::L; i++) { r0.x[i] -= r1.x[TXa::L-1-i]*4096; }
      r0.x[TXa::L-1] *= TXa::epsilon().x[0];
      r = r0;
      print( "normalize test", r );
    }
    {
    	NormalizeOption constexpr Nopt = NormalizeOption::Regular;
    	auto x = r; Normalize<Nopt>( x );
    	print( "normalize"+toString(Nopt), x );
    }
    {
    	NormalizeOption constexpr Nopt = NormalizeOption::Accurate;
    	auto x = r; Normalize<Nopt>(x);
    	print( "normalize"+toString(Nopt), x );
    }
    {
    	NormalizeOption constexpr Nopt = NormalizeOption::Accurate;
    	auto x = r; Normalize<Nopt>(x); Normalize<Nopt>(x);
    	print( "normalize^2"+toString(Nopt), x );
    }
    {
    	NormalizeOption constexpr Nopt = NormalizeOption::Accurate;
    	auto x = r; Normalize<Nopt>(x); Normalize<Nopt>(x); Normalize<Nopt>(x);
    	print( "normalize^3"+toString(Nopt), x );
    }
    {
    	NormalizeOption constexpr Nopt = NormalizeOption::VsumForward;
    	auto x = r; Normalize<Nopt>(x);
    	print( "normalize(Maybe NG)"+toString(Nopt), x );
    }
    {
    	NormalizeOption constexpr Nopt = NormalizeOption::VQsumForward;
    	auto x = r; Normalize<Nopt>(x);
    	print( "normalize(Maybe NG)"+toString(Nopt), x );
    }
    {
    	NormalizeOption constexpr Nopt = NormalizeOption::VsumReverse;
    	auto x = r; Normalize<Nopt>(x);
    	print( "normalize(Maybe NG)"+toString(Nopt), x );
    }
    {
    	NormalizeOption constexpr Nopt = NormalizeOption::VQsumReverse;
    	auto x = r; Normalize<Nopt>(x);
    	print( "normalize(Maybe NG)"+toString(Nopt), x );
    }
  }
};

template < typename TXa >
struct Test_min_Normalize
{
  template < T_mX(TXa) >
  static void run()
  {
    using T = typename TXa::base_T;
    TXa r = TXa::min();
    print( "min=", r );
    Normalize(r);
    print( "Normaize(min)=", r );
  }
};

template < typename TXa >
struct Test_max_Normalize
{
  template < T_mX(TXa) >
  static void run()
  {
    using T = typename TXa::base_T;
    TXa r = TXa::max();
    print( "max=", r );
    Normalize(r);
    print( "Normaize(max)=", r );
  }
};

template < typename TXa >
struct Test_sqrt_tiny
{
  template < T_mX(TXa) >
  static void run()
  {
    using T = typename TXa::base_T;
    {
      auto s = TXa::rand();
      print( "rand", s );
      for(int i=0;i<TXa::L;i++) {
      	s.x[i] *= fp<T>::min() * 4096;
      }
      auto r = fp<T>::denorm_min();
      auto f = fp<T>::epsilon(); f = sqrt(sqrt(f));
      for(int i=TXa::L-1;i>=0;i--) {
        s.x[TXa::L-1]+=r; r/=f;
      }
      if ( TXa::base_A != Algorithm::Quasi ) { Normalize( s ); }
      auto q = sqrt( s );
      print( "<min", s );
      print( "sqrt <min", q );
      print( "sqr(sqrt <min)", q*q );
      print( "|s-q*q|",s-q*q );
    }
  }
};



int
main(int argc, char *argv[])
{
  std::cout << std::scientific << std::setprecision(15);
  mp_real::set_default_prec(64*4);
  
  print( "one=", QxW::fp_const<double>::one() );
  print( "zero=", QxW::fp_const<double>::zero() );
  print( "fmin=", std::numeric_limits<double>::min() );
  print( "xone=", std::ldexp(std::numeric_limits<double>::min(),52) );
  print( "pw52=", std::ldexp((double)1,+52) );
  print( "p-52=", std::ldexp((double)1,-52) );
  printf( "PWRP=\t%016lx\n", QxW::fp_const<double>::PWRP );
  printf( "PWRN=\t%016lx\n", QxW::fp_const<double>::PWRN );
  print( "connect=", fp<double>::connect_fp() );
  print( "disconnect=", fp<double>::disconnect_fp() );

  print( "one=", QxW::fp_const<float>::one() );
  print( "zero=", QxW::fp_const<float>::zero() );
  print( "fmin=", std::numeric_limits<float>::min() );
  print( "xone=", std::ldexp(std::numeric_limits<float>::min(),23) );
  print( "pw23=", std::ldexp((float)1,+23) );
  print( "p-23=", std::ldexp((float)1,-23) );
  printf( "PWRP=\t%08x\n", QxW::fp_const<float>::PWRP );
  printf( "PWRN=\t%08x\n", QxW::fp_const<float>::PWRN );
  print( "connect=", fp<float>::connect_fp() );
  print( "disconnect=", fp<float>::disconnect_fp() );

  int constexpr L = 1000*1000;
  int const     M = (int)sqrt((double)L);
  
  auto * x = new mp_real[L];
  auto * y = new mp_real[L];
  auto * z = new mp_real[L+8+M];
  
  auto alpha = sqrt( mp_real(2) );
  
  For    < Test_sizeof,
	 df_Real_quasi, df_Real_sloppy, df_Real_weakaccurate, df_Real,
	 tf_Real_quasi, tf_Real_sloppy, tf_Real,
	 qf_Real_quasi, qf_Real_sloppy, qf_Real > :: run();

  For    < Test_sizeof,
	 dd_Real_quasi, dd_Real_sloppy, dd_Real_weakaccurate, dd_Real,
	 td_Real_quasi, td_Real_sloppy, td_Real,
	 qd_Real_quasi, qd_Real_sloppy, qd_Real > :: run();

  ForFor < Test_add,
	 df_Real_quasi, df_Real_sloppy, df_Real_weakaccurate, df_Real,
	 tf_Real_quasi, tf_Real_sloppy, tf_Real,
	 qf_Real_quasi, qf_Real_sloppy, qf_Real > :: run();

  ForFor < Test_add,
	 dd_Real_quasi, dd_Real_sloppy, dd_Real_weakaccurate, dd_Real,
	 td_Real_quasi, td_Real_sloppy, td_Real,
	 qd_Real_quasi, qd_Real_sloppy, qd_Real > :: run();

  ForFor < Test_mul,
	 df_Real_quasi, df_Real_sloppy, df_Real_weakaccurate, df_Real,
	 tf_Real_quasi, tf_Real_sloppy, tf_Real,
	 qf_Real_quasi, qf_Real_sloppy, qf_Real > :: run();

  ForFor < Test_mul,
	 dd_Real_quasi, dd_Real_sloppy, dd_Real_weakaccurate, dd_Real,
	 td_Real_quasi, td_Real_sloppy, td_Real,
	 qd_Real_quasi, qd_Real_sloppy, qd_Real > :: run();

  ForFor < Test_div,
	 df_Real_quasi, df_Real_sloppy, df_Real_weakaccurate, df_Real,
	 tf_Real_quasi, tf_Real_sloppy, tf_Real,
	 qf_Real_quasi, qf_Real_sloppy, qf_Real > :: run();

  ForFor < Test_div,
	 dd_Real_quasi, dd_Real_sloppy, dd_Real_weakaccurate, dd_Real,
	 td_Real_quasi, td_Real_sloppy, td_Real,
	 qd_Real_quasi, qd_Real_sloppy, qd_Real > :: run();

  For    < Test_sqr,
	 df_Real_quasi, df_Real_sloppy, df_Real_weakaccurate, df_Real,
	 tf_Real_quasi, tf_Real_sloppy, tf_Real,
	 qf_Real_quasi, qf_Real_sloppy, qf_Real > :: run();

  For    < Test_sqr,
	 dd_Real_quasi, dd_Real_sloppy, dd_Real_weakaccurate, dd_Real,
	 td_Real_quasi, td_Real_sloppy, td_Real,
	 qd_Real_quasi, qd_Real_sloppy, qd_Real > :: run();

  For    < Test_sqrt,
	 df_Real_quasi, df_Real_sloppy, df_Real_weakaccurate, df_Real,
	 tf_Real_quasi, tf_Real_sloppy, tf_Real,
	 qf_Real_quasi, qf_Real_sloppy, qf_Real > :: run();

  For    < Test_sqrt,
	 dd_Real_quasi, dd_Real_sloppy, dd_Real_weakaccurate, dd_Real,
	 td_Real_quasi, td_Real_sloppy, td_Real,
	 qd_Real_quasi, qd_Real_sloppy, qd_Real > :: run();

  For    < Test_NaN,
	 df_Real_quasi, df_Real_sloppy, df_Real_weakaccurate, df_Real,
	 tf_Real_quasi, tf_Real_sloppy, tf_Real,
	 qf_Real_quasi, qf_Real_sloppy, qf_Real > :: run();

  For    < Test_NaN,
	 dd_Real_quasi, dd_Real_sloppy, dd_Real_weakaccurate, dd_Real,
	 td_Real_quasi, td_Real_sloppy, td_Real,
	 qd_Real_quasi, qd_Real_sloppy, qd_Real > :: run();

  For    < Test_inf,
	 df_Real_quasi, df_Real_sloppy, df_Real_weakaccurate, df_Real,
	 tf_Real_quasi, tf_Real_sloppy, tf_Real,
	 qf_Real_quasi, qf_Real_sloppy, qf_Real > :: run();

  For    < Test_inf,
	 dd_Real_quasi, dd_Real_sloppy, dd_Real_weakaccurate, dd_Real,
	 td_Real_quasi, td_Real_sloppy, td_Real,
	 qd_Real_quasi, qd_Real_sloppy, qd_Real > :: run();

  For    < Test_epsilon,
	 df_Real_quasi, df_Real_sloppy, df_Real_weakaccurate, df_Real,
	 tf_Real_quasi, tf_Real_sloppy, tf_Real,
	 qf_Real_quasi, qf_Real_sloppy, qf_Real > :: run();

  For    < Test_epsilon,
	 dd_Real_quasi, dd_Real_sloppy, dd_Real_weakaccurate, dd_Real,
	 td_Real_quasi, td_Real_sloppy, td_Real,
	 qd_Real_quasi, qd_Real_sloppy, qd_Real > :: run();

  For    < Test_denorm_min,
	 df_Real_quasi, df_Real_sloppy, df_Real_weakaccurate, df_Real,
	 tf_Real_quasi, tf_Real_sloppy, tf_Real,
	 qf_Real_quasi, qf_Real_sloppy, qf_Real > :: run();

  For    < Test_denorm_min,
	 dd_Real_quasi, dd_Real_sloppy, dd_Real_weakaccurate, dd_Real,
	 td_Real_quasi, td_Real_sloppy, td_Real,
	 qd_Real_quasi, qd_Real_sloppy, qd_Real > :: run();

  For    < Test_min,
	 df_Real_quasi, df_Real_sloppy, df_Real_weakaccurate, df_Real,
	 tf_Real_quasi, tf_Real_sloppy, tf_Real,
	 qf_Real_quasi, qf_Real_sloppy, qf_Real > :: run();

  For    < Test_min,
	 dd_Real_quasi, dd_Real_sloppy, dd_Real_weakaccurate, dd_Real,
	 td_Real_quasi, td_Real_sloppy, td_Real,
	 qd_Real_quasi, qd_Real_sloppy, qd_Real > :: run();

  For    < Test_maxmax,
	 df_Real_quasi, df_Real_sloppy, df_Real_weakaccurate, df_Real,
	 tf_Real_quasi, tf_Real_sloppy, tf_Real,
	 qf_Real_quasi, qf_Real_sloppy, qf_Real > :: run();

  For    < Test_maxmax,
	 dd_Real_quasi, dd_Real_sloppy, dd_Real_weakaccurate, dd_Real,
	 td_Real_quasi, td_Real_sloppy, td_Real,
	 qd_Real_quasi, qd_Real_sloppy, qd_Real > :: run();

  For    < Test_max,
	 df_Real_quasi, df_Real_sloppy, df_Real_weakaccurate, df_Real,
	 tf_Real_quasi, tf_Real_sloppy, tf_Real,
	 qf_Real_quasi, qf_Real_sloppy, qf_Real > :: run();

  For    < Test_max,
	 dd_Real_quasi, dd_Real_sloppy, dd_Real_weakaccurate, dd_Real,
	 td_Real_quasi, td_Real_sloppy, td_Real,
	 qd_Real_quasi, qd_Real_sloppy, qd_Real > :: run();

  For    < Test_safe_min,
	 df_Real_quasi, df_Real_sloppy, df_Real_weakaccurate, df_Real,
	 tf_Real_quasi, tf_Real_sloppy, tf_Real,
	 qf_Real_quasi, qf_Real_sloppy, qf_Real > :: run();

  For    < Test_safe_min,
	 dd_Real_quasi, dd_Real_sloppy, dd_Real_weakaccurate, dd_Real,
	 td_Real_quasi, td_Real_sloppy, td_Real,
	 qd_Real_quasi, qd_Real_sloppy, qd_Real > :: run();

  For    < Test_safe_max,
	 df_Real_quasi, df_Real_sloppy, df_Real_weakaccurate, df_Real,
	 tf_Real_quasi, tf_Real_sloppy, tf_Real,
	 qf_Real_quasi, qf_Real_sloppy, qf_Real > :: run();

  For    < Test_safe_max,
	 dd_Real_quasi, dd_Real_sloppy, dd_Real_weakaccurate, dd_Real,
	 td_Real_quasi, td_Real_sloppy, td_Real,
	 qd_Real_quasi, qd_Real_sloppy, qd_Real > :: run();

  For    < Test_rand,
	 df_Real_quasi, df_Real_sloppy, df_Real_weakaccurate, df_Real,
	 tf_Real_quasi, tf_Real_sloppy, tf_Real,
	 qf_Real_quasi, qf_Real_sloppy, qf_Real > :: run();

  For    < Test_rand,
	 dd_Real_quasi, dd_Real_sloppy, dd_Real_weakaccurate, dd_Real,
	 td_Real_quasi, td_Real_sloppy, td_Real,
	 qd_Real_quasi, qd_Real_sloppy, qd_Real > :: run();

  For    < Test_Quasi_Accurate,
	 df_Real, tf_Real, qf_Real > :: run();

  For    < Test_Quasi_Accurate,
	 dd_Real, td_Real, qd_Real > :: run();

  For    < Test_Normalize,
	 dd_Real_quasi, td_Real_quasi, qd_Real_quasi > :: run();

  For    < Test_Normalize,
	 df_Real_quasi, tf_Real_quasi, qf_Real_quasi > :: run();

  For    < Test_min_Normalize,
	 dd_Real_quasi, td_Real_quasi, qd_Real_quasi > :: run();

  For    < Test_min_Normalize,
	 df_Real_quasi, tf_Real_quasi, qf_Real_quasi > :: run();

  For    < Test_max_Normalize,
	 dd_Real_quasi, td_Real_quasi, qd_Real_quasi > :: run();

  For    < Test_max_Normalize,
	 df_Real_quasi, tf_Real_quasi, qf_Real_quasi > :: run();

  For    < Test_sqrt_tiny,
	 df_Real_quasi, df_Real_sloppy, df_Real_weakaccurate, df_Real,
	 tf_Real_quasi, tf_Real_sloppy, tf_Real,
	 qf_Real_quasi, qf_Real_sloppy, qf_Real > :: run();

  For    < Test_sqrt_tiny,
	 dd_Real_quasi, dd_Real_sloppy, dd_Real_weakaccurate, dd_Real,
	 td_Real_quasi, td_Real_sloppy, td_Real,
	 qd_Real_quasi, qd_Real_sloppy, qd_Real > :: run();

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
    float x = std::numeric_limits<float>::infinity();
    float y = -x;
    float z = 0.;
    float w = -z;

    std::cout << "(+inf)=" << x << "\n";
    std::cout << "(-inf)=" << y << "\n";
    std::cout << "(+0)=" << z << "\n";
    std::cout << "(-0)=" << w << "\n";

    std::cout << "(+inf) == (+inf) : " << (x == y) << "\n";
    std::cout << "-(+inf) == (-inf) : " << (-x == y) << "\n";
    std::cout << "(+inf) == -(-inf) : " << ( x == -y) << "\n";
    std::cout << "(+0) == (+0) : " << (z == w) << "\n";
    std::cout << "-(+0) == (-0) : " << (-z == w) << "\n";
    std::cout << "(+0) == -(-0) : " << ( z == -w) << "\n";

    std::cout << "(+inf) + (+inf) = " << x + x << "\n";
    std::cout << "(+inf) - (+inf) = " << x - x << "\n";
    std::cout << "(+inf) + (-inf) = " << x + y << "\n";
    std::cout << "(-inf) + (-inf) = " << y + y << "\n";
    std::cout << "(-inf) - (-inf) = " << y - y << "\n";
    std::cout << "(-inf) + (+inf) = " << y + x << "\n";

    std::cout << "(+inf) * (+inf) = " << x * x << "\n";
    std::cout << "(+inf) * (-inf) = " << x * y << "\n";
    std::cout << "(-inf) * (+inf) = " << y * x << "\n";
    std::cout << "(-inf) * (-inf) = " << y * y << "\n";
    std::cout << "(+inf) * (+0) = " << x * z << "\n";
    std::cout << "(+inf) * (-0) = " << x * w << "\n";
    std::cout << "(-inf) * (+0) = " << y * z << "\n";
    std::cout << "(-inf) * (-0) = " << y * w << "\n";

    std::cout << "(+inf) / (+inf) = " << x / x << "\n";
    std::cout << "(+inf) / (-inf) = " << x / y << "\n";
    std::cout << "(-inf) / (+inf) = " << y / x << "\n";
    std::cout << "(-inf) / (-inf) = " << y / y << "\n";
    std::cout << "(+0) / (+inf) = " << z / x << "\n";
    std::cout << "(+0) / (-inf) = " << z / y << "\n";
    std::cout << "(-0) / (+inf) = " << w / x << "\n";
    std::cout << "(-0) / (-inf) = " << w / y << "\n";
    std::cout << "(+inf) / (+0) = " << x / z << "\n";
    std::cout << "(+inf) / (-0) = " << x / w << "\n";
    std::cout << "(-inf) / (+0) = " << y / z << "\n";
    std::cout << "(-inf) / (-0) = " << y / w << "\n";
    std::cout << "(+0) / (+0) = " << z / z << "\n";
    std::cout << "(+0) / (-0) = " << z / w << "\n";
    std::cout << "(-0) / (+0) = " << w / z << "\n";
    std::cout << "(-0) / (-0) = " << w / w << "\n";

    std::cout << "abs(+inf) = " << fabs( x ) << "\n";
    std::cout << "abs(-inf) = " << fabs( y ) << "\n";
    std::cout << "abs(+0) = " << fabs( z ) << "\n";
    std::cout << "abs(-0) = " << fabs( w ) << "\n";

    std::cout << "sqrt(+inf) = " << sqrt( x ) << "\n";
    std::cout << "sqrt(-inf) = " << sqrt( y ) << "\n";
    std::cout << "sqrt(+0) = " << sqrt( z ) << "\n";
    std::cout << "sqrt(-0) = " << sqrt( w ) << "\n";
  }
  {
    double x = std::numeric_limits<double>::infinity();
    double y = -x;
    double z = 0.;
    double w = -z;

    std::cout << "(+inf)=" << x << "\n";
    std::cout << "(-inf)=" << y << "\n";
    std::cout << "(+0)=" << z << "\n";
    std::cout << "(-0)=" << w << "\n";

    std::cout << "(+inf) == (+inf) : " << (x == y) << "\n";
    std::cout << "-(+inf) == (-inf) : " << (-x == y) << "\n";
    std::cout << "(+inf) == -(-inf) : " << ( x == -y) << "\n";
    std::cout << "(+0) == (+0) : " << (z == w) << "\n";
    std::cout << "-(+0) == (-0) : " << (-z == w) << "\n";
    std::cout << "(+0) == -(-0) : " << ( z == -w) << "\n";

    std::cout << "(+inf) + (+inf) = " << x + x << "\n";
    std::cout << "(+inf) - (+inf) = " << x - x << "\n";
    std::cout << "(+inf) + (-inf) = " << x + y << "\n";
    std::cout << "(-inf) + (-inf) = " << y + y << "\n";
    std::cout << "(-inf) - (-inf) = " << y - y << "\n";
    std::cout << "(-inf) + (+inf) = " << y + x << "\n";

    std::cout << "(+inf) * (+inf) = " << x * x << "\n";
    std::cout << "(+inf) * (-inf) = " << x * y << "\n";
    std::cout << "(-inf) * (+inf) = " << y * x << "\n";
    std::cout << "(-inf) * (-inf) = " << y * y << "\n";
    std::cout << "(+inf) * (+0) = " << x * z << "\n";
    std::cout << "(+inf) * (-0) = " << x * w << "\n";
    std::cout << "(-inf) * (+0) = " << y * z << "\n";
    std::cout << "(-inf) * (-0) = " << y * w << "\n";

    std::cout << "(+inf) / (+inf) = " << x / x << "\n";
    std::cout << "(+inf) / (-inf) = " << x / y << "\n";
    std::cout << "(-inf) / (+inf) = " << y / x << "\n";
    std::cout << "(-inf) / (-inf) = " << y / y << "\n";
    std::cout << "(+0) / (+inf) = " << z / x << "\n";
    std::cout << "(+0) / (-inf) = " << z / y << "\n";
    std::cout << "(-0) / (+inf) = " << w / x << "\n";
    std::cout << "(-0) / (-inf) = " << w / y << "\n";
    std::cout << "(+inf) / (+0) = " << x / z << "\n";
    std::cout << "(+inf) / (-0) = " << x / w << "\n";
    std::cout << "(-inf) / (+0) = " << y / z << "\n";
    std::cout << "(-inf) / (-0) = " << y / w << "\n";
    std::cout << "(+0) / (+0) = " << z / z << "\n";
    std::cout << "(+0) / (-0) = " << z / w << "\n";
    std::cout << "(-0) / (+0) = " << w / z << "\n";
    std::cout << "(-0) / (-0) = " << w / w << "\n";

    std::cout << "abs(+inf) = " << fabs( x ) << "\n";
    std::cout << "abs(-inf) = " << fabs( y ) << "\n";
    std::cout << "abs(+0) = " << fabs( z ) << "\n";
    std::cout << "abs(-0) = " << fabs( w ) << "\n";

    std::cout << "sqrt(+inf) = " << sqrt( x ) << "\n";
    std::cout << "sqrt(-inf) = " << sqrt( y ) << "\n";
    std::cout << "sqrt(+0) = " << sqrt( z ) << "\n";
    std::cout << "sqrt(-0) = " << sqrt( w ) << "\n";
  }
  {
    dd_Real x = dd_Real::inf();
    dd_Real y = -x;
    dd_Real z = dd_Real::zero();
    dd_Real w = -z;

    std::cout << "(+inf)=" << x << "\n";
    std::cout << "(-inf)=" << y << "\n";
    std::cout << "(+0)=" << z << "\n";
    std::cout << "(-0)=" << w << "\n";

    std::cout << "(+inf) == (+inf) : " << (x == y) << "\n";
    std::cout << "-(+inf) == (-inf) : " << (-x == y) << "\n";
    std::cout << "(+inf) == -(-inf) : " << ( x == -y) << "\n";
    std::cout << "(+0) == (+0) : " << (z == w) << "\n";
    std::cout << "-(+0) == (-0) : " << (-z == w) << "\n";
    std::cout << "(+0) == -(-0) : " << ( z == -w) << "\n";

    std::cout << "(+inf) + (+inf) = " << x + x << "\n";
    std::cout << "(+inf) - (+inf) = " << x - x << "\n";
    std::cout << "(+inf) + (-inf) = " << x + y << "\n";
    std::cout << "(-inf) + (-inf) = " << y + y << "\n";
    std::cout << "(-inf) - (-inf) = " << y - y << "\n";
    std::cout << "(-inf) + (+inf) = " << y + x << "\n";

    std::cout << "(+inf) * (+inf) = " << x * x << "\n";
    std::cout << "(+inf) * (-inf) = " << x * y << "\n";
    std::cout << "(-inf) * (+inf) = " << y * x << "\n";
    std::cout << "(-inf) * (-inf) = " << y * y << "\n";
    std::cout << "(+inf) * (+0) = " << x * z << "\n";
    std::cout << "(+inf) * (-0) = " << x * w << "\n";
    std::cout << "(-inf) * (+0) = " << y * z << "\n";
    std::cout << "(-inf) * (-0) = " << y * w << "\n";

    std::cout << "(+inf) / (+inf) = " << x / x << "\n";
    std::cout << "(+inf) / (-inf) = " << x / y << "\n";
    std::cout << "(-inf) / (+inf) = " << y / x << "\n";
    std::cout << "(-inf) / (-inf) = " << y / y << "\n";
    std::cout << "(+0) / (+inf) = " << z / x << "\n";
    std::cout << "(+0) / (-inf) = " << z / y << "\n";
    std::cout << "(-0) / (+inf) = " << w / x << "\n";
    std::cout << "(-0) / (-inf) = " << w / y << "\n";
    std::cout << "(+inf) / (+0) = " << x / z << "\n";
    std::cout << "(+inf) / (-0) = " << x / w << "\n";
    std::cout << "(-inf) / (+0) = " << y / z << "\n";
    std::cout << "(-inf) / (-0) = " << y / w << "\n";
    std::cout << "(+0) / (+0) = " << z / z << "\n";
    std::cout << "(+0) / (-0) = " << z / w << "\n";
    std::cout << "(-0) / (+0) = " << w / z << "\n";
    std::cout << "(-0) / (-0) = " << w / w << "\n";

    std::cout << "abs(+inf) = " << fabs( x ) << "\n";
    std::cout << "abs(-inf) = " << fabs( y ) << "\n";
    std::cout << "abs(+0) = " << fabs( z ) << "\n";
    std::cout << "abs(-0) = " << fabs( w ) << "\n";

    std::cout << "sqrt(+inf) = " << sqrt( x ) << "\n";
    std::cout << "sqrt(-inf) = " << sqrt( y ) << "\n";
    std::cout << "sqrt(+0) = " << sqrt( z ) << "\n";
    std::cout << "sqrt(-0) = " << sqrt( w ) << "\n";
  }
  

  init( L, alpha, x, y, z );
  
  std::cout << " BLAS1  : Vector length    = " << L <<"\n",
  std::cout << " BLAS2&3: Matrix dimension = " << M << " x " << M << "\n",
  std::cout << " - elapsed time - : func : - Relative Error in binary & EXP compounded format in [DS]P - \n";
  
#if 1
  verify<float>          ( L, alpha, x, y, z );
  verify<df_Real_quasi>  ( L, alpha, x, y, z );
  verify<df_Real_sloppy> ( L, alpha, x, y, z );
  verify<df_Real_weakaccurate>( L, alpha, x, y, z );
  verify<df_Real>        ( L, alpha, x, y, z );
  verify<double>         ( L, alpha, x, y, z );
  verify<tf_Real_quasi>  ( L, alpha, x, y, z );
  verify<tf_Real_sloppy> ( L, alpha, x, y, z );
  verify<tf_Real>        ( L, alpha, x, y, z );
  verify<qf_Real_quasi>  ( L, alpha, x, y, z );
  verify<qf_Real_sloppy> ( L, alpha, x, y, z );
  verify<qf_Real>        ( L, alpha, x, y, z );
  
  verify<dd_Real_quasi>  ( L, alpha, x, y, z );
  verify<dd_Real_sloppy> ( L, alpha, x, y, z );
#ifdef  _QD_DD_REAL_H
  verify<dd_real>        ( L, alpha, x, y, z ); // DD native
#endif
  verify<dd_Real>        ( L, alpha, x, y, z );
  verify<td_Real_quasi>  ( L, alpha, x, y, z );
  verify<td_Real_sloppy> ( L, alpha, x, y, z );
  verify<td_Real>        ( L, alpha, x, y, z );
  verify<qd_Real_quasi>  ( L, alpha, x, y, z );
  verify<qd_Real_sloppy> ( L, alpha, x, y, z );
#ifdef  _QD_QD_REAL_H
  verify<qd_real>        ( L, alpha, x, y, z ); // QD native
#endif
#endif
  verify<qd_Real>        ( L, alpha, x, y, z );
  
  std::cout << std::endl;
  std::cout.flush();
  

  delete [] x;
  delete [] y;
  delete [] z;
  
  return 0;
}

