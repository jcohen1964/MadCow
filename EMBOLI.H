#ifndef EMBOLI_H
#define EMBOLI_H

#include <istream>

struct Emboli;
std::istream& operator>>(std::istream&, Emboli&);

struct Emboli {
	Emboli() :  prob(0), lo(0), hi(0) {}
	double prob;
	double lo;
	double hi;
};

#endif