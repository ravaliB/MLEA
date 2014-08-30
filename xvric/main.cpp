#include "xvwindows.h"
#include <QApplication>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <unistd.h>

using namespace std;

int biomet_input_format = std::ios_base::xalloc ();

void
usage(char*     progname)
{
  std::cout << "Usage: " << progname
            <<  " [-meh] signaturefile [signaturefile2 ...]" << std::endl;
  std::cout << " Options:" << std::endl;
  std::cout << "\t-m: margin (int)" << std::endl;
  std::cout << "\t\tdefault: 20 pts" << std::endl;
  std::cout << "\t-e: scale" << std::endl;
  std::cout << "\t\t0: lowest" << std::endl;
  std::cout << "\t\t1: highest (default)" << std::endl;
  std::cout << "\t-h: help (this message)" << std::endl;

  std::cout << std::endl;
  std::cout << "Controls:" << std::endl;
  std::cout << "\tspace:     next signature" << std::endl;
  std::cout << "\tbackspace: previous signature" << std::endl;
  std::cout << "\tq:         quit" << std::endl;
  std::cout << "\t<:         zoom out" << std::endl;
  std::cout << "\t>:         zoom in" << std::endl;
  std::cout << std::endl;
}

int main(int ac, char *av[])
{
    if (ac < 2)
        {
            usage(av[0]);
            exit(0);
        }

    QApplication a(ac, av);
    std::vector<std::string> list_file;
      int marge = 20;
      double echelle = 1.;
      char c;

      while ((c = getopt(ac, av, "m:e:h")) != EOF)
        {
          switch (c) {
          case 'm':
            marge = atoi(optarg);
            break;
          case 'e' :
            echelle = atof(optarg);
            break;
          case 'h' :
            {
              usage(av[0]);
              exit (0);
            }
          }
        }

      if (optind == ac)
        {
          std::cerr << "Missing argument" << std::endl;
          usage(av[0]);
          exit (-1);
        }
      for(;optind < ac; ++optind)
        list_file.insert(list_file.end(), av[optind]);

    xvWindows w(list_file, echelle, marge);
    w.show();
    
    return a.exec();
}
