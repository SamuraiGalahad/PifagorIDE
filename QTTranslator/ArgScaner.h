// ArgScaner.h: interface for the CArgScaner class.
//
//////////////////////////////////////////////////////////////////////

#ifndef AFX_ARGSCANER_H__DF47EA21_F7AE_11D5_AB5D_ED51CD1C5954__INCLUDED_
#define AFX_ARGSCANER_H__DF47EA21_F7AE_11D5_AB5D_ED51CD1C5954__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Scaner.h"


class CArgScaner : public CScaner  
{
public:
	CArgScaner(char* newArg);
	virtual ~CArgScaner();

	int GetChar(FILE *F);
	bool OpenAllFiles(char *InFileName);

	virtual void CloseAllFiles() { }; 
protected:
	char* m_sArgument;
	int m_iChPos;
};

#endif // !defined(AFX_ARGSCANER_H__DF47EA21_F7AE_11D5_AB5D_ED51CD1C5954__INCLUDED_)
