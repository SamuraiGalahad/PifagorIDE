#include "redactormainwindow.h"
#include "ui_redactormainwindow.h"
#include "textform.h"
#include "ui_textform.h"
#include "mythread.h"
#include "QTTranslator/Interface.h"
#include "mainwindow.h"


extern void* oiTranslate(char* FName, char* Err,int& rez);
extern void  oiRetFuncList(void* curMod, char* fList, int& stringNum);
extern void* oiArgTranslate(void* curMod, char* arg, char* Err, bool& rez);
extern char* oiInterp(void* inFishka, void *curMod, char *name, char* Err, bool& rez, bool trace);
extern void  oiMakeStep();
extern char* oiGetCurResult();
extern void  oiBreakWork();
extern void  oiSetDataBuffer(const char* cStrValue);

RedactorMainWindow::RedactorMainWindow(QWidget *ap, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::RedactorMainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->addTab(new TextForm(), "noname.pfg");
    QString sPath = "/Users";
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

    linkLib();
    this->par = ap;
    ((MainWindow*)par)->all_windows.push_back(this);
    initWorkTabWidget();
    runThread = new MyThread;

    rez = true;
    Err=new char[1000];
    output=new char[1000000];
    connect(runThread, SIGNAL(finished()), this, SLOT(ThreadFinshed()));
}

RedactorMainWindow::~RedactorMainWindow()
{
    delete ui;
}

//void RedactorMainWindow::connectLib()
//{
//    TransLib.setFileName("../build-trans-Desktop-Debug/PifagorTrans");
//    TransLib.load();
//    translate = (TransFunction)TransLib.resolve("oiTranslate");
//    if(!translate)
//    {
//        QMessageBox::warning(this, tr("Error!"),
//                             tr("Can't load Translate function!"));
//    }
//    retFuncList = (RetFuncListFunction)TransLib.resolve("oiRetFuncList");
//    if(!retFuncList)
//    {
//        QMessageBox::warning(this, tr("Error!"),
//                             tr("Can't load RetFunctionList function!"));
//    }
//    argTranslate = (ArgTransFunction)TransLib.resolve("oiArgTranslate");
//    if(!argTranslate)
//    {
//        QMessageBox::warning(this, tr("Error!"),
//                             tr("Can't load ArgumentTranslate function!"));
//    }
//    interpretate = (InterpFunction)TransLib.resolve("oiInterp");
//    if(!interpretate)
//    {
//        QMessageBox::warning(this, tr("Error!"),
//                             tr("Can't load Interpretate function!"));
//    }
//    makeStep = (StepFunction)TransLib.resolve("oiMakeStep");
//    if(!makeStep)
//    {
//        QMessageBox::warning(this, tr("Error!"),
//                             tr("Can't load MakeStep function!"));
//    }
//    // Подключение функии прерывания текущего рабочего потока(?)
//    breakWork = (StepFunction)TransLib.resolve("oiBreakWork");
//    if(!breakWork)
//    {
//        QMessageBox::warning(this, tr("Error!"),
//                             tr("Can't load BreakWork function!"));
//    }
//    getCurResult = (GetResultFunction)TransLib.resolve("oiGetCurResult");
//    if(!getCurResult)
//    {
//        QMessageBox::warning(this, tr("Error!"),
//                             tr("Can't load GetResult function!"));
//    }
//    setStrBuffer = (SetDataBufferFunction)TransLib.resolve("oiSetDataBuffer");
//    if(!setStrBuffer)
//    {
//        QMessageBox::warning(this, tr("Error!"),
//                             tr("Can't load SetDataBuffer function!"));
//    }
//}

void RedactorMainWindow::initWorkTabWidget() {
    ui->tabWidget_2->addTab((new QTextEdit()), "Translate");
    ui->tabWidget_2->setCurrentIndex(0);
    ((QTextEdit*)ui->tabWidget_2->currentWidget())->setReadOnly(true);
    ui->tabWidget_2->addTab(new QTextEdit(), "Debug");
    ui->tabWidget_2->setCurrentIndex(1);
    ((QTextEdit*)ui->tabWidget_2->currentWidget())->setReadOnly(true);
    ui->tabWidget_2->addTab(new QTextEdit(), "Arguments");
    ui->tabWidget_2->addTab(new QTextEdit(), "Execute");
    ui->tabWidget_2->setCurrentIndex(3);
    ((QTextEdit*)ui->tabWidget_2->currentWidget())->setReadOnly(true);
    ui->tabWidget_2->setCurrentIndex(0);
}

void RedactorMainWindow::linkLib() {
    translate = oiTranslate;
    retFuncList = oiRetFuncList;
    argTranslate = oiArgTranslate;
    interpretate = oiInterp;
    makeStep = oiMakeStep;
    breakWork = oiBreakWork;
    getCurResult = oiGetCurResult;
    setStrBuffer = oiSetDataBuffer;
}

