#include "sickBovine.h"
#include "bovineGroup.h"
#include "reporter.h"
#include "random.h"
#include "AMInspector.h"
#include <string>
#include <limits>
#include <iostream>


int const SickBovine::NONE = std::numeric_limits<int>::max();	//Initialize the constant static member NONE
FunctDistrDisc<int, int> SickBovine::incPeriod;				//Table of time until clinical and probabilities
FunctDistrDisc<int, int> SickBovine::clinicalDuration;			//Table of duratino clinical and probabilities
FunctTable<SickBovine::ClinicalStatus, double>					//Probabilities that animal is non-ambulatory
			SickBovine::probNonAmbulatory;						//as a function of clinical status.
double SickBovine::maternalContagiousPoint=1;					//Fraction of clinical duration that must pass
																//before mother becomes contageous to calf
bool SickBovine::doseDependentIncPeriod;						//True means incubation period depends on infect dose

//Constructor with no arguments -- Create default bovine and use it as a template
SickBovine::SickBovine() :
	Bovine(),					
	myAgeSick(myAge),			
	myMonthsSick(0),
	isClinical(false),
	isContagious(false),
	dieOfBSE(false),
	myGroup(0) {
		myIncPeriod = incPeriod(0);
		myContagiousDelay = myIncPeriod*maternalContagiousPoint;
		myClinicalDuration = clinicalDuration(0);
		myTerminationDelay = myIncPeriod + myClinicalDuration;
		myAgeSlaughter = Bovine::END_AGE+1;
		Random& generator = *(Random::instance());
		isNonAmbulatory_NOSIGNS = (generator.nextFloat() <= probNonAmbulatory(NOSIGNS));
		isNonAmbulatory_CLINICAL = 
				getNonAmbulatory_CLINICAL(probNonAmbulatory(NOSIGNS),
										  probNonAmbulatory(CLINICAL));
		isNonAmbulatory = isNonAmbulatory_NOSIGNS;
	}

//Constructor using a bovine object as a template
SickBovine::SickBovine(Bovine const & bovine) :
	Bovine(bovine),
	myAgeSick(myAge),
	myMonthsSick(0),
	isClinical(false),
	isContagious(false),
	dieOfBSE(false),
	myGroup(0) {
		myIncPeriod = incPeriod(0);
		myContagiousDelay = myIncPeriod*maternalContagiousPoint;
		myClinicalDuration = clinicalDuration(0);
		myTerminationDelay = myIncPeriod + myClinicalDuration;
		myAgeSlaughter = Bovine::END_AGE+1;
		Random& generator = *(Random::instance());
		isNonAmbulatory_NOSIGNS = (generator.nextFloat() <= probNonAmbulatory(NOSIGNS));
		isNonAmbulatory_CLINICAL = 
				getNonAmbulatory_CLINICAL(probNonAmbulatory(NOSIGNS),
										  probNonAmbulatory(CLINICAL));
		isNonAmbulatory = isNonAmbulatory_NOSIGNS;
	}

//Constructor using a bovine object as a template along with specification of 
//age when infected and age at slaughter
SickBovine::SickBovine(Bovine const & bovine, int ageInfected, int ageSlaughter) :
	Bovine(bovine),
	myAgeSick(ageInfected),
	myMonthsSick(myAge - ageInfected),
	myGroup(0) {
		myIncPeriod = incPeriod(0);
		myContagiousDelay = myIncPeriod*maternalContagiousPoint;
		myClinicalDuration = clinicalDuration(0);
		myTerminationDelay = myIncPeriod + myClinicalDuration;
		myAgeSlaughter = ageSlaughter;
		isClinical = (myAge >= ageInfected+myIncPeriod);
		isContagious = (myAge >= ageInfected+myContagiousDelay);
		dieOfBSE = (myAge >= ageInfected+myTerminationDelay);
		Random& generator = *(Random::instance());
		isNonAmbulatory_NOSIGNS = (generator.nextFloat() <= probNonAmbulatory(NOSIGNS));
		isNonAmbulatory_CLINICAL = 
				getNonAmbulatory_CLINICAL(probNonAmbulatory(NOSIGNS),
										  probNonAmbulatory(CLINICAL));
		if(isClinical)  {isNonAmbulatory = isNonAmbulatory_CLINICAL;}
		if(!isClinical) {isNonAmbulatory = isNonAmbulatory_NOSIGNS;};
	}



//Constructor using a bovine object as a template, with infection dose specified;
SickBovine::SickBovine(Bovine const & bovine, double infectDose) :
	Bovine(bovine),
	myAgeSick(myAge),
	myMonthsSick(0),
	isClinical(false),
	isContagious(false),
	dieOfBSE(false),
	myGroup(0) {
		if (doseDependentIncPeriod) {
			myIncPeriod = ComputeIncPeriod(infectDose);
		} else {
			myIncPeriod = incPeriod(0);
		}
		myContagiousDelay = myIncPeriod*maternalContagiousPoint;
		myClinicalDuration = clinicalDuration(0);
		myTerminationDelay = myIncPeriod + myClinicalDuration;
		myAgeSlaughter = Bovine::END_AGE+1;
		Random& generator = *(Random::instance());
		isNonAmbulatory_NOSIGNS = (generator.nextFloat() <= probNonAmbulatory(NOSIGNS));
		isNonAmbulatory_CLINICAL = 
				getNonAmbulatory_CLINICAL(probNonAmbulatory(NOSIGNS),
										  probNonAmbulatory(CLINICAL));
		isNonAmbulatory = isNonAmbulatory_NOSIGNS;
	}


