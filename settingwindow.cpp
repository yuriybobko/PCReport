#include "settingwindow.h"
#include "ui_settingwindow.h"
#include "mainwindow.h"

StaffersTable staffersTable;
CategoriesTable categoriesTable;
TaxesTable taxesTable;
CashTable cashTable;
DefCategoriesView defCategoriesView;
DefCategoriesTable defCategoriesTable;
DefCategoryRegisterView defCategoryRegisterView;
DefCategoryRegisterTable defCategoryRegisterTable;

SettingWindow::SettingWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingWindow)
{
    ui->setupUi(this);

    m_settingFilePath = QApplication::applicationDirPath() + SETTING_FILE;

    this->readSettings();

    ui->EditDataBaseDir->setText(dataBaseFile);
    ui->EditBasicWage->setText(QString::number(basicWage));
    ui->EditBasicWage->setValidator(new QDoubleValidator(0, 1E+12, 2, this));
    ui->EditBasicWageKoefSalary->setText(QString::number(basicWageKoefSalary));
    ui->EditBasicWageKoefSalary->setValidator(new QDoubleValidator(0, 1E+12, 2, this));


    this->openDataBase(dataBaseFile);

    SetFileAttributesW(m_settingFilePath.toStdWString().c_str(), FILE_ATTRIBUTE_HIDDEN);


    connect(ui->BtnCancel, SIGNAL(clicked()), this, SLOT(cancelSettings()));
    connect(ui->BtnOk, SIGNAL(clicked()), this, SLOT(setSettings()));
    connect(ui->BtnFindDataBaseDir, &QPushButton::clicked, [this]() {
        QString stringFile = QFileDialog::getOpenFileName(
                            this,
                            tr("Выбрать базу данных"),
                            QDir::currentPath(),
                            "*.db" );
        if (stringFile != nullptr) {
            ui->EditDataBaseDir->setText(stringFile);
        }
    });
    connect(ui->BtnOpenDataBase, &QPushButton::clicked, [this]() {
        if (dataBaseFile != ui->EditDataBaseDir->text())
            this->openDataBase(ui->EditDataBaseDir->text());
    });

    // Настройки категорий и сотрудников
    connect(ui->BtnAddStaffer, SIGNAL(clicked()), this, SLOT(addStaffer()));
    connect(ui->BtnAddCategory, SIGNAL(clicked()), this, SLOT(addCategory()));
    connect(ui->BtnAddTax, SIGNAL(clicked()), this, SLOT(addTax()));
    connect(ui->BtnRemoveStaffer, SIGNAL(clicked()), this, SLOT(removeStaffer()));
    connect(ui->BtnRemoveCategory, SIGNAL(clicked()), this, SLOT(removeCategory()));
    connect(ui->BtnRemoveTax, SIGNAL(clicked()), this, SLOT(removeTax()));

    connect(ui->BtnAddDefCategory, SIGNAL(clicked()), this, SLOT(addDefCategory()));


    this->hide();
}

SettingWindow::~SettingWindow()
{
    delete ui;
}

void SettingWindow::readSettings()
{
    QSettings setting(m_settingFilePath, QSettings::IniFormat);
    setting.beginGroup("DATA_BASE_DIRECTORY");
    dataBaseFile = setting.value("dataBaseFile",
                                 QApplication::applicationDirPath() + DEFAULT_DATABASE_FILE).toString();
    setting.endGroup();
    setting.beginGroup("SALARY_PARAMETERS");
    basicWage = setting.value("basicWage", DEFAULT_BASIC_WAGE).toFloat();
    basicWageKoefSalary = setting.value("basicWageKoefSalary", DEFAULT_BASIC_WAGE_KOEF_SALARY).toFloat();
    setting.endGroup();
}

void SettingWindow::writeSettings()
{
    QSettings setting(m_settingFilePath, QSettings::IniFormat);
    setting.clear();
    setting.beginGroup("DATA_BASE_DIRECTORY");
    setting.setValue("dataBaseFile", dataBaseFile);
    setting.endGroup();
    setting.beginGroup("SALARY_PARAMETERS");
    setting.setValue("basicWage", basicWage);
    setting.setValue("basicWageKoefSalary", basicWageKoefSalary);
    setting.endGroup();
}

