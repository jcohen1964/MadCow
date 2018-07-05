#include "totalClinicalVisitor.h"

//Visits all BovineGroups in herd and tallies number of clinical bovines
void TotalClinicalVisitor::visit(BovineHerd* herd) {
	total = 0;
	BovineHerd::iterator end = herd->end();
	for(BovineHerd::iterator it = herd->begin(); it != end; ++it)
		it->accept(*this);
}

//Loops through SickBovine list and increments total each time a clinical bovine is
//identified.
void TotalClinicalVisitor::visit(BovineGroup* group) {
	BovineGroup::SickBovineIterator end = group->getSickEnd();
	for(BovineGroup::SickBovineIterator it = group->getSickBegin(); it != end; ++it) {
		if(it->isClinical) {total++;};
	}
}
