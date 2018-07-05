#include <iostream>
#include "MBMTransporter_Dead.h"
#include "Random.h"
#include "reporter.h"
#include "mealGroup.h"
#include "meal.h"

using std::cerr;
using std::endl;

// Table with 4 entries -- 1) producer type, 2) material type, 3) destination, and
// 4) proportion of meal with specified values in (1) and (2) that go to (3) as a 
// destination.

FunctTable<Meal::ProducerType, 
           FunctTable<Meal::MaterialType, 
                      FunctDistrDisc<double,Meal::DestinationType> > >
					  MBMTransporter_Dead::probDestination;

MBMTransporter_Dead::MBMTransporter() {}
	
//Takes pointer to a mealGroup and assigns destinations appropriately.
//May be enhanced to simulate in-transit contamination
void MBMTransporter_Dead::transport(MealGroup& mealGroup) {

	Random& generator = *(Random::instance());

	//Loop through MealGroup
	for(MealGroup::iterator it = mealGroup.begin(); it != mealGroup.end(); ++it) {
		//Select destination
		it->destination = probDestination(it->producer)(it->material)(0);
		if (it->destination == Meal::OUT) {
			Reporter::instance()->reportid50Track(Reporter::ID50_OUTAFTERRENDER, it->id50);
		}
	}
};

void MBMTransporter_Dead::readError() {
	std::cerr << "Error reading MBMTransporter data" << std::endl;
	exit(0);
}


//input function from data stream
istream& operator>> (istream& stream, MBMTransporter_Dead& me) {

	string word;
	char dump[256];
	stream >> word;

	while((word != "</MBMTransporter_Dead>") && !stream.eof()) {
		if     (word == "<probDestination>")      stream.getline(dump, 256).getline(dump, 256) >> me.probDestination >> word;
		else MBMTransporter_Dead::readError();
		stream >> word;
	} if(word != "</MBMTransporter_Dead>") MBMTransporter_Dead::readError();
	return stream;
}


