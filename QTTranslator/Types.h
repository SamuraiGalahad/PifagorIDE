#ifndef __TYPES_H_HEADER_INCLUDED__
#define __TYPES_H_HEADER_INCLUDED__

#include <map>
#include <vector>
#include <string>

class CFunction;
class CGeneralObject;
class CFishka;

typedef std::map<std::string, CGeneralObject*, std::less<std::string> > TNameTable;
typedef std::pair<std::string,CGeneralObject*> TNamedObject;

typedef std::multimap<float, CFunction*, std::less<float> > TOverFuncTableElement;
typedef std::map<std::string, TOverFuncTableElement, std::less<std::string> > TOverFuncTable;

struct TLocalValTableElement
{
    std::map<std::string,CFishka*> LocalValMap;
//	CFishka* RezFishka;
};

typedef TLocalValTableElement* PLocalValTableElement;

typedef std::vector<TLocalValTableElement> TLocalValTable;

#endif
