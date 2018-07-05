// Author  : Keith H. Duggar
// Date    : 2001.01.27
// Purpose : BovineHerd contains an array of BovineGroup objects indexed by   //
// type, gender, and a position that has values running from 0 to END_AGE+1.  //
// The BovineGroup for age n has position = age + (END_AGE+1) - offset) %     //
// (END_AGE+1).  The value of offset is incremented each month, which has the //
// effect of aging each BovineGroup without having to physically move its     //
// location in the list.
////////////////////////////////////////////////////////////////////////////////
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
000000000100000000020000000003000000000400000000050000000006000000000700000000*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BOVINE_HERD
#define BOVINE_HERD

#include "bovine.h"
#include "bovineGroup.h"
#include "bovineHerdIterator.h"
#include "visitor.h"
#include <iostream>


class BovineHerd {
	public:
		typedef BovineHerdIterator<BovineHerd> iterator;
		typedef BovineGroup value_type;

	private:
		typedef Bovine::Type   Type;
		typedef Bovine::Gender Gender;

	public:
		BovineHerd();
		BovineGroup& operator()(Bovine const&);
		BovineGroup& operator()(Type, Gender, int age);
		iterator begin();
		iterator end();

		virtual void live();
		virtual void accept(Visitor& visitor) {visitor.visit(this);}

	public:
		void import(Bovine const&, double, int, int, int);

	private:
		int offset;
		BovineGroup myBovineGroups[Bovine::END_TYPE  ]
		                          [Bovine::END_GENDER]
								  [Bovine::END_AGE +1];
};

#endif