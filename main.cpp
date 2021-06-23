#include <QApplication>

#include "mainwindow.h"
#include "test/hashtest.cpp"
#include "test/xmltest.cpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    hash_test_all();
    xml_test_all();
//    MainWindow window;
//    window.resize(640, 512);
//    window.show();

    return a.exec();
}
