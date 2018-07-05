#ifndef FEED_H
#define FEED_H

#include <iostream>
#include <string>

#include "meal.h"

class Feed {

	public:
		enum DestinationType {FARM, OUT, END_DESTINATION};
		enum MaterialType    {P_FEED, NP_FEED, POULTRY_FEED, BLOOD, END_MBMTYPE};
	
	public:
		double                    id50;
		MaterialType              material;
		Meal::DestinationType     producer;
		DestinationType           destination;
		bool				      misFeed;


	public:
		Feed();
		Feed(double                id50Arg, 
			 MaterialType          materialArg, 
			 Meal::DestinationType producerArg    = Meal::P_FEED_PRODUCER, 
			 DestinationType       destinationArg = FARM,
             bool                  misFeed = false);


};

std::istream& operator>>(std::istream&, Meal::DestinationType&);
std::istream& operator>>(std::istream&, Feed::MaterialType&);
std::istream& operator>>(std::istream&, Feed::DestinationType&);

#endif

