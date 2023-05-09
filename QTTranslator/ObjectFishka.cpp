#include <string.h>

#include "Precompiled.h"

#include "ObjectFishka.h"
#include "GeneralObject.h"

CObjectFishka::CObjectFishka(CFishka* Fishka)
{
	Type=Fishka->Type;
	Object=((CObjectFishka*)Fishka)->Object;
};

CObjectFishka::CObjectFishka(ELexTypeEnum iType,CGeneralObject* iObject)
{
	Type=iType;
	Object=iObject;
};

CGeneralObject* CObjectFishka::GetObject() 
{ 
	return Object;
};

void CObjectFishka::OutConsole()
{
	strcat(data,(Object->SelfName.c_str()));
};

