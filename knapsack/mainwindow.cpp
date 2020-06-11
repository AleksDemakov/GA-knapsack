#include "mainwindow.h"
#include "solverga.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QChartView>
#include <QSplineSeries>
#include <QValueAxis>

using namespace QtCharts;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    const QString fileName = QFileDialog::getOpenFileName(this);
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    QList<QString> dataList = data.split('\n');
    int limit = dataList[0].split(' ')[0].toInt();
    int num = dataList[0].split(' ')[1].toInt();

    QVector<int> a, b;
    for(QString i:dataList[1].split(' '))
        a.push_back(i.toInt());
    for(QString i:dataList[2].split(' '))
        b.push_back(i.toInt());
    int gen = dataList[3].split(' ')[0].toInt();
    double rate = dataList[3].split(' ')[1].toDouble();
    int numOfInd = dataList[3].split(' ')[2].toInt();
//    QVector<int> a = {1,2,3,4,5,6,7,8,9};
//    QVector<int> b = {10,9,8,7,6,5,4,3,2};
    SolverGA *solver = new SolverGA(num, limit, a, b, gen, rate, numOfInd);

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
//    series->append(1, 20);
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

