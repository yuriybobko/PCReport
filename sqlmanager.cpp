#include "sqlmanager.h"

SqlManager::SqlManager(QObject *parent) : QObject(parent)
{
}

SqlManager::~SqlManager()
{
    if (m_defCategoryTable)
        delete m_defCategoryTable;
    QSqlDatabase::removeDatabase(m_currentDataBaseFile);
}

void SqlManager::connectToDataBase(const QString dataBaseFile)
{
    if (dataBase.driverName().isEmpty())
        dataBase = QSqlDatabase::addDatabase("QSQLITE");

    if (dataBase.databaseName() == dataBaseFile) {
        return;
    }

    if (m_defCategoryTable != nullptr)
        delete m_defCategoryTable;
    if (dataBase.isOpen()) {
        dataBase.close();
    }
    if (QFile(dataBaseFile).exists()) {
        if (this->openDataBase(dataBaseFile)) {
            emit signalToStatusBar("База данных открыта");
        }
        else {
            emit signalToStatusBar("Не удается открыть базу данных");
        }
        m_currentDataBaseFile = dataBaseFile;
    }
    else {
        if (this->createDataBase(dataBaseFile)) {
            emit signalToStatusBar("База данных создана!");
        }
        else {
            emit signalToStatusBar("Не удается создать базу данных");
        }
    }
}

bool SqlManager::insertIntoOneTitleTable(const QString table, const QString title, const QString value)
{
       QSqlQuery query;
       QString queryString = "INSERT INTO " + table + " ( " +
               title + " )"
                " VALUES ('" + value + "');";
       if (!dataBase.isOpen()) {
           emit signalToStatusBar("База данных не открыта");
           return false;
       }
       query.prepare(queryString);
       if (query.exec())
           return true;
       else
           return false;
}

bool SqlManager::insertIntoDefCategoryTable(int categoryId, int taxId, DefinedCategory defCtgry)
{
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return false;
    }
    DefCategoriesTable defCtgryTable;
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

bool SqlManager::insertIntoDefCategoryRegisterTable(DefCategoryRegisterRecord defCtgryRegRecord)
{
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return false;
    }
    DefCategoryRegisterTable defCtgryRegTable;
    QSqlQuery query;
    QString queryString = "INSERT INTO " + defCtgryRegTable.table + " (" +
            defCtgryRegTable.date + ", " + defCtgryRegTable.staffer + ", " +
            defCtgryRegTable.category + ", " + defCtgryRegTable.tax + ", " +
            defCtgryRegTable.cash + ", " + defCtgryRegTable.amount + ", " +
            defCtgryRegTable.selfcoast + ")" +
             " VALUES (:date, :stafferId, :defCategoryId, :taxId, :cashId, :amount, :selfcoast)" +
            ";";
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

bool SqlManager::insertIntoSalaryRegisterTable(SalaryRegisterRecord salaryRegRecord)
{
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return false;
    }
    SalaryRegisterTable salaryRegTable;
    QSqlQuery query;
    QString queryString = "INSERT INTO " + salaryRegTable.table + " (" +
            salaryRegTable.date + ", " + salaryRegTable.staffer + ", " +
            salaryRegTable.amount + ", " + salaryRegTable.basicWage + ", " +
            salaryRegTable.koefBasicWage + ")" +
             " VALUES (:date, :stafferId, :amount, :basicWage, :koefBasicWage)" +
            ";";
    if (query.prepare(queryString)) {
        query.bindValue(":date", salaryRegRecord.date);
        query.bindValue(":stafferId", salaryRegRecord.stafferId);
        query.bindValue(":amount", salaryRegRecord.amount);
        query.bindValue(":basicWage", salaryRegRecord.basicWage);
        query.bindValue(":koefBasicWage", salaryRegRecord.koefBasicWage);
        if (query.exec())
            return true;
    }
    return false;
}

bool SqlManager::insertIntoCostsRegisterTable(CostsRegisterRecord costsRegRecord)
{
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return false;
    }
    CostsRegisterTable costsRegTable;
    QSqlQuery query;
    QString queryString = "INSERT INTO " + costsRegTable.table + " (" +
            costsRegTable.date + ", " + costsRegTable.cash + ", " +
            costsRegTable.amount + ", " + costsRegTable.description + ")" +
             " VALUES (:date, :cashId, :amount, :description)" +
            ";";
    if (query.prepare(queryString)) {
        query.bindValue(":date", costsRegRecord.date);
        query.bindValue(":cashId", costsRegRecord.cashId);
        query.bindValue(":amount", costsRegRecord.amount);
        query.bindValue(":description", costsRegRecord.description);
        if (query.exec())
            return true;
    }
    return false;
}

