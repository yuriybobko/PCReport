#include "reportwindow.h"
#include "ui_reportwindow.h"

ReportWindow::ReportWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Печать отчета");

    QString imgCalendarIconPath = PICT_CALENDAR;
    QPixmap imgCalendarIcon(imgCalendarIconPath);
    ui->BtnCalendar->setIconSize(QSize(ui->BtnCalendar->size().width() - 1,
                                       ui->BtnCalendar->size().height() - 1));
    ui->BtnCalendar->setIcon(imgCalendarIcon);
    ui->BtnCalendar->setToolTip("Календарь");

    QDate curDate;
    QString curDateString = curDate.currentDate().toString("yyyy-MM-dd");
    ui->LblSelectedDate->setText(curDateString);

    QPrinterInfo PrinterInfo;
    QStringList strlist;
    strlist = PrinterInfo.availablePrinterNames();
    int Max = -100;
    if (!strlist.isEmpty()) {
        QString str1;
        for (int ii=0; ii < strlist.size(); ii++) {
            str1 = strlist.at(ii);
            Max = (Max > str1.size()) ? Max : str1.size();
            if (strlist.at(ii)!="Microsoft XPS Document Writer")
                ui->CmbBoxPrinter->addItem(strlist.at(ii));
        }
        ui->CmbBoxPrinter->setFixedWidth(6*Max);
    }

    connect(ui->BtnCalendar, &QPushButton::clicked, [this]() {
        QString selectedString = this->getDateFromCalendar();
        if (!selectedString.isEmpty())
            ui->LblSelectedDate->setText(selectedString);
    });
    connect(ui->BtnCancel, &QPushButton::clicked, [this]() {
        this->hide();
    });
    connect(ui->BtnFindReport, SIGNAL(clicked()), this, SLOT(findReportFile()));
    connect(ui->BtnOpenReport, SIGNAL(clicked()), this, SLOT(openReportFile()));
    connect(ui->BtnPrintReport, SIGNAL(clicked()), this, SLOT(printReport()));

    ui->BtnPrintReport->setFocus();
}

ReportWindow::~ReportWindow()
{
    delete ui;
}

QString ReportWindow::getDateFromCalendar()
{
    bool btnOk = false;
    QString selectedDateString = CalendarWindow::getDate(&btnOk, this);
    if (btnOk) {
        return selectedDateString;
    }
    else
        return "";
}

void ReportWindow::openReportFile()
{
    QString fileName = ui->EditReportPath->text();
    if (!fileName.isEmpty())
    {
        ShellExecuteW(nullptr, nullptr, fileName.toStdWString().c_str(), nullptr, nullptr, SW_NORMAL);
    }
    else
        QMessageBox::warning(nullptr,tr("Предупреждение"),tr("Проверьте открываемый файл"));
}

void ReportWindow::findReportFile()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Выбрать файл отчета"),
                QDir::currentPath(),
                "*.*" );
    ui->EditReportPath->setText(fileName);
    ui->BtnOpenReport->setFocus();
}

