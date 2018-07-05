#ifndef REPORTER_H
#define REPORTER_H
#include <list>
#include <vector>
#include "totalNumVisitor.h"
#include "totalInfectedVisitor.h"
#include "totalClinicalVisitor.h"
#include "sickBovine.h"
#include "organ.h"

class Reporter {

	/////////////////////////////////////////////////////////////////////////////
	// Reporter construction management
	/////////////////////////////////////////////////////////////////////////////
	//The single copy of Random
	private:
		static Reporter* _instance;   

	//Prevent client construction
	protected:
		Reporter();                   

	//Function to create a singleton instance of the reporter
	public:
		static Reporter*  instance(); //Access the single copy of reporter

    ////////////////////////////////////////////////////////////////////////////
	// Monthly operations
	////////////////////////////////////////////////////////////////////////////
	public:
		void monthlyReset(int, int); //Monthly cleanup operations
		void monthlyOutput(int, int, int); 
		                     //Output monthly results

		// Update stock values
		void updateStocks(TotalNumVisitor);
		void updateStocks(TotalInfectedVisitor);
		void updateStocks(TotalClinicalVisitor);
		
	////////////////////////////////////////////////////////////////////////////
	// Functions to inform the reporter of events
	////////////////////////////////////////////////////////////////////////////
	public:

		// Event types
		enum DeathType {SLAUGHTER, ONFARM_RENDER, ONFARM_BURY, END_DEATHTYPE};

		enum InfectSourceType {MATERNAL, SPONTANEOUS, FIXED, PROTEIN, BLOOD, EXOGENOUS, 
						       END_SOURCETYPE};

		enum ID50TrackType {	ID50_SLAUGHTER,          // 0
								ID50_DIEONFARM,          // 1
								
								ID50_FAILSRM,			 // 2

								ID50_P_MBM,              // 3
								ID50_NP_MBM,             // 4
								ID50_CONTAM_NP_MBM,      // 5
								ID50_MISLABEL_P_MBM,     // 6
								ID50_RENDER,             // 7
								
								ID50_P_FEED,             // 8
								ID50_NP_FEED,            // 9
								ID50_CONTAM_NP_FEED,     // 10
								ID50_MISLABEL_P_FEED,    // 11
								ID50_BLOOD,              // 12
								ID50_OUTAFTERRENDER,     // 13

								ID50_MISFEED,            // 14
								ID50_TOCATTLE,           // 15
								ID50_OUTAFTERFEEDPROD,   // 16
							
								ID50_TOHUMANS,           // 17
								ID50_AMINCINERATE,		 // 18
								END_ID50TRACKTYPE};      // 19

		enum BovineStates	{ANY, INFECTED, CLINICAL, END_BOVINESTATES};

		// Events to be reported
		void reportDeath(DeathType);       // death of a sick bovine (slaughter or natural)
		void reportInfectSource(InfectSourceType);    // infection of a bovine
		void reportid50Track(ID50TrackType, double);             // flow of id50s
		void reportid50Track(ID50TrackType, std::vector<Organ>&);// flow of id50s
		void reportid50toHumans(Organ::Type, int, bool, bool, double);
		void reportNewClinical();
		

	////////////////////////////////////////////////////////////////////////////
	// Members to tabulate events
	////////////////////////////////////////////////////////////////////////////
	protected:
		
		//Vectors to track manner of death and disposition of animal following death
		std::vector<int> infectDieCountTemp;
		std::vector<int> infectDieCountPerm;


		//Vectors to track source of infection
		std::vector<int> infectSourceCountTemp;
		std::vector<int> infectSourceCountPerm;

		//Vectors to track flow of id50s through slaughter, render, and feed processes
		std::vector<double> id50FlowTemp;
		std::vector<double> id50FlowPerm;

		//Vectors to which organs contribute to infectivity going to humans
		std::vector<double> id50ToHumansTemp;
		std::vector<double> id50ToHumansPerm;
		std::vector<double> id50ToHumansPerm_0to11_Normal;
		std::vector<double> id50ToHumansPerm_0to23_Normal;
		std::vector<double> id50ToHumansPerm_0to29_Normal;
		std::vector<double> id50ToHumansPerm_30plus_Normal;
		std::vector<double> id50ToHumansPerm_All_Normal;
		std::vector<double> id50ToHumansPerm_0to11_NonAmb;
		std::vector<double> id50ToHumansPerm_0to23_NonAmb;
		std::vector<double> id50ToHumansPerm_0to29_NonAmb;
		std::vector<double> id50ToHumansPerm_30plus_NonAmb;
		std::vector<double> id50ToHumansPerm_All_NonAmb;
		std::vector<double> id50ToHumansPerm_0to11_All;
		std::vector<double> id50ToHumansPerm_0to23_All;
		std::vector<double> id50ToHumansPerm_0to29_All;
		std::vector<double> id50ToHumansPerm_30plus_All;
	
		//Vector to track number of clinical and subclincial BSE cases
		std::vector<int> bovine;
		
		//New clinical cases
		int newClinical;
};

#endif //End RandomH

