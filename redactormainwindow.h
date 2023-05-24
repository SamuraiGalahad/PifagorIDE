#ifndef REDACTORMAINWINDOW_H
#define REDACTORMAINWINDOW_H

#include <QMainWindow>
#include "QFileSystemModel"
#include "QFileDialog"
#include "QFile"
#include "QMessageBox"
#include "QLibrary"

class MyThread;


typedef void* (*TransFunction)(char*,char*,int&);
typedef void  (*RetFuncListFunction)(void*,char*,int&);
typedef void* (*ArgTransFunction)(void*,char*,char*,bool&);
typedef char* (*InterpFunction)(void*,void*,char*,char*,bool&,bool);
typedef void  (*StepFunction)();
typedef char* (*GetResultFunction)();
typedef void  (*SetDataBufferFunction)(const char*);

namespace Ui {
class RedactorMainWindow;
}

/**
 * @brief The RedactorMainWindow class -- класс главного окна -- редактора текстового меню.
 */
class RedactorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief RedactorMainWindow -- конструктор
     * @param at cсылка не меню
     * @param parent
     */
    explicit RedactorMainWindow(QWidget* at = nullptr, QWidget *parent = nullptr);
    ~RedactorMainWindow();
    /**
     * @brief debugTextFromThread -- текст из потока Debug
     */
    QString debugTextFromThread;
    /**
     * @brief execTextFromThread -- текст из потока исполнения Execution
     */
    QString execTextFromThread;
    /**
     * @brief mInFishka
     */
    void *mInFishka;
    void *mModule;
    /**
     * @brief interpretate -- интерпретируемая функция
     */
    InterpFunction interpretate;
    /**
     * @brief MakeCharMas -- создание массива символов для передачи в буферы обмена
     * @param text -- текст
     * @return ссылка на массив символов
     */
    char* MakeCharMas(QString text);
    /**
     * @brief SetRez -- метод для установления результата интерпрет.
     * @param num
     */
    void SetRez(int num);
    /**
     * @brief SetFlagsFalse
     */
    void SetFlagsFalse();
    /**
     * @brief setTranslated
     * @param d
     */
    void setTranslated(bool d);
    /**
     * @brief openFile -- открытие конкретного файла по полному пути
     * @param filename
     */
    void openFile(QString filename);
    // флаг поломки, остановки процесса
    bool isBroken;
    // прошлый виджет
    QWidget* par;

protected:
    /**
     * @brief closeEvent -- переопределенный метод закрытия окна редактора.
     *
     * Для реализации ивента закрытия окна с предупреждением о несохранении файлов.
     *
     * @param event
     */
    void closeEvent(QCloseEvent *event) override;

private slots:
    /**
     * @brief on_actionNew_triggered -- слот для создания нового текстового окна
     */
    void on_actionNew_triggered();

    /**
     * @brief on_tabWidget_tabCloseRequested -- слот для закрытия конкретного текстового окна
     * @param index -- индекс для закрытие окна по индексу
     */
    void on_tabWidget_tabCloseRequested(int index);

    /**
     * @brief on_actionChange_File_Tree_Visibility_triggered -- слот для зыкрытия дерева каталогов
     */
    void on_actionChange_File_Tree_Visibility_triggered();

    /**
     * @brief on_actionOpen_triggered -- слот для открытия файлов. Открывает диалоговое окно.
     */
    void on_actionOpen_triggered();

    /**
     * @brief on_actionSave_as_triggered -- слот для Сохранения как. Открывает диалоговое окно.
     */
    void on_actionSave_as_triggered();

    /**
     * @brief on_actionNew_Window_triggered -- слот для создания нового окна редактора.
     */
    void on_actionNew_Window_triggered();

    /**
     * @brief on_actionSave_triggered -- сохрание файла.
     */
    void on_actionSave_triggered();

    /**
     * @brief on_actionCut_triggered -- вырезание в буфер обмена открытого в данный момент файла.
     */
    void on_actionCut_triggered();

    /**
     * @brief on_actionCopy_triggered -- копирование в буфер обмена открытого в данный момент файла.
     */
    void on_actionCopy_triggered();

    /**
     * @brief on_actionPaste_triggered -- вставка из буфера обмена в открытый в данный момент файл.
     */
    void on_actionPaste_triggered();

    /**
     * @brief on_actionClose_triggered -- закрытие приложения с текстовым окном.
     */
    void on_actionClose_triggered();

    /**
     * @brief on_actionClose_All_triggered -- закрытие всех редакторов.
     */
    void on_actionClose_All_triggered();

    /**
     * @brief on_actionAbout_triggered -- открытие информации о приложении.
     */
    void on_actionAbout_triggered();

    /**
     * @brief on_actionGo_up_triggered -- слот для реалиции подъема на один каталог в дереве.
     */
    void on_actionGo_up_triggered();

    /**
     * @brief on_actionChoose_dir_triggered -- слот для реализации открытия конкретного каталога в дереве.
     */
    void on_actionChoose_dir_triggered();

    /**
     * @brief on_actionTranslate_triggered -- слот для передачи действия в метод транслятора.
     */
    void on_actionTranslate_triggered();

    /**
     * @brief on_actionExecute_triggered -- слот для передачи действия в метод интерпретации.
     */
    void on_actionExecute_triggered();

    /**
     * @brief on_actionBreak_execution_triggered -- слот для передачи действия в метод break.
     */
    void on_actionBreak_execution_triggered();

    /**
     * @brief on_actionDebug_triggered -- слот для передачи действия в метод debug.
     */
    void on_actionDebug_triggered();

    /**
     * @brief on_actionBreak_Debug_triggered -- слот для передачи действия в метод break debug.
     */
    void on_actionBreak_Debug_triggered();

    /**
     * @brief on_actionNext_step_triggered -- слот для передачи в метод для следующего шага при отладке.
     */
    void on_actionNext_step_triggered();

    /**
     * @brief on_actionCascade_triggered -- слот для представления всех окон редактора в виде каскада.
     */
    void on_actionCascade_triggered();

    /**
     * @brief on_treeView_doubleClicked -- слот для перехода в следующую по иерархии папку
     * @param index
     */
    void on_treeView_doubleClicked(const QModelIndex &index);

 //   void on_treeView_clicked(const QModelIndex &index);

