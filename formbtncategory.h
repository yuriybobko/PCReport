#ifndef FORMBTNCATEGORY_H
#define FORMBTNCATEGORY_H

#include <QWidget>

namespace Ui {
class FormBtnCategory;
}

class FormBtnCategory : public QWidget
{
    Q_OBJECT

public:
    explicit FormBtnCategory(int formId, QString category, QWidget *parent = nullptr);
    FormBtnCategory(int formId, QString category, QString tax, QWidget *parent = nullptr);
    ~FormBtnCategory();

    void setFormId(int newFormId);

signals:
    //void signalToRemoveForm(int formId);

    void signalToShowFormCategory(int formId);

private:
    Ui::FormBtnCategory *ui;

    int m_formId;

};

#endif // FORMBTNCATEGORY_H