bool SqlManager::updateRecordInSalaryRegisterTable(SalaryRegisterRecord salaryRegRecord)
{
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return false;
    }
    SalaryRegisterTable salaryRegTable;
    QString requiredDate = salaryRegRecord.date;
    int requiredStafferId = salaryRegRecord.stafferId;
    QVector<DefCategoryRegisterRecord> defCtgryRegRecordVector = this->selectDefCategoryRegRecord(requiredDate,
                                                                                               requiredStafferId);
    QVector<DefinedCategoryRecord> defCtgryRecordVector = this->selectDefCategoryRecord(defCtgryRegRecordVector);
    SalaryRegisterRecord oldSalaryRegRecord = this->selectSalaryRecord(requiredDate, requiredStafferId);
    float newAmount = this->calcSalary(defCtgryRegRecordVector, defCtgryRecordVector, oldSalaryRegRecord);

    QSqlQuery query;
    QString queryString = "UPDATE " + salaryRegTable.table + " SET " + salaryRegTable.amount + " = :newAmount " +
            "WHERE " + salaryRegTable.date + " = :requiredDate " +
            " AND " + salaryRegTable.staffer + " = :requiredStafferId" +
            ";";
    if (query.prepare(queryString)) {
        query.bindValue(":newAmount", newAmount);
        query.bindValue(":requiredDate", requiredDate);
        query.bindValue(":requiredStafferId", requiredStafferId);
        if (query.exec())
                return true;
    }
    return false;
}

float SqlManager::calcSalary(QVector<DefCategoryRegisterRecord> defCtgryRegRecordVector,
                             QVector<DefinedCategoryRecord> defCtgryRecordVector,
                             SalaryRegisterRecord oldSalaryRegRecord)
{
    float salary = 0;
    for (int ii = 0; ii < defCtgryRegRecordVector.size(); ii++) {
        float adtnlSum = (defCtgryRegRecordVector.at(ii).amount -
                          defCtgryRegRecordVector.at(ii).selfcoast)*defCtgryRecordVector.at(ii).koefSalary;
        if (!defCtgryRecordVector.at(ii).isSelling) {
            adtnlSum *= defCtgryRecordVector.at(ii).koefProfit;
        }
        salary += adtnlSum;
    }
    salary += oldSalaryRegRecord.basicWage*(1 - oldSalaryRegRecord.koefBasicWage);
    if (salary < oldSalaryRegRecord.basicWage)
        salary = oldSalaryRegRecord.basicWage;
    return salary;
}

float SqlManager::selectTotalSumInPeriodByCtgryTax(QString firstDate, QString secondDate,
                                         int categoryId, int taxId)
{
    DefCategoryRegisterTable defCtgryRegTable;
    float totalSum = 0;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return totalSum;
    }
    QSqlQuery query;
    QString queryString = "SELECT SUM(" + defCtgryRegTable.amount +  ") FROM " + defCtgryRegTable.table +
            " WHERE " + defCtgryRegTable.date + " BETWEEN :firstDate AND :secondDate "
            "AND " + defCtgryRegTable.category + " = :categoryId "
            "AND " + defCtgryRegTable.tax + " = :taxId;";
    if (query.prepare(queryString)) {
        query.bindValue(":firstDate", firstDate);
        query.bindValue(":secondDate", secondDate);
        query.bindValue(":categoryId", categoryId);
        query.bindValue(":taxId", taxId);

        if (query.exec()) {
            while (query.next()) {
                totalSum = query.value(0).toFloat();
            }
        }
    }
    return totalSum;
}

float SqlManager::selectTotalSumInPeriodByCtgryTaxCash(QString firstDate, QString secondDate,
                                                       int categoryId, int taxId, int cashId)
{
    DefCategoryRegisterTable defCtgryRegTable;
    float totalSum = 0;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return totalSum;
    }
    QSqlQuery query;
    QString queryString = "SELECT SUM(" + defCtgryRegTable.amount +  ") FROM " + defCtgryRegTable.table +
            " WHERE " + defCtgryRegTable.date + " BETWEEN :firstDate AND :secondDate "
            "AND " + defCtgryRegTable.category + " = :categoryId "
            "AND " + defCtgryRegTable.tax + " = :taxId "
            "AND " + defCtgryRegTable.cash + " = :cashId;";
    if (query.prepare(queryString)) {
        query.bindValue(":firstDate", firstDate);
        query.bindValue(":secondDate", secondDate);
        query.bindValue(":categoryId", categoryId);
        query.bindValue(":taxId", taxId);
        query.bindValue(":cashId", cashId);
        if (query.exec()) {
            while (query.next()) {
                totalSum = query.value(0).toFloat();
            }
        }
    }
    return totalSum;
}

