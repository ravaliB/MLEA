#include <vector>
#include <fstream>
#include <string>
#include <fstream>
#include <iostream>
#include "parser.h"

using namespace std;

Parser::Parser()
{
}


Parser::~Parser()
{
}

vector<string> Parser::getFiles_1()
{
	return files_1;
}

vector<string> Parser::getFiles_2()
{
	return files_2;
}

void Parser::parse(string input)
{
	ifstream ifs(input);
	vector<string> elems;
	string str;

	size_t pos = 0;
	int i = 0;

	while (getline(ifs, str))
	{
		pos = str.find(" ");		
		files_1.push_back(str.substr(0, pos));
		files_2.push_back(str.substr(pos + 1, string::npos));
	}

	ifs.close();
}

void Parser::writeOutput(vector<double> scores, string output)
{
	ofstream out(output);

	for (int i = 0; i < scores.size(); ++i)
	{
		out << files_1[i] << " " << files_2[i] << " " << scores[i];

		if (scores[i] < 0)
			out << " F" << endl;
		else
			out << " T" << endl;
	}

	out.close();
}
