#include "mainwindow.h"
#include "solverga.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QVector<int> a = {1,2,3,4,5,6,7,8,9};
    QVector<int> b = {10,9,8,7,6,5,4,3,2};
    SolverGA *solver = new SolverGA(9, 20, a, b);


}

MainWindow::~MainWindow()
{
    delete ui;
}

