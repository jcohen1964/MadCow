#include "organ.h"
#include <string>
#include <iostream>

std::istream& operator>>(std::istream& stream, Organ::Type& type) {
	std::string    token;
	stream >> token;
	if     (token == "BRAIN"    ) type = Organ::BRAIN;
	else if(token == "SPINAL"   ) type = Organ::SPINAL;
	else if(token == "DRG"      ) type = Organ::DRG;
	else if(token == "BLOOD"    ) type = Organ::BLOOD;
	else if(token == "GUT"      ) type = Organ::GUT;
	else if(token == "HEART"    ) type = Organ::HEART;
	else if(token == "LUNG"     ) type = Organ::LUNG;
	else if(token == "LIVER"    ) type = Organ::LIVER;
	else if(token == "KIDNEY"   ) type = Organ::KIDNEY;
	else if(token == "ILLEUM"   ) type = Organ::ILLEUM;
	else if(token == "EYES"     ) type = Organ::EYES;
	else if(token == "AMRMEAT"  ) type = Organ::AMRMEAT;
	else if(token == "MUSCLE"   ) type = Organ::MUSCLE;
	else if(token == "BONE"     ) type = Organ::BONE;
	else if(token == "TGGANGLIA") type = Organ::TGGANGLIA;
	else if(token == "TONSILS"  ) type = Organ::TONSILS;
	else if(token == "EXTRA1"   ) type = Organ::EXTRA1;
	else if(token == "EXTRA2"   ) type = Organ::EXTRA2;
	else if(token == "EXTRA3"   ) type = Organ::EXTRA3;
	else {std::cerr << "Organ::Type " << token << " unknown"; exit(0);}
	return stream;
}
