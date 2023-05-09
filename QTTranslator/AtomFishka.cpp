#include <string.h>

#include "AtomFishka.h"
#include "Scaner.h"

extern TKWTabElement KWTab[KWTSIZE];
extern char* RuntimeErrTable[ERRCOUNT];
template<>
CAtomFishka<int>::CAtomFishka(int iVal)
{
	Type=INT_CONST;
	Val=iVal;
}
template<>
CAtomFishka<float>::CAtomFishka(float iVal)
{
	Type=REAL_CONST;
	Val=iVal;
}
template<>
CAtomFishka<bool>::CAtomFishka(bool iVal)
{
	Type=BOOL_CONST;
	Val=iVal;
}
template<>
CAtomFishka<char>::CAtomFishka(char iVal)
{
	Type=CHAR_CONST;
	Val=iVal;
}
template<>
CAtomFishka<ELexTypeEnum>::CAtomFishka(ELexTypeEnum iVal)
{
	Type=iVal;
	Val=iVal;
}
template<>
CAtomFishka<ERuntimeErrorEnum>::CAtomFishka(ERuntimeErrorEnum iVal)
{
	Type=ERROR_CONST;
	Val=iVal;
}
//---------------------------------------------------------------
//---------------------------------------------------------------
template<>
void CAtomFishka<bool>::OutConsole()
{
	if(Val) strcat(data,"true");
	else strcat(data,"false");

};
template<>
void CAtomFishka<char>::OutConsole()
{
	sprintf(data, "%s'%c'",data, Val);
};
template<>
void CAtomFishka<int>::OutConsole()
{
	sprintf(data, "%s%d",data, Val);
};
template<>
void CAtomFishka<float>::OutConsole()
{
	sprintf(data, "%s%f",data, Val);
};
template<>
void CAtomFishka<ELexTypeEnum>::OutConsole()
{
	switch(Val)
	{
	case OPADD:		{ strcat(data,"+") ;break;};
	case OPSUB:		{ strcat(data,"-") ;break;};
	case OPMUL:		{ strcat(data,"*") ;break;};
	case OPDIV:		{ strcat(data,"/") ;break;};
	case OPIDIV:	{ strcat(data,"%") ;break;};
	case OPRS:		{ strcat(data,"()") ;break;};
	case OPCS:		{ strcat(data,"[]") ;break;};
	case OPFS:		{ strcat(data,"{}") ;break;};
	case OPDDOT:	{ strcat(data,"..") ;break;};
	case OPDOT:		{ strcat(data,".") ;break;};
	case OPL:		{ strcat(data,"<") ;break;};
	case OPG:		{ strcat(data,">") ;break;};
	case OPLEN:		{ strcat(data,"|") ;break;};
	case OPTRANS:	{ strcat(data,"#") ;break;};
	case OPLE:		{ strcat(data,"=<") ;break;};
	case OPGE:		{ strcat(data,">=") ;break;};
	case OPE:		{ strcat(data,"=") ;break;};
	case OPNE:		{ strcat(data,"!=") ;break;};
	case OPQ:		{ strcat(data,"?") ;break;};
	case OPDUP:		{ strcat(data,"dup") ;break;};
	case OPLASS:	{ strcat(data,"<<") ;break;};
	case OPRASS:	{ strcat(data,">>") ;break;};
	default:        {if(Val>LDEF_NAME) strcat(data,KWTab[Val-KWBLOCK].Str); break;}
	};
}
template<>
void CAtomFishka<ERuntimeErrorEnum>::OutConsole()
{
	sprintf(data, "%s%s",data, RuntimeErrTable[Val]);
}
