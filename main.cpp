#include "mainwindow.h"

#include <QApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
//    QStringList paths = QCoreApplication::libraryPaths();
//    paths.append(".");
//    paths.append("imageformats");
//    paths.append("platforms");
//    paths.append("sqldrivers");
//    QCoreApplication::setLibraryPaths(paths);
    QApplication a(argc, argv);

//    if (QSqlDatabase::isDriverAvailable("QSQLITE")){
//        QMessageBox::warning(0, "Warning", "QSqlite driver found!");
//        qDebug("QSqlite driver found.");
//    } else {
//        QMessageBox::warning(0, "Warning", "QSqlite driver NOT found!");
//        qFatal("QSqlite driver NOT found!");
//    }

    QSystemSemaphore semaphore("phc_report_semaphore", 1);
    semaphore.acquire();
    QSharedMemory sharedMemmory("phc_report_memmory");
    if (sharedMemmory.attach())
    {
        QMessageBox::warning(nullptr,("Предупреждение"),("Программа PhC-Report уже запущена!"));
        return 1;
    }
    else
    {
        sharedMemmory.create(1);
    }

    bool btnOkCmbBox = false;
    bool isAdmin = false;
    QStringList userType;
    userType.append("Пользователь");
    userType.append("Администратор");
    QString strUserType = QInputDialog::getItem(0,
                                           "Тип работы",
                                           "Пользователь:",
                                           userType,
                                           0,
                                           0,
                                           &btnOkCmbBox,
                                           Qt::WindowTitleHint);
    if (btnOkCmbBox) {
        if (strUserType == "Администратор") {
            bool btnOkAdminPass = false;
            QString strPassword = QInputDialog::getText(0,
                                                       "Введите пароль",
                                                       "Пароль:",
                                                       QLineEdit::Normal,
                                                       "",
                                                       &btnOkAdminPass,
                                                       Qt::WindowTitleHint);
            if (btnOkAdminPass) {
                if (strPassword == "12345")
                    isAdmin = true;
            }
        }
        else {
            isAdmin = false;
        }
    }
    else {
        return 1;
    }

    MainWindow w;
    w.setAdminStatus(isAdmin);
    w.show();

    semaphore.release();

    return a.exec();
}