private:
    // файл ui редактора окна
    Ui::RedactorMainWindow *ui;
    // файловая модель 1
    QFileSystemModel *dirmodel;
    // файловая модель 2
    QFileSystemModel *filemodel;

    /**
     * @brief initWorkTabWidget -- создание текстовых полей интерпретатора
     */
    void initWorkTabWidget();

    /**
     * @brief createNewTabAndSetToNew -- создание нового текстового поля и назначение наблюдаемого индекса
     * @param name -- имя для назначения названия страницы
     */
    void createNewTabAndSetToNew(QString name);

    /**
     * @brief createNewTabAndSetToNew -- cоздание нового текстового поля пустого и назначение.
     */
    void createNewTabAndSetToNew();

    /**
     * @brief getTextFromCurrentTab -- получение текста из открытого в данный момент текстового окна.
     * @return
     */
    QString getTextFromCurrentTab();

    /**
     * @brief maybeSave -- метод для предлога сохранения открытых файлов.
     * @return
     */
    bool maybeSave();

    /**
     * @brief getWasSavedFlag -- берет флаг сохранения.
     * @return
     */
    bool getWasSavedFlag();
    /**
     * @brief isInFileSystem -- берет флаг сохранения как.
     * @return
     */
    bool isInFileSystem();
    /**
     * @brief linkLib -- подключение библиотеки к методам.
     */
    void linkLib();

    /**
     * @brief runThread -- поток процесса интерпретации.
     */
    MyThread *runThread;

    /**
     * @brief rez -- результат.
     */
    bool rez;
    bool trace;
    /**
     * @brief Err -- вывод ошибки.
     */
    char *Err;
    /**
     * @brief output -- вывод интерпретации.
     */
    char *output;

    bool debug;

    QLibrary TransLib;

    /**
     * @brief translate -- функция для трансляции.
     */
    TransFunction		translate;
    /**
     * @brief retFuncList -- метод для возврата списка с функциями для дальнейшей интерпретации.
     */
    RetFuncListFunction retFuncList;
    /**
     * @brief argTranslate -- трансляция аргументов.
     */
    ArgTransFunction	argTranslate;
    /**
     * @brief makeStep, breakWork -- методы для следующего шага и окончания работы интерпретатора.
     */
    StepFunction		makeStep, breakWork;
    /**
     * @brief getCurResult -- метод для сбора результата выполнения функции.
     */
    GetResultFunction   getCurResult;
    /**
     * @brief setStrBuffer -- установление буфера вывода информации
     */
    SetDataBufferFunction setStrBuffer;

    /**
     * @brief Translate -- метод связи трансляции библиотеки и слота трансляции.
     */
    void Translate();

    /**
     * @brief Execute -- метод связи интерпретации библиотеки и слота интерпретации.
     */
    void Execute();

    /**
     * @brief BreakExecute -- метод для остановки интерпретации библиотеки и слота остановки интерпретации.
     */
    void BreakExecute();

    /**
     * @brief Debug -- метод связи метода отладки библиотеки и слота отладки.
     */
    void Debug();

    /**
     * @brief BreakDebug -- метод для связи остановки отладки библиотеки и слота оствноки отладки библиотеки.
     */
    void BreakDebug();

    /**
     * @brief DebugOrExecute -- выбор типа интерпретации.
     */
    void DebugOrExecute();

    /**
     * @brief run -- запуск функции
     * @param inFishka
     * @param name
     * @param trace
     */
    void run(void *inFishka, QString name, bool trace);

    /**
     * @brief ThreadFinshed -- закрытие потока.
     */
    void ThreadFinshed();

    /**
     * @brief fillList -- заполнение комбобокса со списком функции.
     */
    void fillList();
};

#endif // REDACTORMAINWINDOW_H
