#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QVector>
#include <QLayout>
#include <QInputDialog>
#include <QSettings>
#include <windows.h>
#include <QDebug>
#include <QDate>
#include <QTime>
#include "formbtncategory.h"
#include "formcategory.h"
#include "settingwindow.h"
#include "calendarwindow.h"


extern SqlManager sqlManager;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setChildWidgets();

    void addFormBtnCategory(QString strCategory);
    void addFormBtnCategory(DefinedCategory defCategory); // New!

    void removeFormBtnCategory(int formId);

    void showFormCategory(int formId);

    void editFormBtnCategory(const QVector <QString> categoryVector);
    void editFormBtnCategory(const QVector <DefinedCategory> defCategoryVector); // New!

    void addStaffer(QString strStaffer);

    void removeStaffer(int id);

    void editStaffer(const QVector <QString> stafferVector);

    void showStatusBar(QString statusBarText); // Вывод в statusBar

    void showSettingWindow();
    void showCalendarWindow();
    QString getDateFromCalendar();

    void fillFrameTotalSum(); // Заполнить поля полной суммы и зарплаты
    float getTotalCashMoney(); // Получить значения всех полей наличные
    float getTotalNonCashMoney(); // Получить значения полей безналичные
    void setTotalSum(float totalCash, float totalNonCash); // Вычсилить полную сумму
    float calcSalary(); // Вычислить зарплату
    void setSalary(); // Записать зарплату в текстовое поле
    void clearDailyReportTabFinanceEdits(); // Очистить все финансовые текстовые поля во вкладке дневного отчета
    void clearCostsTabEdits(); // Очистить все текстовые поля во вкладке расходов

    void writeDefCategoryRecordToDataBase(); // Сделать запись в реестре категорий
    void writeCostsRecordToDataBase(); // Сделать запись в реестре расходов

    void showProfitInEdit();

    // Методы событий

    void closeEvent(QCloseEvent *event);

    // Тестовые функции

    void testFunc();

private:
    Ui::MainWindow *ui;

    SettingWindow *m_settingWndw;
    CalendarWindow *m_calendarWndw;

    int m_dHeight = 50;
    int m_dWidth = 50;

    //QFrame *FrameLayoutCmdBtns;
    QVBoxLayout *LayoutFormBtnCategory;

    QVector <FormBtnCategory *> m_FormBtnCategory;
    QVector <FormCategory *> m_FormCategory; 
};
#endif // MAINWINDOW_H
