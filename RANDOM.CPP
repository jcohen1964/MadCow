// Author  : Keith H. Duggar
// Date    : 2000.06.20
// Name    : random.c
// Purpose : This is the implementation of class Random.
//
////////////////////////////////////////////////////////////////////////////////
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
000000000100000000020000000003000000000400000000050000000006000000000700000000*/
////////////////////////////////////////////////////////////////////////////////

#include <ctime>
#include <iostream>
#include "math.h"
#include "random.h"

Random*      Random::_instance  = 0;

int    const Random::maxInt    (0x7FFFFFFD);
int    const Random::modulus   (0x7FFFFFFF); 
int    const Random::multiplier(     69621);
int    const Random::quotient  (     30845);
int    const Random::remainder (     23902);
double const Random::maxAngle  (double(maxInt)     / double(maxInt + 1));
double const Random::maxSlice  (double(maxInt + 1) / double(maxInt + 2));
double const Random::minSlice  (1.0                / double(maxInt + 2));

int    const Random::poolSize(128);
int          Random::pool[Random::poolSize];

Random*  Random::instance() {
	if(!_instance) _instance = new Random();
	return _instance;
}

Random::Random() {
	time_t theTime;
	time(&theTime);
	int mySeed(theTime);
    prevGenInt = mySeed;
    initPool();
}

//Initialize the elements of the pool.
void Random::initPool() {

	for(int i = 0; i < poolSize; ++i) {
		prevGenInt = nextInt(prevGenInt);
		pool[i] = prevGenInt;
	}

	prevOutInt = nextInt(prevGenInt);  //Set prevOutInt for the first shuffle index.
}
/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
//Access functions
int    Random::getMaxInt  (void) { return maxInt  ;}
double Random::getMaxAngle(void) { return maxAngle;}
double Random::getMaxSlice(void) { return maxSlice;}
double Random::getMinSlice(void) { return minSlice;}
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//Binomial deviates
//Numerical Recipes in C, 1990.  pp. 223-224 (Section 7.3)
////////////////////////////////////////////////////////////////////////////////

int	   Random::nextBin(double p, int n) {

	int j;
	static int nold=(-1);
	double am,em,g,angle,bnl,sq,t,y, pp;
	static double pold=(-1.0),pc,plog,pclog,en,oldg;

	//The binomial distribution is symetric about the value 0.5.  If pp is 
	//assigned 1.0-p, then the answer must also be transformed -- see below.
	if(p<=0.5) {pp=p;} else {pp=1.0-p;}	

	am = n*p; //Arithmetic mean of the distribution

	//Compute the answer as a sum of bernouli deviates if n is not too large
	if(n < 25) {
		bnl = 0.0;
		for (j=1;j<=n;j++) {
			if(nextFloat()<p) {bnl+=1;}
		}
	} else 
		if(am<1.0) {
			g=exp(-am);
			t=1.0;
			for(j=0;j<=n;j++) {
				t*= nextFloat();
				if (t<g) break;
			}
			if(j<=n) {bnl=j;} else {bnl=n;}

		} else {
			if(n!=nold) {
				en=n;
				oldg=gammln(en+1.0);
				nold=n;
			}
			if (p != pold) {
				pc=1.0-p;
				plog=log(p);
				pclog=log(pc);
				pold=p;
			}
			sq=sqrt(2.0*am*pc);
			do {
				do {
					angle=3.14159*nextFloat();
					y=tan(angle);
					em=sq*y+am;
				} while(em<0.0 || em >= (en+1.0));
				em=floor(em);
				t=1.2*sq*(1.0+y*y)*exp(oldg-gammln(em+1.0) -
					gammln(en-em+1.0)+em*plog+(en-em)*pclog);
			} while (nextFloat() > t);
			bnl=em;
		}
		if(p != pp) {bnl=n-bnl;}
		return bnl;
}

//////////////////////////////////////////////////////////////////////////////////
//Poisson function                                                              //
//From GNU                                                                      //
//////////////////////////////////////////////////////////////////////////////////
int  Random::nextPoisson(double L) {
	if (L == 0) {return 0;}
	if (L < 1.E-6) {return PoissonLow(L);}
	if (L < 17) {return PoissonInver(L);}
    return PoissonRatioUniforms(L);
  }
  
int Random::PoissonLow(double L) {
	/*
   This subfunction generates a random variate with the poisson 
   distribution for extremely low values of L.

   The method is a simple calculation of the probabilities of x = 1
   and x = 2. Higher values are ignored.

   The reason for using this method is to avoid the numerical inaccuracies 
   in other methods.
*/   
  double d, r;
  d = sqrt(L);
  if (nextFloat() >= d) return 0;
  r = nextFloat() * d;
  if (r > L * (1.-L)) return 0;
  if (r > 0.5 * L*L * (1.-L)) return 1;
  return 2;
}

