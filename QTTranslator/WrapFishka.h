// WrapFishka.h: interface for the CWrapFishka class.
//
//////////////////////////////////////////////////////////////////////

#ifndef AFX_WRAPFISHKA_H__284B11C1_5262_11D7_BB4F_CFC98C227E47__INCLUDED_
#define AFX_WRAPFISHKA_H__284B11C1_5262_11D7_BB4F_CFC98C227E47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Fishka.h"

class CObjectFishka;

class CWrapFishka : public CFishka  
{
public:
	CWrapFishka(CObjectFishka* iWrap,CFishka* iVal);
	virtual ~CWrapFishka();

	virtual void OutConsole();

	CObjectFishka* Wrap;
	CFishka* Val;
};

#endif // !defined(AFX_WRAPFISHKA_H__284B11C1_5262_11D7_BB4F_CFC98C227E47__INCLUDED_)
