#include "Precompiled.h"

#include "Block.h"
#include "Exeption.h"
#include "Expression.h"

CBlock::CBlock(CGeneralObject* iOwner) : CTerm()
{ 
	Owner=iOwner; 
	SelfName="block"; 
	ElseTerm=NULL;
};

CBlock::~CBlock()
{
/*	TNameTable::iterator i;
	for (i = NameTable.begin(); i != NameTable.end(); i++)
		if((*i).second) {delete (*i).second;(*i).second=NULL;}

	if(NoNamedElem) {delete NoNamedElem; NoNamedElem=NULL;}
	if(ElseTerm) {delete ElseTerm; ElseTerm=NULL;}*/
}

void CBlock::InsertNObject(TNamedObject Object)
{
	NameTable.insert(Object);
};

void CBlock::AddElem(CExpression* Expr)
{
	NoNamedElemList.push_back(Expr);
};

void* CBlock::GetObject(string Name)
{
	if(NameTable.find(Name)!=NameTable.end())
		return NameTable[Name];
	if(Owner) return Owner->GetObject(Name);
	return NULL;
}

bool CBlock::GetVal(string Str,CFishka* &Rezult)
{
	vector<TLocalValTableElement>::iterator iter=LocalValTable.end();
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
void CBlock::AddVal(string Str,CFishka* Val)
{
	if(NameTable.find(Str)!=NameTable.end())
	{
		vector<TLocalValTableElement>::iterator iter=LocalValTable.end();
		iter--;
		(*iter).LocalValMap[Str]=Val;
	}
	else ((CBlock*)Owner)->AddVal(Str,Val);
}

bool CBlock::GetFishka(CFishka* Arg,CFishka* &Rezult,bool &Trace,CInterpretator* Interpretator)
{
	if(NoNamedElemList.empty()&&NameTable.empty()) 
	{
		CGeneralObject *Tmp=Owner;
		while(!(Owner->IsFunc())) 
			Tmp=Tmp->Owner;
		throw CExeption((CFunction*)Tmp,"Отсутсвует интерпретируемое выражение\nв одном из блоков функции ");
	}
	PLocalValTableElement Tmp=new TLocalValTableElement;
//	Tmp->RezFishka=NULL;
	LocalValTable.push_back(*Tmp);
	bool Rez=true;
	vector<CExpression*>::iterator i;
	map<string, CGeneralObject*, less<string> >::iterator j;

	for(j=NameTable.begin();j!=NameTable.end();j++)
	{
		Rez&=(*j).second->GetFishka(Arg,Rezult,Trace,Interpretator);
		AddVal((*j).first,Rezult);
	}
	for(i=NoNamedElemList.begin();i!=NoNamedElemList.end();i++)
	{
		Rez&=(*i)->GetFishka(Arg,Rezult,Trace,Interpretator);
	}

	vector<TLocalValTableElement>::iterator Iterator;
	Iterator=LocalValTable.end();
	Iterator--;

	Rezult=(*Iterator).LocalValMap["break"];

	LocalValTable.pop_back();

	if(!Rezult) 
	{
		CGeneralObject *Tmp=Owner;
		while(!(Owner->IsFunc())) Tmp=Tmp->Owner;
		throw CExeption((CFunction*)Tmp,"Отсутсвует break выражение\nв одном из блоков функции ");
	}

	//Rezult=m_pRezFishka;
	return Rez;
};
//---------------------------------------------------------------
//---------------------------------------------------------------
/*void CBlock::SetRezFishka(CFishka* Fishka)
{
	vector<TLocalValTableElement>::iterator i;
	i=LocalValTable.end();
	i--;

	if((*i).RezFishka) 
	{
		CGeneralObject *Tmp=Owner;
		while(!(Owner->IsFunc())) Tmp=Tmp->Owner;
		throw CExeption((CFunction*)Tmp,"Несколько break выражений\nв одном из блоков функции ");
	}

	i->RezFishka=Fishka;
};*/


