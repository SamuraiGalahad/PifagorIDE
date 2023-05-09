#include "mythread.h"

void MyThread::Set(InterpFunction _interpretate, void* _inFishka, void *_curMod, char *_name, char* _Err, bool& _rez, bool& _trace) {
    interpretate = _interpretate;
    inFishka = _inFishka;
    curMod = _curMod;
    name = _name;
    Err = _Err;
    rez = _rez;
    trace = _trace;
}


void MyThread::run()
{
    output = interpretate(inFishka, curMod, name, Err, rez, trace);
    return;
}
