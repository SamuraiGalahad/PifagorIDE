#include "redactormainwindow.h"
#include "ui_redactormainwindow.h"
#include "textform.h"
#include "ui_textform.h"
#include "mythread.h"
#include "QTTranslator/Interface.h"
#include "mainwindow.h"

extern "C" void* oiTranslate(char* FName, char* Err,int& rez);
extern "C" void  oiRetFuncList(void* curMod, char* fList, int& stringNum);
extern "C" void* oiArgTranslate(void* curMod, char* arg, char* Err, bool& rez);
extern "C" char* oiInterp(void* inFishka, void *curMod, char *name, char* Err, bool& rez, bool trace);
extern "C" void  oiMakeStep();
extern "C" char* oiGetCurResult();
extern "C" void  oiBreakWork();
extern "C" void  oiSetDataBuffer(const char* cStrValue);

RedactorMainWindow::RedactorMainWindow(QWidget *ap, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::RedactorMainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->addTab(new TextForm(), "main.pyf");
    QString sPath = "/Users/nikolajparahin/Documents";
    dirmodel = new QFileSystemModel(this);
    QStringList filters;
    filters << "*.pfg";
    QStringList l = dirmodel->rootDirectory().entryList(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    filters += l;
    dirmodel->setNameFilters(filters);
    dirmodel->setReadOnly(false);
    ui->treeView->setModel(dirmodel);
    ui->treeView->setRootIndex(dirmodel->setRootPath(sPath));
    for (int i = 1; i < dirmodel->columnCount(); ++i)
        ui->treeView->hideColumn(i);

 //   linkLib();
    this->par = ap;
    ((MainWindow*)par)->all_windows.push_back(this);
    initWorkTabWidget();
    runThread = new MyThread;

    rez = true;
    Err=new char[1000];
    output=new char[1000000];
}

RedactorMainWindow::~RedactorMainWindow()
{
    delete ui;
}

void RedactorMainWindow::initWorkTabWidget() {
    ui->tabWidget_2->addTab((new QTextEdit()), "Translate");
    ui->tabWidget_2->setCurrentIndex(0);
    ((QTextEdit*)ui->tabWidget_2->currentWidget())->setReadOnly(true);
    ui->tabWidget_2->addTab(new QTextEdit(), "Debug");
    ui->tabWidget_2->setCurrentIndex(1);
    ((QTextEdit*)ui->tabWidget_2->currentWidget())->setReadOnly(true);
    ui->tabWidget_2->addTab(new QTextEdit(), "Arguments");
    ui->tabWidget_2->addTab(new QTextEdit(), "Excecute");
    ui->tabWidget_2->setCurrentIndex(3);
    ((QTextEdit*)ui->tabWidget_2->currentWidget())->setReadOnly(true);
    ui->tabWidget_2->setCurrentIndex(0);
}

//void RedactorMainWindow::linkLib() {
//    translate = oiTranslate;
//    retFuncList = oiRetFuncList;
//    argTranslate = oiArgTranslate;
//    interpretate = oiInterp;
//    makeStep = oiMakeStep;
//    breakWork = oiBreakWork;
//    getCurResult = oiGetCurResult;
//    setStrBuffer = oiSetDataBuffer;
//}

void RedactorMainWindow::on_actionNew_triggered()
{
    ui->tabWidget->addTab(new TextForm(), "main.pfg");
}

void RedactorMainWindow::on_tabWidget_tabCloseRequested(int index)
{
    if (!maybeSave()) {
        return;
    }
    ui->tabWidget->removeTab(index);

}

void RedactorMainWindow::on_actionChange_File_Tree_Visibility_triggered()
{
    if (ui->treeView->isHidden()) {
        ui->treeView->show();
    } else {
        ui->treeView->hide();
    }
}

void RedactorMainWindow::createNewTabAndSetToNew(QString name) {
    int count = ui->tabWidget->count();
    ui->tabWidget->addTab(new TextForm(), name);
    ui->tabWidget->setCurrentIndex(count);
}

QString RedactorMainWindow::getTextFromCurrentTab() {
    TextForm* tf = (TextForm*)ui->tabWidget->currentWidget();
    return tf->GetCurrentText();
}

void RedactorMainWindow::openFile(QString filename) {
    QFile file(filename);
    currentFile = filename;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file!");
    }
    createNewTabAndSetToNew(filename);
    TextForm* tf = (TextForm*)ui->tabWidget->currentWidget();
    QTextStream in(&file);
    QString text = in.readAll();
    tf->SetText(text);
    file.close();
}

void RedactorMainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open the file");
    openFile(filename);
}

void RedactorMainWindow::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    currentFile = fileName;
    QTextStream out(&file);
    QString text = getTextFromCurrentTab();
    out << text;
    file.close();
    TextForm* tf = (TextForm*)ui->tabWidget->currentWidget();
    int current_index = ui->tabWidget->currentIndex();
    ui->tabWidget->setTabText(current_index, fileName);
    tf->wasSavedAs = true;
}

void RedactorMainWindow::on_actionNew_Window_triggered()
{
    RedactorMainWindow *other = new RedactorMainWindow(par);
    other->move(x() + 40, y() + 40);
    other->show();
}

void RedactorMainWindow::on_actionSave_triggered()
{
    TextForm* tf = (TextForm*)ui->tabWidget->currentWidget();
    if (tf->wasSavedAs == false) {
        on_actionSave_as_triggered();
        tf->wasSavedAs = true;
    } else {

    }
}

void RedactorMainWindow::on_actionCut_triggered()
{
    TextForm* tf = (TextForm*)ui->tabWidget->currentWidget();
    tf->CutMove();
}

void RedactorMainWindow::on_actionCopy_triggered()
{
    TextForm* tf = (TextForm*)ui->tabWidget->currentWidget();
    tf->CopyMove();
}

void RedactorMainWindow::on_actionPaste_triggered()
{
    TextForm* tf = (TextForm*)ui->tabWidget->currentWidget();
    tf->PasteMove();
}

