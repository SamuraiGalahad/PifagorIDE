#include "textform.h"
#include "ui_textform.h"

TextForm::TextForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextForm)
{
    ui->setupUi(this);
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    ui->plainTextEdit->setFont(font);

    highlighter = new Highlighter(ui->plainTextEdit->document());
}

TextForm::~TextForm()
{
    delete ui;
}

QString TextForm::GetCurrentText() {
    return ui->plainTextEdit->toPlainText();
}

void TextForm::ClearTextForm() {
    ui->plainTextEdit->setPlainText(QString());
}


void TextForm::SetText(QString line) {
    ui->plainTextEdit->setPlainText(line);
}

void TextForm::CutMove() {
    ui->plainTextEdit->cut();
}

void TextForm::CopyMove() {
    ui->plainTextEdit->copy();
}

void TextForm::PasteMove() {
    ui->plainTextEdit->paste();
}
