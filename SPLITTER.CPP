#include <string>
#include "splitter.h"
#include "random.h"
#include "utilities.h"
#include <vector>

using std::cerr;
using std::endl;
using std::string;

//Practice flags (NNN, NNY, etc.) refer to the slaughter practice applied to an animal.  
//The first entry indicates whether there was a mis-split, the second indictes whether 
//there is AMR, and the third indicates whether the spinal cord is removed.

//Returns fraction of spinal cord in muscle, AMRMeat and Bone as a function of age and 
//slaughter practice flags (mis-split, AMR, and whether spinal cord is removed)
FunctInterStep<int, FunctTable<Splitter::Combo, double> > Splitter::fracSCInMuscle;
FunctInterStep<int, FunctTable<Splitter::Combo, double> > Splitter::fracSCInAMRMeat;
FunctInterStep<int, FunctTable<Splitter::Combo, double> > Splitter::fracSCInBone;

//Returns fraction of dorsal route ganglia in muscle, AMRMeat, and bone as a function of 
//age and slaughter practice flags 
FunctInterStep<int, FunctTable<Splitter::Combo, double> > Splitter::fracDRGInMuscle;
FunctInterStep<int, FunctTable<Splitter::Combo, double> > Splitter::fracDRGInAMRMeat;
FunctInterStep<int, FunctTable<Splitter::Combo, double> > Splitter::fracDRGInBone;

//Returns probability weight proportional to the occurence of each combination of 
//slaughter practice flags
FunctInterStep<int, FunctTable<Splitter::Combo, double> > Splitter::probMS_AMR_SCRemove;

//Redistributes infectivity among organs resulting from aerosolization
void Splitter::process(std::vector<Organ>& organs) {

	Random& generator = *(Random::instance()); //alias to random singleton object

	//Create vector of slaughter practice combinations
	std::vector<Combo> choices(END_COMBO);
	choices[NNN] = NNN;
	choices[YNN] = YNN;
	choices[NYN] = NYN;
	choices[YYN] = YYN;
	choices[NNY] = NNY;
	choices[YNY] = YNY;
	choices[NYY] = NYY;
	choices[YYY] = YYY;

	//select practice combination based on age of animal from which organs were taken
	//(organ(0).age).  Uses Poisson selector.
	std::vector<Combo>::iterator outcome;
	outcome = KHD_UTIL::PoissonSelect(choices.begin(), choices.end(), probMS_AMR_SCRemove(organs[0].age));

	//Takes fracAerosol of spinal infectivity and transfers to muscle
	organs[Organ::MUSCLE].ID50 += organs[Organ::SPINAL].ID50 * fracAerosol;
	organs[Organ::SPINAL].ID50 *= 1 - fracAerosol;

	//Store id50s in spinal cord and DRG...
	double id50InSC  = organs[Organ::SPINAL].ID50;
	double id50InDRG = organs[Organ::DRG].ID50;

	//Move some SC and DRG infectivity to human food (muscle, AMRMeat, and bone)...
	organs[Organ::MUSCLE].ID50  += id50InSC  * fracSCInMuscle(organs[0].age)(*outcome);
	organs[Organ::AMRMEAT].ID50 += id50InSC  * fracSCInAMRMeat(organs[0].age)(*outcome);
	organs[Organ::BONE].ID50    += id50InSC  * fracSCInBone(organs[0].age)(*outcome);
	organs[Organ::MUSCLE].ID50  += id50InDRG * fracDRGInMuscle(organs[0].age)(*outcome);
	organs[Organ::AMRMEAT].ID50 += id50InDRG * fracDRGInAMRMeat(organs[0].age)(*outcome);
	organs[Organ::BONE].ID50    += id50InDRG * fracDRGInBone(organs[0].age)(*outcome);

	//Remove what was moved from DRG and SC...
	organs[Organ::SPINAL].ID50 -= id50InSC * (
		fracSCInMuscle(organs[0].age)(*outcome)  +
		fracSCInAMRMeat(organs[0].age)(*outcome) +
		fracSCInBone(organs[0].age)(*outcome));
	organs[Organ::DRG].ID50 -= id50InDRG * (
		fracDRGInMuscle(organs[0].age)(*outcome)  +
		fracDRGInAMRMeat(organs[0].age)(*outcome) +
		fracDRGInBone(organs[0].age)(*outcome));
}

void Splitter::readError() {
	cerr << "Error reading splitter data" << endl;
	exit(0);
}

istream& operator>> (istream& stream, Splitter& me) {

	string word;
	stream >> word;
	char dump[256];

	while((word != "</splitter>") && !stream.eof()) {
		if(word == "<fracAerosol>") stream >> me.fracAerosol >> word;
		else if(word == "<fracSCInMuscle>")      stream.getline(dump, 256).getline(dump, 256) >> me.fracSCInMuscle      >> word;
		else if(word == "<fracDRGInMuscle>")     stream.getline(dump, 256).getline(dump, 256) >> me.fracDRGInMuscle     >> word;
		else if(word == "<fracSCInAMRMeat>")     stream.getline(dump, 256).getline(dump, 256) >> me.fracSCInAMRMeat     >> word;
		else if(word == "<fracDRGInAMRMeat>")    stream.getline(dump, 256).getline(dump, 256) >> me.fracDRGInAMRMeat    >> word;
		else if(word == "<fracSCInBone>")        stream.getline(dump, 256).getline(dump, 256) >> me.fracSCInBone        >> word;
		else if(word == "<fracDRGInBone>")       stream.getline(dump, 256).getline(dump, 256) >> me.fracDRGInBone       >> word;
		else if(word == "<probMS_AMR_SCRemove>") stream.getline(dump, 256).getline(dump, 256) >> me.probMS_AMR_SCRemove >> word;
		else {std::cout << word; 
		Splitter::readError();}
		stream >> word;
	} if(word != "</splitter>") Splitter::readError();

	return stream;
}

std::istream& operator>>(std::istream& stream, Splitter::Combo& combo) {
	std::string    token;
	stream >> token;
	if     (token == "NNN"  ) combo = Splitter::NNN;
	else if(token == "YNN")   combo = Splitter::YNN;
	else if(token == "NYN")   combo = Splitter::NYN;
	else if(token == "YYN")   combo = Splitter::YYN;
	else if(token == "NNY")   combo = Splitter::NNY;
	else if(token == "YNY")   combo = Splitter::YNY;
	else if(token == "NYY")   combo = Splitter::NYY;
	else if(token == "YYY")   combo = Splitter::YYY;
	else {std::cerr << "Splitter::Combo " << token << " uknown"; exit(0);}
	return stream;
}
