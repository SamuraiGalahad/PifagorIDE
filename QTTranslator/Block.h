#ifndef __BLOCK_H_HEADER_INCLUDED__
#define __BLOCK_H_HEADER_INCLUDED__

#include "Term.h"

class CBlock : public CTerm
{
public:
	CBlock(CGeneralObject* iOwner);
	virtual ~CBlock();

	virtual void InsertNObject(TNamedObject Object);
    virtual void AddElem(CExpression* Expr);
    virtual void* GetObject(std::string Name);
    virtual bool GetFishka(CFishka* Arg,CFishka* &Rezult,bool &Trace,CInterpretator* Interpretator);
    virtual bool GetVal(std::string Str,CFishka* &Rezult);
    virtual void AddVal(std::string Str,CFishka* Val);
//	virtual void SetRezFishka(CFishka* Fishka);

	TLocalValTable LocalValTable;

    std::vector<CExpression*> NoNamedElemList;
	TNameTable NameTable;
};

#endif

