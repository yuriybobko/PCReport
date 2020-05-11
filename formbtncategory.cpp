#include "formbtncategory.h"
#include "ui_formbtncategory.h"

FormBtnCategory::FormBtnCategory(int formId, QString category, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormBtnCategory)
{
    ui->setupUi(this);

    m_formId = formId;
    ui->LblCategory->setText(QString::number(m_formId) + " - " + category);


    /*connect(ui->BtnRemove, &QPushButton::clicked, [this]()
    {
        emit this->signalToRemoveForm(m_formId);
    });
    connect(this, SIGNAL(signalToRemoveForm(int)), parent, SLOT(removeFormBtnCategory(int)));*/

    connect(ui->BtnShow, &QPushButton::clicked, [this]()
    {
        emit this->signalToShowFormCategory(m_formId);
    });
    connect(this, SIGNAL(signalToShowFormCategory(int)), parent, SLOT(showFormCategory(int)));
}

FormBtnCategory::FormBtnCategory(int formId, QString category, QString tax, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormBtnCategory)
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
