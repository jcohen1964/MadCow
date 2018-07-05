#ifndef RENDERER_DEAD_H
#define RENDERER_DEAD_H

#include <iostream>
#include <vector>
#include <string>
#include "meal.h"
#include "mealGroup.h"
#include "organ.h"
#include "functTable.h"
#include "functDistrDisc.h"

using std::istream;
using std::string;

class Renderer_Dead;
istream& operator>> (istream&, Renderer_Dead&);

class Renderer_Dead {
	friend istream& operator>> (istream&, Renderer_Dead&);

	public:

		Renderer();
		MealGroup process(std::vector<Organ>&);

	public:
		static double probContamination;
		static FunctDistrDisc<int, double> renderFactor;
		static FunctTable<Meal::ProducerType, double> probType;
		static FunctTable<Meal::ProducerType, double> probMisLabel;
		static double fracContaminate;

	private:
		static void readError();
};

#endif