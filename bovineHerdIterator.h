#ifndef BOVINE_HERD_ITERATOR_H
#define BOVINE_HERD_ITERATOR_H

#include "bovine.h"

template<class HERD> class BovineHerdIterator {

	friend HERD;

	public:
		BovineHerdIterator(BovineHerdIterator const&);

		typedef Bovine::Type     Type;
		typedef Bovine::Gender   Gender;
		typedef HERD::value_type value_type;

		bool                operator==(BovineHerdIterator const&) const;
		bool                operator!=(BovineHerdIterator const&) const;
		BovineHerdIterator& operator= (BovineHerdIterator const&);
		BovineHerdIterator& operator++();
		BovineHerdIterator  operator++(int);
		BovineHerdIterator& operator--();
		BovineHerdIterator  operator--(int);
		value_type&         operator* () const;
		value_type*         operator->() const;

		BovineHerdIterator() {}

		int            getAge()    {return curAge;}
		Bovine::Type   getType()   {return curType;}
		Bovine::Gender getGender() {return curGender;}

	private:
		BovineHerdIterator(Type, Gender, int age, BovineHerd*);

	private:
		int         curAge;
		Type        curType;
		Gender      curGender;
		HERD*       myHerd;
};

template<class HERD> inline
bool BovineHerdIterator<HERD>::operator==(BovineHerdIterator const& compare) const {
	return (curType   == compare.curType   &&
			curGender == compare.curGender &&
			curAge    == compare.curAge    );
}

template<class HERD> inline
bool BovineHerdIterator<HERD>::operator!=(BovineHerdIterator const& compare) const {
	return (curType   != compare.curType   ||
			curGender != compare.curGender ||
			curAge    != compare.curAge    );
}

template<class HERD> inline
BovineHerdIterator<HERD>& BovineHerdIterator<HERD>::operator++() {
	if(curAge < Bovine::END_AGE) {
		++curAge;
	} else {
		curAge = 0;
		if(curGender == Bovine::MALE) {
			curGender = Bovine::FEMALE;
		} else {
			curGender = Bovine::MALE;
			if(curType == Bovine::BEEF) {
				curType = Bovine::BEEFREPRO;
			} else {
				if(curType == Bovine::BEEFREPRO) {
					curType = Bovine::DAIRY;
				} else {
				curAge    = Bovine::END_AGE;
				curGender = Bovine::END_GENDER;
				curType   = Bovine::END_TYPE;
				}
			}
		}
	}
	return *this;
}

template<class HERD> inline
BovineHerdIterator<HERD>& BovineHerdIterator<HERD>::operator--() {
	if(curAge > 0) {
		--curAge;
	} else {
		curAge = Bovine::END_AGE - 1;
		if(curGender == Bovine::FEMALE) {
			curGender = Bovine::MALE;
		} else {
			curGender = Bovine::FEMALE;
			if(curType == Bovine::DAIRY) {
				curType = Bovine::BEEF;
			} else {
				curAge    = 0;
				curGender = Bovine::MALE;
				curType   = Bovine::BEEF;
			}
		}
	}
	return *this;
}

template<class HERD> inline
BovineHerdIterator<HERD> BovineHerdIterator<HERD>::operator++(int) {
	BovineHerdIterator temp(*this);
	operator++();
	return temp;
}

template<class HERD> inline
BovineHerdIterator<HERD> BovineHerdIterator<HERD>::operator--(int) {
	BovineHerdIterator temp(*this);
	operator--();
	return temp;
}

template<class HERD> inline
BovineHerdIterator<HERD>::value_type& BovineHerdIterator<HERD>::operator* () const {
	return   myHerd->operator()(curType,curGender,curAge);
}

template<class HERD> inline
BovineHerdIterator<HERD>::value_type* BovineHerdIterator<HERD>::operator->() const {
	return &(myHerd->operator()(curType,curGender,curAge));
}

template<class HERD>
BovineHerdIterator<HERD>::BovineHerdIterator(BovineHerdIterator const& iter) :
myHerd(iter.myHerd), curType(iter.curType),
curGender(iter.curGender), curAge(iter.curAge) {}

template<class HERD>
BovineHerdIterator<HERD>::BovineHerdIterator
(Type type, Gender gender, int age, BovineHerd* herd) :
curType(type), curGender(gender), curAge(age), myHerd(herd) {}

template<class HERD>
BovineHerdIterator<HERD>& BovineHerdIterator<HERD>::operator= (BovineHerdIterator const& iter) {
	curType  = iter.curType;
	curGender= iter.curGender;
	curAge   = iter.curAge;
	myHerd   = iter.myHerd;
	return *this;
}

#endif