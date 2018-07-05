#include "bovine.h"
#include "bovineGroup.h"
#include <iostream>

//Create an empty BovineGroup
BovineGroup::BovineGroup() :
	myNumTotal(0),
	myNumHealthy(0),
	myNumSick(0) {}

//Constructor using BovineGroup using group fed as argument as a template
BovineGroup::BovineGroup(BovineGroup const& group) :
	myBovine(group.myBovine),								//myBovine set to group.myBovine
	myNumTotal(group.myNumTotal),							//myNumTotal set to group.myNumTotal
	myNumHealthy(group.myNumHealthy),						//myNumHealthy set to group.myNumHealthy
	myNumSick(group.myNumSick),								//myNumSick set to group.myNumSick
	mySickBovines(group.mySickBovines) {					//mySickBovines list set to group.mySickBovines
		SickBovineIterator end = mySickBovines.end();		//Point end to end of mySickBovines list
		for(SickBovineIterator it = mySickBovines.begin();	//loop through mySickBovines
		                       it != end; ++it)
			it->myGroup = this;								//for each bovine, myGroup points to
	}														//group just created.

//Constructor using using a bovine (bovine) and number of cattle (num) to be included 
//as arguments... Creates a group with num healthy cattle (default 0). 
BovineGroup::BovineGroup(Bovine const & bovine, int numTotal) :
	myBovine(bovine),										//myBovine set to bovine argument
	myNumTotal(numTotal),									//myNumTotal set to numTotal
	myNumHealthy(numTotal),									//myNumHealthy set to numTotal
	myNumSick(0) {}											//myNumSick set to zero

//Assignment operator -- Replace contents of BovineGroup with contents of group argument
BovineGroup& BovineGroup::operator=(BovineGroup const& group) {
	myBovine = group.myBovine;								//myBovine set to group myBovine
	myNumTotal = group.myNumTotal;							//myNumTotal set to group.myNumTotal
	myNumHealthy = group.myNumHealthy;						//myNumHealth set to group.myNumHealthy
	myNumSick = group.myNumSick;							//myNumSick set to group.myNumSick
	std::list<SickBovine> newSickBovines(group.mySickBovines);	//newSickBovines set to group.mySickBovines
	mySickBovines.clear();									//delete existing list of sickBovines
	mySickBovines.splice(mySickBovines.end(), newSickBovines);	//Add group.mySickBovines to list
	SickBovineIterator end = mySickBovines.end();			//point end of mySickBovines list
	for(SickBovineIterator it = mySickBovines.begin();		//loop through list
		                   it != end; ++it)
		it->myGroup = this;									//for each bovine in list, myGroup points to
															//group just created.
	return *this;											//Return pointer to bovineGroup fed as argument
}

//Add num healthy bovines to self.BovineGroup 
void BovineGroup::addBovine(int num) {
	myNumTotal += num;										
	myNumHealthy += num;	
}

//Add bovine to self.SickBovine list
void BovineGroup::addBovine(Bovine const& bovine) {

	SickBovine const* ptrSickBovine;						//Create pointer to sickBovine
	if(ptrSickBovine =										//If a SickBovine ptr can be created from bovine
		dynamic_cast<SickBovine const*>(&bovine)) {			//create the ptr.
		myNumSick++;										//Increment myNumSick
		myNumTotal++;										//Increment myNumTotal
		SickBovine newBovine(*ptrSickBovine);				//Create a SickBovine called newBovine
		newBovine.myGroup = this;							//newBovine.myGroup pts to group that called fctn
		mySickBovines.push_back(newBovine);					//put newBovine onto sickBovine list

	} else {												//If SickBovine ptr cannot be created...
		myNumTotal++;										//Increment myNumTotal
		myNumHealthy++;										//Increment myNumHealthy
	}

}

//Add BovineGroup group to self.
void BovineGroup::addBovineGroup(BovineGroup const& group) {
	myNumSick += group.myNumSick;							//Increment myNumSick by group.MyNumSick
	myNumTotal += group.myNumTotal;							//Increment myNumTotal by group.MyNumTotal
	myNumHealthy += group.myNumHealthy;						//Increment myNumHealthy by group.MyNumHealthy
	std::list<SickBovine> newSickBovines(group.mySickBovines);	//Create newSickBovines list = group.mySickBovines
	SickBovineIterator end = newSickBovines.end();			//SickBovines.end points to end of group.mySickBovines
	for(SickBovineIterator it = newSickBovines.begin();		//loop through sickBovines being added
		                   it != end; ++it)	
		it->myGroup = this;									//myGroup points to BovineGroup that called fctn
	mySickBovines.splice(mySickBovines.end(), newSickBovines);	//Splice on new list of SickBovines
}