void ReportWindow::printReport()
{
    QString fileName;
    if (ui->CmbBoxPrinter->currentIndex() == 0) {
        fileName = QFileDialog::getSaveFileName(
                    this,
                    tr("Сохранить отчет"),
                    QDir::currentPath(),
                    "*.pdf" );
    }
    if (fileName.isEmpty())
        return;
    ui->EditReportPath->setText(fileName);

    QPrinter *printer;
    printer = new QPrinter;
    if (ui->CmbBoxPrinter->currentIndex() == 0) {
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setOutputFileName(fileName);
        printer->setOrientation(QPrinter::Portrait);
        printer->setPageSize(QPrinter::A4);
    }
    if (ui->CmbBoxPrinter->currentIndex() != 0) {
        QPrintDialog *printDialog = new QPrintDialog(printer, this);
        if (printDialog->exec() != QDialog::Accepted)
            return;
    }
    QPainter* painter;
    painter = new QPainter(printer);
    // Координаты в пикселях начала рабочего поля для отображения результатов анализа
    int pxbeg = 0;
    int pybeg = 0;
    //int dpxbeg = 20; // Расстояние между областями графиков по x
    int dpybeg = 20; // Расстояние между областями графиков по y
    //int dpHeight = 3; // Приращение по высоте между надписями и т.д.
    int rectHeight = 0; // Высота прямоугольника, которая дает равенство по высоте соседних столбцов
    int rectWidth = 0; // Ширина прямоугольника, которая дает равенство по ширине соседних строков

    QRect rectfull(painter->viewport());
    // Запоминаем параметры шрифтов
    QFont font;
    font.setFamily("Times New Roman");
    font.setPointSize(14);
    painter->setFont(font);

    // ---------------------------------------------------------------------
    // Печать шапки
    this->drawBanner(painter, &font, &pxbeg, &pybeg, &rectWidth);
    pybeg += dpybeg;
    // ---------------------------------------------------------------------
    // Печать полей с категориями
    QString selectedDate = ui->LblSelectedDate->text();
    QVector <ReportDefCategoryRecord> reportDefCtgryRecVector;
    QVector<DefinedCategoryRecord> defCtgryRecVector = sqlManager.selectDefCategoryRecord();
    QVector<DefinedCategory> defCtgryVector = sqlManager.selectDefCategory();
    for (int ii = 0; ii < defCtgryRecVector.size(); ii++) {
        int categoryId = defCtgryRecVector.at(ii).categoryId;
        int taxId = defCtgryRecVector.at(ii).taxId;
        int cashId = 1;
        float totalCashSum = sqlManager.selectTotalSumInPeriodByCtgryTaxCash(selectedDate, selectedDate,
                                                                             categoryId, taxId, cashId);
        cashId = 2;
        float totalNonCashSum = sqlManager.selectTotalSumInPeriodByCtgryTaxCash(selectedDate, selectedDate,
                                                                                categoryId, taxId, cashId);
        float totalSelfcoast = sqlManager.selectTotalSelfcoastInPeriod(selectedDate, selectedDate, categoryId,
                                                                       taxId);
        ReportDefCategoryRecord reportDefCtgryRec;
        reportDefCtgryRec.category = defCtgryVector.at(ii).categoryTitle;
        reportDefCtgryRec.tax = defCtgryVector.at(ii).taxTitle;
        reportDefCtgryRec.cash = totalCashSum;
        reportDefCtgryRec.nonCash = totalNonCashSum;
        reportDefCtgryRec.selfcoast = totalSelfcoast;
        reportDefCtgryRecVector.append(reportDefCtgryRec);
        QRect rectFrame(pxbeg, pybeg, rectfull.width(), 200);
        this->drawDefCategory(painter, &font, &pxbeg, &pybeg, rectFrame, &rectWidth, reportDefCtgryRec);
        if (pybeg + rectFrame.height()>rectfull.height()) {
            printer->newPage();
            pxbeg = 0;
            pybeg = 0;
            painter->drawRect(rectfull);
        }
    }
    pybeg += dpybeg;
    // ---------------------------------------------------------------------
    // Печать поля Всего
    QRect rectFrame(pxbeg, pybeg, rectfull.width(), 200);
    this->drawTotalCashAndTaxTitle(painter, &font, &pxbeg, &pybeg, rectFrame, &rectWidth, &rectHeight);
    pybeg += dpybeg;
    // ---------------------------------------------------------------------
    // печать поля З/П
    this->drawTotalSalaryField(painter, &pxbeg, &pybeg, rectFrame, &rectWidth, &rectHeight);
    // ---------------------------------------------------------------------
    // печать поля комментариев
    this->drawCommentField(painter, &font, &pxbeg, &pybeg, rectFrame);
    // ---------------------------------------------------------------------
    painter->end();
    QMessageBox::information(nullptr, tr("Информация"), tr("Файл отчет сохранен"));

    if (ui->CmbBoxPrinter->currentIndex() == 0) {
        if ((!ui->EditReportPath->text().isEmpty()) && (ui->ChkBoxOpenPDF->isChecked())) {
            QDesktopServices::openUrl(QUrl(ui->EditReportPath->text()));
            //ShellExecuteW(0,0,FileReportPath.toStdWString().c_str(),0, 0 , SW_NORMAL); // Открываем программой по умолчанию в Windows
        }
        else
            QMessageBox::warning(nullptr, tr("Предупреждение"), tr("Проверьте открываемый файл"));
    }

    delete printer;
    delete painter;
}

