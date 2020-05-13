#include "mainwindow.h"
#include "ui_mainwindow.h"

SqlManager sqlManager;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("PhC-Report");

    m_settingWndw = new SettingWindow(this);
    m_calendarWndw = new CalendarWindow(this);

    connect(m_settingWndw, SIGNAL(signalToEditStaffer(QVector <QString>)),
            this, SLOT(editStaffer(QVector <QString>)));
    connect(m_settingWndw, SIGNAL(signalToEditFormBtnCategory(QVector <QString>)),
            this, SLOT(editFormBtnCategory(QVector <QString>)));

    connect(m_settingWndw, SIGNAL(signalToEditFormBtnCategory(QVector <DefinedCategory>)),
            this, SLOT(editFormBtnCategory(QVector <DefinedCategory>)));

    connect(ui->BtnSettings, SIGNAL(clicked()), this, SLOT(showSettingWindow()));
    //connect(ui->BtnCalendar, SIGNAL(clicked()), this, SLOT(showCalendarWindow()));
    connect(ui->BtnCalendar, &QPushButton::clicked, [this]() {
        QString selectedString = this->getDateFromCalendar();
        if (!selectedString.isEmpty())
            ui->LblSelectedDate->setText(selectedString);
    });
    connect(ui->BtnCalendarFrom, &QPushButton::clicked, [this]() {
        QString selectedString = this->getDateFromCalendar();
        if (!selectedString.isEmpty())
            ui->LblFromDate->setText(selectedString);
    });
    connect(ui->BtnCalendarTo, &QPushButton::clicked, [this]() {
        QString selectedString = this->getDateFromCalendar();
        if (!selectedString.isEmpty())
            ui->LblToDate->setText(selectedString);
    });
    connect(ui->BtnTabViewCalendarFrom, &QPushButton::clicked, [this]() {
        QString selectedString = this->getDateFromCalendar();
        if (!selectedString.isEmpty())
            ui->LblTabViewFromDate->setText(selectedString);
    });
    connect(ui->BtnTabViewCalendarTo, &QPushButton::clicked, [this]() {
        QString selectedString = this->getDateFromCalendar();
        if (!selectedString.isEmpty())
            ui->LblTabViewToDate->setText(selectedString);
    });

    connect(ui->BtnWriteRegisterToDataBase, SIGNAL(clicked()), this, SLOT(writeDefCategoryRecordToDataBase()));
    connect(ui->BtnWriteCostsToDataBase, SIGNAL(clicked()), this, SLOT(writeCostsRecordToDataBase()));

    connect(ui->BtnExit, &QPushButton::clicked, []() {
        QApplication::exit();
    });

    // Сигнал в status bar
    connect(&sqlManager, &SqlManager::signalToStatusBar, this, &MainWindow::showStatusBar);

    // Нажатие кнопки вычисления зарплаты и заполнения полей полной суммы
    connect(ui->BtnFillFrameTotalSum, SIGNAL(clicked()), this, SLOT(fillFrameTotalSum()));

    connect(ui->BtnShowProfit, SIGNAL(clicked()), this, SLOT(showProfitInEdit()));

    connect(ui->BtnShowTableView, SIGNAL(clicked()), this, SLOT(editTableView()));
    connect(ui->BtnDeleteRecord, SIGNAL(clicked()), this, SLOT(removeRecordInTableView()));

    this->setChildWidgets();
}

MainWindow::~MainWindow()
{
    foreach (FormBtnCategory *item, m_FormBtnCategory) {
        if (item != nullptr)
            delete item;
    }
    m_FormBtnCategory.clear();

    foreach (FormCategory *item, m_FormCategory) {
        if (item != nullptr)
            delete item;
    }
    m_FormCategory.clear();

    if (this->LayoutFormBtnCategory)
        delete this->LayoutFormBtnCategory;

    if (m_settingWndw)
        delete m_settingWndw;
    if (m_calendarWndw)
        delete m_calendarWndw;
    delete ui;
}

