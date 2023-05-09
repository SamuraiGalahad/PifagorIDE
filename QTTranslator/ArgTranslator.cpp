// ArgTranslator.cpp: implementation of the CArgTranslator class.
//
//////////////////////////////////////////////////////////////////////

#include "ArgTranslator.h"
#include "Exeption.h"
#include "Program.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArgTranslator::CArgTranslator(CScaner* Scaner,CModule* Module) : CTranslator(Scaner)
{
	m_pModule=Module;
}

CArgTranslator::~CArgTranslator()
{

}

bool CArgTranslator::Translate(char *FName, CTerm* &iTerm)
{
	m_pScaner->OpenAllFiles(NULL);
	m_pScaner->GetNextLexem(m_tlLexem);
	try
	{
		if(!Term((CGeneralObject*)m_pModule,iTerm)) return false;
	}
	catch(CExeption Tmp)
	{
		m_pScaner->CloseAllFiles();
		iTerm=NULL;
		throw Tmp;
		return false;
	}
	return true;
}

bool CArgTranslator::IDFind(string ID,CGeneralObject* Owner,CGeneralObject* &Object)
{
	CGeneralObject* Tmp;
	CModule* OModule;
	Tmp=Owner;
	while(Tmp->Owner) Tmp=Tmp->Owner;
	OModule=(CModule*)Tmp;
	Tmp=Owner;
	if(OModule->NameTable.find(ID)!=OModule->NameTable.end()) 
	{
		Object=OModule->NameTable[ID];
		return true;
	}
	
	while(Owner!=OModule)
	{
		if(((CBlock*)Owner)->NameTable.find(ID)!=((CBlock*)Owner)->NameTable.end())
		{
			Object=((CBlock*)Owner)->NameTable[ID];
			return true;
		}
		Owner=Owner->Owner;
	};

	if(OModule->ConstTable.find(ID)!=OModule->ConstTable.end())
	{
		Object=OModule->ConstTable[ID];
		return true;
	}
	return false;
}
