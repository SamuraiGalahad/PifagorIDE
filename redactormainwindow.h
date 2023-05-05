#ifndef REDACTORMAINWINDOW_H
#define REDACTORMAINWINDOW_H

#include <QMainWindow>
#include "QFileSystemModel"

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

private:
    Ui::RedactorMainWindow *ui;
    QFileSystemModel *dirmodel;
    QFileSystemModel *filemodel;
};

#endif // REDACTORMAINWINDOW_H
