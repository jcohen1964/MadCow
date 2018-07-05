#include <iostream>
#include "FeedProducer.h"
#include "Random.h"
#include "reporter.h"
#include "feedGroup.h"

using std::cerr;
using std::endl;

// Table with 2 entries -- 1) producer type, and 2) producer mislabel probility
FunctTable<Meal::DestinationType, double> FeedProducer::probMisLabel;

// Table with 2 entries -- 1) producer type, and 2) producer contamination probability
FunctTable<Meal::DestinationType, double> FeedProducer::probContaminate;

// Table with 3 entries -- 1) Type of MBM source material, 2) feed destination type, and
// 3) proportion of MBM of type (1) made into feed of type (2)
FunctTable<Meal::MaterialType, FunctTable<Feed::MaterialType, double> > 
           FeedProducer::probFeedType;
double FeedProducer::fracContaminate;

FeedProducer::FeedProducer() {}
	
//mealGroup (list of meal objects) and returns feedGroup (list of feed objects)
FeedGroup FeedProducer::process(MealGroup& mealGroup) {

	FeedGroup result;
	FeedGroup contaminatedFeed;
	Feed      feed;
	Random& generator = *(Random::instance());

	//Loop through the meal bundles.  For each that has a non-zero id50 value and
	//a destination of BLOOD_PRODUCER, P_FEED_PRODUCER, or NP_FEED_PRODUCER, create
	//a feed object and store it on the feedGroup list.
	{
	for(MealGroup::iterator it = mealGroup.begin(); it != mealGroup.end(); ++it) {

		//If id50 value is non-zero and headed to a feed producer or the farm...
		if (it->id50>0 && (it->destination == Meal::P_FEED_PRODUCER ||
			               it->destination == Meal::NP_FEED_PRODUCER ||
						   it->destination == Meal::MIXED_FEED_PRODUCER ||
						   it->destination == Meal::BLOOD_PRODUCER ||
						   it->destination == Meal::POULTRY_FEED_PRODUCER)) {

			//Feed producer is set equal to the meal destination
			feed.producer = it->destination;

			//If the meal material is blood, then the feed material is blood
			if(it->material == Meal::BLOOD) {
				feed.material = Feed::BLOOD;
				Reporter::instance()->reportid50Track(Reporter::ID50_BLOOD, it->id50);
			}

			//If the meal destination is poultry feed producer, then the feed 
			// material is POULTRY_FEED
			if(feed.producer == Meal::POULTRY_FEED_PRODUCER)  {
				feed.material = Feed::POULTRY_FEED;
				Reporter::instance()->reportid50Track(Reporter::ID50_NP_FEED, it->id50);
			}

			//If the meal destination is a prohibited feed producer, then the feed
			//material is P_FEED.
			if(feed.producer == Meal::P_FEED_PRODUCER)  {
				feed.material = Feed::P_FEED;
				Reporter::instance()->reportid50Track(Reporter::ID50_P_FEED, it->id50);
			}

			//If the meal destination is a NP feed producer, then the feed 
			//material is NP_FEED.
			if(feed.producer == Meal::NP_FEED_PRODUCER) {
				feed.material = Feed::NP_FEED;
				Reporter::instance()->reportid50Track(Reporter::ID50_NP_FEED, it->id50);
			}

			//If the meal destination is a mixed feed producer, then the feed material is
			//either P_FEED or NP_FEED, based on selection from probFeedType.
			if (feed.producer == Meal::MIXED_FEED_PRODUCER) {
				double unif = generator.nextFloat();
				if(unif <= probFeedType(it->material)(Feed::P_FEED)) {
					feed.material = Feed::P_FEED;
					Reporter::instance()->reportid50Track(Reporter::ID50_P_FEED, it->id50);
				}
				else {
					feed.material = Feed::NP_FEED;
					Reporter::instance()->reportid50Track(Reporter::ID50_NP_FEED, it->id50);
				}
			}

			//The feed packet inherits id50 value from the meal packet
			feed.id50 = it->id50;
			
			//Add the feed packet to the return list of feed packets
			result.addFeed(feed);
		}
	}
	}
	
	//Loop through all the feed packets in the feedGroup list and take care of 
	//mislabeling and contamination
	{
	for(FeedGroup::iterator it = result.begin(); it != result.end(); ++it) {

		//Mislabelling and contamination occur only to prohibited feed packets
		if(it->material == Feed::P_FEED) {

			//If packet is mislabelled, relabel packet as NP
			if(generator.nextFloat() <= probMisLabel(it->producer)) {
				it->material = Feed::NP_FEED;
				Reporter::instance()->reportid50Track(Reporter::ID50_P_FEED, -1*(feed.id50));
				Reporter::instance()->reportid50Track(Reporter::ID50_MISLABEL_P_FEED, feed.id50);
			}
		
			//Contamination happens when some of the prohibited feed ends up in some
			//non-prohibited feed.  In this implementation, we assume that the 
			//prohibited feed ends up contaminating a non-prohibited feed packet that
			//has been unaffected up until this point.  Collect any new contaminated 
			//NP feed packets in the contaminatedFeed list.  Notify reporter.
			if(it->material == Feed::P_FEED) { //Contam can happen if pkt is not mislabeled
				if(generator.nextFloat() <= probContaminate(it->producer)) {
					feed.producer = it->producer;
					feed.material = Feed::NP_FEED;
					feed.id50     = FeedProducer::fracContaminate*it->id50;
					contaminatedFeed.addFeed(feed);
					it->id50 -= feed.id50;
					Reporter::instance()->reportid50Track(Reporter::ID50_P_FEED, -1*(feed.id50));
					Reporter::instance()->reportid50Track(Reporter::ID50_CONTAM_NP_FEED, feed.id50);
				}
			}
		}
	}
	}

	//Add any packets on the contaminatedFeed list to the result list.
	{
	for(FeedGroup::iterator it = contaminatedFeed.begin(); it != contaminatedFeed.end(); 
	     ++it) {
		feed.producer    = it->producer;
		feed.destination = it->destination;
		feed.id50        = it->id50;
		feed.material    = it->material;
		result.addFeed(feed);
	}
	}

	return result;
}

void FeedProducer::readError() {
	std::cerr << "Error reading FeedProducer data" << std::endl;
	exit(0);
}


//input function from data stream
istream& operator>> (istream& stream, FeedProducer& me) {

	string word;
	char dump[256];
	stream >> word;

	while((word != "</feedProducer>") && !stream.eof()) {
		if     (word == "<probFeedType>")    stream.getline(dump, 256).getline(dump, 256) >> me.probFeedType >> word;
		else if(word == "<probMisLabel>")    stream.getline(dump, 256).getline(dump, 256) >> me.probMisLabel >> word;
		else if(word == "<probContaminate>") stream.getline(dump, 256).getline(dump, 256) >> me.probContaminate >> word;
		else if(word == "<fracContaminate>")   stream >> me.fracContaminate >> word;
		else FeedProducer::readError();
		stream >> word;
	} if(word != "</feedProducer>") FeedProducer::readError();
	return stream;
}


