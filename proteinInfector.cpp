#include <iostream>
#include "proteinInfector.h"
#include "bovine.h"
#include "sickBovine.h"
#include "feedGroup.h"
#include "random.h"
#include "reporter.h"
#include "utilities.h"
#include <iostream>

int ProteinInfector::numCowsReceiving;			//Number of cattle among which one pkt of infectivity is divided
FunctInterLine<double, double>					//Dose-response function for infectivity
	ProteinInfector::probInfection;
FunctTable<Bovine::Type, FunctTable<Bovine::	//Relative consumption rate table for protein
	Gender, FunctInterStep<int, double> > > ProteinInfector::consumption;
FunctInterStep<int, double> 
	ProteinInfector::susceptibility;			//Relative susceptibility to infection (multiplies probInfection)

ProteinInfector::ProteinInfector() :
	numTarget(0),								//Total number of animals expcsed to contaminated meal
	curTarget(0),								//Index into the doses array
	doses(0) {}									//Vector containing numTarget entries.  Each entry has id50s going to
												//one bovine.

//Returns relative consumption rate for a bovine group
double ProteinConsumptionAdapter(BovineGroup const& group) {
	Bovine bovine = group.getBovine();
	return ProteinInfector::consumption(bovine.getType())(bovine.getGender())(bovine.getAge() % 24) * group.getNumTotal();
}

//Distributes infectivity from MBM among members of herd and creates the resulting infected bovines.
void ProteinInfector::visit(BovineHerd* herd) {

	curTarget = 0;
	if(numTarget > 0) {							//If total number of exposed animals > 0
	std::vector<BovineHerd::iterator> selected;	//Vector of BovineGroups with numTargets entries, one each for each 
												//exposed bovine. Note -- a bovineGroup can appear multiple times 
												//in this vector.
	double (*weight)(BovineGroup const&) =		//weight function retrieves the consumption weight for bovineGroup
		ProteinConsumptionAdapter;

	selected = KHD_UTIL::PoissonSelectMulti		//Get the list of bovineGroups containing exposed bovines.
		                     (herd->begin(), herd->end(), weight, numTarget);

	std::vector<BovineHerd::iterator>::			//Iterator and end marker for vector of bovineGroups with exposed
		iterator it, end;						//bovines
	end = selected.end();
	for(it = selected.begin();					//Iterate through vector of selected bovineGroups
		it != selected.end(); ++it) {			
		(*it)->accept(*this);					//BovineGroup accepts the proteinInfector, which then visits it
		curTarget++;							
	}}

	numTarget = 0;
	curTarget = 0;
}

//Determines if one bovine in BovineGroup becomes infected as a result of being exposed to
//doses[curTarget] id50s.  Takes into account susceptibility for bovines in this group.
void ProteinInfector::visit(BovineGroup* group) {
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
		delete removed;												//free up space
		Reporter::instance()->reportInfectSource(Reporter::PROTEIN);//Notify reporter of animal becoming infected
		doses[curTarget] = 0;										//Set doses array entry to zero
	}
}

//Identifies feed pkts that will be fed to cattle.  
void ProteinInfector::distributeInfectivity(FeedGroup& feedGroup) {

	for(FeedGroup::iterator it = feedGroup.begin();				//loop through feed pkts
		it != feedGroup.end(); ++it) {
		if(it->id50>0 && it->destination == Feed::FARM && (		//If id50>0, destination is FARM and pkt 
			it->material == Feed::NP_FEED ||					//is NP_FEED or POULTRY_FEED
			it->material == Feed::POULTRY_FEED)) {				
			ProteinInfector::addBolus(it->id50);				//Put pkt infectivity in doses array
		}
		if (it->id50>0 && it->destination == Feed::FARM			//If id50>0, destination is FARM, pkt is  
			&& it->material == Feed::P_FEED &&					//P_FEED, and it is misfed
			it->misFeed) {
			ProteinInfector::addBolus(it->id50);				//Put pkt of infectivity in doses array
			Reporter::instance()->								//Notify reporter of misfeeding
				reportid50Track(Reporter::ID50_MISFEED, it->id50);
		}
		if (it->id50>0 && it->destination == Feed::FARM			//If id50>0, destination is FARM, pkt is
			&& it->material == Feed::P_FEED &&					//P_FEED, and it is NOT misfed
			!it->misFeed) {					
			Reporter::instance()->								//Notify reporter of id50s leaving
				reportid50Track(Reporter::ID50_OUTAFTERFEEDPROD, it->id50);
		}
	}
}

//Distribute id50s from one feed pkt among numCowsReceiving cells in the doses array
void ProteinInfector::addBolus(double dose) {

	numTarget += numCowsReceiving;
	if(numTarget > doses.size()) doses.resize(numTarget);
	for(int i = 0; i < numCowsReceiving; ++i) {
		doses[curTarget] = dose / numCowsReceiving;
		curTarget++;
	}
}

#include <string>
#include <iostream>

void ProteinInfector::readError() {
	std::cerr << "Error reading ProteinInfector data" << std::endl;
	exit(0);
}

std::istream& operator>>(std::istream& stream, ProteinInfector& me) {
	std::string word;
	stream >> word;

	char dump[256];

	while((word != "</proteinInfector>") && !stream.eof()) {
		if     (word == "<numCowsReceiving>") stream >> me.numCowsReceiving >> word;
		else if(word == "<probInfection>"   ) stream.getline(dump, 256).getline(dump, 256) >> me.probInfection >> word;
		else if(word == "<consumption>"     ) stream.getline(dump, 256).getline(dump, 256) >> me.consumption >> word;
		else if(word == "<susceptibility>"  ) stream.getline(dump, 256).getline(dump, 256) >> me.susceptibility >> word;
		else ProteinInfector::readError();
		stream >> word;
	} if(word != "</proteinInfector>") ProteinInfector::readError();

	return stream;
}

