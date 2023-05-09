#include "Precompiled.h"

#include "KeyWord.h"
#include "Fishka.h"
#include "AtomFishka.h"




CKeyWord::CKeyWord(TLexem Lexem) : CTerm()
{
	Val=Lexem.Lex;
	SelfName="keyword";
	ElseTerm=NULL;
};

CKeyWord::~CKeyWord() 
{ 
};

bool CKeyWord::GetFishka(CFishka* Arg,CFishka* &Rezult,bool &Trace,CInterpretator* Interpretator)
{
	if(Rezult=new CAtomFishka<ELexTypeEnum>(Val)) return true;
	return false;
}


