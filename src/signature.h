#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <iterator>
#include <fstream>
#include <math.h>
#include <cmath>

using namespace std;

extern int biomet_input_format;

struct Points
{
  double PosX, PosY, Timestamp, Button, Azimuth, Altitude, Pression;
};

istream &operator>>(istream& is,
		    Points&  p);

ostream &operator<<(ostream&       os,
		    const Points&  p);

class Signature
{
 public:
  Signature();
  ~Signature();
  void load(string filename);
  void save(string filename, vector<Points> data);
  void rotate(vector<Points>& data);
  void centrage(vector<Points>& data);

  // Algorithm
  double DTW(vector<Points>& data1, vector<Points>& data2);

  // Shape Caracteristics
  double L(vector<Points>& data1);
  double DPD(vector<Points>& data);
  double RGD(vector<Points>& data);
  double RHB(vector<Points>& data);
  double RXY(vector<Points>& data);
  double APD(vector<Points>& data);
  double SA(vector<Points>& data);
  double SAA(vector<Points>& data);

  // Dynamic Caracteristics
  double TT(std::vector<Points>& data);
  double VMV(std::vector<Points>& data);
  double VMH(std::vector<Points>& data);


  vector<Points> getData() {return data_;};

 private:
  double means_(vector<Points> data, char variable);
  double variance_(vector<Points> data, char variable);
  double covariance_(vector<Points> data);
  double euclidian_distance_(Points &a, Points &b);
  vector<Points> data_;
};

#endif /* SIGNATURE_HH */
