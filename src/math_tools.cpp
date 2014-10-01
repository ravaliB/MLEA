#include "math_tools.h"

using namespace std;

inline static double sqr(double x) {
    return x*x;
}

MathTools::MathTools()
{
}

MathTools::~MathTools()
{
}

double MathTools::means(vector<Points> data, char variable)
{
  double mX = 0;
  double mY = 0;

  for (vector<Points>::iterator i = data.begin(); i != data.end(); ++i)
    {
      mX += i->PosX;
      mY += i->PosY; 
    }

  switch (variable)
    {
    case 'X':
      return mX / data.size();
      break;
    case 'Y':
      return mY / data.size();
      break;
    default:
      return 0;
      break;
    }
}

double MathTools::variance(vector<Points> data, char variable)
{
  double mx = means(data, 'X');
  double my = means(data, 'Y');
  double varX = 0;
  double varY = 0;

  for (vector<Points>::iterator i = data.begin(); i != data.end(); ++i)
    {
      varX += (mx - i->PosX) * (mx - i->PosX);
      varY += (my - i->PosY) * (my - i->PosY); 
    }

   switch (variable)
    {
    case 'X':
      return varX / data.size();
      break;
    case 'Y':
      return varY / data.size();
      break;
    default:
      return 0;
      break;
    }
}

double MathTools::covariance(vector<Points> data)
{
  double mx = means(data, 'X');
  double my = means(data, 'Y');
  double total = 0;
  
  for (vector<Points>::iterator i = data.begin(); i != data.end(); ++i)
    total += (i->PosX - mx) * (i->PosY - my);

  return total / data.size();
}

int MathTools::linear_regression(int n, std::vector<Points> data, double* m, double* b, double* r)
{
    double   sumx = 0.0;                        /* sum of x                      */
    double   sumx2 = 0.0;                       /* sum of x**2                   */
    double   sumxy = 0.0;                       /* sum of x * y                  */
    double   sumy = 0.0;                        /* sum of y                      */
    double   sumy2 = 0.0;                       /* sum of y**2                   */

   for (int i = 0;i < n; i++)   
      { 
      sumx  += data[i].PosX;       
      sumx2 += sqr(data[i].PosX);  
      sumxy += data[i].PosX * data[i].PosY;
      sumy  += data[i].PosY;      
      sumy2 += sqr(data[i].PosY); 
      } 

   double denom = (n * sumx2 - sqr(sumx));
   if (denom == 0) {
       // singular matrix. can't solve the problem.
       *m = 0;
       *b = 0;
       *r = 0;
       return 1;
   }

   *m = (n * sumxy  -  sumx * sumy) / denom;
   *b = (sumy * sumx2  -  sumx * sumxy) / denom;
   if (r != nullptr) {
      *r = (sumxy - sumx * sumy / n) /          /* compute correlation coeff     */
            sqrt((sumx2 - sqr(sumx)/n) *
            (sumy2 - sqr(sumy)/n));
   }

   return 0; 
}

double MathTools::euclidian_distance(Points &a, Points &b)
{
  return sqrt(pow((a.PosX - b.PosX), 2) + pow((a.PosY - b.PosY), 2));
}
