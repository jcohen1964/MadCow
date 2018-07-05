#include "totalInfectedVisitor.h"

//Loops through BovineGroups in herd and tallies number of infected bovines using
//the getNumSick BovineGroup function.
void TotalInfectedVisitor::visit(BovineHerd* herd) {
	BovineHerd::iterator end = herd->end();
	for(BovineHerd::iterator it = herd->begin(); it != end; ++it)
		total += it->getNumSick();
}