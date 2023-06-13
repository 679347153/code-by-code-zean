#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
     MainWindow w;
     w.setWindowTitle(QStringLiteral("码呀码（在线版）-- by Zean and Linductor"));
     w.show();
     w.Get();
    return a.exec();
}
