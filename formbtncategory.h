#ifndef FORMBTNCATEGORY_H
#define FORMBTNCATEGORY_H

#include <QWidget>
#include <QFrame>

namespace Ui {
class FormBtnShowCategory;
}

class FormBtnCategory :  public QFrame
{
    Q_OBJECT

public:
    explicit FormBtnCategory(int formId, QString category, QWidget *parent = nullptr);
    FormBtnCategory(int formId, QString category, QString tax, QWidget *parent = nullptr);
    ~FormBtnCategory();

    void setFormId(int newFormId);

signals:
    void signalToShowFormCategory(int formId);

private:
    Ui::FormBtnShowCategory *ui;

    int m_formId;

};

#endif // FORMBTNCATEGORY_H
