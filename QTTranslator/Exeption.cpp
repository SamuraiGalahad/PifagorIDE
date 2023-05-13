#include "Precompiled.h"

#include "Exeption.h"

#define ERR_MES_TABLE_SIZE	23

const char *ErrMesTable[ERR_MES_TABLE_SIZE] =
	{   
		"No Error!",	
		"Unknown syntax error", //1
		"Missed \";\"", //2
		"Function isn't implemented", //3
		"Function is already defined", //4
		"Identificator is already defined as constant identificator", //5
		"Constant is already defined", //6
		"Identificator is already defined as function identificator", //7		
		"Identificator is already defined", //8
		"Missed \"{\"", //9
		"Missed unnamed expression in function", //10
		"Right naming is missed", //11 
		"Constant is missed", //12
		"Missed \",\" between list's element", //13
		"Term is missed (atom, list, block or identificator)", //14
		"Unknown identificator", //15
		"Constant term is missed (atom, list, block or identificator)", //16
		"Found keyword \"break\" out of block", //17
		"Found keyword \"return\" out of function", //18
		"Programm execution interrupted by user's request", //19
		"Identificator is already defined as type identificator", // 20
		"Identificator is already defined as overloaded function identificator", //21
		"No Error!"
	};

CExeption::CExeption(CFunction* Context,std::string Message,int ErrCode)
{
	m_sMessage=Message; 
	m_pContext=Context; 
	m_iErrCode=ErrCode; 
};

CExeption::~CExeption() 
{ 
};

const char* CExeption::GetErrMess()
{
	if((m_iErrCode<0)||(m_iErrCode>(ERR_MES_TABLE_SIZE-1)))
		return NULL;
	return ErrMesTable[m_iErrCode];
};

