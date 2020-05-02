#ifndef ADDDEFCATEGORYDIALOG_H
#define ADDDEFCATEGORYDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "structures.h"

namespace Ui {
class AddDefCategoryDialog;
}

class AddDefCategoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDefCategoryDialog(QWidget *parent, QStringList categoryList, QStringList taxList);
    ~AddDefCategoryDialog();

    static DefinedCategory getDefCategory(QWidget *parent, QStringList categoryList,
                                          QStringList taxList, bool *btnOk);

    QString getCategory();
    QString getTax();
    bool getSelfcoast();
    bool isSelling();
    bool isContracted();
    float getKoefSalary();
    float getKoefProfit();

private:
    Ui::AddDefCategoryDialog *ui;
};

#endif // ADDDEFCATEGORYDIALOG_H