//Remove one healthy bovine (if there are any left) from this group.
Bovine BovineGroup::removeBovine() {
	if(myNumHealthy > 0) {
		myNumHealthy--;
		myNumTotal--;
	}
	return Bovine(myBovine);								//Return a bovine of type removed.
}


//Remove num healthy Bovines
BovineGroup BovineGroup::removeBovine(int num) {
	if(myNumHealthy < num) {					//If myNumHealthy < num
		int temp = myNumHealthy;				//store original myNumHealthy value
		myNumTotal -= myNumHealthy;				//Reduce myNumTotal by myNumHealthy
		myNumHealthy = 0;						//Set myNumHealthy to zero.
		return BovineGroup(myBovine, temp);		//Return BovineGroup containing Bovines removed
	} else {									//If myNumHealthy >= num
		myNumHealthy -= num;					//Reduce myNumHealthy by num
		myNumTotal   -= num;					//Reduce myNumTotal by num
		return BovineGroup(myBovine, num);		//return BovineGroup containing Bovines removed
	}
}

//Remove one Bovine from the SickBovine list
BovineGroup::SickBovineIterator BovineGroup::removeBovine(SickBovineIterator iterator) {
	myNumSick--;								//Decrement myNumSick
	myNumTotal--;								//Decrement myNumTotal
	return(mySickBovines.erase(iterator));		//Remove sickBovine pointed to by iterator
}

//Search for bovine in SickBovine list and remove any matches found
void BovineGroup::removeBovine(SickBovine bovine) {
	SickBovineIterator it, end;							//Initialize iterator and end ptr
	end = mySickBovines.end();							//Point end to end of SickBovine list
	for(it = mySickBovines.begin(); it != end; ++it) {	//loop through SickBovine list
		if(*it == bovine) {								//If there is a match
			myNumSick--;								//decrement myNumSick
			myNumTotal--;								//decrement myNumTotal
			mySickBovines.erase(it);					//REmove sickBovine ptd to by iterator
			return;
		}
	}

}

#include "random.h"

//Randomly removes one animal from BovineGroup.  Return the removed Bovine.
Bovine* BovineGroup::removeRandomBovine() {

	if(myNumTotal <= 0) return new Bovine(myBovine);		//If no bovines left in BovineGroup 
															//remove none.
	Random& generator = *(Random::instance());

	double unif = generator.nextAngle();					//Generate u = U[0,1]
	if(unif <= double(myNumHealthy)/double(myNumTotal)) {	//If u < myNumHealthy/myNumTotal
		myNumTotal--;										//decrement myNumTotal
		myNumHealthy--;										//decrement myNumHealthy
		return new Bovine(removeBovine());					//create and return bovine removed
	} else {												//if u >= myNumHealthy/myNumTotal
		int unif = generator.nextAngle() * mySickBovines.size();	//Generate u = U[0,number of sick bovines]
		SickBovineIterator iterator;						//iterator over SickBovine list
		iterator = mySickBovines.begin();					//start iterator at beginning of list
		while(unif--) iterator++;							//iterate until reaching entry in list > u
		if(iterator == mySickBovines.end())					//if we reach end of list w/o deleting a Bovine
			std::cerr << "ERROR!!! CONTACT duggar@mit.edu with code ERROR::bovineGroupNumbers" << std::endl << std::ends;
		if(mySickBovines.end() == mySickBovines.begin())	//if list is empty
			std::cerr << "ERROR!!! CONTACT duggar@mit.edu with code ERROR::bovineGroupNumbers" << std::endl << std::ends;
		SickBovine* result = new SickBovine(*iterator);		//Create bovine eliminated
		result->myGroup = 0;								//set myGroup of return Bovine to zero
		removeBovine(iterator);								//Remove the identified Bovine from the list
		return result;										//Return the removed Bovine
	}
}

