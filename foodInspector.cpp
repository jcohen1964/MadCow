#include <string>
#include "FoodInspector.h"
#include "random.h"
#include "reporter.h"

using std::cerr;
using std::endl;
using std::string;

// Table with two entries -- 1) Organ, 2) Probability that inspector oks organ for 
// human consumption
// FunctTable<Organ::Type, double> FoodInspector::probPassFood;
FunctInterStep<int, FunctTable<Organ::Type, double> > FoodInspector::probPassFood;

void FoodInspector::process(std::vector<Organ>& organs) {
	std::vector<Organ>::iterator iterator;			// allocate iterator to the organs
	Random& generator = *(Random::instance());		// assigns alias for random singleton object
	std::vector<Organ>::iterator end = organs.end(); //cache the location of the last organ

	//Loops through organs... determine if each organ goes to human food.  If
	//organ goes to food, notify reporter and then zero out infectivity.
	for(iterator = organs.begin(); iterator != end; ++iterator) {
		iterator->forHumanFood = (generator.nextFloat() <=
							      probPassFood(iterator->age)(iterator->type) &&
								  iterator->passPM);
		if(iterator->forHumanFood) { //Notify reporter of id50s to humans
			Reporter::instance()->reportid50Track(Reporter::ID50_TOHUMANS, iterator->ID50);
			Reporter::instance()->reportid50toHumans(iterator->type,
														iterator->age,
														iterator->sourceClinical,
														iterator->sourceNonAmbulatory,
														iterator->ID50);
			iterator->ID50 = 0;
		}

	}
}

void FoodInspector::readError() {
	cerr << "Error reading foodInspector data" << endl;
	exit(0);
}

istream& operator>> (istream& stream, FoodInspector& me) {

	string word;
	stream >> word;
	char dump[256];

	if(word == "<probPassFood>") {
		stream.getline(dump, 256).getline(dump, 256) >> me.probPassFood;
	} else FoodInspector::readError();
	stream >> word;
	if(word != "</probPassFood>") FoodInspector::readError();
	stream >> word;
	if(word != "</foodInspector>") FoodInspector::readError();

	return stream;
}