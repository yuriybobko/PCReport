#ifndef CALENDARWINDOW_H
#define CALENDARWINDOW_H

#include <QDialog>

namespace Ui {
class CalendarWindow;
}

class CalendarWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CalendarWindow(QWidget *parent = nullptr);
    ~CalendarWindow();

    static QString getDate(bool *btnOk, QWidget *parent = nullptr);

private slots:
    QString selectDate();
    void cancel();

private:
    Ui::CalendarWindow *ui;
};

#endif // CALENDARWINDOW_H
