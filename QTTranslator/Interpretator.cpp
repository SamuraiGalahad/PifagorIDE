#include "Precompiled.h"

#include "Scaner.h"
#include "Interpretator.h"
#include "Fishka.h"
#include "GeneralObject.h"
#include "AtomFishka.h"
#include "ObjectFishka.h"
#include "ListFishka.h"
#include "SignalFishka.h"
#include "WrapFishka.h"

//---------------------------------------------------------------
//---------------------------------------------------------------
CInterpretator::CInterpretator()
{
}
//---------------------------------------------------------------
//---------------------------------------------------------------
CInterpretator::~CInterpretator()
{
}
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::ErrElemFind(CFishka* Arg)
{
	if(Arg->Type==ERROR_CONST) return true;
	if((Arg->Type==DATA_LIST)||
		(Arg->Type==PAR_LIST)||
		(Arg->Type==DELAY_LIST))
	{
		for(int i=0;i<((CListFishka*)Arg)->ElemCount;i++)
			if(ErrElemFind((*(CListFishka*)Arg)[i])) return true;
	}
	return false;
};
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::InterpretationOperator(CFishka* FuncArg,CFishka* Arg,CFishka* Oper,CFishka* &Rezult,bool &Trace)
{
//	ProcList(Arg);//  [[a,b],[c,d,e]] => [a,b,c,d,e]    ([a,b],[c,d,e]) => (a,b,c,d,e)
//	ProcList(Oper);//  [[a,b],[c,d,e]] => [a,b,c,d,e]   ([a,b],[c,d,e]) => (a,b,c,d,e)

	if(ErrElemFind(Arg)||ErrElemFind(Oper)) 
	{
		Rezult=GetCopyFishka(Arg);
		return false;
	}
	if(Arg->Type==DELAY_FISHKA) // {{expression1}} => [expression2]
	{
		while(Arg->Type==DELAY_FISHKA)
		{
			if(!((CObjectFishka*)Arg)->GetObject()->GetFishka(FuncArg,Arg,Trace,this))
			{
				Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
				return false;
			};
		}
	}
	if(Oper->Type==DELAY_FISHKA) // {{exression1}} => [expression2]
	{
		while(Oper->Type==DELAY_FISHKA)
		{
			if(!((CObjectFishka*)Oper)->GetObject()->GetFishka(FuncArg,Oper,Trace,this))	
			{
				Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
				return false;
			}
		}
	}

	if(Arg->Type==DELAY_LIST) // {expr1,expr2,expr3} => [expr1_1,expr2_1,expr3_1]
	{
		CListFishka* Tmp=new CListFishka(PAR_LIST,((CListFishka*)Arg)->ElemCount);
		for(int i=0;i<Tmp->ElemCount;i++)
		{
			if((*(CListFishka*)Arg)[i]->Type==DELAY_FISHKA)
				if(!((*(CListFishka*)Arg)[i])->GetObject()->GetFishka(FuncArg,(*Tmp)[i],Trace,this))
				{
					Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
					return false;
				}
		};
		Arg=Tmp->CheckOut();
	}
	if(Oper->Type==DELAY_LIST) // {expr1,expr2,expr3} => [expr1_1,expr2_1,expr3_1]
	{
		CListFishka* Tmp=new CListFishka(PAR_LIST,((CListFishka*)Oper)->ElemCount);
		for(int i=0;i<Tmp->ElemCount;i++)
		{
			if((*(CListFishka*)Oper)[i]->Type==DELAY_FISHKA)
				if(!((*(CListFishka*)Oper)[i])->GetObject()->GetFishka(FuncArg,(*Tmp)[i],Trace,this))	
				{
					Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
					return false;
				}
		};
//		Oper=Tmp;
		Oper=Tmp->CheckOut();
	}

	if((Arg->Type!=DATA_LIST)&&			// a:(f,g,c) => (a:f,a:g,a:c)
		(Arg->Type!=PAR_LIST))			// a:[f,g,c] => [a:f,a:g,a:c]
	{
		if((Oper->Type==DATA_LIST)||
			(Oper->Type==PAR_LIST))
		{
			Rezult = new CListFishka(Oper->Type,((CListFishka*)Oper)->ElemCount);
			bool ErrFound=false;
			for(int i=0;i<((CListFishka*)Oper)->ElemCount;i++)
				if(!InterpretationOperator(FuncArg,Arg,(*(CListFishka*)Oper)[i],
				(*(CListFishka*)Rezult)[i],Trace)) ErrFound=true;
			Rezult=((CListFishka*)Rezult)->CheckOut();
			return !ErrFound;
		}
	}

	if((Oper->Type==PAR_LIST)&&(Arg->Type==PAR_LIST))  // [a,b,c]:[f,g] => [[a,b,c]:f,[a,b,c]:gg]
	{		
/*	Old	Rezult = new CListFishka(PAR_LIST,(((CListFishka*)Oper)->ElemCount));
		bool ErrFound=false;
		for(int i=0;i<((CListFishka*)Oper)->ElemCount;i++)
			if(!InterpretationOperator(FuncArg,Arg,
				(*(CListFishka*)Oper)[i],
				(*(CListFishka*)Rezult)[i],Trace)) ErrFound=true;
		Rezult=((CListFishka*)Rezult)->CheckOut();
		return !ErrFound;*/
		  // [a,b,c]:[f,g] => [a:[f,g],b:[f,g],c:[f,g]]
		Rezult = new CListFishka(PAR_LIST,(((CListFishka*)Arg)->ElemCount));
		bool ErrFound=false;
		for(int i=0;i<((CListFishka*)Arg)->ElemCount;i++)
			if(!InterpretationOperator(FuncArg,(*(CListFishka*)Arg)[i],
				Oper,
				(*(CListFishka*)Rezult)[i],Trace)) ErrFound=true;
		Rezult=((CListFishka*)Rezult)->CheckOut();
		return !ErrFound;
	};


	if(Arg->Type==PAR_LIST)
	{
		if((Oper->Type!=DATA_LIST)&&  // [a,b,c]:f => [a:f,b:f,c:f]
			(Oper->Type!=PAR_LIST))
		{
			Rezult = new CListFishka(PAR_LIST,((CListFishka*)Arg)->ElemCount);
			bool ErrFound=false;
			for(int i=0;i<((CListFishka*)Arg)->ElemCount;i++)
				if(!InterpretationOperator(FuncArg,(*(CListFishka*)Arg)[i],
					Oper,(*(CListFishka*)Rezult)[i],Trace)) ErrFound=true;
			Rezult=((CListFishka*)Rezult)->CheckOut();
			return !ErrFound;
		}
		if(Oper->Type==DATA_LIST)	// [a,b,c]:(f,g) => ([a:b,c]:[f,g]) => 
		{							// ([a:f,b:f,c:f,a:g,b:g,c:g]) => (a:f,b:f,c:f,a:g,b:g,c:g)
			Rezult = new CListFishka(DATA_LIST,(((CListFishka*)Arg)->ElemCount)*(((CListFishka*)Oper)->ElemCount));
			bool ErrFound=false;
			for(int i=0;i<((CListFishka*)Oper)->ElemCount;i++)
				for(int j=0;j<((CListFishka*)Arg)->ElemCount;j++)
					if(!InterpretationOperator(FuncArg,(*(CListFishka*)Arg)[j],
						(*(CListFishka*)Oper)[i],
						(*(CListFishka*)Rezult)[i*((CListFishka*)Arg)->ElemCount+j],
						Trace)) ErrFound=true;
			Rezult=((CListFishka*)Rezult)->CheckOut();
			return !ErrFound;
		}
	}

	if(Arg->Type==DATA_LIST)
	{ 
		if((Oper->Type==PAR_LIST)||		// (a,b,c):[f,g] => [(a,b,c):f,(a,b,c):g]
			(Oper->Type==DATA_LIST))	// (a,b,c):(f,g) => ((a,b,c):f,(a,b,c):g)
		{
			Rezult = new CListFishka(Oper->Type,((CListFishka*)Oper)->ElemCount);
			bool ErrFound=false;
			for(int i=0;i<((CListFishka*)Oper)->ElemCount;i++)
				if(!InterpretationOperator(FuncArg,Arg,(*(CListFishka*)Oper)[i],
					(*(CListFishka*)Rezult)[i],Trace)) ErrFound=true;
			Rezult=((CListFishka*)Rezult)->CheckOut();
			return !ErrFound;
		}
	}

	switch(Oper->Type)   // (a,b,c):f => (a,b,c):f
	{
		case OPTRANS:{return Trans((CListFishka*)Arg,Rezult);};	// #
		case OPQ:{return Qwestion((CListFishka*)Arg,Rezult);};	// ?
		case OPLEN: // |
			{	
				if(Arg->Type==DATA_LIST)
				{
					if(((CListFishka*)Arg)->ElemCount==1)
					{
						if((*((CListFishka*)Arg))[0]->Type==OPDOT)
						{
							Rezult=new CAtomFishka<int>(0);
							return true;
						}
						else
						{
							Rezult= new CAtomFishka<int>(((CListFishka*)Arg)->ElemCount);
							return true;
						}
					}
					else
					{					
						Rezult= new CAtomFishka<int>(((CListFishka*)Arg)->ElemCount);
						return true;
					}
				}
				else 
				{
					Rezult=new CAtomFishka<int>(0);
					return true;
				}
			};
		case INT_CONST:{ return IntSelector((CListFishka*)Arg,Oper,Rezult);};	// (x_1,x_2,...,x_n):i => x_i 
		case BOOL_CONST:{ return BoolSelector((CListFishka*)Arg,Oper,Rezult);};	// (x_1,x_2,...,x_n):true => x_1 
																				// (x_1,x_2,...,x_n):false => x_2 

		case KWDATALIST:
		case OPRS:
			{
				Rezult=new CListFishka(DATA_LIST,1);		// a:() => (a)
				(*(CListFishka*)Rezult)[0]=GetCopyFishka(Arg);	// (a,b,c) => ((a,b,c))
				return true;
			}

		case KWPARLIST: 
		case OPCS:
			{
				if(Arg->Type==DATA_LIST)		// (a,b,c):[] => [a,b,c]
				{
					if(((CListFishka*)Arg)->ElemCount==1)
					{
						Rezult=(*(CListFishka*)Arg)[0];
						return true;
					}
					Rezult=new CListFishka(Arg);
					Rezult->Type=PAR_LIST;
					return true;
				}
				else							// a:[] => a
				{
					Rezult=GetCopyFishka(Arg);
					return true;
				}
			}
		case KWSIGNAL:
			{
				Rezult=new CAtomFishka<ELexTypeEnum>(OPDOT);
				return true;
			}
		case KWTYPE:
			{
				{ return Type(Arg,Rezult);} // type
			}
		case KWVALUE:
			{
				{ return Value(Arg,Rezult);}
			}
		case KWIN:
			{
				{ return In(Arg,Rezult,Trace);}
			}
		case SIGNAL_CONST:
			{
				Rezult=GetCopyFishka(Arg);
				return true;
			}
		case OPADD:{return Add((CListFishka*)Arg,Rezult);};// + (||)
		case OPSUB:{return Sub((CListFishka*)Arg,Rezult);};// - 
		case OPMUL:{return Mul((CListFishka*)Arg,Rezult);};// * (&&)
		case OPDIV:{return Div((CListFishka*)Arg,Rezult);};// /
		case OPIDIV:{return IDiv((CListFishka*)Arg,Rezult);};// %
		case OPDUP:{return Dup((CListFishka*)Arg,Rezult);};// dup
		case OPDDOT:{return DDot((CListFishka*)Arg,Rezult);};// ..
		case OPDOT:{Rezult=GetCopyFishka(Arg); return true;}; // .
		case OPL:{return Less((CListFishka*)Arg,Rezult);};// <
		case OPG:{return Great((CListFishka*)Arg,Rezult);};// >
		case OPLE:{return LessEqu((CListFishka*)Arg,Rezult);};// <= / =<
		case OPGE:{return GreatEqu((CListFishka*)Arg,Rezult);};// >= / =>
		case OPE:{return Equal((CListFishka*)Arg,Rezult);};// =
		case OPNE:{return NotEqual((CListFishka*)Arg,Rezult);};// !=

		case KWINT:{return CastScalarType(Arg,Oper,Rezult); };
		case KWFLOAT:{return CastScalarType(Arg,Oper,Rezult); };
		case KWBOOL:{return CastScalarType(Arg,Oper,Rezult); };
		case KWCHAR:{return CastScalarType(Arg,Oper,Rezult); };

	//	case KWRETURN:{Rezult=GetCopyFishka(Arg);return true;};// return
	//	case KWBREAK:{Rezult=GetCopyFishka(Arg);return true;};// break

		case KWOUT:		// out
			{
				Rezult=GetCopyFishka(Arg);
				Arg->OutConsole();
				printf("\n");
				return true;
			};
		case FUNC_FISHKA:
			{
				return Oper->GetObject()->GetFishka(Arg,Rezult,Trace,this);
				return true;
			};
		case TYPEDEF_FISHKA:
			{
				return CastUserType(Arg,Oper,Rezult,Trace);
				return true;
			};
		default:
			break;
	};
	Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
	return false;
};
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::Add(CListFishka* Arg,CFishka* &Rezult)
{
	if((Arg->Type!=DATA_LIST)&&
		(Arg->Type!=PAR_LIST)&&
		(Arg->Type!=DELAY_LIST)) 
	{
		Rezult=GetCopyFishka(Arg);
		return true;
	};

	int i=0;
	// Scalar Addition
	while((i<Arg->ElemCount)&&
		(((*Arg)[i]->Type==INT_CONST)||
		((*Arg)[i]->Type==REAL_CONST))) i++;
	if((i==2)&&(Arg->ElemCount==2))
	{
		if(((*Arg)[0]->Type==INT_CONST)&&((*Arg)[1]->Type==INT_CONST))
		{
			Rezult= new CAtomFishka<int>(IntV((*Arg)[0])+IntV((*Arg)[1]));
			return true;
		}

		float Tmp1=0;
		float Tmp2=0;

		if((*Arg)[0]->Type==INT_CONST) Tmp1=IntV((*Arg)[0]);
		else Tmp1=FloatV((*Arg)[0]);
		if((*Arg)[1]->Type==INT_CONST) Tmp2=IntV((*Arg)[1]);
		else Tmp2=FloatV((*Arg)[1]);
		Rezult= new CAtomFishka<float>(Tmp1+Tmp2);
		return true;
	}
	// Char Addition
	if((Arg->ElemCount==2)&&
		((*Arg)[0]->Type==CHAR_CONST)&&
		((*Arg)[1]->Type==CHAR_CONST))
	{
		Rezult= new CAtomFishka<char>(CharV((*Arg)[0])+CharV((*Arg)[1]));
		return true;
	};	
	// Bool Addition
	i=0;
	while((i<Arg->ElemCount)&&
		((*Arg)[i]->Type==BOOL_CONST)) i++;
	if(i==Arg->ElemCount)
	{
		bool Tmp=BoolV((*Arg)[0]);
		for(i=1;i<Arg->ElemCount;i++)
			Tmp=Tmp||BoolV((*Arg)[i]);
		Rezult= new CAtomFishka<bool>(Tmp);
		return true;
	};	
	Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
	return false;
};
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::Sub(CListFishka* Arg,CFishka* &Rezult)
{
	switch(Arg->Type)
	{
	case INT_CONST:{ Rezult=new CAtomFishka<int>(-IntV(Arg)); return true;};
	case REAL_CONST:{ Rezult=new CAtomFishka<float>(-FloatV(Arg)); return true;};
	case BOOL_CONST:{ Rezult=new CAtomFishka<bool>(!BoolV(Arg)); return true;};
	default: break;
	}
	int i=0;
	// Scalar Subtraction
	while((i<Arg->ElemCount)&&
		(((*Arg)[i]->Type==INT_CONST)||
		((*Arg)[i]->Type==REAL_CONST))) i++;
	if((i==2)&&(Arg->ElemCount==2))
	{
		if(((*Arg)[0]->Type==INT_CONST)&&((*Arg)[1]->Type==INT_CONST))
		{
			Rezult= new CAtomFishka<int>(IntV((*Arg)[0])-IntV((*Arg)[1]));
			return true;
		};

		float Tmp1=0;
		float Tmp2=0;
		
		if((*Arg)[0]->Type==INT_CONST) Tmp1=IntV((*Arg)[0]);
		else Tmp1=FloatV((*Arg)[0]);
		if((*Arg)[1]->Type==INT_CONST) Tmp2=IntV((*Arg)[1]);
		else Tmp2=FloatV((*Arg)[1]);
		Rezult= new CAtomFishka<float>(Tmp1-Tmp2);
		return true;
	};
	// Char Subtraction
	if((Arg->ElemCount==2)&&
		((*Arg)[0]->Type==CHAR_CONST)&&
		((*Arg)[1]->Type==CHAR_CONST))
	{
		Rezult= new CAtomFishka<char>(CharV((*Arg)[0])-CharV((*Arg)[1]));
		return true;
	};	
	Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
	return false;
};
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::Mul(CListFishka* Arg,CFishka* &Rezult)
{
	if(Arg->Type!=DATA_LIST)
	{
		Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
		return false;
	}
	int i=0;
	// Scalar Multiplication
	while((i<Arg->ElemCount)&&
		(((*Arg)[i]->Type==INT_CONST)||
		((*Arg)[i]->Type==REAL_CONST))) i++;
	if((i==2)&&(Arg->ElemCount==2))
	{
		if(((*Arg)[0]->Type==INT_CONST)&&((*Arg)[1]->Type==INT_CONST))
		{
			Rezult= new CAtomFishka<int>(IntV((*Arg)[0])*IntV((*Arg)[1]));
			return true;
		}

		float Tmp1=0;
		float Tmp2=0;
		
		if((*Arg)[0]->Type==INT_CONST) Tmp1=IntV((*Arg)[0]);
		else Tmp1=FloatV((*Arg)[0]);
		if((*Arg)[1]->Type==INT_CONST) Tmp2=IntV((*Arg)[1]);
		else Tmp2=FloatV((*Arg)[1]);
		Rezult= new CAtomFishka<float>(Tmp1*Tmp2);
		return true;
	};
	// Bool Multiplication
	i=0;
	while((i<Arg->ElemCount)&&
		((*Arg)[i]->Type==BOOL_CONST)) i++;
	if(i==Arg->ElemCount)
	{
		bool Tmp=BoolV((*Arg)[0]);
		for(i=1;i<Arg->ElemCount;i++)
			Tmp=Tmp&&BoolV((*Arg)[i]);
		Rezult= new CAtomFishka<bool>(Tmp);
		return true;
	};	
	Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
	return false;
};
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::Div(CListFishka* Arg,CFishka* &Rezult)
{
	if((Arg->Type==DATA_LIST)&&(Arg->ElemCount==2))
	{
		float Tmp1=0;
		float Tmp2=0;
		
		if((*Arg)[0]->Type==INT_CONST) Tmp1=IntV((*Arg)[0]);
		else Tmp1=FloatV((*Arg)[0]);
		if((*Arg)[1]->Type==INT_CONST) Tmp2=IntV((*Arg)[1]);
		else Tmp2=FloatV((*Arg)[1]);
		if(Tmp2!=0)
		{
			Rezult= new CAtomFishka<float>(Tmp1/Tmp2);
			return true;
		}
		Rezult=new CAtomFishka<ERuntimeErrorEnum>(ZERO_DIVIDE);
		return false;
	}
	Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
	return false;
}
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::IDiv(CListFishka* Arg,CFishka* &Rezult)
{
	bool TmpErr=false;
	if((Arg->Type==DATA_LIST)&&(Arg->ElemCount==2))
	{
		int Tmp1=0;
		int Tmp2=0;
		
		if((*Arg)[0]->Type==INT_CONST) Tmp1=IntV((*Arg)[0]);
		else 
		{
			Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
			return false;
		}
		if((*Arg)[1]->Type==INT_CONST) Tmp2=IntV((*Arg)[1]);
		else 
		{
			Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
			return false;
		}
		Rezult=new CListFishka(DATA_LIST,2);
		if(Tmp2!=0)	(*(CListFishka*)Rezult)[0]=new CAtomFishka<int>(Tmp1/Tmp2);
		else 
		{
			Rezult=new CAtomFishka<ERuntimeErrorEnum>(ZERO_DIVIDE);
			TmpErr=true;
		}
		if(Tmp2!=0)	(*(CListFishka*)Rezult)[1]=new CAtomFishka<int>(Tmp1%Tmp2);
		else 
		{
			Rezult=new CAtomFishka<ERuntimeErrorEnum>(ZERO_DIVIDE);
			TmpErr=true;
		}
		return TmpErr;
	}
	Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
	return false;
}
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::Dup(CListFishka* Arg,CFishka* &Rezult)
{
	if(Arg->Type!=DATA_LIST) 
	{
		Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
		return false;
	}
	if(Arg->ElemCount!=2) 
	{
		Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
		return false;
	}
	if((*Arg)[1]->Type!=INT_CONST) 
	{
		Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
		return false;
	}
	Rezult= new CListFishka(DATA_LIST,((CAtomFishka<int>*)(*Arg)[1])->Val);
	for(int i=0;i<((CListFishka*)Rezult)->ElemCount;i++)
		(*(CListFishka*)Rezult)[i]=GetCopyFishka((*Arg)[0]);
	return true;
};
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::DDot(CListFishka* Arg,CFishka* &Rezult)
{
	if(Arg->Type!=DATA_LIST) 
	{
		Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
		return false;
	}
	if(Arg->ElemCount<2) 
	{
		Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
		return false;
	}
	int i=0;

	while((i<Arg->ElemCount)&&
		(((*Arg)[i]->Type==INT_CONST)||
		((*Arg)[i]->Type==REAL_CONST))) i++;
	if(i<Arg->ElemCount) 
	{
		Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
		return false;
	}

	int ECount;
	int Tmp1=0;
	int Tmp2=0;
	int Tmp3=0;
	float Tmp4=0;
	float Tmp5=0;
	float Tmp6=0;
	if((*Arg)[0]->Type==INT_CONST) Tmp1=((CAtomFishka<int>*)(*Arg)[0])->Val;
	if((*Arg)[0]->Type==REAL_CONST) Tmp4=((CAtomFishka<float>*)(*Arg)[0])->Val;
	if((*Arg)[1]->Type==INT_CONST) Tmp2=((CAtomFishka<int>*)(*Arg)[1])->Val;
	if((*Arg)[1]->Type==REAL_CONST) Tmp5=((CAtomFishka<float>*)(*Arg)[1])->Val;
	if(Arg->ElemCount==3)
	{
		if((*Arg)[2]->Type==INT_CONST) Tmp3=((CAtomFishka<int>*)(*Arg)[2])->Val;
		if((*Arg)[2]->Type==REAL_CONST) Tmp6=((CAtomFishka<float>*)(*Arg)[2])->Val;
	}
	else
	{
		Tmp3=1;
		Tmp6=0;
	}

	ECount=(int)(((float)((Tmp2+Tmp5)-(Tmp1+Tmp4)))/((float)(Tmp3+Tmp6))+1);
	
	if(ECount<=0) 
	{
		Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
		return false;
	}
	Rezult= new CListFishka(DATA_LIST,ECount);

	if(Arg->ElemCount==3)
	{
		if(((*Arg)[0]->Type==INT_CONST)&&((*Arg)[2]->Type==INT_CONST))
		{
			for(int i=0;i<ECount;i++)
				(*(CListFishka*)Rezult)[i]=new CAtomFishka<int>(Tmp1+i*Tmp3);
			return true;
		}
	}
	else
	{
		for(int i=0;i<ECount;i++)
			(*(CListFishka*)Rezult)[i]=new CAtomFishka<int>(Tmp1+i*Tmp3);
		return true;
	};

	for(i=0;i<ECount;i++)
		(*(CListFishka*)Rezult)[i]=new CAtomFishka<float>((Tmp1+Tmp4)+i*(Tmp3+Tmp6));
	return true;

};
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::Less(CListFishka* Arg,CFishka* &Rezult)
{
	if((Arg->Type!=DATA_LIST))
		if(Arg->ElemCount!=2) 
		{
			Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
			return false;
		}
	if(((*Arg)[0]->Type==CHAR_CONST)&&
		((*Arg)[1]->Type==CHAR_CONST))
	{
		Rezult= new CAtomFishka<bool>(CharV((*Arg)[0])<CharV((*Arg)[1]));
		return true;
	}
	float Tmp1=0;
	float Tmp2=0;
	if((*Arg)[0]->Type==INT_CONST) Tmp1=IntV((*Arg)[0]);
	else Tmp1=FloatV((*Arg)[0]);
	if((*Arg)[1]->Type==INT_CONST) Tmp2=IntV((*Arg)[1]);
	else Tmp2=FloatV((*Arg)[1]);
	Rezult= new CAtomFishka<bool>(Tmp1<Tmp2);
	return true;
};
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::Great(CListFishka* Arg,CFishka* &Rezult)
{
	if((Arg->Type!=DATA_LIST))
		if(Arg->ElemCount!=2) 
		{
			Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
			return false;
		}
	if(((*Arg)[0]->Type==CHAR_CONST)&&
		((*Arg)[1]->Type==CHAR_CONST))
	{
		Rezult= new CAtomFishka<bool>(CharV((*Arg)[0])>CharV((*Arg)[1]));
		return true;
	}
	float Tmp1=0;
	float Tmp2=0;
	if((*Arg)[0]->Type==INT_CONST) Tmp1=IntV((*Arg)[0]);
	else Tmp1=FloatV((*Arg)[0]);
	if((*Arg)[1]->Type==INT_CONST) Tmp2=IntV((*Arg)[1]);
	else Tmp2=FloatV((*Arg)[1]);
	Rezult= new CAtomFishka<bool>(Tmp1>Tmp2);
	return true;
};
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::LessEqu(CListFishka* Arg,CFishka* &Rezult)
{
	if((Arg->Type!=DATA_LIST))
		if(Arg->ElemCount!=2) 
		{
			Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
			return false;
		}
	if(((*Arg)[0]->Type==CHAR_CONST)&&
		((*Arg)[1]->Type==CHAR_CONST))
	{
		Rezult= new CAtomFishka<bool>(CharV((*Arg)[0])<=CharV((*Arg)[1]));
		return true;
	}
	float Tmp1=0;
	float Tmp2=0;
	if((*Arg)[0]->Type==INT_CONST) Tmp1=IntV((*Arg)[0]);
	else Tmp1=FloatV((*Arg)[0]);
	if((*Arg)[1]->Type==INT_CONST) Tmp2=IntV((*Arg)[1]);
	else Tmp2=FloatV((*Arg)[1]);
	Rezult= new CAtomFishka<bool>(Tmp1<=Tmp2);
	return true;
};
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::GreatEqu(CListFishka* Arg,CFishka* &Rezult)
{
	if((Arg->Type!=DATA_LIST))
		if(Arg->ElemCount!=2) 
		{
			Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
			return false;
		}
	if(((*Arg)[0]->Type==CHAR_CONST)&&
		((*Arg)[1]->Type==CHAR_CONST))
	{
		Rezult= new CAtomFishka<bool>(CharV((*Arg)[0])>=CharV((*Arg)[1]));
		return true;
	}
	float Tmp1=0;
	float Tmp2=0;
	if((*Arg)[0]->Type==INT_CONST) Tmp1=IntV((*Arg)[0]);
	else Tmp1=FloatV((*Arg)[0]);
	if((*Arg)[1]->Type==INT_CONST) Tmp2=IntV((*Arg)[1]);
	else Tmp2=FloatV((*Arg)[1]);
	Rezult= new CAtomFishka<bool>(Tmp1>=Tmp2);
	return true;
};
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::Equal(CListFishka* Arg,CFishka* &Rezult)
{
	if((Arg->Type!=DATA_LIST))
		if(Arg->ElemCount!=2) 
		{
			Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
			return false;
		}
	if(((*Arg)[0]->Type==CHAR_CONST)&&
		((*Arg)[1]->Type==CHAR_CONST))
	{
		Rezult= new CAtomFishka<bool>(CharV((*Arg)[0])==CharV((*Arg)[1]));
		return true;
	}
	float Tmp1=0;
	float Tmp2=0;
	if((*Arg)[0]->Type==INT_CONST) Tmp1=IntV((*Arg)[0]);
	else Tmp1=FloatV((*Arg)[0]);
	if((*Arg)[1]->Type==INT_CONST) Tmp2=IntV((*Arg)[1]);
	else Tmp2=FloatV((*Arg)[1]);
	Rezult= new CAtomFishka<bool>(Tmp1==Tmp2);
	return true;
};
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::NotEqual(CListFishka* Arg,CFishka* &Rezult)
{
	if((Arg->Type!=DATA_LIST))
		if(Arg->ElemCount!=2) 
		{
			Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
			return false;
		}
	if(((*Arg)[0]->Type==CHAR_CONST)&&
		((*Arg)[1]->Type==CHAR_CONST))
	{
		Rezult= new CAtomFishka<bool>(CharV((*Arg)[0])!=CharV((*Arg)[1]));
		return true;
	}
	float Tmp1=0;
	float Tmp2=0;
	if((*Arg)[0]->Type==INT_CONST) Tmp1=IntV((*Arg)[0]);
	else Tmp1=FloatV((*Arg)[0]);
	if((*Arg)[1]->Type==INT_CONST) Tmp2=IntV((*Arg)[1]);
	else Tmp2=FloatV((*Arg)[1]);
	Rezult= new CAtomFishka<bool>(Tmp1!=Tmp2);
	return true;
};
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::Trans(CListFishka* Arg,CFishka* &Rezult)
{
	int i,j;
	if((Arg->Type!=DATA_LIST)&&
		(Arg->Type!=PAR_LIST)&&
		(Arg->Type!=DELAY_LIST)) 
	{
		Rezult = GetCopyFishka(Arg);
		return true;
	}
	for(i=0;i<Arg->ElemCount;i++)
		if(((*Arg)[i]->Type!=DATA_LIST)&&((*Arg)[i]->Type!=DATA_LIST))
		{
			Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
			return false;
		}

	i=0;
	int EquCount=0;
	while((i<(Arg->ElemCount-1))&&		
		((((*Arg)[i])->Type==DATA_LIST)||(((*Arg)[i])->Type==PAR_LIST))&&
		((((*Arg)[i+1])->Type==DATA_LIST)||(((*Arg)[i+1])->Type==PAR_LIST))&&
		(((CListFishka*)(*Arg)[i])->ElemCount==((CListFishka*)(*Arg)[++i])->ElemCount)) EquCount++;
	EquCount++;
	// -----------------------------------
	if(EquCount==Arg->ElemCount)
	{
		Rezult = new CListFishka(DATA_LIST,((CListFishka*)(*Arg)[0])->ElemCount);
		for(i=0;i<((CListFishka*)(*Arg)[0])->ElemCount;i++)
			(*(CListFishka*)Rezult)[i]=new CListFishka(DATA_LIST,Arg->ElemCount);
		for(j=0;j<Arg->ElemCount;j++)
			for(i=0;i<((CListFishka*)(*Arg)[0])->ElemCount;i++)
				(*(CListFishka*)((*(CListFishka*)Rezult)[i]))[j]=
					GetCopyFishka((CListFishka*)((CListFishka*)(*(CListFishka*)(*Arg)[j])[i]));
		return true;
	}
	//------------------------------------
	int MaxLen=1;
	i=0;
	while(i<Arg->ElemCount)
	{
		if(((((*Arg)[i])->Type==DATA_LIST)||(((*Arg)[i])->Type==PAR_LIST))&&
			(((CListFishka*)((*Arg)[i]))->ElemCount>MaxLen)) 
			MaxLen=((CListFishka*)((*Arg)[i]))->ElemCount;
		i++;
	}
	Rezult = new CListFishka(Arg->Type,MaxLen);
	for(j=0;j<MaxLen;j++)
	{
		int Len=0;
		for(i=0;i<Arg->ElemCount;i++)
			if(j<((CListFishka*)(*Arg)[i])->ElemCount) Len++;
		
		(*(CListFishka*)(Rezult))[j]=new CListFishka(DATA_LIST,Len);
		int IndX=0;
		for(i=0;i<Arg->ElemCount;i++)
		{
			if(j<((CListFishka*)(*Arg)[i])->ElemCount)
			{
				(*(CListFishka*)(*(CListFishka*)(Rezult))[j])[IndX]=GetCopyFishka((*(CListFishka*)((*Arg)[i]))[j]);
				IndX++;
			}
		}

	}
	return true;
}
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::Qwestion(CListFishka* Arg,CFishka* &Rezult)
{
	if((Arg->Type!=DATA_LIST)&&(Arg->Type!=PAR_LIST))
	{
		Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
		return false;
	}

	int i=0;
	while((i<Arg->ElemCount)&&((*Arg)[i]->Type==BOOL_CONST)) i++;

	if(i<Arg->ElemCount)
	{
		Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
		return false;
	}
	int TCount=0;
	for(i=0;i<Arg->ElemCount;i++)
		if(BoolV((*Arg)[i])) TCount++;

	if(TCount==0) 
	{
/*		CListFishka* TmpRes=new CListFishka(PAR_LIST,1);
		(*TmpRes)[0]=new CAtomFishka<ELexTypeEnum>(OPDOT);*/
//		Rezult=TmpRes;
		Rezult=new CAtomFishka<int>(0);
		return true;
	}
	if(TCount==1)
	{
		for(int i=0;i<Arg->ElemCount;i++)
			if(BoolV((*Arg)[i]))
			{
				CListFishka* TmpRes=new CListFishka(PAR_LIST,1);
				(*TmpRes)[0]=new CAtomFishka<int>(i+1);
				Rezult=TmpRes;
				return true;
			}
	}
	Rezult=new CListFishka(PAR_LIST,TCount);
	int j=0;
	for(i=0;i<Arg->ElemCount;i++)
	{
		if(BoolV((*Arg)[i])) 
			(*(CListFishka*)Rezult)[j++]=new CAtomFishka<int>(i+1);
	}
	return true;
}
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::IntSelector(CListFishka* Arg,CFishka* Oper,CFishka* &Rezult)
{
	if(Arg->Type==DATA_LIST)
	{
		if(IntV(Oper)==0)
		{
			Rezult=new CAtomFishka<ELexTypeEnum>(OPDOT);
			return true;
		};

		if(((CListFishka*)Arg)->ElemCount<abs(IntV(Oper)))
		{
			Rezult=new CAtomFishka<ERuntimeErrorEnum>(BOUND_ERROR);
			return false;
		}

		if(IntV(Oper)>0) Rezult=(*(CListFishka*)Arg)[IntV(Oper)-1];
		else
		{
			if(((CListFishka*)Arg)->ElemCount==1)
			{
				CListFishka* TmpRes=new CListFishka(DATA_LIST,1);
				(*TmpRes)[0]=new CAtomFishka<ELexTypeEnum>(OPDOT);
				Rezult=TmpRes;
				return true;
			}

			Rezult=new CListFishka(Arg->Type,((CListFishka*)Arg)->ElemCount-1);
			int j=0;
			int Ex=-IntV(Oper)-1;
			for(int i=0;i<((CListFishka*)Arg)->ElemCount;i++)
				if(i!=Ex) 
				{
					(*(CListFishka*)Rezult)[j]=(*(CListFishka*)Arg)[i];
					j++;
				}
		}
		return true;
	}

	Rezult=new CAtomFishka<ERuntimeErrorEnum>(BOUND_ERROR);
	return false;
}
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::BoolSelector(CListFishka* Arg,CFishka* Oper,CFishka* &Rezult)
{
	if(BoolV(Oper)==true)
	{
		Rezult=GetCopyFishka(Arg);
		return true;
	}

	Rezult=new CAtomFishka<ELexTypeEnum>(OPDOT);
	return true;
};
//---------------------------------------------------------------
//---------------------------------------------------------------
bool CInterpretator::CastScalarType(CFishka* Arg,CFishka* Oper,CFishka* &Rezult)
{
	if(Oper->Type==KWINT)
	{
		switch(Arg->Type)
		{
		case INT_CONST :  Rezult=new CAtomFishka<int>(IntV(Arg)); return true;
		case REAL_CONST : 
			{
				float fArg=FloatV(Arg);
				int iArg=(int)fArg;
				if(fabs(fArg-iArg)>=0.5f)
					if(iArg>0) iArg++; else iArg--;
				Rezult=new CAtomFishka<int>(iArg);
				return true;
			}
		case CHAR_CONST : Rezult=new CAtomFishka<int>((int)CharV(Arg)); return true;
		case BOOL_CONST : Rezult=new CAtomFishka<int>((int)BoolV(Arg)); return true;
		default: break;
		}
	}
	
	if(Oper->Type==KWFLOAT)
	{
		switch(Arg->Type)
		{
		case INT_CONST : Rezult=new CAtomFishka<float>((float)IntV(Arg)); return true;
		case REAL_CONST : Rezult=new CAtomFishka<float>(FloatV(Arg)); return true;
		case CHAR_CONST : Rezult=new CAtomFishka<float>((float)CharV(Arg)); return true;
		case BOOL_CONST : Rezult=new CAtomFishka<float>((float)BoolV(Arg)); return true;
		default: break;
		}
	}

	if(Oper->Type==KWBOOL)
	{
		switch(Arg->Type)
		{
		case INT_CONST : Rezult=new CAtomFishka<bool>((bool)IntV(Arg)); return true;
		case REAL_CONST : Rezult=new CAtomFishka<bool>((bool)FloatV(Arg)); return true;
		case CHAR_CONST : Rezult=new CAtomFishka<bool>((bool)CharV(Arg)); return true;
		case BOOL_CONST : Rezult=new CAtomFishka<bool>(BoolV(Arg)); return true;
		default: break;
		}
	}

	if(Oper->Type==KWCHAR)
	{
		switch(Arg->Type)
		{
		case INT_CONST : Rezult=new CAtomFishka<char>((char)IntV(Arg)); return true;
		case REAL_CONST : Rezult=new CAtomFishka<char>((char)FloatV(Arg)); return true;
		case CHAR_CONST : Rezult=new CAtomFishka<char>(CharV(Arg)); return true;
		case BOOL_CONST : Rezult=new CAtomFishka<char>((char)BoolV(Arg)); return true;
		default: break;
		}
	}
	
	//if(Oper->Type==KWDATALIST)

	Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
	return false;
};

