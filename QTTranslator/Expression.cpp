///#include <unistd.h>
#include <string.h>
#include <iostream>

#include "Precompiled.h"

#include "Expression.h"
#include "Term.h"
#include "Fishka.h"
#include "Interpretator.h"
#include "Scaner.h"
#include "Exeption.h"

volatile bool WaitForSignal=false;
volatile bool StopThread=false;


CExpression::~CExpression()
{
/*	vector<PTerm>::iterator i;
	for (i = TermList.begin(); i != TermList.end(); i++)
		if(*i) delete (*i);*/
}


void CExpression::AddTerm(CTerm* Term)
{
	TermList.push_back(Term);
}
void CExpression::AddTermBeforeEnd(CTerm* Term)
{
    std::vector<CTerm*>::iterator i=TermList.end();
	TermList.insert(--i,Term);
}

bool CExpression::GetFishka(CFishka* Arg,CFishka* &Rezult,bool &Trace,CInterpretator* Interpretator)
{
	CFishka *TmpFishka1,*TmpFishka2,*TmpFishka3,*ETmpFishka2;
	int Size=TermList.size();
	if(!(TermList[0]->GetFishka(Arg,TmpFishka1,Trace,Interpretator))) 
		return false;
	if(Size<2) 
	{
		Rezult=TmpFishka1;
		return true;
	}
	for(int i=1;i<Size;i++)
	{
		if(StopThread) throw CExeption(NULL,"",19);
			//AfxEndThread(0);
		if(!(TermList[i]->GetFishka(Arg,TmpFishka2,Trace,Interpretator))) 
			return false;
		if(Trace)
		{
            /// Debug Info
            ///std::cout << i << ": StopThread = " << (StopThread?"true":"false")
            ///               << ", WaitForSignal = " << (WaitForSignal?"true":"false")
            ///                << " before"  << std::endl;
            TmpFishka1->OutConsole();
			strcat(data,":");
			TmpFishka2->OutConsole();
			strcat(data," ");
			WaitForSignal=true;
            while(WaitForSignal) {
                if(StopThread) {
					throw CExeption(NULL,"",19);
                }
            }
            /// Debug Info
            ///std::cout << i << ": StopThread = " << (StopThread?"true":"false")
            ///               << ", WaitForSignal = " << (WaitForSignal?"true":"false")
            ///               << " after"  << std::endl;
        }
//		if((TmpFishka2->Type==KWRETURN)||
//			(TmpFishka2->Type==KWBREAK)) 	Owner->SetRezFishka(TmpFishka1);

		if(!Interpretator->InterpretationOperator(Arg,TmpFishka1,TmpFishka2,TmpFishka3,Trace))
		{
			if(Trace)
			{
				strcat(data," -> ");
				TmpFishka3->OutConsole();
				strcat(data,"\n");
			}
			if(TermList[i]->ElseTerm)
			{
				if(TermList[i]->ElseTerm)
				{
					if(Trace)
					{
						strcat(data," else ");
//						TermList[i]->ElseTerm->OutConsole();
						strcat(data,"\n");
						//char Ch;
						//=getch();
						/*if(Ch==27) 
						{
							printf("\n Debug session interrupted by user !\n");
							Trace=false;
						}*/
					};
					if(!(TermList[i]->ElseTerm->GetFishka(Arg,ETmpFishka2,Trace,Interpretator)))
						return false;
					TmpFishka3=ETmpFishka2;
				};
			} 
		}
		if(Trace)
		{
			strcat(data," -> ");
			TmpFishka3->OutConsole();
			strcat(data,"\n");
		}
		if(Owner->IsFunc())
		{
//			switch(TmpFishka2->Type)
//			{
//			case KWRETURN:
//				{
//					Rezult=TmpFishka3;
//					return true;
//				}
//			case KWBREAK:
//				{
//					(*IOBufferPtr)<<"Key word \"break\" in function \""<<Owner->SelfName<<"\""<<"\n";
//					return false;
//				}
//			}
		}
		else
		{
/*			switch(TmpFishka2->Type)
			{
			case KWBREAK:
				{
					Rezult=TmpFishka3;
					return true;
				}
			case KWRETURN:
				{
					(*IOBufferPtr)<<"Key word \"return\" in block !"<<"\n";
					return false;
				}
			}*/
		}
		TmpFishka1=TmpFishka3;
	};
	Rezult=TmpFishka1;
	return true;
};
//---------------------------------------------------------------
//---------------------------------------------------------------

