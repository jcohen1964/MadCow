#ifndef STUNNER_H
#define STUNNER_H

#include <vector>
#include <string>
#include <iostream>
#include "sickBovine.h"
#include "organ.h"
#include "FunctTable.h"
#include "FunctInterStep.h"
#include "emboli.h"

using std::istream;
using std::string;

class Stunner;
istream& operator>> (istream&, Stunner&);

class Stunner {

	friend istream& operator>> (istream&, Stunner&);

	public:
		enum Type   {BOLT, PNEUMO, END_TYPE};
		enum Status {OK, NOK, END_STATUS};

		void process   (std::vector<Organ>&);

	private:
		void distEmboli(std::vector<Organ>&);

		static void readError();

		static FunctTable<Type, double> probType;
		static FunctTable<Type, double> probOK;
		static FunctTable<Type, FunctTable<Status, FunctTable<Organ::Type, Emboli> > > emboli;

		static FunctTable<Type, double> probDrip;
		static double fracDrip;
};

istream& operator>>(istream&, Stunner::Type&);
istream& operator>>(istream&, Stunner::Status&);

#endif