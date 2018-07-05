#include <iostream>
#include "meal.h"

using std::cerr;
using std::endl;

Meal::Meal() :
    producer(P_MBM_PRODUCER),
	destination(P_FEED_PRODUCER),
	material(P_MBM),
	id50(0)
	{}

	Meal::Meal(double                    id50, 
		       MaterialType              material, 
			   ProducerType              producer, 
		       DestinationType           destination) {
	id50        = id50;
	material    = material;
	producer    = producer;
	destination = destination;

}

//data stream input function helpers
std::istream& operator>>(std::istream& stream, Meal::ProducerType& producer) {
	std::string    token;
	stream >> token;
	if     (token == "P_MBM_PRODUCER"     ) producer = Meal::P_MBM_PRODUCER;
	else if(token == "NP_MBM_PRODUCER"    ) producer = Meal::NP_MBM_PRODUCER;
	else if(token == "MIXED_MBM_PRODUCER" ) producer = Meal::MIXED_MBM_PRODUCER;
	else {std::cerr << "Meal::ProducerType " << token << " unknown"; exit(0);}
	return stream;
}

std::istream& operator>>(std::istream& stream, Meal::MaterialType& material) {
	std::string    token;
	stream >> token;
	if     (token == "P_MBM" ) material = Meal::P_MBM;
	else if(token == "NP_MBM") material = Meal::NP_MBM;
	else if(token == "BLOOD" ) material = Meal::BLOOD;
	else {std::cerr << "Meal::MaterialType " << token << " unknown"; exit(0);}
	return stream;
}

std::istream& operator>>(std::istream& stream, Meal::DestinationType& destination) {
	std::string    token;
	stream >> token;
	if     (token == "P_FEED_PRODUCER"      ) destination = Meal::P_FEED_PRODUCER;
    else if(token == "NP_FEED_PRODUCER"     ) destination = Meal::NP_FEED_PRODUCER;
    else if(token == "MIXED_FEED_PRODUCER"  ) destination = Meal::MIXED_FEED_PRODUCER;
    else if(token == "BLOOD_PRODUCER"       ) destination = Meal::BLOOD_PRODUCER;
	else if(token == "POULTRY_FEED_PRODUCER") destination = Meal::POULTRY_FEED_PRODUCER;
    else if(token == "OUT"                  ) destination = Meal::OUT;
	else {std::cerr << "Meal::DestinationType " << token << " unknown"; exit(0);}
	return stream;
}