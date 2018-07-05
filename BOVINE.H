// Author  : Keith H. Duggar
// Date    : 2001.01.27
// Purpose : Bovine represents a fundamental bovine unit.  Its functionality
//           is simple.  First, it provides a namespace for the properties that
//           a bovine can have.  These include type and gender.  Every bovine
//           also maintains an age.  This represents the age of the animal in
//           in months.  Newborns are considered to be 0 months old.  Second,
//           bovine provides a function liveFor which is meants to carry out
//           the life processess of a bovine for a number of months.  Bovine
//           simply increments its age.  However, the function should be
//           overriden by subclasses to perform appropriate behavior.
//
//           As a final note, bovine must be constructed with a starting type,
//           gender, and age.  Type and gender cannot thereafter change (since
//           it would be illogical in this implementation) and age can only be
//           changed indirectly through the liveFor function.
//
////////////////////////////////////////////////////////////////////////////////
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
000000000100000000020000000003000000000400000000050000000006000000000700000000*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BOVINE_H
#define BOVINE_H

#include <istream>
#include "visitor.h"

class Bovine;
std::istream& operator>>(std::istream&, Bovine&);

class Bovine {
	friend std::istream& operator>>(std::istream&, Bovine&);

	public:
		enum Type   { BEEF, BEEFREPRO, DAIRY , END_TYPE  };
		enum Gender { MALE, FEMALE, END_GENDER};
		enum        { END_AGE = 256           };

		Bovine();
		Bovine(Type, Gender, int);
		virtual Bovine& operator=(Bovine const&);

		int    getAge   (void) const {return myAge   ;}
		Type   getType  (void) const {return myType  ;}
		Gender getGender(void) const {return myGender;}

		virtual void live  (); //Carry out life processess for a month
		virtual void accept(Visitor& visitor); //Accept a visitor

		virtual bool operator<(Bovine const&); //For use in containers

	protected:
		int    myAge;
		Type   myType;
		Gender myGender;
};

std::istream& operator>>(std::istream&, Bovine::Type&);
std::istream& operator>>(std::istream&, Bovine::Gender&);

#endif