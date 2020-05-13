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
    //float basicWageKoefSalary;
    float koefBasicWage;

public slots:
    // Методы для настроек

    void readSettings();
    void writeSettings();
    void setSettings();
    void cancelSettings();

    // Методы для настроек значений из БД
    void openDataBase(const QString dbFile);
    void connectToDataBase(const QString dbFile);
    void editWidgets();

//    void readDBStaffer(SqlManager *sqlmngr);
//    void readDBCategory(SqlManager *sqlmngr);
//    void readDBTaxes(SqlManager *sqlmngr);

    // Проверка всех полей
    bool isEditChanged();
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
//    void signalToAddStrItem(QString strItem);

//    void signalToRemoveStrItem(int itemId);

    void signalToRemoveFormBtnCategory(int formId);
    void signalToAddFormBtnCategory(QString strCategory);
    void signalToEditFormBtnCategory(QVector <QString> categoryVector);

    void signalToRemoveStaffer(int id);
    void signalToAddStaffer(const QString strStaffer);
    void signalToEditStaffer(const QVector <QString> stafferVector);

    void signalToEditFormBtnCategory(QVector <DefinedCategory> defCategoryVector);

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