float SqlManager::selectTotalSumInPeriodByTaxCash(QString firstDate, QString secondDate,
                                                  int taxId, int cashId)
{
    DefCategoryRegisterTable defCtgryRegTable;
    float totalSum = 0;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return totalSum;
    }
    QSqlQuery query;
    QString queryString = "SELECT SUM(" + defCtgryRegTable.amount +  ") FROM " + defCtgryRegTable.table +
            " WHERE " + defCtgryRegTable.date + " BETWEEN :firstDate AND :secondDate "
            "AND " + defCtgryRegTable.tax + " = :taxId "
            "AND " + defCtgryRegTable.cash + " = :cashId;";
    if (query.prepare(queryString)) {
        query.bindValue(":firstDate", firstDate);
        query.bindValue(":secondDate", secondDate);
        query.bindValue(":taxId", taxId);
        query.bindValue(":cashId", cashId);
        if (query.exec()) {
            while (query.next()) {
                totalSum = query.value(0).toFloat();
            }
        }
    }
    return totalSum;
}

float SqlManager::selectTotalSelfcoastInPeriod(QString firstDate, QString secondDate,
                                               int categoryId, int taxId)
{
    DefCategoryRegisterTable defCtgryRegTable;
    float totalSelfcoast = 0;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return totalSelfcoast;
    }
    QSqlQuery query;
    QString queryString = "SELECT SUM(" + defCtgryRegTable.selfcoast +  ") FROM " + defCtgryRegTable.table +
            " WHERE " + defCtgryRegTable.date + " BETWEEN :firstDate AND :secondDate "
            "AND " + defCtgryRegTable.category + " = :categoryId "
            "AND " + defCtgryRegTable.tax + " = :taxId;";
    if (query.prepare(queryString)) {
        query.bindValue(":firstDate", firstDate);
        query.bindValue(":secondDate", secondDate);
        query.bindValue(":categoryId", categoryId);
        query.bindValue(":taxId", taxId);

        if (query.exec()) {
            while (query.next()) {
                totalSelfcoast = query.value(0).toFloat();
            }
        }
    }
    return totalSelfcoast;
}

float SqlManager::selectTotalSalaryInPeriod(QString firstDate, QString secondDate)
{
    SalaryRegisterTable salaryRegTable;
    float totalSalary = 0;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return totalSalary;
    }
    QSqlQuery query;
    QString queryString = "SELECT SUM(" + salaryRegTable.amount +  ") FROM " + salaryRegTable.table +
            " WHERE " + salaryRegTable.date + " BETWEEN :firstDate AND :secondDate;";
    if (query.prepare(queryString)) {
        query.bindValue(":firstDate", firstDate);
        query.bindValue(":secondDate", secondDate);
        if (query.exec()) {
            while (query.next()) {
                totalSalary = query.value(0).toFloat();
            }
        }
    }
    return totalSalary;
}

float SqlManager::selectTotalCostsInPeriod(QString firstDate, QString secondDate)
{
    CostsRegisterTable costsRegTable;
    float totalCosts = 0;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return totalCosts;
    }
    QSqlQuery query;
    QString queryString = "SELECT SUM(" + costsRegTable.amount +  ") FROM " + costsRegTable.table +
            " WHERE " + costsRegTable.date + " BETWEEN :firstDate AND :secondDate;";
    if (query.prepare(queryString)) {
        query.bindValue(":firstDate", firstDate);
        query.bindValue(":secondDate", secondDate);
        if (query.exec()) {
            while (query.next()) {
                totalCosts = query.value(0).toFloat();
            }
        }
    }
    return totalCosts;
}

int SqlManager::selectMaxCategoryIdInPeriod(QString firstDate, QString secondDate)
{
    DefCategoryRegisterTable defCtgryRegTable;
    int maxCategoryId = 0;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return maxCategoryId;
    }
    QSqlQuery query;
    QString queryString = "SELECT MAX(" + defCtgryRegTable.category +  ") FROM " + defCtgryRegTable.table +
            " WHERE " + defCtgryRegTable.date + " BETWEEN :firstDate AND :secondDate;";
    if (query.prepare(queryString)) {
        query.bindValue(":firstDate", firstDate);
        query.bindValue(":secondDate", secondDate);
        if (query.exec()) {
            while (query.next()) {
                maxCategoryId = query.value(0).toInt();
            }
        }
    }
    return maxCategoryId;
}

QVector<QString> SqlManager::selectTitlesFromTable(QString table, QString title)
{
    QVector <QString> titleVector;
    QSqlQuery query;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return titleVector;
    }
    if (query.exec("SELECT " + title + " FROM " + table)) {
        int ii = 0;
        while (query.next()) {
            titleVector.append(query.value(0).toString());
            ii++;
        }

    }
    else {
        emit signalToStatusBar("Не удалось прочитать");
        qDebug() << "Не удалось прочитать " + title + " из " + table;
    }
    return titleVector;
}

QVector<DefinedCategory> SqlManager::selectDefCategory()
{
    DefCategoriesView defCtgrsView;
    QVector<DefinedCategory> defCategoryVector;
    QSqlQuery query;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return defCategoryVector;
    }
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
    }
    else {
        qDebug() << "Не удалось прочитать " + defCtgrsView.table;
    }
    return defCategoryVector;
}

