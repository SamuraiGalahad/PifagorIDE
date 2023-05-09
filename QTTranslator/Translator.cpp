#include <string.h>

#include "Precompiled.h"

#include "Translator.h"
#include "Exeption.h"
#include "Program.h"
#include "Function.h"
#include "Expression.h"
#include "List.h"
#include "Atom.h"
#include "Identifier.h"
#include "KeyWord.h"


vector <string> GlobalNameList;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTranslator::CTranslator(CScaner *Scaner) 
{ 
	m_pScaner=Scaner;
	m_bBlockContext=false;
};

CTranslator::~CTranslator() 
{ 
	EraseGlobalObjectTable();
};

extern "C" void CTranslator::EraseGlobalObjectTable()
{
	if(GlobalObjectTable.empty()) return;
	vector<CGeneralObject*>::iterator i;

	i=GlobalObjectTable.end();
	i--;
	for(;i!=GlobalObjectTable.begin();i--)
		delete (*i);
	delete (*i);

	GlobalObjectTable.clear();
}; 

bool CTranslator::Const_Element(CGeneralObject* NameOwner,CGeneralObject* Owner)
{
	CExpression* Expr;
	if(Const_Left_Named_El(NameOwner,Owner,Expr))
	{
		do
		{
			if(m_tlLexem.Lex!=RDEF_NAME) return true;
			((CModule*)NameOwner)->InsertConstant(make_pair(string(m_tlLexem.Str),Expr));
			m_pScaner->GetNextLexem(m_tlLexem);
		}while(1);
	}
	if(Const_Expr(NameOwner,Expr))
	{
		if(m_tlLexem.Lex!=RDEF_NAME)
			{
				Owner->AddElem(Expr);
				return true;
			}
		CIdentifier* Tmp = new CIdentifier(NameOwner,m_tlLexem);
		GlobalObjectTable.push_back(Tmp);

		Owner->AddElem((CExpression*)Tmp);
		do
		{
			if(m_tlLexem.Lex!=RDEF_NAME) return true;
			((CModule*)NameOwner)->InsertConstant(make_pair(string(m_tlLexem.Str),Expr));
			m_pScaner->GetNextLexem(m_tlLexem);
		}while(1);
	}
	return false;
}

bool CTranslator::Const_Left_Named_El(CGeneralObject* NameOwner,CGeneralObject* Owner,CExpression* &Expr)
{
	if(m_tlLexem.Lex!=LDEF_NAME) return false;
	string LeftName=string(m_tlLexem.Str);
	TLexem OldLexem=m_tlLexem;
	m_pScaner->GetNextLexem(m_tlLexem);
	if(Const_Expr(NameOwner,Expr))
	{
		((CModule*)NameOwner)->InsertConstant(make_pair(LeftName,Expr));

		CIdentifier* Tmp = new CIdentifier(NameOwner,OldLexem);
		GlobalObjectTable.push_back(Tmp);

		Owner->AddElem((CExpression*)Tmp);
		return true;
	}
	if(Const_Left_Named_El(NameOwner,Owner,Expr))
	{
		((CModule*)NameOwner)->InsertConstant(make_pair(LeftName,Expr));
		return true;
	}
	return false;
}

bool CTranslator::Const_Expr(CGeneralObject* NameOwner, CExpression* &Expr)
{
	Expr = new CExpression(NameOwner);
	GlobalObjectTable.push_back(Expr);

	CTerm* Trm;
	if(!Const_Term(NameOwner,Trm))
	{
		throw CExeption(NULL,"",16);
		return false;
	}

	Expr->AddTerm(Trm);
_1:	if(Const_Pref_Expr(NameOwner,Expr)) goto _2;
	if(m_tlLexem.Lex==DDOT)
	{
		m_pScaner->GetNextLexem(m_tlLexem);
		if(!Const_Term(NameOwner,Trm)) 	
		{
			throw CExeption(NULL,"",16);
			return false;
		}
		else 
		{
			Expr->AddTerm(Trm);
			goto _2;
		}
	}
	return true;

_2:	if(m_tlLexem.Lex==KWELSE)
	{
		if(!Const_Term(NameOwner,Trm)) return false;
		else 
		{
			Expr->AddTerm(Trm);
			goto _1;
		}
	}
	goto _1;
	return false;
}

bool CTranslator::Const_Term(CGeneralObject* NameOwner,CTerm* &Trm)
{
	if(Const_Atom(NameOwner,Trm)) goto _1;
	if(Const_List(NameOwner,Trm)) goto _1;
	if(m_tlLexem.Lex==ID) 
	{
		Trm=(CTerm*)new CIdentifier(NameOwner,m_tlLexem);
		GlobalObjectTable.push_back(Trm);
		m_pScaner->GetNextLexem(m_tlLexem);
		goto _1;
	}
	return false;
_1: return true;
}

