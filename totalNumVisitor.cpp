#include "totalNumVisitor.h"

//Loops through BovineGroups in herd and tallies number of bovines using
//the getNumTotal BovineGroup function.
void TotalNumVisitor::visit(BovineHerd* herd) {
	BovineHerd::iterator end = herd->end();
	for(BovineHerd::iterator it = herd->begin(); it != end; ++it)
		total += it->getNumTotal();
}

