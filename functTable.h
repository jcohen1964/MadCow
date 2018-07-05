#ifndef FUNCT_TABLE_H
#define FUNCT_TABLE_H

#include <map>
#include <iostream>

using std::istream;
using std::ostream;

template<class ARG, class RES> class FunctTable;
template<class ARG, class RES> istream& operator>>(istream&, FunctTable<ARG, RES> &);
template<class ARG, class RES> ostream& operator<<(ostream&, FunctTable<ARG, RES> const&);

template<class ARG, class RES> class FunctTable {
	friend istream& operator>> (istream&, FunctTable<ARG, RES> &); //A MSVC++ bug prevents binding to the appropriate template function via operator>> <ARG, RES>
	friend ostream& operator<< (ostream&, FunctTable<ARG, RES> const&); //A MSVC++ bug prevents binding to the appropriate template function via operator<< <ARG, RES>

	public:
		RES operator()(ARG const&) const;

	public:
		std::map<ARG, RES> table;
		typedef std::map<ARG, RES>::const_iterator const_iterator;
};

template<class ARG, class RES> inline
RES FunctTable<ARG, RES>::operator()(ARG const& argument) const{
		const_iterator result;
		result = table.lower_bound(argument); 
		if (result->first == argument) return result->second; //Exact match
		//Otherwise resutn a default RES
		return RES();
}

template<class ARG, class RES> inline
istream& operator>>(istream& stream, FunctTable<ARG, RES>& source) {
	source.table.clear();
	int domainSize;
	ARG argument;
	RES result;
	stream >> domainSize;
	while(domainSize--) {
		stream >> argument;
		stream >> result;
		source.table[argument] = result;
	}
	return stream;
}

template<class ARG, class RES> inline
ostream& operator<<(ostream& stream, FunctTable<ARG, RES>& sink) {

	Function<ARG, RES>::const_iterator i, end;
	end = sink.table.end();
	stream << sink.table.size() << endl;
	for(i = sink.table.begin(); i != end; ++i) {
		stream << i->first << '\t' << i->second << endl;
	}
	return stream;
}

#endif