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
#include "points.h"
#include "math_tools.h"

using namespace std;

extern int biomet_input_format;

istream &operator>>(istream& is,
		    Points&  p);

ostream &operator<<(ostream&       os,
		    const Points&  p);

class Signature
{
 public:
  Signature();
  ~Signature();
  
  void reset();
  void loadSignatures(string file_1, string file_2);
  void reduction();
  void getCharacteristics();
  void normalization();
  void computeScore();

  vector<Points> getData1() {return data_1;};
	vector<Points> getData2() {return data_2;};
  vector<double> getScores() {return scores;};

 private:
  // Loader
  void load(string filename, vector<Points> *data);
  void save(string filename, vector<Points> data);

  // Reduction
  void minimum_speed_reduction(vector<Points>& data, int range);

  // Normalization
  void rotate(vector<Points>& data);
  void center(vector<Points>& data);

  // Caracteristics
  double DPD(vector<Points>& data);
  double SA(vector<Points>& data);
  
  // Comparison Method
  double DTW();

  // Attributes
  vector<Points> data_1;
  vector<Points> data_2;
  vector<double> scores;
  MathTools mt;
  double threshold;
};

#endif /* SIGNATURE_HH */
