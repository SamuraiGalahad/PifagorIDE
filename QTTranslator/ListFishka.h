#ifndef __LISTFISHKA_H_HEADER_INCLUDED__
#define __LISTFISHKA_H_HEADER_INCLUDED__

#include "Fishka.h"

class CListFishka : public CFishka
{
public:
	CListFishka(CFishka* Fishka);
	CListFishka(ELexTypeEnum iType,int iElemCount);
	
	~CListFishka();

	virtual void OutConsole();
	virtual CGeneralObject* GetObject();
	CFishka* &operator[](int i);
	CFishka* operator[](int i) const;
	CFishka* CheckOut();

	int ElemCount;
private:
	CFishka** Elements;
};

#endif
