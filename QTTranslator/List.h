#ifndef __LIST_H_HEADER_INCLUDED__
#define __LIST_H_HEADER_INCLUDED__

#include "Term.h"
#include "Scaner.h"

class CGeneralObject;
enum ELexTypeEnum;

class CList : public CTerm
{
public:
	CList(CGeneralObject* Owner,ELexTypeEnum iType);
	virtual  ~CList();

	virtual void AddElem(CExpression* Expr);
	virtual bool GetFishka(CFishka* Arg,CFishka* &Rezult,bool &Trace,CInterpretator* Interpretator);
	virtual bool IsList() {return true;};
private:
    std::vector<CExpression*> ElemList;
	ELexTypeEnum Type;
};

#endif
