#ifndef SRM_INSPECTOR_DEAD_H
#define SRM_INSPECTOR_DEAD_H

#include <vector>
#include <iostream>
#include "organ.h"
#include "functTable.h"
#include "functInterStep.h"
#include "functDistrDisc.h"

using std::istream;

class SRMInspector_Dead;
istream& operator>> (istream&, SRMInspector_Dead&);

class SRMInspector_Dead {
	friend istream& operator>> (istream&, SRMInspector_Dead&);

	public:
		void process(std::vector<Organ>&);

	private:
		static void readError();
		static FunctInterStep<int, 
								FunctTable<Organ::Type, 
								FunctDistrDisc<double, double> > > fracElimBySRM;
};

#endif
