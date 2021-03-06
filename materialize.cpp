#include "materialize.h"
#include "splitter.h"
#include "random.h"
#include "utilities.h"
#include <valarray>

using std::cerr;
using std::endl;

// Table with 2 entries -- 1) Months since infection, 2) Total id50s in bovine.
// Note that the time since infection is scaled to correspond to the incubation 
// period specified for a particular anaimal
FunctInterStep<int, double> Materializer::totalInfectivity;

// Table with 3 entries -- 1) Months since infection, 2) Organ, and 3) Proportion of
// total animal infectivity in that organ at that time.
FunctInterStep<int, FunctTable<Organ::Type, double> > Materializer::organDistribution;

//Takes a reference to a sick bovine and returns a vector of organs
std::vector<Organ> Materializer::process(SickBovine& bovine) {

	//Allocate a result vector.  Initialize vector to size = END_TYPE with each 
	//entry in vector containing the default organ generated by Organ() constructor
	std::vector<Organ> result(Organ::END_TYPE, Organ());

	//Agescale is the ratio of the time between infection and death to the number of 
	//entries in the table defining total infectivity by months since infection
	double ageScale = static_cast<double>(totalInfectivity.table.size()) / bovine.getincPeriod();

	//Compute infectivity for this animal given time since infection
	double total = totalInfectivity(int(bovine.getMonthsSick() * ageScale));

	//Compute fraction of infectivity in each organ
	for(int i = 0; i < Organ::END_TYPE; ++i) {
		Organ res;
		res.type 				= Organ::Type(i);
		res.age  				= bovine.getAge();
		res.sourceClinical		= bovine.isClinical;
		res.sourceNonAmbulatory = bovine.isNonAmbulatory;
		res.ID50				= organDistribution(int(bovine.getMonthsSick() * ageScale))(res.type) * total;
		res.forHumanFood		= false;
		result[i]				= res;
	}
	return result;
}

istream& operator>> (istream& stream, Materializer& me) {

	string word;
	stream >> word;
	char dump[256];

		while((word != "</materializer>") && !stream.eof()) {
			if(word == "<totalInfectivity>") stream.getline(dump, 256).getline(dump, 256) >> me.totalInfectivity >> word;
			else if(word == "<organDistribution>") stream.getline(dump, 256).getline(dump, 256) >> me.organDistribution >> word;
			else Materializer::readError();
			stream >> word;
		}	if(word != "</materializer>") Materializer::readError();
	return stream;
}

void Materializer::readError() {
	cerr << "Error reading Materializer data" << endl;
	exit(0);
}
