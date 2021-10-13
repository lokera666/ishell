#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
//    LoadLibrary(L"D:\\project\\ishell\\Dll1.dll");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
