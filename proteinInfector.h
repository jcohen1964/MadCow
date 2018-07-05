#ifndef PROTEIN_INFECTOR_H
#define PROTEIN_INFECTOR_H

#include "visitor.h"
#include "bovineHerd.h"
#include "bovineGroup.h"
#include "FunctInterStep.h"
#include "FunctInterLine.h"
#include "feedGroup.h"
#include <istream>
#include <vector>

class ProteinInfector;
std::istream& operator>>(std::istream&, ProteinInfector&);
double ProteinConsumptionAdapter(BovineGroup const&);

class ProteinInfector : public Visitor {
	friend std::istream& operator>>(std::istream&, ProteinInfector&);
	friend double ProteinConsumptionAdapter(BovineGroup const&);
	public:
		ProteinInfector();

		virtual void visit(BovineHerd*);
		virtual void visit(BovineGroup*);

		virtual void addBolus(double);
		virtual void distributeInfectivity(FeedGroup&);


	private:
		int  numTarget;					//Total number of animals exposed to contaminated meal
		int  curTarget;					//Index into the doses array
		std::vector<double> doses;		//Vector containing numTarget entries.  Each entry has id50s going to
										//one bovine.
		
	protected:
		static void readError();				//Input error msg function
		static int numCowsReceiving;			//Number of cattle among which one pkt of protein is divided
		static FunctInterLine<double, double>	//Dose response function -- 1st entry is id50s, 2nd is prob infection
			probInfection;
		static FunctTable<Bovine::Type,			//Consumption table -- 1st entry is bovine type, 2nd is gender, 3rd
			FunctTable<Bovine::Gender,			//age.  Value is relative consumption rate
			FunctInterStep<int, double> > >  consumption;
		static FunctInterStep<int, double>		//Susceptability table -- 1st entry is age in months, 2nd is the 
			susceptibility;						//relative susceptability to infection.  Prob infection is
												//proportional to susceptability.
};

#endif