void ReportWindow::drawBanner(QPainter *painter, QFont *font, int *pxbeg, int *pybeg, int *rectWidth)
{
    QRect rectfull(painter->viewport());
    QString imgLogoGSFile=":app_imgs/Logo_small.jpg";
    QImage imgLogoGS(imgLogoGSFile);
    // Прямоугольник для изображения логотипа
    QRect rectImgGS(0, 0, 6 + imgLogoGS.width(), 6 + imgLogoGS.height());
    painter->setPen(QPen(Qt::black, 1,Qt::SolidLine, Qt::FlatCap));
    painter->drawRect(rectImgGS);
    painter->drawImage(3, 3, imgLogoGS);
    // Прямоугольник для заголовка
    QRect rectHeaderTxt(rectImgGS.width(), 0, (rectfull.width() - rectImgGS.width()), 50);
    font->setPointSize(14);
    font->setBold(true);
    painter->setFont(*font);
    QString txt;
    txt = "Дневной отчет";
    painter->drawRect(rectHeaderTxt);
    painter->drawText(rectHeaderTxt, Qt::AlignCenter | Qt::AlignTop | Qt::TextWordWrap, txt);
    QRect rectBannerTxt(rectImgGS.width(), rectHeaderTxt.height(),
                        (rectfull.width() - rectImgGS.width()), rectImgGS.height() - rectHeaderTxt.height());
    // Прямоугольник для даты и имен сотрудников
    font->setPointSize(12);
    font->setBold(false);
    painter->setFont(*font);
    QString dateStr = "Дата: ";
    QString selectedDate = ui->LblSelectedDate->text();
    dateStr += selectedDate;
    painter->drawRect(rectfull);
    painter->drawRect(rectBannerTxt);
    txt = dateStr + "\n";
    txt += "Фотограф-мастер: ";
    QStringList stafferNames = sqlManager.selectStafferFromDefCategoryRegRecord(selectedDate, selectedDate);
    for (auto &ii: stafferNames) {
        txt += ii;
        if (ii != stafferNames.last())
            txt += ", ";
        else
            txt += "\n";
    }
    txt += "Отчет №: ";
    txt += ui->EditReportNum->text();
    painter->drawText(rectImgGS.width(), 0,
                    rectBannerTxt.width(), rectBannerTxt.height() + 100,
                    Qt::AlignCenter | Qt::TextWordWrap, txt);
    *pxbeg = 0;
    *pybeg = rectImgGS.height();
    *rectWidth = rectImgGS.width();
}

void ReportWindow::drawDefCategory(QPainter *painter, QFont *font, int *pxbeg, int *pybeg,
                                   QRect rectFrame, int *rectWidth, ReportDefCategoryRecord reportDefCtgryRec)
{
    // Прямоугольник с названием
    QString txt = reportDefCtgryRec.category + "\n";
    txt += reportDefCtgryRec.tax;
    QRect rectDefCategoryTitle(*pxbeg, *pybeg, *rectWidth, (font->pointSize() + 9)*4);
    painter->drawRect(rectDefCategoryTitle);
    font->setItalic(true);
    painter->setFont(*font);
    painter->drawText(rectDefCategoryTitle, Qt::AlignCenter | Qt::AlignTop | Qt::TextWordWrap, txt);

    int rectHeight = 0;
    int curRowCount = 0;
    if (reportDefCtgryRec.selfcoast != 0) {
        curRowCount = 4;
    }
    else {
        curRowCount = 3;
    }
    rectHeight = rectDefCategoryTitle.height()/curRowCount;
    for (int ii = 0; ii < curRowCount; ii++) {
        QRect rectCashTitle(*pxbeg + rectDefCategoryTitle.width(), *pybeg, (font->pointSize() + 9)*5,
                            rectHeight);
        painter->drawRect(rectCashTitle);
        QRect rectCashValue(*pxbeg + rectDefCategoryTitle.width() + rectCashTitle.width(),
                            *pybeg,
                            rectFrame.width() - (rectCashTitle.x() + rectCashTitle.width()),
                            rectHeight);
        painter->drawRect(rectCashValue);
        switch (ii) {
        case 0:
        painter->drawText(rectCashTitle, Qt::AlignCenter | Qt::AlignTop | Qt::TextWordWrap, CASH_STRING);
        painter->drawText(rectCashValue, "    " + QString::number(reportDefCtgryRec.cash));
        break;
        case 1:
        painter->drawText(rectCashTitle, Qt::AlignCenter | Qt::AlignTop | Qt::TextWordWrap, NONCASH_STRING);
        painter->drawText(rectCashValue, "    " + QString::number(reportDefCtgryRec.nonCash));
        break;
        case 2:
        painter->drawText(rectCashTitle, Qt::AlignCenter | Qt::AlignTop | Qt::TextWordWrap, TOTAL_CASH_STRING);
        painter->drawText(rectCashValue, "    " + QString::number(reportDefCtgryRec.cash +
                                                                  reportDefCtgryRec.nonCash));
        break;
        case 3:
        painter->drawText(rectCashTitle, Qt::AlignCenter | Qt::AlignTop | Qt::TextWordWrap, SELFCOAST_STRING);
        painter->drawText(rectCashValue, "    " + QString::number(reportDefCtgryRec.selfcoast));
        break;
        }
        *pybeg += rectCashValue.height();
    }
    *pxbeg += 0;
}

