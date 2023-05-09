// SignalFishka.cpp: implementation of the CSignalFishka class.
//
//////////////////////////////////////////////////////////////////////
#include <string.h>

#include "Precompiled.h"
#include "SignalFishka.h"
#include "Scaner.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSignalFishka::CSignalFishka() : CFishka()
{
	Type=SIGNAL_CONST;
}

CSignalFishka::~CSignalFishka()
{

}

void CSignalFishka::OutConsole()
{
	strcat(data,"signal fishka");
}

