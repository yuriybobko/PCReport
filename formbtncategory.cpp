#include "formbtncategory.h"
#include "ui_formbtnshowcategory.h"

FormBtnCategory::FormBtnCategory(int formId, QString category, QWidget *parent) :
    QFrame(parent),  //QWidget(parent),
    ui(new Ui::FormBtnShowCategory)
{
    ui->setupUi(this);

    m_formId = formId;
    ui->LblCategory->setText(QString::number(m_formId) + " - " + category);

    connect(ui->BtnShow, &QPushButton::clicked, [this]()
    {
        emit this->signalToShowFormCategory(m_formId);
    });
    connect(this, SIGNAL(signalToShowFormCategory(int)), parent, SLOT(showFormCategory(int)));
}

FormBtnCategory::FormBtnCategory(int formId, QString category, QString tax, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FormBtnShowCategory)
{
    ui->setupUi(this);

    m_formId = formId;
    ui->LblCategory->setText(QString::number(m_formId + 1) + " - " + category);
    ui->LblTax->setText(tax);

    connect(ui->BtnShow, &QPushButton::clicked, [this]()
    {
        emit this->signalToShowFormCategory(m_formId);
    });
    connect(this, SIGNAL(signalToShowFormCategory(int)), parent, SLOT(showFormCategory(int)));
}

FormBtnCategory::~FormBtnCategory()
{
    delete ui;
}

void FormBtnCategory::setFormId(int newFormId)
{
    m_formId = newFormId;
}
