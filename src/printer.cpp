#include <vector>
#include <stdlib.h>
#include <iostream>
#include "printer.h"

#define NUM_COMMANDS 4

Printer::Printer()
{

}

Printer::~Printer()
{

}

void Printer::writeSignature(std::string file, std::vector<Points> data)
{
	FILE *temp = fopen(file.c_str(), "w+");

	for (int i = 0; i < data.size(); ++i)
		fprintf(temp, "%f %f \n", data[i].PosX, data[i].PosY);

	fclose(temp);
}

void Printer::addSignature(std::string title, std::vector<Points> data)
{
	int numSignal = files.size() + 1;

	std::string file = "log/data" + std::to_string(numSignal) + ".temp";

	std::vector<std::string> command = {"set title \"" + title + "\"",
								"set xlabel \"X\"",
								"set ylabel \"Y\"",
								"plot '" + file + "' with lines"};

	FILE *pipe = new FILE;

	files.push_back(file);
	commands.push_back(command);
	pipes.push_back(pipe);

	writeSignature(file, data);
}

void Printer::print()
{
	for (int i = 0; i < (int)files.size(); i++)
	{
		pipes[i] = popen("gnuplot -persistent", "w");

		for (int j = 0; j < NUM_COMMANDS; j++)
			fprintf(pipes[i], "%s \n", commands[i][j].c_str());
	}
}