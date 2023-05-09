#include <iostream>

#include "Precompiled.h"
#include "Program.h"

CModule::CModule() : CBlock(NULL) 
{
	Owner=NULL;
	SelfName="module";
	ElseTerm=NULL;
	//NoNamedElem=NULL;
};

CModule::~CModule()
{
/*	TNameTable::iterator i;
	for (i = NameTable.begin(); i != NameTable.end(); i++)
		if(((*i).second)!=NULL) 
		{
			delete (*i).second; 
			(*i).second=NULL;
		};

	for (i = ConstTable.begin(); i != ConstTable.end(); i++)
		if(((*i).second)!=NULL) 
		{
			delete (*i).second; 
			(*i).second=NULL;
		}

	if(NoNamedElem) {delete NoNamedElem; NoNamedElem=NULL;}
	if(ElseTerm) {delete ElseTerm; ElseTerm=NULL;}*/
}
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CModule::GetFishka(CFishka* Arg,CFishka* &Rezult,bool &Trace,CInterpretator* Interpretator) 
{
	return false;
};

//---------------------------------------------------------------
//---------------------------------------------------------------
void CModule::InsertNObject(TNamedObject Object)
{
	NameTable[Object.first]=Object.second;
};

void CModule::InsertOverFunc(TNamedObject Object,float Rang)
{
	OverFuncTable[Object.first].insert(make_pair(Rang,(CFunction*)(Object.second)));
};

void CModule::InsertConstant(TNamedObject Object)
{
	ConstTable.insert(Object);
};
void CModule::InsertUserType(TNamedObject Object)
{
	TypeDefTable.insert(Object);
}

void* CModule::GetObject(string Name)
{
	CGeneralObject* Obj;
	if(Obj=ConstTable[Name]) return Obj;
	return NULL;
};
void CModule::OutNameTable()
{
	TNameTable::iterator i;
	//int size=NameTable.size();
	for (i = NameTable.begin(); i != NameTable.end(); i++);	
		//cout << (*i).first << endl;
};

bool CModule::Call(string Name,CFishka* Arg,CFishka* &Rezult,bool &Trace,CInterpretator* Interpretator)
{
	PLocalValTableElement Tmp=new TLocalValTableElement;
	Rezult=NULL;
	LocalValTable.push_back(*Tmp);
	bool Rez=(NameTable[Name]->GetFishka(Arg,Rezult,Trace,Interpretator));
	LocalValTable.pop_back();

    return Rez;
}


bool CModule::GetVal(string Str,CFishka* &Rezult)
{
	vector<TLocalValTableElement>::iterator iter=LocalValTable.end();
	iter--;
	//int S1=LocalValTable.size();
	//int S2=(*iter).LocalValMap.size();
 	if((*iter).LocalValMap.find(Str)!=(*iter).LocalValMap.end())
	{
		Rezult=(*iter).LocalValMap[Str];
		return true;
	}
	return false;
}
void CModule::AddVal(string Str,CFishka* Val)
{
	if(ConstTable.find(Str)!=ConstTable.end())
	{
		vector<TLocalValTableElement>::iterator iter=LocalValTable.end();
		iter--;
		(*iter).LocalValMap[Str]=Val;
	}
}

