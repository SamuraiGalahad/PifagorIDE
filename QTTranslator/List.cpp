#include "Precompiled.h"

#include "List.h"
#include "Fishka.h"
#include "Expression.h"
#include "ObjectFishka.h"
#include "ListFishka.h"

CList::CList(CGeneralObject* Owner,ELexTypeEnum iType) : CTerm()
{
	Type=iType;
	SelfName="list";
	ElseTerm=NULL;
};

CList::~CList()
{
/*	vector<PExpression>::iterator i;
	for (i = ElemList.begin(); i != ElemList.end(); i++)
		if(*i) delete (*i);
	if(ElseTerm) delete ElseTerm;*/
}

void CList::AddElem(CExpression* Expr)
{
	ElemList.push_back(Expr);
}

bool CList::GetFishka(CFishka* Arg,CFishka* &Rezult,bool &Trace,CInterpretator* Interpretator)
{
    std::vector<CExpression*>::iterator i;
	int j=0;
	if(Type==DELAY_LIST)
	{
		if(ElemList.size()==1)
		{
			Rezult=new CObjectFishka(DELAY_FISHKA,(CGeneralObject*)(ElemList[0]));
			return true;
		};
		Rezult=new CListFishka(DELAY_LIST,ElemList.size());
		for(i=ElemList.begin();i!=ElemList.end();i++)
		{
			(*(CListFishka*)Rezult)[j]=new CObjectFishka(DELAY_FISHKA,(CGeneralObject*)(*i));
			j++;
		}
		return true;
	}
	Rezult = new CListFishka(Type,ElemList.size());
	for(i=ElemList.begin();i!=ElemList.end();i++)
	{
		if(!((*i)->GetFishka(Arg,(*(CListFishka*)Rezult)[j],Trace,Interpretator))) return false;
		j++;
	};
	Rezult=((CListFishka*)Rezult)->CheckOut();
	return true;
};
//---------------------------------------------------------------
//---------------------------------------------------------------
/*void CInterpretator::ProcList(CFishka* &Arg)
{
	if((Arg->Type==PAR_LIST)&&
		(((CListFishka*)Arg)->ElemCount==1))
	{
		Arg=(*(CListFishka*)Arg)[0];
		return;
	}

	if((Arg->Type==PAR_LIST)||
		(Arg->Type==DATA_LIST))
	{
		if(Arg->Type!=PAR_LIST)
		{
			if(((CListFishka*)Arg)->ElemCount>1)
			{
				int DotCount=0;
				for(int i=0;i<((CListFishka*)Arg)->ElemCount;i++)
					if((*(CListFishka*)Arg)[i]->Type==OPDOT) DotCount++;
				if(DotCount>0)
				{
					int TmpCount=((CListFishka*)Arg)->ElemCount;
					CListFishka* Tmp=new CListFishka(Arg->Type,TmpCount-DotCount);
					int k=0;
					for(int i=0;i<((CListFishka*)Arg)->ElemCount;i++)
						if((*(CListFishka*)Arg)[i]->Type!=OPDOT)
							(*Tmp)[k++]=(*(CListFishka*)Arg)[i];
						Arg=Tmp;
				}
			}
		}

		int PCount=0;
		for(int i=0;i<((CListFishka*)Arg)->ElemCount;i++)
			if((*(CListFishka*)Arg)[i]->Type==PAR_LIST) PCount++;
		if(PCount>0)
		{
			int AllElemCount=0;
			for(i=0;i<((CListFishka*)Arg)->ElemCount;i++)
			{
				if(((*(CListFishka*)Arg)[i]->Type)==PAR_LIST)
					AllElemCount+=((CListFishka*)(*(CListFishka*)Arg)[i])->ElemCount;
				else AllElemCount++;
			}
			//int y=AllElemCount;
			CListFishka* Tmp=new CListFishka(Arg->Type,AllElemCount);
			int k=0;
			for(i=0;i<((CListFishka*)Arg)->ElemCount;i++)
			{
				if(((*(CListFishka*)Arg)[i]->Type)==PAR_LIST)
					for(int j=0;j<((CListFishka*)(*(CListFishka*)Arg)[i])->ElemCount;j++)
					{
						(*Tmp)[k++]=(*(CListFishka*)(*(CListFishka*)Arg)[i])[j];
					}
				else (*Tmp)[k++]=(*(CListFishka*)Arg)[i];
			}
			Arg=Tmp;
		}
		for(i=0;i<((CListFishka*)Arg)->ElemCount;i++)
			ProcList((*(CListFishka*)Arg)[i]);
	}
	else return;
}*/



