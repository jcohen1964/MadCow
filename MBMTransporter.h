#ifndef MBM_TRANSPORTER_H
#define MBM_TRANSPORTER_H

#include <iostream>
#include <string>
#include "functTable.h"
#include "functDistrDisc.h"
#include "meal.h"
#include "mealGroup.h"

using std::istream;
using std::string;

class MBMTransporter;
std::istream& operator>> (std::istream&, MBMTransporter&);

class MBMTransporter {
	friend std::istream& operator>> (std::istream&, MBMTransporter&);

	public:
		MBMTransporter();
		void transport(MealGroup&);

	public:
        static FunctTable<Meal::ProducerType, 
			              FunctTable<Meal::MaterialType, 
			                         FunctDistrDisc<double,Meal::DestinationType> > >
			   probDestination;

	private:
		static void readError();
		
};


#endif