//getBovinesSlaughtered.  Returns a list of bovines to be slaughtered.  These
//bovines fall into two categories: 1) bovines scheduled to be slaughtered
//i.e., bovine age >= slaughter age; and 2) a fraction of the bovines 
//that have no slaughter age scheduled.  Returns a list of the slaughtered
//bovines.
BovineGroup BovineGroup::getBovinesSlaughtered(double fraction) {

	BovineGroup result(myBovine, 0);	//Store BovineGroup of zero myBovines in result
	int numFutureSchedSlaughter = 0;	//Bovines to be slaughtered on schedule after this month
	SickBovineIterator it = mySickBovines.begin();	
										//iterator over SickBovine list
	int schedSlaughterCount = 0;		//Number of sched slaughters

	//Deal with bovines sched to be slaughtered...
	while(it != mySickBovines.end()) {		//loop until reaching the end of the list
		if (it->getAgeSlaughter() >= 0 && it->getAgeSlaughter() <= Bovine::END_AGE+1) {
			if (it->getAgeSlaughter() <= this->getBovine().getAge()) {
										 	//If animal is sched to be slaughtered now
				result.addBovine(*it);		//Add current bovine to the result BovineGroup
				it = removeBovine(it);		//Remove current bovine from the SickBovineList
				schedSlaughterCount++;		//Increment count
			} else {
				numFutureSchedSlaughter++;	//If animal sched to be slaughtered later, count
				it++;
			}
		} else {
			it++;
		}
	}

	//Remove fraction of bovines not scheduled to be slaughtered...
	int chosen = 0;							//Number of unscheduled slaughers identified so far
	int tested = 0;							//Number of SickBovines checked in mySickBovines list
	Random& generator = *(Random::instance());
	double unif = 0;
	int numNotSched = myNumSick - numFutureSchedSlaughter;
	int numToRemove = fraction*(myNumTotal - numFutureSchedSlaughter);
											//Total bovines to be slaughtered
	it = mySickBovines.begin();				//Reset it to beginning of SickBovine list
	while((chosen < numToRemove) && (it != mySickBovines.end())) {
											//loop until reaching the end of the list
											//or choosing numToRemove animals
		while (it != mySickBovines.end() && 
			it->getAgeSlaughter() >= 0 && it->getAgeSlaughter() <= Bovine::END_AGE) {
											//Skip over animals scheduled to be slaughtered in future;
			it++;
		}
		if (it != mySickBovines.end()) {	//If there are bovines to be removed...
			unif = generator.nextFloat();	//u = U[0,1]
			if(unif <= 
				double(numToRemove - chosen)/double(myNumTotal - tested)) {
											//If u <= choose probability for remaining bovines...
					result.addBovine(*it);	//Add current bovine to the result BovineGroup
					it = removeBovine(it);	//Remove current bovine from the SickBovineList
					chosen++;				//Increment chosen
			} else {						//If u > (numToRemove-chosen)/(myNumTotal-tested)
				it++;						//proceed to next bovine in SickBovineList
				tested++;					//Increment tested
			}
		}
	}

	int healthySlaughtered = numToRemove - chosen;
											//Healthy slaughters = number to remove - sick removed
	result.addBovine(healthySlaughtered);	//Store in result BovineGroup with num-chosen bovines 
											//(i.e. num bovines remaining)
	myNumTotal -= healthySlaughtered;		//decrement myNumTotal by num bovines remaining
	myNumHealthy -= healthySlaughtered;		//decrement myNumHealthy by num bovines remaining

	return result;							//Return BovineGroup with num Bovines
}


//Remove num Bovines from BovineGroup.  Return BovineGroup with bovines removed.
BovineGroup BovineGroup::removeRandomBovineGroup(int num) {
	if(myNumTotal < num) {				//If myNumTotal < num to be removed
		BovineGroup result(*this);		//Store copy of BovineGroup in result
		myNumTotal = 0;					//set myNumTotal to zero
		myNumHealthy = 0;				//set myNumHealthy to zero
		myNumSick = 0;					//set myNumSick to zero
		mySickBovines.clear();			//Remove all animals on the SickBovine list
		return result;					//return result
	}

	int chosen = 0;								//Number of SickBovines thus far selected for removal
	int tested = 0;								//Number of SickBovines checked in mySickBovines list
	Random& generator = *(Random::instance());
	double unif = 0;

	BovineGroup result(myBovine, 0);						//Store BovineGroup of zero myBovines in result
	SickBovineIterator iterator = mySickBovines.begin();	//iterator over SickBovine list
	while((chosen < num) && (iterator !=					//loop until reaching the end of the list
		mySickBovines.end())) {
		unif = generator.nextFloat();						//u = U[0,1]
		if(unif <= double(num - chosen) /					//If u <= (num-chosen)/(myNumTotal-tested)
			double(myNumTotal - tested)) {
			result.addBovine(*iterator);					//Add current bovine to the result BovineGroup
			iterator = removeBovine(iterator);				//Remove current bovine from the SickBovineList
			chosen++;										//Increment chosen
		} else												//If u > (num-chosen)/(myNumTotal-tested)
			iterator++;										//proceed to next bovine in SickBovineList
		tested++;											//Increment tested
	}

	result.addBovine(num - chosen);			//Store in result BovineGroup with num-chosen bovines 
											//(i.e. num bovines remaining)
	myNumTotal -= num - chosen;				//decrement myNumTotal by num bovines remaining
	myNumHealthy -= num - chosen;			//decrement myNumHealthy by num bovines remaining

	return result;							//Return BovineGroup with num Bovines
}

//Age all bovines in BovineGroup 
void BovineGroup::live() {
	myBovine.live();									//Age myBovine by 1 month
	SickBovineIterator end = mySickBovines.end();		//Point end to end of mySickBovines list

	for(SickBovineIterator it =							//loop through mySickBovines list
		mySickBovines.begin(); it != end; ++it) {
		it->live();										//Age bovines by 1 month
	}
}