//Constructor using a sickbovine as a template
SickBovine::SickBovine(SickBovine const & sickBovine) :
	Bovine(sickBovine),
	myAgeSick(sickBovine.myAgeSick),
	myMonthsSick(sickBovine.myMonthsSick),
	myIncPeriod(sickBovine.myIncPeriod),
	myContagiousDelay(sickBovine.myContagiousDelay),
	myClinicalDuration(sickBovine.myClinicalDuration),
	myTerminationDelay(sickBovine.myTerminationDelay),
	myAgeSlaughter(sickBovine.myAgeSlaughter),
	isClinical(sickBovine.isClinical),
	isContagious(sickBovine.isContagious),
	isNonAmbulatory(sickBovine.isNonAmbulatory),
	isNonAmbulatory_NOSIGNS(sickBovine.isNonAmbulatory_NOSIGNS),
	isNonAmbulatory_CLINICAL(sickBovine.isNonAmbulatory_CLINICAL),
	dieOfBSE(sickBovine.dieOfBSE),
	myGroup(sickBovine.myGroup) {}


//Constructor using specified Bovine type, gender, and age
SickBovine::SickBovine(Type type, Gender gender, int age) :
	Bovine(type, gender, age),
	myAgeSick(age),
	myMonthsSick(0),
	isClinical(false),
	isContagious(false),
	dieOfBSE(false),
	myGroup(0) {
		myIncPeriod = incPeriod(0);
		myContagiousDelay = myIncPeriod*maternalContagiousPoint;
		myClinicalDuration = clinicalDuration(0);
		myTerminationDelay = myIncPeriod + myClinicalDuration;
		myAgeSlaughter = Bovine::END_AGE+1;
		Random& generator = *(Random::instance());
		isNonAmbulatory_NOSIGNS = (generator.nextFloat() <= probNonAmbulatory(NOSIGNS));
		isNonAmbulatory_CLINICAL = 
				getNonAmbulatory_CLINICAL(probNonAmbulatory(NOSIGNS),
										  probNonAmbulatory(CLINICAL));
		isNonAmbulatory = isNonAmbulatory_NOSIGNS;
	}

//Constructor using specified Bovine type, gender, age, age when became sick, and months sick
SickBovine::SickBovine(Type type, Gender gender, int age, int ageSick, int monthsSick) :
	Bovine(type, gender, age),
	myAgeSick(ageSick),
	myMonthsSick(monthsSick),
	isClinical(false),
	isContagious(false),
	dieOfBSE(false),
	myGroup(0) {
		myIncPeriod = incPeriod(0);
		myContagiousDelay = myIncPeriod*maternalContagiousPoint;
		myClinicalDuration = clinicalDuration(0);
		myTerminationDelay = myIncPeriod + myClinicalDuration;
		myAgeSlaughter = Bovine::END_AGE+1;
		Random& generator = *(Random::instance());
		isNonAmbulatory_NOSIGNS = (generator.nextFloat() <= probNonAmbulatory(NOSIGNS));
		isNonAmbulatory_CLINICAL = 
				getNonAmbulatory_CLINICAL(probNonAmbulatory(NOSIGNS),
										  probNonAmbulatory(CLINICAL));
		isNonAmbulatory = isNonAmbulatory_NOSIGNS;
	}

//Assignment operator
SickBovine& SickBovine::operator=(SickBovine const& bovine) {
	Bovine::operator=(bovine);
	myAgeSick = bovine.myAgeSick;
	myMonthsSick = bovine.myMonthsSick;
	myIncPeriod = bovine.myIncPeriod;
	myContagiousDelay = bovine.myContagiousDelay;
	myClinicalDuration = bovine.myClinicalDuration;
	myTerminationDelay = bovine.myTerminationDelay;
	myAgeSlaughter = bovine.myAgeSlaughter;
	isClinical = bovine.isClinical;
	isContagious = bovine.isContagious;
	isNonAmbulatory = bovine.isNonAmbulatory;
	isNonAmbulatory_NOSIGNS = isNonAmbulatory_NOSIGNS;
	isNonAmbulatory_CLINICAL = isNonAmbulatory_CLINICAL;
	dieOfBSE = bovine.dieOfBSE;
	return *this;
}

