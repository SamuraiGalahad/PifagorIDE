#ifndef __GENERALOBJECT_H_ICLUDED__
#define __GENERALOBJECT_H_ICLUDED__

#include "Types.h"

class CExpression;
class CGeneralObject;
class CFishka;
class CInterpretator;

class CGeneralObject
{
public:
	CGeneralObject() { };
	virtual  ~CGeneralObject() { };

	virtual void AddElem(CExpression* Expression) { };
	virtual void InsertNObject(TNamedObject Object) { };
	virtual void* GetObject(string Name) { return (CGeneralObject*)NULL;};

	virtual bool GetFishka(CFishka* Arg,CFishka* &Rezult,bool &Trace,CInterpretator* Interpretator) {return false;};

	virtual void SetRezFishka(CFishka* Fishka) { };
	virtual bool IsList() {return false;};
	virtual bool IsFunc() {return false;};

	CGeneralObject* Owner;
	string SelfName;
};

#endif
