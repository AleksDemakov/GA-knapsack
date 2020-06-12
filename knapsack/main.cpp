#include "mainwindow.h"
#include "solverga.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowTitle("Genetic Algorithm");

    w.show();
    return a.exec();
}
