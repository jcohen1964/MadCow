#include "reporter.h"
#include "sickBovine.h"
#include "bovineGroup.h"
#include <iostream>
#include "utilities.h"
#include "globals.h"

Reporter::Reporter() :
	infectDieCountTemp(END_DEATHTYPE, 0),
	infectDieCountPerm(END_DEATHTYPE, 0),
	infectSourceCountTemp(END_SOURCETYPE, 0),
	infectSourceCountPerm(END_SOURCETYPE, 0),
	id50FlowTemp(END_ID50TRACKTYPE, 0),
	id50FlowPerm(END_ID50TRACKTYPE, 0),
	id50ToHumansTemp(Organ::END_TYPE, 0),
	id50ToHumansPerm(Organ::END_TYPE, 0),
	id50ToHumansPerm_0to11_Normal(Organ::END_TYPE, 0),
	id50ToHumansPerm_0to23_Normal(Organ::END_TYPE, 0),
	id50ToHumansPerm_0to29_Normal(Organ::END_TYPE, 0),
	id50ToHumansPerm_30plus_Normal(Organ::END_TYPE, 0),
	id50ToHumansPerm_All_Normal(Organ::END_TYPE, 0),
	id50ToHumansPerm_0to11_NonAmb(Organ::END_TYPE, 0),
	id50ToHumansPerm_0to23_NonAmb(Organ::END_TYPE, 0),
	id50ToHumansPerm_0to29_NonAmb(Organ::END_TYPE, 0),
	id50ToHumansPerm_30plus_NonAmb(Organ::END_TYPE, 0),
	id50ToHumansPerm_All_NonAmb(Organ::END_TYPE, 0),
	id50ToHumansPerm_0to11_All(Organ::END_TYPE, 0),
	id50ToHumansPerm_0to23_All(Organ::END_TYPE, 0),
	id50ToHumansPerm_0to29_All(Organ::END_TYPE, 0),
	id50ToHumansPerm_30plus_All(Organ::END_TYPE, 0),
	bovine(END_BOVINESTATES, 0),
	newClinical(0)
	{}

/////////////////////////////////////////////////////////////////////////////
// Reporter construction management
/////////////////////////////////////////////////////////////////////////////
Reporter* Reporter::_instance  = 0;
Reporter* Reporter::instance() {
	if(!_instance) _instance = new Reporter();
	return _instance;
}

////////////////////////////////////////////////////////////////////////////
//Monthly operations
////////////////////////////////////////////////////////////////////////////
// Cleanup
void Reporter::monthlyReset(int month, int outputInterval) {

	//Reset incremental values if we are at the proper interval 
	if((month%outputInterval == 0)) {
		{
		for(std::vector<int>::iterator it = infectDieCountTemp.begin();
			it != infectDieCountTemp.end(); ++it) 
				*it = 0;
		}
		{
		for(std::vector<int>::iterator it = infectSourceCountTemp.begin();
			it != infectSourceCountTemp.end(); ++it) 
				*it = 0;
		}
		{
		for(std::vector<double>::iterator it = id50FlowTemp.begin();
			it != id50FlowTemp.end(); ++it) 
				*it = 0;
		}
		newClinical = 0;
		{
		for(std::vector<double>::iterator it = id50ToHumansTemp.begin();
			it != id50ToHumansTemp.end(); ++it) 
				*it = 0;
		}
	}
}
// Update stock counts
void Reporter::updateStocks(TotalNumVisitor totalNumVisitor) {
	Reporter::bovine[Reporter::ANY] = totalNumVisitor.total;
}

void Reporter::updateStocks(TotalInfectedVisitor totalInfectedVisitor) {
	Reporter::bovine[Reporter::INFECTED] = totalInfectedVisitor.total;
}

void Reporter::updateStocks(TotalClinicalVisitor totalClinicalVisitor) {
	Reporter::bovine[Reporter::CLINICAL] = totalClinicalVisitor.total;
}