QVector<DefinedCategoryRecord> SqlManager::selectDefCategoryRecord()
{
    QVector<DefinedCategoryRecord> defCategoryVector;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return defCategoryVector;
    }
    DefCategoriesTable defCtgryTable;
    QSqlQuery query;
    QString queryString = "SELECT * FROM " + defCtgryTable.table +
            ";";
    if (query.prepare(queryString)) {
        if (query.exec()) {
            while (query.next()) {
                DefinedCategoryRecord defCategory;
                int kk = 0;
                defCategory.id = query.value(kk).toInt(); kk++;
                defCategory.categoryId = query.value(kk).toInt(); kk++;
                defCategory.taxId = query.value(kk).toInt(); kk++;
                defCategory.selfcoast = query.value(kk).toInt(); kk++;
                defCategory.koefSalary = query.value(kk).toFloat(); kk++;
                defCategory.koefProfit = query.value(kk).toFloat(); kk++;
                defCategory.isSelling = query.value(kk).toInt(); kk++;
                defCategory.isContracted = query.value(kk).toInt(); kk++;
                defCategoryVector.append(defCategory);
            }
        }
    }
    return defCategoryVector;
}

QVector<DefinedCategoryRecord> SqlManager::selectDefCategoryRecord(QVector<DefCategoryRegisterRecord>
                                                                   defCtgryRegRecordVector)
{
    QVector<DefinedCategoryRecord> defCategoryVector;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return defCategoryVector;
    }
    DefCategoriesTable defCtgryTable;
    for (int ii = 0; ii < defCtgryRegRecordVector.size(); ii++) {
        QSqlQuery query;
        QString queryString = "SELECT * FROM " + defCtgryTable.table +
                " WHERE " + defCtgryTable.title + " = :categoryId" +
                " AND " + defCtgryTable.tax + " = :taxId" +
                ";";
        if (query.prepare(queryString)) {
            query.bindValue(":categoryId", defCtgryRegRecordVector.at(ii).categoryId);
            query.bindValue(":taxId", defCtgryRegRecordVector.at(ii).taxId);
            if (query.exec()) {
                while (query.next()) {
                    DefinedCategoryRecord defCategory;
                    int kk = 0;
                    defCategory.id = query.value(kk).toInt(); kk++;
                    defCategory.categoryId = query.value(kk).toInt(); kk++;
                    defCategory.taxId = query.value(kk).toInt(); kk++;
                    defCategory.selfcoast = query.value(kk).toInt(); kk++;
                    defCategory.koefSalary = query.value(kk).toFloat(); kk++;
                    defCategory.koefProfit = query.value(kk).toFloat(); kk++;
                    defCategory.isSelling = query.value(kk).toInt(); kk++;
                    defCategory.isContracted = query.value(kk).toInt(); kk++;
                    defCategoryVector.append(defCategory);
                }
            }
        }
    }
    return defCategoryVector;
}

QVector<DefCategoryRegisterRecord> SqlManager::selectDefCategoryRegRecord(QString date, int stafferId)
{
    QVector<DefCategoryRegisterRecord> defCtgryRegRecordVector;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return defCtgryRegRecordVector;
    }
    DefCategoryRegisterTable defCtgryRegTable;
    QSqlQuery query;
    QString queryString = "SELECT * FROM " + defCtgryRegTable.table +
                        " WHERE " + defCtgryRegTable.date + " = :requiredDate " +
                        " AND " + defCtgryRegTable.staffer + " = :requiredStafferId" +
                        ";";

    if (query.prepare(queryString)) {
        query.bindValue(":requiredDate", date);
        query.bindValue(":requiredStafferId", stafferId);
        if (query.exec()) {
            while (query.next()) {
                DefCategoryRegisterRecord defCtgryRegRecord;
                int kk = 0;
                defCtgryRegRecord.id = query.value(kk).toInt(); kk++;
                defCtgryRegRecord.date = query.value(kk).toString(); kk++;
                defCtgryRegRecord.stafferId = query.value(kk).toInt(); kk++;
                defCtgryRegRecord.categoryId = query.value(kk).toInt(); kk++;
                defCtgryRegRecord.taxId = query.value(kk).toInt(); kk++;
                defCtgryRegRecord.cashId = query.value(kk).toInt(); kk++;
                defCtgryRegRecord.amount = query.value(kk).toFloat(); kk++;
                defCtgryRegRecord.selfcoast = query.value(kk).toInt(); kk++;
                defCtgryRegRecordVector.append(defCtgryRegRecord);
            }
        }
    }
    else {
        qDebug() << "Selecting error in defcategory_register";
    }
    return defCtgryRegRecordVector;

}

