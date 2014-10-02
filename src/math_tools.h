#ifndef MATH_TOOLS_H
#define MATH_TOOLS_H

#include <vector>
#include <math.h>
#include "points.h"

using namespace std;

class MathTools
{
	public:
		MathTools();
		~MathTools();

		// TOOLS
  		double means(vector<Points> data, char variable);
  		double variance(vector<Points> data, char variable);
  		double covariance(vector<Points> data);
  		double euclidian_distance(Points &a, Points &b);
		double getArea(vector<Points> data);
		vector<Points> dilatation(vector<Points> data, int scale);
  		int linear_regression(int n, std::vector<Points> data, double* m, double* b, double* r);
};

#endif /* MATH_TOOLS_HH */
