#include "signature.h"
#include "printer.h"

using namespace std;

int biomet_input_format = ios_base::xalloc();

int main(int argc, char *argv[])
{
  Signature sign;
  vector<Points> data1;
  vector<Points> data2;
  Printer p;

  double threshold = 0;
  double res = 0;

  if (argc < 2)
    {
      cout << "veuillez mettre 2 arguments" << endl;
    }
  else
    {
      sign.load(argv[1]);
      data1 = sign.getData();

      sign.load(argv[2]);
      data2 = sign.getData();


      //p.addSignature("OLD SIGNATURE", data);

      
      sign.rotate(data1);
      sign.centrage(data1);

      sign.rotate(data2);
      sign.centrage(data2);

      res = sign.DTW(data1, data2);

  //    std::cout << "Res : " << res << std::endl; 

      //p.addSignature("NEW SIGNATURE", data);
      //p.print();

      //sign.save(argv[2], data1);
    }

  return 0;
}