void SettingWindow::setSettings()
{
    if (this->isSettingChanged()) {
        bool btnNo = QMessageBox::information(0,
                                        "Внимание",
                                        "Сохранить изменения?",
                                        "Да",
                                        "Нет",
                                        QString(),
                                        0,
                                        1);
        if (!btnNo) {
            dataBaseFile = ui->EditDataBaseDir->text();
            basicWage = ui->EditBasicWage->text().toFloat();
            this->writeSettings();
            this->setStaffer();
            this->setCategory();
            this->setTax();
            this->setDefCategory();
            this->hide();
        }
        else {
            this->cancelSettings();
        }
    }
    else {
        this->hide();
    }
}

void SettingWindow::cancelSettings()
{
    ui->CmbBoxCategory->clear();
    for (int ii = 0; ii < m_category.size(); ii++) {
        ui->CmbBoxCategory->addItem(m_category.at(ii));
    }
    ui->CmbBoxStaffer->clear();
    for (int ii = 0; ii < m_staffer.size(); ii++) {
        ui->CmbBoxStaffer->addItem(m_staffer.at(ii));
    }
    ui->CmbBoxTax->clear();
    for (int ii = 0; ii < m_tax.size(); ii++) {
        ui->CmbBoxTax->addItem(m_tax.at(ii));
    }
    this->editTableDefCategory();
    this->hide();
}

void SettingWindow::openDataBase(const QString dbFile)
{
    this->connectToDataBase(dbFile);
    this->editWidgets();
}

void SettingWindow::connectToDataBase(QString dbFile)
{
    sqlManager.connectToDataBase(dbFile);
    m_staffer = sqlManager.selectTitlesFromTable(staffersTable.table, staffersTable.name);
    m_category = sqlManager.selectTitlesFromTable(categoriesTable.table, categoriesTable.title);
    m_tax = sqlManager.selectTitlesFromTable(taxesTable.table, taxesTable.title);
    m_defCategory = sqlManager.selectDefCategory(defCategoriesView);
}

void SettingWindow::editWidgets()
{
//    dataBaseFile = ui->EditDataBaseDir->text();

    ui->CmbBoxStaffer->clear();
    for (int ii = 0; ii < m_staffer.size(); ii++) {
        ui->CmbBoxStaffer->addItem(m_staffer.at(ii));
    }
    ui->CmbBoxCategory->clear();
    for (int ii = 0; ii < m_category.size(); ii++) {
        ui->CmbBoxCategory->addItem(m_category.at(ii));
    }
    ui->CmbBoxTax->clear();
    for (int ii = 0; ii < m_tax.size(); ii++) {
        ui->CmbBoxTax->addItem(m_tax.at(ii));
    }
    this->editTableDefCategory();
    emit this->signalToEditStaffer(m_staffer);
    emit this->signalToEditFormBtnCategory(m_defCategory);
}

bool SettingWindow::isEditChanged()
{
    if (ui->EditDataBaseDir->text() != dataBaseFile) {
        return true;
    }
    else
        return false;
}

bool SettingWindow::isStafferChanged()
{
    if (ui->CmbBoxStaffer->count() != m_staffer.size()) {
        return true;
    }
    else {
        for (int ii = 0; ii < m_staffer.size(); ii++) {
            if (m_staffer.at(ii) != ui->CmbBoxStaffer->itemText(ii))
                return true;
        }
    }
    return false;
}

bool SettingWindow::isCategoryChanged()
{
    if (ui->CmbBoxCategory->count() != m_category.size()) {
        return true;
    }
    else {
        for (int ii = 0; ii < m_category.size(); ii++) {
            if (m_category.at(ii) != ui->CmbBoxCategory->itemText(ii))
                return true;
        }
    }
    return false;
}

bool SettingWindow::isTaxChanged()
{
    if (ui->CmbBoxTax->count() != m_tax.size()) {
        return true;
    }
    else
    {
        for (int ii = 0; ii < m_tax.size(); ii++) {
            if (m_tax.at(ii) != ui->CmbBoxTax->itemText(ii))
                return true;
        }
    }
    return false;
}

