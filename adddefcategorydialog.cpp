#include "adddefcategorydialog.h"
#include "ui_adddefcategorydialog.h"

AddDefCategoryDialog::AddDefCategoryDialog(QWidget *parent, QStringList categoryList, QStringList taxList) :
    QDialog(parent),
    ui(new Ui::AddDefCategoryDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Создать новую категорию деятельности");
    ui->CmbBoxCategory->addItems(categoryList);
    ui->CmbBoxTax->addItems(taxList);
    QStringList noYesList = {"Нет", "Да"};
    ui->CmbBoxSelfcoast->addItems(noYesList);
    ui->CmbBoxSelling->addItems(noYesList);
    ui->CmbBoxContract->addItems(noYesList);

    ui->EditKoefSalary->setText("1");
    ui->EditKoefSalary->setValidator(new QDoubleValidator(0, 1E+12, 5, this));
    ui->EditKoefProfit->setText("1");
    ui->EditKoefProfit->setValidator(new QDoubleValidator(0, 1E+12, 5, this));

    connect(ui->BtnAdd, &QPushButton::clicked, [this]() {
        if (!ui->EditKoefSalary->text().isEmpty() && !ui->EditKoefProfit->text().isEmpty())
            this->accept();
        else
            QMessageBox::warning(this, "Внимание", "Проверьте корректность данных");
        return;
    });
    connect(ui->BtnCancel, &QPushButton::clicked, [this]() {
        this->reject();
    });
}

AddDefCategoryDialog::~AddDefCategoryDialog()
{
    delete ui;
}

DefinedCategory AddDefCategoryDialog::getDefCategory(QWidget *parent, QStringList categoryList,
                                                     QStringList taxList, bool *btnOk)
{
    AddDefCategoryDialog addDefCtgryDlg(parent, categoryList, taxList);
    DefinedCategory defCtgry;
    if (addDefCtgryDlg.exec() == QDialog::Accepted) {
        defCtgry.categoryTitle = addDefCtgryDlg.getCategory();
        defCtgry.taxTitle = addDefCtgryDlg.getTax();
        defCtgry.selfcoast = addDefCtgryDlg.getSelfcoast();
        defCtgry.isSelling = addDefCtgryDlg.isSelling();
        defCtgry.isContracted = addDefCtgryDlg.isContracted();
        defCtgry.koefSalary = addDefCtgryDlg.getKoefSalary();
        defCtgry.koefProfit = addDefCtgryDlg.getKoefProfit();
        *btnOk = true;
    }
    else {
        *btnOk = false;
    }
    return defCtgry;
}

QString AddDefCategoryDialog::getCategory()
{
    return ui->CmbBoxCategory->currentText();
}

QString AddDefCategoryDialog::getTax()
{
    return ui->CmbBoxTax->currentText();
}

bool AddDefCategoryDialog::getSelfcoast()
{
    if (ui->CmbBoxSelfcoast->currentText() == "Да")
        return true;
    else
        return false;
}

bool AddDefCategoryDialog::isSelling()
{
    if (ui->CmbBoxSelling->currentText() == "Да")
        return true;
    else
        return false;
}

bool AddDefCategoryDialog::isContracted()
{
    if (ui->CmbBoxContract->currentText() == "Да")
        return true;
    else
        return false;
}

float AddDefCategoryDialog::getKoefSalary()
{
    return ui->EditKoefSalary->text().toFloat();
}

float AddDefCategoryDialog::getKoefProfit()
{
    return ui->EditKoefProfit->text().toFloat();
}
