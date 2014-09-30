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
  long PosX, PosY, Timestamp, Button, Azimuth, Altitude, Pression;
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

  // Algorithm
  long DTW(vector<Points>& data1, vector<Points>& data2);

  // Shape Caracteristics
  long L(vector<Points>& data1);
  long DPD(vector<Points>& data);
  long RGD(vector<Points>& data);
  long RHB(vector<Points>& data);
  long RXY(vector<Points>& data);
  long APD(vector<Points>& data);
  long SA(vector<Points>& data);
  long SAA(vector<Points>& data);

  // Dynamic Caracteristics
  long TT(std::vector<Points>& data);
  long VMV(std::vector<Points>& data);
  long VMH(std::vector<Points>& data);


  vector<Points> getData() {return data_;};

 private:
  long means_(vector<Points> data, char variable);
  long variance_(vector<Points> data, char variable);
  long covariance_(vector<Points> data);
  long euclidian_distance_(Points &a, Points &b);
  vector<Points> data_;
};

#endif /* SIGNATURE_HH */
