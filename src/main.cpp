#include "signature.h"

using namespace std;

int biomet_input_format = ios_base::xalloc();

int main(int argc, char *argv[])
{
  Signature sign;
  vector<Points> data;
 
  if (argc < 2)
    {
      cout << "veuillez mettre 2 arguments" << endl;
    }
  else
    {
      sign.load(argv[1]);
      data = sign.getData();
      sign.save(argv[2], data);
    }

  return 0;
}
