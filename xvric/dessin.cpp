#include <fstream>
#include "dessin.h"

std::istream &operator>>(std::istream&  is,
                         Points&        p)
{
  if(is.iword (biomet_input_format)) // BIOMET Signature File format
    is >> p.PosX >> p.PosY >> p.Pression >> p.Azimuth >> p.Altitude ;
  else // SVC2004 Signature File format
    is >> p.PosX >> p.PosY >> p.Timestamp >> p.Button >> p.Azimuth >> p.Altitude >> p.Pression;
  return is;

}

std::string extension(std::string filename)
{
  return filename.substr(filename.find_last_of(".") + 1);
}

void Dessin::zoom(int ech)
  {
    Echelle = (double) ech / 100.;
    Trace_in_buffer();
  }

void Dessin::reload(const QString & file)
  {
    data_.clear();
    Load(file.toStdString());
    CalcZone();
    Trace_in_buffer();
  }


void Dessin::paintEvent(QPaintEvent *)
  {
    //    bitBlt(this, 0, 0, &buffer_);
    QPainter p(this);
    p.drawPixmap(0,0, *buffer_);
  }

void Dessin::resizeEvent(QResizeEvent *)
  {
  }

void Dessin::CalcZone()
  {
    if (data_.begin() >= data_.end())
      return;

    Min_x = Max_x = (data_.begin())->PosX;
    Min_y = Max_y = (data_.begin())->PosY;
    for(std::vector<Points>::iterator i = data_.begin(); i < data_.end(); ++i)
      {
        if (Min_x > i->PosX)
          Min_x = i->PosX;
        else if (Max_x < i->PosX)
          Max_x = i->PosX;
        if (Min_y > i->PosY)
          Min_y = i->PosY;
        else if (Max_y < i->PosY)
          Max_y = i->PosY;
      }
  }

void Dessin::Trace_in_buffer()
  {
    if (buffer_ == NULL)
      delete buffer_;
    buffer_ = new QPixmap( (int)((Max_x - Min_x) * Echelle + 2 * Marge),
                    (int)((Max_y - Min_y) * Echelle + 2 * Marge));
    buffer_->fill(Qt::white);
    Trace_in_buffer_line();

    this->resize((int)((Max_x - Min_x) * Echelle + 2 * Marge),
                 (int)((Max_y - Min_y) * Echelle + 2 * Marge));
  }

void Dessin::Trace_in_buffer_line()
  {
    QPainter bufferpainter;
    bufferpainter.begin(buffer_);
    std::vector<Points>::iterator last = data_.begin();
    for(std::vector<Points>::iterator i = data_.begin(); i != data_.end(); ++i)
      {
        if(i->Button != 0)
          bufferpainter.drawLine((int)((last->PosX - Min_x) * Echelle + Marge),
                                 (int)((last->PosY - Min_y) * Echelle + Marge),
                                 (int)((i->PosX - Min_x) * Echelle + Marge),
                                 (int)((i->PosY - Min_y) * Echelle + Marge));

        last = i;
      }
    bufferpainter.end();
  }

void Dessin::Trace_in_buffer_points()
  {
    QPainter bufferpainter;
    QPen Pen;
    Pen.setColor(Qt::red);
    bufferpainter.begin(buffer_);
    bufferpainter.setPen(Pen);
    for(std::vector<Points>::iterator i = data_.begin(); i != data_.end(); ++i)
      if(i->Pression != 0)
        bufferpainter.drawEllipse((int)((i->PosX - Min_x) * Echelle + Marge),
                                  (int)((i->PosY - Min_y) * Echelle + Marge),
                                  5,5);
    bufferpainter.end();
  }

void Dessin::Load(std::string filename)
  {

    std::ifstream source(filename.c_str());
    std::string ext = extension(filename);

    if(strcmp(ext.c_str(),"ric") == 0) // BIOMET Signature File Format
      source.iword (biomet_input_format) = true;
    else // SVC2004 Signature File Format
      source.ignore(200, '\n'); // number of signature points

    std::istream_iterator< Points > start(source);
    std::istream_iterator< Points > end;
    std::back_insert_iterator< std::vector<Points> > dest(data_);

    std::copy(start,end,dest);
  }

Dessin::Dessin(std::string filename, double Echelle_, int Marge_, QWidget *parent) :
    QWidget(parent)
  {
    Echelle = Echelle_;
    Marge = Marge_;
    Load(filename);
    CalcZone();
    Trace_in_buffer();
  }

Dessin::~Dessin()
{
}
