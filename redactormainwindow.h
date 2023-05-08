#ifndef REDACTORMAINWINDOW_H
#define REDACTORMAINWINDOW_H

#include <QMainWindow>
#include "QFileSystemModel"
#include "QFileDialog"
#include "QFile"
#include "QMessageBox"

namespace Ui {
class RedactorMainWindow;
}

class RedactorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RedactorMainWindow(QWidget *parent = nullptr);
    ~RedactorMainWindow();

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
    QString currentFile;
    void createNewTabAndSetToNew(QString name);
    void createNewTabAndSetToNew();
    QString getTextFromCurrentTab();
    void maybeSave();
};

#endif // REDACTORMAINWINDOW_H
