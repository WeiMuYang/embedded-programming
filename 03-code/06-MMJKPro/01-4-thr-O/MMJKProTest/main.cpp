#include "mainwindow.h"
#include <QApplication>
#include "io_ctrl.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    IOCtrl ioCtrl;
    ioCtrl.dataOut(0, 1, 1, 0.5);
    ioCtrl.dataOut(0, 2, 1, 0.5);





    w.show();

    return a.exec();
}
