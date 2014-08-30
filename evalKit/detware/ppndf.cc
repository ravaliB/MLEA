#include <cstdio>
#include <math.h>
#include <cstdlib>
#include "plot.hh"

#define abs(x)  (x < 0  ?  -x : x)

#define SPLIT      0.42

#define A0      2.5066282388
#define A1    -18.6150006252
#define A2     41.3911977353
#define A3    -25.4410604963
#define B1     -8.4735109309
#define B2     23.0833674374
#define B3    -21.0622410182
#define B4      3.1308290983
#define C0     -2.7871893113
#define C1     -2.2979647913
#define C2      4.8501412713
#define C3      2.3212127685
#define D1      3.5438892476
#define D2      1.6370678189

#define LL      140

#define eps     2.2204e-16

// Convert lines with ROC coordinates to normal deviate scale.
// Ignore lines beginning with "#" (9/26/94)

double ppndf (double p)
{
  double q;
  double r;
  double retval;


/* printf ("p = %f\n", p); */

  if (p >= 1.0)
    p = 1 - eps;
  if (p <= 0.0)
    p = eps;

  q = p - 0.5;
  if (abs(q) <= SPLIT)
    {
      r = q * q;
      retval = q * (((A3 * r + A2) * r + A1) * r + A0) /
	((((B4 * r + B3) * r + B2) * r + B1) * r + 1.0);
    }
  else
    {
/*
    r = sqrt (log (0.5 - abs(q)));
*/
      r = (q > 0.0  ?  1.0 - p : p);
      if (r <= 0.0)
	{
	  printf ("Found r = %f\n", r);
	  exit(1);
	}
      r = sqrt ((-1.0) * log (r));
      retval = (((C3 * r + C2) * r + C1) * r + C0) /
	((D2 * r + D1) * r + 1.0);
      if (q < 0)
	retval *= -1.0;
    }

  return (retval);
}