void ReportWindow::drawTotalCashAndTaxTitle(QPainter *painter, QFont *font, int *pxbeg, int *pybeg, QRect rectFrame,
                                      int *rectWidth, int *rectHeight)
{
    int px0 = *pxbeg;
    *rectHeight = (font->pointSize() + 9);
    *rectWidth = 150;
    CashTable cashTable;
    TaxesTable taxTable;
    QVector <QString> taxesList = sqlManager.selectTitlesFromTable(taxTable.table, taxTable.title);
    QVector <QString> cashList = sqlManager.selectTitlesFromTable(cashTable.table, cashTable.title);
    QString selectedDate = ui->LblSelectedDate->text();
    QVector <float> totalTaxValue;
    totalTaxValue.resize(taxesList.size());
    for (int ii = -1; ii <= cashList.size(); ii++) {
        QRect rectField(*pxbeg, *pybeg, *rectWidth, *rectHeight);
        painter->drawRect(rectField);
        if (ii == -1) {
            font->setBold(true);
            painter->setFont(*font);
            painter->drawText(rectField, Qt::AlignCenter | Qt::AlignTop | Qt::TextWordWrap, "");
            font->setBold(false);
            painter->setFont(*font);
            *pxbeg += *rectWidth;
            this->drawTotalTaxTitleField(painter, pxbeg, pybeg, rectFrame,
                                         rectWidth, rectHeight, taxesList);
        }
        else if (ii == cashList.size()) {
            painter->drawText(rectField, Qt::AlignCenter | Qt::AlignTop | Qt::TextWordWrap, TOTAL_CASH_STRING);
            int curRectWidth = (rectFrame.width() - rectField.width())/taxesList.size();
            *pxbeg += rectField.width();
            for (int jj = 0; jj < taxesList.size(); jj++) {
                QRect rectFieldValue(*pxbeg, *pybeg, curRectWidth, *rectHeight);
                painter->drawRect(rectFieldValue);
                painter->drawText(rectFieldValue, Qt::AlignCenter | Qt::AlignTop | Qt::TextWordWrap,
                                  QString::number(totalTaxValue.at(jj)));
                *pxbeg += rectFieldValue.width();
            }
            *pybeg += *rectHeight;
        }
        else {
            painter->drawText(rectField, Qt::AlignCenter | Qt::AlignTop | Qt::TextWordWrap, cashList.at(ii));
            *pxbeg += *rectWidth;
            QVector <float> taxValue;
            int taxId = 0;
            int cashId = 0;
            cashId = sqlManager.selectIdFromTable(cashTable.table, cashTable.id, cashTable.title,
                                                  cashList.at(ii));
            for (int jj = 0; jj < taxesList.size(); jj++) {
                taxId = sqlManager.selectIdFromTable(taxTable.table, taxTable.id, taxTable.title, taxesList.at(jj));
                taxValue.append(sqlManager.selectTotalSumInPeriodByTaxCash(selectedDate, selectedDate,
                                                                           taxId, cashId));
                float value = totalTaxValue.at(jj) + taxValue.at(jj);
                totalTaxValue.replace(jj, value);
            }
            this->drawTotalTaxValueField(painter, pxbeg, pybeg, rectFrame,
                                         rectWidth, rectHeight, taxValue);
            taxValue.clear();
        }
        *pxbeg = px0;
    }
    *pxbeg = px0;
}

