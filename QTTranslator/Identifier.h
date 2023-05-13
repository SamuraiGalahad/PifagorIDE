#ifndef __IDENTIFIER_H_HEADER_INCLUDED__
#define __IDENTIFIER_H_HEADER_INCLUDED__

#include "Term.h"

struct TKWTabElement;

typedef TKWTabElement TLexem;

class CGeneralObject;

class CIdentifier : public CTerm
{
public:
	CIdentifier(CGeneralObject* iOwner,TLexem Lexem);/* : CTerm()
	{
		Owner=iOwner;
		Str=string(Lexem.Str);
		SelfName="id";
		ElseTerm=NULL;
	};*/
	virtual ~CIdentifier() { };

	virtual bool GetFishka(CFishka* Arg,CFishka* &Rezult,bool &Trace,CInterpretator* Interpretator);
	CGeneralObject* Object;
private:
    std::string Str;
};

#endif
