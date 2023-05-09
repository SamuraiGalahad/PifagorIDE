#include <string.h>
#include <iostream>

#include "Interface.h"
void DW(char *s);

extern "C" void* oiTranslate(char* FName, char* Err,int& rez)
{
    /// Debug information
    ///std::cout << "Start of Translator" << std::endl;

	int L,C;
	CScaner *Scaner=new CScaner();
	CTranslator* castedTrans=new CTranslator(Scaner);
	castedTrans->EraseGlobalObjectTable();
	CModule *curMod=new CModule;
	try
	{
		castedTrans->Translate(FName,curMod);
	}
	catch(CExeption Tmp)
	{
		castedTrans->GetCurrPos(L,C);
		if(C<0)
			L--;
		strcpy(Err,(char*)Tmp.GetErrMess());
		rez=L;
		return (void*)curMod;
	};
	castedTrans->GetCurrPos(L,C);
	if(C<0)
		L--;
	rez=-L;
	return (void*)curMod;
}

extern "C" void oiRetFuncList(void* curMod, char* fList, int& stringNum)
{
	CModule* castedModule;
	char *tmpList=new char[10000];
	castedModule=(CModule*)curMod;
	stringNum=0;
	TNameTable::iterator it;
	TNameTable::iterator ist;
	if(castedModule)
	{
		strcpy(fList,"Argument's evaluation");
		stringNum++;
		for(it=castedModule->NameTable.begin();it!=castedModule->NameTable.end();it++)
		{
			sprintf(tmpList,"%s\n%s",fList,((*it).first.c_str()));
			strcpy(fList,tmpList);
			stringNum++;
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Функция, осуществляющая трансляцию аргумента
extern "C" void* oiArgTranslate(void* curMod, char* arg, char* Err, bool& rez)
{
    /// Debug information
    ///std::cout << "Start of ArgTranslator" << std::endl;

    // Передается модуль
    CModule* castedModule;
    castedModule=(CModule*)curMod;

    CFishka* InputFishka=NULL,*RezFishka=NULL;
    CTerm* ArgObject=NULL;
    bool Trace=false;

    CArgScaner* ArgScaner=new CArgScaner(arg);
    CArgTranslator* ArgTranslator=new CArgTranslator(ArgScaner,castedModule);
    CTerm* Term;

    // Осуществляется трансляция аргумента
    try
    {
        if(!(ArgTranslator->Translate(NULL,Term)))
        {
            delete ArgScaner;
            delete ArgTranslator;
            strcpy(Err,"Syntax argument error");
            rez=false;
            return NULL;
        }
    }
    catch(CExeption Exept)
    {
        sprintf(Err,"Syntax argument error: %s",(char*)Exept.GetErrMess());
        rez=false;
        return NULL;
    }

    CFunction* Func=new CFunction(castedModule);
    Func->ParamID="$$$";
    Term->Owner=Func;
    bool ArgTrace=false;
    try
    {
        CInterpretator Interpretator;
        Term->GetFishka(NULL,InputFishka,ArgTrace,&Interpretator);
    }
    catch(CExeption Tmp)
    {
        string Str;
        if(Tmp.m_pContext)
            Str=Tmp.m_sMessage+Tmp.m_pContext->SelfName;
        else
            Str=Tmp.m_sMessage;
        strcpy(Err,Str.c_str());
        delete Func;
        delete ArgScaner;
        delete ArgTranslator;
        rez=false;
        return NULL;
    }
    delete Func;
    delete ArgScaner;
    delete ArgTranslator;
    rez=true;
    /// Debug information
    ///std::cout << "The End of ArgTranslator" << std::endl;
    return (void*)InputFishka;
}

extern "C" char* oiInterp(void* inFishka, void *curMod, char *name, char* Err, bool& rez, bool trace)
{
    /// Debug information
    ///std::cout << "Start of Interpretator" << std::endl;

    /// Пока непонятно, что и зачем запоминается
    //ofstream out1("x.myinit");
    //out1 << "0";
    //out1.close();

	StopThread=false;
	data[0]='\0';
	CFishka *Arg=(CFishka*)inFishka;
	CFishka *RezFishka;
	CModule *castedModule=(CModule*)curMod;
	if(!strcmp(name,"Argument's evaluation"))
	{
            Arg->OutConsole();
			/*Tmp->Parent->m_bDebugSession=false;
			GlobalDebugSession=false;
			Tmp->Parent->m_bRunSession=false;
			GlobalRunSession=false;*/
			rez=true;
			return data;
	}
    /// Далее запускается интерпретатор
	try
	{
        CInterpretator Interpretator;
		if(castedModule->Call(name,Arg,RezFishka,trace,&Interpretator))
		{
            RezFishka->OutConsole();
            /// Debug Information
            ///std::cout << "RezFishka->OutConsole() after Call: " << data << std::endl;
		}
	}
	catch(CExeption Tmp)
	{
		if(Tmp.m_iErrCode!=19)
		{
			string Str;
			/*if(Tmp.m_pContext)	
				Str=Tmp.m_sMessage+Tmp.m_pContext->SelfName;
			else*/ Str=Tmp.m_sMessage;
			strcpy(Err,Str.c_str());			
		}
		rez=false;
        /// Debug Information
        ///std::cout << "CExeption after Call: " << data << std::endl;
        return data;
	}
	catch(...)
	{
		strcpy(Err,"Unknown exception");
		rez=false;
        /// Debug Information
        ///std::cout << "...Exeption after Call: " << data << std::endl;
        return data;
	}
	/*Tmp->Parent->m_bDebugSession=false;
	GlobalDebugSession=false;
	Tmp->Parent->m_bRunSession=false;
	GlobalRunSession=false;*/
	rez=true;
    /// Debug Information
    ///std::cout << "oiInterp: " << data << std::endl;
    return data;
}

extern "C" void oiMakeStep()
{
    WaitForSignal = false;
}

extern "C" char* oiGetCurResult()
{
	return data;
}

extern "C" void oiSetDataBuffer(const char *cStrValue)
{
    strcpy(data, cStrValue);
    return;
}

extern "C" void  oiBreakWork()
{
	StopThread=true;
}
