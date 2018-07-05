#ifndef RANDOM_INFECTOR_H
#define RANDOM_INFECTOR_H

#include "visitor.h"
#include "bovineHerd.h"
#include "bovineGroup.h"
#include "FunctInterStep.h"
#include "FunctInterLine.h"
#include <istream>
#include <vector>

class RandomInfector;
std::istream& operator>>(std::istream&, RandomInfector&);
double RandomConsumptionAdapter(BovineGroup const&);

class RandomInfector : public Visitor {
	friend std::istream& operator>>(std::istream&, RandomInfector&);
	friend double RandomConsumptionAdapter(BovineGroup const&);
	public:
		RandomInfector();

		virtual void visit(BovineHerd*);
		virtual void visit(BovineGroup*);

		virtual void addBolus(double);

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