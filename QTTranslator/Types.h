#ifndef __TYPES_H_HEADER_INCLUDED__
#define __TYPES_H_HEADER_INCLUDED__

#include <map>
#include <vector>

class CFunction;
class CGeneralObject;
class CFishka;

typedef map<string, CGeneralObject*, less<string> > TNameTable;
typedef pair<string,CGeneralObject*> TNamedObject;

typedef multimap<float, CFunction*, less<float> > TOverFuncTableElement;
typedef map<string, TOverFuncTableElement, less<string> > TOverFuncTable;

struct TLocalValTableElement
{
	map<string,CFishka*> LocalValMap;
//	CFishka* RezFishka;
};

typedef TLocalValTableElement* PLocalValTableElement;

typedef vector<TLocalValTableElement> TLocalValTable;

#endif
