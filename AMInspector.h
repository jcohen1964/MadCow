#ifndef AM_INSPECTOR_H
#define AM_INSPECTOR_H

#include <iostream>
#include "feed.h"
#include "bovine.h"
#include "sickBovine.h"
#include "functInterStep.h"
#include "classes.h"
#include "organ.h"

using std::string;
using std::istream;


class AMInspector;

class AMInspector {
	friend istream& operator>> (istream&, AMInspector&);

	public:
		void process(SickBovine &);
		void incinerate(std::vector<Organ>&);
		
		enum	TestOK_OtherFactors		{TESTOK, TESTNOTOK, ENDTESTOK_OTHERFACTORS};
		enum	TestClinical			{TESTNOTCLINICAL, TESTCLINICAL, ENDTESTCLINICAL};

		static	FunctTable<SickBovine::AmbStatus,
				FunctInterStep<int, double> > probOK_OtherFactors;
		static	FunctTable<SickBovine::ClinicalStatus,
				FunctTable<SickBovine::AmbStatus, double> > probTestClinical;

		static	FunctTable<TestOK_OtherFactors, 
					FunctTable<TestClinical, 
					FunctTable<SickBovine::AmbStatus,
					Tokens::TF> > > okForFood;
		static	FunctTable<TestOK_OtherFactors, 
					FunctTable<TestClinical, 
					FunctTable<SickBovine::AmbStatus,
					Tokens::TF> > > okForFeed;

	private:
		static void readError();

};

istream& operator>> (istream&, AMInspector&);
std::istream& operator>>(std::istream&, SickBovine::ClinicalStatus&);
std::istream& operator>>(std::istream&, SickBovine::AmbStatus&);

#endif


