#ifndef __KEYWORD_H_HEADER_INCLUDED__
#define __KEYWORD_H_HEADER_INCLUDED__

#include "Term.h"
#include "Scaner.h"

enum ELexTypeEnum;

struct TKWTabElement;

typedef TKWTabElement TLexem;

class CKeyWord : public CTerm
{
public:
	CKeyWord(TLexem Lexem);
	~CKeyWord();

	virtual bool GetFishka(CFishka* Arg,CFishka* &Rezult,bool &Trace,CInterpretator* Interpretator);
private:
	ELexTypeEnum Val;
};

#endif
