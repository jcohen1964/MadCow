#ifndef SRM_INSPECTOR_H
#define SRM_INSPECTOR_H

#include <vector>
#include <iostream>
#include "organ.h"
#include "functTable.h"
#include "functInterStep.h"
#include "functDistrDisc.h"

using std::istream;

class SRMInspector;
istream& operator>> (istream&, SRMInspector&);

class SRMInspector {
	friend istream& operator>> (istream&, SRMInspector&);

	public:
		void process(std::vector<Organ>&);

	private:
		static void readError();
		static FunctInterStep<int, 
								FunctTable<Organ::Type, 
								FunctDistrDisc<double, double> > > fracElimBySRM;
};

#endif
