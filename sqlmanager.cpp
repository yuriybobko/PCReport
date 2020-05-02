#include "sqlmanager.h"

SqlManager::SqlManager(QObject *parent) : QObject(parent)
{
    dataBase = QSqlDatabase::addDatabase("QSQLITE");
}

SqlManager::~SqlManager()
{
    if (m_defCategoryTable)
        delete m_defCategoryTable;
    QSqlDatabase::removeDatabase(m_currentDataBaseFile);
}

void SqlManager::connectToDataBase(const QString dataBaseFile)
{
        if (m_defCategoryTable != nullptr)
            delete m_defCategoryTable;
        if (dataBase.isOpen()) {
            dataBase.close();
        }
    if (QFile(dataBaseFile).exists()) {
        this->openDataBase(dataBaseFile);
        m_currentDataBaseFile = dataBaseFile;
        emit signalToStatusBar("База данных открыта!");
        qDebug() << "База данных открыта!";
    }
    else {
        emit signalToStatusBar("База данных создана!");
        this->createDataBase(dataBaseFile);
    }
}

void SqlManager::testFunc()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:/Temp/staf_data.db");
    if (!db.open())
    {
        qDebug() << "Opening error!";
        return;
    }

    //Осуществляем запрос
    QSqlQuery query;
    if (!query.exec("SELECT id, task_typ FROM tasks"))
    {
        emit signalToStatusBar("Команда не выполнима");
        return;
    }

    //Выводим значения из запроса
    while (query.next())
    {
    QString id = query.value(0).toString();
    QString firstName = query.value(1).toString();
//    QString lastName = query.value(2).toString();
//    QString salary = query.value(3).toString();
    qDebug() << id << " " << firstName;;
    }
    db.close();

}

bool SqlManager::insertIntoOneTitleTable(const QString table, const QString title, const QString value)
{
       QSqlQuery query;
       QString queryString = "INSERT INTO " + table + " ( " +
               title + " )"
                " VALUES ('" + value + "');";
       query.prepare(queryString);
       if (query.exec())
           return true;
       else
           return false;
}

bool SqlManager::insertIntoDefCategoryTable(DefCategoriesTable defCtgryTable, int categoryId, int taxId,
                                            DefinedCategory defCtgry)
{
    QSqlQuery query;
    QString queryString = "SELECT " + defCtgryTable.title + ", " + defCtgryTable.tax + " FROM "
            + defCtgryTable.table +
            " WHERE " + defCtgryTable.title + " = " + QString::number(categoryId) + " AND " +
            defCtgryTable.tax + " = " + QString::number(taxId);
    if (query.exec(queryString)) {
        if (!query.next()) {
            query.prepare("INSERT INTO " + defCtgryTable.table + " ( " +
                          defCtgryTable.title + ", " + defCtgryTable.tax + ", " +  defCtgryTable.selfcoast + ", " +
                          defCtgryTable.koefSalary + ", " + defCtgryTable.koefProfit + " , " +
                          defCtgryTable.isSelling + " , " + defCtgryTable.isContracted + ") " +
                           " VALUES (:categoryId, :taxId, :selfcoast, :koefSalary, :koefProfit, "
                           ":isSelling, :isContracted)");
            query.bindValue(":categoryId", QString::number(categoryId));
            query.bindValue(":taxId", QString::number(taxId));
            query.bindValue(":selfcoast", QString::number(defCtgry.selfcoast));
            query.bindValue(":koefSalary", QString::number(defCtgry.koefSalary));
            query.bindValue(":koefProfit", QString::number(defCtgry.koefProfit));
            query.bindValue(":isSelling", QString::number(defCtgry.isSelling));
            query.bindValue(":isContracted", QString::number(defCtgry.isContracted));
            if (query.exec())
                return true;
        }
    }
    return false;
}

bool SqlManager::insertIntoDefCategoryRegisterTable(DefCategoryRegisterTable defCtgryRegTable,
                                                    DefCategoryRegisterRecord defCtgryRegRecord)
{
    QSqlQuery query;
    QString queryString = "INSERT INTO " + defCtgryRegTable.table + " ( " +
            defCtgryRegTable.date + ", " + defCtgryRegTable.staffer + ", " +
            defCtgryRegTable.category + ", " + defCtgryRegTable.tax + ", " +
            defCtgryRegTable.cash + " , " + defCtgryRegTable.amount + " , " + defCtgryRegTable.selfcoast + ") " +
             " VALUES (:date, :stafferId, :defCategoryId, :taxId, :cashId, :amount, :selfcoast)";
            if (query.prepare(queryString)) {
                query.bindValue(":date", defCtgryRegRecord.date);
                query.bindValue(":stafferId", defCtgryRegRecord.stafferId);
                query.bindValue(":defCategoryId", defCtgryRegRecord.categoryId);
                query.bindValue(":taxId", defCtgryRegRecord.taxId);
                query.bindValue(":cashId", defCtgryRegRecord.cashId);
                query.bindValue(":amount", defCtgryRegRecord.amount);
                query.bindValue(":selfcoast", defCtgryRegRecord.selfcoast);
                if (query.exec())
                    return true;
            }
    return false;
}