bool CTranslator::Const_Pref_Expr(CGeneralObject* NameOwner,CExpression* &Expr)
{
	CTerm* Trm;
	if(m_tlLexem.Lex!=ANG) return false;
	m_pScaner->GetNextLexem(m_tlLexem);
	if(!Const_Term(NameOwner,Trm)) 
	{
		throw CExeption(NULL,"",16);
		return false;
	}
	Const_Pref_Expr(NameOwner,Expr);
	Expr->AddTermBeforeEnd(Trm);
	return true;
}

bool CTranslator::Const_Atom(CGeneralObject* NameOwner,CTerm* &Trm)
{
	if((m_tlLexem.Lex>ENDF)&&(m_tlLexem.Lex<OPLASS))
	{
		Trm = new CAtom<ELexTypeEnum>(m_tlLexem.Lex);
		GlobalObjectTable.push_back(Trm);

		goto _1;
	};
	switch(m_tlLexem.Lex)
	{
	case INT_CONST : 
		{
			Trm = new CAtom<int>(atoi(m_tlLexem.Str));
			GlobalObjectTable.push_back(Trm);

			goto _1;
		};
	case REAL_CONST :
		{
			Trm = new CAtom<float>(atof(m_tlLexem.Str));
			GlobalObjectTable.push_back(Trm);

			goto _1;
		};
	case BOOL_CONST : 		
		{
			string Str=m_tlLexem.Str;

			if(Str=="true") Trm = new CAtom<bool>(true);
			else Trm = new CAtom<bool>(false);
			GlobalObjectTable.push_back(Trm);

			goto _1;
		};
	case SIGNAL_CONST : goto _1;
	case CHAR_CONST : 
		{
			Trm = new CAtom<char>(m_tlLexem.Str[0]);
			GlobalObjectTable.push_back(Trm);

			goto _1;
		};
	case STRING_CONST:
		{
			Trm=new CList(NULL,DATA_LIST);
			GlobalObjectTable.push_back(Trm);

			for(unsigned int i=0;i<strlen(m_tlLexem.Str);i++)
			{
				CAtom<char>* TmpCh=new CAtom<char>(m_tlLexem.Str[i]);
				GlobalObjectTable.push_back(TmpCh);

				Trm->AddElem((CExpression*)TmpCh);
			}
			goto _1;
		};
	case ERROR_CONST : goto _1;
	case SPEC_CONST : goto _1;
	default : return false;
	}
_1:	m_pScaner->GetNextLexem(m_tlLexem);
	return true;
}

bool CTranslator::Const_List(CGeneralObject* NameOwner,CTerm* &Trm)
{
	if(Const_Data_List(NameOwner,Trm)) return true;
	if(Const_Par_List(NameOwner,Trm)) return true;
	if(m_tlLexem.Lex!=STRING_CONST) return false;
	return false;
}

bool CTranslator::Const_Data_List(CGeneralObject* NameOwner,CTerm* &Trm)
{
	if(m_tlLexem.Lex!=LRS) return false;

	Trm = new CList(NameOwner,DATA_LIST);
	GlobalObjectTable.push_back(Trm);

	do
	{
		m_pScaner->GetNextLexem(m_tlLexem);
		if(!Const_Element(NameOwner,Trm)) return false;
		if(m_tlLexem.Lex==RRS) goto _1;
		if(m_tlLexem.Lex!=ZPT) 
		{
			throw CExeption(NULL,"",13);
			return false;
		}
	}while(1);
	return false;
_1:	m_pScaner->GetNextLexem(m_tlLexem);
	return true;
}

bool CTranslator::Const_Par_List(CGeneralObject* NameOwner,CTerm* &Trm)
{
	if(m_tlLexem.Lex!=LCS) return false;
	Trm = new CList(NameOwner,PAR_LIST);
	GlobalObjectTable.push_back(Trm);

	do
	{
		m_pScaner->GetNextLexem(m_tlLexem);
		if(!Const_Element(NameOwner,Trm)) return false;
		if(m_tlLexem.Lex==RCS) goto _1;
		if(m_tlLexem.Lex!=ZPT)
		{
			throw CExeption(NULL,"",13);
			return false;
		}
	}while(1);
	return false;
_1:	m_pScaner->GetNextLexem(m_tlLexem);
	return true;
}


bool CTranslator::Module(CModule* &iModule)
{
	m_bBlockContext=false;
	/*while(1) 
	{
		m_Scaner.GetNextLexem(m_tlLexem);
	}*/

//	if(!iProgram)
	iModule=new CModule();
	GlobalObjectTable.push_back(iModule);

	m_pScaner->GetNextLexem(m_tlLexem);

	GlobalNameList.clear();

	if(!Named_Def(iModule)) 
	{
		throw CExeption(NULL,"",1);
		return false;
	}

	do
	{
		while(m_tlLexem.Lex==DOTZAP) m_pScaner->GetNextLexem(m_tlLexem);
		if(m_tlLexem.Lex==ENDF) goto _1;
/*		if(m_tlLexem.Lex!=DOTZAP) 
		{
			throw CExeption(NULL,"",2);
			return false;
		}*/
		//if(m_tlLexem.Lex==DOTZAP)

//		if(m_tlLexem.Lex==KWSTART) goto _1;
		GlobalNameList.clear();

		if(!Named_Def(iModule)) 
		{
			throw CExeption(NULL,"",1);
			return false;
		}
	}while(1);
_1:	TNameTable::iterator i;
	int UnRes=0;

	for(i=iModule->NameTable.begin();i!=iModule->NameTable.end();i++)
		if(!(*i).second)
		{
			throw CExeption(NULL,(*i).first,3);
//			InOutBuffer<<"\n"<<"Unresolved function \""<<(*i).first<< "\" !!!"<<"\n";
			UnRes++;
		}
	if(UnRes) 
	{
		strcat(data,"\n");
		return false;
	}
	return true;
}

