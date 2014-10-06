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

//get The bounding box of Signature
double MathTools::getArea(vector<Points> data)
{
  double minX = means(data, 'X');
  double minY = means(data, 'Y');
  double maxX = means(data, 'X');
  double maxY = means(data, 'Y');
  double width, weight;
 
  for (vector<Points>::iterator it = data.begin(); it != data.end(); ++it)
    {
      minX = (it->PosX < minX) ? it->PosX : minX;
      minY = (it->PosY < minY) ? it->PosY : minY;
      maxX = (it->PosX > maxX) ? it->PosX : maxX;
      maxY = (it->PosY > maxY) ? it->PosY : maxY;
    }

  width = maxY - minY;
  weight = maxX - minX;

  return width * weight;
}

double MathTools::getSimpsonArea(vector<Points> data)
{
  double a = means(data, 'X');
  double b = means(data, 'X');
  double n = data.size();
  double h;
  double sum1, sum2, res;
  
  for (vector<Points>::iterator it = data.begin(); it != data.end(); ++it)
    {
      a = (a > it->PosX) ? it->PosX : a;
      b = (b < it->PosX) ? it->PosX : b;
    }
  
  h = (b - a) / n;
  
  for (int i = 2; i < n / (2 - 1); i += 2)
    {
      sum1 += data[i].PosY;      
    }

  for (int j = 1; j < n / 2; j += 2) 
    {
      sum2 += data[j].PosY;
    } 
 
  res = h/3 * (data[0].PosY + 2 * sum1 + 4 * sum2 + data[n].PosY);

  return abs(res);
}

//A modifier si possible
vector<Points> MathTools::dilatation(vector<Points> data, int scale)
{
  vector<Points> d = data;
  double mx = means(data, 'X');
  double my = means(data, 'Y');
  double dx, dy;

  for (vector<Points>::iterator it = d.begin(); it != d.end(); ++it)
    {
      dx = it->PosX - mx;
      dy = it->PosY - my;
      it->PosX = (it->PosX + dx) * scale;
      it->PosY = (it->PosY + dy) * scale;
    }
    
  return d;
}

vector<Points> MathTools::dilate(vector<Points> data, int scale)
{
  vector<Points> d;
  Points pr, pl, pt, pb; //points at right, left, top and bottom

  for (vector<Points>::iterator it = data.begin(); it != data.end(); ++it)
    {
      pr = (*it);
      pl = (*it);
      pt = (*it);
      pb = (*it);

      pr.PosX += scale;
      pl.PosX -= scale;
      pt.PosY += scale;
      pb.PosY -= scale;
     
      d.push_back(pr);
      d.push_back(pl);
      d.push_back((*it));
      d.push_back(pt);
      d.push_back(pb);
    }

  return d;
}


int MathTools::vectorisation(vector<Points> data, vector<Points>::iterator& it1, int thresh, bool& vectorChanged)
{
  double coef, origin_ordone;
  int n = 0;

  if (vectorChanged)
    {
      vector<Points>::iterator it2 = it1 + 1;
      coef = (it2->PosY - it1->PosY) / (it2->PosX - it1->PosX);
      origin_ordone = it1->PosY - (coef * it1->PosX);
      vectorChanged = false;
      ++it1;
   }
  else
    {
      if (abs((coef * it1->PosX + origin_ordone) - it1->PosY) <= thresh)
	++it1;
      else
	{
	  n = 1;
	  vectorChanged = true;
	}
    }

  return n;
}

double MathTools::nbPtInRect(vector<Points> data, double width, double weight)
{
  double mx = means(data, 'X');
  double my = means(data, 'Y');
  double nb = 0;
  double mwidth = width / 2;
  double mweight = weight / 2;

  for (vector<Points>::iterator it = data.begin(); it != data.end(); ++it)
    {
      if ((it->PosX <= (mx + mwidth) &&
	   it->PosX >= (mx - mwidth)) &&
	  (it->PosY <= (my + mweight) &&
	   it->PosY >= (my - mweight)))
	nb++;
    }

  return nb;
}
