#include "signature.h"
#include "printer.h"

using namespace std;

int biomet_input_format = ios_base::xalloc();

int main(int argc, char *argv[])
{
  Signature sign;
  vector<Points> data;
  Printer p;

  if (argc < 2)
    {
      cout << "veuillez mettre 2 arguments" << endl;
    }
  else
    {
      sign.load(argv[1]);
      data = sign.getData();
      p.addSignature("OLD SIGNATURE", data);

      
      sign.centrage(data);
      //sign.rotate(data);

      p.addSignature("NEW SIGNATURE", data);
      p.print();

      sign.save(argv[2], data);
    }

  return 0;
}