bool CInterpretator::Type(CFishka* Arg,CFishka* &Rezult)
{
	switch(Arg->Type)
	{
	case INT_CONST: {Rezult=new CAtomFishka<ELexTypeEnum>(KWINT); return true;}
	case CHAR_CONST: {Rezult=new CAtomFishka<ELexTypeEnum>(KWCHAR); return true;}
	case REAL_CONST: {Rezult=new CAtomFishka<ELexTypeEnum>(KWFLOAT); return true;}
	case BOOL_CONST: {Rezult=new CAtomFishka<ELexTypeEnum>(KWBOOL); return true;}
	case DATA_LIST: {Rezult=new CAtomFishka<ELexTypeEnum>(KWDATALIST); return true;}
	case SIGNAL_CONST: {Rezult=new CAtomFishka<ELexTypeEnum>(KWSIGNAL); return true;}
	case WRAP_FISHKA: {Rezult=new CObjectFishka(((CWrapFishka*)Arg)->Wrap); return true;}
	case TYPEDEF_FISHKA: {Rezult=new CAtomFishka<ELexTypeEnum>(KWTYPE); return true;}
	default: break;
	}
	Rezult=new CAtomFishka<ELexTypeEnum>(KWNIL);
	return true;
};

bool CInterpretator::Value(CFishka* Arg,CFishka* &Rezult)
{
	if(Arg->Type==WRAP_FISHKA)
	{
		Rezult=GetCopyFishka(((CWrapFishka*)Arg)->Val);
	}
	else Rezult=GetCopyFishka(Arg);
	return true;
}

