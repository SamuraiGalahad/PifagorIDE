#ifndef __TRANSLATOR_H_HEADER_INCLUDED__
#define __TRANSLATOR_H_HEADER_INCLUDED__


#include <vector>

#include "Scaner.h"

class CFunction;
class CGeneralObject;
class CModule;
class CTerm;
class CExpression;

class CTranslator  
{
public:
	CTranslator(CScaner *Scaner);
    virtual ~CTranslator();

    std::vector<CGeneralObject*> GlobalObjectTable;

	virtual void GetCurrPos(int &LNum, int &CNum);
	virtual bool Translate(char *FName, CModule* &ppProgram);

	CScaner *m_pScaner;
	TLexem m_tlLexem;

    virtual bool IDFind(std::string ID,CGeneralObject* Owner,CGeneralObject* &Object);
    virtual bool KW_Const(CTerm* &Trm);
    virtual bool Delay_List(CGeneralObject* NameOwner,CTerm* &Trm);
    virtual bool Par_List(CGeneralObject* NameOwner,CTerm* &Trm);
    virtual bool Data_List(CGeneralObject* NameOwner,CTerm* &Trm);
    virtual bool Block(CGeneralObject* NameOwner,CTerm* &Trm);
    virtual bool List(CGeneralObject* NameOwner,CTerm* &Trm);
    virtual bool Atom(CGeneralObject* NameOwner,CTerm* &Trm);
    virtual bool Term(CGeneralObject* NameOwner,CTerm* &Trm);
    virtual bool Pref_Expr(CGeneralObject* NameOwner,CExpression* &Expr,CTerm* &Trm);
    virtual bool Expression(CGeneralObject* NameOwner,CExpression* &Expr);
    virtual bool Left_Named_El(CGeneralObject* NameOwner,CGeneralObject* Owner,CExpression* &Expr);
    virtual bool Element(CGeneralObject* NameOwner,CGeneralObject* Owner);
    virtual bool Function(CModule* Owner,CFunction* &Func,std::string Name);
    virtual bool UserType(CModule* Owner,CFunction* &Func,std::string Name);
    virtual bool Left_Named_Def(CModule* Owner,CExpression* &Expr,CFunction* &Func,CFunction* &TypeDef,float &Rang);
    virtual bool Const_Definition(CModule* Owner,CExpression* &Expr);
    virtual bool Definition(CModule* Owner,CFunction* &Func,std::string Name);
    virtual bool DefinitionOverFunc(CModule* Owner,CFunction* &Func,std::string Name);
    virtual bool UserTypeDefinition(CModule* Owner,CFunction* &Func, std::string Name);
    virtual bool Named_Def(CModule* Owner);
    virtual bool Module(CModule* &iModule);
    virtual bool Const_Par_List(CGeneralObject* NameOwner,CTerm* &Trm);
    virtual bool Const_Data_List(CGeneralObject* NameOwner,CTerm* &Trm);
    virtual bool Const_List(CGeneralObject* NameOwner,CTerm* &Trm);
    virtual bool Const_Atom(CGeneralObject* NameOwner,CTerm* &Trm);
    virtual bool Const_Pref_Expr(CGeneralObject* NameOwner,CExpression* &Expr);
    virtual bool Const_Term(CGeneralObject* NameOwner,CTerm* &Trm);
    virtual bool Const_Expr(CGeneralObject* NameOwner,CExpression* &Expr);
    virtual bool Const_Left_Named_El(CGeneralObject* NameOwner,CGeneralObject* Owner,CExpression* &Expr);
    virtual bool Const_Element(CGeneralObject* NameOwner,CGeneralObject* Owner);

    virtual void EraseGlobalObjectTable();


protected:
    bool m_bBlockContext;
    bool CheckID(std::string ID,CModule* Owner,bool CheckOverFunc);
};

#endif // !defined(AFX_TRANSLATOR_H__71A25D04_D9E2_11D5_AB5D_C707748D444A__INCLUDED_)
