#include "Precompiled.h"

#include "Identifier.h"
#include "Program.h"
#include "Function.h"
#include "Fishka.h"
#include "Exeption.h"
#include "ObjectFishka.h"
#include "Scaner.h"
#include "ListFishka.h"

CIdentifier::CIdentifier(CGeneralObject* iOwner,TLexem Lexem) : CTerm()
{
    Owner=iOwner;
    Str=std::string(Lexem.Str);
	SelfName="id";
	ElseTerm=NULL;
};


bool CIdentifier::GetFishka(CFishka* Arg,CFishka* &Rezult,bool &Trace,CInterpretator* Interpretator)
{
	CGeneralObject* Tmp;
	CModule* OModule;
	Tmp=Owner;
	while(Tmp->Owner) Tmp=Tmp->Owner;
	OModule=(CModule*)Tmp;
	Tmp=Owner;
	if(Tmp!=((CGeneralObject*)OModule))
	{
		while((Tmp->Owner)!=((CGeneralObject*)OModule)) 
			Tmp=Tmp->Owner;	
		if(Str==(((CFunction*)Tmp)->ParamID)) 
		{
			if(Arg)
			{
				Rezult=GetCopyFishka(Arg);
				return true;
			}
			else 
				throw CExeption((CFunction*)Tmp,"Аргумент функции не инициализирован в функции: ");
		};
	}
	if(OModule->NameTable.find(Str)!=OModule->NameTable.end()) 
	{
		if(Rezult = new CObjectFishka(FUNC_FISHKA,OModule->NameTable[Str])) return true;
	}
	if(OModule->TypeDefTable.find(Str)!=OModule->TypeDefTable.end()) 
	{
		if(Rezult = new CObjectFishka(TYPEDEF_FISHKA,OModule->TypeDefTable[Str])) return true;
	}
	if(OModule->OverFuncTable.find(Str)!=OModule->OverFuncTable.end()) 
	{
		TOverFuncTableElement Element=OModule->OverFuncTable[Str];
		TOverFuncTableElement::iterator it;
		int Count=Element.size();
		Rezult=new CListFishka(PAR_LIST,Count);
		int Index=0;
		for(it=Element.begin();it!=Element.end();it++)
		{
			(*((CListFishka*)Rezult))[Index]=new CObjectFishka(FUNC_FISHKA,(*it).second);
			Index++;
		}
		if(Rezult) return true;
	}
	if(((CBlock*)Owner)->GetVal(Str,Rezult)) return true;
	Tmp=(CGeneralObject*)(Owner->GetObject(Str));
	if(Tmp)	
		if(Tmp->GetFishka(Arg,Rezult,Trace,Interpretator))
		{
			((CBlock*)Owner)->AddVal(Str,Rezult);
			return true;
		}
	Tmp=this;
	while(Tmp->Owner!=OModule) Tmp=Tmp->Owner;
	printf("Error in function \"%s\":\nIdentifier \"%s\" NOT found !!!\n",Tmp->SelfName.c_str(),Str.c_str());
	return false;
}
