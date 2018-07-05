#ifndef FUNCT_DISTR_UNIF_H
#define FUNCT_DISTR_UNIF_H

#include <istream>
#include "random.h"

using std::istream;

template<class ARG, class RES> class FunctDistrUnif;
template<class ARG, class RES> istream& operator>>(istream&, FunctDistrUnif<ARG, RES>&);

template<class ARG, class RES> class FunctDistrUnif {
	friend istream& operator>>(istream&, FunctDistrUnif<ARG, RES>&);

	public:
		RES operator()(ARG const&) const;
	private:
		RES lo;
		RES hi;
};

template<class ARG, class RES>
inline RES FunctDistrUnif<ARG, RES>::operator()(ARG const&) const {
	static Random& generator = *(Random::instance());
	return (lo + (hi - lo)*generator.nextFloat());
}

template<class ARG, class RES>
istream& operator>>(istream& stream, FunctDistrUnif<ARG, RES>& me) {
	stream >> me.lo;
	stream >> me.hi;
	return stream;
}

#endif