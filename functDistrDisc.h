#ifndef FUNCT_DISTR_DISC_H
#define FUNCT_DISTR_DISC_H

#include <istream>
#include <map>
#include <vector>
#include "random.h"

using std::istream;

//declarations
template<class ARG, class RES> class FunctDistrDisc;
template<class ARG, class RES> istream& operator>>(istream&, FunctDistrDisc<ARG, RES>&);

//constructor
template<class ARG, class RES> class FunctDistrDisc {
	friend istream& operator>>(istream&, FunctDistrDisc<ARG, RES>&);

	public:
		RES operator()(ARG const&) const;
	private:
		std::map<double, int> cumulative;
		std::vector<RES> choices;
};

//Randomly select and return element from the choices table, with the selection weighted 
//by its associated probability density.
template<class ARG, class RES>
inline RES FunctDistrDisc<ARG, RES>::operator()(ARG const&) const {
	static Random& generator = *(Random::instance());		//Get u[0,1]
	std::map<double, int>::const_iterator choice;			//Pair -- cumulative value and table row
	choice = cumulative.upper_bound(generator.nextFloat());	//Find pair with smallest value >= u
	if(cumulative.size() == 0)     return RES();			//Empty table return default value
	if(choice == cumulative.end()) --choice;				//past end of table should not happen if true cumulative
	return choices[choice->second];							//retrieve choice value with row entry equal 
															//to int value associated with cumulative
}

//Read a table into object me, which contains two members -- a one-to-one map of cumulative
//probability values to table row numbers, and an array of "choices" (table entries) indexed by 
//row number.  This function reads in ascii text and creates the corresponding objects.  The
//first value read in is the number of rows in the table.  Then each row contains a choice
//value and a probability density weight.  
template<class ARG, class RES>
istream& operator>>(istream& stream, FunctDistrDisc<ARG, RES>& me) {
	int num;						//Number of rows in table
	double cum = 0;					//cumulative probability
	double tempCum;					//probability density for individual entry in table
	RES tempRes;					//choice value for individual entry in table
	stream >> num;					//Read in number of entries in table
	me.choices.resize(num);			//make choices array sufficiently large to hold all entries
	me.cumulative.clear();			//Clear the cumulative array
	for(int i = 0; i < num; ++i) {	//loop through table
		stream >> tempRes;			//Get choice value
		stream >> tempCum;			//Get probability density
		me.choices[i] = tempRes;	//Put choice value into choices array
		if(tempCum>0) {				//Make a new entry only if prob density > 0
			cum += tempCum;			//Increment cumulative prob by density
			me.cumulative[cum] = i;	//Create entry in the one-to-one cumulative map
		}
	}

	return stream;
}

#endif