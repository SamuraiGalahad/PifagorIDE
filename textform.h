#ifndef TEXTFORM_H
#define TEXTFORM_H

#include <QWidget>
#include <QFileDialog>
#include "Highlighter.h"


namespace Ui {
class TextForm;
}

/**
 * @brief Класс для текстового поля.
 *
 * Включает в себя методы для
 * связи текстового поля с редактором, а также для работы
 * с буфером обмена и файловой системой.
*/
class TextForm : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор.
    */
    explicit TextForm(QWidget *parent = nullptr);
    /**
     * @brief Метод для передачи текста в методы редактора.
    */
    QString GetCurrentText();
    /**
     * @brief Очищение текстового окна.
    */
    void ClearTextForm();
    /**
     * @brief Установка в текстовое поле текста из передаваемого параметра.
     *
     * @param text передаваемый текст для вставки в поле.
    */
    void SetText(QString text);
    /**
     * @brief CutMove
     */
    void CutMove();
    /**
     * @brief CopyMove
     */
    void CopyMove();
    /**
     * @brief PasteMove
     */
    void PasteMove();
    /**
     * @brief ui
     */
    Ui::TextForm *ui;
    /**
     * @brief wasSavedAs
     */
    // переменная для обработки состояния сохранения документа
    bool wasSavedAs = false;
    // переменная для обработки состояния трансляции
    bool isTranslated = false;
    // полный путь до документа
    QString currentFile = "";
    ~TextForm();
private:
    // последний сохраненный текст
    QString lastSavedText = "";
    // переменная с правилами для подсветки синтаксиса
    Highlighter* highlighter;
};

#endif // TEXTFORM_H
