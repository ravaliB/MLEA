#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

#include "plot.hh"

extern double Cmiss, Cfa, Ptrue, Pfalse;

#define PMIN    0.0005
#define PMAX    0.5


using namespace std;

static bool cmp_score(const double &a, const double &b)
{
  return a < b;
}

void
Compute_ROC (vector<double> &true_scores, vector<double> &false_scores,
	     vector<pair<double, double> > &Pdet,
	     vector<double> &DCF_dec,
	     vector<double> &DCF_best)
{
  sort(true_scores.begin(), true_scores.end());
  sort(false_scores.begin(), false_scores.end());

  double sum = 0.;
  unsigned long ntrue = 0;
  unsigned long nfalse = 0;
  unsigned long ind = 0;
  double thr;
  double mindcf, mindcf_miss, mindcf_fa, minthr, mindcf_pmiss, mindcf_pfa;
  double eermiss, eerfa;

  //  Pdet.push_back(pair<double, double>(ppndf(PMIN), ppndf(PMAX)));
  mindcf = Cfa * Pfalse;
  mindcf_miss = 0;
  mindcf_pfa = 1.;
  minthr = -INFINITY;
  mindcf_pmiss = 0;
  mindcf_pfa = 1.;

  Pdet.push_back(pair<double, double>(ppndf(0.), ppndf(1.)));

  while ((ntrue < true_scores.size()) || (nfalse < false_scores.size()))
    {
      if ((nfalse >= false_scores.size()) ||
	  ((ntrue < true_scores.size()) && (true_scores[ntrue] <= false_scores[nfalse])))
	++ntrue;
      else
	++nfalse;
      ++ind;
      thr = true_scores[ntrue];
      double Pmiss = (double)(ntrue) / true_scores.size();
      double Pfa   = (double)(false_scores.size() - nfalse) / false_scores.size();
      if( Pmiss <= Pfa)
	{
	  eermiss = Pmiss;
	  eerfa = Pfa;
	}
      Pdet.push_back(pair<double, double>(ppndf(Pmiss), ppndf(Pfa)));
      double dcf_miss = (Cmiss * Pmiss * Ptrue);
      double dcf_fa = (Cfa * Pfa * Pfalse);
      double dcf = dcf_miss + dcf_fa;
      if (dcf <= mindcf)
	{
	  mindcf = dcf;
	  mindcf_miss = dcf_miss;
	  mindcf_fa = dcf_fa;
	  minthr = thr;
	  mindcf_pmiss = Pmiss;
	  mindcf_pfa = Pfa;
	}
    }
  double norm = min (Cmiss * Ptrue, Cfa * Pfalse);

  DCF_dec[3] = Cmiss * DCF_dec[1] * Ptrue;
  DCF_dec[4] = Cfa * DCF_dec[2] * Pfalse;
  DCF_dec[0] = (DCF_dec[3] + DCF_dec[4]) / norm;

  DCF_best.resize(0);
  DCF_best.push_back(mindcf / norm);
  DCF_best.push_back(ppndf(mindcf_pmiss));
  DCF_best.push_back(ppndf(mindcf_pfa));
  DCF_best.push_back(mindcf_miss);
  DCF_best.push_back(mindcf_fa);


  cout << " EER              = ";
  cout.width(10);
  cout << eerfa << " => PFA = "; cout.width(10); cout << eerfa << " PMiss = " << eermiss << endl;
  cout << " DCF Decision     = ";
  cout.width(10);
  cout << DCF_dec[0] << " => PFA = "; cout.width(10); cout <<  DCF_dec[2] << " PMiss = " <<  DCF_dec[1] << endl;
  cout << " MinDCF           = ";
  cout.width(10);
  cout << mindcf / norm << " => PFA = "; cout.width(10); cout << mindcf_pfa << " PMiss = " << mindcf_pmiss << endl;
  cout << "                  Optimal theshold = " << minthr << endl;

  DCF_dec[1] = ppndf(DCF_dec[1]);
  DCF_dec[2] = ppndf(DCF_dec[2]);
}
