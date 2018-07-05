#include "RandomInfector.h"
#include "bovine.h"
#include "sickBovine.h"
#include "random.h"
#include "reporter.h"
#include "utilities.h"
#include <iostream>

int RandomInfector::numCowsReceiving;			//Number of cattle among which one pkt of infectivity is divided
FunctInterLine<double, double>					//Dose-response function for infectivity
	RandomInfector::probInfection;
FunctTable<Bovine::Type, FunctTable<Bovine::	//Relative consumption rate table for this source of protein
	Gender, FunctInterStep<int, double> > > RandomInfector::consumption;
FunctInterStep<int, double>						
	RandomInfector::susceptibility;				//Relative susceptibility to infection (multiplies probInfection)

RandomInfector::RandomInfector() :
	numTarget(0),								//Total number of animlas exposed to contaminated meal
	curTarget(0),								//Index into the doses array
	doses(0) {}									//Vector containing numTarget entries.  Each entry has id50s going to
												//one bovine.

//Returns relative consumption rate for a bovine group
double RandomConsumptionAdapter(BovineGroup const& group) {
	Bovine bovine = group.getBovine();
	return RandomInfector::consumption(bovine.getType())(bovine.getGender())(bovine.getAge() % 24) * group.getNumTotal();
}

//Distributes infectivity from this source among members of herd and creates the resulting infected bovines
void RandomInfector::visit(BovineHerd* herd) {

	curTarget = 0;
	if(numTarget > 0) {							//If total number of exposed animlas > 0
	std::vector<BovineHerd::iterator> selected;	//Vector of BovineGroups with numTargets entries, one for each
												//exposed bovine. Note -- a bovineGroup can appear multiple times
												//in this vector.
	double (*weight)(BovineGroup const&) =		//weight function retrieves the consumption weight for bovineGroup
		RandomConsumptionAdapter;

	selected = KHD_UTIL::PoissonSelectMulti		//Get the list of bovineGroups containing exposed bovines.
		                     (herd->begin(), herd->end(), weight, numTarget);

	std::vector<BovineHerd::iterator>::			//Iterator and end marker for vector of bovineGroups with exposed
		iterator it, end;						//bovines
	end = selected.end();
	for(it = selected.begin();					//Iterate through vector of selected bovineGroups
		it != end; ++it) {
		(*it)->accept(*this);					//BovineGroup accepts the proteinInfector, which then visits it
		curTarget++;
	}}

	numTarget = 0;
	curTarget = 0;
}

//Determins if one bovine in BovineGropu becomes infected as a rsult of being exposed to
//doses[curTarget] id50s.  Takes into account susceptibility for bovines in this group.
void RandomInfector::visit(BovineGroup* group) {
	Random& generator = *(Random::instance());
	if(generator.nextFloat() <=							//If animal becomes infected
		probInfection(doses[curTarget] * susceptibility(group->getBovine().getAge()))) {
		Bovine* removed = group->removeRandomBovine();	//Remove a randomly selected bovine from this group
		group->addBovine(SickBovine(
			*removed,
			doses[curTarget] * susceptibility(group->getBovine().getAge())));						
																	//Put that bovine in the mySickBovines list
		delete removed;									//free up space
		Reporter::instance()->							//Notify reporter of animal becoming infected
			reportInfectSource(Reporter::EXOGENOUS);	
		doses[curTarget] = 0;							//Set doses array entry to zero
	}
}

//Distribute id50s representing one month of exogenous contamination among numCowsReceiving
//cells in the doses array.
void RandomInfector::addBolus(double dose) {
	numTarget += numCowsReceiving;							
	if(numTarget > doses.size()) doses.resize(numTarget);
	for(int i = 0; i < numCowsReceiving; ++i) {
		doses[curTarget] = dose / numCowsReceiving;
		curTarget++;
	}
}

#include <string>
#include <iostream>

void RandomInfector::readError() {
	std::cerr << "Error reading RandomInfector data" << std::endl;
	exit(0);
}

std::istream& operator>>(std::istream& stream, RandomInfector& me) {
	std::string word;
	stream >> word;

	char dump[256];

	while((word != "</randomInfector>") && !stream.eof()) {
		if     (word == "<numCowsReceiving>") stream >> me.numCowsReceiving >> word;
		else if(word == "<probInfection>"   ) stream.getline(dump, 256).getline(dump, 256) >> me.probInfection >> word;
		else if(word == "<consumption>"     ) stream.getline(dump, 256).getline(dump, 256) >> me.consumption >> word;
		else if(word == "<susceptibility>"  ) stream.getline(dump, 256).getline(dump, 256) >> me.susceptibility >> word;
		else RandomInfector::readError();
		stream >> word;
	} if(word != "</randomInfector>") RandomInfector::readError();

	return stream;
}

