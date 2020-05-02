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

    // Выбрать заданное поле из таблицы
    QVector <QString> selectTitlesFromTable(QString table, QString title);
    // Выбрать заданную пользователем категорию через представление-таблицу
    QVector <DefinedCategory> selectDefCategory(DefCategoriesView defCtgrsView);
    //Выбрать id из таблицы
    int selectIdFromTable(QString table, QString tableId, QString tableTitle, QString requiredTitle);

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

private:
    QSqlDatabase dataBase;
    QString m_currentDataBaseFile;


    QSqlTableModel *m_defCategoryTable = nullptr;

};

#endif // SQLMANAGER_H
