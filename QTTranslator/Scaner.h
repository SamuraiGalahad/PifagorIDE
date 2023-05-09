// Scaner.h: interface for the CScaner class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SCANER_H_HEADER_INCLUDED__
#define __SCANER_H_HEADER_INCLUDED__

#define KWTSIZE 24
#define STRSIZE 256

#include "Precompiled.h"

enum ELexTypeEnum
{
	NO_LEX=0,
	ENDF,
	OPADD,// +
	OPSUB,// -
	OPMUL,// *
	OPDIV,// /
	OPIDIV,// %
	OPRS,// ()
	OPCS,// []
	OPFS,// {}
	OPDOT,// .
	OPDDOT,// ..
	OPL,// <
	OPG,// >
	OPLEN,// |
	OPTRANS, // #
	OPLE,// <= / =<
	OPGE,// >= / =>
	OPE,// =
	OPNE,// !=
	OPQ,// ?
	OPDUP,// dup

	OPLASS,// <<
	OPRASS,// >>
	ZPT,// ,
	KAV,// '
	ANG,// ^
	DDOT,// :
	DOTZAP,// ;
	RRS,// )
	LRS,// (
	RCS,// [
	LCS,// ]
	RFS,// }
	LFS,// {
	ID,// identifier
	DATA_LIST,
	PAR_LIST,
	DELAY_LIST,
	FUNC_FISHKA,
	TYPEDEF_FISHKA,
	WRAP_FISHKA,
	DELAY_FISHKA,
	INT_CONST,// integer constant
	REAL_CONST,// real constant
	BOOL_CONST,// boolean constant
	SIGNAL_CONST,// signal constant
	CHAR_CONST,// character constant
	ERROR_CONST,// error constant
	SPEC_CONST,// special constant
	STRING_CONST,// string constant
	RDEF_NAME,
	LDEF_NAME,
	KWBLOCK,//KWBREAK,
	KWBOOL,KWDUP,KWCONST,KWELSE,KWERROR,KWFALSE,KWFLOAT,
	KWFUNCDEF,KWINT,KWNIL,KWPREFUNC,//KWRETURN,
	KWSIGNAL,KWSTART,KWTRUE,KWTYPE,KWIN,KWOUT,KWCHAR,
	KWDATALIST,KWPARLIST,KWDELAYLIST,KWTYPEDEF,KWVALUE
};

struct TKWTabElement
{
	char     Str[STRSIZE];
	ELexTypeEnum Lex;
};

typedef TKWTabElement TLexem;

typedef enum 
{
	NO_CHAR=0,IGN,BTFN,LETTER,DIG,ENDL
} TCharType;


class CScaner  
{
public:
	CScaner();
	virtual ~CScaner();
	
	/*virtual*/ 
	int GetChar(FILE *F);
	virtual bool String(TLexem &Lexem);
	virtual bool OpenAllFiles(char *InFileName);
	virtual bool Number(TLexem &Lexem);
	virtual void LexCircle(TLexem &Lexem);
	virtual bool IdentOrKeyWord(TLexem &Lexem);
	virtual void GetNextLexem(TLexem &Lexem);
	virtual bool GetLexem(TLexem &Lexem);
	virtual void GetCurrPos(int &LNum, int &CNum);
	virtual void CloseAllFiles();
	virtual ELexTypeEnum FindKeyWord(char *Word);
	virtual TCharType CharType(char Ch);
protected:
	TLexem m_tlOldLex;
	TLexem m_tlMemLexem;
	TCharType m_tctInCharType;
	FILE* m_fpInFile;
	char m_cInChar;
	int m_iLexCount;
	int m_iLNumber;
	int m_iCNumber;
};

#endif // !defined(AFX_SCANER_H__738A9641_D9E8_11D5_AB5D_C707748D444A__INCLUDED_)
