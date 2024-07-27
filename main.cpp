#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/SMS.jpeg"));
    MainWindow * w = new MainWindow();
    w->show();
    return a.exec();
    delete w;
    return 0;
}
