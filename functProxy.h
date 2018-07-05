#ifndef FUNCT_PROXY_H
#define FUNCT_PROXY_H

#include <iostream>
#include "funct.h"
#include "functTable.h"
#include "functInterStep.h"
#include "functInterLine.h"
#include "functDistrDisc.h"
#include "functDistrUnif.h"
#include "functDistrGaus.h"

using std::istream;
using std::ostream;

template<class ARG, class RES> class FunctProxy;
template<class ARG, class RES> istream& operator>>(istream&, FunctProxy<ARG, RES> &);
template<class ARG, class RES> ostream& operator<<(ostream&, FunctProxy<ARG, RES> &);

template<class ARG, class RES> class FunctProxy {

	friend istream& operator>> (istream&, FunctProxy&);
	friend ostream& operator<< (ostream&, FunctProxy&);

	public:
		virtual RES operator()(ARG&);

	public:
	   ~FunctProxy();
		FunctProxy();
		FunctProxy(Funct<ARG, RES>*);
	private:
		FunctProxy(FunctProxy const&) {}

	private:
		FunctProxy* _funct;
};

template<class ARG, class RES> inline
RES FunctProxy<ARG, RES>::operator()(ARG& arg) {
	return _funct->operator()(arg);
}

template<class ARG, class RES> inline
FunctProxy<ARG, RES>::~FunctProxy() {
	if(_funct) delete _funct;
}

template<class ARG, class RES> inline
FunctProxy<ARG, RES>::FunctProxy() : _funct(0) {}

template<class ARG, class RES> inline
FunctProxy<ARG, RES>::FunctProxy(Funct<ARG, RES>* newFunc) {
	newFunc->setProxy();
	_funct = newFunc;
}

template<class ARG, class RES> istream& operator>>
(istream& stream, FunctProxy<ARG, RES>& proxy) {

	if(_funct) delete _funct;

	string functType;
	stream >> functType;

	     if(functType == "<functTable>")     _funct = new FunctTable();
	else if(functType == "<functInterStep>") _funct = new FunctInterStep();
	else if(functType == "<functInterLine>") _funct = new FunctInterLine();
	else if(functType == "<functDistrDisc>") _funct = new FunctDistrDisc();
	else if(functType == "<functDistrUnif>") _funct = new FunctDistrUnif();
	else if(functType == "<functDistrGaus>") _funct = new FunctDistrGaus();
	else {
		cerr << "Uknown Funct type " << functType << endl;
		exit(0);
	}

	_funct->setProxy(true);

	stream >> *_funct;
	return stream;
}

template<class ARG, class RES> ostream& operator<<
(ostream& stream, FunctProxy<ARG, RES>& proxy) {
	stream << *_funct;
}

#endif