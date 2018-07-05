#include <string>

#include "feeder.h"
#include "random.h"
#include "reporter.h"

using std::cerr;
using std::endl;
using std::string;

double Feeder::probFeedOK = 0;

Feeder::Feeder() {}

//Takes feedGroup as argument.  Loops through and identifies prohibited packets.  If they 
//are .misfed, then they are converted to non-prohibited packets.
void Feeder::markForMisfeed(FeedGroup& feedGroup) {

	Random& generator = *(Random::instance());
	for(FeedGroup::iterator it = feedGroup.begin(); it != feedGroup.end(); ++it) {
		if(it->material == Feed::P_FEED) it->misFeed = (generator.nextFloat() > probFeedOK);
	}
}

void Feeder::readError() {
	cerr << "Error reading feeder data" << endl;
	exit(0);
}

istream& operator>> (istream& stream, Feeder& me) {

	string word;
	stream >> word;

	if(word == "<probFeedOK>") {
		stream >> me.probFeedOK;
	} else {
		Feeder::readError();
	}
	stream >> word;
	if(word != "</probFeedOK>") Feeder::readError();
	stream >> word;
	if(word != "</feeder>") Feeder::readError();

	return stream;
}
