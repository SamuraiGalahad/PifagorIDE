#include <string.h>

#include "Scaner.h"

TKWTabElement KWTab[KWTSIZE]=
		{{"block",KWBLOCK},
//		{"break",KWBREAK},
		{"bool",KWBOOL},
		{"const",KWCONST},
		{"dup",KWDUP},
		{"else",KWELSE},
		{"error",KWERROR},
		{"false",KWFALSE},
		{"float",KWFLOAT},
		{"funcdef",KWFUNCDEF},
		{"int",KWINT},
		{"nil",KWNIL},
		{"prefunc",KWPREFUNC},
//		{"return",KWRETURN},
		{"signal",KWSIGNAL},
		{"start",KWSTART},
		{"true",KWTRUE},
		{"type",KWTYPE},
		{"in",KWIN},
		{"out",KWOUT},
		{"char",KWCHAR},
		{"datalist",KWDATALIST},
		{"parlist",KWPARLIST},
		{"delaylist",KWDELAYLIST},
		{"typedef",KWTYPEDEF},
		{"value",KWVALUE}
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScaner::CScaner()
{
	m_cInChar='\255';
	m_iLNumber=1;
	m_iCNumber=0;
	m_iLexCount=0;
}

CScaner::~CScaner()
{

}

TCharType CScaner::CharType(char Ch)
{
	if(Ch!='\f'&&Ch!='\t'&&Ch!='\n') m_iCNumber++;
	if(Ch=='\n')
	{
		m_iLNumber++;
		m_iCNumber=0;
	}
//	if(Ch=='.') return DOT;
	if(Ch<' ') return IGN;
	if(isalpha(Ch)||Ch=='_') return LETTER;
	if(isdigit(Ch)) return DIG;
	return NO_CHAR;
}

void CScaner::CloseAllFiles()
{
	fclose(m_fpInFile);
}

ELexTypeEnum CScaner::FindKeyWord(char *Word)
{
	for(int i=0;i<KWTSIZE;i++)
	if(!strcmp(KWTab[i].Str,Word)) return KWTab[i].Lex;
	return ID;
}

void CScaner::GetCurrPos(int &LNum, int &CNum)
{
	LNum=m_iLNumber;
	CNum=m_iCNumber;
}

bool CScaner::GetLexem(TLexem &Lexem)
{
	int iStr=-1;
	Lexem.Str[0]=0;
	Lexem.Lex=NO_LEX;



	if(m_cInChar=='\255')
	{
		m_cInChar=GetChar(m_fpInFile);
		m_tctInCharType=CharType(m_cInChar);
	}
	

	if(m_cInChar==EOF)
	{
		Lexem.Lex=ENDF;
		return true;
	}

	if(m_tctInCharType==LETTER) return IdentOrKeyWord(Lexem);
	if(m_tctInCharType==DIG) return Number(Lexem);
	if(m_cInChar=='"') return String(Lexem);
	if(m_cInChar=='\'') 
	{
		m_cInChar=GetChar(m_fpInFile);
		Lexem.Str[++iStr]=m_cInChar;
		m_cInChar=GetChar(m_fpInFile);
		if(m_cInChar!='\'') return false;
		Lexem.Str[++iStr]=0;
		Lexem.Lex=CHAR_CONST;
		m_cInChar=GetChar(m_fpInFile);
		return true;
	}
	Lexem.Str[++iStr]=m_cInChar;
	Lexem.Str[iStr+1]=0;
	switch(m_cInChar)
	{
	case '+':	{ Lexem.Lex=OPADD; break;}
	case '*':	{ Lexem.Lex=OPMUL; break;}
	case '%':	{ Lexem.Lex=OPIDIV; break;}
	case '|':	{ Lexem.Lex=OPLEN; break;}
	case '#':	{ Lexem.Lex=OPTRANS; break;}
	case ')':	{ Lexem.Lex=RRS; break;}
	case ']':	{ Lexem.Lex=RCS; break;}
	case '}':	{ Lexem.Lex=RFS; break;}
	case '?':	{ Lexem.Lex=OPQ; break;}
	case '\'':	{ Lexem.Lex=KAV; break;}
	case '^':	{ Lexem.Lex=ANG; break;}
	case ':':	{ Lexem.Lex=DDOT; break;}
	case ';':	{ Lexem.Lex=DOTZAP; break;}
	case ',':	{ Lexem.Lex=ZPT; break;}
	}
	char OldChar=m_cInChar;
	m_cInChar=GetChar(m_fpInFile);
	m_tctInCharType=CharType(m_cInChar);
	switch(OldChar)
	{
	case '/': 
		{
			if(m_cInChar=='/')
			{
				do ; while((m_cInChar=GetChar(m_fpInFile))!='\n');
				m_tctInCharType=CharType(m_cInChar);
				return true;
			}
			if(m_cInChar=='*')
			{
				char Tmp=GetChar(m_fpInFile);
				m_tctInCharType=CharType(m_cInChar);
				m_cInChar=GetChar(m_fpInFile);
				m_tctInCharType=CharType(m_cInChar);
				while((Tmp!='*')||(m_cInChar!='/'))
				{
					Tmp=m_cInChar;
					m_cInChar=GetChar(m_fpInFile);
					m_tctInCharType=CharType(m_cInChar);
					if(m_cInChar==EOF) return false;
				}
				m_cInChar=GetChar(m_fpInFile);
				m_tctInCharType=CharType(m_cInChar);
				return GetLexem(Lexem);
			}
			Lexem.Lex=OPDIV;
			return true;
		}
	case '-': 
		{
			if((m_tctInCharType==DIG)||(m_cInChar=='.')) return Number(Lexem);
			Lexem.Lex=OPSUB;
			return true;
		}
	case '.': 
		{
			if(m_cInChar=='.')
			{
				Lexem.Str[++iStr]=m_cInChar;
				Lexem.Lex=OPDDOT;
				break;
			}
			Lexem.Lex=OPDOT;
			Lexem.Str[++iStr]=0;
			return true;
		}
	case '!': 
		{
			if(m_cInChar=='=')
			{
				Lexem.Str[++iStr]=m_cInChar;
				Lexem.Lex=OPNE;
				break;
			}
			return false;
		}
	case '=':
		{
			Lexem.Str[++iStr]=m_cInChar;
			switch(m_cInChar)
			{
			case '<': { Lexem.Lex=OPLE;break;}
			case '>': { Lexem.Lex=OPGE;break;}
			default : { Lexem.Lex=OPE;Lexem.Str[iStr]=0; return true;}
			}
			break;
		}
	case '<': 
		{	
			Lexem.Str[++iStr]=m_cInChar;
			switch(m_cInChar)
			{
			case '<':{ Lexem.Lex=OPLASS; break;}
			case '=':{ Lexem.Lex=OPLE; break;}
			default :{ Lexem.Lex=OPL; Lexem.Str[iStr]=0; return true;}
			}
			break;
		}
	case '>': 
		{
			Lexem.Str[++iStr]=m_cInChar;
			switch(m_cInChar)
			{
			case '>':{ Lexem.Lex=OPRASS; break;}
			case '=':{ Lexem.Lex=OPGE; break;}
			default :{ Lexem.Lex=OPG; Lexem.Str[iStr]=0; return true;}
			}
			break;
		}
	case '(':
		{
			Lexem.Str[++iStr]=m_cInChar;
			if(m_cInChar==')')	{Lexem.Lex=OPRS; break; }
			else { Lexem.Lex=LRS; Lexem.Str[iStr]=0; return true;}
		}
	case '[': 
		{
			Lexem.Str[++iStr]=m_cInChar;
			if(m_cInChar==']') { Lexem.Lex=OPCS; break; }
			else { Lexem.Lex=LCS; Lexem.Str[iStr]=0; return true;}
		}
	case '{':
		{
			Lexem.Str[++iStr]=m_cInChar;
			if(m_cInChar=='}') { Lexem.Lex=OPFS; break; }
			else { Lexem.Lex=LFS; Lexem.Str[iStr]=0; return true;}
		}
	default : OldChar=0;
	}
	if(OldChar)
	{
		m_cInChar=GetChar(m_fpInFile);
		m_tctInCharType=CharType(m_cInChar);
	}
	Lexem.Str[++iStr]=0;
	return true;
}

void CScaner::GetNextLexem(TLexem &Lexem)
{
    std::ifstream i;
    std::ofstream o;
	int k=0;
	if(m_iLexCount>0)
	{
		Lexem=m_tlMemLexem;
		m_iLexCount--;
//		return;
	}
	else LexCircle(Lexem);
	if(Lexem.Lex==ID)
	{
		m_tlOldLex=Lexem;
		LexCircle(Lexem);
		if(Lexem.Lex==OPLASS)
		{
			Lexem=m_tlOldLex;
			Lexem.Lex=LDEF_NAME;
		}
		else
		{
			m_iLexCount++;
			m_tlMemLexem=Lexem;
			Lexem=m_tlOldLex;
		}
	}

	if(Lexem.Lex==OPRASS)
	{
		m_tlOldLex=Lexem;
		LexCircle(Lexem);
		if(Lexem.Lex==ID)
		{
			Lexem.Lex=RDEF_NAME;
		}
		else
		{
			m_iLexCount++;
			m_tlMemLexem=Lexem;
			Lexem=m_tlOldLex;
		}
	}

//	fprintf(OutFile,"%s",Lexem.Str);
//	if(!strcmp(Lexem.Str,";")) fprintf(OutFile,"\n");
	return;
}

bool CScaner::IdentOrKeyWord(TLexem &Lexem)
{
	int iStr=-1;
	Lexem.Str[++iStr]=m_cInChar;
	m_cInChar=GetChar(m_fpInFile);
	m_tctInCharType=CharType(m_cInChar);
	while((m_tctInCharType==LETTER)||
		(m_tctInCharType==DIG)||
		((m_cInChar=='.')&&(Lexem.Str[iStr]!='.')))
	{
		Lexem.Str[++iStr]=m_cInChar;
		m_cInChar=GetChar(m_fpInFile);
		m_tctInCharType=CharType(m_cInChar);
	}
	Lexem.Str[++iStr]=0;
	Lexem.Lex=FindKeyWord(Lexem.Str);
	if(Lexem.Lex==KWDUP) Lexem.Lex=OPDUP;
	if((Lexem.Lex==KWTRUE)||(Lexem.Lex==KWFALSE)) Lexem.Lex=BOOL_CONST;
	return true;
}

void CScaner::LexCircle(TLexem &Lexem)
{
	do
	{
		if(!GetLexem(Lexem))
		{
			int C,L;
			GetCurrPos(L,C);
			printf("Scaner error: line %d, row %d\n",L,C);
			return;
		}
	}while(Lexem.Lex==NO_LEX);
}

bool CScaner::Number(TLexem &Lexem)
{
	int EFinded=0;
	int PFinded=0;
	int iStr=strlen(Lexem.Str)-1;
	Lexem.Str[++iStr]=m_cInChar;
	m_cInChar=GetChar(m_fpInFile);
	m_tctInCharType=CharType(m_cInChar);
	while(m_tctInCharType!=IGN)
	{
		switch(m_tctInCharType)
		{
		case DIG    : {
			Lexem.Str[++iStr]=m_cInChar;
			m_cInChar=GetChar(m_fpInFile);
			m_tctInCharType=CharType(m_cInChar);
			break;
					  }
		case LETTER : {
			if((m_cInChar=='e')||(m_cInChar=='E'))
			{
				if(EFinded) return false;
				if(isdigit(Lexem.Str[iStr]))
				{
					EFinded=1;
					Lexem.Str[++iStr]=m_cInChar;
					m_cInChar=GetChar(m_fpInFile);
					m_tctInCharType=CharType(m_cInChar);
					break;
				} else return false;
			} else return false;
					  }
		case NO_CHAR: {
			switch(m_cInChar)
			{
			case '.': {
				if(EFinded) return false;
				if(PFinded) return false;
				PFinded=1;
				break;
					  }
			case '-':
			case '+': {
				if((Lexem.Str[iStr]=='E')||
					(Lexem.Str[iStr]=='e')) break;
				else return false;
					  }
			default : {
				Lexem.Str[++iStr]=0;
				if(EFinded||PFinded) Lexem.Lex=REAL_CONST;
				else Lexem.Lex=INT_CONST;
				return true;
					  }
			}
			Lexem.Str[++iStr]=m_cInChar;
			m_cInChar=GetChar(m_fpInFile);
			m_tctInCharType=CharType(m_cInChar);
			break;
					  }
		default: break;
		}
	}
	Lexem.Str[++iStr]=0;
	if(EFinded||PFinded) Lexem.Lex=REAL_CONST;
	else Lexem.Lex=INT_CONST;
	m_cInChar=GetChar(m_fpInFile);
	m_tctInCharType=CharType(m_cInChar);
	return true;
}

bool CScaner::OpenAllFiles(char *InFileName)
{
	m_cInChar='\255';
	m_iLNumber=1;
	m_iCNumber=0;
	m_iLexCount=0;

	//char *OutFileName,*Tmp;
	//char *Extension=".lex";
	if (!(m_fpInFile=fopen(InFileName,"r"))) return false;
	return true;
}

bool CScaner::String(TLexem &Lexem)
{
	int iStr=-1;
	int OldC=m_iCNumber;
	int OldL=m_iLNumber;
	m_cInChar=GetChar(m_fpInFile);
	m_tctInCharType=CharType(m_cInChar);
	Lexem.Str[++iStr]=m_cInChar;
	char Tmp;
	do
	{
		if((m_cInChar=='\n')||(m_cInChar==EOF))
		{
			m_iCNumber=OldC;
			m_iLNumber=OldL;
			return false;
		}
		if(m_cInChar=='\\')
		{
			m_cInChar=GetChar(m_fpInFile);
			m_tctInCharType=CharType(m_cInChar);
			switch(m_cInChar)
			{
			case '"': {Lexem.Str[iStr]='"';break;}
			case 'n': {Lexem.Str[iStr]='\n';break;}
			case 't': {Lexem.Str[iStr]='\t';break;}
			case 'v': {Lexem.Str[iStr]='\v';break;}
			case 'b': {Lexem.Str[iStr]='\b';break;}
			case 'r': {Lexem.Str[iStr]='\r';break;}
			case '\\': {Lexem.Str[iStr]='\\';break;}
			case '0': {Lexem.Str[iStr]='\0';break;}
			}
		}
		Tmp=m_cInChar;
		m_cInChar=GetChar(m_fpInFile);
		m_tctInCharType=CharType(m_cInChar);
		Lexem.Str[++iStr]=m_cInChar;
	}
	while(m_cInChar!='"');
	Lexem.Str[iStr]=0;
	Lexem.Lex=STRING_CONST;
	m_cInChar=GetChar(m_fpInFile);
	m_tctInCharType=CharType(m_cInChar);
	return true;
}

int CScaner::GetChar(FILE *F)
{
	return getc(F);
}

