#include "mainwindow.h"

#include <QApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

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
        w.show();
    }
    semaphore.release();

    return a.exec();
}
