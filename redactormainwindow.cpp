#include "redactormainwindow.h"
#include "ui_redactormainwindow.h"
#include "textform.h"

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
    RedactorMainWindow *other = new RedactorMainWindow;
    other->move(x() + 40, y() + 40);
    other->show();
}


void RedactorMainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}

