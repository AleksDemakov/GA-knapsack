#include "mainwindow.h"
#include "solverga.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include <QChartView>
#include <QSplineSeries>
#include <QValueAxis>

using namespace QtCharts;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QVector<int> a = {1,2,3,4,5,6,7,8,9};
    QVector<int> b = {10,9,8,7,6,5,4,3,2};
    SolverGA *solver = new SolverGA(9, 20, a, b, 20, 0.4);

    QVector<int> res = solver->getAns();
    QVector< QVector<int> > fitness = solver->getFitnessScoreHistory();

    QLineSeries* series = new QtCharts::QLineSeries();

    int min_value = INT_MAX;
    int max_value = INT_MIN;

    for (int i = 0; i < fitness.size(); i++) {
        series->append( i, fitness[i][0] );

        min_value = qMin(min_value, fitness[i][0]);
        max_value = qMax(max_value, fitness[i][0]);

        qDebug() << fitness[i][0];
    }

//    series->append(0, 10);
//    //series->append(1, 20);
//    series->append( QPointF( 30, 30 ) );
//    series->append(60, 20);

    qDebug() << min_value << " " << max_value;

    QPen pen(QRgb(0xfdb157));
    pen.setWidth(3);
    series->setPen(pen);

    QChart * chart = new QChart();

    QValueAxis * axisX = new QValueAxis();
    axisX->setMax( 20 );

    QValueAxis * axisY = new QValueAxis();
    axisY->setRange( min_value - 10, max_value + 10 );

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    chart->addSeries( series );
    //chart->createDefaultAxes();
    chart->legend()->hide();



    QChartView * chartView = new QChartView(this);
    chartView->setChart( chart );
    chartView->setRenderHint(QPainter::Antialiasing);

    setCentralWidget( chartView );

    //chartView->setFixedSize(500, 500);



    qDebug() << res;

    //exit(0);


}

MainWindow::~MainWindow()
{
    delete ui;
}

