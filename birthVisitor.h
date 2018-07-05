#ifndef BIRTH_VISITOR_H
#define BIRTH_VISITOR_H

#include "visitor.h"
#include "bovineGroup.h"
#include "bovineHerd.h"
#include "sickBovine.h"

#include <istream>

class BirthVisitor;
std::istream& operator>>(std::istream&, BirthVisitor&);

class BirthVisitor : public Visitor {
	friend std::istream& operator>>(std::istream&, BirthVisitor&);

	public:
		virtual void visit(BovineHerd*);
		virtual void visit(BovineGroup*);

	protected:
		static void readError();

		static int    beginCalving;
		static int    endCalving;
		static double probBirth;
		static double probTrans;

		BovineGroup groupMaleAdditions;
		BovineGroup groupFemaleAdditions;
};

#endif