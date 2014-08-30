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
  unsigned long PosX, PosY, Timestamp, Button, Azimuth, Altitude, Pression;
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
  vector<Points> getData() {return data_;};

 private:
  unsigned long means_(vector<Points> data, char variable);
  unsigned long variance_(vector<Points> data, char variable);
  unsigned long covariance_(vector<Points> data);
  vector<Points> data_;
};

#endif
