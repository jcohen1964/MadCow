#ifndef BLOOD_INFECTOR_H
#define BLOOD_INFECTOR_H

#include "visitor.h"
#include "bovineHerd.h"
#include "bovineGroup.h"
#include "feedGroup.h"
#include "FunctInterStep.h"
#include "FunctInterLine.h"
#include "FunctTable.h"

#include <istream>
#include <vector>

class BloodInfector;
std::istream& operator>>(std::istream&, BloodInfector&);
double BloodConsumptionAdapter(BovineGroup const&);

class BloodInfector : public Visitor {
	friend istream& operator>>(istream&, BloodInfector&);
	friend double BloodConsumptionAdapter(BovineGroup const&);
	public:
		BloodInfector();

		virtual void visit(BovineHerd*);
		virtual void visit(BovineGroup*);

		virtual void addBolus(double);

		virtual void distributeInfectivity(FeedGroup&);

	private:
		int  numTarget;
		int  curTarget;
		std::vector<double> doses;

	protected:
		static void readError();
		static int numCowsReceiving;
		static FunctInterLine<double, double> probInfection;
		static FunctTable<Bovine::Type, FunctTable<Bovine::Gender, FunctInterStep<int, double> > >  consumption;
		static FunctInterStep<int, double>    susceptibility;
};

#endif