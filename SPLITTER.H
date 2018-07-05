#ifndef SPLITTER_H
#define SPLITTER_H

#include <vector>
#include <iostream>
#include "organ.h"
#include "functTable.h"
#include "functInterStep.h"

using std::istream;

class Splitter;
istream& operator>> (istream&, Splitter&);

class Splitter {

	friend istream& operator>> (istream&, Splitter&);

	public:
		enum Combo {
				NNN,
				YNN,
				NYN,
				YYN,
				NNY,
				YNY,
				NYY,
				YYY,
				END_COMBO
		};
				
		void process(std::vector<Organ>&);

	private:
		static void readError();

		double fracAerosol;
		static	FunctInterStep<int, FunctTable<Combo, double> > fracSCInMuscle;
		static	FunctInterStep<int, FunctTable<Combo, double> > fracDRGInMuscle;
		static	FunctInterStep<int, FunctTable<Combo, double> > fracSCInAMRMeat;
		static	FunctInterStep<int, FunctTable<Combo, double> > fracDRGInAMRMeat;
		static	FunctInterStep<int, FunctTable<Combo, double> > fracSCInBone;
		static	FunctInterStep<int, FunctTable<Combo, double> > fracDRGInBone;
		static	FunctInterStep<int, FunctTable<Combo, double> > probMS_AMR_SCRemove;
};

std::istream& operator>>(std::istream&, Splitter::Combo&);



#endif