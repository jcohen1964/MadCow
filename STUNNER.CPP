#include "stunner.h"
#include "splitter.h"
#include "random.h"
#include "utilities.h"
#include <valarray>

using std::cerr;
using std::endl;

//Takes stunner type and returns weight representing probability of choosing
//that type
FunctTable<Stunner::Type, double> Stunner::probType;

//Takes stunner type and returns probability that the stunner operates properly
FunctTable<Stunner::Type, double> Stunner::probOK;

//Takes stunner type, stunner operating status, organ type, and returns emboli
//object.  Emboli object contains distribution of fraction of brain sent as emboli 
//to that organ.
FunctTable<Stunner::Type, FunctTable<Stunner::Status, FunctTable<Organ::Type, Emboli> > > Stunner::emboli;

FunctTable<Stunner::Type, double> Stunner::probDrip;
double Stunner::fracDrip;

//Takes organ vector and distributes emboli
void Stunner::process(std::vector<Organ>& organs) {
	Random& generator = *(Random::instance()); //Alias random singleton object

	Type type;

	//Choose stunner type
	double unif = generator.nextFloat();
	if(unif <= probType(BOLT)) type = BOLT;
	else type = PNEUMO;

	//Determine whether stunner is operating properly
	Status status = OK;
	if(generator.nextFloat() > probOK(type)) status = NOK;

	//Distribute emboli to the various organs in the organ vector.
	std::vector<Organ>::iterator it, end;
	end = organs.end();
	for(it = organs.begin(); it != end; ++it) {						//iterate over organs
		Emboli emb = emboli(type)(status)(it->type);				//get emboli distribution
		if(generator.nextFloat() <= emb.prob) {						//If there are emboli
			it->emboli = true;										//Mark indicator true
			double embFrac = KHD_UTIL::pickLogUnif(emb.lo, emb.hi);	//Randomly select pct of brain in emboli
			it->ID50 += organs[Organ::BRAIN].ID50 * embFrac;		//Calc ID50s in emboli
			organs[Organ::BRAIN].ID50 *= 1 - embFrac;				//Remove those id50s from brain
		} else {													//If no emboli
			it->emboli = false;										//Mark indicator false
		}
	}

	//If braindrip occurs and the blood is NOT chosen for human consumption, then
	//move fracDrip of Brain ID50s to blood
	if (generator.nextFloat() <= probDrip(type) && 
		!organs[Organ::BLOOD].forHumanFood) {
		organs[Organ::BLOOD].ID50 += fracDrip*organs[Organ::BRAIN].ID50;
		organs[Organ::BRAIN].ID50 *= 1-fracDrip;
	}
};

istream& operator>> (istream& stream, Stunner& me) {

	string word;
	stream >> word;
	char dump[256];

		while((word != "</stunner>") && !stream.eof()) {
			if(word == "<emboli>") stream.getline(dump, 256).getline(dump, 256) >> me.emboli >> word;
			else if(word == "<probType>")  stream.getline(dump, 256).getline(dump, 256) >> me.probType >> word;
			else if(word == "<probOK>") stream.getline(dump, 256).getline(dump, 256) >> me.probOK >> word;
			else if(word == "<probDrip>") stream.getline(dump, 256).getline(dump, 256) >> me.probDrip >> word;
			else if(word == "<fracDrip>") stream >> me.fracDrip >> word;
			else Stunner::readError();
			stream >> word;
		}	if(word != "</stunner>") Stunner::readError();
	return stream;
}

void Stunner::readError() {
	cerr << "Error reading stunner data" << endl;
	exit(0);
}

istream& operator>>(istream& stream, Stunner::Type& type) {
	std::string    token;
	stream >> token;
	if     (token == "BOLT"  ) type = Stunner::BOLT;
	else if(token == "PNEUMO") type = Stunner::PNEUMO;
	else {std::cerr << "Stunner::Type " << token << " uknown"; exit(0);}
	return stream;
}

istream& operator>>(istream& stream, Stunner::Status& status) {
	std::string    token;
	stream >> token;
	if     (token == "OK"  ) status = Stunner::OK;
	else if(token == "NOK") status = Stunner::NOK;
	else {std::cerr << "Stunner::Status " << token << " uknown"; exit(0);}
	return stream;
}