//Age SickBovine one month
void SickBovine::live() {
	bool startClinical = isClinical;
	myAge++;
	myMonthsSick++;
	if(myAge > END_AGE) myAge = END_AGE;
	if(myMonthsSick >= myIncPeriod) {	//If SickBovine reaches myIncPeriod
		isClinical = true;					//Make isClinical true
	}
	if(myMonthsSick >= myContagiousDelay) {	//If SickBovine reaches myContagiousDelay
		isContagious = true;				//Make isContageious true
	}
	if(myMonthsSick >= myTerminationDelay) {	//If SickBovine reaches myTerminationDelay
		dieOfBSE = true;					//Make dieOfBSE true
	}
	if(!startClinical && isClinical) {				//If animal just became clinical...
		Reporter::instance()->reportNewClinical();	//Notify reporter
		isNonAmbulatory = isNonAmbulatory_CLINICAL;	//Update non-ambulatory status
	}
}

//Determine if bovine is non-ambulatory when clinical.  The determination depends on
//the non-amb probabilities for the stages with no signs (PNS) and when clinical (PC)
//and on whether the bovine is non-amb already prior to developing signs (i.e.,
//isNonAmbulatory_NOSIGNS.  The computation is necessary because the parameter file
//contains the overall probability that a clinical animal is non-amb and the 
//simulation assumes that the animal is non-amb when clincical if a U[0,1] < PCStar
//or if the bovine was non-amb when showing no signs.
bool SickBovine::getNonAmbulatory_CLINICAL(double PNS, double PC) {
	double PCStar;	//Probability that makes non-amb probability when clinical 
					//come out to be PC.
	PCStar = (1-(1-PC)/(1-PNS));
	Random& generator = *(Random::instance());	
	return (isNonAmbulatory_NOSIGNS || (generator.nextFloat() <= PCStar));
}

//Determine date (months after infection) at manifestation of clinical signs based 
//on dose causing infection.  Uses relationship from G. Wells presentation
//that showed time until manifestation of clinical signs was: 1) 283 months for 
//0.01 ID50s, 123 months for 0.1 ID50s, 60 months for 1 ID50, 35 months for 10
//ID50s, 25 months for 100 ID50s, and 22 months for 1,000 ID50s.
int SickBovine::ComputeIncPeriod(double infectDose) {
	return 19.5+pow((10000/infectDose),(1/2.43));
}


//Comparison operator
bool SickBovine::operator==(SickBovine const& bovine) {
	return((myAge == bovine.myAge) &&
		   (myType == bovine.myType) &&
		   (myGender == bovine.myGender) &&
		   (myAgeSick == bovine.myAgeSick) &&
		   (myMonthsSick == bovine.myMonthsSick) &&
		   (myIncPeriod == bovine.myIncPeriod) &&
		   (myContagiousDelay == bovine.myContagiousDelay) &&
		   (myClinicalDuration == bovine.myClinicalDuration) &&
		   (myTerminationDelay == bovine.myTerminationDelay) &&
		   (myAgeSlaughter == bovine.myAgeSlaughter) &&
		   (isClinical == bovine.isClinical) &&
           (isContagious == bovine.isContagious) &&
		   (isNonAmbulatory == bovine.isNonAmbulatory) &&
		   (isNonAmbulatory_NOSIGNS == isNonAmbulatory_NOSIGNS) &&
		   (isNonAmbulatory_CLINICAL == isNonAmbulatory_CLINICAL) &&
		   (dieOfBSE == bovine.dieOfBSE) &&
		   (myGroup == bovine.myGroup));
}



//Read in parameter values...
std::istream& operator>> (std::istream& stream, SickBovine& me) {

	std::string word;
	char dump[256];
	stream >> word;

	while((word != "</sickBovine>") && !stream.eof()) {
		if     (word == "<incPeriod>")               stream.getline(dump, 256).getline(dump, 256) >> me.incPeriod         >> word;
		else if(word == "<clinicalDuration>")        stream.getline(dump, 256).getline(dump, 256) >> me.clinicalDuration  >> word;
		else if(word == "<probNonAmbulatory>")       stream.getline(dump, 256).getline(dump, 256) >> me.probNonAmbulatory >> word; 
		else if(word == "<maternalContagiousPoint>") stream >> me.maternalContagiousPoint >> word;
		else if(word == "<doseDependentIncPeriod>" ) stream >> me.doseDependentIncPeriod  >> word;
		else SickBovine::readError();
		stream >> word;
	} if(word != "</sickBovine>") SickBovine::readError();
	return stream;
}

std::istream& operator>>(std::istream& stream, SickBovine::ClinicalStatus& clinicalStatus) {
	std::string    token;
	stream >> token;
	if     (token == "NOSIGNS"   ) clinicalStatus = SickBovine::NOSIGNS;
	else if(token == "CLINICAL"  ) clinicalStatus = SickBovine::CLINICAL;
	else {std::cerr << "SickBovine::clinicalStatus " << token << " unknown"; exit(0);}
	return stream;
}

std::istream& operator>>(std::istream& stream, SickBovine::AmbStatus& ambStatus) {
	std::string    token;
	stream >> token;
	if     (token == "NORMAL"  ) ambStatus = SickBovine::NORMAL;
	else if(token == "NONAMB"  ) ambStatus = SickBovine::NONAMB;
	else {std::cerr << "SickBovine::ambStatus " << token << " unknown"; exit(0);}
	return stream;
}


// error function utility
void SickBovine::readError() {
	std::cerr << "Error reading SickBovine data" << std::endl;
	exit(0);
}