bool CTranslator::Named_Def(CModule* Owner)
{
	CFunction* TmpFunc=NULL;
	CFunction* TmpTypeDef=NULL;
	CExpression* TmpExpr=NULL;
	/// нопедекемхе тсмйжхх
	if(Definition(Owner,TmpFunc,m_tlLexem.Str))
	{
		if(m_tlLexem.Lex!=RDEF_NAME)
		{
			throw CExeption(NULL,"",11);
			return false;
		};
		if(TmpFunc) TmpFunc->SelfName=string(m_tlLexem.Str);
		CheckID(TmpFunc->SelfName,Owner,true);
		Owner->InsertNObject(make_pair(string(m_tlLexem.Str),TmpFunc));
		m_pScaner->GetNextLexem(m_tlLexem);
		do
		{
			if(m_tlLexem.Lex!=RDEF_NAME) return true;
			CheckID(m_tlLexem.Str,Owner,true);
			Owner->InsertNObject(make_pair(string(m_tlLexem.Str),TmpFunc));
			m_pScaner->GetNextLexem(m_tlLexem);
		}while(1);
	}
	/// нопедекемхе оепецпсфеммни тсмйжхх
	if(DefinitionOverFunc(Owner,TmpFunc,m_tlLexem.Str))
	{
		if(m_tlLexem.Lex!=RDEF_NAME)
		{
			throw CExeption(NULL,"",11);
			return false;
		};
		if(TmpFunc) TmpFunc->SelfName=string(m_tlLexem.Str);
		CheckID(m_tlLexem.Str,Owner,true);
		Owner->InsertNObject(make_pair(string(m_tlLexem.Str),TmpFunc));
		m_pScaner->GetNextLexem(m_tlLexem);
		do
		{
			if(m_tlLexem.Lex!=RDEF_NAME) return true;
			CheckID(m_tlLexem.Str,Owner,true);
			Owner->InsertNObject(make_pair(string(m_tlLexem.Str),TmpFunc));
			m_pScaner->GetNextLexem(m_tlLexem);
		}while(1);
	}
	/// нопедекемхе онкэгнбюрекэяйнцн рхою
	if(UserTypeDefinition(Owner,TmpFunc,m_tlLexem.Str))
	{
		if(m_tlLexem.Lex!=RDEF_NAME)
		{
			throw CExeption(NULL,"",11);
			return false;
		};
		if(TmpFunc) TmpFunc->SelfName=string(m_tlLexem.Str);
		CheckID(m_tlLexem.Str,Owner,true);
		Owner->InsertUserType(make_pair(string(m_tlLexem.Str),TmpFunc));
		m_pScaner->GetNextLexem(m_tlLexem);
		do
		{
			if(m_tlLexem.Lex!=RDEF_NAME) return true;
			CheckID(m_tlLexem.Str,Owner,true);
			Owner->InsertUserType(make_pair(string(m_tlLexem.Str),TmpFunc));
			m_pScaner->GetNextLexem(m_tlLexem);
		}while(1);
	}
	// нопедекемхе йнмярюмрш
	if(Const_Definition(Owner,TmpExpr))
	{
		if(m_tlLexem.Lex!=RDEF_NAME) 
		{
			throw CExeption(NULL,"",11);
			return false;
		}
		TmpExpr->SelfName=string(m_tlLexem.Str);
		CheckID(m_tlLexem.Str,Owner,true);
		Owner->InsertConstant(make_pair(string(m_tlLexem.Str),TmpExpr));
		m_pScaner->GetNextLexem(m_tlLexem);
		do
		{
			if(m_tlLexem.Lex!=RDEF_NAME) return true;
			CheckID(m_tlLexem.Str,Owner,true);
			Owner->InsertConstant(make_pair(string(m_tlLexem.Str),TmpExpr));
			m_pScaner->GetNextLexem(m_tlLexem);
		}while(1);
	}
	float Rang;
	if(Left_Named_Def(Owner,TmpExpr,TmpFunc,TmpTypeDef,Rang))
	{
		do
		{
			if(m_tlLexem.Lex!=RDEF_NAME) return true;
			if(TmpExpr)
			{
				CheckID(m_tlLexem.Str,Owner,true);
				Owner->InsertConstant(make_pair(string(m_tlLexem.Str),TmpExpr));
			}
			if(TmpFunc)
			{
				CheckID(m_tlLexem.Str,Owner,true);
				Owner->InsertNObject(make_pair(string(m_tlLexem.Str),TmpFunc));
			}
			if(TmpTypeDef)
			{
				CheckID(m_tlLexem.Str,Owner,true);
				Owner->InsertUserType(make_pair(string(m_tlLexem.Str),TmpTypeDef));
			}

			m_pScaner->GetNextLexem(m_tlLexem);
		}while(1);
	}
	return false;
}

