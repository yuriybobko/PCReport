#include "formcategory.h"
#include "ui_formcategory.h"
#include <QDoubleValidator>

FormCategory::FormCategory(int formId, QString category, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCategory)
{
    m_formId = formId;
    ui->setupUi(this);
    ui->LblCategoryDescr->setText(category);
}

FormCategory::FormCategory(int formId, DefinedCategory defCategory, QWidget *parent):
    QWidget(parent),
    ui(new Ui::FormCategory)
{
    ui->setupUi(this);
    m_formId = formId;
    ui->LblCategoryDescr->setText(QString::number(m_formId + 1) + " - " + defCategory.categoryTitle);
    ui->LblTaxDescr->setText(defCategory.taxTitle);
    m_categoryTitle = defCategory.categoryTitle;
    m_taxTitle = defCategory.taxTitle;
    if (!defCategory.selfcoast) {
        ui->LblSelfcoast->hide();
        ui->EditSelfcoast->hide();
    }
    m_koefSalary = defCategory.koefSalary;
    m_koefProfit = defCategory.koefProfit;
    m_isSelling = defCategory.isSelling;
    m_isContracted = defCategory.isContracted;

    ui->EditTotalSum->setValidator(new QDoubleValidator(0, 1E+12, 2, this));
    ui->EditTotalSum->setReadOnly(true);
    ui->EditCashMoney->setValidator(new QDoubleValidator(0, 1E+12, 2, this));
    ui->EditNonCashMoney->setValidator(new QDoubleValidator(0, 1E+12, 2, this));

    connect(ui->EditCashMoney, SIGNAL(textChanged(QString)), this, SLOT(setTotalSum()));
    connect(ui->EditNonCashMoney, SIGNAL(textChanged(QString)), this, SLOT(setTotalSum()));
}

FormCategory::~FormCategory()
{
    disconnect(ui->EditCashMoney, SIGNAL(textChanged(QString)), this, SLOT(setTotalSum()));
    disconnect(ui->EditNonCashMoney, SIGNAL(textChanged(QString)), this, SLOT(setTotalSum()));
    delete ui;
}

void FormCategory::setFormId(int newFormId)
{
    m_formId = newFormId;
}

float FormCategory::getCashMoney()
{
    float cashMoney = 0;
    if (!ui->EditCashMoney->text().isEmpty())
        cashMoney = ui->EditCashMoney->text().toFloat();
    return cashMoney;
}

float FormCategory::getNonCashMoney()
{
    float nonCashMoney = 0;
    if (!ui->EditNonCashMoney->text().isEmpty())
        nonCashMoney = ui->EditNonCashMoney->text().toFloat();
    return nonCashMoney;
}

float FormCategory::getSelfcoast()
{
    float selfcoast = 0;
    if (!ui->EditSelfcoast->text().isEmpty())
        selfcoast = ui->EditSelfcoast->text().toFloat();
    return selfcoast;
}

float FormCategory::getTotalSum()
{
    float totalSum = 0;
    if (!ui->EditTotalSum->text().isEmpty())
        totalSum = ui->EditTotalSum->text().toFloat();
    return totalSum;
}

QString FormCategory::getCategoryTitle()
{
    return m_categoryTitle;
}

QString FormCategory::getTaxTitle()
{
    return m_taxTitle;
}

float FormCategory::getKoefSalary()
{
    return m_koefSalary;
}

float FormCategory::getKoefProfit()
{
    return m_koefProfit;
}

bool FormCategory::isSelling()
{
    return m_isSelling;
}

bool FormCategory::isContracted()
{
    return m_isContracted;
}

void FormCategory::clearEdits()
{
    ui->EditTotalSum->setText("");
    ui->EditCashMoney->setText("");
    ui->EditNonCashMoney->setText("");
    ui->EditSelfcoast->setText("");
}

void FormCategory::setTotalSum()
{
    float totalSum = 0;
    totalSum = ui->EditCashMoney->text().toFloat() + ui->EditNonCashMoney->text().toFloat();
    ui->EditTotalSum->setText(QString::number(totalSum));
}