void ReportWindow::drawTotalTaxTitleField(QPainter *painter, int *pxbeg, int *pybeg,
                                     QRect rectFrame, int *rectWidth, int *rectHeight,
                                     QVector<QString> taxes)
{
    int px0 = *pxbeg;
    for (int jj = 0; jj < taxes.size(); jj++) {
        QRect rectField(*pxbeg, *pybeg, (rectFrame.width() - *rectWidth)/taxes.size(), *rectHeight);
        painter->drawRect(rectField);
        painter->drawText(rectField, Qt::AlignCenter | Qt::AlignTop | Qt::TextWordWrap, taxes.at(jj));
        *pxbeg += rectField.width();
    }
    *pybeg += *rectHeight;
    *pxbeg = px0;
}

void ReportWindow::drawTotalTaxValueField(QPainter *painter, int *pxbeg, int *pybeg,
                                          QRect rectFrame, int *rectWidth, int *rectHeight,
                                          QVector<float> taxValue)
{
    int px0 = *pxbeg;
    float totalSum = 0;
    for (int jj = 0; jj < taxValue.size(); jj++) {
        totalSum += taxValue.at(jj);
        QRect rectField(*pxbeg, *pybeg, (rectFrame.width() - *rectWidth)/taxValue.size(), *rectHeight);
        painter->drawRect(rectField);
        painter->drawText(rectField, Qt::AlignCenter | Qt::AlignTop | Qt::TextWordWrap,
                          QString::number(taxValue.at(jj)));
        *pxbeg += rectField.width();
    }
    *pybeg += *rectHeight;
    *pxbeg = px0;
}

void ReportWindow::drawTotalSalaryField(QPainter *painter, int *pxbeg, int *pybeg, QRect rectFrame,
                                        int *rectWidth, int *rectHeight)
{
    StaffersTable stafferTable;
    int px0 = *pxbeg;
    QString selectedDate = ui->LblSelectedDate->text();
    QStringList stafferNames = sqlManager.selectStafferFromDefCategoryRegRecord(selectedDate, selectedDate);
    for (int ii = 0; ii < stafferNames.size(); ii++) {
        QRect rectFieldTitle(*pxbeg, *pybeg, *rectWidth, *rectHeight);
        painter->drawRect(rectFieldTitle);
        painter->drawText(rectFieldTitle, Qt::AlignCenter | Qt::AlignTop | Qt::TextWordWrap, "З/П");
        *pxbeg += rectFieldTitle.width();
        QRect rectFieldStaffer(*pxbeg, *pybeg, 2*(rectFrame.width() - *rectWidth)/3, *rectHeight);
        painter->drawRect(rectFieldStaffer);
        painter->drawText(rectFieldStaffer, Qt::AlignCenter | Qt::AlignTop | Qt::TextWordWrap,
                          stafferNames.at(ii));
        *pxbeg += rectFieldStaffer.width();
        QRect rectFieldSalary(*pxbeg, *pybeg,
                              (rectFrame.width() - *pxbeg),
                              *rectHeight);
        painter->drawRect(rectFieldSalary);
        int stafferId = sqlManager.selectIdFromTable(stafferTable.table, stafferTable.id,
                                                     stafferTable.name, stafferNames.at(ii));
        SalaryRegisterRecord salaryRecord = sqlManager.selectSalaryRecord(selectedDate, stafferId);
        painter->drawText(rectFieldSalary, Qt::AlignCenter | Qt::AlignTop | Qt::TextWordWrap,
                          QString::number((salaryRecord.amount)));
        *pybeg += *rectHeight;
        *pxbeg = px0;
    }
}

void ReportWindow::drawCommentField(QPainter *painter, QFont *font, int *pxbeg, int *pybeg, QRect rectFrame)
{
    QRect rectFieldTitle(*pxbeg, *pybeg, rectFrame.width(), 30);
    painter->drawRect(rectFieldTitle);
    font->setItalic(false);
    font->setBold(true);
    painter->setFont(*font);
    painter->drawText(rectFieldTitle,  Qt::AlignCenter | Qt::AlignTop | Qt::TextWordWrap, "Комментарии:");
    *pybeg += rectFieldTitle.height();
    QRect rectFieldComment(*pxbeg, *pybeg, rectFrame.width(), 200);
    painter->drawRect(rectFieldComment);
    font->setBold(false);
    painter->setFont(*font);
    painter->drawText(rectFieldComment, ui->TextEditComments->toPlainText());

    *pybeg += rectFieldComment.height();
}
