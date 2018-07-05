#include "deathVisitor.h"
#include "random.h"

//Returns cattle monthly natural death incidence as a function of age in months
FunctInterStep<int, double> DeathVisitor::probDeath;

//Loops through BovineGroups in BovineHerd and kills both healthy and BSE-infected
//cattle using the death incidence rate returned by probDeath.
void DeathVisitor::visit(BovineHerd* herd) {
	BovineHerd::iterator end = herd->end();
	for(BovineHerd::iterator it = herd->begin(); it != end; ++it)
		it->accept(*this);
}

//Kills both health and BSE-infected cattle in a BovineGroup using the death
//incidence rate returned by probDeath
void DeathVisitor::visit(BovineGroup* group) {
	Random& generator = *(Random::instance());					//Alias random number singleton
	BovineGroup::SickBovineIterator end = group->getSickEnd();	//Alias end of SickBovine list
	for(BovineGroup::SickBovineIterator it =					//Loop through all sick bovines in this bovineGroup
		group->getSickBegin(); it != end; ++it) {
		if(generator.nextFloat() <=								//If this infected Bovine dies this month
			probDeath(group->getBovine().getAge()) ||			//of BSE or any other cause
           (it->dieOfBSE)) {
			deadBovines.push_back(*it);							//Put the bovine on the deadBovines list
			it = group->removeBovine(it);						//Remove the bovine from the BovineGroup.
		}
	}
	int numDie =												//Kill uninfected bovines
		group->getNumHealthy()*probDeath(group->getBovine().getAge());
	group->removeBovine(numDie);
}

#include <string>
#include <iostream>

void DeathVisitor::readError() {
	std::cerr << "Error reading DeathVisitor data" << std::endl;
	exit(0);
}

std::istream& operator>>(std::istream& stream, DeathVisitor& me) {
	std::string word;
	stream >> word;

	if(word == "<probDeath>") {
		stream >> me.probDeath;
	} else DeathVisitor::readError();
	stream >> word;
	if(word != "</probDeath>") DeathVisitor::readError();
	stream >> word;
	if(word != "</deathVisitor>") DeathVisitor::readError();

	return stream;
}