#ifndef FEED_TRANSPORTER_H
#define FEED_TRANSPORTER_H

#include <iostream>
#include <string>
#include "functTable.h"
#include "functDistrDisc.h"
#include "feed.h"
#include "meal.h"
#include "feedGroup.h"

using std::istream;
using std::string;

class FeedTransporter;
std::istream& operator>> (std::istream&, FeedTransporter&);

class FeedTransporter {
	friend std::istream& operator>> (std::istream&, FeedTransporter&);

	public:
		FeedTransporter();
		void transport(FeedGroup&);

	public:
        static FunctTable<Meal::DestinationType, 
			              FunctTable<Feed::MaterialType, 
			                         FunctDistrDisc<int,Feed::DestinationType> > >
			   probDestination;

	private:
		static void readError();
		
};

#endif
