#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

#include "plot.hh"

using namespace std;

double Pfalse;
double Ptrue;
double Cmiss;
double Cfa;

unsigned char flags;

unsigned nbToken(const string &str)
{
  istringstream istr(str);
  string ch;
  unsigned i = 0;
  while (istr >> ch)
    ++i;

  return i;
}

void LoadScoreFile(vector<double> &res, double &PTrue, double &PFalse, const string &filename)
{
  double s;
  res.resize(0);
  ifstream file(filename.c_str());
  if (!file)
    {
      cerr << "Enable to open speaker file score : " << filename << endl;
      exit(1);
    }
  string ligne, ch;
  if (!getline(file, ligne))
    {
      cerr << "Empty Score File : " << filename << endl;
      exit(1);
    }
  unsigned nbTok = nbToken(ligne);
  double nbTrue, nbFalse, nbTrials;
  nbTrue = nbFalse = nbTrials = 0.;

  do
    {
      istringstream istr(ligne);
      for(unsigned i = 0; i < nbTok - 2; ++i)
	istr >> ch;
      char c;
      istr >> s;
      istr >> c;
      res.push_back(s);
      if (tolower(c) == 't')
	nbTrue += 1;
      else if (tolower(c) == 'f')
	nbFalse += 1;
      else
	cerr << "Warning : " << c << " is not a decision char" << endl;

      nbTrials += 1;
    }
  while (getline(file, ligne));

  file.close();
  PTrue = nbTrue / nbTrials;
  PFalse = nbFalse / nbTrials;
}

void parseOpt(const std::string &optName, const std::string &Value)
{
  char *f = NULL;
  if (optName == "Pfa")
    {
      Pfalse = strtod(Value.c_str(), &f);
      flags |= 1;
    }
  else if (optName == "Cmiss")
    {
      Cmiss = strtod(Value.c_str(), &f);
      flags |= 2;
    }
  else if (optName == "Cfa")
    {
      Cfa = strtod(Value.c_str(), &f);
      flags |= 4;
    }
  else
    {
      std::cerr << "Unknown option : --" << optName << std::endl;
      exit(1);
    }

  if (*f)
    {
      std::cerr << "Wrong Double Value : " << Value << std::endl;
      exit(1);
    }
}


void checkOptions(int &ac, char *av[])
{
  flags = 0;
  for(int i = 0; i < ac; ++i)
    {
      if (av[i][0] == '-')
        {
          if (av[i][1] == '-')
            parseOpt(av[i] + 2, av[i+1]);
	  else
	    parseOpt(av[i] + 1, av[i+1]);

	  for(int j = i; j < ac - 2; ++j)
	    av[j] = av[j+2];
	  ac -= 2;
	  --i;
        }
    }
  if (flags == 0)
    {
      Pfalse = 0.8;
      Cmiss = 10;
      Cfa = 1;
    }
  else if (flags != 7)
    {
      std::cerr << "Need Options --Pfa <double> --Cmiss <double> --Cfa <double>" << std::endl;
      exit(1);
    }
  Ptrue = 1 - Pfalse;
}


int main (int ac, char *av[])
{

  checkOptions(ac, av);

  vector<double> tar;
  vector<double> non;
  vector<pair<double, double> > pts;
  vector<double> DCF_best, DCF_Decision(5, 0.);
  double PT, PF;

  LoadScoreFile(tar, PT, PF, av[1]);
  DCF_Decision[1] = PF;
  LoadScoreFile(non, PT, PF, av[2]);
  DCF_Decision[2] = PT;

  if (tar.size() == 0 && non.size() == 0)
    {
      cerr << "Empty TAR and NON files" << endl;
      exit(1);
    }
  Compute_ROC (tar, non, pts, DCF_Decision, DCF_best);

  ofstream outfile (av[3]);
  for (vector<pair<double, double> >::iterator i = pts.begin(); i != pts.end(); ++i)
    outfile << i->first << " " << i->second << endl;
  outfile.close();

  ofstream bestfile((string(av[3]) + ".BestDCF").c_str());
  bestfile << DCF_best[0] << " " << DCF_best[1] << " " << DCF_best[2]
	   << " " << DCF_best[3] << " " << DCF_best[4]<<endl;
  bestfile.close();

  ofstream decfile((string(av[3]) + ".DCF").c_str());
  decfile << DCF_Decision[0] << " " << DCF_Decision[1] << " " << DCF_Decision[2]
	   << " " << DCF_Decision[3] << " " << DCF_Decision[4] << endl;
  decfile.close();
}


