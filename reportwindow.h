#ifndef REPORTWINDOW_H
#define REPORTWINDOW_H

#include <QDialog>
#include <QDate>
#include <QDesktopServices>
#include <QtPrintSupport/qprinter.h>
#include <QtPrintSupport/QPrinterInfo>
#include <QtPrintSupport/QPrintDialog>
#include <QPainter>
#include <QTextCodec>
#include <QMessageBox>
#include <windows.h>
#include <QFileDialog>
#include "calendarwindow.h"
#include "structures.h"
#include "sqlmanager.h"
#include "settingwindow.h"

struct ReportDefCategoryRecord {
    QString category;
    QString tax;
    float cash;
    float nonCash;
    float selfcoast;
};

extern SqlManager sqlManager;

namespace Ui {
class ReportWindow;
}

class ReportWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ReportWindow(QWidget *parent = nullptr);
    ~ReportWindow();

private slots:
    QString getDateFromCalendar();

    void openReportFile();

    void findReportFile();

    void printReport();

private:
    // Печать шапки отчета
    void drawBanner(QPainter *painter, QFont *font, int *pxbeg, int *pybeg, int *rectWidth);
    // Печать полей с категориями
    void drawDefCategory(QPainter *painter, QFont *font, int *pxbeg, int *pybeg, QRect rectFrame, int *rectWidth,
                         ReportDefCategoryRecord reportDefCtgryRec);
    // Печать полей  нал. и б/нал. в разделе Всего
    void drawTotalCashAndTaxTitle(QPainter *painter, QFont *font, int *pxbeg, int *pybeg, QRect rectFrame,
                            int *rectWidth, int *rectHeight);
    // Печать названий полей налогов
    void drawTotalTaxTitleField(QPainter *painter, QFont *font, int *pxbeg, int *pybeg, QRect rectFrame,
                                int *rectWidth, int *rectHeight, QVector<QString> taxes);
    // Печать полей с итоговой суммой по каждому из налогов
    void drawTotalTaxValueField(QPainter *painter, QFont *font, int *pxbeg, int *pybeg, QRect rectFrame,
                                int *rectWidth, int *rectHeight, QVector<float> taxValue);
    // Печать поля с З/П
    void drawTotalSalaryField(QPainter *painter, QFont *font, int *pxbeg, int *pybeg, QRect rectFrame,
                                int *rectWidth, int *rectHeight);
    // Печать поля комментариев
    void drawCommentField(QPainter *painter, QFont *font, int *pxbeg, int *pybeg, QRect rectFrame);

private:
    Ui::ReportWindow *ui;
};

#endif // REPORTWINDOW_H
