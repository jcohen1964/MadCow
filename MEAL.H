#ifndef MEAL_H
#define MEAL_H

#include <iostream>
#include <string>

class Meal {

	public:
		enum ProducerType    {P_MBM_PRODUCER, NP_MBM_PRODUCER, MIXED_MBM_PRODUCER, 
			                  END_PRODUCER_TYPE};
		enum DestinationType {P_FEED_PRODUCER, NP_FEED_PRODUCER, MIXED_FEED_PRODUCER, 
			                  BLOOD_PRODUCER, POULTRY_FEED_PRODUCER, OUT, END_DESTINATION};
		enum MaterialType    {P_MBM, NP_MBM, BLOOD, END_MBMTYPE};

	
	public:
		double                    id50;
		MaterialType              material;
		ProducerType              producer;
		DestinationType           destination;


	public:
		Meal();
		Meal(double          id50Arg, 
			 MaterialType    materialArg, 
			 ProducerType    producerArg    = P_MBM_PRODUCER, 
			 DestinationType destinationArg = P_FEED_PRODUCER);


};

std::istream& operator>>(std::istream&, Meal::ProducerType&);
std::istream& operator>>(std::istream&, Meal::MaterialType&);
std::istream& operator>>(std::istream&, Meal::DestinationType&);


#endif