bool CTranslator::Definition(CModule* Owner,CFunction* &Func, string Name)
{
	if(m_tlLexem.Lex==KWPREFUNC) 
	{
		Func=NULL;
		m_pScaner->GetNextLexem(m_tlLexem);
		Owner->InsertNObject(make_pair(Name,Func));
		return true;
	}
	return Function(Owner,Func,Name);
}

bool CTranslator::DefinitionOverFunc(CModule* Owner,CFunction* &Func, string Name)
{
	return Function(Owner,Func,Name);
}


bool CTranslator::UserTypeDefinition(CModule* Owner,CFunction* &TypeDef, string Name)
{
	return UserType(Owner,TypeDef,Name);
}

bool CTranslator::Const_Definition(CModule* Owner,CExpression* &Expr)
{
	if(m_tlLexem.Lex==KWCONST) 
	{
		m_pScaner->GetNextLexem(m_tlLexem);
		if(!Const_Expr(Owner,Expr)) 
		{	
			throw CExeption(NULL,"",12);
			return false;
		}
		return true;
	}
	return false;
}

bool CTranslator::Left_Named_Def(CModule* Owner,CExpression* &Expr,CFunction* &Func,CFunction* &TypeDef,float &Rang)
{
	string LeftName;
	Rang=0;
	bool Overloaded=false;

	LeftName=string(m_tlLexem.Str);

	if(m_tlLexem.Lex==ID)
	{
		m_pScaner->GetNextLexem(m_tlLexem);
		if((m_tlLexem.Lex!=LCS)&&(m_tlLexem.Lex!=OPCS)) return false;
		if(m_tlLexem.Lex!=OPCS)
		{
			m_pScaner->GetNextLexem(m_tlLexem);
			if((m_tlLexem.Lex!=INT_CONST)&&(m_tlLexem.Lex!=REAL_CONST)) return false;
			if(m_tlLexem.Lex==INT_CONST) Rang=atoi(m_tlLexem.Str);
			if(m_tlLexem.Lex==REAL_CONST) Rang=atof(m_tlLexem.Str);
			m_pScaner->GetNextLexem(m_tlLexem);
			if(m_tlLexem.Lex!=RCS) return false;
		}
		else Rang=0;
		m_pScaner->GetNextLexem(m_tlLexem);
		if(m_tlLexem.Lex!=OPLASS) return false;
		Overloaded=true;
	}
	else
	{
		if(m_tlLexem.Lex!=LDEF_NAME) return false;

		//CGeneralObject* Object=NULL;
		//Owner->InsertNObject(make_pair(string(m_tlLexem.Str),Object));
		GlobalNameList.push_back(string(m_tlLexem.Str));
	}

	CheckID(LeftName,Owner,!Overloaded);

	m_pScaner->GetNextLexem(m_tlLexem);
	if((!Overloaded)&&(Definition(Owner,Func,LeftName)))
	{
		if(Func) Func->SelfName=LeftName;
		Owner->InsertNObject(make_pair(LeftName,Func));
		return true;
	};
	if((Overloaded)&&(DefinitionOverFunc(Owner,Func,LeftName)))
	{
		if(Func) Func->SelfName=LeftName;
		Owner->InsertOverFunc(make_pair(LeftName,Func),Rang);
		return true;
	};
	if(UserTypeDefinition(Owner,TypeDef,LeftName))
	{
		if(TypeDef) TypeDef->SelfName=LeftName;
		Owner->InsertUserType(make_pair(LeftName,TypeDef));
		return true;
	};
	if(Const_Definition(Owner,Expr))
	{
		if(Owner->NameTable.find(LeftName)!=Owner->NameTable.end())
		{
			throw CExeption(NULL,LeftName,7);
			return false;
		}
		Expr->SelfName=LeftName;
		Owner->InsertConstant(make_pair(LeftName,Expr));
		return true;
	};
	if(Left_Named_Def(Owner,Expr,Func,TypeDef,Rang))
	{
		if(Expr) Owner->InsertConstant(make_pair(LeftName,Expr));
		if(Func)
			if(Overloaded) Owner->InsertOverFunc(make_pair(LeftName,Func),Rang);
				else Owner->InsertNObject(make_pair(LeftName,Func));
		if(TypeDef) Owner->InsertUserType(make_pair(LeftName,TypeDef));
		return true;
	};
	return false;
}