bool SqlManager::openDataBase(QString databaseFile)
{
    dataBase.setDatabaseName(databaseFile);
        if(dataBase.open()) {
            m_defCategoryTable = new QSqlTableModel(this, dataBase);
            QSqlQuery query;
            if (query.exec("PRAGMA foreign_keys = on;"))
                qDebug() << "foreign_keys = on";
            else
                qDebug() << "foreign_keys = off";
            return true;
        } else {
            return false;
        }
}

bool SqlManager::createDataBase(QString databaseFile)
{
    if (this->openDataBase(databaseFile)) {
        return (this->createTables()) ? true : false;
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}

void SqlManager::closeDataBase()
{
    if (dataBase.isOpen())
        dataBase.close();
    QSqlDatabase::removeDatabase(m_currentDataBaseFile);
}

QVector<QString> SqlManager::selectTitlesFromTable(QString table, QString title)
{
    QVector <QString> titleVector;
    QSqlQuery query;
    if (query.exec("SELECT " + title + " FROM " + table)) {
        int ii = 0;
        while (query.next()) {
            titleVector.append(query.value(0).toString());
            ii++;
        }

    }
    else {
        //emit signalToStatusBar("Не удалось прочитать");
        qDebug() << "Не удалось прочитать " + title + " из " + table;
    }
    return titleVector;
}

QVector<DefinedCategory> SqlManager::selectDefCategory(DefCategoriesView defCtgrsView)
{
    QVector<DefinedCategory> defCategoryVector;
    QSqlQuery query;
    if (query.exec("SELECT * FROM " + defCtgrsView.table)) {
        int ii = 0;
        while (query.next()) {
            int kk = 1;
            DefinedCategory defCategory;
            defCategory.categoryTitle = query.value(kk).toString(); kk++;
            defCategory.taxTitle = query.value(kk).toString(); kk++;
            defCategory.selfcoast = query.value(kk).toBool(); kk++;
            defCategory.koefSalary = query.value(kk).toFloat(); kk++;
            defCategory.koefProfit = query.value(kk).toFloat(); kk++;
            defCategory.isSelling = query.value(kk).toBool(); kk++;
            defCategory.isContracted = query.value(kk).toBool(); kk++;
            defCategoryVector.append(defCategory);
            ii++;
        }

        m_defCategoryTable->setTable(defCtgrsView.table);
        m_defCategoryTable->setEditStrategy(QSqlTableModel::OnManualSubmit);
        if (m_defCategoryTable->select())
            qDebug() << "model is selected";
        else
            qDebug() << "model is not selected";

    }
    else {
        //emit signalToStatusBar("Не удалось прочитать");
        qDebug() << "Не удалось прочитать " + defCtgrsView.table;
    }
    return defCategoryVector;
}

int SqlManager::selectIdFromTable(QString table, QString tableId, QString tableTitle, QString requiredTitle)
{
    int id = -1;
    QSqlQuery query;
    if (query.exec("SELECT " + tableId + " FROM " + table +
                   " WHERE " + tableTitle + " = '" + requiredTitle + "'")) {
        while (query.next()) {
            id = query.value(0).toInt();
        }
    }
    else {
        //emit signalToStatusBar("Не удалось прочитать");
        qDebug() << "Не удалось найти id для " + tableTitle + " из " + table;
    }
    return id;
}

QSqlTableModel *SqlManager::getDefCategoryTable()
{
    return m_defCategoryTable;
}

bool SqlManager::createTables()
{
    bool result = true;
    if (!this->createOneTitleTable(staffersTable.table, staffersTable.id, staffersTable.name))
            result = false;
    if (!this->createOneTitleTable(categoriesTable.table, categoriesTable.id, categoriesTable.title))
            result = false;
    if (!this->createOneTitleTable(taxesTable.table, taxesTable.id, taxesTable.title))
            result = false;
    if (!this->createOneTitleTable(cashTable.table, cashTable.id, cashTable.title)) {
        result = false;
    }
    this->insertIntoOneTitleTable(cashTable.table, cashTable.title, CASH_STRING);
    this->insertIntoOneTitleTable(cashTable.table, cashTable.title, NONCASH_STRING);
    if (!this->createDefCategoriesTable(defCategoriesTable, categoriesTable, taxesTable))
        result = false;
    if (!this->createDefCategoriesView(defCategoriesView, defCategoriesTable, categoriesTable, taxesTable))
        result = false;
    if (!this->createDefCategoryRegisterTable(defCategoryRegisterTable, staffersTable, categoriesTable,
                                              taxesTable, cashTable))
        result = false;
    if (!this->createDefCategoryRegisterView(defCategoryRegisterView, defCategoryRegisterTable, staffersTable,
                                             categoriesTable, taxesTable, cashTable))
        result = false;
    if (!this->createCostsRegisterTable(costsRegisterTable, cashTable))
        result = false;
    return result;
}

bool SqlManager::createOneTitleTable(QString table, QString id, QString title)
{
    QSqlQuery query;
    if (query.exec("CREATE TABLE " + table + " (" +
                   id + " INTEGER PRIMARY KEY AUTOINCREMENT," +
                   title + " TEXT UNIQUE NOT NULL"
                   ");")) {
        return true;
    }
    else
        return false;
}

bool SqlManager::createDefCategoriesTable(DefCategoriesTable defCtgryTable,
                                          CategoriesTable ctgryTable, TaxesTable taxesTable)
{
    QSqlQuery query;
    QString queryString = "CREATE TABLE " + defCtgryTable.table + " (" +
            defCtgryTable.id + " INTEGER PRIMARY KEY, " +
            defCtgryTable.title + " INTEGER NOT NULL, " +
            defCtgryTable.tax + " INTEGER NOT NULL, " +
            defCtgryTable.selfcoast + " INTEGER NOT NULL, " +
            defCtgryTable.koefSalary + " REAL, " +
            defCtgryTable.koefProfit + " REAL, " +
            defCtgryTable.isSelling + " INTEGER, " +
            defCtgryTable.isContracted + " INTEGER, " +
            "FOREIGN KEY (" + defCtgryTable.title + ") REFERENCES " + ctgryTable.table + "(" +
            ctgryTable.id + ") ON DELETE CASCADE" +
            " FOREIGN KEY (" + defCtgryTable.tax + ") REFERENCES " + taxesTable.table + "(" +
            taxesTable.id + ") ON DELETE CASCADE" +
            ");";
    if (query.exec(queryString)) {
        return true;
    }
    else
        return false;
}

bool SqlManager::createDefCategoriesView(DefCategoriesView defCtgryView, DefCategoriesTable defCtgryTable,
                                         CategoriesTable ctgryTable, TaxesTable taxesTable)
{
    QSqlQuery query;
    QString queryString = "CREATE VIEW " + defCtgryView.table + " AS "
            "SELECT " + defCtgryTable.table + "." + defCtgryTable.id + " AS " + defCtgryView.id + ", " +
            ctgryTable.table + "." + ctgryTable.title + " AS " + defCtgryView.title + ", " +
            taxesTable.table + "." + taxesTable.title + " AS " + defCtgryView.tax + ", " +
            defCtgryTable.table + "." + defCtgryTable.selfcoast + " AS " + defCtgryView.selfcoast + ", " +
            defCtgryTable.table + "." + defCtgryTable.koefSalary + " AS " + defCtgryView.koefSalary + ", " +
            defCtgryTable.table + "." + defCtgryTable.koefProfit + " AS " + defCtgryView.koefProfit + ", " +
            defCtgryTable.table + "." + defCtgryTable.isSelling + " AS " + defCtgryView.isSelling + ", " +
            defCtgryTable.table + "." + defCtgryTable.isContracted + " AS " + defCtgryView.isContracted +
            " FROM " + defCtgryTable.table + ", " +
            ctgryTable.table + ", " +
            taxesTable.table +
            " WHERE " + defCtgryTable.table + "." + defCtgryTable.title + " = " +
            ctgryTable.table + "." + ctgryTable.id +
            " AND " + defCtgryTable.table + "." + defCtgryTable.tax + " = " +
            taxesTable.table + "." + taxesTable.id + ";";
    if (query.exec(queryString)) {
        return true;
    }
    else
        return false;
}

bool SqlManager::createDefCategoryRegisterTable(DefCategoryRegisterTable defCtgryRegTable,
                                                StaffersTable staffersTable, CategoriesTable ctgryTable,
                                                TaxesTable taxesTable, CashTable cashTable)
{
    QSqlQuery query;
    QString queryString = "CREATE TABLE " + defCtgryRegTable.table + " (" +
            defCtgryRegTable.id + " INTEGER PRIMARY KEY AUTOINCREMENT, " +
            defCtgryRegTable.date + " TEXT NOT NULL, " +
            defCtgryRegTable.staffer + " INTEGER NOT NULL, " +
            defCtgryRegTable.category + " INTEGER NOT NULL, " +
            defCtgryRegTable.tax + " INTEGER NOT NULL, " +
            defCtgryRegTable.cash + " INTEGER NOT NULL, " +
            defCtgryRegTable.amount + " REAL, " +
            defCtgryRegTable.selfcoast + " REAL, " +
            "FOREIGN KEY (" + defCtgryRegTable.staffer + ") REFERENCES " + staffersTable.table + "(" +
            staffersTable.id + ") ON DELETE CASCADE" +
            " FOREIGN KEY (" + defCtgryRegTable.category + ") REFERENCES " + ctgryTable.table + "(" +
            ctgryTable.id + ") ON DELETE CASCADE" +
            " FOREIGN KEY (" + defCtgryRegTable.tax + ") REFERENCES " + taxesTable.table + "(" +
            taxesTable.id + ") ON DELETE CASCADE" +
            " FOREIGN KEY (" + defCtgryRegTable.cash + ") REFERENCES " + cashTable.table + "(" +
            cashTable.id + ") ON DELETE CASCADE" +
            ");";
    if (query.exec(queryString)) {
        return true;
    }
    else
        return false;
}

bool SqlManager::createDefCategoryRegisterView(DefCategoryRegisterView defCtgryRegView,
                                               DefCategoryRegisterTable defCtgryRegTable,
                                               StaffersTable staffersTable, CategoriesTable ctgryTable,
                                                TaxesTable taxesTable,
                                               CashTable cashTable)
{
    QSqlQuery query;
    QString queryString = "CREATE VIEW " + defCtgryRegView.table + " AS "
            "SELECT " + defCtgryRegTable.table + "." + defCtgryRegTable.id + " AS " + defCtgryRegView.id + ", " +
            defCtgryRegTable.table + "." + defCtgryRegTable.date + " AS " + defCtgryRegView.date + ", " +
            staffersTable.table + "." + staffersTable.name+ " AS " + defCtgryRegView.staffer + ", " +
            ctgryTable.table + "." + ctgryTable.title + " AS " + defCtgryRegView.category + ", " +
            taxesTable.table + "." + taxesTable.title + " AS " + defCtgryRegView.tax + ", " +
            cashTable.table + "." + cashTable.title + " AS " + defCtgryRegView.cash + ", " +
            defCtgryRegTable.table + "." + defCtgryRegTable.amount + " AS " + defCtgryRegView.amount + ", " +
            defCtgryRegTable.table + "." + defCtgryRegTable.selfcoast + " AS " + defCtgryRegView.selfcoast +
            " FROM " + defCtgryRegTable.table + ", " +
            staffersTable.table + ", " +
            ctgryTable.table + ", " +
            taxesTable.table + ", " +
            cashTable.table +
            " WHERE " + defCtgryRegTable.table + "." + defCtgryRegTable.staffer + " = " +
            staffersTable.table + "." + staffersTable.id +
            " AND " + defCtgryRegTable.table + "." + defCtgryRegTable.category + " = " +
            ctgryTable.table + "." + ctgryTable.id +
            " AND " + defCtgryRegTable.table + "." + defCtgryRegTable.tax + " = " +
            taxesTable.table + "." + taxesTable.id +
            " AND " + defCtgryRegTable.table + "." + defCtgryRegTable.cash + " = " +
            cashTable.table + "." + cashTable.id +
            ";";
    if (query.exec(queryString)) {
        return true;
    }
    else
        return false;
}

bool SqlManager::createCostsRegisterTable(CostsRegisterTable costsRegTable, CashTable cashTable)
{
    QSqlQuery query;
    QString queryString = "CREATE TABLE " + costsRegTable.table + " (" +
            costsRegTable.id + " INTEGER PRIMARY KEY AUTOINCREMENT, " +
            costsRegTable.date + " TEXT NOT NULL, " +
            costsRegTable.cash + " INTEGER NOT NULL, " +
            costsRegTable.amount + " REAL, " +
            costsRegTable.description + " TEXT NOT NULL, " +
            "FOREIGN KEY (" + costsRegTable.cash + ") REFERENCES " + cashTable.table + "(" +
            cashTable.id + ") ON DELETE CASCADE" +
            ");";
    if (query.exec(queryString)) {
        return true;
    }
    else
        return false;
}