QVector<DefCategoryRegisterRecord> SqlManager::selectDefCategoryRegRecord(QString firstDate, QString secondDate)
{
    QVector<DefCategoryRegisterRecord> defCtgryRegRecordVector;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return defCtgryRegRecordVector;
    }
    DefCategoryRegisterTable defCtgryRegTable;
    QSqlQuery query;
    QString queryString = "SELECT * FROM " + defCtgryRegTable.table +
                        " WHERE " + defCtgryRegTable.date + " BETWEEN :firstDate AND :secondDate"
                        ";";

    if (query.prepare(queryString)) {
        query.bindValue(":firstDate", firstDate);
        query.bindValue(":secondDate", secondDate);
        if (query.exec()) {
            while (query.next()) {
                DefCategoryRegisterRecord defCtgryRegRecord;
                int kk = 0;
                defCtgryRegRecord.id = query.value(kk).toInt(); kk++;
                defCtgryRegRecord.date = query.value(kk).toString(); kk++;
                defCtgryRegRecord.stafferId = query.value(kk).toInt(); kk++;
                defCtgryRegRecord.categoryId = query.value(kk).toInt(); kk++;
                defCtgryRegRecord.taxId = query.value(kk).toInt(); kk++;
                defCtgryRegRecord.cashId = query.value(kk).toInt(); kk++;
                defCtgryRegRecord.amount = query.value(kk).toFloat(); kk++;
                defCtgryRegRecord.selfcoast = query.value(kk).toInt(); kk++;
                defCtgryRegRecordVector.append(defCtgryRegRecord);
            }
        }
    }
    else {
        qDebug() << "Selecting error in defcategory_register";
    }
    return defCtgryRegRecordVector;
}

QVector<DefCategoryRegisterRecordView> SqlManager::selectDefCategoryRegRecordView(QString firstDate,
                                                                                  QString secondDate)
{
    DefCategoryRegisterView defCtgryRegView;
    QVector<DefCategoryRegisterRecordView> defCtgryRegRecViewVector;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return defCtgryRegRecViewVector;
    }
    QSqlQuery query;
    QString queryString = "SELECT * FROM " + defCtgryRegView.table +
            " WHERE " + defCtgryRegView.date + " BETWEEN :firstDate AND :secondDate ORDER BY " +
            defCtgryRegView.date +
            ";";
    if (query.prepare(queryString)) {
        query.bindValue(":firstDate", firstDate);
        query.bindValue(":secondDate", secondDate);
        if (query.exec()) {
            while (query.next()) {
                DefCategoryRegisterRecordView defCtgryRegRecView;
                int kk = 0;
                defCtgryRegRecView.id = query.value(kk).toInt(); kk++;
                defCtgryRegRecView.date = query.value(kk).toString(); kk++;
                defCtgryRegRecView.staffer = query.value(kk).toString(); kk++;
                defCtgryRegRecView.category = query.value(kk).toString(); kk++;
                defCtgryRegRecView.tax = query.value(kk).toString(); kk++;
                defCtgryRegRecView.cash = query.value(kk).toString(); kk++;
                defCtgryRegRecView.amount = query.value(kk).toFloat(); kk++;
                defCtgryRegRecView.selfcoast = query.value(kk).toFloat(); kk++;
                defCtgryRegRecViewVector.append(defCtgryRegRecView);
            }
        }
    }
    return defCtgryRegRecViewVector;
}

QStringList SqlManager::selectStafferFromDefCategoryRegRecord(QString firstDate, QString secondDate)
{
    DefCategoryRegisterView defCtgryRegView;
    QStringList stafferNames;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return stafferNames;
    }
    QSqlQuery query;
    QString queryString = "SELECT DISTINCT " + defCtgryRegView.staffer + " FROM " + defCtgryRegView.table +
            " WHERE " + defCtgryRegView.date + " BETWEEN :firstDate AND :secondDate;";
    if (query.prepare(queryString)) {
        query.bindValue(":firstDate", firstDate);
        query.bindValue(":secondDate", secondDate);
        if (query.exec()) {
            while (query.next()) {
                stafferNames.append(query.value(defCtgryRegView.staffer).toString());
            }
        }
    }
    return stafferNames;
}

QVector<SalaryRegisterRecordView> SqlManager::selectSalaryRegRecordView(QString firstDate, QString secondDate)
{
    SalaryRegisterView salaryRegView;
    QVector<SalaryRegisterRecordView> salaryRegRecViewVector;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return salaryRegRecViewVector;
    }
    QSqlQuery query;
    QString queryString = "SELECT * FROM " + salaryRegView.table +
            " WHERE " + salaryRegView.date + " BETWEEN :firstDate AND :secondDate ORDER BY " +
            salaryRegView.date +
            ";";
    if (query.prepare(queryString)) {
        query.bindValue(":firstDate", firstDate);
        query.bindValue(":secondDate", secondDate);
        if (query.exec()) {
            while (query.next()) {
                SalaryRegisterRecordView salaryRegRecView;
                int kk = 0;
                salaryRegRecView.id = query.value(kk).toInt(); kk++;
                salaryRegRecView.date = query.value(kk).toString(); kk++;
                salaryRegRecView.staffer = query.value(kk).toString(); kk++;
                salaryRegRecView.amount = query.value(kk).toFloat(); kk++;
                salaryRegRecView.basicWage = query.value(kk).toFloat(); kk++;
                salaryRegRecView.koefBasicWage = query.value(kk).toFloat(); kk++;
                salaryRegRecViewVector.append(salaryRegRecView);
            }
        }
    }
    return salaryRegRecViewVector;
}

