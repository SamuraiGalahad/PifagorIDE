#ifndef __ATOM_H_HEADER_INCLUDED__
#define __ATOM_H_HEADER_INCLUDED__

#include "Term.h"
#include "AtomFishka.h"

template<class T> class CAtom : public CTerm
{
public:
	CAtom(T iVal) : CTerm() 
	{
		Val=iVal;
		SelfName="atom";
		ElseTerm=NULL;
	};
	virtual ~CAtom() { };

	bool GetFishka(CFishka* Arg,CFishka* &Rezult,bool &Trace,CInterpretator* Interpretator)
	{
		Rezult=new CAtomFishka<T>(Val);
		return true;
	};

	T Val;
};

#endif