bool CTranslator::Function(CModule* Owner,CFunction* &Func,string Name)
{
	string ParamID;
	if(m_tlLexem.Lex!=KWFUNCDEF) return false;
	int Line,Pos;
	m_pScaner->GetCurrPos(Line,Pos);
	m_pScaner->GetNextLexem(m_tlLexem);
	//if(m_tlLexem.Lex!=ID) return false;

	if(m_tlLexem.Lex==ID)
	{
		ParamID=string(m_tlLexem.Str);
		m_pScaner->GetNextLexem(m_tlLexem);
	}
	else ParamID="";

	Func=new CFunction(Owner);
	GlobalObjectTable.push_back(Func);

	Func->Line=Line;
	Func->ParamID=ParamID;
	
	if(m_tlLexem.Lex!=LFS) 
	{
		throw CExeption(NULL,"",9);
		return false;
	}

	//Owner->InsertNObject(make_pair(Name,Func));

	m_pScaner->GetNextLexem(m_tlLexem);
	if(m_tlLexem.Lex==RFS) goto _1;

	do
	{
		if(!Element(Func,Func)) return false;
		if(m_tlLexem.Lex==RFS) goto _1;
		if(m_tlLexem.Lex!=DOTZAP) 
		{
			throw CExeption(NULL,"",2);
			return false;
		}
		m_pScaner->GetNextLexem(m_tlLexem);
		if(m_tlLexem.Lex==RFS) goto _1;
	}while(1);

	return false;
_1:	
/*	if(!(Func->NoNamedElem)) 
	{
		throw CExeption(NULL,Name,10);
//		InOutBuffer<<"No named expression missing in function \""<< Name <<"\"!"<<"\n";
		return false;
	}*/
	Func->ParamID=ParamID;
	m_pScaner->GetCurrPos(Line,Pos);
	Func->EndLine=Line;
	m_pScaner->GetNextLexem(m_tlLexem);
	return true;
}

bool CTranslator::UserType(CModule* Owner,CFunction* &Func,string Name)
{
	string ParamID;
	if(m_tlLexem.Lex!=KWTYPEDEF) return false;
	int Line,Pos;
	m_pScaner->GetCurrPos(Line,Pos);
	m_pScaner->GetNextLexem(m_tlLexem);
	//if(m_tlLexem.Lex!=ID) return false;

	if(m_tlLexem.Lex==ID)
	{
		ParamID=string(m_tlLexem.Str);
		m_pScaner->GetNextLexem(m_tlLexem);
	}
	else ParamID="";

	Func=new CFunction(Owner);
	GlobalObjectTable.push_back(Func);

	Func->Line=Line;
	Func->ParamID=ParamID;
	
	if(m_tlLexem.Lex!=LFS) 
	{
		throw CExeption(NULL,"",9);
		return false;
	}

	//Owner->InsertNObject(make_pair(Name,Func));

	m_pScaner->GetNextLexem(m_tlLexem);
	if(m_tlLexem.Lex==RFS) goto _1;

	do
	{
		if(!Element(Func,Func)) return false;
		if(m_tlLexem.Lex==RFS) goto _1;
		if(m_tlLexem.Lex!=DOTZAP) 
		{
			throw CExeption(NULL,"",2);
			return false;
		}
		m_pScaner->GetNextLexem(m_tlLexem);
		if(m_tlLexem.Lex==RFS) goto _1;
	}while(1);

	return false;
_1:	
/*	if(!(Func->NoNamedElem)) 
	{
		throw CExeption(NULL,Name,10);
//		InOutBuffer<<"No named expression missing in function \""<< Name <<"\"!"<<"\n";
		return false;
	}*/
	Func->ParamID=ParamID;
	m_pScaner->GetCurrPos(Line,Pos);
	Func->EndLine=Line;
	m_pScaner->GetNextLexem(m_tlLexem);
	return true;
}

bool CTranslator::Element(CGeneralObject* NameOwner,CGeneralObject* Owner)
{
	CExpression* Expr;
	if(Left_Named_El(NameOwner,Owner,Expr))
	{
		do
		{
			if(m_tlLexem.Lex!=RDEF_NAME) return true;
			if(((CBlock*)NameOwner)->NameTable.find(m_tlLexem.Str)!=((CBlock*)NameOwner)->NameTable.end())
			{
				throw CExeption(NULL,string(m_tlLexem.Str),8);
//				InOutBuffer<<"\n"<<"Identifier \""<< m_tlLexem.Str <<"\" is already defined!!!"<<"\n";
				return false;
			}
			Expr->SelfName=string(m_tlLexem.Str);
			NameOwner->InsertNObject(make_pair(string(m_tlLexem.Str),Expr));
			m_pScaner->GetNextLexem(m_tlLexem);
		}while(1);
	}
	if(Expression(NameOwner,Expr))
	{
		if(m_tlLexem.Lex!=RDEF_NAME) 
		{
			Owner->AddElem(Expr);
			return true;
		}
		if(((CBlock*)NameOwner)->NameTable.find(m_tlLexem.Str)!=((CBlock*)NameOwner)->NameTable.end())
		{
			throw CExeption(NULL,string(m_tlLexem.Str),8);
//			InOutBuffer<<"\n"<<"Identifier \""<< m_tlLexem.Str <<"\" is already defined!!!"<<"\n";
			return false;
		}
		if(Owner->IsList()) 
		{
			CIdentifier* Tmp = new CIdentifier(NameOwner,m_tlLexem);
			GlobalObjectTable.push_back(Tmp);

			Owner->AddElem((CExpression*)Tmp);
		}
		do
		{
			if(m_tlLexem.Lex!=RDEF_NAME) return true;
			if(((CBlock*)NameOwner)->NameTable.find(m_tlLexem.Str)!=((CBlock*)NameOwner)->NameTable.end())
			{
				throw CExeption(NULL,string(m_tlLexem.Str),8);
//				InOutBuffer<<"\n"<<"Identifier \""<< m_tlLexem.Str <<"\" is already defined!!!"<<"\n";
				return false;
			}
			NameOwner->InsertNObject(make_pair(string(m_tlLexem.Str),Expr));
			m_pScaner->GetNextLexem(m_tlLexem);
		}while(1);
	}
 return false;
}