QVector<CostsRegisterRecordView> SqlManager::selectCostsRegRecordView(QString firstDate, QString secondDate)
{
    CostsRegisterView costsRegView;
    QVector<CostsRegisterRecordView> costsRegRecViewVector;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return costsRegRecViewVector;
    }
    QSqlQuery query;
    QString queryString = "SELECT * FROM " + costsRegView.table +
            " WHERE " + costsRegView.date + " BETWEEN :firstDate AND :secondDate ORDER BY " +
            costsRegView.date +
            ";";
    if (query.prepare(queryString)) {
        query.bindValue(":firstDate", firstDate);
        query.bindValue(":secondDate", secondDate);
        if (query.exec()) {
            while (query.next()) {
                CostsRegisterRecordView costsRegRecView;
                int kk = 0;
                costsRegRecView.id = query.value(kk).toInt(); kk++;
                costsRegRecView.date = query.value(kk).toString(); kk++;
                costsRegRecView.cash = query.value(kk).toString(); kk++;
                costsRegRecView.amount = query.value(kk).toFloat(); kk++;
                costsRegRecView.description = query.value(kk).toString(); kk++;
                costsRegRecViewVector.append(costsRegRecView);
            }
        }
    }
    return costsRegRecViewVector;
}

SalaryRegisterRecord SqlManager::selectSalaryRecord(QString date, int stafferId)
{
    SalaryRegisterRecord salaryRegRecord;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return salaryRegRecord;
    }
    SalaryRegisterTable salaryRegTable;
    QSqlQuery query;
    QString queryStrigng = "SELECT * FROM " + salaryRegTable.table +
                        " WHERE " + salaryRegTable.date + " = :requiredDate " +
                        " AND " + salaryRegTable.staffer + " = :requiredStafferId" +
                        ";";

    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return salaryRegRecord;
    }
    if (query.prepare(queryStrigng)) {
        query.bindValue(":requiredDate", date);
        query.bindValue(":requiredStafferId", stafferId);
        if (query.exec()) {
            while (query.next()) {
                int kk = 0;
                salaryRegRecord.id = query.value(kk).toInt(); kk++;
                salaryRegRecord.date = query.value(kk).toString(); kk++;
                salaryRegRecord.stafferId = query.value(kk).toInt(); kk++;
                salaryRegRecord.amount = query.value(kk).toFloat(); kk++;
                salaryRegRecord.basicWage = query.value(kk).toFloat(); kk++;
                salaryRegRecord.koefBasicWage = query.value(kk).toFloat(); kk++;
            }
        }
    }
    else {
        qDebug() << "Selecting error in salary_register";
    }
    return salaryRegRecord;
}

int SqlManager::selectIdFromTable(QString table, QString tableId, QString tableTitle, QString requiredTitle)
{
    int id = -1;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return id;
    }
    QSqlQuery query;
    QString queryStrigng = "SELECT " + tableId + " FROM " + table +
            " WHERE " + tableTitle + " = :requiredTitle;";
    if (query.prepare(queryStrigng)) {
        query.bindValue(":requiredTitle", requiredTitle);
        if (query.exec()) {
            while (query.next()) {
                id = query.value(0).toInt();
            }
        }
    }
    else {
        qDebug() << "Не удалось найти id для " + tableTitle + " из " + table;
    }
    return id;
}

bool SqlManager::deleteInDefCategoryRegisterTable(int requiredId)
{
    bool result = false;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return result;
    }
    DefCategoryRegisterTable defCtgryRegTable;
    QSqlQuery query;
    QString queryString = "DELETE FROM " + defCtgryRegTable.table +
                        " WHERE " + defCtgryRegTable.id + " = :requiredId" +
                        ";";

    if (query.prepare(queryString)) {
        query.bindValue(":requiredId", requiredId);
        if (query.exec()) {
            result = true;
        }
        else {
            qDebug() << "Deleting from defcategory_register is enabled";
        }
    }
    return result;
}

