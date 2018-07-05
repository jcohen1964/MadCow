#include <iostream>
#include "FeedTransporter.h"
#include "Random.h"
#include "reporter.h"
#include "feedGroup.h"

using std::cerr;
using std::endl;


// Table with 4 entries -- 1) producer type, 2) material type, 3) destination, and
// 4) proportion of feed with specified values in (1) and (2) that go to (3) as a 
// destination.
FunctTable<Meal::DestinationType, 
           FunctTable<Feed::MaterialType, 
                      FunctDistrDisc<int,Feed::DestinationType> > >
					  FeedTransporter::probDestination;

FeedTransporter::FeedTransporter() {}
	
//Takes pointer to a feedGroup and assigns destinations appropriately.
//May be enhanced to simulate in-transit contamination
void FeedTransporter::transport(FeedGroup& feedGroup) {

	Random& generator = *(Random::instance());

	//Loop through feedGroup
	for(FeedGroup::iterator it = feedGroup.begin(); it != feedGroup.end(); ++it) {
		it->destination =									//Select the destination
			probDestination(it->producer)(it->material)(0);
		if (it->destination == Feed::OUT) {					//If destination is OUT...
			Reporter::instance()->reportid50Track(Reporter::ID50_OUTAFTERFEEDPROD, it->id50);
		}
	}
};

void FeedTransporter::readError() {
	std::cerr << "xxx Error reading feedTransporter data" << std::endl;
	exit(0);
}


//input function from data stream
istream& operator>> (istream& stream, FeedTransporter& me) {

	string word;
	char dump[256];
	stream >> word;

	while((word != "</feedTransporter>") && !stream.eof()) {
		if     (word == "<probDestination>")      stream.getline(dump, 256).getline(dump, 256) >> me.probDestination >> word;
		else FeedTransporter::readError();
		stream >> word;
	} if(word != "</feedTransporter>") FeedTransporter::readError();
	return stream;
}

