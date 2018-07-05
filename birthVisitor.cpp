#include "BirthVisitor.h"
#include "bovine.h"
#include "sickbovine.h"
#include "random.h"
#include "reporter.h"

int    BirthVisitor::endCalving   = Bovine::END_AGE;
int    BirthVisitor::beginCalving = 0;
double BirthVisitor::probBirth = 0;
double BirthVisitor::probTrans = 0;

void BirthVisitor::visit(BovineHerd* herd) {
	Random& generator = *(Random::instance());
	
	//Do BEEF
	groupMaleAdditions   =									//Create BovineGroup of BEEF MALE, age 0, 0 cattle
		BovineGroup(Bovine(Bovine::BEEF, Bovine::MALE,   0));
	groupFemaleAdditions = 									//Create BovineGroup of BEEF FEMALE, age 0, 0 cattle
		BovineGroup(Bovine(Bovine::BEEF, Bovine::FEMALE, 0));
	for(int i = beginCalving;								//Loop through the female bovine groups
		(i <= endCalving) && (i <= Bovine::END_AGE); ++i) {
		(*herd)(Bovine::BEEF, Bovine::FEMALE, i).
			accept(*this);									//Check if sick mothers give birth.  Put new births
	}														//in groupMaleAdditions and groupFemaleAdditions
	(*herd)(Bovine::BEEF, Bovine::MALE,   0).				//Add new males in groupMaleAdditions to 
		addBovineGroup(groupMaleAdditions);					//herd BovineGroup age 0 months
	(*herd)(Bovine::BEEF, Bovine::FEMALE, 0).				//Add new females in groupFemaleAdditions to
		addBovineGroup(groupFemaleAdditions);				//herd BovineGroup age 0 months

	//Do BEEFREPRO -- Same as for BEEF
	groupMaleAdditions   = BovineGroup(Bovine(Bovine::BEEFREPRO, Bovine::MALE,   0));
	groupFemaleAdditions = BovineGroup(Bovine(Bovine::BEEFREPRO, Bovine::FEMALE, 0));
	for(int j = beginCalving; (j <= endCalving) && (j <= Bovine::END_AGE); ++j) {
		(*herd)(Bovine::BEEFREPRO, Bovine::FEMALE, j).accept(*this);
	}
	(*herd)(Bovine::BEEFREPRO, Bovine::MALE,   0).addBovineGroup(groupMaleAdditions);
	(*herd)(Bovine::BEEFREPRO, Bovine::FEMALE, 0).addBovineGroup(groupFemaleAdditions);

	//Do DAIRY -- Same as for BEEF
	groupMaleAdditions   = BovineGroup(Bovine(Bovine::DAIRY, Bovine::MALE,   0));
	groupFemaleAdditions = BovineGroup(Bovine(Bovine::DAIRY, Bovine::FEMALE, 0));
	for(int k = beginCalving; (k <= endCalving) && (k <= Bovine::END_AGE); ++k) {
		(*herd)(Bovine::DAIRY, Bovine::FEMALE, k).accept(*this);
	}
	(*herd)(Bovine::DAIRY, Bovine::MALE,   0).addBovineGroup(groupMaleAdditions);
	(*herd)(Bovine::DAIRY, Bovine::FEMALE, 0).addBovineGroup(groupFemaleAdditions);
}

//Loops through members of a BovineGroup and checks if any give birth.  Cattle that are
//born are added to either groupMaleAdditions or groupFemaleAdditions.
void BirthVisitor::visit(BovineGroup* group) {

	Random& generator = *(Random::instance()); //Alias random number singleton
	BovineGroup::SickBovineIterator it;        //Create an iterator over sickBovines
	BovineGroup::SickBovineIterator end = group->getSickEnd(); //Get the end of the sickBovines

	for(it = group->getSickBegin(); it != end; ++it) {			//Loop through all sickBovines
		if(generator.nextFloat() <=								//If mother is contageous and disease
			(probBirth * probTrans) && it->isContagious)		//and disease is passed to calf
			 {
				Bovine bovine = group->getBovine();				//Create a bovine 
				if(generator.nextFloat() < 0.5) {				//If it is a male (50% probability)
					groupMaleAdditions.
						addBovine(SickBovine(bovine.getType(), Bovine::MALE, 0));
				} else {										//If it is a female
					groupFemaleAdditions.
						addBovine(SickBovine(bovine.getType(), Bovine::FEMALE, 0));
				}
				Reporter::										//Notify reporter if calf is infected
					instance()->reportInfectSource(Reporter::MATERNAL);
		}
	}
}

#include <iostream>
#include <string>

//Input functions
std::istream& operator>>(std::istream& stream, BirthVisitor& me) {
	std::string word;
	stream >> word;

	while((word != "</birthVisitor>") && !stream.eof()) {
		if     (word == "<probBirth>"            ) stream >> me.probBirth >> word;
		else if(word == "<probTrans>"            ) stream >> me.probTrans >> word;
		else if(word == "<endCalving>"           ) stream >> me.endCalving >> word;
		else if(word == "<beginCalving>"         ) stream >> me.beginCalving >> word;
		else BirthVisitor::readError();
		stream >> word;
	} if(word != "</birthVisitor>") BirthVisitor::readError();

	return stream;
}

void BirthVisitor::readError() {
	std::cerr << "Error reading BirthVisitor data" << std::endl;
	exit(0);
}