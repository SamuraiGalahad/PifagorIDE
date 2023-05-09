// WrapFishka.cpp: implementation of the CWrapFishka class.
//
//////////////////////////////////////////////////////////////////////
#include <string.h>

#include "Precompiled.h"
#include "WrapFishka.h"
#include "Scaner.h"
#include "ObjectFishka.h"
#include "GeneralObject.h"

char data[10000000];

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWrapFishka::CWrapFishka(CObjectFishka* iWrap,CFishka* iVal)
{
	Wrap=iWrap;
	Val=iVal;
	Type=WRAP_FISHKA;
}

CWrapFishka::~CWrapFishka()
{

}


void CWrapFishka::OutConsole()
{
	strcat(data,"(");
	strcat(data,Wrap->GetObject()->SelfName.c_str());
	strcat(data,",");
	Val->OutConsole();
	strcat(data,")");
};