using KHD_UTIL::operator<<;
void Reporter::monthlyOutput(int month, int outputInterval, int numMonths) {

	//Print output if we are at the proper interval or if it is the end of the simulation
	if((month%outputInterval == 0) || (month == numMonths)) {

		//Incremental values;
		std::cout << month                 << '\t';
		std::cout << "Increment "          << '\t';
		std::cout << infectDieCountTemp    << '\t';
		std::cout << infectSourceCountTemp << '\t';
		std::cout << id50FlowTemp          << '\t';
		std::cout << id50ToHumansTemp      << '\t';
		std::cout << newClinical           << '\t';
		std::cout << std::endl;

		//Cumulative values;
		std::cout << month                 << '\t';
		std::cout << "Cumulative "         << '\t';
		std::cout << infectDieCountPerm    << '\t';
		std::cout << infectSourceCountPerm << '\t';
		std::cout << id50FlowPerm          << '\t';
		std::cout << id50ToHumansPerm      << '\t';
		std::cout << std::endl;

		//Stock values;
		std::cout << month                 << '\t';
		std::cout << "Stock "              << '\t';
		std::cout << bovine                << '\t';
		std::cout << std::endl;

		//Human food -- broken down by animal age and ambulatory characteristics
		if (reportDetailedFoodData) {

			//Normal cattle
			std::cout << month					    << '\t';
			std::cout << "X01_FoodDetail"			<< '\t';
			std::cout << "00to11"					<< '\t';
			std::cout << "Normal"					<< '\t';
			std::cout << id50ToHumansPerm_0to11_Normal;
			std::cout << std::endl;

			std::cout << month					    << '\t';
			std::cout << "X01_FoodDetail"			<< '\t';
			std::cout << "00to23"					<< '\t';
			std::cout << "Normal"					<< '\t';
			std::cout << id50ToHumansPerm_0to23_Normal;
			std::cout << std::endl;

			std::cout << month					    << '\t';
			std::cout << "X01_FoodDetail"			<< '\t';
			std::cout << "00to29"					<< '\t';
			std::cout << "Normal"					<< '\t';
			std::cout << id50ToHumansPerm_0to29_Normal;
			std::cout << std::endl;

			std::cout << month					    << '\t';
			std::cout << "X01_FoodDetail"			<< '\t';
			std::cout << "30plus"					<< '\t';
			std::cout << "Normal"					<< '\t';
			std::cout << id50ToHumansPerm_30plus_Normal;
			std::cout << std::endl;
		
			std::cout << month					    << '\t';
			std::cout << "X01_FoodDetail"			<< '\t';
			std::cout << "All"						<< '\t';
			std::cout << "Normal"					<< '\t';
			std::cout << id50ToHumansPerm_All_Normal;
			std::cout << std::endl;
		
			//Non-ambulatory cattle
			std::cout << month					    << '\t';
			std::cout << "X01_FoodDetail"			<< '\t';
			std::cout << "00to11"					<< '\t';
			std::cout << "NonAmb"					<< '\t';
			std::cout << id50ToHumansPerm_0to11_NonAmb;
			std::cout << std::endl;

			std::cout << month					    << '\t';
			std::cout << "X01_FoodDetail"			<< '\t';
			std::cout << "00to23"					<< '\t';
			std::cout << "NonAmb"					<< '\t';
			std::cout << id50ToHumansPerm_0to23_NonAmb;
			std::cout << std::endl;

			std::cout << month					    << '\t';
			std::cout << "X01_FoodDetail"			<< '\t';
			std::cout << "00to29"					<< '\t';
			std::cout << "NonAmb"					<< '\t';
			std::cout << id50ToHumansPerm_0to29_NonAmb;
			std::cout << std::endl;

			std::cout << month					    << '\t';
			std::cout << "X01_FoodDetail"			<< '\t';
			std::cout << "30plus"					<< '\t';
			std::cout << "NonAmb"					<< '\t';
			std::cout << id50ToHumansPerm_30plus_NonAmb;
			std::cout << std::endl;
		
			std::cout << month					    << '\t';
			std::cout << "X01_FoodDetail"			<< '\t';
			std::cout << "All"						<< '\t';
			std::cout << "NonAmb"					<< '\t';
			std::cout << id50ToHumansPerm_All_NonAmb;
			std::cout << std::endl;		
		
			//All cattle, regardless of ambulatory status
			std::cout << month					    << '\t';
			std::cout << "X01_FoodDetail"			<< '\t';
			std::cout << "00to11"					<< '\t';
			std::cout << "All"						<< '\t';
			std::cout << id50ToHumansPerm_0to11_All;
			std::cout << std::endl;

			std::cout << month					    << '\t';
			std::cout << "X01_FoodDetail"			<< '\t';
			std::cout << "00to23"					<< '\t';
			std::cout << "All"						<< '\t';
			std::cout << id50ToHumansPerm_0to23_All;
			std::cout << std::endl;

			std::cout << month					    << '\t';
			std::cout << "X01_FoodDetail"			<< '\t';
			std::cout << "00to29"					<< '\t';
			std::cout << "All"						<< '\t';
			std::cout << id50ToHumansPerm_0to29_All;
			std::cout << std::endl;

			std::cout << month					    << '\t';
			std::cout << "X01_FoodDetail"			<< '\t';
			std::cout << "30plus"					<< '\t';
			std::cout << "All"						<< '\t';
			std::cout << id50ToHumansPerm_30plus_All;
			std::cout << std::endl;
		
			std::cout << month					    << '\t';
			std::cout << "X01_FoodDetail"			<< '\t';
			std::cout << "All"						<< '\t';
			std::cout << "All"						<< '\t';
			std::cout << id50ToHumansPerm;
			std::cout << std::endl;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
//Inform reporter of events
///////////////////////////////////////////////////////////////////////////////

void Reporter::reportDeath(Reporter::DeathType deathType) {
	++(infectDieCountTemp[deathType]);
	++(infectDieCountPerm[deathType]);
}


void Reporter::reportInfectSource(Reporter::InfectSourceType infectSourceType) {
	++(infectSourceCountTemp[infectSourceType]);
	++(infectSourceCountPerm[infectSourceType]);
}

void Reporter::reportid50Track(Reporter::ID50TrackType id50TrackType, double id50) {
	(id50FlowTemp[id50TrackType])+= id50;
	(id50FlowPerm[id50TrackType])+= id50;
}

void Reporter::reportid50Track(Reporter::ID50TrackType id50TrackType, std::vector<Organ>& organs) {
	double result = 0;

	//Loop through organs and total id50s
	for(int i = 0; i < Organ::END_TYPE; ++i) {
			result += organs[i].ID50;
	}
	Reporter::id50FlowTemp[id50TrackType] += result;
	Reporter::id50FlowPerm[id50TrackType] += result;
}

void Reporter::reportNewClinical() {
	Reporter::newClinical++;
}

void Reporter::reportid50toHumans(Organ::Type meatType, 
									int age,
									bool sourceClinical,
									bool sourceNonAmbulatory,
									double id50) {
	Reporter::id50ToHumansTemp[meatType] += id50;
	Reporter::id50ToHumansPerm[meatType] += id50;
	if (!sourceNonAmbulatory) {
		Reporter::id50ToHumansPerm_All_Normal[meatType]			+= id50;
		if (age <= 11) {
			Reporter::id50ToHumansPerm_0to11_Normal[meatType]	+= id50;
			Reporter::id50ToHumansPerm_0to11_All[meatType]		+= id50;
			}
		if (age <= 23) {
			Reporter::id50ToHumansPerm_0to23_Normal[meatType]	+= id50;
			Reporter::id50ToHumansPerm_0to23_All[meatType]		+= id50;
			}
		if (age <= 29) {
			Reporter::id50ToHumansPerm_0to29_Normal[meatType]	+= id50;
			Reporter::id50ToHumansPerm_0to29_All[meatType]		+= id50;
		}
		if (age >= 30) {
			Reporter::id50ToHumansPerm_30plus_Normal[meatType]	+= id50;
			Reporter::id50ToHumansPerm_30plus_All[meatType]		+= id50;
		}
	}
	if (sourceNonAmbulatory) {
		Reporter::id50ToHumansPerm_All_NonAmb[meatType]			+= id50;
		if (age <= 11) {
			Reporter::id50ToHumansPerm_0to11_NonAmb[meatType]	+= id50;
			Reporter::id50ToHumansPerm_0to11_All[meatType]		+= id50;
			}
		if (age <= 23) {
			Reporter::id50ToHumansPerm_0to23_NonAmb[meatType]	+= id50;
			Reporter::id50ToHumansPerm_0to23_All[meatType]		+= id50;
			}
		if (age <= 29) {
			Reporter::id50ToHumansPerm_0to29_NonAmb[meatType]	+= id50;
			Reporter::id50ToHumansPerm_0to29_All[meatType]		+= id50;
		}
		if (age >= 30) {
			Reporter::id50ToHumansPerm_30plus_NonAmb[meatType]	+= id50;
			Reporter::id50ToHumansPerm_30plus_All[meatType]		+= id50;
		}
	}
};
