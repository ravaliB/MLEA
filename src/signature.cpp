#include <limits>
#include <algorithm>
#include <math.h>
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

void Signature::rotate(vector<Points>& data)
{
  int dir, x, y;
  long mx = means_(data, 'X');
  long my = means_(data, 'Y');
  long varx = variance_(data, 'X');
  long vary = variance_(data, 'Y');
  long cov = covariance_(data);
  long a, angle, b, coef, D, E = 0;

  D = sqrt((varx - vary) * (varx - vary) + 4 * cov);
  a = sqrt(1 / 2 + (varx - vary) / 2 * D);
  b = cov / abs(cov) * sqrt(1 / 2 - (varx - vary) / 2 * D);
  coef = b/a;
  angle = atan(coef);
  dir = coef > 0 ? -1 : 1;

  for (vector<Points>::iterator i = data.begin(); i != data.end(); ++i)
    {
      x = mx + (i->PosX - mx) * cos(angle) - dir * (i->PosY - my) * sin(angle);
      y = my + dir * (i->PosX - mx) * sin(angle) + (i->PosY - my) * cos(angle);
      
      i->PosX = x;
      i->PosY = y;
    }
 }

// Might Require some improvement
long Signature::DTW(vector<Points>& data1, vector<Points>& data2)
{
  int N = data1.size();
  int M = data2.size();
  long cost = 0;
  int total = 0;

  long DTW[N][M];

  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < M; ++j)
      DTW[i][j] = std::numeric_limits<long>::max();
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

long Signature::L(vector<Points>& data)
{
  long L = 0;
  int N = data.size();

  for (int i = 0; i < N - 1; ++i)
    L += euclidian_distance_(data[i], data[i + 1]);

  return L;
}

long Signature::DPD(vector<Points>& data)
{
  long DPD = 0;
  int N = data.size();

  DPD = euclidian_distance_(data[0], data[N - 1]);

  return DPD;
}

// La formule donnée dans le pdf n'a pas de sens...
long Signature::RGD(vector<Points>& data)
{
  long maxG = 0;
  long maxD = 0;
  int N = data.size();

  return 0;
}

// idem
long Signature::RHB(vector<Points>& data)
{
  long RHB = 0;
  int N = data.size();



  return RHB;
}


long Signature::RXY(vector<Points>& data)
{
  long RX = 0;
  long RY = 0;
  long RXY = 0;
  int N = data.size();

  for (int i = 0; i < N - 1; ++i)
    RX += (data[i].PosX - data[i + 1].PosX);

  for (int i = 0; i < N - 1; ++i)
    RY += (data[i].PosY - data[i + 1].PosY);

  RXY = RX / RY;

  return RXY;
}

long Signature::APD(vector<Points>& data)
{
  long APD = 0;
  int N = data.size();

  APD = atan((data[N - 1].PosY - data[0].PosY) / (data[N - 1].PosX - data[0].PosX));

  return APD;
}

long Signature::SA(vector<Points>& data)
{
  long SA = 0;
  int N = data.size();

  for (int i = 0; i < N - 1; ++i)
    SA += atan((data[i + 1].PosY -  data[i].PosY) / (data[i + 1].PosX -  data[i].PosX));

  return SA;
}

long Signature::SAA(vector<Points>& data)
{
  long SAA = 0;
  int N = data.size();

  for (int i = 0; i < N - 1; ++i)
    SAA += std::abs(atan((data[i + 1].PosY -  data[i].PosY) / (data[i + 1].PosX -  data[i].PosX)));

  return SAA;
}

//=============== DYNAMIC CHARACTERISTICS ===============

long Signature::TT(std::vector<Points>& data)
{
  long N = data.size();
  long TT = 0;

  TT = data[N - 1].Timestamp - data[0].Timestamp;

  return TT;
}

long Signature::VMV(std::vector<Points>& data)
{
  long N = data.size();
  long VMV = 0;
  long VMY = 0;

  for (int i = 0; i < N - 1; ++i)
    VMY += std::abs(data[i + 1].PosY - data[i].PosY);

  VMV = VMY / TT(data);

  return VMV;
}

long Signature::VMH(std::vector<Points>& data)
{
  long N = data.size();
  long VMH = 0;
  long VMX = 0;

  for (int i = 0; i < N - 1; ++i)
    VMX += std::abs(data[i + 1].PosX - data[i].PosX);

  VMH = VMX / TT(data);
  
  return VMH;
}
//=================== PRIVATE METHODS ===================

long Signature::means_(vector<Points> data, char variable)
{
  long mX = 0;
  long mY = 0;

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

long Signature::variance_(vector<Points> data, char variable)
{
  long mx = means_(data, 'X');
  long my = means_(data, 'Y');
  long varX = 0;
  long varY = 0;

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

long Signature::covariance_(vector<Points> data)
{
  long mx = means_(data, 'X');
  long my = means_(data, 'Y');
  long total = 0;
  
  for (vector<Points>::iterator i = data.begin(); i != data.end(); ++i)
    total += (i->PosX - mx) * (i->PosY - my);

  return total / data.size();
}

long Signature::euclidian_distance_(Points &a, Points &b)
{
  return sqrt(pow((a.PosX - b.PosX), 2) + pow((a.PosY - b.PosY), 2));
}
