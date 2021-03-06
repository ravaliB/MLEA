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
:threshold(14)
{
  mt = MathTools();
}

Signature::~Signature()
{
}


void Signature::reset()
{
  data_1.clear();
  data_2.clear();
}

void Signature::loadSignatures(string file_1, string file_2)
{
  load(file_1, &data_1);
  load(file_2, &data_2);
}

void Signature::reduction()
{
  minimum_speed_reduction(data_1, 3);
  minimum_speed_reduction(data_2, 3);
}

void Signature::normalization()
{
  rotate(data_1);
  rotate(data_2);

  center(data_1);
  center(data_2);
}

void Signature::getCharacteristics()
{
  double df1 = DF(data_1, 02);
  double dv1 = DV(data_1, 30);
  sa1 = SA(data_1);
  dpd1 = DPD(data_1);
  vector<double> dm1 = DM(data_1, 500, mt.means(data_1, 'X'), mt.means(data_1, 'Y'));

  double df2 = DF(data_2, 02);
  double dv2 = DV(data_2, 30);
  sa2 = SA(data_2);
  dpd2 = DPD(data_2);
  vector<double> dm2 = DM(data_2, 500, mt.means(data_2, 'X'), mt.means(data_2, 'Y')); 
  
  vector<double> descriptors1;
  vector<double> descriptors2;
  vector<double>::iterator it1, it2;
  
  descriptors1.push_back(df1);
  descriptors1.push_back(dv1);
  descriptors1.push_back(sa1);
  descriptors1.push_back(dpd1);

  descriptors2.push_back(df2);
  descriptors2.push_back(dv2);
  descriptors2.push_back(sa2);
  descriptors2.push_back(dpd2);
  
  for (it1 = dm1.begin(), it2 = dm2.begin();
       it1 != dm1.end() && it2 != dm2.end();
       ++it1, ++it2)
    {
      if (((*it1) != INFINITY) && 
	  ((*it1) >= 0) &&
	  ((*it2) != INFINITY) &&
	  ((*it2) >= 0))
	{
	  descriptors1.push_back(*it1);
	  descriptors2.push_back(*it2);
	} 
    }

  comparators.insert(std::pair<vector<double>, vector<double> > (descriptors1, descriptors2));
}

void Signature::computeScore()
{
  double res = threshold - DTW(1, 0.00);

  if (abs(sa1 - sa2) < 3)
    res += (6 - abs(sa1 - sa2)) / 2;

  else if (30 < abs(sa1 - sa2))
    res -= 1.5;

  double p = 100 * (abs(dpd1 - dpd2) / max(dpd1, dpd2));

  if (p < 3)
    res += 3 - p;

  else if (25 < p)
    res -= p - 25;

  scores.push_back(res); 
}

//============================== PRIVATE METHODS ==============================

// LOADER

void Signature::load(string filename, vector<Points> *data)
{
  ifstream source(filename.c_str());
  string ext;
   
  if (strcmp(ext.c_str(), "ric") == 0)
    source.iword(biomet_input_format) = true;
  else
    source.ignore(200, '\n');

  istream_iterator<Points> start(source);
  istream_iterator<Points> end;

  back_insert_iterator< vector<Points> > dest(*data);

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

// REDUCTION

void Signature::minimum_speed_reduction(vector<Points>& data, int range)
{
  int N = data.size();
  int min = 0;
  int inst_speed = 0;
  vector<int> speeds;
  vector<Points> buf;

  for (int i = 1; i < N - 1; ++i)
  {
    inst_speed = mt.euclidian_distance(data[i + 1], data[i - 1]) / (data[i + 1].Timestamp - data[i - 1].Timestamp);
    speeds.push_back(inst_speed);
  } 

  buf.push_back(data[0]);
  buf.push_back(data[N - 1]);

  for (int i = 0; i < speeds.size(); i += range)
  {
    min = i;

    for (int j = 1; j < range; ++j)
    {
      if (i + j < speeds.size() && speeds[min] > speeds[i + j])
        min = i + j;
    }

    buf.push_back(data[min + 1]);
  }

  data.clear();

  for (int i = 0; i < buf.size(); i++)
    data.push_back(buf[i]);
}

// NORMALIZATION

void Signature::rotate(vector<Points>& data)
{
  double *m = new double;
  double *b = new double;
  int N = data.size();
  int res = 0;

  res = mt.linear_regression(data.size(), data, m, b, NULL);

  if (res == 0)
  {
    for (int i = 0; i < N; ++i)
    {
      data[i].PosY = data[i].PosY - *m * data[i].PosX - *b;
      data[i].PosX = data[i].PosX - *b;
    }
  }
}

void Signature::center(vector<Points>& data)
{
  double mx = mt.means(data, 'X');
  double my = mt.means(data, 'Y');
  double x,y;
  
  for (vector<Points>::iterator it = data.begin(); it != data.end(); ++it)
  {
    it->PosX = it->PosX - mx;
    it->PosY = it->PosY - my;
  }
}

// CHARACTERISTICS

double Signature::DPD(vector<Points> data)
{
  double DPD = 0;
  int N = data.size();

  DPD = mt.euclidian_distance(data[0], data[N - 1]);

  return DPD;
}

double Signature::SA(vector<Points> data)
{
  double SA = 0;
  double res = 0;
  int N = data.size();


  for (int i = 0; i < N - 1; i++)
  {
	 res = atan((data[i + 1].PosY -  data[i].PosY) / (data[i + 1].PosX - data[i].PosX));

   if (res == res)
    SA += res;
  }

  return SA;
}

double Signature::DF(vector<Points> data, int scale)
{
  vector<Points> Xe = mt.dilate(data, scale);
  double A = mt.getSimpsonArea(Xe);
  double res = 2 - (log(A / scale) / log(scale));

  return A;
}

double Signature::DV(vector<Points> data, int thresh)
{
  int nbvector = 0;
  double res;
  vector<Points>::iterator it = data.begin();
  bool isChangingVector = true;

  while (it != data.end())
    {
      nbvector += mt.vectorisation(data, it, thresh, isChangingVector); 
    }

  res = 1 - (log(nbvector / thresh) / log(thresh));
  
  return res;
}

vector<double> Signature::DM(vector<Points> data, double ecart, double width, double weight)
{
  double area = mt.getArea(data);
  double nbPointsInRect;
  double res;
  vector<double> vectRect;
  int i = 0;

  while ((width * weight) < area)
    {
      nbPointsInRect = mt.nbPtInRect(data, width, weight);
      res = log(nbPointsInRect) / log(width * weight);
      vectRect.push_back(res);
      width += ecart;
      weight += ecart;
      i++;
    }
  
  return vectRect;
}

// COMPARISON METHOD

// Might Require some improvement
double Signature::DTW(double a, double b)
{
  int N = data_1.size();
  int M = data_2.size();
  double cost = 0;
  int total = 0;

  double DTW[N][M];

  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < M; ++j)
      DTW[i][j] = numeric_limits<double>::max();
  }

  DTW[0][0] = 0;

  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < M; ++j)
    {
      total++;
      cost = a * mt.euclidian_distance(data_1[i], data_2[j]) + b * (abs(data_1[i].Timestamp - data_2[j].Timestamp)) / 10000;
      DTW[i][j] = cost + min(DTW[i - 1][j], min(DTW[i][j - 1], DTW[i - 1][j - 1]));
    }
  }

  return DTW[N - 1][M - 1] / total;
}
