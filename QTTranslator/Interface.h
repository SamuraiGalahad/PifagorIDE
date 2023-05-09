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

extern "C" void* oiTranslate(char* FName, char* Err,int& rez);
extern "C" void  oiRetFuncList(void* curMod, char* fList, int& stringNum);
extern "C" void* oiArgTranslate(void* curMod, char* arg, char* Err, bool& rez);
extern "C" char* oiInterp(void* inFishka, void *curMod, char *name, char* Err, bool& rez, bool trace);
extern "C" void  oiMakeStep();
extern "C" char* oiGetCurResult();
extern "C" void  oiBreakWork();
extern "C" void  oiSetDataBuffer(const char* cStrValue);

#endif
