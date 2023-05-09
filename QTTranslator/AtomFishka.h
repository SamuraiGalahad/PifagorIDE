#ifndef __ATOMFISHKA_H_HEADER_INCLUDED__
#define __ATOMFISHKA_H_HEADER_INCLUDED__

#include "Fishka.h"

template<class T> class CAtomFishka:public CFishka
{
public:
	CAtomFishka(T iVal);
	CAtomFishka(CFishka* Fishka)
	{
		Type=Fishka->Type;
		Val=((CAtomFishka*)Fishka)->Val;
	}
	virtual void OutConsole();
	/*{
		(*IOBufferPtr)<<Val;
	};*/
	T Val;
};

#endif