void RedactorMainWindow::on_actionNew_triggered()
{
    ui->tabWidget->addTab(new TextForm(), "noname.pfg");

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

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file!");
        return;
    }
    createNewTabAndSetToNew(filename);
    TextForm* tf = (TextForm*)ui->tabWidget->currentWidget();
    QTextStream in(&file);
    QString text = in.readAll();
    tf->SetText(text);
    tf->currentFile = filename;
    file.close();
    tf->wasSavedAs = true;
}

void RedactorMainWindow::on_actionOpen_triggered()
{
    QStringList filter;
    filter << "*.pfg";
    QStringList l = dirmodel->rootDirectory().entryList(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    filter += l;
    QString filename = QFileDialog::getOpenFileName(this, "Open the file", "", "*pfg");
    openFile(filename);
}

void RedactorMainWindow::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    this->setWindowTitle(fileName);
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString text = getTextFromCurrentTab();
    out << text;
    QApplication::restoreOverrideCursor();
    file.close();
    TextForm* tf = (TextForm*)ui->tabWidget->currentWidget();
    int current_index = ui->tabWidget->currentIndex();
    ui->tabWidget->setTabText(current_index, fileName);
    tf->wasSavedAs = true;
    tf->ui->plainTextEdit->document()->modificationChanged(false);
    tf->currentFile = fileName;
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
    } else {
        QString current_file = tf->currentFile;
        QFile file(current_file);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            return;
        }
        QString text = getTextFromCurrentTab();
        QTextStream out(&file);
        out << text;
        file.close();
        TextForm* tf = (TextForm*)ui->tabWidget->currentWidget();
        int current_index = ui->tabWidget->currentIndex();
        ui->tabWidget->setTabText(current_index, current_file);
        tf->ui->plainTextEdit->document()->modificationChanged(false);
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

void RedactorMainWindow::fillList()
{
    char* tmp=new char[10000];
    int num;
    QString ret;
    QStringList funcs;

    retFuncList(mModule,tmp,num);
    ret=QString("%1").arg(tmp);
    funcs=ret.split('\n');
    for(int i=0;i<funcs.count();i++)
        ui->comboBox->addItem(funcs[i]);
}

char* RedactorMainWindow::MakeCharMas(QString text)
{
    QChar* QFName = new QChar[1000];
    char *fname = new char[1000];
    QFName = text.data();
    for(int i=0;i<=text.length();i++)
        ///        fname[i]=QFName[i].toAscii();
        fname[i] = QFName[i].toLatin1();
    return fname;
}

void RedactorMainWindow::setTranslated(bool data)
{
    TextForm* tf = (TextForm*)ui->tabWidget->currentWidget();
    tf->isTranslated=data;
}

bool RedactorMainWindow::isInFileSystem() {
    TextForm* tf = (TextForm*)ui->tabWidget->currentWidget();
    QString file_path = tf->currentFile;
    QFileInfo file(file_path);
    return file.exists() && file.isFile();
}

void RedactorMainWindow::Translate()
{
    debug=false;
    char* Err=new char[1000];
    QString rez="";
    int num;

    // transEdit->clear();
    ui->tabWidget_2->setCurrentIndex(0);
    //clearList();
    ui->comboBox->clear();
    TextForm* tf = (TextForm*)ui->tabWidget->currentWidget();
    QString current_file = tf->currentFile;
    if (!isInFileSystem()) {
        this->setWindowTitle(current_file);
    }
    if(!maybeSave())
    {
        rez = "Please, save file before\n";
    }
    else
    {
        if (tf->GetCurrentText().size() == 0) {
            QMessageBox::warning(this,"Now nothing in file!","Now nothing in file!");
            return;
        }
        rez = QString("Translating %1...\n").arg(current_file);
        mModule = translate(MakeCharMas(current_file),Err,num);
        if(num>0)
        {
            rez += QString("Error in line number %1: %2\n").arg(num).arg(Err);
            this->setTranslated(false);
        }
        else
        {
            rez += QString("Translation complete\n%1 lines translated\n").arg(-num);
            this->setTranslated(true);
            fillList();
        }
    }
    ui->tabWidget_2->setCurrentIndex(0);
    ((QTextEdit*)ui->tabWidget_2->currentWidget())->setText(rez);
    // transEdit->setText(rez);
    ui->tabWidget_2->setCurrentIndex(0);
    // workTab->setCurrentWidget(transEdit);
}

