#include "emboli.h"

std::istream& operator>>(std::istream& stream, Emboli& me) {
	stream >> me.prob >> me.lo >> me.hi;
	return stream;
}