#include <string.h>

#include "Precompiled.h"

#include "ListFishka.h"
#include "AtomFishka.h"
#include "List.h"
#include "Scaner.h"

CListFishka::CListFishka(CFishka* Fishka)
{
	Type=Fishka->Type;
	CListFishka* Tmp=(CListFishka*)Fishka;
	ElemCount=Tmp->ElemCount;
	Elements=(CFishka**)malloc(sizeof(CFishka*)*ElemCount);
	for(int i=0;i<ElemCount;i++)
		Elements[i]=GetCopyFishka((*Tmp)[i]);
};

CListFishka::CListFishka(ELexTypeEnum iType,int iElemCount)
{
	Type=iType;
	ElemCount=iElemCount;
	Elements=(CFishka**)malloc(sizeof(CFishka*)*ElemCount);
};

CListFishka::~CListFishka()
{
	free(Elements);
}

void CListFishka::OutConsole()
{
	int CharCount=0;
	int i;
	for(i=0;i<ElemCount;i++)
		if((Elements[i]->Type)==CHAR_CONST) CharCount++;
	if(CharCount==ElemCount)
	{
		switch(Type)
		{
			case DATA_LIST:{ strcat(data,"("); break;};
			case PAR_LIST:{  strcat(data,"["); break;};
			case DELAY_LIST:{ strcat(data,"{");break;};
			default: break;
		};
		strcat(data,"\\");
		for(i=0;i<(ElemCount-1);i++)
			if(Elements[i]) sprintf(data,"%s%c",data,((CAtomFishka<char>*)Elements[i])->Val);
		if(Elements[i]) sprintf(data,"%s%c",data,((CAtomFishka<char>*)Elements[i])->Val);
		strcat(data,"\"");
		switch(Type)
		{
			case DATA_LIST:{ strcat(data,")"); break;};
			case PAR_LIST:{  strcat(data,"]"); break;};
			case DELAY_LIST:{ strcat(data,"}");break;};
			default: break;
		};
		return;
	}
	switch(Type)
	{
		case DATA_LIST:{ strcat(data,"("); break;};
		case PAR_LIST:{  strcat(data,"["); break;};
		case DELAY_LIST:{ strcat(data,"{");break;};
		default: break;
	};
	for(i=0;i<(ElemCount-1);i++)
	{
		if(Elements[i]) Elements[i]->OutConsole();
		strcat(data,",");
	};
	if(Elements[i]) Elements[i]->OutConsole();
	switch(Type)
	{
		case DATA_LIST:{ strcat(data,")"); break;};
		case PAR_LIST:{  strcat(data,"]"); break;};
		case DELAY_LIST:{ strcat(data,"}");break;};
		default: break;
	};
};

CGeneralObject* CListFishka::GetObject()
{
	CGeneralObject* Rez=new CList(NULL,Type);
	for(int i=0;i<ElemCount;i++)
		Rez->AddElem((CExpression*)(Elements[i]->GetObject()));
	return Rez;
}

CFishka* &CListFishka::operator[](int i)
{
	return Elements[i];
}

CFishka* CListFishka::operator[](int i) const
{
	return Elements[i];
}

CFishka* CListFishka::CheckOut()
{
	CFishka* Rez=this;
	int iL;
	// CHECK OUT DOTS AND PARLIST INSIDE LIST //////////////////
	if((Type==PAR_LIST)&&
		(ElemCount==1))
	{
		Rez=(*this)[0];
		return Rez;
	}

	int PCount=0;
	for(int iL=0;iL<ElemCount;iL++)
		if((*this)[iL]->Type==PAR_LIST) PCount++;

	if(PCount>0)
	{
		int AllElemCount=0;
		for(iL=0;iL<ElemCount;iL++)
		{
			if(((*this)[iL]->Type)==PAR_LIST)
				AllElemCount+=((CListFishka*)(*this)[iL])->ElemCount;
			else AllElemCount++;
		}
		//int y=AllElemCount;
		CListFishka* Tmp=new CListFishka(Type,AllElemCount);
		int k=0;
		for(iL=0;iL<ElemCount;iL++)
		{
			if(((*this)[iL]->Type)==PAR_LIST)
				for(int j=0;j<((CListFishka*)(*this)[iL])->ElemCount;j++)
				{
					(*Tmp)[k++]=(*(CListFishka*)(*this)[iL])[j];
				}
				else (*Tmp)[k++]=(*this)[iL];
		}
		Rez=Tmp;
	}

	if(Rez->Type!=PAR_LIST)
	{
		if(((CListFishka*)Rez)->ElemCount>1)
		{
			int DotCount=0;
			for(int i=0;i<((CListFishka*)Rez)->ElemCount;i++)
				if((*((CListFishka*)Rez))[i]->Type==OPDOT) DotCount++;
			if(DotCount>0)
			{
				int TmpCount=((CListFishka*)Rez)->ElemCount;
				CListFishka* Tmp=new CListFishka(Rez->Type,TmpCount-DotCount);
				int k=0;
				for(int i=0;i<((CListFishka*)Rez)->ElemCount;i++)
					if((*((CListFishka*)Rez))[i]->Type!=OPDOT)
						(*Tmp)[k++]=(*((CListFishka*)Rez))[i];
				Rez=Tmp;
			}
		}
	}
	return Rez;
}