void MainWindow::setChildWidgets()
{
    QString imgLogoFile=":app_imgs/Logo.jpg";
    QPixmap imgLogo(imgLogoFile);
    ui->LblImgLogo->setScaledContents(true);
    ui->LblImgLogo->setPixmap(imgLogo);

    QDate curDate;
    QString curDateString = "Текущая дата:\n";
    curDateString += curDate.currentDate().toString("yyyy-MM-dd");
    ui->LblCurDate->setText(curDateString);

    curDateString = curDate.currentDate().toString("yyyy-MM-dd");
    ui->LblSelectedDate->setText(curDateString);

    ui->ScrollAreaFormBtnCategory->setWidgetResizable(false);
    ui->ScrollAreaFormBtnCategory->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QString imgSettingsIconPath = ":app_imgs/SettingsIcon.JPG";
    QPixmap imgSettingsIcon(imgSettingsIconPath);
    ui->BtnSettings->setIconSize(QSize(ui->BtnSettings->size().width() - 5,
                                       ui->BtnSettings->size().height() - 5));
    ui->BtnSettings->setIcon(imgSettingsIcon);
    ui->BtnSettings->setToolTip("Настройки");

    QString imgCalendarIconPath = ":app_imgs/CalendarIcon.JPG";
    QPixmap imgCalendarIcon(imgCalendarIconPath);
    ui->BtnCalendar->setIconSize(QSize(ui->BtnCalendar->size().width() - 1,
                                       ui->BtnCalendar->size().height() - 1));
    ui->BtnCalendar->setIcon(imgCalendarIcon);
    ui->BtnCalendar->setToolTip("Календарь");

    ui->BtnCalendarFrom->setIconSize(QSize(ui->BtnCalendarFrom->size().width() - 1,
                                       ui->BtnCalendarFrom->size().height() - 1));
    ui->BtnCalendarFrom->setIcon(imgCalendarIcon);
    ui->BtnCalendarFrom->setToolTip("Календарь");
    ui->BtnCalendarTo->setIconSize(QSize(ui->BtnCalendarTo->size().width() - 1,
                                       ui->BtnCalendarTo->size().height() - 1));
    ui->BtnCalendarTo->setIcon(imgCalendarIcon);
    ui->BtnCalendarTo->setToolTip("Календарь");
    ui->LblFromDate->setText(curDateString);
    ui->LblToDate->setText(curDateString);

    ui->BtnTabViewCalendarFrom->setIconSize(QSize(ui->BtnTabViewCalendarFrom->size().width() - 1,
                                       ui->BtnTabViewCalendarFrom->size().height() - 1));
    ui->BtnTabViewCalendarFrom->setIcon(imgCalendarIcon);
    ui->BtnTabViewCalendarFrom->setToolTip("Календарь");
    ui->BtnTabViewCalendarTo->setIconSize(QSize(ui->BtnCalendarTo->size().width() - 1,
                                       ui->BtnCalendarTo->size().height() - 1));
    ui->BtnTabViewCalendarTo->setIcon(imgCalendarIcon);
    ui->BtnTabViewCalendarTo->setToolTip("Календарь");
    ui->LblTabViewFromDate->setText(curDateString);
    ui->LblTabViewToDate->setText(curDateString);

    this->LayoutFormBtnCategory = new QVBoxLayout(ui->scrollAreaWidgetContents);
    this->LayoutFormBtnCategory->setObjectName(QStringLiteral("LayoutFormBtnCategory"));
    this->LayoutFormBtnCategory->setContentsMargins(6, 6, 6, 6);

    this->editStaffer(m_settingWndw->getStaffer());
    this->editFormBtnCategory(m_settingWndw->getDefCategory());

    ui->EditTotalSum->setReadOnly(true);
    ui->EditSalary->setReadOnly(true);
    ui->EditCashMoney->setReadOnly(true);
    ui->EditNonCashMoney->setReadOnly(true);

    QStringList cmbBosList {CASH_STRING, NONCASH_STRING};
    ui->CmbBoxCash->addItems(cmbBosList);

    ui->EditCostsTotalSum->setValidator(new QDoubleValidator(0, 1E+12, 2, this));

    ui->TableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->TabWidgetMain->setStyleSheet("background-color: rgb(235, 235, 235);");
}

