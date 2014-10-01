#include <limits>
#include <algorithm>
#include <math.h>
#include <gsl/gsl_fit.h>
#include "signature.h"

using namespace std;

istream &operator>>(istream& is,
		    Points&  p)
{
  if(is.iword(biomet_input_format))
    is >> p.PosX >> p.PosY >> p.Pression >> p.Azimuth >> p.Altitude;
  else
    is >> p.PosX >> p.PosY >> p.Timestamp >> p.Button >> p.Azimuth >> p.Altitude >> p.Pression;

  return is;
}

ostream &operator<<(ostream&       os,
		    const Points&  p)
{
  if(os.iword(biomet_input_format))
    os << p.PosX << ' ' << p.PosY << ' ' << p.Pression << ' ' << p.Azimuth << ' ' << p.Altitude << endl;
  else
    os << p.PosX << ' ' << p.PosY << ' ' << p.Timestamp << ' ' << p.Button << ' ' << p.Azimuth << ' '
       << p.Altitude << ' ' << p.Pression << endl;

  return os;
}


Signature::Signature()
{
}

Signature::~Signature()
{
}

void Signature::load(string filename)
{
  ifstream source(filename.c_str());
  string ext;
   
  if (strcmp(ext.c_str(), "ric") == 0)
    source.iword(biomet_input_format) = true;
  else
    source.ignore(200, '\n');

  istream_iterator<Points> start(source);
  istream_iterator<Points> end;
  back_insert_iterator< vector<Points> > dest(data_);

  copy(start, end, dest);
  source.close();
}


void Signature::save(string filename, vector<Points> data)
{
  ofstream dest(filename.c_str());
  ostream_iterator<Points> out_it(dest);

  dest << data.size() << endl;
  copy(data.begin(), data.end(), out_it);
  dest.close();
}


inline static double sqr(double x) {
    return x*x;
}

int linreg(int n, std::vector<Points> data, double* m, double* b, double* r)
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
   if (r!=NULL) {
      *r = (sumxy - sumx * sumy / n) /          /* compute correlation coeff     */
            sqrt((sumx2 - sqr(sumx)/n) *
            (sumy2 - sqr(sumy)/n));
   }

   return 0; 
}

void Signature::rotate(vector<Points>& data)
{
  double *m = new double;
  double *b = new double;
  int N = data.size();
  int res = 0;

  res = linreg(data.size(), data, m, b, NULL);

  if (res == 0)
  {
    std::cout << "m : " << *m << std::endl;
    std::cout << "b : " << *b << std::endl;

    for (int i = 0; i < N; ++i)
    {
      data[i].PosY = data[i].PosY - *m * data[i].PosX - *b;
      data[i].PosX = data[i].PosX - *b;
    }

    // Flip
    // for (int i = 0; i < N; ++i)
    //   data[i].PosY = data[N - i - 1].PosY;

  }
  else
    std::cout << "Linear Regression failed" << std::endl;
}

// void Signature::rotate(vector<Points>& data)
// {
//   int dir, x, y;
//   double mx = means_(data, 'X');
//   double my = means_(data, 'Y');
//   double varx = variance_(data, 'X');
//   double vary = variance_(data, 'Y');
//   double cov = covariance_(data);
//   double a, angle, b, coef, D, E = 0;

//   D = sqrt((varx - vary) * (varx - vary) + 4 * cov);
//   a = sqrt(1 / 2 + ((varx - vary) / (2 * D)));
//   b = (cov / abs(cov)) * sqrt(1 / 2 - ((varx - vary) / (2 * D)));
//   coef = b/a;
//   angle = atan(coef);
//   dir = coef > 0 ? -1 : 1;

//   for (vector<Points>::iterator i = data.begin(); i != data.end(); ++i)
//     {
//       x = mx + (i->PosX - mx) * cos(angle) - dir * (i->PosY - my) * sin(angle);
//       y = my + dir * (i->PosX - mx) * sin(angle) + (i->PosY - my) * cos(angle);
      
//       i->PosX = x;
//       i->PosY = y;
//     }
//  }

void Signature::centrage(vector<Points>& data)
{
  double mx = means_(data, 'X');
  double my = means_(data, 'Y');
  double x,y;
  
  for (vector<Points>::iterator it = data.begin(); it != data.end(); ++it)
  {
    it->PosX = it->PosX - mx;
    it->PosY = it->PosY - my;
  }
}

