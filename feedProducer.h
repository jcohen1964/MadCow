#ifndef FEED_PRODUCER_H
#define FEED_PRODUCER_H

#include <iostream>
#include <vector>
#include <string>
#include "functTable.h"
#include "functDistrDisc.h"
#include "meal.h"
#include "feed.h"
#include "mealGroup.h"
#include "feedgroup.h"

using std::istream;
using std::string;

class FeedProducer;
std::istream& operator>> (std::istream&, FeedProducer&);

class FeedProducer {
	friend std::istream& operator>> (std::istream&, FeedProducer&);

	public:
		FeedProducer();
		FeedGroup process(MealGroup&);

	public:
		static FunctTable<Meal::DestinationType, double> probMisLabel;
		static FunctTable<Meal::DestinationType, double> probContaminate;
		static FunctTable<Meal::MaterialType, FunctTable<Feed::MaterialType, double> > 
           FeedProducer::probFeedType;
		static double fracContaminate;

	private:
		static void readError();
		friend std::istream& operator>>(std::istream&, Meal::DestinationType&);
};

#endif