bool CTranslator::Left_Named_El(CGeneralObject* NameOwner,CGeneralObject* Owner,CExpression* &Expr)
{
	if(m_tlLexem.Lex!=LDEF_NAME) return false;
	string LeftName=string(m_tlLexem.Str);	
	if(((CBlock*)NameOwner)->NameTable.find(m_tlLexem.Str)!=((CBlock*)NameOwner)->NameTable.end())
	{
//		InOutBuffer<<"\n"<<"Identifier \""<< m_tlLexem.Str <<"\" is already defined!!!"<<"\n";
		throw CExeption(NULL,string(m_tlLexem.Str),8);
		return false;
	}
	TLexem OldLexem=m_tlLexem;
	m_pScaner->GetNextLexem(m_tlLexem);
	if(Expression(NameOwner,Expr))
	{
		Expr->SelfName=LeftName;
		NameOwner->InsertNObject(make_pair(LeftName,Expr));
		if(Owner->IsList()) 
		{
			CIdentifier* Tmp = new CIdentifier(NameOwner,OldLexem);
			GlobalObjectTable.push_back(Tmp);

			Owner->AddElem((CExpression*)Tmp);
		}
		return true;
	}
	if(Left_Named_El(NameOwner,Owner,Expr))
	{
		NameOwner->InsertNObject(make_pair(LeftName,Expr));
		return true;
	}
	return false;
}

bool CTranslator::Expression(CGeneralObject* NameOwner,CExpression* &Expr)
{
	Expr = new CExpression(NameOwner);
	GlobalObjectTable.push_back(Expr);

	CTerm* Trm;
	if(!Term(NameOwner,Trm))
	{
		throw CExeption(NULL,"",14);
		return false;
	}
	Expr->AddTerm(Trm);
_1:	if(Pref_Expr(NameOwner,Expr,Trm)) goto _2;
	if(m_tlLexem.Lex==DDOT)
	{
		m_pScaner->GetNextLexem(m_tlLexem);
		if(!Term(NameOwner,Trm)) 
		{
			throw CExeption(NULL,"",14);
			return false;
		}
		else 
		{
			Expr->AddTerm(Trm);
			goto _2;
		}
	}
	return true;

_2:	if(m_tlLexem.Lex==KWELSE)
	{
		CTerm* lTrm;
		m_pScaner->GetNextLexem(m_tlLexem);
		if(!Term(NameOwner,lTrm)) 
		{
			throw CExeption(NULL,"",14);
			return false;
		}
		else 
		{
			Trm->ElseTerm=lTrm;
			goto _1;
		}
	}
	goto _1;
	return false;
}

bool CTranslator::Pref_Expr(CGeneralObject* NameOwner,CExpression* &Expr,CTerm* &Trm)
{
	CTerm* lTrm;
	if(m_tlLexem.Lex!=ANG) return false;
	m_pScaner->GetNextLexem(m_tlLexem);
	if(!Term(NameOwner,lTrm))
	{
		throw CExeption(NULL,"",14);
		return false;
	}
	Trm=lTrm;
	Pref_Expr(NameOwner,Expr,Trm);
	Expr->AddTermBeforeEnd(lTrm);
	return true;
}

bool CTranslator::Term(CGeneralObject* NameOwner,CTerm* &Trm)
{
	if(Atom(NameOwner,Trm)) goto _1;
	if(List(NameOwner,Trm)) goto _1;
	if(Block(NameOwner,Trm)) goto _1;
	if(m_tlLexem.Lex==ID) 
	{
		CGeneralObject* Tmp;
		if(!IDFind(m_tlLexem.Str,NameOwner,Tmp))
		{
			throw CExeption(NULL,string(m_tlLexem.Str),15);// InOutBuffer<<"\n"<<"Unknown identifier \""<< m_tlLexem.Str <<"\""<<"\n";
			return false;
		}
		Trm = (CTerm*) new CIdentifier(NameOwner,m_tlLexem);
		GlobalObjectTable.push_back(Trm);

		m_pScaner->GetNextLexem(m_tlLexem);
		goto _1;
	}
	return false;
_1: return true;
}

