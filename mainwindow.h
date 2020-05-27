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
//#include <QLibraryInfo>
#include <QDesktopWidget>
#include "formbtncategory.h"
#include "formcategory.h"
#include "settingwindow.h"
#include "calendarwindow.h"
#include "reportwindow.h"

#define CMBBOX_ITEM_CATEGORY "Записи категорий"
#define CMBBOX_ITEM_COSTS "Записи расходов"
#define CMBBOX_ITEM_SALARY "Записи заработной платы"

extern SqlManager sqlManager;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void signalToSettingWindowAdminMode(bool isAdmin);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void openUserModeDialog();
    void setAdminStatus(bool isAdmin);
    bool isAdminMode();

private slots:
    void setChildWidgets();

    void setAdminMode();

    void setEnableWorkFields(bool isDatabaseOpen);

    void addFormBtnCategory(QString strCategory);
    void addFormBtnCategory(DefinedCategory defCategory);

    void removeFormBtnCategory(int formId);

    void showFormCategory(int formId);

    void editFormBtnCategory(const QVector <QString> categoryVector);
    void editFormBtnCategory(const QVector <DefinedCategory> defCategoryVector);

    void addStaffer(QString strStaffer);

    void removeStaffer(int id);

    void editStaffer(const QVector <QString> stafferVector);

    void showStatusBar(QString statusBarText); // Вывод в statusBar

    void showSettingWindow();
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
    void writeSalaryRecordToDataBase(); // Сделать запись в реестр зарплат
    void writeCostsRecordToDataBase(); // Сделать запись в реестре расходов

    void showProfitInEdit();

    void editTableView(); // Редактировать таблицу записей из реестров

    void setDefCategoryRegisterInTableView(); // Задать таблицу из реестра категорий
    void setSalaryRegisterInTableView(); // Задать таблицу из реестра зарплат
    void setCostsRegisterInTableView(); // Задать таблицу из реестра расходов

    // Добавить строку в таблицу записей из реестра категорий
    void addRowInTableView(DefCategoryRegisterRecordView defCtgryRegRecordView);
    // Добавить строку в таблицу записей из реестра зарплат
    void addRowInTableView(SalaryRegisterRecordView salaryRegRecordView);
    // Добавить строку в таблицу записей из реестра расходов
    void addRowInTableView(CostsRegisterRecordView costsRegRecordView);

    // Удалить строку из таблицы записей
    void removeRecordInTableView();

    // Работа с настройками
    void readSettings();
    void writeSettings();

    void closeApp();

    // Методы событий
    void closeEvent(QCloseEvent *event);


private:
    Ui::MainWindow *ui;

    SettingWindow *m_settingWndw;
    ReportWindow *m_reportWndw;

    int m_dHeight = 50;
    int m_dWidth = 50;
    int m_wndInitWidth;
    int m_wndInitHeight;

    //QFrame *FrameLayoutCmdBtns;
    QVBoxLayout *LayoutFormBtnCategory;

    QVector <FormBtnCategory *> m_FormBtnCategory;
    QVector <FormCategory *> m_FormCategory; 

//    QVector <DefCategoryRegisterRecordView> m_defCategoryRegRecordView;
//    QVector <CostsRegisterRecordView> m_salaryRegRecordView;
//    QVector <CostsRegisterRecordView> m_costsRegRecordView;

    QStandardItemModel *m_itemModel = nullptr;

    RegisterType m_registerType;

    bool m_isAdmin;

    QVBoxLayout *LayoutDefCategory;
};
#endif // MAINWINDOW_H
