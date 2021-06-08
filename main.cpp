#include <QApplication>

#include "mainwindow.h"
#include "test/hashtest.cpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    testAll();
    MainWindow window;
    window.resize(640, 512);
    window.show();

    return a.exec();
}