bool SettingWindow::isDefCategoryChanged()
{
    if (m_defCategory.size() != m_defCategoryModel->rowCount()) {
        return true;
    }
    else {
        for (int row = 0; row < m_defCategoryModel->rowCount(); row++) {
            int column = 0;
            if (m_defCategory.at(row).categoryTitle != m_defCategoryModel->item(row, column)->text()) {
                return true;
            }
            column++;
            if (m_defCategory.at(row).taxTitle != m_defCategoryModel->item(row, column)->text()) {
                return true;
            }
            column++;
            if ((int)m_defCategory.at(row).selfcoast !=
                    m_defCategoryModel->item(row, column)->text().toInt()) {
                return true;
            }
            column++;
            if (m_defCategory.at(row).koefSalary !=
                    m_defCategoryModel->item(row, column)->text().toFloat()) {
                return true;
            }
            column++;
            if (m_defCategory.at(row).koefProfit !=
                    m_defCategoryModel->item(row, column)->text().toFloat()) {
                return true;
            }
            column++;
            if ((int)m_defCategory.at(row).isSelling !=
                    m_defCategoryModel->item(row, column)->text().toInt()) {
                return true;
            }
            column++;
            if ((int)m_defCategory.at(row).isContracted !=
                    m_defCategoryModel->item(row, column)->text().toInt()) {
                return true;
            }
        }
    }
    return false;
}

bool SettingWindow::isSettingChanged()
{
    bool anyChnanges = false;
    if (this->isEditChanged()) {
        anyChnanges = true;
    }
    if (this->isStafferChanged()) {
        anyChnanges = true;
    }
    if (this->isCategoryChanged()) {
        anyChnanges = true;
    }
    if (this->isTaxChanged()) {
        anyChnanges = true;
    }
    if (this->isDefCategoryChanged()) {
        anyChnanges = true;
    }
    return anyChnanges;
}

void SettingWindow::addCategory()
{
    bool btnOk = false;
    QString strCategory = QInputDialog::getText(this,
                                               "Новая категория",
                                               "Категория:",
                                               QLineEdit::Normal,
                                               "Категория 1",
                                               &btnOk,
                                               Qt::WindowTitleHint);
    if (btnOk) {
        if (!this->isCmbBoxItemExist(ui->CmbBoxCategory, strCategory))
            ui->CmbBoxCategory->addItem(strCategory);
        else
            QMessageBox::warning(0, "Внимание", "Данное поле уже существует");
    }
}

void SettingWindow::removeCategory()
{
    int btnNo = QMessageBox::warning(0,
                                    "Внимание",
                                    "Удалить запись?",
                                    "Да",
                                    "Нет",
                                    QString(),
                                    0,
                                    1);
    if (!btnNo)
        ui->CmbBoxCategory->removeItem(ui->CmbBoxCategory->currentIndex());
}

QVector<QString> SettingWindow::getCategory()
{
    return m_category;
}

void SettingWindow::setCategory()
{
    m_category.clear();
    for (int ii = 0; ii < ui->CmbBoxCategory->count(); ii++) {
        m_category.append(ui->CmbBoxCategory->itemText(ii));
        sqlManager.insertIntoOneTitleTable(categoriesTable.table, categoriesTable.title, ui->CmbBoxCategory->itemText(ii));
    }
}

void SettingWindow::addStaffer()
{
    bool btnOk = false;
    QString strStaffer= QInputDialog::getText(this,
                                               "Новый сотрудник",
                                               "Имя сотрудника:",
                                               QLineEdit::Normal,
                                               "ФИО 1",
                                               &btnOk,
                                               Qt::WindowTitleHint);
    if (btnOk) {
        if (!this->isCmbBoxItemExist(ui->CmbBoxStaffer, strStaffer))
            ui->CmbBoxStaffer->addItem(strStaffer);
        else
            QMessageBox::warning(0, "Внимание", "Данное поле уже существует");
    }
}

void SettingWindow::removeStaffer()
{
    int btnNo = QMessageBox::warning(0,
                                    "Внимание",
                                    "Удалить запись?",
                                    "Да",
                                    "Нет",
                                    QString(),
                                    0,
                                    1);
    if (!btnNo) {
        ui->CmbBoxStaffer->removeItem(ui->CmbBoxStaffer->currentIndex());
    }
}

QVector<QString> SettingWindow::getStaffer()
{
    return m_staffer;
}