void MainWindow::addFormBtnCategory(QString strCategory)
{
        FormBtnCategory *formBtnCtgry = new FormBtnCategory(m_FormBtnCategory.size(), strCategory, this);
        m_FormBtnCategory.append(formBtnCtgry);
        this->LayoutFormBtnCategory->addWidget(m_FormBtnCategory.at(m_FormBtnCategory.size() - 1));
        ui->scrollAreaWidgetContents->setGeometry(QRect(0, 0,
                                                        ui->ScrollAreaFormBtnCategory->width() - 10,
                                                        m_FormBtnCategory.size()*
                                                        m_FormBtnCategory.at(m_FormBtnCategory.size() - 1)->height()
                                                        + m_dHeight));

        FormCategory *formCtgry = new FormCategory(m_FormCategory.size(), strCategory,
                                                   this);
        m_FormCategory.append(formCtgry);
}

void MainWindow::addFormBtnCategory(DefinedCategory defCategory)
{
        FormBtnCategory *formBtnCtgry = new FormBtnCategory(m_FormBtnCategory.size(),
                                                            defCategory.categoryTitle,
                                                            defCategory.taxTitle,
                                                            this);
        m_FormBtnCategory.append(formBtnCtgry);
        this->LayoutFormBtnCategory->addWidget(m_FormBtnCategory.at(m_FormBtnCategory.size() - 1));
        ui->scrollAreaWidgetContents->setGeometry(QRect(0, 0,
                                                        ui->ScrollAreaFormBtnCategory->width() - 10,
                                                        m_FormBtnCategory.size()*
                                                        m_FormBtnCategory.at(m_FormBtnCategory.size() - 1)->height()
                                                        + m_dHeight));

//        FormCategory *formCtgry = new FormCategory(m_FormCategory.size(),
//                                                   defCategory,
//                                                   this);
        FormCategory *formCtgry = new FormCategory(m_FormCategory.size(),
                                                   defCategory,
                                                   ui->TabDailyReport);
        m_FormCategory.append(formCtgry);
}

void MainWindow::removeFormBtnCategory(int formId)
{
    this->LayoutFormBtnCategory->removeWidget(m_FormBtnCategory.at(formId));
    delete m_FormBtnCategory.at(formId);
    m_FormBtnCategory.remove(formId);
    if (m_FormBtnCategory.size() > 0) {
        ui->scrollAreaWidgetContents->setGeometry(QRect(0, 0,
                                                        ui->ScrollAreaFormBtnCategory->width() - 10,
                                                        m_FormBtnCategory.size()*
                                                        m_FormBtnCategory.at(m_FormBtnCategory.size() - 1)->height()
                                                        + m_dHeight));
    }
    m_FormCategory.at(formId)->hide();
    delete m_FormCategory.at(formId);
    m_FormCategory.remove(formId);
}

void MainWindow::showFormCategory(int formId)
{
    foreach (FormCategory *item, m_FormCategory) {
        item->hide();
    }
    m_FormCategory.at(formId)->setGeometry(490, 30,
                                           m_FormCategory.at(formId)->width(),
                                           m_FormCategory.at(formId)->height());
    m_FormCategory.at(formId)->show();
}

void MainWindow::editFormBtnCategory(const QVector<QString> categoryVector)
{
    for (int ii = m_FormBtnCategory.size() - 1; ii >= 0; ii--) {
        this->removeFormBtnCategory(ii);
    }
    for (int ii = 0; ii < categoryVector.size(); ii++) {
        this->addFormBtnCategory(categoryVector.at(ii));
    }
    if (m_FormCategory.size() > 0)
        this->showFormCategory(0);
}

void MainWindow::editFormBtnCategory(const QVector<DefinedCategory> defCategoryVector)
{
    for (int ii = m_FormBtnCategory.size() - 1; ii >= 0; ii--) {
        this->removeFormBtnCategory(ii);
    }
    for (int ii = 0; ii < defCategoryVector.size(); ii++) {
        this->addFormBtnCategory(defCategoryVector.at(ii));
    }
    if (m_FormCategory.size() > 0)
        this->showFormCategory(0);
}

void MainWindow::addStaffer(QString strStaffer)
{
        ui->CmbBoxStaffer->addItem(strStaffer);
}