bool SqlManager::deleteInCostsRegisterTable(int requiredId)
{
    bool result = false;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return result;
    }
    CostsRegisterTable costsRegTable;
    QSqlQuery query;
    QString queryString = "DELETE FROM " + costsRegTable.table +
                        " WHERE " + costsRegTable.id + " = :requiredId" +
                        ";";

    if (query.prepare(queryString)) {
        query.bindValue(":requiredId", requiredId);
        if (query.exec()) {
            result = true;
        }
        else {
            qDebug() << "Deleting from defcategory_register is enabled";
        }
    }
    return result;
}

bool SqlManager::isSalaryRecordExist(SalaryRegisterRecord salaryRegRecord)
{
    SalaryRegisterTable salaryRegTable;
    bool isRecordExist = false;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return isRecordExist;
    }
    QSqlQuery query;
    QString queryStrigng = "SELECT " + salaryRegTable.date + ", " + salaryRegTable.staffer +
            " FROM " + salaryRegTable.table +
            " WHERE " + salaryRegTable.date + " = :requiredDate "
            "AND " + salaryRegTable.staffer + " = :requiredStafferId;";
    if (query.prepare(queryStrigng)) {
        query.bindValue(":requiredDate", salaryRegRecord.date);
        query.bindValue(":requiredStafferId", salaryRegRecord.stafferId);
        if (query.exec()) {
            while (query.next()) {
                isRecordExist = true;
            }
        }
    }
    else {
        //emit signalToStatusBar("Не удалось прочитать");
        qDebug() << "Не удалось выполнить запрос выборе существующей записи в реестре зарплат";
    }
    return isRecordExist;
}

int SqlManager::isDefCategoryRecordExist(int categoryId, int taxId)
{
    DefCategoriesTable defCategoriesTable;
    bool isRecordExist = false;
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return isRecordExist;
    }
    QSqlQuery query;
    QString queryStrigng = "SELECT " + defCategoriesTable.id +
            " FROM " + defCategoriesTable.table +
            " WHERE " + defCategoriesTable.title + " = :categoryId "
            "AND " + defCategoriesTable.tax + " = :taxId;";
    if (query.prepare(queryStrigng)) {
        query.bindValue(":categoryId", categoryId);
        query.bindValue(":taxId", taxId);
        if (query.exec()) {
            while (query.next()) {
                isRecordExist = true;
            }
        }
    }
    else {
        qDebug() << "Не удалось выполнить запрос выборе существующей записи в реестре зарплат";
    }
    return isRecordExist;
}

bool SqlManager::isDatabaseOpen()
{
    if (dataBase.isOpen())
        return true;
    else
        return false;
}

bool SqlManager::openDataBase(QString databaseFile)
{
    dataBase.setDatabaseName(databaseFile);
    if (dataBase.isValid()) {
        if(dataBase.open()) {
            m_defCategoryTable = new QSqlTableModel(this, dataBase);
            QSqlQuery query;
            query.exec("PRAGMA foreign_keys = on;");
            return true;
        } else {
            emit signalToStatusBar("База данных не открыта");
            return false;
        }
    }
    else {
        emit signalToStatusBar("База данных не валидна");
        return false;
    }
}

