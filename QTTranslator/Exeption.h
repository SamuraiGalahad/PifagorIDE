#ifndef __EXEPTION_H_HEADER_INCLUDED__
#define __EXEPTION_H_HEADER_INCLUDED__

#include "Precompiled.h"

class CFunction;


class CExeption  
{
public:
	CExeption(CFunction* Context,string Message,int ErrCode=0); 
	virtual ~CExeption();
	
	virtual const char* GetErrMess();

	string m_sMessage;
	CFunction* m_pContext;
	int m_iErrCode;
};

#endif // !defined(AFX_EXEPTION_H__DF5BF683_DEC4_11D5_AB5D_C749D8462B4C__INCLUDED_)