void MainWindow::removeStaffer(int id)
{
    ui->CmbBoxStaffer->removeItem(id);
}

void MainWindow::editStaffer(const QVector<QString> stafferVector)
{
    ui->CmbBoxStaffer->clear();
    for (int ii = 0; ii < stafferVector.size(); ii++) {
        this->addStaffer(stafferVector.at(ii));
    }
}

void MainWindow::showStatusBar(QString statusBarText)
{
    this->statusBar()->showMessage(statusBarText);
}

void MainWindow::showSettingWindow()
{
    m_settingWndw->show();
}

void MainWindow::showCalendarWindow()
{
    bool btnOk = false;
    QString selectedDateString = CalendarWindow::getDate(&btnOk, this);
    if (btnOk) {
        ui->LblSelectedDate->setText(selectedDateString);
    }
}

QString MainWindow::getDateFromCalendar()
{
    bool btnOk = false;
    QString selectedDateString = CalendarWindow::getDate(&btnOk, this);
    if (btnOk) {
        return selectedDateString;
    }
    else
        return "";
}

void MainWindow::fillFrameTotalSum()
{
    float totalCash = this->getTotalCashMoney();
    float totalNonCash = this->getTotalNonCashMoney();
    ui->EditCashMoney->setText(QString::number(totalCash));
    ui->EditNonCashMoney->setText(QString::number(totalNonCash));
    this->setTotalSum(totalCash, totalNonCash);
    this->setSalary();
}

float MainWindow::getTotalCashMoney()
{
    float totalCash = 0.0;
    for (int ii = 0; ii < m_FormCategory.size(); ii++) {
        totalCash += m_FormCategory.at(ii)->getCashMoney();
    }
    return totalCash;
}

float MainWindow::getTotalNonCashMoney()
{
    float totalNonCash = 0.0;
    for (int ii = 0; ii < m_FormCategory.size(); ii++) {
        totalNonCash += m_FormCategory.at(ii)->getNonCashMoney();
    }
    return totalNonCash;
}

void MainWindow::setTotalSum(float totalCash, float totalNonCash)
{
    float totalSum = totalCash + totalNonCash;
    ui->EditTotalSum->setText(QString::number(totalSum));
}

float MainWindow::calcSalary()
{
    float salary = 0;
    for (int ii = 0; ii < m_FormCategory.size(); ii++) {
        float kSalary = m_FormCategory.at(ii)->getKoefSalary();
        float totalSum = m_FormCategory.at(ii)->getTotalSum();
        float selfcoast = m_FormCategory.at(ii)->getSelfcoast();
        if (totalSum - selfcoast < 0) {
            this->showStatusBar("Себестоимость больше общей суммы");
            this->clearDailyReportTabFinanceEdits();
            return -1;
        }
        float adtnlSum = (totalSum - selfcoast)*kSalary;
        if (!m_FormCategory.at(ii)->isSelling() && totalSum > 0) {
            float kProfit = m_FormCategory.at(ii)->getKoefProfit();
            adtnlSum *= kProfit;
        }
        salary += adtnlSum;
    }
    salary += m_settingWndw->basicWage*(1 - m_settingWndw->koefBasicWage);
    if (salary < m_settingWndw->basicWage)
        salary = m_settingWndw->basicWage;
    return salary;
}

void MainWindow::setSalary()
{
    float salary = this->calcSalary();
    if (salary < 0) {
        this->showStatusBar("Себестоимость больше общей суммы");
        return;
    }
    ui->EditSalary->setText(QString::number(salary));
    this->showStatusBar("Заработная плата вычислена");
}

void MainWindow::clearDailyReportTabFinanceEdits()
{
    ui->EditSalary->setText("");
    ui->EditTotalSum->setText("");
    ui->EditCashMoney->setText("");
    ui->EditNonCashMoney->setText("");
}

void MainWindow::clearCostsTabEdits()
{
    ui->EditCostsTotalSum->setText("");
    ui->TextEditCostsDescr->setPlainText("");
}

