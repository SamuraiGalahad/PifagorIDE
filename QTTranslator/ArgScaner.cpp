// ArgScaner.cpp: implementation of the CArgScaner class.
//
//////////////////////////////////////////////////////////////////////
#include <string.h>

#include "ArgScaner.h"


/*#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CArgScaner::CArgScaner(char* newArg)
{
	m_cInChar='\255';
	m_iLNumber=1;
	m_iCNumber=0;
	m_iLexCount=0;
	m_iChPos=0;
	m_sArgument=new char[strlen(newArg)+1];
	strcpy(m_sArgument,newArg);
}
//////////////////////////////////////////////////////////////////////
CArgScaner::~CArgScaner()
{

}
//////////////////////////////////////////////////////////////////////
bool CArgScaner::OpenAllFiles(char *InFileName) 
{
	m_cInChar='\255';
	m_iLNumber=1;
	m_iCNumber=0;
	m_iLexCount=0;
	m_iChPos=0;
	FILE *out;
	out=fopen("arg.arg","w");
	fprintf(out,"%s",m_sArgument);
	fclose(out);
	m_fpInFile=fopen("arg.arg","r");
	return true;
};
//////////////////////////////////////////////////////////////////////
int CArgScaner::GetChar(FILE *F) 
{ 
	int StringLen=strlen(m_sArgument);
	if(m_iChPos>=StringLen) return EOF;
	return m_sArgument[m_iChPos++];
};
//////////////////////////////////////////////////////////////////////