bool CInterpretator::In(CFishka* Arg,CFishka* &Rezult,bool& Trace)
{
	if((Arg->Type!=DATA_LIST))
		if(((CListFishka*)Arg)->ElemCount!=2) 
		{
			Rezult=new CAtomFishka<ERuntimeErrorEnum>(INTERPR_ERROR);
			return false;
		}
	
	CFishka* lArg=(*((CListFishka*)Arg))[0];
	CObjectFishka* lOper=(CObjectFishka*)((*((CListFishka*)Arg))[1]);

	CFishka* TCP;
	lOper->GetObject()->GetFishka(lArg,TCP,Trace,this);
	if(TCP->Type!=BOOL_CONST) return false;

	Rezult=GetCopyFishka(TCP);
	return true;
};

bool CInterpretator::CastUserType(CFishka* Arg,CFishka* Oper,CFishka* &Rezult,bool &Trace)
{
	if(Arg->Type==WRAP_FISHKA)
	{
		if(((CWrapFishka*)Arg)->Wrap->GetObject()->SelfName==
			Oper->GetObject()->SelfName)
		{
			Rezult=GetCopyFishka(Arg);
			return true;
		}
	}
	CFishka* TCP;
	Oper->GetObject()->GetFishka(Arg,TCP,Trace,this);
	if(TCP->Type!=BOOL_CONST) 
	{
		Rezult=new CAtomFishka<ERuntimeErrorEnum>(TYPE_ERROR);
		return true;
	}//return false;

	if(!((CAtomFishka<bool>*)TCP)->Val)
		Rezult=new CAtomFishka<ERuntimeErrorEnum>(TYPE_ERROR);
	else Rezult=new CWrapFishka((CObjectFishka*)Oper,Arg);

//	Rezult=new CAtomFishka<int>(77);
	return true;
};


