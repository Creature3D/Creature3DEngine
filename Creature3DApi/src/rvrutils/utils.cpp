#include "rvrutils/utils.h"
#include <time.h>

using namespace rvrutils;

int
rvrutils::myround( float n )
{
    double i, f, r;

    f = modf( n, &i );
    if ( f < 0.0 ) {
        i--;
        f = 1.0 + f;
    }

    r = ((f > 0.5) ? i + 1 : i);

//    cerr << "n: " << n << " i: " << i << " f: " << f << " r: " << r << "\n";

    return (int)r;
}

#ifdef _WIN32
#include <stdlib.h>
/*! rint returns the integer (represented as a float precision number)
     nearest its float argument x in the direction of the prevailing
     rounding mode.  rintl is the long float counterpart of rint.  rint
     has no counterpart that accepts an argument of type float.
     */
//float rint(float x) 
//{
//  float y, z;
//  int n;
//  
//  if(x >= 0) 
//  {
//    y = x + 0.5;
//    z = floor(y);
//    n = (int) z;
//    if (y == z && n % 2) --z;
//  } 
//  else 
//  {
//    y = x - 0.5;
//    z = ceil(y);
//    n = (int) z;
//    if(y == z && n % 2) ++z;
//  }
//  return z;
//}


double drand48( void )
{
  static bool first=true;

  if (first) {
    srand(0);
    first = false;
  }

  int i = rand();

  return (i / (double)RAND_MAX);
}

#endif

double rvrutils::randInterval(double low, double high)
{
  static bool first=true;

  if (first) {
    first = false;
    srand(time(0));
  }
  if (low > high) {
    double t = high;
    high = low;
    low = t;
  }

  double length = high - low; // Length of interval

  double a = rand() / (double)RAND_MAX; // // Rand between 0.0 and 1.0
  double b = length * a + low;  // Rand between low and high

  return b;
}
