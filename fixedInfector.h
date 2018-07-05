#ifndef FIXED_INFECTOR_H
#define FIXED_INFECTOR_H

#include "visitor.h"
#include "bovine.h"
#include "FunctTable.h"
#include "FunctInterStep.h"

#include <iosfwd>

class BovineHerd;
class BovineGroup;
class FixedInfector;
std::istream& operator>>(std::istream&, FixedInfector&);

class FixedInfector : public Visitor {
	friend std::istream& operator>>(std::istream&, FixedInfector&);
	public:
		virtual void visit(BovineHerd*);
		virtual void visit(BovineGroup*);
		
	protected:
		static void readError();
		static FunctTable<Bovine::Type, FunctTable<Bovine::Gender, FunctInterStep<int, int> > > numCases;
};

#endif