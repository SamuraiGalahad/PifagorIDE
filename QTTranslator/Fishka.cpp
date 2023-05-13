//#include "Precompiled.h"

#include "Fishka.h"
#include "List.h"
#include "AtomFishka.h"
#include "ObjectFishka.h"
#include "ListFishka.h"
#include "WrapFishka.h"

extern TKWTabElement KWTab[10000];

const char* RuntimeErrTable[ERRCOUNT]=
	{"No Errors!",
	"Unknown error!",
	"Error type cast(real)!",
	"Error type cast (int)!",
	"Zero division!",
	"Interpretation error!",
	"Bound error!",
	"No expression error!",
	"Error type cast!",
	"Value extract error!"};
//---------------------------------------------------------------
//---------------------------------------------------------------
CFishka* GetCopyFishka(CFishka* Fishka)
{
	switch(Fishka->Type)
	{
	case FUNC_FISHKA:{return new CObjectFishka(Fishka);break;};
	case DELAY_FISHKA:{return new CObjectFishka(Fishka);break;};
	case DATA_LIST:{return new CListFishka(Fishka);break;};
	case PAR_LIST:{return new CListFishka(Fishka);break;};
	case DELAY_LIST:{return new CListFishka(Fishka);break;};
	case INT_CONST:{return new CAtomFishka<int>(Fishka);break;};
	case REAL_CONST:{return new CAtomFishka<float>(Fishka);break;};
	case BOOL_CONST:{return new CAtomFishka<bool>(Fishka);break;};
	case CHAR_CONST:{return new CAtomFishka<char>(Fishka);break;};
//////////////////////////////////////////////////////////	case CHAR_CONST:{return new CAtomFishka<char>(Fishka);break;};
	case ERROR_CONST:{return new CAtomFishka<ERuntimeErrorEnum>(Fishka);break;};
	case WRAP_FISHKA:{return new CWrapFishka(((CWrapFishka*)Fishka)->Wrap,
						 ((CWrapFishka*)Fishka)->Val); break;};
	case TYPEDEF_FISHKA: { return new CObjectFishka(Fishka); break; };
	default:{return new CAtomFishka<ELexTypeEnum>(Fishka);break;};
	};
};
//---------------------------------------------------------------
//---------------------------------------------------------------
