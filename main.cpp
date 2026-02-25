#include "mainwindow.h"

#include <QApplication>
#include <QSharedMemory>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Single instance check
    QSharedMemory sharedMemory("InstagramTaskerSingleInstance");
    if (!sharedMemory.create(1)) {
        if (sharedMemory.attach()) {
            sharedMemory.detach();
        }
        QMessageBox::warning(nullptr, "Already Running", "InstagramTasker is already running.");
        return 1;
    }

    MainWindow w;
    // w.show();  // Comment out to start minimized to tray

    int result = a.exec();

    // Detach shared memory on exit
    sharedMemory.detach();

    return result;
}
