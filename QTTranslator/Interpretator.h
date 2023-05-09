#ifndef __INTERPR_H_HEADER_INCLUDED__
#define __INTERPR_H_HEADER_INCLUDED__

class CFishka;
class CListFishka;

#include "AtomFishka.h"

class CInterpretator
{
public:
	CInterpretator();
	virtual ~CInterpretator();

	bool InterpretationOperator(CFishka* FuncArg,CFishka* Arg,CFishka* Oper,CFishka* &Rezult,bool &Trace);


	bool Add(CListFishka* Arg,CFishka* &Rezult);
	bool Sub(CListFishka* Arg,CFishka* &Rezult);
	bool Mul(CListFishka* Arg,CFishka* &Rezult);
	bool Div(CListFishka* Arg,CFishka* &Rezult);
	bool IDiv(CListFishka* Arg,CFishka* &Rezult);
	bool Dup(CListFishka* Arg,CFishka* &Rezult);
	bool DDot(CListFishka* Arg,CFishka* &Rezult);
	bool Less(CListFishka* Arg,CFishka* &Rezult);
	bool Great(CListFishka* Arg,CFishka* &Rezult);
	bool LessEqu(CListFishka* Arg,CFishka* &Rezult);
	bool GreatEqu(CListFishka* Arg,CFishka* &Rezult);
	bool Equal(CListFishka* Arg,CFishka* &Rezult);
	bool NotEqual(CListFishka* Arg,CFishka* &Rezult);
	bool Trans(CListFishka* Arg,CFishka* &Rezult);
	bool Qwestion(CListFishka* Arg,CFishka* &Rezult);
	bool IntSelector(CListFishka* Arg,CFishka* Oper,CFishka* &Rezult);
	bool BoolSelector(CListFishka* Arg,CFishka* Oper,CFishka* &Rezult);
	bool CastScalarType(CFishka* Arg,CFishka* Oper,CFishka* &Rezult);
	bool Type(CFishka* Arg,CFishka* &Rezult);
	bool Value(CFishka* Arg,CFishka* &Rezult);
	bool In(CFishka* Arg,CFishka* &Rezult,bool& Trace);

	bool CastUserType(CFishka* Arg,CFishka* Oper,CFishka* &Rezult,bool &Trace);

protected:
//	void ProcList(CFishka* &Arg);
	bool ErrElemFind(CFishka* Arg);

	int IntV(CFishka* Fishka) {	return (int)((CAtomFishka<int>*)Fishka)->Val; };
	float FloatV(CFishka* Fishka) {	return (float)((CAtomFishka<float>*)Fishka)->Val; };
	bool BoolV(CFishka* Fishka) { return (bool)((CAtomFishka<bool>*)Fishka)->Val; };
	unsigned char CharV(CFishka* Fishka) { return (unsigned char)((CAtomFishka<char>*)Fishka)->Val; };
};

#endif
