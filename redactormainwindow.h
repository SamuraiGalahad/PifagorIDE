#ifndef REDACTORMAINWINDOW_H
#define REDACTORMAINWINDOW_H

#include <QMainWindow>
#include "QFileSystemModel"
#include "QFileDialog"
#include "QFile"
#include "QMessageBox"
#include "QLibrary"

class MyThread;


typedef void* (*TransFunction)(char*,char*,int&);
typedef void  (*RetFuncListFunction)(void*,char*,int&);
typedef void* (*ArgTransFunction)(void*,char*,char*,bool&);
typedef char* (*InterpFunction)(void*,void*,char*,char*,bool&,bool);
typedef void  (*StepFunction)();
typedef char* (*GetResultFunction)();
typedef void  (*SetDataBufferFunction)(const char*);

namespace Ui {
class RedactorMainWindow;
}

class RedactorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RedactorMainWindow(QWidget *parent = nullptr);
    ~RedactorMainWindow();

    QString debugTextFromThread;
    QString execTextFromThread;
    void *mInFishka;
    void *mModule;
    InterpFunction interpretate;
    char* MakeCharMas(QString text);
    void SetRez(int num);
    void SetFlagsFalse();
    void setTranslated(bool data);
    bool isBroken;

private slots:
    void on_actionNew_triggered();

    void on_tabWidget_tabCloseRequested(int index);

    void on_actionChange_File_Tree_Visibility_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_as_triggered();

    void on_actionNew_Window_triggered();

    void on_actionSave_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

private:
    Ui::RedactorMainWindow *ui;
    QFileSystemModel *dirmodel;
    QFileSystemModel *filemodel;

    void initWorkTabWidget();

    QString currentFile;
    void createNewTabAndSetToNew(QString name);
    void createNewTabAndSetToNew();
    QString getTextFromCurrentTab();
    bool maybeSave();
    //void linkLib();
    MyThread *runThread;

    bool rez;
    bool trace;
    char *Err;
    char *output;

    bool debug;

    QLibrary TransLib;

    TransFunction		translate;
    RetFuncListFunction retFuncList;
    ArgTransFunction	argTranslate;
    StepFunction		makeStep, breakWork;
    GetResultFunction   getCurResult;
    SetDataBufferFunction setStrBuffer;

    void Translate();
    void Execute();
    void BreakExecute();
    void Debug();
    void BreakDebug();
    void DebugOrExecute();
    void run(void *inFishka, QString name, bool trace);
    void ThreadFinshed();
};

#endif // REDACTORMAINWINDOW_H
