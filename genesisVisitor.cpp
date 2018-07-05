#include "genesisVisitor.h"
#include <iostream>

//Takes three arguments (bovine type, gender, and age in months) and returns number of animals to be 
//included in the corresponding bovineGroup at the beginning of the simulation
FunctTable<Bovine::Type, FunctTable<Bovine::Gender, FunctInterStep<int, int> > > GenesisVisitor::initSize;

//Populate all BovineGroups in the herd...
void GenesisVisitor::visit(BovineHerd* herd) {
	BovineHerd::iterator it, end;					//Alias iterator over herd BovineGroup list
	end = herd->end();								//Alias end marker for herd BovineGroup list
	for(it = herd->begin(); it != end; ++it) {		//Loop over all BovineGroups in herd
		int             age = it.getAge();			//Alias age for this BovineGroup
		Bovine::Type   type = it.getType();			//Alias type for this BovineGroup
		Bovine::Gender gend = it.getGender();		//Alias gender for this BovineGroup
		int size = initSize(type)(gend)(age);		//Lookup initial size of this BovineGroup
		Bovine bovine(type, gend, age);				//Create a representative bovine for this BovineGroup
		BovineGroup group(bovine, size);			//Create a BovineGroup using bovine template and initSize
		*it = group;								//Insert this newly created BovineGroup into the herd.
	}
}

//Takes BovineGroup and populates it with the number of cattle specified by initSize
void GenesisVisitor::visit(BovineGroup* group) {
		Bovine bovine = group->getBovine();			//bovine is a representative bovine in group
		int             age = bovine.getAge();		//retrieve age from bovine
		Bovine::Type   type = bovine.getType();		//retrieve type from bovine
		Bovine::Gender gend = bovine.getGender();	//retrieve gender from bovine
		*group = BovineGroup(bovine,				//Create BovineGroup with the identified age, type, and
			initSize(type)(gend)(age));				//gender characteristics and initSize animals.
}

#include <string>
#include <iostream>

void GenesisVisitor::readError() {
	std::cerr << "Error reading GenesisVisitor data" << std::endl;
	exit(0);
}

std::istream& operator>>(std::istream& stream, GenesisVisitor& me) {
	std::string word;
	stream >> word;
	char dump[256];

	if(word == "<initSize>") {
		stream.getline(dump, 256).getline(dump, 256) >> me.initSize;
	} else GenesisVisitor::readError();
	stream >> word;
	if(word != "</initSize>") GenesisVisitor::readError();
	stream >> word;
	if(word != "</genesisVisitor>") GenesisVisitor::readError();

	return stream;
}
