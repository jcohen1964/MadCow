// Author  : Keith H. Duggar
// Date    : 2001.01.27
// Purpose : BovineGroup represents a group bovines having the same age, type,
//           and gender.  Some of the bovines in a group may be sick.  The
//           number of healthy individuals and sick individuals is maintained.
//           In addition, a linked list of the sick bovines is maintained.
//
////////////////////////////////////////////////////////////////////////////////
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
000000000100000000020000000003000000000400000000050000000006000000000700000000*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BOVINE_GROUP_H
#define BOVINE_GROUP_H

#include <list>
#include "bovine.h"
#include "sickBovine.h"
#include "visitor.h"

class BovineGroup {

	public:
		typedef std::list<SickBovine>::iterator SickBovineIterator;
		typedef std::list<SickBovine>::const_iterator const_SickBovineIterator;

	public:
		BovineGroup();
		BovineGroup(BovineGroup const&);
		BovineGroup(Bovine const&, int numTotal = 0);

		BovineGroup& operator=(BovineGroup const&);

	public:
		virtual void accept(Visitor& visitor) {visitor.visit(this);}

		//Simple read only access functions
		Bovine getBovine    (void) const {return myBovine    ;}
		int    getNumTotal  (void) const {return myNumTotal  ;}
		int    getNumHealthy(void) const {return myNumHealthy;}
		int    getNumSick   (void) const {return myNumSick   ;}
		std::list<SickBovine> getSickBovines(void) const {return mySickBovines;}

		//Iterators to the sick bovines
		SickBovineIterator getSickBegin (void) {return mySickBovines.begin();}
		SickBovineIterator getSickEnd   (void) {return mySickBovines.end()  ;}
		const_SickBovineIterator getSickBegin (void) const {return mySickBovines.begin();}
		const_SickBovineIterator getSickEnd   (void) const {return mySickBovines.end()  ;}

	public:
		//Add and remove bovines, sick bovines and random samples
		void        addBovine(int num);
		void        addBovine(Bovine const&);
		void        addBovineGroup(BovineGroup const&);
		Bovine      removeBovine();
		BovineGroup removeBovine(int num);
		Bovine*     removeRandomBovine();
		BovineGroup getBovinesSlaughtered(double fraction);
		BovineGroup removeRandomBovineGroup(int num);
		void		removeBovine(SickBovine);
		SickBovineIterator removeBovine(SickBovineIterator);
		void        live();

	protected:
		Bovine myBovine;
		int    myNumTotal;
		int    myNumHealthy;
		int    myNumSick;

		std::list<SickBovine> mySickBovines;
};

#endif
