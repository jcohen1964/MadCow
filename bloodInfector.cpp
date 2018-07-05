#include <iostream>
#include "bloodInfector.h"
#include "bovine.h"
#include "sickBovine.h"
#include "random.h"
#include "reporter.h"
#include "utilities.h"

int BloodInfector::numCowsReceiving;			//Number of cattle among which one pkt of infectivity is divided		
FunctInterLine<double, double>					//Dose-response function for infectivity
	BloodInfector::probInfection;
FunctTable<Bovine::Type, FunctTable<Bovine::	//Relative consumption rate table for blood meal
	Gender, FunctInterStep<int, double> > >  BloodInfector::consumption;
FunctInterStep<int, double> 
	BloodInfector::susceptibility;				//Relative susceptibility to infection (multiplies probInfection)

BloodInfector::BloodInfector() :
	numTarget(0),								//Total number of animals exposed to contaminated blood meal
	curTarget(0),								//Index into the doses array
	doses(0) {}									//Vector containing numTarget entries.  Each entry has id50s going to
												//one bovine

//Returns the relative consumption rate for a bovine group
double BloodConsumptionAdapter(BovineGroup const& group) {
	Bovine bovine = group.getBovine();
	return BloodInfector::consumption(bovine.getType())(bovine.getGender())(bovine.getAge()) * group.getNumTotal();
}

//Distributes infectivity from blood meal among members of herd and creates the resulting infected bovines.
void BloodInfector::visit(BovineHerd* herd) {

	curTarget = 0;
	std::vector<BovineHerd::iterator> selected;	//Vector of BovineGroups with numTargets entries, one for each
												//exposed bovine.  Note -- a BovineGroup can appear multiple
												//times in this vector.
	double (*weight)(BovineGroup const&) =		//weight function retrieves the consumption weight for bovineGroup
		BloodConsumptionAdapter;

	selected = KHD_UTIL::PoissonSelectMulti		//Get the list of bovineGroups containing exposed bovines.
		                     (herd->begin(), herd->end(), weight, numTarget);

	std::vector<BovineHerd::iterator>::			//Iterator and end marker for vector of bovineGroups with exposed
		iterator it, end;						//bovines
	end = selected.end();
	for(it = selected.begin();					//Iterate through vector of selected bovineGroups
		it != end; ++it) {
		(*it)->accept(*this);					//BovineGroup accepts the proteinInfector, which then visits it
 		curTarget++;
	}

	numTarget = 0;
	curTarget = 0;
}

//Determines if one bovine in BovineGroup becomes infected as a result of being exposed to 
//doses[curTarget] id50s.  Takes into account susceptibility for bovines in this group.
void BloodInfector::visit(BovineGroup* group) {
	Random& generator = *(Random::instance());
	Reporter::instance()->											//Notify reporter of id50s sent to cattle
		reportid50Track(Reporter::ID50_TOCATTLE, doses[curTarget]);
	if(generator.nextFloat() <=										//If animal becomes infected
		probInfection(doses[curTarget] * susceptibility(group->getBovine().getAge()))) {
		Bovine* removed = group->removeRandomBovine();				//Remove a randomly selected bovine from this group
		group->addBovine(SickBovine(
			*removed,
			doses[curTarget] * susceptibility(group->getBovine().getAge())));						
																	//Put that bovine in the mySickBovines list
		delete removed;												//Free up space
		Reporter::instance()->reportInfectSource(Reporter::BLOOD);	//Notify reporter of animal becoming infected
		doses[curTarget] = 0;										//Set the doses array entry to zero.
	}
}

//IDentifies blood meal pkts that will be fed to cattle
void BloodInfector::distributeInfectivity(FeedGroup& feedGroup) {

	for(FeedGroup::iterator it =								//loop through the blood meal pkts
		feedGroup.begin(); it != feedGroup.end(); ++it) {
		if(it->id50>0 && it->destination == Feed::FARM &&		//If id50>0, destination is FARM, and
			it->material == Feed::BLOOD) {						//material is blood meal
			BloodInfector::addBolus(it->id50);					//Put pkt of infectivity into the doses array
		}
	}
}

//Distributes id50s from one blood meal pkt among numCowsReceiving cells in the doses array
void BloodInfector::addBolus(double dose) {

	numTarget += numCowsReceiving;
	if(numTarget > doses.size()) doses.resize(numTarget);
	for(int i = 0; i < numCowsReceiving; ++i) {
		doses[curTarget] = dose / numCowsReceiving;
		curTarget++;
	}
}

#include <string>
#include <iostream>

void BloodInfector::readError() {
	std::cerr << "Error reading BloodInfector data" << std::endl;
	exit(0);
}

std::istream& operator>>(std::istream& stream, BloodInfector& me) {
	std::string word;
	stream >> word;

	char dump[256];

	while((word != "</bloodInfector>") && !stream.eof()) {
		if     (word == "<numCowsReceiving>") stream >> me.numCowsReceiving >> word;
		else if(word == "<probInfection>"   ) stream.getline(dump, 256).getline(dump, 256) >> me.probInfection >> word;
		else if(word == "<consumption>"     ) stream.getline(dump, 256).getline(dump, 256) >> me.consumption >> word;
		else if(word == "<susceptibility>"  ) stream.getline(dump, 256).getline(dump, 256) >> me.susceptibility >> word;
		else BloodInfector::readError();
		stream >> word;
	} if(word != "</bloodInfector>") BloodInfector::readError();

	return stream;
}
