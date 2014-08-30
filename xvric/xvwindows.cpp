#include "xvwindows.h"
#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QPixmap>
#include <QWidget>
#include <QMenuBar>
#include <QEvent>
#include <QScrollArea>
#include <QToolBar>
#include <QToolButton>
#include <QMainWindow>
#include <QPixmap>
#include <QComboBox>
#include <QSlider>
#include <QLCDNumber>
#include <QFileDialog>
#include <QMenu>
#include <QKeyEvent>
#include <QApplication>
#include <QGridLayout>
#include <unistd.h>

/* XPM */
static const char *fileopen[] = {
    "    16    13        5            1",
    ". c #040404",
    "# c #808304",
    "a c None",
    "b c #f3f704",
    "c c #f3f7f3",
    "aaaaaaaaa...aaaa",
    "aaaaaaaa.aaa.a.a",
    "aaaaaaaaaaaaa..a",
    "a...aaaaaaaa...a",
    ".bcb.......aaaaa",
    ".cbcbcbcbc.aaaaa",
    ".bcbcbcbcb.aaaaa",
    ".cbcb...........",
    ".bcb.#########.a",
    ".cb.#########.aa",
    ".b.#########.aaa",
    "..#########.aaaa",
    "...........aaaaa"
};

void xvWindows::open()
{
    char *buf = get_current_dir_name(); //getwd(NULL);
    QString nom = QFileDialog::getOpenFileName(this, "Selectionner un Fichier", buf, "*.txt");
    _combo->insertItem(0, nom);
    _combo->setCurrentIndex(0);
    _dessin->reload(nom);
    free(buf);
}

void xvWindows::resizeEvent(QResizeEvent *)
{
    _scrollview->setGeometry(0, menuBar()->height() + _toolbar->height(),
                             width(), height() - menuBar()->height() - _toolbar->height());

}

void xvWindows::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_Q:
        qApp->quit();
        break;
    case Qt::Key_Less:
        _slider->setValue(_slider->value() - 5);
        _mylcd->display(_slider->value());
        _dessin->zoom(_slider->value());
        break;
    case Qt::Key_Greater:
        _slider->setValue(_slider->value() + 5);
        _mylcd->display(_slider->value());
        _dessin->zoom(_slider->value());
        break;
    case Qt::Key_Space:
        if(_combo->currentIndex() < _combo->count() - 1)
        {
            _combo->setCurrentIndex(_combo->currentIndex() + 1);
            _dessin->reload(_combo->currentText());
        }
        break;
    case Qt::Key_Backspace:
        if(_combo->currentIndex() > 0)
        {
            _combo->setCurrentIndex(_combo->currentIndex() - 1);
            _dessin->reload(_combo->currentText());
        }
        break;
    }
}

xvWindows::xvWindows(std::vector<std::string> &list_filename, double Echelle, int Marge, QWidget *parent)
    : QMainWindow(parent)
{
    setUnifiedTitleAndToolBarOnMac(true);
    QMenu *_filemenu = menuBar()->addMenu("&Fichier");
    QAction *quit = new QAction ("&Quit", this);
    _filemenu -> addAction(quit);
    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));

    setGeometry(0,0,500,500);


    QIcon *p = new QIcon(QPixmap(fileopen));

    _toolbar = addToolBar("Toolbar");
    _toolbar->setFocusPolicy(Qt::NoFocus);
    _toolbar->setGeometry(0,0,500,100);


    _combo = new QComboBox(_toolbar);
    _combo->setFocusPolicy(Qt::NoFocus);
    for(std::vector<std::string>::const_iterator i=list_filename.begin(); i<list_filename.end(); ++i)
        _combo->insertItem(0, i->c_str());
    _toolbar->addWidget(_combo);

    QAction *act = new QAction(*p, "&Ouvrir", _toolbar);
    act->setStatusTip("Ouvrir un nouveau Fichier");
    connect(act, SIGNAL(triggered()), this, SLOT(open()));
    _toolbar -> addAction(act);

    //_toolbar->addWidget(_x);
    //    _x->setUsesTextLabel(true);
    //_x->setFocusPolicy(Qt::NoFocus);

    _mylcd = new QLCDNumber(3, _toolbar);
    _mylcd->display((int)(Echelle * 100));
    _mylcd->setFocusPolicy(Qt::NoFocus);
    _toolbar->addWidget(_mylcd);

    //    _slider = new QSlider(0, 100, 1, (int)(Echelle * 100),
    //    Qt::Horizontal, _toolbar, "Zoom");
    _slider = new QSlider(Qt::Horizontal, _toolbar);
    _slider -> setObjectName("Zoom");
    _slider -> setMinimum(0);
    _slider -> setMaximum(100);
    _slider -> setPageStep(1);
    _slider -> setValue((int)(Echelle * 100));
    _slider->setFocusPolicy(Qt::NoFocus);
    _toolbar->addWidget(_slider);

    _scrollview = new QScrollArea(this);

    _scrollview->setFocusPolicy(Qt::NoFocus);

    _dessin = new Dessin(list_filename[0], Echelle, Marge, _scrollview);
    _dessin->setFocusPolicy(Qt::NoFocus);

    QGridLayout *layout = new QGridLayout(_dessin);

    _dessin->setLayout(layout);
    _scrollview->setWidget(_dessin);
    _scrollview->setGeometry(0, menuBar()->height() + _toolbar->height(),
                             width(), height() - menuBar()->height() - _toolbar->height());

    QObject::connect(_slider, SIGNAL(sliderMoved(int)), _dessin, SLOT(zoom(int)));
    QObject::connect(_slider, SIGNAL(sliderMoved(int)), _mylcd, SLOT(display(int)));
    QObject::connect(_combo, SIGNAL(activated(const QString &)), _dessin, SLOT(reload(const QString&)));

}

xvWindows::~xvWindows()
{
    
}
