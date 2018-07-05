#include "renderer.h"
#include "Random.h"
#include "reporter.h"
#include "util.h"
#include "Meal.h"
#include "MealGroup.h"

using std::cerr;
using std::endl;

// Probability that a P meal pkt will contaminate an NP pkt at a mixed producer plant
double Renderer::probContamination;

// Table with 2 entries -- 1) render reduction factor, and 2) probability that rendering
// will achieve reduction factor specified by (1)
FunctDistrDisc<int, double> Renderer::renderFactor;

// Table with 2 entries -- 1) Type of renderer, and 2) probability that animal remains
// will be sent to a renderer of type in (1)
FunctTable<Meal::ProducerType, double> Renderer::probType;
FunctTable<Meal::ProducerType, double> Renderer::probMisLabel;
double Renderer::fracContaminate;

Renderer::Renderer() {}
	
//Receives a vector of organs (i.e., the carcas) and returns a mealGroup
//Infectivity ends up in P MBM, NP MBM, and in BLOOD
MealGroup Renderer::process(std::vector<Organ>& organs) {

	double				  thisRenderFactor; //Holds renderFactor value;

	MealGroup             result;    //List of meal objects;
	Meal                  mealBlood; //meal packet for blood meal
	Meal                  mealP;     //meal packet for prohibited MBM
	Meal                  mealNP;    //meal packet for non-prohibited MBM
	Meal                  contaminatedMeal; 
		                             //NP meal packet containing contamination from P MBM
	Meal                  mislabeledMeal; 
		                             //NP meal packet containing mislabeled P MBM
	Meal::ProducerType    producer;   //meal producer type
	Random&               generator = *(Random::instance()); 
	                                 //Alias random number singleton
	//Designate the material type;
	mealBlood.material        = Meal::BLOOD;
	mealP.material            = Meal::P_MBM;
	mealNP.material           = Meal::NP_MBM;
	contaminatedMeal.material = Meal::NP_MBM;
	mislabeledMeal.material   = Meal::NP_MBM;

	//Choose the meal producerType
	double unif = generator.nextFloat();
	if(unif <= probType(Meal::P_MBM_PRODUCER)) {
		producer = Meal::P_MBM_PRODUCER;
	} else if(unif <= probType(Meal::P_MBM_PRODUCER) + probType(Meal::NP_MBM_PRODUCER)) {
		producer = Meal::NP_MBM_PRODUCER;
	} else {
		producer = Meal::MIXED_MBM_PRODUCER;
	}

	//If there is any infectivity in blood, and if the blood is not going to human
	//consumption, then copy blood info into meal packet.
	if(organs[Organ::BLOOD].ID50 > 0 && !organs[Organ::BLOOD].forHumanFood) {
		mealBlood.destination = Meal::BLOOD_PRODUCER;
		mealBlood.id50        = organs[Organ::BLOOD].ID50;
		organs[Organ::BLOOD].ID50 = 0;
	}

	//If not chosen for food then add organs to P_MBM
	mealP.id50 = 0; //Initialize id50 field to zero
	for(int i = 0; i < Organ::END_TYPE; ++i) {
		if(!organs[i].forHumanFood) {
			mealP.id50 += organs[i].ID50;
			organs[i].ID50     = 0;
		}
	}

	//Apply render factor
	thisRenderFactor = renderFactor(0); //0 is dummy argument;
	Reporter::instance()->reportid50Track(Reporter::ID50_RENDER, 
		                                  mealP.id50 * 
										  (thisRenderFactor-1) / thisRenderFactor);
	mealP.id50 /= thisRenderFactor;

	//If mislabelling occurs, move infectivity to mislabeledMeal.
	//Note -- the mislabel probability should be set to 1.0 for the NP MBM producer
	if(generator.nextFloat() <= probMisLabel(producer)) {
		mislabeledMeal.id50 = mealP.id50;
		mealP.id50  = 0; }

	//If contamination occurs, NP MBM gets some of the P MBM infectivity.
	if(producer == Meal::MIXED_MBM_PRODUCER && generator.nextFloat() <= probContamination ) {
		double deltaContamination = Renderer::fracContaminate*mealP.id50;
		mealP.id50  -=  deltaContamination;
		contaminatedMeal.id50 = deltaContamination;
	}

	//Add to MealGroup list those meal packets with non-zero id50 values
	if(mealBlood.id50>0) {
		mealBlood.producer = producer;
		result.addMeal(mealBlood);
	}
	if(mealP.id50>0) {
		mealP.producer = producer;
		result.addMeal(mealP);
		Reporter::instance()->reportid50Track(Reporter::ID50_P_MBM, mealP.id50);
	}
	if(mealNP.id50>0) {
		mealNP.producer = producer;
		result.addMeal(mealNP);
		Reporter::instance()->reportid50Track(Reporter::ID50_NP_MBM, mealNP.id50);
	}
	if(mislabeledMeal.id50>0) {
		mislabeledMeal.producer = producer;
		result.addMeal(mislabeledMeal);
		Reporter::instance()->reportid50Track(Reporter::ID50_MISLABEL_P_MBM, 
			      mislabeledMeal.id50);
	}
	if(contaminatedMeal.id50>0) {
		contaminatedMeal.producer = producer;
		result.addMeal(contaminatedMeal);
		Reporter::instance()->reportid50Track(Reporter::ID50_CONTAM_NP_MBM, 
			      contaminatedMeal.id50);
	}

	return result;
	
};

// error function utility
void Renderer::readError() {
	cerr << "Error reading Renderer data" << endl;
	exit(0);
}

//input function from data stream
istream& operator>> (istream& stream, Renderer& me) {

	string word;
	char dump[256];
	stream >> word;

	while((word != "</renderer>") && !stream.eof()) {
		if(word == "<renderFactor>") stream >> me.renderFactor >> word;
		else if(word == "<probContamination>") stream >> me.probContamination >> word;
		else if(word == "<probType>") stream.getline(dump, 256).getline(dump, 256) >> me.probType >> word;
		else if(word == "<probMisLabel>") stream.getline(dump, 256).getline(dump, 256) >> me.probMisLabel >> word;
		else if(word == "<fracContaminate>")   stream >> me.fracContaminate >> word;
		else Renderer::readError();
		stream >> word;
	} if(word != "</renderer>") Renderer::readError();
	return stream;
}

