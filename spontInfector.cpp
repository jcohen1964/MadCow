#include "SpontInfector.h"
#include "bovine.h"
#include "sickBovine.h"
#include "Random.h"
#include "utilities.h"
#include "reporter.h"
#include <iostream>

//Takes one argument (age in months) and returns the spontaneous incidence rate
FunctTable<int, double> SpontInfector::spontRate;

//Visit each bovine group with a non-zero spontaneous infection rate, determine 
//number of bovines to become infected, and create the right number of sick bovines
void SpontInfector::visit(BovineHerd* herd) {
	Random& generator = *(Random::instance());			//Create single instance of random generator
	BovineHerd::iterator end = herd->end();				//Alias end of herd BovineGroup list
	for(BovineHerd::iterator it =						//Loop through herd list of BovineGroups
		herd->begin(); it != end; ++it) {
		int age = it->getBovine().getAge();				//Get age for this BovineGroup
		double rate =									//Lookup spontaneous incidence rate
			spontRate((it->getBovine()).getAge());
		if (rate > 0) {									//If spontaneous rate exceeds zero
			int numHealthy = it->getNumHealthy();		//Get number of healthy animals in this group
			int numNewSick =							//Use binomial dist to randomly choose number to be infected 
				generator.nextBin(rate, numHealthy);
			while (numNewSick--) {						//While number to be made sick remains > 0
				Bovine* removed =						//Randomly remove a healthy bovine from group, store in removed
					it->removeRandomBovine();   
				it->addBovine(SickBovine(*removed));	//Add the removed bovine to the SickBovine list
				delete removed;							//Clean up
				Reporter::instance()->					//Notify reporter of spontaneous infection
					reportInfectSource(Reporter::SPONTANEOUS);
			}
		}
	}
}

#include <string>
#include <iostream>

void SpontInfector::readError() {
	std::cerr << "Error reading SpontInfector data" << std::endl;
	exit(0);
}

std::istream& operator>>(std::istream& stream, SpontInfector& me) {
	std::string word;
	stream >> word;

	char dump[256];

	while((word != "</spontInfector>") && !stream.eof()) {
		if     (word == "<spontRate>"    ) stream.getline(dump, 256).getline(dump, 256) >> me.spontRate  >> word;
		else SpontInfector::readError();
		stream >> word;
	} if(word != "</spontInfector>") SpontInfector::readError();

	return stream;
}

