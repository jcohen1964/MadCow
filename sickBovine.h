// Author  : Keith H. Duggar
// Date    : 2001.01.27
// Purpose : SickBovine extends the notion of a bovine to bovines infected by
//           BSE.  The additional properties are teh age of infection, the
//           months since infection, and the date at which the animal reaches
//           a terminal stage.  In this class, liveFor is overidden to provide
//           behavior relative to a sick bovine.  Not only does it increment
//           the age but also the months infected.  If it reaches terminal
//           illness it reports its death to its death observer.
//
////////////////////////////////////////////////////////////////////////////////
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
000000000100000000020000000003000000000400000000050000000006000000000700000000*/
////////////////////////////////////////////////////////////////////////////////

#ifndef SICK_BOVINE_H
#define SICK_BOVINE_H

#include "bovine.h"
#include "visitor.h"
#include <iosfwd>
#include "functDistrDisc.h"
#include "functInterStep.h"

class BovineGroup;
class SickBovine;
std::istream& operator>> (std::istream&, SickBovine&);

class SickBovine : public Bovine {
	friend class BovineGroup;
	friend std::istream& operator>> (std::istream&, SickBovine&);

	public:
		static int const NONE;

	public:
		SickBovine();
		SickBovine(Bovine const &);
		SickBovine(Bovine const &, int ageInfected, int ageSlaughter);
		SickBovine(Bovine const &, double infectDose);
		SickBovine(SickBovine const &);
		SickBovine(Type, Gender, int age);
		SickBovine(Type, Gender, int age, int ageSick, int monthsSick);

		virtual bool operator==(SickBovine const&);

		virtual SickBovine& operator=(SickBovine const&);

		virtual void accept(Visitor& visitor) {visitor.visit(this);}



	public: //Simple read only access functions
		int getAgeSick         (void) const {return myAgeSick;			}
		int getMonthsSick      (void) const {return myMonthsSick;		}
		int getincPeriod       (void) const {return myIncPeriod;		}
		int getTerminationDate (void) const {return myTerminationDelay;	}
		int getAgeSlaughter	   (void) const {return myAgeSlaughter;     }
		BovineGroup* getGroup  (void) const {return myGroup;			}

	public: //Simple write only access functions
		void setTerminationDelay (int date) {myTerminationDelay = date;}

	public:
		virtual void live();	//Age bovine one month;

	protected:
		virtual bool getNonAmbulatory_CLINICAL(double PNS, double PC);
								//Determine if bovine is non-ambulatory when clinical
		int ComputeIncPeriod(double infectDose);
								//Determine age at manifestation of clinical signs

	protected:
		int  myAgeSick;				//The age bovine was infected
		int  myMonthsSick;			//How long bovine has been infected
		int  myIncPeriod;			//Incubation period
		int  myContagiousDelay;		//Simulation month when bovine will become contagious to offspring
		int  myClinicalDuration;	//Number of months bovine can survive clinical
		int  myTerminationDelay;	//Simulation month when bovine will die (ie incPeriod + ClinicalDuration)
		int  myAgeSlaughter;		//Age when slaugher scheduled (or < 0 if not scheduled)
		bool isNonAmbulatory_NOSIGNS;	//True if non-ambulatory when preclinical
		bool isNonAmbulatory_CLINICAL;	//True if non-ambulatory when clinical

	public:
		bool isClinical;		//True if bovine is clinical
		bool isContagious;		//True if bovine is contagious to offspring
		bool isNonAmbulatory;	//True if bovine is non-ambulatory
		bool dieOfBSE;			//Set to true when bovine dies due to clincial BSE
		bool testOKnonBSE;		//True if animal is ok on non-BSE factors at AM
		bool testClinical;		//True if animal tests positive for BSE at AM
		bool okForFood;			//True if animal can be used in human food;
		bool okForFeed;			//True if animal can be used in feed;

		enum ClinicalStatus {NOSIGNS, CLINICAL, ENDCLINICALSTATUSTYPE};
		SickBovine::ClinicalStatus clinicalStatus;
		enum AmbStatus {NORMAL, NONAMB, ENDAMBSTATUSTYPE};
		SickBovine::AmbStatus ambStatus;

	protected:
		BovineGroup* myGroup;

		static FunctDistrDisc<int, int> incPeriod;
		static FunctDistrDisc<int, int> clinicalDuration;
		static FunctTable<ClinicalStatus, double> probNonAmbulatory;

		static double maternalContagiousPoint; //Frac incub til maternal trans possible
		static bool doseDependentIncPeriod;    //True if incubation period is dose dependent;

		static void readError();
};


#endif