// Might Require some improvement
double Signature::DTW(vector<Points>& data1, vector<Points>& data2)
{
  int N = data1.size();
  int M = data2.size();
  double cost = 0;
  int total = 0;

  double DTW[N][M];

  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < M; ++j)
      DTW[i][j] = std::numeric_limits<double>::max();
  }

  DTW[0][0] = 0;

  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < M; ++j)
    {
      total++;
      cost = euclidian_distance_(data1[i], data2[j]);
      DTW[i][j] = cost + min(DTW[i - 1][j], min(DTW[i][j - 1], DTW[i - 1][j - 1]));
    }
  }

  return DTW[N - 1][M - 1] / total;
}

//================ SHAPE CHARACTERISTICS ================

double Signature::L(vector<Points>& data)
{
  double L = 0;
  int N = data.size();

  for (int i = 0; i < N - 1; ++i)
    L += euclidian_distance_(data[i], data[i + 1]);

  return L;
}

double Signature::DPD(vector<Points>& data)
{
  double DPD = 0;
  int N = data.size();

  DPD = euclidian_distance_(data[0], data[N - 1]);

  return DPD;
}

// La formule donnée dans le pdf n'a pas de sens...
double Signature::RGD(vector<Points>& data)
{
  double maxG = 0;
  double maxD = 0;
  int N = data.size();

  return 0;
}

// idem
double Signature::RHB(vector<Points>& data)
{
  double RHB = 0;
  int N = data.size();



  return RHB;
}


double Signature::RXY(vector<Points>& data)
{
  double RX = 0;
  double RY = 0;
  double RXY = 0;
  int N = data.size();

  for (int i = 0; i < N - 1; ++i)
    RX += (data[i].PosX - data[i + 1].PosX);

  for (int i = 0; i < N - 1; ++i)
    RY += (data[i].PosY - data[i + 1].PosY);

  RXY = RX / RY;

  return RXY;
}

double Signature::APD(vector<Points>& data)
{
  double APD = 0;
  int N = data.size();

  APD = atan((data[N - 1].PosY - data[0].PosY) / (data[N - 1].PosX - data[0].PosX));

  return APD;
}

double Signature::SA(vector<Points>& data)
{
  double SA = 0;
  int N = data.size();

  for (int i = 0; i < N - 1; ++i)
    SA += atan((data[i + 1].PosY -  data[i].PosY) / (data[i + 1].PosX -  data[i].PosX));

  return SA;
}

double Signature::SAA(vector<Points>& data)
{
  double SAA = 0;
  int N = data.size();

  for (int i = 0; i < N - 1; ++i)
    SAA += std::abs(atan((data[i + 1].PosY -  data[i].PosY) / (data[i + 1].PosX -  data[i].PosX)));

  return SAA;
}

//=============== DYNAMIC CHARACTERISTICS ===============

double Signature::TT(std::vector<Points>& data)
{
  double N = data.size();
  double TT = 0;

  TT = data[N - 1].Timestamp - data[0].Timestamp;

  return TT;
}

double Signature::VMV(std::vector<Points>& data)
{
  double N = data.size();
  double VMV = 0;
  double VMY = 0;

  for (int i = 0; i < N - 1; ++i)
    VMY += std::abs(data[i + 1].PosY - data[i].PosY);

  VMV = VMY / TT(data);

  return VMV;
}

double Signature::VMH(std::vector<Points>& data)
{
  double N = data.size();
  double VMH = 0;
  double VMX = 0;

  for (int i = 0; i < N - 1; ++i)
    VMX += std::abs(data[i + 1].PosX - data[i].PosX);

  VMH = VMX / TT(data);
  
  return VMH;
}
//=================== PRIVATE METHODS ===================

double Signature::means_(vector<Points> data, char variable)
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

double Signature::variance_(vector<Points> data, char variable)
{
  double mx = means_(data, 'X');
  double my = means_(data, 'Y');
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

double Signature::covariance_(vector<Points> data)
{
  double mx = means_(data, 'X');
  double my = means_(data, 'Y');
  double total = 0;
  
  for (vector<Points>::iterator i = data.begin(); i != data.end(); ++i)
    total += (i->PosX - mx) * (i->PosY - my);

  return total / data.size();
}

double Signature::euclidian_distance_(Points &a, Points &b)
{
  return sqrt(pow((a.PosX - b.PosX), 2) + pow((a.PosY - b.PosY), 2));
}
