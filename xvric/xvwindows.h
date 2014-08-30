#ifndef XVWINDOWS_H
#define XVWINDOWS_H

#include <QMainWindow>
#include <QMenuBar>
#include <QSlider>
#include <QToolBar>
#include <QLCDNumber>
#include <QComboBox>
#include <QScrollArea>
#include <QKeyEvent>
#include "dessin.h"

class xvWindows : public QMainWindow
{
    Q_OBJECT

    QScrollArea *_scrollview;
    QToolBar *_toolbar;
    QComboBox *_combo;
    QSlider *_slider;
    Dessin *_dessin;
    QLCDNumber *_mylcd;

public slots:
    void open();

protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void keyPressEvent(QKeyEvent *event);

public:
    xvWindows(std::vector<std::string> &list_filename, double Echelle = 1., int Marge = 20, QWidget *parent = 0);
    ~xvWindows();
};

#endif // XVWINDOWS_H
