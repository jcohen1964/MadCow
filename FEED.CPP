#include "feed.h"

Feed::Feed() :
    producer(Meal::P_FEED_PRODUCER),
	destination(FARM),
	material(P_FEED),
	id50(0),
	misFeed(false)
	{}

	Feed::Feed(double                    id50, 
		       MaterialType              material, 
			   Meal::DestinationType     producer, 
		       DestinationType           destination,
			   bool					     misFeed)
	{
	id50        = id50;
	material    = material;
	producer    = producer;
	destination = destination;
	misFeed     = misFeed;

}

std::istream& operator>>(std::istream& stream, Feed::MaterialType& material) {
	std::string    token;
	stream >> token;
	if     (token == "P_FEED" )			material = Feed::P_FEED;
	else if(token == "NP_FEED")			material = Feed::NP_FEED;
	else if(token == "BLOOD"  )			material = Feed::BLOOD;
	else if(token == "POULTRY_FEED")	material = Feed::POULTRY_FEED;
	else {std::cerr << "feedTransporter::Material " << token << " unknown"; exit(0);}
	return stream;
}

std::istream& operator>>(std::istream& stream, Feed::DestinationType& destination) {
	std::string    token;
	stream >> token;
	if     (token == "FARM") destination = Feed::FARM;
    else if(token == "OUT" ) destination = Feed::OUT;
	else {std::cerr << "feedTransporter::Destination " << token << " unknown"; exit(0);}
	return stream;
}
