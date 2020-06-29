#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QDialog>
#include <QSettings>
#include <windows.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QVector>
#include <QStandardItemModel>
#include <QComboBox>
#include "sqlmanager.h"
#include "adddefcategorydialog.h"


#define SETTING_FILE "/settings.ini"
#define DEFAULT_DATABASE_FILE "/database.db"
#define GROUP_DATABASE_DIRECTORY "DATA_BASE_DIRECTORY"
#define VAR_DATABASE_FILE "dataBaseFile"
#define VAR_DATABASE_AUTOOPENING "dbAutoOpening"
#define DEFAULT_DATABASE_AUTOOPENING "false"
#define GROUP_SALARY_PARAMETERS "SALARY_PARAMETERS"
#define VAR_BASIC_WAGE "basicWage"
#define VAR_BASIC_WAGE_KOEF_SALARY "basicWageKoefSalary"
#define GROUP_MAIN_WND_GEOMETRY "MAIN_WINDOW_GEOMETRY"
#define VAR_MAIN_WND_WIDTH "mainWindowWidth"
#define VAR_MAIN_WND_HEIGHT "mainWindowHeight"
#define DEFAULT_MAIN_WND_WIDTH 880
#define DEFAULT_MAIN_WND_HEIGHT 600

#define PICT_LOGO ":app_imgs/Logo.jpg"
#define PICT_LOGO_SMALL ":app_imgs/Logo_small.jpg"
#define PICT_LOGO_95 ":app_imgs/Logo_95.jpg"
#define PICT_SETTINGS ":app_imgs/SettingsIcon.JPG"
#define PICT_CALENDAR ":app_imgs/DateBook_48.png"
#define PICT_CALCULATOR ":app_imgs/calculator.png"
#define PICT_SAVE ":app_imgs/floppy_32.png"
#define PICT_REPORT ":app_imgs/report_editor.png"
#define PICT_DELETE ":app_imgs/delete.png"
#define PICT_ADD ":app_imgs/add.png"


namespace Ui {
class SettingWindow;
}

class SettingWindow : public QDialog
{
    Q_OBJECT

    // Методы
public:
    explicit SettingWindow(QWidget *parent = nullptr);
    ~SettingWindow();

public:
    QString dataBaseFile;
    float basicWage;
    float koefBasicWage;
    bool isDBAutoOpening;

public slots:
    // Методы для настроек

    void readSettings();
    void writeSettings();
    void setSettings();
    void cancelSettings();
    QString getSettingFilePath();

    void setAdminMode(bool isAdmin);

    // Методы для настроек значений из БД
    void openDataBase(const QString dbFile);
    void saveDataBase();
    void connectToDataBase(const QString dbFile);
    void editWidgets();

    // Проверка всех полей
    bool isEditChanged();
    bool isAutoOpeningChanged();
    bool isStafferChanged();
    bool isCategoryChanged();
    bool isTaxChanged();
    bool isDefCategoryChanged();
    bool isSettingChanged();

    // Изменение категорий и сотрудников
    void addCategory();
    void removeCategory();
    QVector <QString> getCategory();
    void setCategory();

    void addStaffer();
    void removeStaffer();
    QVector <QString> getStaffer();
    void setStaffer();

    void addTax();
    void removeTax();
    QVector <QString> getTax();
    void setTax();

    bool isCmbBoxItemExist(QComboBox *cmbBox, QString checkedString);

    void addDefCategory(); // Добавить конкретную категорию деятельности, заданную пользователем
    void removeDefCategory();
    QVector <DefinedCategory> getDefCategory();
    void setDefCategory();
    bool isDefCategoryExist(QStandardItemModel *itemModel, DefinedCategory checkedDefCtgry);

    void addRowInTableDefCategory(DefinedCategory defCtgry); // Добавить строку в таблицу заданных категорий
    void removeRowInTableDefCategory(int selectedRow); // Удалить строку из таблицы заданных категорий
    void editTableDefCategory(); // Полностью редактировать таблицу заданных категорий


signals:
    void signalToRemoveFormBtnCategory(int formId);
    void signalToAddFormBtnCategory(QString strCategory);
    void signalToEditFormBtnCategory(QVector <QString> categoryVector);

    void signalToRemoveStaffer(int id);
    void signalToAddStaffer(const QString strStaffer);
    void signalToEditStaffer(const QVector <QString> stafferVector);

    void signalToEditFormBtnCategory(QVector <DefinedCategory> defCategoryVector);

    // Сигнал на доступ к работе в главном окне, если база данных открыта
    void signalToSetEnableWorkFields(bool isDatabaseOpen);

private:
    void virtual reject();



private:
    Ui::SettingWindow *ui;

    QString m_settingFilePath;

    QVector <QString> m_category; // сохранить текущие названия категорий
    QVector <QString> m_staffer; // сохранить текущие имена сотрудников
    QVector <QString> m_tax; // сохрнить текущие виды налогов
    QVector <DefinedCategory> m_defCategory; // сохранить текущие созданные категории

    QStandardItemModel *m_defCategoryModel = nullptr;
};

#endif // SETTINGWINDOW_H
