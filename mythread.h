#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
class MainWindow;
typedef char* (*InterpFunction)(void*,void*,char*,char*,bool&,bool);

class MyThread : public QThread
{
    InterpFunction interpretate;
    void* inFishka;
    void* curMod;
    char* name;
    char* Err;
    bool rez;
    bool trace;

    char* output;
public:
    MyThread() {}
    void Set(InterpFunction _interpretate, void* inFishka, void *curMod, char *_name, char* _Err, bool& _rez, bool& _trace);
    char* GetOutput() {return output;}
    bool GetRez() {return rez;}
    bool GetTrace() {return trace;}
    void run();
};

#endif
