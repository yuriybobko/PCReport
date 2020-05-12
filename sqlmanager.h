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
    bool insertIntoDefCategoryTable(int categoryId, int taxId,
                                    DefinedCategory defCtgry);

    bool insertIntoDefCategoryRegisterTable(DefCategoryRegisterRecord defCtgryRegRecord);
    bool insertIntoSalaryRegisterTable(SalaryRegisterRecord salaryRegRecord);
    bool insertIntoCostsRegisterTable(CostsRegisterRecord costsRegRecord);
    bool updateRecordInSalaryRegisterTable(SalaryRegisterRecord SalaryRegRecord);

    float calcSalary(QVector <DefCategoryRegisterRecord> defCtgryRegRecordVector,
                     QVector<DefinedCategoryRecord> defCtgryRecordVector,
                     SalaryRegisterRecord oldSalaryRegRecord);

    // Выбрать сумму за определенный период из реестра категорий по заданному id категории и налога
    float selectTotalSumInPeriod(QString firstDate, QString secondDate, int categoryId, int taxId);
    // Выбрать себестоимость за определенный период из реестра категорий по заданному id категории и налога
    float selectTotalSelfcoastInPeriod(QString firstDate, QString secondDate, int categoryId, int taxId);
    // Выбрать сумму за определенный период из реестра категорий по заданному id категории и налога
    float selectTotalSalaryInPeriod(QString firstDate, QString secondDate);
    // Выбрать максимальный id из таблицы заданных категорий в выбранный период
    float selectTotalCostsInPeriod(QString firstDate, QString secondDate);
    // Выбрать максимальный id из таблицы заданных категорий в выбранный период
    int selectMaxCategoryIdInPeriod(QString firstDate, QString secondDate);
    // Выбрать заданное поле из таблицы
    QVector <QString> selectTitlesFromTable(QString table, QString title);
    // Выбрать заданную пользователем категорию через представление-таблицу заданных категорий
    QVector <DefinedCategory> selectDefCategory();
    // Выбрать заданную пользователем категорию через таблицу заданных категорий
    QVector <DefinedCategoryRecord> selectDefCategoryRecord();
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
    bool isSalaryRecordExist(SalaryRegisterRecord salaryRegRecord);


private:
    bool openDataBase(QString databaseFile);
    bool createDataBase(QString databaseFile);

    bool createTables();
    bool createOneTitleTable(QString table, QString id, QString title);
    bool createDefCategoriesTable();
    bool createDefCategoriesView();

    bool createDefCategoryRegisterTable();
    bool createDefCategoryRegisterView();
    bool createCostsRegisterTable();
    bool createCostsRegisterView();
    bool createSalaryRegisterTable();
    bool createSalaryRegisterView();

private:
    QSqlDatabase dataBase;
    QString m_currentDataBaseFile;


    QSqlTableModel *m_defCategoryTable = nullptr;

};

#endif // SQLMANAGER_H
