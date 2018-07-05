#ifndef MATERIZLIZE_H
#define Materializer_H

#include <vector>
#include <string>
#include <iostream>
#include "sickBovine.h"
#include "organ.h"
#include "FunctTable.h"
#include "FunctInterStep.h"
#include "emboli.h"

using std::istream;
using std::string;

class Materializer;
istream& operator>> (istream&, Materializer&);

class Materializer {

	friend istream& operator>> (istream&, Materializer&);

	public:
		std::vector<Organ> process(SickBovine&);

	private:
		static void readError();

		static FunctInterStep<int, double> totalInfectivity;
		static FunctInterStep<int, FunctTable<Organ::Type, double> > organDistribution;
};

#endif