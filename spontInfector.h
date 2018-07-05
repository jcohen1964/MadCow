#ifndef SPONT_INFECTOR_H
#define SPONT_INFECTOR_H

#include "visitor.h"
#include "bovineHerd.h"
#include "bovineGroup.h"
#include "FunctInterStep.h"
#include "FunctDistrDisc.h"

#include <istream>
#include <vector>

class SpontInfector;
std::istream& operator>>(std::istream&, SpontInfector&);
double SpontConsumptionAdapter(BovineGroup const&);

class SpontInfector : public Visitor {
	friend std::istream& operator>>(std::istream&, SpontInfector&);
	friend double SpontConsumptionAdapter(BovineGroup const&);
	public:
		virtual void visit(BovineHerd*);
		
	protected:
		static void readError();
		static FunctTable<int, double> spontRate;
};

#endif