void MainWindow::writeDefCategoryRecordToDataBase()
{
    bool isAnyRecordExist = false;
    bool isRecordExist = false;
    this->showStatusBar("Идет запись в реестр категорий");
    DefCategoryRegisterRecord defCtgryRegRecord;
    defCtgryRegRecord.date = ui->LblSelectedDate->text();
    QString stafferName = ui->CmbBoxStaffer->currentText();
    defCtgryRegRecord.stafferId = sqlManager.selectIdFromTable(staffersTable.table, staffersTable.id,
                                                               staffersTable.name, stafferName);
    for (int ii = 0; ii < m_FormCategory.size(); ii++) {
        for (int kk = 1; kk < 3; kk++) {
            isRecordExist = false;
            QString ctgryTitle = m_FormCategory.at(ii)->getCategoryTitle();
            defCtgryRegRecord.categoryId = sqlManager.selectIdFromTable(categoriesTable.table, categoriesTable.id,
                                                                        categoriesTable.title, ctgryTitle);
            QString taxTitle = m_FormCategory.at(ii)->getTaxTitle();
            defCtgryRegRecord.taxId = sqlManager.selectIdFromTable(taxesTable.table, taxesTable.id,
                                                                       taxesTable.title, taxTitle);
            if (kk == 1) {
                QString cashTitle = CASH_STRING;
                defCtgryRegRecord.cashId = sqlManager.selectIdFromTable(cashTable.table, cashTable.id,
                                                                           cashTable.title, cashTitle);
                defCtgryRegRecord.amount = m_FormCategory.at(ii)->getCashMoney();
                if (m_FormCategory.at(ii)->getCashMoney() > 0) {
                    defCtgryRegRecord.selfcoast = m_FormCategory.at(ii)->getSelfcoast();
                    isRecordExist = true;
                    isAnyRecordExist = true;
                }
            }
            else {
                QString cashTitle = NONCASH_STRING;
                defCtgryRegRecord.cashId = sqlManager.selectIdFromTable(cashTable.table, cashTable.id,
                                                                           cashTable.title, cashTitle);
                defCtgryRegRecord.amount = m_FormCategory.at(ii)->getNonCashMoney();
                if (m_FormCategory.at(ii)->getNonCashMoney() > 0) {
                    if (m_FormCategory.at(ii)->getCashMoney() > 0) {
                        defCtgryRegRecord.selfcoast = 0;
                    }
                    else
                        defCtgryRegRecord.selfcoast = m_FormCategory.at(ii)->getSelfcoast();
                    isRecordExist = true;
                    isAnyRecordExist = true;
                }
            }
            if (isRecordExist) {
                if (sqlManager.insertIntoDefCategoryRegisterTable(defCtgryRegRecord)) {
                    //this->showStatusBar("Запись в реестр категорий выполнена");
                }
                else {
                    this->showStatusBar("Не удается выполнить запись в реестр категорий");
                    return;
                }
            }
        }
    }
    if (isAnyRecordExist) {
        this->writeSalaryRecordToDataBase();
        this->showStatusBar("Запись в реестр категорий выполнена");
        this->clearDailyReportTabFinanceEdits();
        for (int ii = 0; ii < m_FormCategory.size(); ii++) {
            m_FormCategory.at(ii)->clearEdits();
        }
    }
    else
        this->showStatusBar("Все поля пусты");
}

void MainWindow::writeSalaryRecordToDataBase()
{
    SalaryRegisterRecord salaryRegRecord;
    salaryRegRecord.date = ui->LblSelectedDate->text();
    QString stafferName = ui->CmbBoxStaffer->currentText();
    salaryRegRecord.stafferId = sqlManager.selectIdFromTable(staffersTable.table, staffersTable.id,
                                                               staffersTable.name, stafferName);
    salaryRegRecord.amount = this->calcSalary();
    salaryRegRecord.basicWage = m_settingWndw->basicWage;
    salaryRegRecord.koefBasicWage = m_settingWndw->koefBasicWage;

    if (!sqlManager.isSalaryRecordExist(salaryRegRecord))
        sqlManager.insertIntoSalaryRegisterTable(salaryRegRecord);
    else {
        sqlManager.updateRecordInSalaryRegisterTable(salaryRegRecord);
    }
}

