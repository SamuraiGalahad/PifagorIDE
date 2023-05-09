// ArgTranslator.h: interface for the CArgTranslator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARGTRANSLATOR_H__0E91E6E1_0A07_11D6_AB5E_F8880A02B24D__INCLUDED_)
#define AFX_ARGTRANSLATOR_H__0E91E6E1_0A07_11D6_AB5E_F8880A02B24D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Translator.h"

class CArgTranslator : public CTranslator  
{
public:
	CArgTranslator(CScaner* Scaner,CModule* Module);
	virtual ~CArgTranslator();
	
	virtual bool Translate(char *FName, CTerm* &iTerm);
	virtual bool IDFind(string ID,CGeneralObject* Owner,CGeneralObject* &Object);

	virtual bool Delay_List(CGeneralObject* NameOwner,CTerm* &Trm) { return false; };
protected:
	CModule* m_pModule;
};

#endif // !defined(AFX_ARGTRANSLATOR_H__0E91E6E1_0A07_11D6_AB5E_F8880A02B24D__INCLUDED_)
