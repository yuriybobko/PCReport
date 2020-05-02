#ifndef FORMCATEGORY_H
#define FORMCATEGORY_H

#include <QWidget>
#include <QDebug>
#include "structures.h"

namespace Ui {
class FormCategory;
}

class FormCategory : public QWidget
{
    Q_OBJECT

public:
    explicit FormCategory(int formId, QString category, QWidget *parent = nullptr);
    FormCategory(int formId, DefinedCategory defCategory, QWidget *parent = nullptr);
    ~FormCategory();

    void setFormId(int newFormId);

    float getCashMoney();
    float getNonCashMoney();
    float getSelfcoast();
    float getTotalSum();

    QString getCategoryTitle();
    QString getTaxTitle();
    float getKoefSalary();
    float getKoefProfit();
    bool isSelling();
    bool isContracted();

    void clearEdits();

public slots:

private slots:
    void setTotalSum();

private:
    Ui::FormCategory *ui;

    int m_formId;
    QString m_categoryTitle;
    QString m_taxTitle;
    float m_koefSalary;
    float m_koefProfit;
    bool m_isSelling = false;
    bool m_isContracted = false;


};

#endif // FORMCATEGORY_H
