#ifndef __OBJECTFISHKA_H_HEADER_INCLUDED__
#define __OBJECTFISHKA_H_HEADER_INCLUDED__

#include "Fishka.h"

class CObjectFishka:public CFishka
{
public:
	CObjectFishka(CFishka* Fishka);
	CObjectFishka(ELexTypeEnum iType,CGeneralObject* iObject);
	virtual void OutConsole();
	virtual CGeneralObject* GetObject();
private:
	CGeneralObject* Object;
};

#endif

