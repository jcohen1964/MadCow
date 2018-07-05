#include <string>
#include "AMInspector.h"
#include "random.h"
#include "reporter.h"
#include "sickBovine.h"

using std::cerr;
using std::endl;
using std::string;

// Table has 3 entries -- 
//	1) ambulatory status (NORMAL, NONAMB),
//	2) age, and 
//	3) prob of passing AM inspection based on non-BSE factors;
FunctTable<SickBovine::AmbStatus,
			FunctInterStep<int, double> > AMInspector::probOK_OtherFactors;

// Table has 3 entries -- 
//	1) clinical status (boolean TRUE or FALSE)
//	2) ambulatory status (NORMAL, NONAMB),
//	3) prob of testing clinical;
FunctTable<SickBovine::ClinicalStatus, 
			FunctTable<SickBovine::AmbStatus, double> > AMInspector::probTestClinical;

FunctTable<AMInspector::TestOK_OtherFactors, 
					FunctTable<AMInspector::TestClinical, 
					FunctTable<SickBovine::AmbStatus,
					Tokens::TF> > > AMInspector::okForFood;

FunctTable<AMInspector::TestOK_OtherFactors, 
					FunctTable<AMInspector::TestClinical, 
					FunctTable<SickBovine::AmbStatus,
					Tokens::TF> > > AMInspector::okForFeed;


//Sends sick bovine to the AM inspector.  AM inspector determins non-bse characteristics
//(parameter testOKnonBSE) and whether the animal tests positive for clinical signs of
//BSE (parameter testClinical).  Based on these parameters, the AM inspector determines
//if the animal is ok for feed and ok for food.
void AMInspector::process(SickBovine & bovine) {
	
	//Assign results of AM inspection (nonBSE pass/fail and clinical status)
	Random& generator = *(Random::instance());
	SickBovine::ClinicalStatus	clinicalStatus;
	SickBovine::AmbStatus ambStatus;

	if (bovine.isClinical) {clinicalStatus = SickBovine::CLINICAL;}
	else {clinicalStatus = SickBovine::NOSIGNS;}

	if (bovine.isNonAmbulatory) {ambStatus = SickBovine::NONAMB;}
	else {ambStatus = SickBovine::NORMAL;}

	bovine.testOKnonBSE = (generator.nextFloat() <= 
		probOK_OtherFactors(ambStatus)(bovine.getAge()));
	bovine.testClinical = (generator.nextFloat() <= 
			probTestClinical(clinicalStatus)(ambStatus));

	//Based on TestOKnonBSE, TestClinical, and AmbStatus, decide if animal is 
	//ok for food and feed.

	AMInspector::TestOK_OtherFactors testOK_OtherFactors;
	AMInspector::TestClinical testClinical;

	if (bovine.testOKnonBSE) {testOK_OtherFactors = AMInspector::TESTOK;}
	else {testOK_OtherFactors = AMInspector::TESTNOTOK;}

	if (bovine.testClinical) {testClinical = AMInspector::TESTCLINICAL;}
	else {testClinical = AMInspector::TESTNOTCLINICAL;}

	bovine.okForFood = 
		(okForFood(testOK_OtherFactors)(testClinical)(ambStatus)
			== Tokens::TRUE);
	bovine.okForFeed = 
		(okForFeed(testOK_OtherFactors)(testClinical)(ambStatus)
			== Tokens::TRUE);

}

void AMInspector::incinerate(std::vector<Organ>& organs) {
	std::vector<Organ>::iterator iterator;				//Allocate iterator over organs
	Random& generator = *(Random::instance());			//Alias random singleton object
	std::vector<Organ>::iterator end = organs.end();	//Cache last organ location

	//For material identified for incineration by AM, set infectivity to zero -- i.e., remove the
	//infectivity from the simulation.
	for(iterator = organs.begin(); iterator != end; ++iterator) {
			Reporter::instance()->reportid50Track(Reporter::ID50_AMINCINERATE, iterator->ID50);
			iterator->ID50 = 0;
	}
}
	
//Error utility function
void AMInspector::readError() {
	cerr << "Error reading AMInspector data" << endl;
	exit(0);
}

//input function from data stream
istream& operator>> (istream& stream, AMInspector& me) {

	string word;
	char dump[256];
	stream >> word;

	while((word != "</AMInspector>") && !stream.eof()) {
		if		(word == "<probOK_OtherFactors>")   stream.getline(dump, 256).getline(dump, 256) >> me.probOK_OtherFactors >> word;
		else if (word == "<probTestClinical>")		stream.getline(dump, 256).getline(dump, 256) >> me.probTestClinical >> word;
		else if (word == "<okForFood>")				stream.getline(dump, 256).getline(dump, 256) >> me.okForFood >> word;
		else if (word == "<okForFeed>")				stream.getline(dump, 256).getline(dump, 256) >> me.okForFeed >> word;
		else AMInspector::readError();
		stream >> word;
	} if(word != "</AMInspector>") AMInspector::readError();
	return stream;
}

std::istream& operator>>(std::istream& stream, Tokens::TF& tf) {
	std::string    token;
	stream >> token;
	if     (token == "TRUE"  )	tf = Tokens::TRUE;
	else if(token == "FALSE"  ) tf = Tokens::FALSE;
	else {std::cerr << "AMInspector::tf" << token << " unknown"; exit(0);}
	return stream;
}

std::istream& operator>>(std::istream& stream, AMInspector::TestOK_OtherFactors& testOK_OtherFactors) {
	std::string    token;
	stream >> token;
	if     (token == "TESTOK")		testOK_OtherFactors = AMInspector::TESTOK;
	else if(token == "TESTNOTOK")	testOK_OtherFactors = AMInspector::TESTNOTOK;
	else {std::cerr << "AMInspector::testOK_OtherFactors " << token << " unknown"; exit(0);}
	return stream;
}

std::istream& operator>>(std::istream& stream, AMInspector::TestClinical& testClinical) {
	std::string    token;
	stream >> token;
	if     (token == "TESTNOTCLINICAL")	testClinical = AMInspector::TESTNOTCLINICAL;
	else if(token == "TESTCLINICAL")	testClinical = AMInspector::TESTCLINICAL;
	else {std::cerr << "AMInspector::testClinical " << token << " unknown"; exit(0);}
	return stream;
}
