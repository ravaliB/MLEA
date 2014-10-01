#include "signature.h"
#include "printer.h"
#include "parser.h"
#include "points.h"

using namespace std;

int biomet_input_format = ios_base::xalloc();

int usage()
{
	cout << "USAGE : ./compSign [FILE]" << endl;
	cout << "\tEach line of [FILE] must be written in the following format :" << endl;
	cout << "\t\\path\\to\\file1.txt \\path\\to\\file2.txt" << endl;

	return -1;
}

int main(int argc, char *argv[])
{
	Signature sign;
  	Printer pr;
  	Parser pa;
  	
  	vector<Points> data1;
  	vector<Points> data2;
  	vector<string> files_1;
  	vector<string> files_2;

  	double threshold = 0;
  	double res = 0;

  	if (argc != 2)
    	usage();
  	else
    {
    	pa.parse(argv[1]);
    	files_1 = pa.getFiles_1();
    	files_2 = pa.getFiles_2();

    	if (files_1.size() != files_2.size())
    		usage();
    	else
    	{
    		for (int i = 0; i < files_1.size(); ++i)
    		{
    			sign.loadSignatures(files_1[i], files_2[i]);
    			sign.reduction();
    			sign.normalization();
    			sign.getCharacteristics();
    			sign.computeScore();
    			sign.reset();
    		}

    		pa.writeOutput(sign.getScores());
    	}
    }

  return 0;
}
