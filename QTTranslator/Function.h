#ifndef __FUNCTION_H_HEADER_INCLUDED__
#define __FUNCTION_H_HEADER_INCLUDED__

#include "Block.h"

class CFunction : public CBlock
{
public:
	CFunction(CGeneralObject* iOwner); 
	virtual ~CFunction();

	virtual void AddElem(CExpression* Expr);
	virtual void InsertNObject(TNamedObject Object);
	virtual void* GetObject(string Name);
	virtual bool GetFishka(CFishka* Arg,CFishka* &Rezult,bool &Trace,CInterpretator* Interpretator);
	virtual bool IsFunc() {return true;};
	virtual bool GetVal(string Str,CFishka* &Rezult);
	virtual void AddVal(string Str,CFishka* Val);
//	virtual void SetRezFishka(CFishka* Fishka);
	string ParamID;
	int Line;
	int EndLine;
};

#endif
