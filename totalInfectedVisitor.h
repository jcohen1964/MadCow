#ifndef TOTAL_INFECTED_VISITOR_H
#define TOTAL_INFECTED_VISITOR_H

#include "visitor.h"
#include "bovineHerd.h"

struct TotalInfectedVisitor : public Visitor {
	TotalInfectedVisitor() : total(0) {}
	virtual void visit(BovineHerd*);
	virtual void reset() {total = 0;}
	int total;
};

#endif