#ifndef FUNCT_INTER_LINE_H
#define FUNCT_INTER_LINE_H

#include <map>
#include "functTable.h"

template<class ARG, class RES> class FunctInterLine : public FunctTable<ARG, RES> {
	public:
		RES operator()(ARG const&) const;
};

template<class ARG, class RES> inline
RES FunctInterLine<ARG, RES>::operator()(ARG const& argument) const{

		const_iterator result_lo;
		const_iterator result_hi;
		result_lo = table.lower_bound(argument); //Equal to or higher
		result_hi = table.upper_bound(argument); //Higher

		if(result_lo == table.end()) return (--result_lo)->second; //past end of table
		if(result_lo == table.begin()) return result_lo->second;   //before begining
		if(result_lo->first == argument) return result_lo->second; //found exact match

		//Otherwise interpolate
		result_lo--;
		ARG delta  = argument          - result_lo->first;
		ARG deltax = result_hi->first  - result_lo->first;
		RES deltay = result_hi->second - result_lo->second;
		RES slope  = deltay / deltax;
		RES result = result_lo->second + delta * slope;
		return result;
}

#endif