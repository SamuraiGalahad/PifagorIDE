#ifndef __PROGRAM_H_HEADER_INCLUDED__
#define __PROGRAM_H_HEADER_INCLUDED__

//#include "Objects.h"
#include "Block.h"
//---------------------------------------------------------------
//---------------------------------------------------------------
class CModule : public CBlock
{
public:
	CModule();
	virtual ~CModule();

	virtual void InsertNObject(TNamedObject Object);
    virtual void InsertOverFunc(TNamedObject Object,float Rang);
    virtual void* GetObject(std::string Name);
    virtual bool GetFishka(CFishka* Arg,CFishka* &Rezult,bool &Trace,CInterpretator* Interpretator);
    virtual bool GetVal(std::string Str,CFishka* &Rezult);
    virtual void AddVal(std::string Str,CFishka* Val);
    bool Call(std::string Name,CFishka* Arg,CFishka* &Rezult,bool &Trace,CInterpretator* Interpretator);
	void InsertConstant(TNamedObject Object);
	void InsertUserType(TNamedObject Object);
    void OutNameTable();
    std::string StartID;
	TNameTable ConstTable;
	TNameTable TypeDefTable;
	TOverFuncTable OverFuncTable;
};



#endif