void MainWindow::writeCostsRecordToDataBase()
{
    bool isAnyRecordExist = false;
    if (!ui->EditCostsTotalSum->text().isEmpty() && !ui->TextEditCostsDescr->toPlainText().isEmpty())
        isAnyRecordExist = true;

    if (isAnyRecordExist) {
        this->showStatusBar("Идет запись в реестр расходов");
        CostsRegisterRecord costsRegRecord;
        costsRegRecord.date = ui->LblSelectedDate->text();
        QString cashString = ui->CmbBoxCash->currentText();
        costsRegRecord.cashId = sqlManager.selectIdFromTable(cashTable.table, cashTable.id,
                                                                   cashTable.title, cashString);
        costsRegRecord.amount = ui->EditCostsTotalSum->text().toFloat();
        costsRegRecord.description = ui->TextEditCostsDescr->toPlainText();
        if (sqlManager.insertIntoCostsRegisterTable(costsRegRecord))
            this->showStatusBar("Запись в реестр расходов выполнена");
        else
            this->showStatusBar("Не удается выполнить запись в реестр расходов");
        this->clearCostsTabEdits();
    }
    else
        this->showStatusBar("Все поля пусты");
}

void MainWindow::showProfitInEdit()
{
    QString firstDate = ui->LblFromDate->text();
    QString secondDate = ui->LblToDate->text();
    float netSum = 0;
    QVector <DefinedCategoryRecord> defCategoryRecord = sqlManager.selectDefCategoryRecord();
    for (int ii = 0; ii < defCategoryRecord.size(); ii++) {
        float totalSum = 0;
        int categoryId = defCategoryRecord.at(ii).categoryId;
        int taxId = defCategoryRecord.at(ii).taxId;
        totalSum = sqlManager.selectTotalSumInPeriod(firstDate, secondDate, categoryId, taxId);
        float totalSelfcoast = 0;
        totalSelfcoast = sqlManager.selectTotalSelfcoastInPeriod(firstDate, secondDate, categoryId, taxId);
        netSum += (totalSum - totalSelfcoast)*defCategoryRecord.at(ii).koefProfit;
    }
    float totalSalary = 0;
    totalSalary = sqlManager.selectTotalSalaryInPeriod(firstDate, secondDate);
    float totalCosts = 0;
    totalCosts = sqlManager.selectTotalCostsInPeriod(firstDate, secondDate);
    float totalProfit = netSum - (totalSalary + totalCosts);
    ui->TextEditProfit->setText("Прибыль за период с " + firstDate + " по " + secondDate + "\n"
                                                                                           "составила " + QString::number(totalProfit) + " руб.");
}

void MainWindow::editTableView()
{
    this->setDefCategoryRegisterInTableView();
}

void MainWindow::setDefCategoryRegisterInTableView()
{
    QString firstDate = ui->LblTabViewFromDate->text();
    QString secondDate = ui->LblTabViewToDate->text();
    m_defCategoryRegRecordView = sqlManager.selectDefCategoryRegRecordView(firstDate, secondDate);

    ui->TableView->clearSpans();

    if (m_defCategoryRegisterModel != nullptr)
        delete m_defCategoryRegisterModel;
    m_defCategoryRegisterModel = new QStandardItemModel(0, 7, ui->TableView);
    ui->TableView->setModel(m_defCategoryRegisterModel);
    int column = 0;
    for(int row = 0; row != m_defCategoryRegRecordView.size(); ++row) {
       this->addRowInTableDefCategoryRegister(m_defCategoryRegRecordView.at(row));
    }
    //ui->TableView->setColumnWidth(column, 30);
    m_defCategoryRegisterModel->setHeaderData(column, Qt::Horizontal, "Номер");
    ui->TableView->hideColumn(column);
    column++;
    ui->TableView->setColumnWidth(column, 80);
    m_defCategoryRegisterModel->setHeaderData(column, Qt::Horizontal, "Дата"); column++;
    ui->TableView->setColumnWidth(column, 140);
    m_defCategoryRegisterModel->setHeaderData(column, Qt::Horizontal, "Сотрудник"); column++;
    m_defCategoryRegisterModel->setHeaderData(column, Qt::Horizontal, "Категория"); column++;
    ui->TableView->setColumnWidth(column, 70);
    m_defCategoryRegisterModel->setHeaderData(column, Qt::Horizontal, "Налог"); column++;
    ui->TableView->setColumnWidth(column, 70);
    m_defCategoryRegisterModel->setHeaderData(column, Qt::Horizontal, "Тип расчета"); column++;
    ui->TableView->setColumnWidth(column, 70);
    m_defCategoryRegisterModel->setHeaderData(column, Qt::Horizontal, "Сумма"); column++;
    m_defCategoryRegisterModel->setHeaderData(column, Qt::Horizontal, "Себестоимость"); column++;
}

