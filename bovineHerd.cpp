#include "bovineHerd.h"
#include "genesisVisitor.h"
#include "reporter.h"
#include "random.h"
#include "classes.h"

//Create empty BovineHerd.  Set the offset to 0 -- i.e., the newborns go at the beginning
//of the list of BovineGroups.
BovineHerd::BovineHerd() :
	offset(0) {}			

//Age bovine herd by 1 month.
void BovineHerd::live() {

	iterator end = this->end();							//Pointer to end of BovineGroup list
	for(iterator it = this->begin(); it != end; ++it)	//loop through list and age each BovineGroup
		it->live();

	//Aggragate the current final bin into the new final bin (ie. the bin just before
	//the current final bin.
	operator()(Bovine::BEEF,       Bovine::MALE,   Bovine::END_AGE - 1).addBovineGroup(
    operator()(Bovine::BEEF,       Bovine::MALE,   Bovine::END_AGE));
	operator()(Bovine::BEEF,       Bovine::FEMALE, Bovine::END_AGE - 1).addBovineGroup(
    operator()(Bovine::BEEF,       Bovine::FEMALE, Bovine::END_AGE));

	operator()(Bovine::BEEFREPRO,  Bovine::MALE,   Bovine::END_AGE - 1).addBovineGroup(
	operator()(Bovine::BEEFREPRO,  Bovine::MALE,   Bovine::END_AGE));
	operator()(Bovine::BEEFREPRO,  Bovine::FEMALE, Bovine::END_AGE - 1).addBovineGroup(
    operator()(Bovine::BEEFREPRO,  Bovine::FEMALE, Bovine::END_AGE));
	
	operator()(Bovine::DAIRY,      Bovine::MALE,   Bovine::END_AGE - 1).addBovineGroup(
    operator()(Bovine::DAIRY,      Bovine::MALE,   Bovine::END_AGE));
	operator()(Bovine::DAIRY,      Bovine::FEMALE, Bovine::END_AGE - 1).addBovineGroup(
    operator()(Bovine::DAIRY,      Bovine::FEMALE, Bovine::END_AGE));

	//The bovines have been aged and the final two bins have been aggragated.
	//Set the offset of year 0 to the previous final bin
	offset++;
	offset %= Bovine::END_AGE + 1;

	//The bovines have been aged in place by shifting the offset of year 0.  However,
	//the final bovine group, now represented by year 0, still has an age of Bovine::END_AGE.
	//Thus, we must reset their age to 0 so that GenesisVisitor obtains the proper information
    operator()(Bovine::BEEF,      Bovine::MALE,   0) = BovineGroup(Bovine(Bovine::BEEF,      Bovine::MALE,   0), 0);
    operator()(Bovine::BEEF,      Bovine::FEMALE, 0) = BovineGroup(Bovine(Bovine::BEEF,      Bovine::FEMALE, 0), 0);
    operator()(Bovine::BEEFREPRO, Bovine::MALE,   0) = BovineGroup(Bovine(Bovine::BEEFREPRO, Bovine::MALE,   0), 0);
    operator()(Bovine::BEEFREPRO, Bovine::FEMALE, 0) = BovineGroup(Bovine(Bovine::BEEFREPRO, Bovine::FEMALE, 0), 0);
	operator()(Bovine::DAIRY,     Bovine::MALE,   0) = BovineGroup(Bovine(Bovine::DAIRY,     Bovine::MALE,   0), 0);
    operator()(Bovine::DAIRY,     Bovine::FEMALE, 0) = BovineGroup(Bovine(Bovine::DAIRY,     Bovine::FEMALE, 0), 0);

	//Now allow GenesisVisitor to initialize the appropriate population for the newborns
	//at year 0.
    operator()(Bovine::BEEF,       Bovine::MALE,   0).accept(GenesisVisitor());
    operator()(Bovine::BEEF,       Bovine::FEMALE, 0).accept(GenesisVisitor());
    operator()(Bovine::BEEFREPRO,  Bovine::MALE,   0).accept(GenesisVisitor());
    operator()(Bovine::BEEFREPRO,  Bovine::FEMALE, 0).accept(GenesisVisitor());
	operator()(Bovine::DAIRY,      Bovine::MALE,   0).accept(GenesisVisitor());
    operator()(Bovine::DAIRY,      Bovine::FEMALE, 0).accept(GenesisVisitor());
}

//Return BovineGroup containing bovine
BovineGroup& BovineHerd::operator()(Bovine const& bovine) {
	return operator()(bovine.getType(), bovine.getGender(), bovine.getAge());
}

//Return BovineGroup containing bovines with specified type, gender, and age
BovineGroup& BovineHerd::operator()(Type type, Gender gend, int age) {
	int modulus = Bovine::END_AGE + 1;
	int agePosition = (age + modulus - offset) % modulus;
	return myBovineGroups[type][gend][agePosition];
}

//Return the first element of the BovineGroup list in the BovineHerd
BovineHerd::iterator BovineHerd::begin() {
	return iterator(Bovine::BEEF, Bovine::MALE, 0, this);
}

//Return the last element of the BovineGroup list
BovineHerd::iterator BovineHerd::end() {
	return iterator(Bovine::END_TYPE, Bovine::END_GENDER, Bovine::END_AGE , this);
}

//Import infected animals
void BovineHerd::import(Bovine const& myBovine, 
						double importRate,
						int distType,
						int ageInfected, 
						int ageSlaughter) {
	iterator end = this->end();
	Random& generator = *(Random::instance());

	int numCases;
	for(iterator it = this->begin(); it != end; ++it) {
		if (it->getBovine().getType() == myBovine.getType() &&
			it->getBovine().getGender() == myBovine.getGender() &&
			it->getBovine().getAge() == myBovine.getAge()) {
			if (distType == Tokens::DistType::CONSTANT) {
				numCases = importRate;
			}
			if (distType == Tokens::DistType::POISSON) {
				numCases = generator.nextPoisson(importRate);
			}
			while (numCases--) {
				it->addBovine(SickBovine(myBovine,ageInfected,ageSlaughter));
																//Add one sick bovine
				Reporter::instance()->							//Notify reporter
					reportInfectSource(Reporter::FIXED);
			}
		}
	}
}