bool SqlManager::createDataBase(QString databaseFile)
{
    if (this->openDataBase(databaseFile)) {
        return (this->createTables()) ? true : false;
    } else {
        emit signalToStatusBar("База данных не удалось открыть");
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


QSqlTableModel *SqlManager::getDefCategoryTable()
{
    return m_defCategoryTable;
}

bool SqlManager::setDefCategoryModel()
{
    bool result = false;
    DefCategoriesView defCtgrsView;
    m_defCategoryTable->setTable(defCtgrsView.table);
    m_defCategoryTable->setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (m_defCategoryTable->select()) {
        emit signalToStatusBar("Модель выбрана");
        result = true;
    }
    else {
        emit signalToStatusBar("Модель не выбрана");
    }
    return result;
}

bool SqlManager::createTables()
{
    bool result = true;
    StaffersTable staffersTable;
    CategoriesTable categoriesTable;
    TaxesTable taxesTable;
    CashTable cashTable;
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
    if (!this->createDefCategoriesTable())
        result = false;
    if (!this->createDefCategoriesView())
        result = false;
    if (!this->createDefCategoryRegisterTable())
        result = false;
    if (!this->createDefCategoryRegisterView())
        result = false;
    if (!this->createCostsRegisterTable())
        result = false;
    if (!this->createCostsRegisterView())
        result = false;
    if (!this->createSalaryRegisterTable())
        result = false;
    if (!this->createSalaryRegisterView())
        result = false;
    return result;
}

bool SqlManager::createOneTitleTable(QString table, QString id, QString title)
{
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return false;
    }
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

bool SqlManager::createDefCategoriesTable()
{
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return false;
    }
    DefCategoriesTable defCtgryTable;
    CategoriesTable ctgryTable;
    TaxesTable taxesTable;
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

bool SqlManager::createDefCategoriesView()
{
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return false;
    }
    DefCategoriesView defCtgryView;
    DefCategoriesTable defCtgryTable;
    CategoriesTable ctgryTable;
    TaxesTable taxesTable;
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

bool SqlManager::createDefCategoryRegisterTable()
{
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return false;
    }
    DefCategoryRegisterTable defCtgryRegTable;
    StaffersTable staffersTable;
    CategoriesTable ctgryTable;
    TaxesTable taxesTable;
    CashTable cashTable;
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

bool SqlManager::createDefCategoryRegisterView()
{
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return false;
    }
    DefCategoryRegisterView defCtgryRegView;
    DefCategoryRegisterTable defCtgryRegTable;
    StaffersTable staffersTable;
    CategoriesTable ctgryTable;
    TaxesTable taxesTable;
    CashTable cashTable;
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

bool SqlManager::createCostsRegisterTable()
{
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return false;
    }
    CostsRegisterTable costsRegTable;
    CashTable cashTable;
    QSqlQuery query;
    QString queryString = "CREATE TABLE " + costsRegTable.table + " (" +
            costsRegTable.id + " INTEGER PRIMARY KEY AUTOINCREMENT, " +
            costsRegTable.date + " TEXT NOT NULL, " +
            costsRegTable.cash + " INTEGER NOT NULL, " +
            costsRegTable.amount + " REAL NOT NULL, " +
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

bool SqlManager::createCostsRegisterView()
{
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return false;
    }
    CostsRegisterView costsRegView;
    CostsRegisterTable costsRegTable;
    CashTable cashTable;
    QSqlQuery query;
    QString queryString = "CREATE VIEW " + costsRegView.table + " AS "
            "SELECT " + costsRegTable.table + "." + costsRegTable.id + " AS " + costsRegView.id + ", " +
            costsRegTable.table + "." + costsRegTable.date + " AS " + costsRegView.date + ", " +
            cashTable.table + "." + cashTable.title+ " AS " + costsRegView.cash + ", " +
            costsRegTable.table + "." + costsRegTable.amount + " AS " + costsRegView.amount + ", " +
            costsRegTable.table + "." + costsRegTable.description + " AS " + costsRegView.description +
            " FROM " + costsRegTable.table + ", " +
            cashTable.table +
            " WHERE " + costsRegTable.table + "." + costsRegTable.cash + " = " +
            cashTable.table + "." + cashTable.id +
            ";";
    if (query.exec(queryString)) {
        return true;
    }
    else
        return false;
}

bool SqlManager::createSalaryRegisterTable()
{
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return false;
    }
    SalaryRegisterTable salaryRegTable;
    StaffersTable staffersTable;
    QSqlQuery query;
    QString queryString = "CREATE TABLE " + salaryRegTable.table + " (" +
            salaryRegTable.id + " INTEGER PRIMARY KEY AUTOINCREMENT, " +
            salaryRegTable.date + " TEXT NOT NULL, " +
            salaryRegTable.staffer+ " INTEGER NOT NULL, " +
            salaryRegTable.amount + " REAL NOT NULL, " +
            salaryRegTable.basicWage + " REAL NOT NULL, " +
            salaryRegTable.koefBasicWage + " REAL NOT NULL, " +
            "FOREIGN KEY (" + salaryRegTable.staffer + ") REFERENCES " + staffersTable.table + "(" +
            staffersTable.id + ") ON DELETE CASCADE" +
            ");";
    if (query.exec(queryString)) {
        return true;
    }
    else
        return false;
}

bool SqlManager::createSalaryRegisterView()
{
    if (!dataBase.isOpen()) {
        emit signalToStatusBar("База данных не открыта");
        return false;
    }
    SalaryRegisterView salaryRegView;
    SalaryRegisterTable salaryRegTable;
    StaffersTable staffersTable;
    QSqlQuery query;
    QString queryString = "CREATE VIEW " + salaryRegView.table + " AS "
            "SELECT " + salaryRegTable.table + "." + salaryRegTable.id + " AS " + salaryRegView.id + ", " +
            salaryRegTable.table + "." + salaryRegTable.date + " AS " + salaryRegTable.date + ", " +
            staffersTable.table + "." + staffersTable.name + " AS " + salaryRegView.staffer + ", " +
            salaryRegTable.table + "." + salaryRegTable.amount + " AS " + salaryRegView.amount + ", " +
            salaryRegTable.table + "." + salaryRegTable.basicWage + " AS " + salaryRegView.basicWage + ", " +
            salaryRegTable.table + "." + salaryRegTable.koefBasicWage + " AS " + salaryRegView.koefBasicWage +
            " FROM " + salaryRegTable.table + ", " +
            staffersTable.table +
            " WHERE " + salaryRegTable.table + "." + salaryRegTable.staffer + " = " +
            staffersTable.table + "." + staffersTable.id +
            ";";
    if (query.exec(queryString)) {
        return true;
    }
    else
        return false;
}