int Random::PoissonInver(double L) {
/*
   This subfunction generates a random variate with the poisson 
   distribution using inversion by the chop down method (PIN).

   Execution time grows with L. Gives overflow for L > 80.

   The value of bound must be adjusted to the maximal value of L.
*/   
  const int bound = 130;             // safety bound. Must be > L + 8*sqrt(L).
  static double p_L_last = -1.;      // previous value of L
  static double p_f0;                // value at x=0
  double r;                          // uniform random number
  double f;                          // function value
  int x;                           // return value

  if (L != p_L_last) {               // set up
    p_L_last = L;
    p_f0 = exp(-L);}                 // f(0) = probability of x=0

  while (1) {  
    r = nextFloat();  x = 0;  f = p_f0;
    do {                        // recursive calculation: f(x) = f(x-1) * L / x
      r -= f;
      if (r <= 0) return x;
      x++;
      f *= L;
      r *= x;}                       // instead of f /= x
    while (x <= bound);}
}

int Random::PoissonRatioUniforms(double L) {
/*
   This subfunction generates a random variate with the poisson 
   distribution using the ratio-of-uniforms rejection method (PRUAt).

   Execution time does not depend on L, except that it matters whether L
   is within the range where ln(n!) is tabulated.

   Reference: E. Stadlober: "The ratio of uniforms approach for generating
   discrete random variates". Journal of Computational and Applied Mathematics,
   vol. 31, no. 1, 1990, pp. 181-189.
*/
  static double p_L_last = -1.0;            // previous L
  static double p_a;                       // hat center
  static double p_h;                       // hat width
  static double p_g;                       // ln(L)
  static double p_q;                       // value at mode
  static int    p_bound;                    // upper bound
  int mode;                              // mode
  double u;                                // uniform random
  double lf;                               // ln(f(x))
  double x;                                // real sample
  int k;                                 // integer sample
	const double SHAT1 = 2.943035529371538573;    // 8/e
	const double SHAT2 = 0.8989161620588987408;   // 3-sqrt(12/e)


  if (p_L_last != L) {
    p_L_last = L;                           // Set-up
    p_a = L + 0.5;                          // hat center
    mode = (int)L;                        // mode
    p_g  = log(L);
    p_q = mode * p_g - LnFac(mode);         // value at mode
    p_h = sqrt(SHAT1 * (L+0.5)) + SHAT2;    // hat width
    p_bound = (int)(p_a + 6.0 * p_h);}    // safety-bound

  while(1) {
    u = nextFloat();
    if (u == 0) continue;                   // avoid division by 0
    x = p_a + p_h * (nextFloat() - 0.5) / u;
    if (x < 0 || x >= p_bound) continue;    // reject if outside valid range
    k = (int)(x);
    lf = k * p_g - LnFac(k) - p_q;
    if (lf >= u * (4.0 - u) - 3.0) break;   // quick acceptance
    if (u * (u - lf) > 1.0) continue;       // quick rejection
    if (2.0 * log(u) <= lf) break;}         // final acceptance
  return(k);}


/***********************************************************************
                     Log factorial function
***********************************************************************/
double Random::LnFac(int n) {
  // log factorial function. gives natural logarithm of n!

  // define constants
  static const double        // coefficients in Stirling approximation     
    C0 =  0.918938533204672722,   // ln(sqrt(2*pi))
    C1 =  1./12., 
    C3 = -1./360.;
    // C5 =  1./1260.,  // use r^5 term if FAK_LEN < 50
    // C7 = -1./1680.;  // use r^7 term if FAK_LEN < 20
  // static variables
	static const int FAK_LEN = 1024;       // length of factorial table
	static double fac_table[FAK_LEN]; // table of ln(n!):
	static int initialized = 0;   // remember if fac_table has been initialized

  if (n < FAK_LEN) {
    if (n <= 1) {
      return 0;}
    if (!initialized) { // first time. Must initialize table
      // make table of ln(n!)
      double sum = fac_table[0] = 0.;
      for (int i=1; i<FAK_LEN; i++) {
        sum += log(i);
        fac_table[i] = sum;}
      initialized = 1;}
    return fac_table[n];}
    
  // not found in table. use Stirling approximation
  double  n1, r;
  n1 = n;  r  = 1. / n1;
  return (n1 + 0.5)*log(n1) - n1 + C0 + r*(C1 + r*r*C3);}


/////////////////////////////////////////////////////////////////////////////////
//ln Gamma function
//Numerical Recipes in C, 1990.  p. 168. (Section 6.1)
////////////////////////////////////////////////////////////////////////////////
double   Random::gammln(double xx) {
	
	double x, tmp, ser;
	static double cof[6] = {76.18009173,
							-86.50532033,
							24.01409822,
							-1.231739516,
							0.120858003e-2,
							-0.536382e-5};
	int j;

	x=xx-1.0;
	tmp=x+5.5;
	tmp -= (x+0.5)*log(tmp);
	ser=1.0;
	for(j=0;j<=5;j++) {
		x+=1.0;
		ser += cof[j]/x;
	}
	return -tmp+log(2.50662827465*ser);
}
