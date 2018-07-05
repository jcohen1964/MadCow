#include <string>
#include "PMInspector.h"
#include "random.h"

using std::cerr;
using std::endl;
using std::string;

//Returns prob of passing PM inspection as function of age, emboli present, and organ type
FunctInterStep<int,	FunctTable<bool, FunctTable<Organ::Type, double> > > PMInspector::probPassPM;

//Takes reference to organ vector and flags each organ as passing or failing PM inspection
void PMInspector::process(std::vector<Organ>& organs) {
	std::vector<Organ>::iterator iterator;     // allocate iterator over organ vector
	Random& generator = *(Random::instance()); // alias to random singleton object

	std::vector<Organ>::iterator end = organs.end(); //cache location of last organ in vector

	//Loop through organs and flag each as passing or failing PM inspection
	for(iterator = organs.begin(); iterator != end; ++iterator) {
		iterator->passPM = (generator.nextFloat() <=
							   probPassPM(iterator->age)(iterator->emboli)(iterator->type));
	}
}

void PMInspector::readError() {
	cerr << "Error reading PMInspector data" << endl;
	exit(0);
}

istream& operator>> (istream& stream, PMInspector& me) {

	string word;
	stream >> word;
	char dump[256];

	if(word == "<probPassPM>") {
		stream.getline(dump, 256).getline(dump, 256) >> me.probPassPM;
	} else PMInspector::readError();
	stream >> word;
	if(word != "</probPassPM>") PMInspector::readError();
	stream >> word;
	if(word != "</PMInspector>") PMInspector::readError();
	return stream;
}