#ifndef PRINTER_H
#define PRINTER_H

#include "signature.h"
#include <vector>

class Printer
{
	public:
		Printer();
		~Printer();

		void print();
		void writeSignature(std::string file, std::vector<Points> data);
		void addSignature(std::string title, std::vector<Points> data);

	private:
		std::vector<std::string> files;
		std::vector<std::vector<std::string> > commands;
		std::vector<FILE *> pipes;

};

#endif /* PRINTER_HH */