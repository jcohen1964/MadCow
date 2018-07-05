#ifndef CONTROL_H
#define CONTROL_H

#include <iostream>

using std::string;
using std::istream;


class ControlParms;

class ControlParms {
	friend istream& operator>> (istream&, ControlParms&);

	public:
		int			outputInterval;
		int			numMonths;
		enum		BoolTokens {FALSE, TRUE, ENDBOOLTOKENS};
		bool		reportDetailedFoodData;
		bool		treatNonAmbAsDead;

	private:
		static void readError();

};

istream& operator>> (istream&, ControlParms&);
std::istream& operator>>(std::istream&, ControlParms::BoolTokens&);

#endif


