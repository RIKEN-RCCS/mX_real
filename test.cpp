#include <cstdio>
#include <cstdlib>

#define MPFR_REAL_DATA_PUBLIC 1
//#include "real.hpp"
#include "mX_real.hpp"
//#include "convert.hpp"

using namespace mX_real;

typedef mpfr::real<113, MPFR_RNDN> mpfr128;
//typedef mpfr::real<237, MPFR_RNDN> mpfr256;

#define FP21
//#define mpfrrnd MPFR_RNDN
#ifdef FP21
typedef dmpfr_Real_sloppy quasidouble; // mX_real::dX_real
typedef qmpfr_Real_sloppy quasiquad;  // mX_real::qX_real
typedef pmpfr_Real_sloppy quadruple;  // mX_real::pX_real
#else
typedef df_Real_sloppy quasidouble; // mX_real::dX_real
typedef qf_Real_sloppy quasiquad;  // mX_real::qX_real
typedef pf_Real_sloppy quadruple;  // mX_real::pX_real
#endif

inline quasidouble fabs(const quasidouble &x) { return mX_real::dX_real::abs(x); }
inline quasiquad fabs(const quasiquad &x) { return mX_real::qX_real::abs(x); }
inline quadruple fabs(const quadruple &x) { return mX_real::pX_real::abs(x); }
//inline quadruple fabs(const octuple &x) { return x; } // dummy

template<typename T>
std::string tostring(const T &x) { std::string dummy; return dummy; };

template<>
std::string tostring<float>(const float &x)  {
  char buf[256];
  snprintf(buf, sizeof(buf), "%20.8e", x);
  return std::string(buf);
}

template<>
std::string tostring<double>(const double &x)  {
  char buf[256];
  snprintf(buf, sizeof(buf), "%24.16e", x);
  return std::string(buf);
}
#ifdef FP21
template<>
std::string tostring<mpfr128>(const mpfr128 &x)  {
  char buf[256];
  mpfr_sprintf(buf, "%24.16Re", x._x);
  return std::string(buf);
}


template<>
 std::string tostring<quasidouble>(const quasidouble &x)  { 
  char buf[256];
  char bufs[2][256];
  char buf0[256];
  mpfr128 sum(0);
  for (int i = 0; i < 2; i++) {
    mpfr_sprintf(bufs[i], "%14.7Re", x.x[i]._x);
    mpfr_add(sum._x, sum._x, x.x[i]._x, MPFR_RNDN);    
  }
  mpfr_set_exp(sum._x, x.iexp);  
  mpfr_sprintf(buf0, "%24.16Re", sum._x);    
  snprintf(buf, sizeof(buf), "%s : %d %s %s",
	   buf0, x.iexp,
  	   bufs[0], bufs[1]);
  return std::string(buf);  
}

template<>
 std::string tostring<quasiquad>(const quasiquad &x)  { 
  char buf[256];
  char bufs[4][256];
  char buf0[256];
  mpfr128 sum(0);
  for (int i = 0; i < 4; i++) {
    mpfr_sprintf(bufs[i], "%14.7Re", x.x[i]._x);
    mpfr_add(sum._x, sum._x, x.x[i]._x, MPFR_RNDN);
  }
  mpfr_set_exp(sum._x, x.iexp);  
  mpfr_sprintf(buf0, "%24.16Re", sum._x);  
  snprintf(buf, sizeof(buf), "%s : %d %s %s %s %s",
	   buf0, x.iexp,
  	   bufs[0], bufs[1], bufs[2], bufs[3]);
  return std::string(buf);
}
#else
template<>
 std::string tostring<quasidouble>(const quasidouble &x)  { 
  char buf[256];
  char bufs[2][256];
  char buf0[256];
  double sum(0);
  for (int i = 0; i < 2; i++) {
    snprintf(bufs[i], 256, "%14.7e", x.x[i]);
    sum += x.x[i];
  }
  sum = ldexp(sum, x.iexp);
  snprintf(buf0, sizeof(buf0), "%24.16e", sum);    
  snprintf(buf, sizeof(buf), "%s : %d %s %s",
	   buf0, x.iexp,
  	   bufs[0], bufs[1]);
  return std::string(buf);  
}
template<>
 std::string tostring<quasiquad>(const quasiquad &x)  { 
  char buf[256];
  char bufs[4][256];
  char buf0[256];
  double sum(0);
  for (int i = 0; i < 4; i++) {
    snprintf(bufs[i], 256, "%14.7e", x.x[i]);
    sum += x.x[i];
  }
  sum = ldexp(sum, x.iexp);
  snprintf(buf0, sizeof(buf0), "%24.16e", sum);    
  snprintf(buf, sizeof(buf), "%s : %d %s %s",
	   buf0, x.iexp,
  	   bufs[0], bufs[1]);
  return std::string(buf);  
}
#endif