void SettingWindow::setStaffer()
{

    m_staffer.clear();
    for (int ii = 0; ii < ui->CmbBoxStaffer->count(); ii++) {
        m_staffer.append(ui->CmbBoxStaffer->itemText(ii));
        sqlManager.insertIntoOneTitleTable(staffersTable.table, staffersTable.name, ui->CmbBoxStaffer->itemText(ii));
    }
    emit signalToEditStaffer(m_staffer);
}

void SettingWindow::addTax()
{
    bool btnOk = false;
    QString strTax = QInputDialog::getText(this,
                                               "Новый вид налога",
                                               "Налог:",
                                               QLineEdit::Normal,
                                               "Налог 1",
                                               &btnOk,
                                               Qt::WindowTitleHint);
    if (btnOk) {
        if (!this->isCmbBoxItemExist(ui->CmbBoxTax, strTax))
            ui->CmbBoxTax->addItem(strTax);
        else
            QMessageBox::warning(0, "Внимание", "Данное поле уже существует");
    }
}

void SettingWindow::removeTax()
{
    int btnNo = QMessageBox::warning(0,
                                    "Внимание",
                                    "Удалить запись?",
                                    "Да",
                                    "Нет",
                                    QString(),
                                    0,
                                    1);
    if (!btnNo)
        ui->CmbBoxTax->removeItem(ui->CmbBoxTax->currentIndex());
}

QVector<QString> SettingWindow::getTax()
{
    return m_tax;
}

void SettingWindow::setTax()
{
    m_tax.clear();
    for (int ii = 0; ii < ui->CmbBoxTax->count(); ii++) {
        m_tax.append(ui->CmbBoxTax->itemText(ii));
        sqlManager.insertIntoOneTitleTable(taxesTable.table, taxesTable.title, ui->CmbBoxTax->itemText(ii));
    }
}

bool SettingWindow::isCmbBoxItemExist(QComboBox *cmbBox, QString checkedString)
{
    for (int ii = 0; ii < cmbBox->count(); ii++) {
        if (checkedString == cmbBox->itemText(ii))
            return true;
    }
    return false;
}

void SettingWindow::addDefCategory()
{
    bool btnOk = false;
    QStringList categoryCmbBoxList;
    for (int ii = 0; ii < ui->CmbBoxCategory->count(); ii++) {
        categoryCmbBoxList.append(ui->CmbBoxCategory->itemText(ii));
    }
    QStringList taxCmbBoxList;
    for (int ii = 0; ii < ui->CmbBoxTax->count(); ii++) {
        taxCmbBoxList.append(ui->CmbBoxTax->itemText(ii));
    }
    DefinedCategory defCategory = AddDefCategoryDialog::getDefCategory(this, categoryCmbBoxList,
                                                                       taxCmbBoxList, &btnOk);
    if (btnOk) {
        if (!this->isDefCategoryExist(m_defCategoryModel, defCategory))
            this->addRowInTableDefCategory(defCategory);
        else
            QMessageBox::warning(0, "Внимание", "Данное поле уже существует");
    }
}

void SettingWindow::removeDefCategory()
{

}

QVector<DefinedCategory> SettingWindow::getDefCategory()
{
    return m_defCategory;
}

void SettingWindow::setDefCategory()
{
    m_defCategory.clear();
    DefinedCategory defCtgry;
    for (int row = 0; row < m_defCategoryModel->rowCount(); row++) {
        int column = 0;
        defCtgry.categoryTitle = m_defCategoryModel->item(row, column)->text();
        column++;
        defCtgry.taxTitle = m_defCategoryModel->item(row, column)->text();
        column++;
        defCtgry.selfcoast = (bool)(m_defCategoryModel->item(row, column)->text().toInt());
        column++;
        defCtgry.koefSalary = m_defCategoryModel->item(row, column)->text().toFloat();
        column++;
        defCtgry.koefProfit = m_defCategoryModel->item(row, column)->text().toFloat();
        column++;
        defCtgry.isSelling = m_defCategoryModel->item(row, column)->text().toInt();
        column++;
        defCtgry.isContracted = m_defCategoryModel->item(row, column)->text().toInt();
        m_defCategory.append(defCtgry);

        int categoryId = sqlManager.selectIdFromTable(categoriesTable.table, categoriesTable.id,
                                                    categoriesTable.title, defCtgry.categoryTitle);
        int taxId = sqlManager.selectIdFromTable(taxesTable.table, taxesTable.id,
                                                    taxesTable.title, defCtgry.taxTitle);
//        sqlManager.insertIntoDefCategoryTable(defCategoriesTable, categoryId, taxId, (int) defCtgry.selfcoast,
//                                              defCtgry.koefSalary, defCtgry.koefProfit);
        sqlManager.insertIntoDefCategoryTable(defCategoriesTable, categoryId, taxId, defCtgry);
    }
    emit signalToEditFormBtnCategory(m_defCategory);
}

