#ifndef __INTERFACE_H_HEADER_INCLUDED__
#define __INTERFACE_H_HEADER_INCLUDED__

#include "Exeption.h"
#include "Program.h"
#include "Function.h"
#include "Fishka.h"
#include "Interpretator.h"
#include "Translator.h"
#include "ArgTranslator.h"
#include "ArgScaner.h"

extern void* oiTranslate(char* FName, char* Err,int& rez);
extern void  oiRetFuncList(void* curMod, char* fList, int& stringNum);
extern void* oiArgTranslate(void* curMod, char* arg, char* Err, bool& rez);
extern char* oiInterp(void* inFishka, void *curMod, char *name, char* Err, bool& rez, bool trace);
extern void  oiMakeStep();
extern char* oiGetCurResult();
extern void  oiBreakWork();
extern void  oiSetDataBuffer(const char* cStrValue);

#endif