template <typename T, typename S,
	  typename std::enable_if<
	    std::is_same<T, double>::value,
	    std::nullptr_t>::type = nullptr	    
	  > T convert(S const &x )
{
  return static_cast<double>(x);
}

template <typename T, typename S,
	  typename std::enable_if<
	    std::is_same<S, double>::value,
	    std::nullptr_t>::type = nullptr	    
	  > T convert(S const &x )
{
  T y(x);
  return y;
}

template<> quasidouble convert(double const &x)
{
  quasidouble y(x);
  return y;
}

template<> quasiquad convert(double const &x)
{
  quasiquad y(x);
  return y;
}

template<> double convert(quasidouble const &x)
{
  return static_cast<double>(x);
}

template<> double convert(quasiquad const &x)
{
  return static_cast<double>(x);
}

int main(int argc, char **argv)
{
  /* {
    mpfrfp x(1), y(0);
    double xx;
    y = x + x;
    xx = static_cast<double>(y);
    std::cout << "L96 " << xx << std::endl;
    } */ 
  srand(1);
  for (int i = 0; i < 1000000; i++) {
    //  for (int i = 0; i < 5; i++) {  
    double a, b, c, d, aex, bex, cex, dqd, dqq, x;
    a = ((double)rand() / (double)RAND_MAX - 0.5);
    b = ((double)rand() / (double)RAND_MAX - 0.5);
    c = ((double)rand() / (double)RAND_MAX - 0.5);
#if 1
    aex = ((double)rand() / (double)RAND_MAX - 0.5) * 32.0;
    bex = ((double)rand() / (double)RAND_MAX - 0.5) * 32.0;
    cex = ((double)rand() / (double)RAND_MAX - 0.5) * 32.0;
    a = a * pow(10.0, aex);
    b = b * pow(10.0, bex);
    c = c * pow(10.0, cex);
#endif
    quasidouble qda, qdb, qdc, qdd;
    quasiquad qqa, qqb, qqc, qqd;
    qda = quasidouble(a);
    qdb = quasidouble(b);
    qdc = quasidouble(c);
    qqa = quasiquad(a);
    qqb = quasiquad(b);
    qqc = quasiquad(c);
    //    x = static_cast<double>(qqc); // quasiquad -> double
    // add
    {
      d = a + b;
      qdd = qda + qdb;
      qqd = qqa + qqb;
      dqd = convert<double, quasidouble>(qdd);
      dqq = convert<double, quasiquad>(qqd);
      {
	bool flagf = (fabs(d - dqd) / fabs(d) > 1e-5);
	bool flagd = (fabs(d - dqq) / fabs(d) > 1e-14);
	std::string buff, bufd;
	if (flagf) {
	  buff = "f"+tostring<double>(fabs(d - dqd) / fabs(d))+" "+tostring<double>(a) + " " + tostring<double>(b);
	}
	else {
	  buff = "";
	};
	if (flagd) {
	  bufd = " d"+tostring<double>(fabs(d - dqq) / fabs(d))+" "+tostring<double>(a) + " " + tostring<quasiquad>(qqa) + " " +tostring<double>(b) + " " + tostring<quasiquad>(qqb);      
	}
	else {
	  bufd = "";
	};
	
	if (flagf || flagd) {
	  fprintf(stderr, "add : %s %s %s %s %s%s\n", tostring<double>(d).c_str(),
		  tostring<double>(dqd).c_str(),
		  tostring<quasidouble>(qdd).c_str(),
		  tostring<quasiquad>(qqd).c_str(),
		  buff.c_str(),
		  bufd.c_str());
	}
      }
    }
    {
      d = b;
      d += a;
      qdd = qda;
      qdd += qdb;
      qqd = qqa;
      qqd += qqb;
      dqd = convert<double, quasidouble>(qdd);
      dqq = convert<double, quasiquad>(qqd);
      {
	bool flagf = (fabs(d - dqd) / fabs(d) > 1e-5);
	bool flagd = (fabs(d - dqq) / fabs(d) > 1e-14);
	std::string buff, bufd;
	if (flagf) {
	  buff = "f"+tostring<double>(fabs(d - dqd) / fabs(d))+" "+tostring<double>(a) + " " + tostring<double>(b);
	}
	else {
	  buff = "";
	};
	if (flagd) {
	  bufd = " d"+tostring<double>(fabs(d - dqq) / fabs(d))+" "+tostring<double>(a) + " " + tostring<quasiquad>(qqa) + " " +tostring<double>(b) + " " + tostring<quasiquad>(qqb);      
	}
	else {
	  bufd = "";
	};
	
	if (flagf || flagd) {
	  fprintf(stderr, "ADD : %s %s %s %s %s%s\n", tostring<double>(d).c_str(),
		  tostring<double>(dqd).c_str(),
		  tostring<quasidouble>(qdd).c_str(),
		  tostring<quasiquad>(qqd).c_str(),
		  buff.c_str(),
		  bufd.c_str());
	}
      }
    }
      // sub
    {
      d = a - b;
      qdd = qda - qdb;
      qqd = qqa - qqb;
      dqd = convert<double, quasidouble>(qdd);
      dqq = convert<double, quasiquad>(qqd);
      {
	bool flagf = (fabs(d - dqd) / fabs(d) > 1e-5);
	bool flagd = (fabs(d - dqq) / fabs(d) > 1e-14);
	std::string buff, bufd;
	if (flagf) {
	  buff = "f"+tostring<double>(fabs(d - dqd) / fabs(d))+" "+tostring<double>(a) + " " + tostring<double>(b);
	}
	else {
	  buff = "";
	};
	if (flagd) {
	  bufd = " d"+tostring<double>(fabs(d - dqq) / fabs(d))+" "+tostring<double>(a) + " " + tostring<quasiquad>(qqa) + " " +tostring<double>(b) + " " + tostring<quasiquad>(qqb);
	}
	else {
	  bufd = "";
	};
	
	if (flagf || flagd) {
      fprintf(stderr, "sub : %s %s %s %s %s%s\n", tostring<double>(d).c_str(),
	      tostring<double>(dqd).c_str(),
	      tostring<quasidouble>(qdd).c_str(),
	      tostring<quasiquad>(qqd).c_str(),
	      buff.c_str(),
	      bufd.c_str());
	}
      }
    }
    {
      d = a;
      d -= b;
      qdd = qda;
      qdd -= qdb;
      qqd = qqa;
      qqd -= qqb;
      dqd = convert<double, quasidouble>(qdd);
      dqq = convert<double, quasiquad>(qqd);
      {
	bool flagf = (fabs(d - dqd) / fabs(d) > 1e-5);
	bool flagd = (fabs(d - dqq) / fabs(d) > 1e-14);
	std::string buff, bufd;
	if (flagf) {
	  buff = "f"+tostring<double>(fabs(d - dqd) / fabs(d))+" "+tostring<double>(a) + " " + tostring<double>(b);
	}
	else {
	  buff = "";
	};
	if (flagd) {
	  bufd = " d"+tostring<double>(fabs(d - dqq) / fabs(d))+" "+tostring<double>(a) + " " + tostring<quasiquad>(qqa) + " " +tostring<double>(b) + " " + tostring<quasiquad>(qqb);
	}
	else {
	  bufd = "";
	};
	
	if (flagf || flagd) {
      fprintf(stderr, "SUB : %s %s %s %s %s%s\n", tostring<double>(d).c_str(),
	      tostring<double>(dqd).c_str(),
	      tostring<quasidouble>(qdd).c_str(),
	      tostring<quasiquad>(qqd).c_str(),
	      buff.c_str(),
	      bufd.c_str());
	}
      }
    }    
    
    // mul
    {
      d = a * b;
      qdd = qda * qdb;
      qqd = qqa * qqb;
      dqd = convert<double, quasidouble>(qdd);
      dqq = convert<double, quasiquad>(qqd);
      {
	bool flagf = (fabs(d - dqd) / fabs(d) > 1e-5);
	bool flagd = (fabs(d - dqq) / fabs(d) > 1e-14);
	std::string buff, bufd;
	if (flagf) {
	  buff = "f"+tostring<double>(fabs(d - dqd) / fabs(d))+" "+tostring<quasidouble>(qda) + " " + tostring<quasidouble>(qdb);
	}
	else {
	  buff = "";
	};
	if (flagd) {
	  bufd = " d"+tostring<double>(fabs(d - dqq) / fabs(d))+" "+tostring<double>(a) + " " + tostring<quasiquad>(qqa) + " " +tostring<double>(b) + " " + tostring<quasiquad>(qqb);
	}
	else {
	  bufd = "";
	};
	
	if (flagf ||flagd) {
	  fprintf(stderr, "mul : %s %s %s %s %s%s\n", tostring<double>(d).c_str(),
		  tostring<double>(dqd).c_str(),
		  tostring<quasidouble>(qdd).c_str(),
		  tostring<quasiquad>(qqd).c_str(),
		  buff.c_str(),
		  bufd.c_str());
	}
      }
    }
    {
      d = a;
      d *= b;
      qdd = qda;
      qdd *= qdb;
      qqd = qqa;
      qqd *= qqb;
      dqd = convert<double, quasidouble>(qdd);
      dqq = convert<double, quasiquad>(qqd);
      {
	bool flagf = (fabs(d - dqd) / fabs(d) > 1e-5);
	bool flagd = (fabs(d - dqq) / fabs(d) > 1e-14);
	std::string buff, bufd;
	if (flagf) {
	  buff = "f"+tostring<double>(fabs(d - dqd) / fabs(d))+" "+tostring<double>(a) + " " + tostring<double>(b);
	}
	else {
	  buff = "";
	};
	if (flagd) {
	  bufd = " d"+tostring<double>(fabs(d - dqq) / fabs(d))+" "+tostring<double>(a) + " " + tostring<quasiquad>(qqa) + " " +tostring<double>(b) + " " + tostring<quasiquad>(qqb);      
	}
	else {
	  bufd = "";
	};
	
	if (flagf ||flagd) {
	  fprintf(stderr, "MUL : %s %s %s %s %s%s\n", tostring<double>(d).c_str(),
		  tostring<double>(dqd).c_str(),
		  tostring<quasidouble>(qdd).c_str(),
		  tostring<quasiquad>(qqd).c_str(),
		  buff.c_str(),
		  bufd.c_str());
	}
      }
    }
    // div
    {
      d = a / b;
      qdd = qda / qdb;
      qqd = qqa / qqb;
      dqd = convert<double, quasidouble>(qdd);
      dqq = convert<double, quasiquad>(qqd);
      {
	bool flagf = (fabs(d - dqd) / fabs(d) > 1e-5);
	bool flagd = (fabs(d - dqq) / fabs(d) > 1e-14);
	std::string buff, bufd;
	if (flagf) {
	  buff = "f"+tostring<double>(fabs(d - dqd) / fabs(d))+" "+tostring<double>(a) + " " + tostring<double>(b);
	}
	else {
	  buff = "";
	};
	if (flagd) {
	  bufd = " d"+tostring<double>(fabs(d - dqq) / fabs(d))+" "+tostring<double>(a) + " " + tostring<quasiquad>(qqa) + " " +tostring<double>(b) + " " + tostring<quasiquad>(qqb);      
	}
	else {
	  bufd = "";
	};
	
	if (flagf ||flagd) {
	  fprintf(stderr, "div : %s %s %s %s %s%s\n", tostring<double>(d).c_str(),
		  tostring<double>(dqd).c_str(),
		  tostring<quasidouble>(qdd).c_str(),
		  tostring<quasiquad>(qqd).c_str(),
		  buff.c_str(),
		  bufd.c_str());
	}
      }
    }
    {
      d = a;
      d /= b;
      qdd = qda / qdb;
      qqd = qqa / qqb;
      dqd = convert<double, quasidouble>(qdd);
      dqq = convert<double, quasiquad>(qqd);
      {
	bool flagf = (fabs(d - dqd) / fabs(d) > 1e-5);
	bool flagd = (fabs(d - dqq) / fabs(d) > 1e-14);
	std::string buff, bufd;
	if (flagf) {
	  buff = "f"+tostring<double>(fabs(d - dqd) / fabs(d))+" "+tostring<double>(a) + " " + tostring<double>(b);
	}
	else {
	  buff = "";
	};
	if (flagd) {
	  bufd = " d"+tostring<double>(fabs(d - dqq) / fabs(d))+" "+tostring<double>(a) + " " + tostring<quasiquad>(qqa) + " " +tostring<double>(b) + " " + tostring<quasiquad>(qqb);      
	}
	else {
	  bufd = "";
	};
	
	if (flagf ||flagd) {
	  fprintf(stderr, "DIV : %s %s %s %s %s%s\n", tostring<double>(d).c_str(),
		  tostring<double>(dqd).c_str(),
		  tostring<quasidouble>(qdd).c_str(),
		  tostring<quasiquad>(qqd).c_str(),
		  buff.c_str(),
		  bufd.c_str());
	}
      }
    }    
    
    // mad
    d = fma(a, b, c);
    qdd = qdc;
    madd_(qda, qdb, qdd);
    qqd = qqc;
    madd_(qqa, qqb, qqd);
    dqd = convert<double, quasidouble>(qdd);
    dqq = convert<double, quasiquad>(qqd);
    {
      bool flagf = ((fabs(d - dqd) / fabs(d) > 1e-5) || i == 5574  || i == 7379 || i == 7770);
      bool flagd = ((fabs(d - dqq) / fabs(d) > 1e-14) || i == 5574  || i == 7379 || i == 7770);
    std::string buff, bufd;
    if (flagf) {
      buff = "f"+tostring<double>(fabs(d - dqd) / fabs(d))+" "+tostring<double>(a) + " " + tostring<double>(b);
    }
    else {
      buff = "";
    };
    if (flagd) {
      bufd = " d"+tostring<double>(fabs(d - dqq) / fabs(d))+" "+tostring<double>(a) + " " + tostring<quasiquad>(qqa) + " " +tostring<double>(b) + " " + tostring<quasiquad>(qqb) + " " + tostring<double>(c) + " " + tostring<quasiquad>(qqc);      
    }
    else {
      bufd = "";
    };
     
    if (flagf ||flagd) {
      fprintf(stderr, "fma : %d %s %s %s %s %s%s\n", i, tostring<double>(d).c_str(),
	      tostring<double>(dqd).c_str(),
	      tostring<quasidouble>(qdd).c_str(),
	      tostring<quasiquad>(qqd).c_str(),
	      buff.c_str(),
	      bufd.c_str());
    }
    }

    // sqrt
    d = sqrt(a);
    qdd = sqrt(qda);
    qqd = sqrt(qqa);
    dqd = convert<double, quasidouble>(qdd);
    dqq = convert<double, quasiquad>(qqd);
    {
    bool flagf = (fabs(d - dqd) / fabs(d) > 1e-5);
    bool flagd = (fabs(d - dqq) / fabs(d) > 1e-14);
    std::string buff, bufd;
    if (flagf) {
      buff = "f"+tostring<double>(fabs(d - dqd) / fabs(d))+" "+tostring<double>(a) + " " + tostring<double>(b);
    }
    else {
      buff = "";
    };
    if (flagd) {
      bufd = " d"+tostring<double>(fabs(d - dqq) / fabs(d))+" "+tostring<double>(a) + " " + tostring<quasiquad>(qqa) + " " +tostring<double>(b) + " " + tostring<quasiquad>(qqb);
    }
    else {
      bufd = "";
    };
     
    if (flagf || flagd) {
      fprintf(stderr, "div : %s %s %s %s %s%s\n", tostring<double>(d).c_str(),
	      tostring<double>(dqd).c_str(),
	      tostring<quasidouble>(qdd).c_str(),
	      tostring<quasiquad>(qqd).c_str(),
	      buff.c_str(),
	      bufd.c_str());
    }
    }    
  }
}  
