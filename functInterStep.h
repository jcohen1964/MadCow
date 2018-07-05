#ifndef FUNCT_INTER_STEP_H
#define FUNCT_INTER_STEP_H

#include <map>
#include "functTable.h"

template<class ARG, class RES> class FunctInterStep : public FunctTable<ARG, RES> {
	public:
		RES operator()(ARG const&) const;
};

template<class ARG, class RES> inline
RES FunctInterStep<ARG, RES>::operator()(ARG const& argument) const {

		const_iterator result;
		result = table.lower_bound(argument); 

		if(table.size() == 0)         return RES(); //Empty table return default value
		if(result == table.end())     return (--result)->second; //past end of table
		if(result == table.begin())   return result->second;     //before begining
		if(result->first == argument) return result->second;     //found exact match
		//Otherwise interpolate
		return (--result)->second;
}

#endif