bool RedactorMainWindow::maybeSave() {
    TextForm* tf = (TextForm*)ui->tabWidget->currentWidget();
    if (tf->ui->plainTextEdit->document()->isModified()) {
        QMessageBox::StandardButton r;
        r = QMessageBox::warning(this, tr("Pifagor IDE"),
                                 "The document has been modified. \n Do you want to save your changes?",
                                 QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (r == QMessageBox::Save) {
            if (tf->wasSavedAs) {
                on_actionSave_triggered();
            } else {
                on_actionSave_as_triggered();
            }
            return true;
        } else if (r == QMessageBox::Discard) {
            return true;
        } else {
            return false;
        }
    }
    return true;
}


// Транслятор

//void RedactorMainWindow::Translate()
//{
//    debug=false;
//    char* Err=new char[1000];
//    QString rez="";
//    int num;

//    transEdit->clear();

//    clearList();

//    if(!this->save())
//    {
//        rez = "Please, save file before\n";
//    }
//    else
//    {
//        rez = QString("Translating %1...\n").arg((this->curFile));
//        mModule = translate(MakeCharMas(this->curFile),Err,num);
//        if(num>0)
//        {
//            rez += QString("Error in line number %1: %2\n").arg(num).arg(Err);
//            this->setTranslated(false);
//        }
//        else
//        {
//            rez += QString("Translation complete\n%1 lines translated\n").arg(-num);
//            this->setTranslated(true);
//            fillList();
//        }
//    }
//    transEdit->setText(rez);
//    workTab->setCurrentWidget(transEdit);
//}



//void RedactorMainWindow::Execute()
//{
//    debug=false;
//    DebugOrExecute();
//    rezEdit->setText(tr(getCurResult()));
//    //workTab->setCurrentIndex(3);    // Установка активным таба результатов
//    workTab->setCurrentWidget(rezEdit);    // Установка активным таба результатов
//}

//void RedactorMainWindow::BreakExecute()
//{
//    isBroken=true;
//    breakWork();
//}

//void RedactorMainWindow::Debug()
//{
//    workTab->setCurrentWidget(debugEdit);    // Установка активным отладочного таба
//    /// Debug Info
//    ///qDebug() << "Debug(): debug = " << (debug? "true":"false");

//    if(debug)   // Отладка уже идет. Продолжаем...
//    {
//        makeStep();
//        /// Debug Info
//        ///qDebug() << "Debug(): WaitForSignal = false";
//    }
//    else    //if(!debug)    // Отладка только началась. Запускаем интерпретатор...
//    {
//        // Вытаскиваются аргумент и текущая функция
//        ///QString tmpStr = argEdit->toPlainText() + " : " + funcList->currentItem()->text();
//        ///const char* tmpStrBuf = tmpStr.toLocal8Bit().data();
//        /// Debug Info
//        ///qDebug() << "Debug(): tmpStr = " << tmpStr;
//        //debugEdit->setText(tmpStr);
//        ///setStrBuffer(tmpStrBuf);
//        // Очистка таба результатов. Его подготовка к получению результата
//        rezEdit->clear();

//        debug=true;
//        // Запуск интерпретатора
//        DebugOrExecute();
//    }
//    debugEdit->setText(tr(getCurResult())); // Выдается в отладочное окно результат очередного шага.
//    // Фокус смещается на последнюю позицию
//    QTextCursor c = debugEdit->textCursor();
//    c.movePosition(QTextCursor::End);
//    debugEdit->setTextCursor(c);
//}

//void RedactorMainWindow::BreakDebug()
//{
//    isBroken=true;
//    breakWork();
//    // Очистка отладочного окна
//    debugEdit->clear();
//}

//void RedactorMainWindow::DebugOrExecute()
//{
//    QString curFuncName;
//    QString argName;
//    bool rez;

//    char *Err=new char[1000];

//    if(funcList->currentRow()==-1)
//    {
//        QMessageBox::warning(this, tr("Error"), tr("Please, select a function"));
//        return;
//    }
//    // Из таба вытаскивается текущая функция
//    curFuncName=funcList->currentItem()->text();

//    // Очистка таба результатов. Его подготовка к получению результата
//    rezEdit->clear();

//    // ВЫтаскивается из редактора аргумент
//    argName = argEdit->toPlainText();

//    if(argName.count()>0)
//    {
//        mInFishka = argTranslate(mModule, MakeCharMas(argName), Err, rez);
//        if(!rez)
//        {
//            QMessageBox::warning(this,tr("Argument interpretating error!"),tr(Err));
//            return;
//        }
//    }
//    else
//    {
//        if(funcList->currentRow()==0)
//        {
//            QMessageBox::warning(this,tr("Argument interpretating error!"),tr("Empty argument string"));
//            return;
//        }
//    }
//    /// !!! Debug Info...
//    ///qDebug()<< "qDebug(): Succeful argument parsing. Arg = " << argName << " Func = " << curFuncName;

//    run(mInFishka, curFuncName, debug);
//    return;
//}

//void RedactorMainWindow::run(void *inFishka, QString name, bool trace)
//{

//    //output=interpretate(inFishka, mModule, MakeCharMas(name), Err, rez, trace);

//    /// Новый вариант run, использующий только запуск интерпретатора
//    /// без обращения к главному окну и его компонентам
//    runThread->Set(interpretate, inFishka, mModule, MakeCharMas(name), Err, rez, trace);
//    runThread->start();
//}

//// Слот, обеспечивающий реакцию на завершение потока
//void RedactorMainWindow::ThreadFinshed() {
//    output = runThread->GetOutput();
//    rez = runThread->GetRez();
//    trace = runThread->GetTrace();

//    if(rez)
//    {
//        if(trace)
//        {
//            //debugTextFromThread=tr(output);
//            //SetRez(2);
//            debugEdit->setText(tr(getCurResult()));
//            workTab->setCurrentWidget(debugEdit);
//            // Фокус смещается на последнюю позицию
//            QTextCursor c = debugEdit->textCursor();
//            c.movePosition(QTextCursor::End);
//            debugEdit->setTextCursor(c);
//            // Чтобы не начать по новой, используется отвлечение внимания диалоговым окном.
//            QMessageBox::information(this,"Debugging","Debugging has been Completed!");
//        }
//        else
//        {
//            //execTextFromThread=tr(output);
//            //SetRez(3);
//            rezEdit->setText(tr(getCurResult()));
//            workTab->setCurrentWidget(rezEdit);

//        }
//    }
//    else
//    {
//        if(!(isBroken)) {
//            QMessageBox::warning(this,"Execution error!", tr(Err));
//        }
//        else
//        {
//            QMessageBox::warning(this,"Execution interrupted!", "Programm execution interrupted by user's request");
//        }
//    }
//    SetFlagsFalse();
//    //QMessageBox::warning(this,"Completed!","Completed!");
//    return;
//}

void RedactorMainWindow::on_actionClose_triggered()
{
    QMessageBox::StandardButton r;
    r = QMessageBox::warning(this, tr("Pifagor IDE"),
                             "Are you sure you want to exit the app? \n Unsaved data will be lost!",
                             QMessageBox::Close | QMessageBox::Cancel);
    if (r == QMessageBox::Close) {
        std::vector<RedactorMainWindow*> all_w = ((MainWindow*)par)->all_windows;
        close();
        for (auto i : all_w) {
            if (i != nullptr && i != this) {
                return;
            }
        }
        par->close();
    }
}


void RedactorMainWindow::on_actionClose_All_triggered()
{
    QMessageBox::StandardButton r;
    r = QMessageBox::warning(this, tr("Pifagor IDE"),
                             "Are you sure you want to exit the app? \n Unsaved data will be lost!",
                             QMessageBox::Close | QMessageBox::Cancel);
    if (r == QMessageBox::Close) {
        std::vector<RedactorMainWindow*> all_w = ((MainWindow*)par)->all_windows;
        for (auto i : all_w) {
            if (i != nullptr) {
                i->close();
            }
        }
        par->close();
    }
}


void RedactorMainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About Application"),
                       tr("The <b>PifagorIDE</b> -- is a software development environment\n"
                          "in the Pythagoras programming language.\n"
                          "For more information, visit the language developer's website:\n"
                          "http://www.softcraft.ru"));
}


