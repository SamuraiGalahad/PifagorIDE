// SignalFishka.h: interface for the CSignalFishka class.
//
//////////////////////////////////////////////////////////////////////

#ifndef AFX_SIGNALFISHKA_H__851F93A1_E528_11D6_AB5E_9DD64A6BAD4D__INCLUDED_
#define AFX_SIGNALFISHKA_H__851F93A1_E528_11D6_AB5E_9DD64A6BAD4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Fishka.h"

class CSignalFishka : public CFishka  
{
public:
	CSignalFishka();
	virtual ~CSignalFishka();

	virtual void OutConsole();
};

#endif // !defined(AFX_SIGNALFISHKA_H__851F93A1_E528_11D6_AB5E_9DD64A6BAD4D__INCLUDED_)