void RedactorMainWindow::run(void *inFishka, QString name, bool trace)
{

    //output=interpretate(inFishka, mModule, MakeCharMas(name), Err, rez, trace);

    /// Новый вариант run, использующий только запуск интерпретатора
    /// без обращения к главному окну и его компонентам
    runThread->Set(interpretate, inFishka, mModule, MakeCharMas(name), Err, rez, trace);
    runThread->start();
}

void RedactorMainWindow::DebugOrExecute()
{
    QString curFuncName;
    QString argName;
    bool rez;

    char *Err=new char[1000];

    if(ui->comboBox->currentIndex()==-1)
    {
        QMessageBox::warning(this, tr("Error"), tr("Please, select a function"));
        return;
    }
    // Из таба вытаскивается текущая функция
    curFuncName=ui->comboBox->currentText();

    // Очистка таба результатов. Его подготовка к получению результата
    //rezEdit->clear();
    ui->tabWidget_2->setCurrentIndex(3);
    ((QTextEdit*)ui->tabWidget_2->currentWidget())->setText("");

    // ВЫтаскивается из редактора аргумент
    ui->tabWidget_2->setCurrentIndex(2);
    argName = ((QTextEdit*)ui->tabWidget_2->currentWidget())->toPlainText();

    if(argName.size()>0)
    {
        mInFishka = argTranslate(mModule, MakeCharMas(argName), Err, rez);
        if(!rez)
        {
            QMessageBox::warning(this,tr("Argument interpretating error!"),tr(Err));
            return;
        }
    }
    else
    {
        if(ui->comboBox->currentIndex()==0)
        {
            QMessageBox::warning(this,tr("Argument interpretating error!"),tr("Empty argument string"));
            return;
        }
    }
    /// !!! Debug Info...
    ///qDebug()<< "qDebug(): Succeful argument parsing. Arg = " << argName << " Func = " << curFuncName;

    run(mInFishka, curFuncName, debug);
    return;
}


void RedactorMainWindow::Execute()
{
    debug=false;
    DebugOrExecute();
    ui->tabWidget_2->setCurrentIndex(3);

    ((QTextEdit*)ui->tabWidget_2->currentWidget())->setText(tr(getCurResult()));

    //workTab->setCurrentIndex(3);    // Установка активным таба результатов    // Установка активным таба результатов
}

void RedactorMainWindow::BreakExecute()
{
    isBroken=true;
    oiBreakWork();
}

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

//// Слот, обеспечивающий реакцию на завершение потока
void RedactorMainWindow::ThreadFinshed() {
    output = runThread->GetOutput();
    rez = runThread->GetRez();
    trace = runThread->GetTrace();

    if(rez)
    {
        if(trace)
        {
            //debugTextFromThread=tr(output);
            //SetRez(2);
            ui->tabWidget_2->setCurrentIndex(1);

            ((QTextEdit*)ui->tabWidget_2->currentWidget())->setText(tr(getCurResult()));
            // Фокус смещается на последнюю позицию
            QTextCursor c = ((QTextEdit*)ui->tabWidget_2->currentWidget())->textCursor();
            c.movePosition(QTextCursor::End);
            ((QTextEdit*)ui->tabWidget_2->currentWidget())->setTextCursor(c);
            // Чтобы не начать по новой, используется отвлечение внимания диалоговым окном.
            QMessageBox::information(this,"Debugging","Debugging has been Completed!");
        }
        else
        {
            //execTextFromThread=tr(output);
            //SetRez(3);
            ui->tabWidget_2->setCurrentIndex(2);
            ((QTextEdit*)ui->tabWidget_2->currentWidget())->setText(tr(getCurResult()));
            //workTab->setCurrentWidget(rezEdit);

        }
    }
    else
    {
        if(!(isBroken)) {
            QMessageBox::warning(this,"Execution error!", tr(Err));
        }
        else
        {
            QMessageBox::warning(this,"Execution interrupted!", "Programm execution interrupted by user's request");
        }
    }
    debug = false;
    //QMessageBox::warning(this,"Completed!","Completed!");
    return;
}

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
    try {
        Translate();
    } catch (...)
    {
        QMessageBox warning = QMessageBox(QMessageBox::Warning, "...", "Exception while Translation!");
        warning.exec();
    }
}


void RedactorMainWindow::on_actionExecute_triggered()
{
    try {
        Execute();
    } catch (...)
    {
        QMessageBox warning = QMessageBox(QMessageBox::Warning, "...", "Exception while Execution!");
        warning.exec();
    }

}


void RedactorMainWindow::on_actionBreak_execution_triggered()
{
    try {
        BreakExecute();
    } catch (...)
    {
        QMessageBox warning = QMessageBox(QMessageBox::Warning, "...", "Exception while Breaking Execution Action!");
    }

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
    } else {
        event->ignore();
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

