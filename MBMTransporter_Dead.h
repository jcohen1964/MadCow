#ifndef MBM_TRANSPORTER_DEAD_H
#define MBM_TRANSPORTER_DEAD_H

#include <iostream>
#include <string>
#include "functTable.h"
#include "functDistrDisc.h"
#include "meal.h"
#include "mealGroup.h"

using std::istream;
using std::string;

class MBMTransporter_Dead;
std::istream& operator>> (std::istream&, MBMTransporter_Dead&);

class MBMTransporter_Dead {
	friend std::istream& operator>> (std::istream&, MBMTransporter_Dead&);

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
