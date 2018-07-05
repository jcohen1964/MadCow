#ifndef FEEDER_H
#define FEEDER_H

#include <iostream>
#include "feed.h"
#include "feedGroup.h"

using std::istream;

class Feeder;
istream& operator>> (istream&, Feeder&);

class Feeder {
	friend istream& operator>> (istream&, Feeder&);

	public:
		Feeder();
		void markForMisfeed(FeedGroup&);

	public:
		static double probFeedOK;
		static void readError();
};

#endif