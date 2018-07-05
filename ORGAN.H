#ifndef ORGAN_H
#define ORGAN_H

struct Organ {
	enum Type {
			BRAIN	,       // 0
			SPINAL	,       // 1
			DRG		,       // 2
			BLOOD	,       // 3
			GUT		,       // 4
			HEART	,       // 5
			LUNG	,       // 6
			LIVER	,       // 7
			KIDNEY	,       // 8
			ILLEUM	,       // 9
			EYES	,       // 10
			MUSCLE  ,       // 11
			AMRMEAT ,       // 12
			BONE    ,       // 13
			TGGANGLIA,		// 14
			TONSILS,		// 15
			EXTRA1,			// 16
			EXTRA2,			// 17
			EXTRA3,			// 18
			END_TYPE};

	Organ() : type(END_TYPE), ID50(0), emboli(false), passPM(true), forHumanFood(false) {}

	Type      type;
	double     age;
	double    ID50;
	bool    emboli;
	bool	passPM;
	bool    forHumanFood;
	bool	sourceClinical;
	bool	sourceNonAmbulatory;
};

#include <istream>
std::istream& operator>>(std::istream&, Organ::Type&);

#endif