#ifndef DEATH_VISITOR_H
#define DEATH_VISITOR_H

#include "visitor.h"
#include "bovineHerd.h"
#include "bovineGroup.h"
#include "sickBovine.h"
#include "functInterStep.h"

#include <istream>
#include <list>

class DeathVisitor;
std::istream& operator>>(std::istream&, DeathVisitor&);

class DeathVisitor : public Visitor {
	friend std::istream& operator>>(std::istream&, DeathVisitor&);

	public:
		virtual void visit(BovineHerd*);
		virtual void visit(BovineGroup*);
	protected:
		static void readError();
		static FunctInterStep<int, double> probDeath;
	public:
		typedef std::list<SickBovine> deadBovineCont;
		deadBovineCont deadBovines;
};

#endif