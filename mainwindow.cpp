#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "redactormainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    redactor = new RedactorMainWindow;
    redactor->show();
    this->hide();
}


void MainWindow::on_pushButton_2_clicked()
{

}