bool SettingWindow::isDefCategoryExist(QStandardItemModel *itemModel, DefinedCategory checkedDefCtgry)
{
    for (int row = 0; row < itemModel->rowCount(); row++) {
        int column = 0;
        if (checkedDefCtgry.categoryTitle == itemModel->item(row, column)->text()) {
            column++;
            if (checkedDefCtgry.taxTitle == itemModel->item(row, column)->text())
                return true;
        }
    }
    return false;
}

void SettingWindow::addRowInTableDefCategory(DefinedCategory defCtgry)
{
    m_defCategoryModel->setRowCount(m_defCategoryModel->rowCount() + 1);
    int row = m_defCategoryModel->rowCount() - 1;
    int column = 0;
    QStandardItem *newItem0 = new QStandardItem(defCtgry.categoryTitle);
    m_defCategoryModel->setItem(row, column, newItem0);
    column++;
    QStandardItem *newItem1 = new QStandardItem(defCtgry.taxTitle);
    m_defCategoryModel->setItem(row, column, newItem1);
    column++;
    QStandardItem *newItem2 = new QStandardItem(QString::number(defCtgry.selfcoast));
    m_defCategoryModel->setItem(row, column, newItem2);
    column++;
    QStandardItem *newItem3 = new QStandardItem(QString::number(defCtgry.koefSalary));
    m_defCategoryModel->setItem(row, column, newItem3);
    column++;
    QStandardItem *newItem4 = new QStandardItem(QString::number(defCtgry.koefProfit));
    m_defCategoryModel->setItem(row, column, newItem4);
    column++;
    QStandardItem *newItem5 = new QStandardItem(QString::number(defCtgry.isSelling));
    m_defCategoryModel->setItem(row, column, newItem5);
    column++;
    QStandardItem *newItem6 = new QStandardItem(QString::number(defCtgry.isContracted));
    m_defCategoryModel->setItem(row, column, newItem6);
    column++;
}

void SettingWindow::removeRowInTableDefCategory(int selectedRow)
{
    m_defCategoryModel->removeRow(selectedRow);
    ui->TableViewDefCategory->repaint();
}

void SettingWindow::editTableDefCategory()
{
    ui->TableViewDefCategory->clearSpans();
    if (m_defCategoryModel != nullptr)
        delete m_defCategoryModel;
    //m_defCategoryModel = new QStandardItemModel(m_defCategory.size(), 5, ui->TableViewDefCategory);
    m_defCategoryModel = new QStandardItemModel(0, 7, ui->TableViewDefCategory);
    ui->TableViewDefCategory->setModel(m_defCategoryModel);
    int column = 0;
    for(int row = 0; row != m_defCategory.size(); ++row) {
       this->addRowInTableDefCategory(m_defCategory.at(row));
    }
    ui->TableViewDefCategory->setColumnWidth(column, 80);
    m_defCategoryModel->setHeaderData(column, Qt::Horizontal, "Категория"); column++;
    ui->TableViewDefCategory->setColumnWidth(column, 40);
    m_defCategoryModel->setHeaderData(column, Qt::Horizontal, "Налог"); column++;
    m_defCategoryModel->setHeaderData(column, Qt::Horizontal, "Себестоимость"); column++;
    m_defCategoryModel->setHeaderData(column, Qt::Horizontal, "Коэф. зарплаты"); column++;
    m_defCategoryModel->setHeaderData(column, Qt::Horizontal, "Коэф. прибыли"); column++;
    ui->TableViewDefCategory->setColumnWidth(column, 70);
    m_defCategoryModel->setHeaderData(column, Qt::Horizontal, "Продажа"); column++;
    ui->TableViewDefCategory->setColumnWidth(column, 100);
    m_defCategoryModel->setHeaderData(column, Qt::Horizontal, "По договору"); column++;
}

void SettingWindow::reject()
{

}