void RedactorMainWindow::on_actionGo_up_triggered()
{
    QDir current_dir = dirmodel->rootDirectory();
    current_dir.cdUp();
    ui->treeView->setRootIndex(dirmodel->setRootPath(current_dir.absolutePath()));
}


void RedactorMainWindow::on_actionChoose_dir_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    dirmodel->rootDirectory().absolutePath(),
                                                    QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    ui->treeView->setRootIndex(dirmodel->setRootPath(dir));
}


void RedactorMainWindow::on_actionTranslate_triggered()
{

}


void RedactorMainWindow::on_actionExecute_triggered()
{

}


void RedactorMainWindow::on_actionBreak_execution_triggered()
{

}


void RedactorMainWindow::on_actionDebug_triggered()
{

}


void RedactorMainWindow::on_actionBreak_Debug_triggered()
{

}


void RedactorMainWindow::on_actionNext_step_triggered()
{

}


void RedactorMainWindow::on_actionCascade_triggered()
{
    std::vector<RedactorMainWindow*> all_w  = ((MainWindow*)par)->all_windows;
    int x = 0, y = 0;
    for (auto i : all_w) {
        if (i != nullptr) {
            i->move(x + 10, y + 20);
        }
        x += 40;
        y += 40;
    }
}

void RedactorMainWindow::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton r;
    r = QMessageBox::warning(this, tr("Pifagor IDE"),
                             "Are you sure you want to exit the app? \n Unsaved data will be lost!",
                             QMessageBox::Close | QMessageBox::Cancel);
    if (r == QMessageBox::Close) {
        std::vector<RedactorMainWindow*> all_w = ((MainWindow*)par)->all_windows;
        close();
        for (auto i : all_w) {
            if (i != nullptr && i != this) {
                return;
            }
        }
        par->close();
    }
}


void RedactorMainWindow::on_treeView_doubleClicked(const QModelIndex &index) {
    QString path = dirmodel->fileInfo(index).absoluteFilePath();
    if (dirmodel->isDir(index)) {
        ui->treeView->setRootIndex(dirmodel->setRootPath(path));
    } else {
        openFile(path);
    }
}

//void RedactorMainWindow::on_treeView_clicked(const QModelIndex &index)
//{
//    QString path = dirmodel->fileInfo(index).absoluteFilePath();
//    ui->treeView->setRootIndex(dirmodel->setRootPath(path));
//}

