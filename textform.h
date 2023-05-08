#ifndef TEXTFORM_H
#define TEXTFORM_H

#include <QWidget>
#include <QFileDialog>
#include "Highlighter.h"


namespace Ui {
class TextForm;
}

class TextForm : public QWidget
{
    Q_OBJECT

public:
    explicit TextForm(QWidget *parent = nullptr);
    QString GetCurrentText();
    void ClearTextForm();
    void SetText(QString text);
    void CutMove();
    void CopyMove();
    void PasteMove();
    Ui::TextForm *ui;
    bool wasSavedAs = false;
    QString currentFile = "";
    ~TextForm();
private:
    QString lastSavedText = "";
    Highlighter* highlighter;
};

#endif // TEXTFORM_H