bool CTranslator::Atom(CGeneralObject* NameOwner,CTerm* &Trm)
{
	if(KW_Const(Trm)) return true;

	if((m_tlLexem.Lex>ENDF)&&(m_tlLexem.Lex<OPLASS))
	{
		Trm = new CAtom<ELexTypeEnum>(m_tlLexem.Lex);
		GlobalObjectTable.push_back(Trm);
		goto _1;
	};

	switch(m_tlLexem.Lex)
	{
	case INT_CONST : 
		{	
			Trm = new CAtom<int>(atoi(m_tlLexem.Str));
			GlobalObjectTable.push_back(Trm);
			goto _1;
		};
	case REAL_CONST :
		{
			Trm = new CAtom<float>(atof(m_tlLexem.Str));
			GlobalObjectTable.push_back(Trm);
			goto _1;
		};
	case BOOL_CONST : 
		{
			string Str=m_tlLexem.Str;
			if(Str=="true") Trm = new CAtom<bool>(true);
			else Trm = new CAtom<bool>(false);
			GlobalObjectTable.push_back(Trm);
			goto _1;
		}
	case SIGNAL_CONST : goto _1;
	case CHAR_CONST : 
		{
			Trm = new CAtom<char>(m_tlLexem.Str[0]);
			GlobalObjectTable.push_back(Trm);
			goto _1;
		};
	case ERROR_CONST : goto _1;
	case SPEC_CONST : goto _1;
	case STRING_CONST : 
		{
			Trm=new CList(NULL,DATA_LIST);
			GlobalObjectTable.push_back(Trm);

			for(unsigned int i=0;i<strlen(m_tlLexem.Str);i++)
			{
				CAtom<char>* TmpCh=new CAtom<char>(m_tlLexem.Str[i]);
				GlobalObjectTable.push_back(TmpCh);

				Trm->AddElem((CExpression*)TmpCh);
			}
			goto _1;
		};
	default:
		break;
	}
	return false;
_1:	m_pScaner->GetNextLexem(m_tlLexem);
	return true;
}

bool CTranslator::List(CGeneralObject* NameOwner,CTerm* &Trm)
{
	if(Data_List(NameOwner,Trm)) return true;
	if(Par_List(NameOwner,Trm)) return true;
	if(Delay_List(NameOwner,Trm)) return true;
	return false;
}

bool CTranslator::Block(CGeneralObject* NameOwner,CTerm* &Trm)
{
	if(m_tlLexem.Lex!=KWBLOCK) return false;

	Trm = new CBlock(NameOwner);
	GlobalObjectTable.push_back(Trm);

	m_bBlockContext=true;

	m_pScaner->GetNextLexem(m_tlLexem);
	if(m_tlLexem.Lex!=LFS) 
	{
		m_bBlockContext=false;
		throw CExeption(NULL,"",9);
		return false;
	}
	m_pScaner->GetNextLexem(m_tlLexem);
	if(m_tlLexem.Lex==RFS) goto _1; 
	do
	{
		if(!Element(Trm,Trm)) 
		{
			m_bBlockContext=false;
			return false;
		}

		if(m_tlLexem.Lex==RFS) goto _1;
		if(m_tlLexem.Lex!=DOTZAP) 
		{
			m_bBlockContext=false;
			throw CExeption(NULL,"",2);
			return false;
		}
		m_pScaner->GetNextLexem(m_tlLexem);	
		if(m_tlLexem.Lex==RFS) goto _1;
	}while(1);

	m_bBlockContext=false;
	return false;

_1:	m_pScaner->GetNextLexem(m_tlLexem);
	m_bBlockContext=false;
	return true;
}

bool CTranslator::Data_List(CGeneralObject* NameOwner,CTerm* &Trm)
{
	if(m_tlLexem.Lex!=LRS) return false;

	Trm = new CList(NameOwner,DATA_LIST);
	GlobalObjectTable.push_back(Trm);

	do
	{
		m_pScaner->GetNextLexem(m_tlLexem);
		if(!Element(NameOwner,Trm)) return false;
		if(m_tlLexem.Lex==RRS) goto _1;
		if(m_tlLexem.Lex!=ZPT) 
		{
			throw CExeption(NULL,"",13);
			return false;
		}
	}while(1);
	return false;
_1:	m_pScaner->GetNextLexem(m_tlLexem);
	return true;
}

bool CTranslator::Par_List(CGeneralObject* NameOwner,CTerm* &Trm)
{
	if(m_tlLexem.Lex!=LCS) return false;

	Trm = new CList(NameOwner,PAR_LIST);
	GlobalObjectTable.push_back(Trm);

	do
	{
		m_pScaner->GetNextLexem(m_tlLexem);
		if(!Element(NameOwner,Trm)) return false;
		if(m_tlLexem.Lex==RCS) goto _1;
		if(m_tlLexem.Lex!=ZPT) 
		{
			throw CExeption(NULL,"",13);
			return false;
		}
	}while(1);
	return false;
_1:	m_pScaner->GetNextLexem(m_tlLexem);
	return true;
}

