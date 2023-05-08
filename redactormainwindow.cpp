#include "redactormainwindow.h"
#include "ui_redactormainwindow.h"
#include "textform.h"
#include "ui_textform.h"

extern "C" void* oiTranslate(char* FName, char* Err,int& rez);
extern "C" void  oiRetFuncList(void* curMod, char* fList, int& stringNum);
extern "C" void* oiArgTranslate(void* curMod, char* arg, char* Err, bool& rez);
extern "C" char* oiInterp(void* inFishka, void *curMod, char *name, char* Err, bool& rez, bool trace);
extern "C" void  oiMakeStep();
extern "C" char* oiGetCurResult();
extern "C" void  oiBreakWork();
extern "C" void  oiSetDataBuffer(const char* cStrValue);

RedactorMainWindow::RedactorMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RedactorMainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->addTab(new TextForm(), "main.pyf");
    QString sPath = "/Users/nikolajparahin/Documents";
    dirmodel = new QFileSystemModel(this);
    dirmodel->setReadOnly(false);
    ui->treeView->setModel(dirmodel);
    ui->treeView->setRootIndex(dirmodel->setRootPath(sPath));
    for (int i = 1; i < dirmodel->columnCount(); ++i)
        ui->treeView->hideColumn(i);
}

RedactorMainWindow::~RedactorMainWindow()
{
    delete ui;
}

void RedactorMainWindow::on_actionNew_triggered()
{
    ui->tabWidget->addTab(new TextForm(), "main.pyf");
}

void RedactorMainWindow::on_tabWidget_tabCloseRequested(int index)
{
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

void RedactorMainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open the file");
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
    RedactorMainWindow *other = new RedactorMainWindow;
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

void RedactorMainWindow::maybeSave() {
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
        }

    }
}

