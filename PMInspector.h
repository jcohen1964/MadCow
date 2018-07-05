#ifndef PM_INSPECTOR_H
#define PM_INSPECTOR_H

#include <vector>
#include <iostream>
#include "organ.h"
#include "functTable.h"
#include "functInterStep.h"

using std::istream;

class PMInspector;
istream& operator>> (istream&, PMInspector&);

class PMInspector {
	friend istream& operator>> (istream&, PMInspector&);

	public:
		void process(std::vector<Organ>&);

	private:
		static void readError();
		static	FunctInterStep<int,
					FunctTable<bool,
						FunctTable<Organ::Type, double> > > probPassPM;
};

#endif