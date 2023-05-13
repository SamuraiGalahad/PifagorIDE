#include "Precompiled.h"

#include "Function.h"
#include "Exeption.h"
#include "Expression.h"

CFunction::CFunction(CGeneralObject* iOwner) : CBlock(iOwner)
{
	Owner=iOwner;
	SelfName="function";
	ElseTerm=NULL;
}; 

CFunction::~CFunction()
{
/*	TNameTable::iterator i;
	for (i = NameTable.begin(); i != NameTable.end(); i++)
		if((*i).second) {delete (*i).second;(*i).second=NULL;}

	if(NoNamedElem) { delete NoNamedElem; NoNamedElem=NULL;}
	if(ElseTerm) { delete ElseTerm;	ElseTerm=NULL;}*/
}

bool CFunction::GetFishka(CFishka* Arg,CFishka* &Rezult,bool &Trace,CInterpretator* Interpretator) 
{
//	m_pRezFishka=NULL;
	if(NoNamedElemList.empty()&&NameTable.empty()) 
		throw CExeption(this,"There is no interpretable expression in function ");

	PLocalValTableElement Tmp=new TLocalValTableElement;
//	Tmp->RezFishka=NULL;
	LocalValTable.push_back(*Tmp);

    std::vector<CExpression*>::iterator i;
    std::map<std::string, CGeneralObject*, std::less<std::string> >::iterator j;

	bool Rez=true;

	//int Size=NameTable.size();

	for(j=NameTable.begin();j!=NameTable.end();j++)
    {
        std::string name=(*j).first;
		//CGeneralObject* Obj=(*j).second;
		Rez&=(*j).second->GetFishka(Arg,Rezult,Trace,Interpretator);
		AddVal((*j).first,Rezult);
	}
	for(i=NoNamedElemList.begin();i!=NoNamedElemList.end();i++)
	{
		Rez&=(*i)->GetFishka(Arg,Rezult,Trace,Interpretator);
	}

    std::vector<TLocalValTableElement>::iterator Iterator;
	Iterator=LocalValTable.end();
	Iterator--;

	Rezult=(*Iterator).LocalValMap["return"];

	LocalValTable.pop_back();

	if(!Rezult) 
		throw CExeption(this,"There is no return expression in function ");

	//Rezult=m_pRezFishka;
	return Rez;
};
//---------------------------------------------------------------
//---------------------------------------------------------------
/*void CFunction::SetRezFishka(CFishka* Fishka)
{
	vector<TLocalValTableElement>::iterator i;
	i=LocalValTable.end();
	i--;

	if((*i).RezFishka) 
		throw CExeption(this,"Несколько return выражений в функции ");

	(*i).RezFishka=Fishka;
};*/

void CFunction::AddElem(CExpression* Expr)
{
	NoNamedElemList.push_back(Expr);
}
void CFunction::InsertNObject(TNamedObject Object)
{
	NameTable.insert(Object);
}

void* CFunction::GetObject(std::string Name)
{
	//CGeneralObject* Obj;
	if(NameTable.find(Name)!=NameTable.end())
		return NameTable[Name];
	if(Owner) return Owner->GetObject(Name);
	return NULL;
}

bool CFunction::GetVal(std::string Str,CFishka* &Rezult)
{
    std::vector<TLocalValTableElement>::iterator iter=LocalValTable.end();
	iter--;
	if((*iter).LocalValMap.find(Str)!=(*iter).LocalValMap.end())
	{
		Rezult=(*iter).LocalValMap[Str];
		return true;
	}
	if(NameTable.find(Str)!=NameTable.end()) return false;
	else return ((CBlock*)Owner)->GetVal(Str,Rezult);
	return false;
}
void CFunction::AddVal(std::string Str,CFishka* Val)
{
	if(NameTable.find(Str)!=NameTable.end())
    {
        std::vector<TLocalValTableElement>::iterator iter=LocalValTable.end();
		iter--;
		(*iter).LocalValMap[Str]=Val;
	}
	else ((CBlock*)Owner)->AddVal(Str,Val);
}
 
