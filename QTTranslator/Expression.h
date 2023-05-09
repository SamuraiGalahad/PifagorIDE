#ifndef __EXPRESSION_H_HEADER_INCLUDED__
#define __EXPRESSION_H_HEADER_INCLUDED__

#include "GeneralObject.h"

class CTerm;

class CExpression : public CGeneralObject
{
public:
	CExpression(CGeneralObject* iOwner) : CGeneralObject()
	{
		Owner=iOwner;
		SelfName="expression";
	};
	virtual  ~CExpression();

	void AddTerm(CTerm* Term);
	void AddTermBeforeEnd(CTerm* Term);
	virtual bool GetFishka(CFishka* Arg,CFishka* &Rezult,bool &Trace,CInterpretator* Interpretator);
protected:
	vector<CTerm*> TermList;
};

#endif
