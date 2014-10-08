#ifndef PARSER_H
#define PARSER_H

using namespace std;

class Parser
{
public:
	Parser();
	~Parser();

	vector<string> getFiles_1();
	vector<string> getFiles_2();

	void parse(string input);
	void writeOutput(vector<double> scores, string output);

private:
	vector<string> files_1;
	vector<string> files_2;	
};

#endif /* PARSER_HH */