bool CTranslator::Delay_List(CGeneralObject* NameOwner,CTerm* &Trm)
{
	if(m_tlLexem.Lex!=LFS) return false;

	Trm = new CList(NameOwner,DELAY_LIST);
	GlobalObjectTable.push_back(Trm);

	do
	{
		m_pScaner->GetNextLexem(m_tlLexem);
		if(!Element(NameOwner,Trm)) return false;
		if(m_tlLexem.Lex==RFS) goto _1;
		if(m_tlLexem.Lex!=ZPT) 
		{
			throw CExeption(NULL,"",13);
			return false;
		}
	}while(1);
	return false;
_1:	m_pScaner->GetNextLexem(m_tlLexem);
	return true;
}

bool CTranslator::KW_Const(CTerm* &Trm)
{
	switch(m_tlLexem.Lex)
	{
	case KWOUT : goto _1;
	case KWINT : goto _1;
	case KWFLOAT : goto _1;
	case KWBOOL : goto _1;
	case KWCHAR : goto _1;
	case KWDATALIST : goto _1;
	case KWPARLIST : goto _1;
	case KWDELAYLIST : goto _1;
	case KWSIGNAL : goto _1;
	case KWTYPE: goto _1;
	case KWVALUE: goto _1;
	case KWIN: goto _1;

/*	case KWRETURN : if(m_bBlockContext) throw CExeption(NULL,"",18);
					else goto _1;
	case KWBREAK : if(!m_bBlockContext) throw CExeption(NULL,"",17);
					else goto _1;*/
	default : return false;
	}
_1:	Trm = new CKeyWord(m_tlLexem);
	GlobalObjectTable.push_back(Trm);

	m_pScaner->GetNextLexem(m_tlLexem);
	return true;
}

bool CTranslator::IDFind(string ID,CGeneralObject* Owner,CGeneralObject* &Object)
{
	CGeneralObject* Tmp;
	CModule* OModule;
	Tmp=Owner;
	while(Tmp->Owner) Tmp=Tmp->Owner;
	OModule=(CModule*)Tmp;
	Tmp=Owner;
	if(Tmp!=OModule)
	{
		while((Tmp->Owner)!=OModule) Tmp=Tmp->Owner;	
		if(ID==((CFunction*)Tmp)->ParamID) 
		{
			Object=NULL;
			return true;
		};
	}
	if(OModule->NameTable.find(ID)!=OModule->NameTable.end()) 
	{
		Object=OModule->NameTable[ID];
		return true;
	}
	for(unsigned int i=0;i<GlobalNameList.size();i++)
	{
		if(GlobalNameList[i]==ID);
		Object=NULL;
		return true;
	}
	if(OModule->OverFuncTable.find(ID)!=OModule->OverFuncTable.end()) 
	{
		//Object=OProgram->OverFuncTable[ID];
		return true;
	}
	if(OModule->TypeDefTable.find(ID)!=OModule->TypeDefTable.end()) 
	{
		Object=OModule->TypeDefTable[ID];
		return true;
	}
	
	while(Owner!=OModule)
	{
		if(((CBlock*)Owner)->NameTable.find(ID)!=((CBlock*)Owner)->NameTable.end())
		{
			Object=((CBlock*)Owner)->NameTable[ID];
			return true;
		}
		Owner=Owner->Owner;
	};

	if(OModule->ConstTable.find(ID)!=OModule->ConstTable.end())
	{
		Object=OModule->ConstTable[ID];
		return true;
	}
	return false;
}


extern "C" bool CTranslator::Translate(char *FName,CModule* &ppModule)
{
	m_bBlockContext=false;
	m_pScaner->OpenAllFiles(FName);

	try
	{
		Module(ppModule);
	}
	catch(CExeption Tmp)
	{
		m_pScaner->CloseAllFiles();
		ppModule=NULL;
		throw Tmp;
		return false;
	}

	m_pScaner->CloseAllFiles();
	return true;
}

void CTranslator::GetCurrPos(int &LNum, int &CNum)
{
	m_pScaner->GetCurrPos(LNum,CNum);
}

bool CTranslator::CheckID(string ID,CModule* Owner,bool CheckOverFunc)
{
	if(Owner->NameTable.find(ID)!=Owner->NameTable.end())
	{
		if(Owner->NameTable[ID]!=NULL)
		{
			throw CExeption(NULL,string(ID),7);
			return false;
		};
	}
	if(Owner->ConstTable.find(ID)!=Owner->ConstTable.end())
	{
		throw CExeption(NULL,string(ID),5);
		return false;
	}

	if(Owner->TypeDefTable.find(ID)!=Owner->TypeDefTable.end())
	{
		throw CExeption(NULL,string(ID),20);
		return false;
	}
	if(CheckOverFunc)
	{
		if(Owner->OverFuncTable.find(ID)!=Owner->OverFuncTable.end())
		{
			throw CExeption(NULL,string(ID),21);
			return false;
		}
	}

	return true;
};

