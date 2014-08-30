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
  unsigned long mx = means_(data, 'X');
  unsigned long my = means_(data, 'Y');
  unsigned long varx = variance_(data, 'X');
  unsigned long vary = variance_(data, 'Y');
  unsigned long cov = covariance_(data);
  unsigned long a, angle, b, coef, D, E = 0;

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

//=================== PRIVATE METHODS =================

unsigned long Signature::means_(vector<Points> data, char variable)
{
  unsigned long mX = 0;
  unsigned long mY = 0;

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

unsigned long Signature::variance_(vector<Points> data, char variable)
{
  unsigned long mx = means_(data, 'X');
  unsigned long my = means_(data, 'Y');
  unsigned long varX = 0;
  unsigned long varY = 0;

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

unsigned long Signature::covariance_(vector<Points> data)
{
  unsigned long mx = means_(data, 'X');
  unsigned long my = means_(data, 'Y');
  unsigned long total = 0;
  
  for (vector<Points>::iterator i = data.begin(); i != data.end(); ++i)
    total += (i->PosX - mx) * (i->PosY - my);

  return total / data.size();
}
