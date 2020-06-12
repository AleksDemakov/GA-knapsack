#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChart>

class SolverGA;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    double getMean( QVector<int> const &v );

    QtCharts::QChart * createGenerationsChart( int num, int limit, QVector<int> &a, QVector<int> &b, int gen, double rate, int numOfInd );

    QtCharts::QChart * createMutationRateChart( int num, int limit, QVector<int> &a, QVector<int> &b, int gen, int numOfInd );

    QtCharts::QChart * createNumOfIndChart( int num, int limit, QVector<int> &a, QVector<int> &b, int gen, double rate );

    double getFastAnswer( SolverGA * solver, int iterations );
};
#endif // MAINWINDOW_H
