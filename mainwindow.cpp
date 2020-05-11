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
                if (sqlManager.insertIntoDefCategoryRegisterTable(defCategoryRegisterTable, defCtgryRegRecord)) {
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

    if (!sqlManager.isSalaryRecordExist(salaryRegisterTable, salaryRegRecord))
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
        if (sqlManager.insertIntoCostsRegisterTable(costsRegisterTable, costsRegRecord))
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
    int requiredId = 1;
    float totalSum;
    totalSum = sqlManager.selectTotalSumInPeriod(defCategoryRegisterTable, firstDate, secondDate, requiredId);
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
