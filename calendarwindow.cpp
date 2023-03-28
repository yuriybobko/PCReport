#include "calendarwindow.h"
#include "ui_calendarwindow.h"

CalendarWindow::CalendarWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalendarWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Выбор даты");
    connect(ui->BtnSelectDate, &QPushButton::clicked, [this]() {
        this->accept();
    });
    connect(ui->BtnCancel, &QPushButton::clicked, [this]() {
        this->reject();
    });
}

CalendarWindow::~CalendarWindow()
{
    delete ui;
}

QString CalendarWindow::getDate(bool *btnOk, QWidget *parent)
{
    CalendarWindow clndrWndw(parent);
    if (clndrWndw.exec() == QDialog::Accepted) {
        QString selectedDate = clndrWndw.selectDate();
        *btnOk = true;
        return selectedDate;
    }
    else {
        *btnOk = false;
        return "";
    }
}

QString CalendarWindow::selectDate()
{
    QString selectedDate = ui->CalendarWidget->selectedDate().toString("yyyy-MM-dd");
    return selectedDate;
}

void CalendarWindow::cancel()
{
    this->hide();
}
