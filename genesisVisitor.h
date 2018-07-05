#ifndef GENESIS_VISITOR_H
#define GENESIS_VISITOR_H

#include "visitor.h"
#include "bovine.h"
#include "bovineHerd.h"
#include "bovineGroup.h"
#include "functTable.h"
#include "functInterStep.h"

#include <istream>

class GenesisVisitor;
std::istream& operator>>(std::istream&, GenesisVisitor&);

class GenesisVisitor : public Visitor {
	friend std::istream& operator>>(std::istream&, GenesisVisitor&);

	public:
		virtual void visit(BovineHerd*);
		virtual void visit(BovineGroup*);
	protected:
		static void readError();
		static FunctTable<Bovine::Type, FunctTable<Bovine::Gender, FunctInterStep<int, int> > > initSize;
};

#endif