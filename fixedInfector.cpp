#include "FixedInfector.h"
#include "bovine.h"
#include "sickBovine.h"
#include "bovineHerd.h"
#include "bovineGroup.h"
#include "reporter.h"
#include <string>
#include <iostream>

//Takes three arguments (bovine type, gender, and age in months) and returns number of infected
//bovines with those characteristics introduced into the herd at the beginning of the simulation
FunctTable<Bovine::Type, FunctTable<Bovine::Gender, FunctInterStep<int, int> > > FixedInfector::numCases;

//Loops through all BovineGroups in the BovineHerd.  Each bovineGroup accepts the fixed infector.
void FixedInfector::visit(BovineHerd* herd) {
	BovineHerd::iterator end = herd->end();
	for(BovineHerd::iterator it = herd->begin(); it != end; ++it) {
		it->accept(*this);
	}
}

//For each BovineGroup, make bovineGroup.numCases additional animals sick.
void FixedInfector::visit(BovineGroup* group) {
	Bovine bovine = group->getBovine();					//Get a represenative bovine from this group
	int num = numCases(bovine.getType())				//Lookup number of fixed cases to be added to
		(bovine.getGender())(bovine.getAge());			//this group at the beginning of the simulation
	while(num--) {										//For each sick animal to be added
		Bovine* removed = group->removeRandomBovine();	//Remove a healthy bovine ???
		group->addBovine(SickBovine(*removed));			//Add one sick bovine
		delete removed;									//Clean up
		Reporter::instance()->							//Notify reporter
			reportInfectSource(Reporter::FIXED);
	}
}

void FixedInfector::readError() {
	std::cerr << "Error reading FixedInfector data" << std::endl;
	exit(0);
}

std::istream& operator>>(std::istream& stream, FixedInfector& me) {
	std::string word;
	stream >> word;

	char dump[256];

	while((word != "</fixedInfector>") && !stream.eof()) {
		if(word == "<numCases>"   ) stream.getline(dump, 256).getline(dump, 256) >> me.numCases >> word;
		else FixedInfector::readError();
		stream >> word;
	} if(word != "</fixedInfector>") FixedInfector::readError();

	return stream;
}

