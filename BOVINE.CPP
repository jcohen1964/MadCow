#include "bovine.h"

Bovine::Bovine() :
	myAge(0),
	myType(BEEF),
	myGender(MALE) {}

Bovine::Bovine(Type type, Gender gender, int age) :
	myAge(age),
	myType(type),
	myGender(gender) {}

Bovine& Bovine::operator=(Bovine const& bovine) {
	myAge    = bovine.myAge;
	myType   = bovine.myType;
	myGender = bovine.myGender;
	return *this;
}

void Bovine::live() {
	myAge++;
	if(myAge > END_AGE) myAge = END_AGE;
}

void Bovine::accept(Visitor& visitor) {
	visitor.visit(this);
}

bool Bovine::operator<(Bovine const& x) {

	return (((myType   <  x.myType  )  ) ||
		    ((myType   == x.myType  )&&
			 (myGender <  x.myGender)  ) ||
		    ((myType   == x.myType  )&&
			 (myGender == x.myGender)&&
			 (myAge    <  x.myAge   )  )   );
}


#include <string>
using std::string;

std::istream& operator>>(std::istream& stream, Bovine& bovine) {
	stream >> bovine.myType >> bovine.myGender >> bovine.myAge;
	return stream;
}
std::istream& operator>>(std::istream& stream, Bovine::Type& type) {
	string    token;
	stream >> token;
	if     (token == "BEEF" )     type = Bovine::BEEF;
	else if(token == "BEEFREPRO") type = Bovine::BEEFREPRO;
	else if(token == "DAIRY")     type = Bovine::DAIRY;
	return stream;
}

std::istream& operator>>(std::istream& stream, Bovine::Gender& gender) {
	string    token;
	stream >> token;
	if     (token == "MALE"  ) gender = Bovine::MALE;
	else if(token == "FEMALE") gender = Bovine::FEMALE;
	return stream;
}