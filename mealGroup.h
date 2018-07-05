#ifndef MEAL_GROUP_H
#define MEAL_GROUP_H

#include <list>
#include "meal.h"

class MealGroup : public std::list<Meal> {
	public:
		void addMeal(Meal const& meal) {push_back(meal);}
};
#endif

