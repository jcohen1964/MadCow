#include <string>
#include "SRMInspector.h"
#include "random.h"
#include "reporter.h"

using std::cerr;
using std::endl;
using std::string;

//Returns fraction of infectivity eliminated by SRM ban as a function of age, organ
FunctInterStep<int, 
				FunctTable<Organ::Type, 
				FunctDistrDisc<double, double> > > SRMInspector::fracElimBySRM;

//Eliminate infectivity in tissues rejected by the SRM inspector
void SRMInspector::process(std::vector<Organ>& organs) {
	std::vector<Organ>::iterator iterator;  //Allocate iterator over organs
	Random& generator = *(Random::instance()); //Alias random singleton object
	std::vector<Organ>::iterator end = organs.end(); //Cache last organ location

	//Determine fraction of materials to be eliminated by the SRM ban.  Eliminate
	//the infectivity by the amount specified and report that elimination.
	for(iterator = organs.begin(); iterator != end; ++iterator) {
		double fracElim = fracElimBySRM(iterator->age)(iterator->type)(0);
		if (fracElim > 0) {
			Reporter::instance()->reportid50Track(Reporter::ID50_FAILSRM, 
				fracElim*iterator->ID50);
			iterator->ID50 = iterator->ID50*(1-fracElim);
		}
	}
}

void SRMInspector::readError() {
	cerr << "Error reading SRMInspector data" << endl;
	exit(0);
}

istream& operator>> (istream& stream, SRMInspector& me) {

	string word;
	stream >> word;
	char dump[256];

	if(word == "<fracElimBySRM>") {
		stream.getline(dump, 256).getline(dump, 256) >> me.fracElimBySRM;
	} else SRMInspector::readError();
	stream >> word;
	if(word != "</fracElimBySRM>") SRMInspector::readError();
	stream >> word;
	if(word != "</SRMInspector>") SRMInspector::readError();

	return stream;
}