#ifndef TOTAL_CLINICAL_VISITOR_H
#define TOTAL_CLINICAL_VISITOR_H

#include "visitor.h"
#include "bovineHerd.h"
#include "bovineGroup.h"
#include "sickBovine.h"

struct TotalClinicalVisitor : public Visitor {

	TotalClinicalVisitor() : total(0) {}
	virtual void visit(BovineHerd*);
	virtual void visit(BovineGroup*);
	virtual void reset() {total = 0;}
	int total;
};

#endif

