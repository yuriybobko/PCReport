#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QVariant>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include "structures.h"

class SqlManager : public QObject
{
    Q_OBJECT
public:
    explicit SqlManager(QObject *parent = nullptr);
    ~SqlManager();

    void connectToDataBase(const QString dataBaseFile);
    void closeDataBase();

    QSqlTableModel *getDefCategoryTable();

    void testFunc();

signals:
    // Сигнал в status bar
    void signalToStatusBar(QString statusBarText);

public slots:
    bool insertIntoOneTitleTable(const QString table, const QString title, const QString value);
    bool insertIntoDefCategoryTable(DefCategoriesTable defCtgryTable, int categoryId, int taxId,
                                    DefinedCategory defCtgry);

    bool insertIntoDefCategoryRegisterTable(DefCategoryRegisterTable defCtgryRegTable,
                                            DefCategoryRegisterRecord defCtgryRegRecord);
    bool insertIntoSalaryRegisterTable(SalaryRegisterRecord salaryRegRecord);
    bool insertIntoCostsRegisterTable(CostsRegisterTable costsRegTable, CostsRegisterRecord costsRegRecord);
    bool updateRecordInSalaryRegisterTable(SalaryRegisterRecord SalaryRegRecord);

    float calcSalary(QVector <DefCategoryRegisterRecord> defCtgryRegRecordVector,
                     QVector<DefinedCategoryRecord> defCtgryRecordVector,
                     SalaryRegisterRecord oldSalaryRegRecord);

    // Выбрать сумму за определенный период
    float selectTotalSumInPeriod(DefCategoryRegisterTable defCtgryRegTable,
                                 QString firstDate, QString secondDate, int requiredId);
    // Выбрать заданное поле из таблицы
    QVector <QString> selectTitlesFromTable(QString table, QString title);
    // Выбрать заданную пользователем категорию через представление-таблицу реестра категорий
    QVector <DefinedCategory> selectDefCategory(DefCategoriesView defCtgrsView);
    // Выбрать заданную пользователем категорию через представление-таблицу реестра категорий
    QVector <DefinedCategoryRecord> selectDefCategoryRecord(QVector<DefCategoryRegisterRecord>
                                                            defCtgryRegRecordVector);
    // Выбрать запись из реестра категорий по заданной дате и id сотрудника
    QVector <DefCategoryRegisterRecord> selectDefCategoryRegRecord(QString date, int stafferId);
    // Выбрать запись из реестра зарплат по заданной дате и id сотрудника
    SalaryRegisterRecord selectSalaryRecord(QString date, int stafferId);
    //Выбрать id из таблицы
    int selectIdFromTable(QString table, QString tableId, QString tableTitle, QString requiredTitle);

    // Проверка существования записи с текущей датой и сотрудником
    bool isSalaryRecordExist(SalaryRegisterTable salaryRegTable, SalaryRegisterRecord salaryRegRecord);


private:
    bool openDataBase(QString databaseFile);
    bool createDataBase(QString databaseFile);

    bool createTables();
    bool createOneTitleTable(QString table, QString id, QString title);
    bool createDefCategoriesTable(DefCategoriesTable defCtgryTable, CategoriesTable ctgryTable,
                                  TaxesTable taxesTable);
    bool createDefCategoriesView(DefCategoriesView defCtgryView, DefCategoriesTable defCtgryTable,
                                 CategoriesTable ctgryTable, TaxesTable taxesTable);

    bool createDefCategoryRegisterTable(DefCategoryRegisterTable defCtgryRegTable, StaffersTable staffersTable,
                                        CategoriesTable ctgryTable, TaxesTable taxesTable,
                                        CashTable cashTable);
    bool createDefCategoryRegisterView(DefCategoryRegisterView defCtgryRegView,
                                       DefCategoryRegisterTable defCtgryRegTable, StaffersTable staffersTable,
                                       CategoriesTable ctgryTable, TaxesTable taxesTable, CashTable cashTable);
    bool createCostsRegisterTable(CostsRegisterTable costsRegTable, CashTable cashTable);
    bool createCostsRegisterView(CostsRegisterView costsRegView, CostsRegisterTable costsRegTable,
                                 CashTable cashTable);
    bool createSalaryRegisterTable(SalaryRegisterTable salaryRegTable, StaffersTable staffersTable);
    bool createSalaryRegisterView(SalaryRegisterView salaryRegView, SalaryRegisterTable salaryRegTable,
                                 StaffersTable staffersTable);

private:
    QSqlDatabase dataBase;
    QString m_currentDataBaseFile;


    QSqlTableModel *m_defCategoryTable = nullptr;

};

#endif // SQLMANAGER_H