void MainWindow::addRowInTableDefCategoryRegister(DefCategoryRegisterRecordView defCtgryRegRecordView)
{
    m_defCategoryRegisterModel->setRowCount(m_defCategoryRegisterModel->rowCount() + 1);
    int row = m_defCategoryRegisterModel->rowCount() - 1;
    int column = 0;
    QStandardItem *newItem0 = new QStandardItem(QString::number(defCtgryRegRecordView.id));
    m_defCategoryRegisterModel->setItem(row, column, newItem0);
    column++;
    QStandardItem *newItem1 = new QStandardItem(defCtgryRegRecordView.date);
    m_defCategoryRegisterModel->setItem(row, column, newItem1);
    column++;
    QStandardItem *newItem2 = new QStandardItem(defCtgryRegRecordView.staffer);
    m_defCategoryRegisterModel->setItem(row, column, newItem2);
    column++;
    QStandardItem *newItem3 = new QStandardItem(defCtgryRegRecordView.category);
    m_defCategoryRegisterModel->setItem(row, column, newItem3);
    column++;
    QStandardItem *newItem4 = new QStandardItem(defCtgryRegRecordView.tax);
    m_defCategoryRegisterModel->setItem(row, column, newItem4);
    column++;
    QStandardItem *newItem5 = new QStandardItem(defCtgryRegRecordView.cash);
    m_defCategoryRegisterModel->setItem(row, column, newItem5);
    column++;
    QStandardItem *newItem6 = new QStandardItem(QString::number(defCtgryRegRecordView.amount));
    m_defCategoryRegisterModel->setItem(row, column, newItem6);
    column++;
    QStandardItem *newItem7 = new QStandardItem(QString::number(defCtgryRegRecordView.selfcoast));
    m_defCategoryRegisterModel->setItem(row, column, newItem7);
    column++;
}

void MainWindow::setCostsRegisterInTableView()
{

}

void MainWindow::addRowInTableCostsRegister(CostsRegisterRecordView costsRegRecordView)
{

}

void MainWindow::removeRecordInTableView()
{
    bool btnNo = QMessageBox::information(0,
                                    "Внимание",
                                    "Удалить выбранные строки?",
                                    "Да",
                                    "Нет",
                                    QString(),
                                    0,
                                    1);
    if (!btnNo) {
        QModelIndexList selectedRows = ui->TableView->selectionModel()->selectedRows();
        for (int ii = 0; ii < selectedRows.size(); ii++){
            int requiredId = selectedRows[ii].data(0).toInt();
            int selectedRow = selectedRows[ii].row();
            QModelIndex index = ui->TableView->model()->index(selectedRow, 1);
            QString date = ui->TableView->model()->data(index).toString();
            index = ui->TableView->model()->index(selectedRow, 2);
            QString staffer = ui->TableView->model()->data(index).toString();
            StaffersTable stafferTable;
            int stafferId = sqlManager.selectIdFromTable(stafferTable.table, stafferTable.id,
                                                         stafferTable.name, staffer);
            SalaryRegisterRecord salaryRegRec = sqlManager.selectSalaryRecord(date, stafferId);
            if (sqlManager.deleteInDefCategoryRegisterTable(requiredId))
                this->showStatusBar("Строка удалена");
            sqlManager.updateRecordInSalaryRegisterTable(salaryRegRec);
        }
        this->setDefCategoryRegisterInTableView();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    sqlManager.closeDataBase();
    QApplication::exit();
}

void MainWindow::testFunc()
{

}
