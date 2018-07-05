#ifndef FOOD_INSPECTOR_H
#define FOOD_INSPECTOR_H

#include <vector>
#include <iostream>
#include "organ.h"
#include "functTable.h"
#include "FunctInterStep.h"

using std::istream;

class FoodInspector;
istream& operator>> (istream&, FoodInspector&);

class FoodInspector {
	friend istream& operator>> (istream&, FoodInspector&);

	public:
		void process(std::vector<Organ>&);

	private:
		static void readError();
		static		FunctInterStep<int, FunctTable<Organ::Type, double> > probPassFood;
};

#endif