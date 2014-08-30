#ifndef DESSIN_H
#define DESSIN_H

#include <QPixmap>
#include <QWidget>
#include <QPainter>
#include <QPen>

extern int biomet_input_format;

struct Points
{
  unsigned long PosX, PosY, Timestamp, Button, Azimuth, Altitude, Pression;
};

std::istream &operator>>(std::istream&  is,
                         Points&        p);


class Dessin : public QWidget
{
    Q_OBJECT

    QPixmap *buffer_;
      std::vector<Points> data_;
      unsigned long Min_x, Max_x, Min_y, Max_y;
      unsigned int Marge;
      double Echelle;

    public slots:

      void zoom(int ech);
      void reload(const QString & file);

    protected :

      void paintEvent(QPaintEvent *);
      void resizeEvent(QResizeEvent *);


    private :

      void CalcZone();
      void Trace_in_buffer();
      void Trace_in_buffer_line();
      void Trace_in_buffer_points();
      void Load(std::string filename);

    public :

      Dessin(std::string filename, double Echelle_, int Marge_ = 20, QWidget *parent = 0);
      ~Dessin();
};

#endif // DESSIN_H
